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
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "231109/results_v1/231109_calibration.dat";
  data_prefix_path = "../hdda/Data/231109/results_v1/";
  data_output_path = "231109/results_v1/";
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
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_20kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_18kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_16kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_14kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_12kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_10kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_0kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V_1");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_18kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_16kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_14kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_12kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_10kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_0kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V");
  exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_20kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_18kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_16kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_14kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_12kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_10kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("231109_2ph_LArN2_X-ray_6mm_coll_0kV_850V_46V");

  // For calibration only
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_20kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_14mm_coll_0kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_16kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_10kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_8kV_850V_46V");
  // exp_area.experiments.push_back("231109_1ph_LArN2_X-ray_6mm_coll_0kV_850V_46V");

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
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V"] = 20;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V"] = 18;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V"] = 16;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V"] = 14;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V"] = 12;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V"] = 10;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V"] = 8;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V"] = 0;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_20kV_850V_46V"] = 20;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_18kV_850V_46V"] = 18;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_16kV_850V_46V"] = 16;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_14kV_850V_46V"] = 14;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_12kV_850V_46V"] = 12;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_10kV_850V_46V"] = 10;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_8kV_850V_46V"] = 8;
  experiment_fields["231109_1ph_LArN2_X-ray_14mm_coll_0kV_850V_46V"] = 0;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V_1"] = 20.01;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V"] = 20;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_18kV_850V_46V"] = 18;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_16kV_850V_46V"] = 16;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_14kV_850V_46V"] = 14;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_12kV_850V_46V"] = 12;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_10kV_850V_46V"] = 10;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_8kV_850V_46V"] = 8;
  experiment_fields["231109_1ph_LArN2_X-ray_6mm_coll_0kV_850V_46V"] = 0;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V"] = 0;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V"] = 8;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V"] = 10;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V"] = 12;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V"] = 14;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V"] = 16;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V"] = 18;
  experiment_fields["231109_2ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V"] = 20;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_20kV_850V_46V"] = 20;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_18kV_850V_46V"] = 18;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_16kV_850V_46V"] = 16;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_14kV_850V_46V"] = 14;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_12kV_850V_46V"] = 12;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_10kV_850V_46V"] = 10;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_8kV_850V_46V"] = 8;
  experiment_fields["231109_2ph_LArN2_X-ray_6mm_coll_0kV_850V_46V"] = 0;

  std::map<std::string, int> experiment_runs; //required for printing accepted/rejected events
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V"] = 471;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V"] = 459;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V"] = 448;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V"] = 437;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V"] = 425;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V"] = 414;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V"] = 402;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V"] = 384;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_20kV_850V_46V"] = 294;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_18kV_850V_46V"] = 306;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_16kV_850V_46V"] = 317;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_14kV_850V_46V"] = 328;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_12kV_850V_46V"] = 339;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_10kV_850V_46V"] = 350;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_8kV_850V_46V"] = 361;
  experiment_runs["231109_1ph_LArN2_X-ray_14mm_coll_0kV_850V_46V"] = 373;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V_1"] = 283;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V"] = 272;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_18kV_850V_46V"] = 260;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_16kV_850V_46V"] = 248;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_14kV_850V_46V"] = 237;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_12kV_850V_46V"] = 226;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_10kV_850V_46V"] = 214;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_8kV_850V_46V"] = 202;
  experiment_runs["231109_1ph_LArN2_X-ray_6mm_coll_0kV_850V_46V"] = 186;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V"] = 1;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V"] = 12;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V"] = 24;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V"] = 35;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V"] = 47;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V"] = 58;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V"] = 69;
  experiment_runs["231109_2ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V"] = 84;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_20kV_850V_46V"] = 95;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_18kV_850V_46V"] = 106;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_16kV_850V_46V"] = 118;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_14kV_850V_46V"] = 129;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_12kV_850V_46V"] = 141;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_10kV_850V_46V"] = 152;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_8kV_850V_46V"] = 163;
  experiment_runs["231109_2ph_LArN2_X-ray_6mm_coll_0kV_850V_46V"] = 174;

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

  channel_info<std::vector<Npe_in_ranges_data>> g_Npe_data; // channel->experiment
  std::deque<int> chs_for_Npe = post_processor->MPPC_channels;
  for (int i = 1; i != 9; ++i)
    chs_for_Npe.push_back(i);
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
