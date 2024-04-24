{
  struct Npe_in_ranges_data {
    std::pair<double, double> t_pre_trigger;
    std::pair<double, double> t_S1;
    std::pair<double, double> t_S2;
    double Npe_pre_trigger;
    double Npe_S1;
    double Npe_S2;
  };

  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gStyle->SetOptStat(1110);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "240418/results_v1/240418_calibration.dat";
  data_prefix_path = "../hdda/Data/240418/results_v1/";
  data_output_path = "240418/results_v1/";
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
  // For full analysis
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_0kV_850V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_8kV_700V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_10kV_700V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_12kV_700V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_14kV_700V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_16kV_700V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_bkg_18kV_850V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_0kV_850V_46V_1");
  // exp_area.experiments.push_back("240418_ArN2_1ph_X-ray_14mm_coll_18kV_850V_46V");
  // No PMTs - must be processed separately:
  exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_0kV_0V_46V");

  // For calibration only
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_0kV_850V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_8kV_700V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_X-ray_14mm_coll_14kV_700V_46V");
  // exp_area.experiments.push_back("240418_ArN2_2ph_bkg_18kV_850V_46V");
  // exp_area.experiments.push_back("240418_ArN2_1ph_X-ray_14mm_coll_18kV_850V_46V");

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
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 46;
    if (i->find("_850V_") != std::string::npos)
  	  PMT_V[*i] = 850;
    else if (i->find("_800V_") != std::string::npos)
      PMT_V[*i] = 800;
    else if (i->find("_650V_") != std::string::npos)
      PMT_V[*i] = 650;
    else if (i->find("_700V_") != std::string::npos)
      PMT_V[*i] = 700;
    else
      PMT_V[*i] = 0;
  }

  experiment_fields.clear();
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_0kV_850V_46V"] = 0;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_8kV_850V_46V"] = 8;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_8kV_700V_46V"] = 8.01;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_10kV_700V_46V"] = 10;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_12kV_700V_46V"] = 12;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_14kV_700V_46V"] = 14;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_16kV_700V_46V"] = 16;
  experiment_fields["240418_ArN2_2ph_bkg_18kV_850V_46V"] = 18;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_0kV_0V_46V"] = 0.01;
  experiment_fields["240418_ArN2_2ph_X-ray_14mm_coll_0kV_850V_46V_1"] = 0.02;
  experiment_fields["240418_ArN2_1ph_X-ray_14mm_coll_18kV_850V_46V"] = 18.01;

  std::map<std::string, int> experiment_runs; //required for printing accepted/rejected events
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_0kV_850V_46V"] = 1;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_8kV_850V_46V"] = 42;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_8kV_700V_46V"] = 63;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_10kV_700V_46V"] = 79;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_12kV_700V_46V"] = 96;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_14kV_700V_46V"] = 113;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_16kV_700V_46V"] = 125;
  experiment_runs["240418_ArN2_2ph_bkg_18kV_850V_46V"] = 152;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_0kV_0V_46V"] = 188;
  experiment_runs["240418_ArN2_2ph_X-ray_14mm_coll_0kV_850V_46V_1"] = 209;
  experiment_runs["240418_ArN2_1ph_X-ray_14mm_coll_18kV_850V_46V"] = 288;

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
  // std::vector<int> calib_channels = {1, 2, 3, 4, 6, 5, 7, 8};
  std::vector<int> calib_channels = {1, 3, 4, 5, 7, 8};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }

  channel_info<std::vector<Npe_in_ranges_data>> g_Npe_data; // channel->experiment
  std::deque<int> chs_for_Npe = post_processor->MPPC_channels;
  for (int i = 1; i != 9; ++i) {
    if (i == 2 || i == 6)
      continue;
    chs_for_Npe.push_back(i);
  }
  for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
		int chan = chs_for_Npe[ich];
    std::vector<Npe_in_ranges_data> vec;
    for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
      Npe_in_ranges_data data;
      data.t_pre_trigger = std::pair<double, double> (0,0);
      data.t_S1 = std::pair<double, double> (0,0);
      data.t_S2 = std::pair<double, double> (0,0);
      data.Npe_pre_trigger = 0;
      data.Npe_S1 = 0;
      data.Npe_S2 = 0;
      vec.push_back(data);
    }
    g_Npe_data.push(chan, vec);
	}
}
