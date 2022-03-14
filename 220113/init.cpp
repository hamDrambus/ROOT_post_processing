{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "220113/results_v5/220113_calibration.dat";
  data_prefix_path = "../Data/220113/results_v1/";
  data_output_path = "220113/results_v5/";
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
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2873V");
  //exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2513V");
  /*exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2155V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1797V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1616V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1330V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1065V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_852V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_745V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_639V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_533V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_425V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_318V");
  exp_area.experiments.push_back("220113_Pu_1.5atm_11.1kV_850V_46V_12dB_0V");

  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2350V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2260V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2009V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1757V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1506V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1257V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1130V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_930V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_746V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_595V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_521V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_447V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_373V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_298V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_178V");
  exp_area.experiments.push_back("220113_Pu_1.0atm_7.8kV_850V_46V_12dB_0V");*/

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
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2873V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2513V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2155V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1797V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1616V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1330V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1065V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_852V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_745V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_639V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_533V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_425V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_318V"] = 11.1;
  experiment_fields["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_0V"] = 11.1;

  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2350V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2260V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2009V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1757V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1506V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1257V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1130V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_930V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_746V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_595V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_521V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_447V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_373V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_298V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_178V"] = 7.8;
  experiment_fields["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_0V"] = 7.8;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2873V"] = 766;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2513V"] = 750;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_2155V"] = 729;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1797V"] = 708;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1616V"] = 687;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1330V"] = 656;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_1065V"] = 625;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_852V"] = 594;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_745V"] = 563;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_639V"] = 532;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_533V"] = 501;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_425V"] = 470;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_318V"] = 439;
  experiment_runs["220113_Pu_1.5atm_11.1kV_850V_46V_12dB_0V"] = 407;

  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2350V"] = 391;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2260V"] = 375;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_2009V"] = 359;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1757V"] = 343;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1506V"] = 322;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1257V"] = 301;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_1130V"] = 280;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_930V"] = 249;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_746V"] = 218;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_595V"] = 187;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_521V"] = 156;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_447V"] = 125;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_373V"] = 94;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_298V"] = 63;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_178V"] = 32;
  experiment_runs["220113_Pu_1.0atm_7.8kV_850V_46V_12dB_0V"] = 1;

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
