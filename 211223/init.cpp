{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "211223/results_v5/211223_calibration.dat";
  data_prefix_path = "../Data/211223/results_v1/";
  data_output_path = "211223/results_v5/";
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
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_3592V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_3233V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2874V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2514V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2155V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1796V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1615V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1332V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1065V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_852V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_744V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_639V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_532V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_426V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_318V");
  exp_area.experiments.push_back("211223_Pu_1.5atm_11.1kV_850V_46V_12dB_0V");

  //exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2513V");
  //exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2260V");
  //exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2009V");
  //exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1758V");
  //exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1507V");
  /*exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1256V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1130V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_931V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_745V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_596V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_522V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_448V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_372V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_298V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_222V");
  exp_area.experiments.push_back("211223_Pu_1.0atm_7.8kV_850V_46V_12dB_0V");*/

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
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_3592V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_3233V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2874V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2514V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2155V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1796V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1615V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1332V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1065V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_852V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_744V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_639V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_532V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_426V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_318V"] = 11.1;
  experiment_fields["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_0V"] = 11.1;

  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2513V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2260V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2009V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1758V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1507V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1256V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1130V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_931V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_745V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_596V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_522V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_448V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_372V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_298V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_222V"] = 7.8;
  experiment_fields["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_0V"] = 7.8;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_3592V"] = 804;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_3233V"] = 788;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2874V"] = 772;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2514V"] = 756;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_2155V"] = 735;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1796V"] = 714;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1615V"] = 693;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1332V"] = 662;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_1065V"] = 630;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_852V"] = 598;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_744V"] = 568;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_639V"] = 537;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_532V"] = 506;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_426V"] = 475;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_318V"] = 444;
  experiment_runs["211223_Pu_1.5atm_11.1kV_850V_46V_12dB_0V"] = 412;

  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2513V"] = 396;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2260V"] = 378;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_2009V"] = 362;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1758V"] = 346;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1507V"] = 325;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1256V"] = 303;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_1130V"] = 282;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_931V"] = 251;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_745V"] = 220;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_596V"] = 189;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_522V"] = 158;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_448V"] = 127;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_372V"] = 95;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_298V"] = 64;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_222V"] = 32;
  experiment_runs["211223_Pu_1.0atm_7.8kV_850V_46V_12dB_0V"] = 1;

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
