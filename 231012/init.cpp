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
  calibration_file = "231012/results_v1/231012_calibration.dat";
  data_prefix_path = "../hdda/Data/231012/results_v1/";
  data_output_path = "231012/results_v1/";
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
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_923V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_877V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_831V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_785V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_739V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_693V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_646V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_600V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_554V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_508V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_462V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_416V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_369V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_323V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_277V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_231V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_185V_850V_46V_no_coll");
  exp_area.experiments.push_back("231012_X-ray_S2_LArN2_10kV_0V_850V_46V_no_coll");

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
  experiment_fields["231012_X-ray_S2_LArN2_10kV_923V_850V_46V_no_coll"] = 923;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_877V_850V_46V_no_coll"] = 877;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_831V_850V_46V_no_coll"] = 831;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_785V_850V_46V_no_coll"] = 785;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_739V_850V_46V_no_coll"] = 739;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_693V_850V_46V_no_coll"] = 693;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_646V_850V_46V_no_coll"] = 646;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_600V_850V_46V_no_coll"] = 600;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_554V_850V_46V_no_coll"] = 554;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_508V_850V_46V_no_coll"] = 508;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_462V_850V_46V_no_coll"] = 462;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_416V_850V_46V_no_coll"] = 416;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_369V_850V_46V_no_coll"] = 369;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_323V_850V_46V_no_coll"] = 323;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_277V_850V_46V_no_coll"] = 277;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_231V_850V_46V_no_coll"] = 231;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_185V_850V_46V_no_coll"] = 185;
  experiment_fields["231012_X-ray_S2_LArN2_10kV_0V_850V_46V_no_coll"] = 0;

  std::map<std::string, int> experiment_runs; //required for printing accepted/rejected events
  experiment_runs["231012_X-ray_S2_LArN2_10kV_923V_850V_46V_no_coll"] = 370;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_877V_850V_46V_no_coll"] = 352;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_831V_850V_46V_no_coll"] = 334;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_785V_850V_46V_no_coll"] = 317;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_739V_850V_46V_no_coll"] = 301;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_693V_850V_46V_no_coll"] = 284;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_646V_850V_46V_no_coll"] = 268;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_600V_850V_46V_no_coll"] = 252;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_554V_850V_46V_no_coll"] = 236;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_508V_850V_46V_no_coll"] = 220;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_462V_850V_46V_no_coll"] = 203;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_416V_850V_46V_no_coll"] = 187;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_369V_850V_46V_no_coll"] = 170;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_323V_850V_46V_no_coll"] = 154;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_277V_850V_46V_no_coll"] = 133;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_231V_850V_46V_no_coll"] = 112;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_185V_850V_46V_no_coll"] = 91;
  experiment_runs["231012_X-ray_S2_LArN2_10kV_0V_850V_46V_no_coll"] = 48;

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
