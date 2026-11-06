#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TTree.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
struct EventRecord {
  double edep[1080];
  double weight;
  double px;
  double py;
  double pz;
  double vx;
  double vy;
  double vz;
  int entered;
};

void Bind(TTree* tree, EventRecord& event) {
  tree->SetBranchAddress("edep", event.edep);
  tree->SetBranchAddress("psf", &event.weight);
  tree->SetBranchAddress("GSE_px", &event.px);
  tree->SetBranchAddress("GSE_py", &event.py);
  tree->SetBranchAddress("GSE_pz", &event.pz);
  tree->SetBranchAddress("GV_x", &event.vx);
  tree->SetBranchAddress("GV_y", &event.vy);
  tree->SetBranchAddress("GV_z", &event.vz);
  tree->SetBranchAddress("nps_entry", &event.entered);
}

double TotalDeposit(const EventRecord& event) {
  double total = 0.;
  for (int block = 0; block < 1080; ++block) total += event.edep[block];
  return total;
}
}

void analyze_simc_material_closure() {
  const std::string base = "/work/hallc/gep/brash/gep4_nps_geant4/validation/";
  TFile off_file((base + "simc_hybrid_target_off_78.root").c_str());
  TFile on_file((base + "simc_hybrid_target_on_78.root").c_str());
  auto* off_tree = static_cast<TTree*>(off_file.Get("t_dvcs"));
  auto* on_tree = static_cast<TTree*>(on_file.Get("t_dvcs"));
  if (!off_tree || !on_tree || off_tree->GetEntries() != 78 ||
      on_tree->GetEntries() != 78) {
    std::cerr << "Expected paired 78-entry trees" << std::endl;
    return;
  }

  EventRecord off{}, on{};
  Bind(off_tree, off);
  Bind(on_tree, on);
  TH1D h_off("h_off", "NPS raw response;#Sigma E_{dep}/E_{primary};Events", 50, 0., 1.05);
  TH1D h_on("h_on", "NPS raw response;#Sigma E_{dep}/E_{primary};Events", 50, 0., 1.05);
  TH1D h_delta("h_delta", "Effect of duplicated target;E_{dep}^{target on}-E_{dep}^{vacuum} (MeV);Events", 60, -6000., 6000.);
  TH1D h_vertex("h_vertex", "Deposit change versus vertex;SIMC vertex z (cm);#Delta E_{dep} (MeV)", 30, -15., 15.);

  std::ofstream csv(base + "simc_material_closure_78.csv");
  csv << "event,weight,primary_E_MeV,vertex_z_cm,entry_target_off,entry_target_on,"
         "edep_target_off_MeV,edep_target_on_MeV,delta_edep_MeV\n";
  csv << std::setprecision(15);

  double sumw = 0.;
  double off_fraction = 0., on_fraction = 0.;
  double off_low = 0., on_low = 0.;
  double off_entry = 0., on_entry = 0.;
  double delta_sum = 0., delta_sum2 = 0.;
  int changed_entry = 0;
  double max_truth_difference = 0.;
  const double electron_mass_GeV = 0.510999e-3;
  for (Long64_t i = 0; i < 78; ++i) {
    off_tree->GetEntry(i);
    on_tree->GetEntry(i);
    const double energy = 1000. * std::sqrt(off.px * off.px + off.py * off.py +
                                             off.pz * off.pz +
                                             electron_mass_GeV * electron_mass_GeV);
    const double off_edep = TotalDeposit(off);
    const double on_edep = TotalDeposit(on);
    const double delta = on_edep - off_edep;
    max_truth_difference = std::max(max_truth_difference,
      std::fabs(off.weight - on.weight) + std::fabs(off.px - on.px) +
      std::fabs(off.py - on.py) + std::fabs(off.pz - on.pz) +
      std::fabs(off.vx - on.vx) + std::fabs(off.vy - on.vy) +
      std::fabs(off.vz - on.vz));
    sumw += off.weight;
    off_fraction += off.weight * off_edep / energy;
    on_fraction += off.weight * on_edep / energy;
    off_low += off.weight * (off_edep < 0.9 * energy);
    on_low += off.weight * (on_edep < 0.9 * energy);
    off_entry += off.weight * (off.entered != 0);
    on_entry += off.weight * (on.entered != 0);
    delta_sum += delta;
    delta_sum2 += delta * delta;
    changed_entry += (off.entered != on.entered);
    h_off.Fill(off_edep / energy);
    h_on.Fill(on_edep / energy);
    h_delta.Fill(delta);
    h_vertex.Fill(off.vz, delta);
    csv << i << ',' << off.weight << ',' << energy << ',' << off.vz << ','
        << off.entered << ',' << on.entered << ',' << off_edep << ',' << on_edep
        << ',' << delta << '\n';
  }
  csv.close();

  const double delta_mean = delta_sum / 78.;
  const double delta_rms = std::sqrt(delta_sum2 / 78. - delta_mean * delta_mean);
  std::cout << std::setprecision(10)
            << "entries=78\n"
            << "max_truth_difference=" << max_truth_difference << "\n"
            << "weighted_entry_target_off=" << off_entry / sumw << "\n"
            << "weighted_entry_target_on=" << on_entry / sumw << "\n"
            << "weighted_below90_target_off=" << off_low / sumw << "\n"
            << "weighted_below90_target_on=" << on_low / sumw << "\n"
            << "weighted_mean_edep_fraction_target_off=" << off_fraction / sumw << "\n"
            << "weighted_mean_edep_fraction_target_on=" << on_fraction / sumw << "\n"
            << "changed_entry_events=" << changed_entry << "\n"
            << "mean_delta_edep_MeV=" << delta_mean << "\n"
            << "rms_delta_edep_MeV=" << delta_rms << std::endl;

  gStyle->SetOptStat(0);
  h_off.SetLineColor(kBlue + 1);
  h_on.SetLineColor(kRed + 1);
  h_off.SetLineWidth(2);
  h_on.SetLineWidth(2);
  h_delta.SetLineColor(kBlack);
  h_delta.SetLineWidth(2);
  h_vertex.SetMarkerStyle(20);
  h_vertex.SetMarkerSize(0.8);
  TCanvas canvas("canvas", "SIMC-GEANT4 material closure", 1500, 500);
  canvas.Divide(3, 1);
  canvas.cd(1);
  h_off.Draw("hist");
  h_on.Draw("hist same");
  TLegend legend(0.14, 0.72, 0.58, 0.88);
  legend.SetBorderSize(0);
  legend.AddEntry(&h_off, "Hybrid boundary (vacuum)", "l");
  legend.AddEntry(&h_on, "Target/cell duplicated", "l");
  legend.Draw();
  canvas.cd(2);
  h_delta.Draw("hist");
  canvas.cd(3);
  h_vertex.Draw("P");
  canvas.SaveAs((base + "simc_material_closure_78.png").c_str());
  canvas.SaveAs((base + "simc_material_closure_78.pdf").c_str());
}
