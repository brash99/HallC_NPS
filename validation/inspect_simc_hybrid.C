void inspect_simc_hybrid()
{
  TFile f("/work/hallc/gep/brash/gep4_nps_geant4/validation/simc_hybrid_10.root");
  TTree *t = dynamic_cast<TTree *>(f.Get("t_dvcs"));
  std::cout << "entries=" << (t ? t->GetEntries() : -1) << std::endl;
  if (t) {
    TIter next(t->GetListOfBranches());
    while (TObject *object = next())
      std::cout << "branch=" << object->GetName() << std::endl;
    t->Scan("evtNb:psf:GV_x:GV_y:GV_z:GSE_px:GSE_py:GSE_pz", "", "colsize=16", 10, 0);
  }
}
