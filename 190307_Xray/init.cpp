{
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "190307_Xray/results_v1/190307_calibration.dat";
  data_prefix_path = "../Data/190307/results_v1/";
  data_output_path = "190307_Xray/results_v1/";
  DATA_MPPC_VERSION = "SiPM";
  DATA_PMT_VERSION = "PMT";
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
  exp_area.experiments.push_back("190307_X-ray_20kV_650V_46V_coll_6mm");
  exp_area.experiments.push_back("190307_X-ray_18kV_650V_46V_coll_6mm");
  exp_area.experiments.push_back("190307_X-ray_16kV_650V_46V_coll_6mm");
  exp_area.experiments.push_back("190307_X-ray_14kV_650V_46V_coll_6mm");
  exp_area.experiments.push_back("190307_X-ray_12kV_650V_46V_coll_6mm");
  exp_area.experiments.push_back("190307_X-ray_10kV_650V_46V_coll_6mm");
  exp_area.experiments.push_back("190307_X-ray_9kV_650V_46V_coll_6mm");

  PMT_V.clear();
  MPPC_V.clear();
  channel_info<dB_info> atten0;
  atten0.push(0, dB_info(12)); //decibells, not ratio
  atten0.push(1, dB_info(12));
  dBs.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 650;
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 46;
  }

  experiment_fields.clear();
  experiment_fields["190307_X-ray_20kV_650V_46V_coll_6mm"] = 20;
  experiment_fields["190307_X-ray_18kV_650V_46V_coll_6mm"] = 18;
  experiment_fields["190307_X-ray_16kV_650V_46V_coll_6mm"] = 16;
  experiment_fields["190307_X-ray_14kV_650V_46V_coll_6mm"] = 14;
  experiment_fields["190307_X-ray_12kV_650V_46V_coll_6mm"] = 12;
  experiment_fields["190307_X-ray_10kV_650V_46V_coll_6mm"] = 10;
  experiment_fields["190307_X-ray_9kV_650V_46V_coll_6mm"] = 9;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["190307_X-ray_20kV_650V_46V_coll_6mm"] = 1;
  experiment_runs["190307_X-ray_18kV_650V_46V_coll_6mm"] = 12;
  experiment_runs["190307_X-ray_16kV_650V_46V_coll_6mm"] = 17;
  experiment_runs["190307_X-ray_14kV_650V_46V_coll_6mm"] = 23;
  experiment_runs["190307_X-ray_12kV_650V_46V_coll_6mm"] = 29;
  experiment_runs["190307_X-ray_10kV_650V_46V_coll_6mm"] = 35;
  experiment_runs["190307_X-ray_9kV_650V_46V_coll_6mm"] = 41;


  if (areas_to_draw.empty())
	areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(0, 1); //slow PMTs no. sum of 4, sum of 3
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(0, 1);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
  Initialize();
  //Following variables are required for analysis_history.cpp
  std::vector<int> calib_channels;
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
