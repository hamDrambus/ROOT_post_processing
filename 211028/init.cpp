{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "211028/results_v5/211028_calibration.dat";
  data_prefix_path = "../Data/211028/results_v1/";
  data_output_path = "211028/results_v5/";
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
  exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_2650V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_2500V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_2300V");
  exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_2083V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_1875V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_1666V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_1438V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_1250V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_833V");
  //exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_292V");
  /*exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_208V");
  exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_187V");
  exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_167V");
  exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_125V");
  exp_area.experiments.push_back("211028_Pu_7.8kV_850V_46V_12dB_0V");*/

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
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_2650V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_2500V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_2300V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_2083V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_1875V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_1666V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_1438V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_1250V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_833V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_292V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_208V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_187V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_167V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_125V"] = 7.8;
  experiment_fields["211028_Pu_7.8kV_850V_46V_12dB_0V"] = 7.8;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_2650V"] = 399;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_2500V"] = 378;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_2300V"] = 354;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_2083V"] = 328;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_1875V"] = 302;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_1666V"] = 271;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_1438V"] = 240;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_1250V"] = 209;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_833V"] = 188;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_292V"] = 157;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_208V"] = 126;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_187V"] = 95;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_167V"] = 64;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_125V"] = 33;
  experiment_runs["211028_Pu_7.8kV_850V_46V_12dB_0V"] = 1;

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
