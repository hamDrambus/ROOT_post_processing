{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "220203/results_v5/220203_calibration.dat";
  data_prefix_path = "../Data/220203/results_v1/";
  data_output_path = "220203/results_v5/";
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
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_6180V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_5993V");
  /*exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_5738V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_5297V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_4856V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_4413V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_3972V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_3531V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_3090V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_2648V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_2206V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_1765V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_0V_6180V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_0V_5736V");
  exp_area.experiments.push_back("220203_X-ray_20kV_850V_46V_12dB_0V");*/

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
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_6180V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_5993V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_5738V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_5297V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_4856V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_4413V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_3972V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_3531V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_3090V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_2648V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_2206V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_1765V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_0V_6180V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_0V_5736V"] = 20.0;
  experiment_fields["220203_X-ray_20kV_850V_46V_12dB_0V"] = 20.0;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_6180V"] = 564;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_5993V"] = 571;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_5738V"] = 548;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_5297V"] = 531;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_4856V"] = 515;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_4413V"] = 499;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_3972V"] = 483;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_3531V"] = 467;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_3090V"] = 451;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_2648V"] = 435;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_2206V"] = 419;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_1765V"] = 403;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_0V_6180V"] = 387;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_0V_5736V"] = 371;
  experiment_runs["220203_X-ray_20kV_850V_46V_12dB_0V"] = 197;

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
  std::vector<int> calib_channels = {1, 2, 3, 4, 5, 6, 7, 8};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
