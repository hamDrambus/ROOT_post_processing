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
  calibration_file = "220804/results_v5/220804_calibration.dat";
  data_prefix_path = "../Data/220804/results_v1/";
  data_output_path = "220804/results_v5/";
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
/*
  exp_area.experiments.clear();
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_6169V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_5728V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_5288V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_4847V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_3966V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_3525V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_2644V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_1762V");
  exp_area.experiments.push_back("220804_X-ray_Q1.00_20kV_850V_46V_12dB_0V");

  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_6169V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_5728V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_5288V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_4847V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_3966V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_3525V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_2644V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_1762V");
  exp_area.experiments.push_back("220804_X-ray_Q0.20_20kV_850V_46V_12dB_0V");
*/
  exp_area.experiments.push_back("220804_X-ray_Q0.04_20kV_850V_46V_12dB_6169V");
  exp_area.experiments.push_back("220804_X-ray_Q0.04_20kV_850V_46V_12dB_5288V");
  exp_area.experiments.push_back("220804_X-ray_Q0.04_20kV_850V_46V_12dB_3966V");
  exp_area.experiments.push_back("220804_X-ray_Q0.04_20kV_850V_46V_12dB_2644V");
  exp_area.experiments.push_back("220804_X-ray_Q0.04_20kV_850V_46V_12dB_0V");
  
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
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_6169V"] = 6169;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_5728V"] = 5728;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_5288V"] = 5288;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_4847V"] = 4847;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_3966V"] = 3966;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_3525V"] = 3525;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_2644V"] = 2644;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_1762V"] = 1762;
  experiment_fields["220804_X-ray_Q1.00_20kV_850V_46V_12dB_0V"] = 0;

  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_6169V"] = 6169;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_5728V"] = 5728;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_5288V"] = 5288;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_4847V"] = 4847;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_3966V"] = 3966;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_3525V"] = 3525;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_2644V"] = 2644;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_1762V"] = 1762;
  experiment_fields["220804_X-ray_Q0.20_20kV_850V_46V_12dB_0V"] = 0;

  experiment_fields["220804_X-ray_Q0.04_20kV_850V_46V_12dB_6169V"] = 6169;
  experiment_fields["220804_X-ray_Q0.04_20kV_850V_46V_12dB_5288V"] = 5288;
  experiment_fields["220804_X-ray_Q0.04_20kV_850V_46V_12dB_3966V"] = 3966;
  experiment_fields["220804_X-ray_Q0.04_20kV_850V_46V_12dB_2644V"] = 2644;
  experiment_fields["220804_X-ray_Q0.04_20kV_850V_46V_12dB_0V"] = 0;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_6169V"] = 251;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_5728V"] = 246;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_5288V"] = 241;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_4847V"] = 236;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_3966V"] = 231;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_3525V"] = 226;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_2644V"] = 221;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_1762V"] = 216;
  experiment_runs["220804_X-ray_Q1.00_20kV_850V_46V_12dB_0V"] = 206;

  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_6169V"] = 201;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_5728V"] = 196;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_5288V"] = 191;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_4847V"] = 186;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_3966V"] = 178;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_3525V"] = 171;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_2644V"] = 167;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_1762V"] = 161;
  experiment_runs["220804_X-ray_Q0.20_20kV_850V_46V_12dB_0V"] = 151;

  experiment_runs["220804_X-ray_Q0.04_20kV_850V_46V_12dB_6169V"] = 141;
  experiment_runs["220804_X-ray_Q0.04_20kV_850V_46V_12dB_5288V"] = 131;
  experiment_runs["220804_X-ray_Q0.04_20kV_850V_46V_12dB_3966V"] = 121;
  experiment_runs["220804_X-ray_Q0.04_20kV_850V_46V_12dB_2644V"] = 111;
  experiment_runs["220804_X-ray_Q0.04_20kV_850V_46V_12dB_0V"] = 101;

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
