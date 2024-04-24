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
  calibration_file = "240215/results_v1/240215_calibration.dat";
  data_prefix_path = "../hdda/Data/240215/results_v1/";
  data_output_path = "240215/results_v1/";
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
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_20kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_18kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_16kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_14kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_12kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_10kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_8kV_850V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_0kV_850V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_20kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_18kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_16kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_14kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_12kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_10kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_8kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_0kV_650V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_20kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_18kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_16kV_800V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_14kV_850V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_12kV_850V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_10kV_850V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_8kV_850V_46V");
  exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_0kV_850V_46V");

  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_20kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_18kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_16kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_14kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_12kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_10kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_14mm_coll_0kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_20kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_18kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_16kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_14kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_12kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_10kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_1ph_X-ray_6mm_coll_0kV_850V_46V");

  // For calibration only
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_16kV_800V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_20kV_800V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_12kV_800V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_10kV_800V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_20kV_800V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_14kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_6mm_coll_0kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_8kV_850V_46V");
  // exp_area.experiments.push_back("240215_Ar_2ph_X-ray_14mm_coll_filt3_0kV_850V_46V");

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
    else
      PMT_V[*i] = 0;
  }

  experiment_fields.clear();
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_20kV_800V_46V"] = 20;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_18kV_800V_46V"] = 18;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_16kV_800V_46V"] = 16;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_14kV_800V_46V"] = 14;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_12kV_800V_46V"] = 12;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_10kV_800V_46V"] = 10;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_8kV_850V_46V"] = 8;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_filt3_0kV_850V_46V"] = 0;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_20kV_650V_46V"] = 20;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_18kV_650V_46V"] = 18;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_16kV_650V_46V"] = 16;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_14kV_650V_46V"] = 14;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_12kV_650V_46V"] = 12;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_10kV_650V_46V"] = 10;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_8kV_650V_46V"] = 8;
  experiment_fields["240215_Ar_2ph_X-ray_14mm_coll_0kV_650V_46V"] = 0;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_20kV_800V_46V"] = 20;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_18kV_800V_46V"] = 18;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_16kV_800V_46V"] = 16;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_14kV_850V_46V"] = 14;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_12kV_850V_46V"] = 12;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_10kV_850V_46V"] = 10;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_8kV_850V_46V"] = 8;
  experiment_fields["240215_Ar_2ph_X-ray_6mm_coll_0kV_850V_46V"] = 0;

  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_20kV_850V_46V"] = 20;
  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_18kV_850V_46V"] = 18;
  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_16kV_850V_46V"] = 16;
  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_14kV_850V_46V"] = 14;
  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_12kV_850V_46V"] = 12;
  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_10kV_850V_46V"] = 10;
  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_8kV_850V_46V"] = 8;
  experiment_fields["240215_Ar_1ph_X-ray_14mm_coll_0kV_850V_46V"] = 0;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_20kV_850V_46V"] = 20;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_18kV_850V_46V"] = 18;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_16kV_850V_46V"] = 16;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_14kV_850V_46V"] = 14;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_12kV_850V_46V"] = 12;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_10kV_850V_46V"] = 10;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_8kV_850V_46V"] = 8;
  experiment_fields["240215_Ar_1ph_X-ray_6mm_coll_0kV_850V_46V"] = 0;

  std::map<std::string, int> experiment_runs; //required for printing accepted/rejected events
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_20kV_800V_46V"] = 331;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_18kV_800V_46V"] = 308;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_16kV_800V_46V"] = 285;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_14kV_800V_46V"] = 263;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_12kV_800V_46V"] = 242;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_10kV_800V_46V"] = 220;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_8kV_850V_46V"] = 199;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_filt3_0kV_850V_46V"] = 158;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_20kV_650V_46V"] = 147;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_18kV_650V_46V"] = 136;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_16kV_650V_46V"] = 125;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_14kV_650V_46V"] = 110;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_12kV_650V_46V"] = 89;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_10kV_650V_46V"] = 68;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_8kV_650V_46V"] = 47;
  experiment_runs["240215_Ar_2ph_X-ray_14mm_coll_0kV_650V_46V"] = 1;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_20kV_800V_46V"] = 353;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_18kV_800V_46V"] = 376;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_16kV_800V_46V"] = 397;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_14kV_850V_46V"] = 418;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_12kV_850V_46V"] = 440;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_10kV_850V_46V"] = 461;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_8kV_850V_46V"] = 482;
  experiment_runs["240215_Ar_2ph_X-ray_6mm_coll_0kV_850V_46V"] = 506;

  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_20kV_850V_46V"] = 872; 
  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_18kV_850V_46V"] = 851; 
  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_16kV_850V_46V"] = 835; 
  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_14kV_850V_46V"] = 819; 
  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_12kV_850V_46V"] = 801; 
  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_10kV_850V_46V"] = 785; 
  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_8kV_850V_46V"] = 768; 
  experiment_runs["240215_Ar_1ph_X-ray_14mm_coll_0kV_850V_46V"] = 737; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_20kV_850V_46V"] = 714; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_18kV_850V_46V"] = 693; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_16kV_850V_46V"] = 672; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_14kV_850V_46V"] = 651; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_12kV_850V_46V"] = 630; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_10kV_850V_46V"] = 609; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_8kV_850V_46V"] = 588; 
  experiment_runs["240215_Ar_1ph_X-ray_6mm_coll_0kV_850V_46V"] = 557; 

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
