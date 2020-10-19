{
 name_scheme_version = name_scheme_v2;
 int aggressive_PMT_cuts = 0; //0 - no cuts at all, only PMT15 (original analysis of 08.2019). 1 - very low A-S exclusion, also exclude high A low S. Calibration as well as Npe and signal forms must be plotted from scratch for every agrressiveness level.
  //From global parameters:
  if (name_scheme_version == name_scheme_v1) {
    if (aggressive_PMT_cuts==1)
      calibration_file = "190404/results/190404_calibration_aggr=1.dat";
    else
      calibration_file = "190404/results/190404_calibration.dat";
    data_prefix_path = "../Data/190404/results/";
    data_output_path = "190404/results/";
    DATA_MPPC_VERSION = "MPPCs_v1";
    DATA_PMT_VERSION = "PMT_v1";
  }
  if (name_scheme_version == name_scheme_v2) {
    data_prefix_path = "../Data/190404/results_v2/";
    calibration_file = "190404/results_v6/190404_calibration.dat";
    data_output_path = "190404/results_v6/";
    DATA_MPPC_VERSION = "SiPM";
    DATA_PMT_VERSION = "PMT";
  }
  std::cout<<"data_prefix_path: \""<<data_prefix_path<<"\""<<std::endl;
  //Initialize data for utility functions:
	std::ifstream str;
	str.open(Vdrift_data_fname);
	if (!str.is_open()) {
	  std::cerr << "Error: Failed to open file with e drift velocity data \"" << Vdrift_data_fname << "\"!" << std::endl;
	} else {
    Vdrift.read(str);
    Vdrift.setOrder(1);
    Vdrift.setNused(2);
    Vdrift.use_leftmost(true); //how to behave if Td is outside of data Tds range
    Vdrift.use_rightmost(true);
  }
	str.close();

  exp_area.experiments.clear();
  exp_area.experiments.push_back("190404_Cd_20kV_850V_46V_th250mV");
  exp_area.experiments.push_back("190404_Cd_20kV_850V_46V_th250mV_0");
  exp_area.experiments.push_back("190404_Cd_18kV_850V_46V_th230mV");
  exp_area.experiments.push_back("190404_Cd_16kV_850V_46V_th210mV");
  exp_area.experiments.push_back("190404_Cd_14kV_850V_46V_th200mV");
  exp_area.experiments.push_back("190404_Cd_12kV_850V_46V_th160mV");
  exp_area.experiments.push_back("190404_Cd_10kV_850V_46V_th150mV");
  exp_area.experiments.push_back("190404_Cd_8kV_850V_46V_th140mV");

  exp_area.experiments.push_back("190404_Cd_20kV_850V_48V_th230mV");
  exp_area.experiments.push_back("190404_Cd_18kV_850V_48V_th210mV");
  exp_area.experiments.push_back("190404_Cd_16kV_850V_48V_th200mV");
  exp_area.experiments.push_back("190404_Cd_14kV_850V_48V_th160mV");
  exp_area.experiments.push_back("190404_Cd_12kV_850V_48V_th150mV");
  exp_area.experiments.push_back("190404_Cd_10kV_850V_48V_th150mV");
  exp_area.experiments.push_back("190404_Cd_8kV_850V_48V_th140mV");

  PMT_V.clear();
  MPPC_V.clear();
  channel_info<dB_info> atten0;
  atten0.push(12, dB_info(12)); //decibells, not ratio
  atten0.push(13, dB_info(12));
  atten0.push(14, dB_info(12));
  atten0.push(15, dB_info(12));
  dBs.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.begin()+8; i != i_end_; ++i) {
	  PMT_V[*i] = 850;
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 46;
  }
  for (auto i = exp_area.experiments.begin()+8, i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 850;
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 48;
  }

  experiment_fields.clear();
  experiment_fields["190404_Cd_20kV_850V_46V_th250mV_0"] = 20;
  experiment_fields["190404_Cd_20kV_850V_46V_th250mV"] = 20;
  experiment_fields["190404_Cd_18kV_850V_46V_th230mV"] = 18;
  experiment_fields["190404_Cd_16kV_850V_46V_th210mV"] = 16;
  experiment_fields["190404_Cd_14kV_850V_46V_th200mV"] = 14;
  experiment_fields["190404_Cd_12kV_850V_46V_th160mV"] = 12;
  experiment_fields["190404_Cd_10kV_850V_46V_th150mV"] = 10;
  experiment_fields["190404_Cd_8kV_850V_46V_th140mV"] = 8;

  experiment_fields["190404_Cd_20kV_850V_48V_th230mV"] = 20;
  experiment_fields["190404_Cd_18kV_850V_48V_th210mV"] = 18;
  experiment_fields["190404_Cd_16kV_850V_48V_th200mV"] = 16;
  experiment_fields["190404_Cd_14kV_850V_48V_th160mV"] = 14;
  experiment_fields["190404_Cd_12kV_850V_48V_th150mV"] = 12;
  experiment_fields["190404_Cd_10kV_850V_48V_th150mV"] = 10;
  experiment_fields["190404_Cd_8kV_850V_48V_th140mV"] = 8;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["190404_Cd_20kV_850V_46V_th250mV_0"] = 1;
  experiment_runs["190404_Cd_20kV_850V_46V_th250mV"] = 33;
  experiment_runs["190404_Cd_18kV_850V_46V_th230mV"] = 64;
  experiment_runs["190404_Cd_16kV_850V_46V_th210mV"] = 96;
  experiment_runs["190404_Cd_14kV_850V_46V_th200mV"] = 127;
  experiment_runs["190404_Cd_12kV_850V_46V_th160mV"] = 149;
  experiment_runs["190404_Cd_10kV_850V_46V_th150mV"] = 172;
  experiment_runs["190404_Cd_8kV_850V_46V_th140mV"] = 199;

  experiment_runs["190404_Cd_20kV_850V_48V_th230mV"] = 229;
  experiment_runs["190404_Cd_18kV_850V_48V_th210mV"] = 252;
  experiment_runs["190404_Cd_16kV_850V_48V_th200mV"] = 273;
  experiment_runs["190404_Cd_14kV_850V_48V_th160mV"] = 295;
  experiment_runs["190404_Cd_12kV_850V_48V_th150mV"] = 318;
  experiment_runs["190404_Cd_10kV_850V_48V_th150mV"] = 339;
  experiment_runs["190404_Cd_8kV_850V_48V_th140mV"] = 362;


  if (areas_to_draw.empty())
	areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(8, 11); //fast PMTs no. 1-4
  areas_to_draw.back().channels.push_pair(12, 15); //slow PMTs no. 1-4
  areas_to_draw.back().channels.push_pair(16, 16); //Sum of slow PMTs after shaping amplifier
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(8, 16);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
  Initialize();
  //Following variables are required for analysis_history.cpp
  std::vector<int> calib_channels = {15, 14, 13, 12, 11, 10, 9, 8};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
