{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "211007/results_v5/211007_calibration.dat";
  data_prefix_path = "../Data/211007/results_v1/";
  data_output_path = "211007/results_v5/";
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
  //exp_area.experiments.push_back("211007_Pu_20.0kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_18.8kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_17.0kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_15.3kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_14.4kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_13.6kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_12.7kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_11.8kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_11.0kV_800V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211007_Pu_19.9kV_800V_46V_12dB_2550V");
  exp_area.experiments.push_back("211007_Pu_19.8kV_800V_46V_12dB_2500V");
  exp_area.experiments.push_back("211007_Pu_19.9kV_800V_46V_12dB_2600V");

  PMT_V.clear();
  MPPC_V.clear();
  channel_info<dB_info> atten0;
  atten0.push(0, dB_info(12)); //decibells, not ratio
  atten0.push(1, dB_info(12));
  atten0.push(2, dB_info(12));
  atten0.push(3, dB_info(12));
  atten0.push(4, dB_info(12));
  dBs.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 800;
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 46;
  }

  experiment_fields.clear();
  experiment_fields["211007_Pu_20.0kV_800V_46V_12dB_2650V"] = 16.8;
  experiment_fields["211007_Pu_18.8kV_800V_46V_12dB_2650V"] = 15.6;
  experiment_fields["211007_Pu_17.0kV_800V_46V_12dB_2650V"] = 13.8;
  experiment_fields["211007_Pu_15.3kV_800V_46V_12dB_2650V"] = 12.1;
  experiment_fields["211007_Pu_14.4kV_800V_46V_12dB_2650V"] = 11.2;
  experiment_fields["211007_Pu_13.6kV_800V_46V_12dB_2650V"] = 10.4;
  experiment_fields["211007_Pu_12.7kV_800V_46V_12dB_2650V"] = 9.5;
  experiment_fields["211007_Pu_11.8kV_800V_46V_12dB_2650V"] = 8.6;
  experiment_fields["211007_Pu_11.0kV_800V_46V_12dB_2650V"] = 7.8;
  experiment_fields["211007_Pu_19.9kV_800V_46V_12dB_2550V"] = 16.8;
  experiment_fields["211007_Pu_19.8kV_800V_46V_12dB_2500V"] = 16.8;
  experiment_fields["211007_Pu_19.9kV_800V_46V_12dB_2600V"] = 16.8;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["211007_Pu_20.0kV_800V_46V_12dB_2650V"] = 1;
  experiment_runs["211007_Pu_18.8kV_800V_46V_12dB_2650V"] = 27;
  experiment_runs["211007_Pu_17.0kV_800V_46V_12dB_2650V"] = 53;
  experiment_runs["211007_Pu_15.3kV_800V_46V_12dB_2650V"] = 79;
  experiment_runs["211007_Pu_14.4kV_800V_46V_12dB_2650V"] = 105;
  experiment_runs["211007_Pu_13.6kV_800V_46V_12dB_2650V"] = 131;
  experiment_runs["211007_Pu_12.7kV_800V_46V_12dB_2650V"] = 162;
  experiment_runs["211007_Pu_11.8kV_800V_46V_12dB_2650V"] = 193;
  experiment_runs["211007_Pu_11.0kV_800V_46V_12dB_2650V"] = 224;
  experiment_runs["211007_Pu_19.9kV_800V_46V_12dB_2550V"] = 255;
  experiment_runs["211007_Pu_19.8kV_800V_46V_12dB_2500V"] = 281;
  experiment_runs["211007_Pu_19.9kV_800V_46V_12dB_2600V"] = 307;

  if (areas_to_draw.empty())
    areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(0, 4); //slow PMTs no. sum, 1-4
  areas_to_draw.back().channels.push_pair(5, 8); //fast PMTs no. 1-4
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(0, 8);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
  Initialize();
  //Following variables are required for analysis_history.cpp
  //ch 1 and ch 5 are absent - no PMT#1 signal
  std::vector<int> calib_channels;// = {2, 3, 4, 6, 7, 8};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
