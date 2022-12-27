{
  struct SiPM_Npe_data {
    std::pair<double, double> t_pre_trigger;
    std::pair<double, double> t_S1;
    std::pair<double, double> t_S2;
    double Npe_pre_trigger;
    double Npe_pre_S1;
    double Npe_S1;
    double Npe_S2;
    double Npe_S2_only;
  };

  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "221215/results_v1/221215_calibration.dat";
  data_prefix_path = "../Data/221215/results_v1/";
  data_output_path = "221215/results_v1/";
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
  exp_area.experiments.push_back("221215_X-ray_S2_LAr_20kV_831V_850V_46V_14mm_coll_filt3");
  exp_area.experiments.push_back("221215_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3");
  exp_area.experiments.push_back("221215_X-ray_S2_LAr_20kV_739V_850V_46V_14mm_coll_filt3");
  exp_area.experiments.push_back("221215_X-ray_S2_LAr_20kV_693V_850V_46V_14mm_coll_filt3");
  exp_area.experiments.push_back("221215_X-ray_S2_LAr_20kV_646V_850V_46V_14mm_coll_filt3");
  exp_area.experiments.push_back("221215_X-ray_S2_LAr_20kV_0V_850V_46V_14mm_coll_filt3");

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
  experiment_fields["221215_X-ray_S2_LAr_20kV_831V_850V_46V_14mm_coll_filt3"] = 831;
  experiment_fields["221215_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3"] = 785;
  experiment_fields["221215_X-ray_S2_LAr_20kV_739V_850V_46V_14mm_coll_filt3"] = 739;
  experiment_fields["221215_X-ray_S2_LAr_20kV_693V_850V_46V_14mm_coll_filt3"] = 693;
  experiment_fields["221215_X-ray_S2_LAr_20kV_646V_850V_46V_14mm_coll_filt3"] = 646;
  experiment_fields["221215_X-ray_S2_LAr_20kV_0V_850V_46V_14mm_coll_filt3"] = 0;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["221215_X-ray_S2_LAr_20kV_831V_850V_46V_14mm_coll_filt3"] = 83;
  experiment_runs["221215_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3"] = 71;
  experiment_runs["221215_X-ray_S2_LAr_20kV_739V_850V_46V_14mm_coll_filt3"] = 55;
  experiment_runs["221215_X-ray_S2_LAr_20kV_693V_850V_46V_14mm_coll_filt3"] = 43;
  experiment_runs["221215_X-ray_S2_LAr_20kV_646V_850V_46V_14mm_coll_filt3"] = 32;
  experiment_runs["221215_X-ray_S2_LAr_20kV_0V_850V_46V_14mm_coll_filt3"] = 1;

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

  channel_info<std::vector<SiPM_Npe_data>> gSiPM_Npe_data; // channel->experiment
  for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
    std::vector<SiPM_Npe_data> vec;
    for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
      SiPM_Npe_data data;
      data.t_pre_trigger = std::pair<double, double> (0,0);
      data.t_S1 = std::pair<double, double> (0,0);
      data.t_S2 = std::pair<double, double> (0,0);
      data.Npe_pre_trigger = 0;
      data.Npe_pre_S1 = 0;
      data.Npe_S1 = 0;
      data.Npe_S2 = 0;
      data.Npe_S2_only = 0;
      vec.push_back(data);
    }
    gSiPM_Npe_data.push(chan, vec);
	}
}
