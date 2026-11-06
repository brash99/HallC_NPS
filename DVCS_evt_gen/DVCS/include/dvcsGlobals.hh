
#include <string>

struct dvcsGlobals
{
  static int    run_number;
  static double Ebeam; // Beam energy in GeV's
  static double HMS_angle; // central angle of HMS window
  static double HMS_momentum; // HMS momentum central value
  static double Calo_distance; // Calorimeter distance
  static double Calo_angle;    // Calorimeter angle
  static int    target_type;   // target type p or deuteron
  static int    target_gen_proc_type; // target type for determining process (like DVCS on p = 0, n = 1, or d = 2)
  static double target_density; // density of target
  static double target_offset; // shift of the target center from the SC center
  static double target_length; // target lengt
  static bool   hit_HMS_CALO_flag;

  // Primary-generator mode. 0 preserves the historical DVCS generator;
  // 1 fires one deterministic mono-energetic electron toward NPS.
  static int    primary_mode;
  static double mono_energy;   // total electron energy [GeV]
  static double mono_angle;    // Hall polar angle [rad]
  static double mono_vertex_z; // Hall z vertex [cm]
  static double mono_local_x;  // requested NPS-face local x [mm]
  static double mono_local_y;  // requested NPS-face local y [mm]
  static double mono_nps_distance; // explicit NPS face distance [cm], <=0 uses DB

  // SIMC-to-GEANT4 hybrid input mode.
  static std::string simc_event_file;
  static double simc_nps_angle;    // Hall polar angle [rad]
  static double simc_nps_distance; // NPS face distance [cm]
  static bool simc_keep_target_material; // diagnostic double-counting test only

  static bool   SM_field_flag; //Sweeping Magnet field ON||OFF
  static double SM_field_str; //Sweeping Magnet field strength

  static double SM_angle; //Sweeper Magnet angle
};
