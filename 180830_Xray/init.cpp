{
  //!Only SiPM-matrix is processed
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "180830_Xray/results_v1/180830_Xray_calibration.dat";
  data_prefix_path = "../Data/180830/results_v1/";
  data_output_path = "180830_Xray/results_v1/";
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
  exp_area.experiments.push_back("180830_Xray_0kV_600V_12bB_49V_1ph_35mm_coll");

  PMT_V.clear();
  MPPC_V.clear();
  channel_info<dB_info> atten0; //12 what dB were used for PMTs
  dBs.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 600;
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 49;
  }

  experiment_fields.clear();
  experiment_fields["180830_Xray_0kV_600V_12bB_49V_1ph_35mm_coll"] = 0;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["180830_Xray_0kV_600V_12bB_49V_1ph_35mm_coll"] = 23;

  if (areas_to_draw.empty())
	areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
  Initialize();
  //Following variables are required for analysis_history.cpp
  std::vector<int> calib_channels;
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
