{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "210909/results_v5/210909_calibration.dat";
  data_prefix_path = "../Data/210909/results_v1/";
  data_output_path = "210909/results_v5/";
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
  exp_area.experiments.push_back("210909_Pu_13.8kV_850V_46V_12dB_2588V");
  exp_area.experiments.push_back("210909_Pu_13.8kV_850V_46V_12dB_2465V");
  exp_area.experiments.push_back("210909_Pu_13.8kV_850V_46V_12dB_2300V");
  exp_area.experiments.push_back("210909_Pu_17.3kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_15.6kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_13.8kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_12.1kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_11.2kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_10.4kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_9.5kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_8.6kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_7.8kV_850V_46V_12dB");
  exp_area.experiments.push_back("210909_Pu_6.9kV_850V_46V_12dB");

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
	  PMT_V[*i] = 850;
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 46;
  }

  experiment_fields.clear();
  experiment_fields["210909_Pu_13.8kV_850V_46V_12dB_2588V"] = 13.8;
  experiment_fields["210909_Pu_13.8kV_850V_46V_12dB_2465V"] = 13.8;
  experiment_fields["210909_Pu_13.8kV_850V_46V_12dB_2300V"] = 13.8;
  experiment_fields["210909_Pu_17.3kV_850V_46V_12dB"] = 17.3;
  experiment_fields["210909_Pu_15.6kV_850V_46V_12dB"] = 15.6;
  experiment_fields["210909_Pu_13.8kV_850V_46V_12dB"] = 13.8;
  experiment_fields["210909_Pu_12.1kV_850V_46V_12dB"] = 12.1;
  experiment_fields["210909_Pu_11.2kV_850V_46V_12dB"] = 11.2;
  experiment_fields["210909_Pu_10.4kV_850V_46V_12dB"] = 10.4;
  experiment_fields["210909_Pu_9.5kV_850V_46V_12dB"] = 9.5;
  experiment_fields["210909_Pu_8.6kV_850V_46V_12dB"] = 8.6;
  experiment_fields["210909_Pu_7.8kV_850V_46V_12dB"] = 7.8;
  experiment_fields["210909_Pu_6.9kV_850V_46V_12dB"] = 6.9;


  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["210909_Pu_13.8kV_850V_46V_12dB_2588V"] = 465;
  experiment_runs["210909_Pu_13.8kV_850V_46V_12dB_2465V"] = 434;
  experiment_runs["210909_Pu_13.8kV_850V_46V_12dB_2300V"] = 383;
  experiment_runs["210909_Pu_17.3kV_850V_46V_12dB"] = 352;
  experiment_runs["210909_Pu_15.6kV_850V_46V_12dB"] = 321;
  experiment_runs["210909_Pu_13.8kV_850V_46V_12dB"] = 290;
  experiment_runs["210909_Pu_12.1kV_850V_46V_12dB"] = 254;
  experiment_runs["210909_Pu_11.2kV_850V_46V_12dB"] = 218;
  experiment_runs["210909_Pu_10.4kV_850V_46V_12dB"] = 177;
  experiment_runs["210909_Pu_9.5kV_850V_46V_12dB"] = 135;
  experiment_runs["210909_Pu_8.6kV_850V_46V_12dB"] = 93;
  experiment_runs["210909_Pu_7.8kV_850V_46V_12dB"] = 47;
  experiment_runs["210909_Pu_6.9kV_850V_46V_12dB"] = 1;

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
  std::vector<int> calib_channels = {2, 3, 4, 6, 7, 8};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
