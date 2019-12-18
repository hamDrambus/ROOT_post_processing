{
  name_scheme_version = name_scheme_v2;
  //From global parameters:
  if (name_scheme_version == name_scheme_v1) {
    data_prefix_path = "../Data/180705/results/";
    calibration_file = "180705/results/180705_calibration.dat";
    data_output_path = "180705/results/";
    DATA_MPPC_VERSION = "MPPCs_v1";
    DATA_PMT_VERSION = "PMT_v1";
  }
  if (name_scheme_version == name_scheme_v2) {
    data_prefix_path = "../Data/180705/results_v2/";
    calibration_file = "180705/results_v4/180705_calibration.dat";
    data_output_path = "180705/results_v4/";
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
  exp_area.experiments.push_back("180705_Cd_20kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_18kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_16kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_14kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_13kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_12kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_11kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_10kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_9kV_800V_0bB_48V");
  exp_area.experiments.push_back("180705_Cd_8kV_800V_0bB_48V");
  
  PMT_V.clear();
  MPPC_V.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 800;
	  MPPC_V[*i] = 48;
  }
  {
  channel_info<dB_info> atten0;
  atten0.push(0, dB_info(12)); //decibells, not ratio
  channel_info<dB_info> atten1;
  atten1.push(0, dB_info(6)); //decibells, not ratio
  dBs.clear();
  dBs["180705_Cd_20kV_800V_12bB_48V"] = atten0;
  dBs["180705_Cd_18kV_800V_12bB_48V"] = atten0;
  dBs["180705_Cd_16kV_800V_12bB_48V"] = atten0;
  dBs["180705_Cd_14kV_800V_12bB_48V"] = atten0;
  dBs["180705_Cd_13kV_800V_12bB_48V"] = atten0;
  dBs["180705_Cd_12kV_800V_6bB_48V"] = atten0;
  dBs["180705_Cd_12kV_800V_6bB_48V"] = atten1;
  dBs["180705_Cd_11kV_800V_6bB_48V"] = atten1;
  dBs["180705_Cd_10kV_800V_6bB_48V"] = atten1;
  }

  experiment_fields.clear();
  experiment_fields["180705_Cd_20kV_800V_12bB_48V"] = 20;
  experiment_fields["180705_Cd_18kV_800V_12bB_48V"] = 18;
  experiment_fields["180705_Cd_16kV_800V_12bB_48V"] = 16;
  experiment_fields["180705_Cd_14kV_800V_12bB_48V"] = 14;
  experiment_fields["180705_Cd_13kV_800V_12bB_48V"] = 13;
  experiment_fields["180705_Cd_12kV_800V_6bB_48V"] = 12;
  experiment_fields["180705_Cd_11kV_800V_6bB_48V"] = 11;
  experiment_fields["180705_Cd_10kV_800V_6bB_48V"] = 10;
  experiment_fields["180705_Cd_9kV_800V_0bB_48V"] = 9;
  experiment_fields["180705_Cd_8kV_800V_0bB_48V"] = 8;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["180705_Cd_20kV_800V_12bB_48V"] = 360;
  experiment_runs["180705_Cd_18kV_800V_12bB_48V"] = 349;
  experiment_runs["180705_Cd_16kV_800V_12bB_48V"] = 338;
  experiment_runs["180705_Cd_14kV_800V_12bB_48V"] = 327;
  experiment_runs["180705_Cd_13kV_800V_12bB_48V"] = 316;
  experiment_runs["180705_Cd_12kV_800V_6bB_48V"] = 305;
  experiment_runs["180705_Cd_11kV_800V_6bB_48V"] = 248;
  experiment_runs["180705_Cd_10kV_800V_6bB_48V"] = 259;
  experiment_runs["180705_Cd_9kV_800V_0bB_48V"] = 270;
  experiment_runs["180705_Cd_8kV_800V_0bB_48V"] = 281;
 
  if (areas_to_draw.empty())
	areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(0, 1); //slow PMTs #2+3+4 (with WLS) and slow PMT#1 (no WLS)
  areas_to_draw.back().channels.push_pair(8, 12); //fast PMTs no. 1-4 and sum
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(0, 16);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
  Initialize();
  //Following variables are required for analysis_history.cpp
  std::vector<int> calib_channels = {0, 1, 8, 9, 10, 11, 12};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
