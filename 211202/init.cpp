{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "211202/results_v5/211202_calibration.dat";
  data_prefix_path = "../Data/211202/results_v1/";
  data_output_path = "211202/results_v5/";
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
  /*exp_area.experiments.push_back("211202_Pu_16kV_850V_46V_12dB_16mm");
  exp_area.experiments.push_back("211202_Pu_15kV_850V_46V_12dB_16mm");
  exp_area.experiments.push_back("211202_Pu_15kV_850V_46V_12dB_18mm");
  exp_area.experiments.push_back("211202_Pu_14kV_850V_46V_12dB_18mm");
  exp_area.experiments.push_back("211202_Pu_13kV_850V_46V_12dB_18mm");
  exp_area.experiments.push_back("211202_Pu_12kV_850V_46V_12dB_18mm");
  exp_area.experiments.push_back("211202_Pu_11kV_850V_46V_12dB_18mm");
  exp_area.experiments.push_back("211202_Pu_10kV_850V_46V_12dB_18mm");
  exp_area.experiments.push_back("211202_Pu_9kV_850V_46V_12dB_18mm");
  exp_area.experiments.push_back("211202_Pu_8kV_850V_46V_12dB_18mm");*/

  exp_area.experiments.push_back("211202_Pu_17.3kV_850V_46V_12dB_10mm");
  //exp_area.experiments.push_back("211202_Pu_15.6kV_850V_46V_12dB_10mm");
  exp_area.experiments.push_back("211202_Pu_13.8kV_850V_46V_12dB_10mm");
  //exp_area.experiments.push_back("211202_Pu_12.1kV_850V_46V_12dB_10mm");
  exp_area.experiments.push_back("211202_Pu_11.2kV_850V_46V_12dB_10mm");
  //exp_area.experiments.push_back("211202_Pu_10.4kV_850V_46V_12dB_10mm");
  //exp_area.experiments.push_back("211202_Pu_9.5kV_850V_46V_12dB_10mm");
  //exp_area.experiments.push_back("211202_Pu_8.6kV_850V_46V_12dB_10mm");
  //exp_area.experiments.push_back("211202_Pu_7.8kV_850V_46V_12dB_10mm");

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
  experiment_fields["211202_Pu_16kV_850V_46V_12dB_16mm"] = 16;
  experiment_fields["211202_Pu_15kV_850V_46V_12dB_16mm"] = 15;
  experiment_fields["211202_Pu_15kV_850V_46V_12dB_18mm"] = 15;
  experiment_fields["211202_Pu_14kV_850V_46V_12dB_18mm"] = 14;
  experiment_fields["211202_Pu_13kV_850V_46V_12dB_18mm"] = 13;
  experiment_fields["211202_Pu_12kV_850V_46V_12dB_18mm"] = 12;
  experiment_fields["211202_Pu_11kV_850V_46V_12dB_18mm"] = 11;
  experiment_fields["211202_Pu_10kV_850V_46V_12dB_18mm"] = 10;
  experiment_fields["211202_Pu_9kV_850V_46V_12dB_18mm"] = 9;
  experiment_fields["211202_Pu_8kV_850V_46V_12dB_18mm"] = 8;

  experiment_fields["211202_Pu_17.3kV_850V_46V_12dB_10mm"] = 17.3;
  experiment_fields["211202_Pu_15.6kV_850V_46V_12dB_10mm"] = 15.6;
  experiment_fields["211202_Pu_13.8kV_850V_46V_12dB_10mm"] = 13.8;
  experiment_fields["211202_Pu_12.1kV_850V_46V_12dB_10mm"] = 12.1;
  experiment_fields["211202_Pu_11.2kV_850V_46V_12dB_10mm"] = 11.2;
  experiment_fields["211202_Pu_10.4kV_850V_46V_12dB_10mm"] = 10.4;
  experiment_fields["211202_Pu_9.5kV_850V_46V_12dB_10mm"] = 9.5;
  experiment_fields["211202_Pu_8.6kV_850V_46V_12dB_10mm"] = 8.6;
  experiment_fields["211202_Pu_7.8kV_850V_46V_12dB_10mm"] = 7.8;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["211202_Pu_16kV_850V_46V_12dB_16mm"] = 328;
  experiment_runs["211202_Pu_15kV_850V_46V_12dB_16mm"] = 297;
  experiment_runs["211202_Pu_15kV_850V_46V_12dB_18mm"] = 293;
  experiment_runs["211202_Pu_14kV_850V_46V_12dB_18mm"] = 257;
  experiment_runs["211202_Pu_13kV_850V_46V_12dB_18mm"] = 216;
  experiment_runs["211202_Pu_12kV_850V_46V_12dB_18mm"] = 175;
  experiment_runs["211202_Pu_11kV_850V_46V_12dB_18mm"] = 134;
  experiment_runs["211202_Pu_10kV_850V_46V_12dB_18mm"] = 93;
  experiment_runs["211202_Pu_9kV_850V_46V_12dB_18mm"] = 52;
  experiment_runs["211202_Pu_8kV_850V_46V_12dB_18mm"] = 1;

  experiment_runs["211202_Pu_17.3kV_850V_46V_12dB_10mm"] = 359;
  experiment_runs["211202_Pu_15.6kV_850V_46V_12dB_10mm"] = 394;
  experiment_runs["211202_Pu_13.8kV_850V_46V_12dB_10mm"] = 426;
  experiment_runs["211202_Pu_12.1kV_850V_46V_12dB_10mm"] = 457;
  experiment_runs["211202_Pu_11.2kV_850V_46V_12dB_10mm"] = 496;
  experiment_runs["211202_Pu_10.4kV_850V_46V_12dB_10mm"] = 538;
  experiment_runs["211202_Pu_9.5kV_850V_46V_12dB_10mm"] = 586;
  experiment_runs["211202_Pu_8.6kV_850V_46V_12dB_10mm"] = 632;
  experiment_runs["211202_Pu_7.8kV_850V_46V_12dB_10mm"] = 683;

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
  std::vector<int> calib_channels = {1, 2, 3, 4, 6, 5, 7, 8};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
