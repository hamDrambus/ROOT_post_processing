{
  gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001; //To shut up minuit output during failed(?) fitting
  threads_number = 10;
  name_scheme_version = name_scheme_v2;
  trigger_version = TriggerVersion::trigger_v2;
  //From global parameters:
  calibration_file = "220915/results_v7/220915_calibration.dat";
  data_prefix_path = "../Data/220915/results_v1/";
  data_output_path = "220915/results_v7/";
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
  /*exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_15.4kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_14.6kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_13.8kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_12.3kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_10.8kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_10.0kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_9.2kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_8.4kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_7.7kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_0.75atm_16.7mm_6.9kV_850V_46V");*/

  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_20kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_19kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_17kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_16kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_14kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_13kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_12kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_11kV_850V_46V");
  exp_area.experiments.push_back("220915_Pu_1.00atm_16.0mm_10kV_850V_46V");

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
  experiment_fields["220915_Pu_0.75atm_16.7mm_15.4kV_850V_46V"] = 15.4;
  experiment_fields["220915_Pu_0.75atm_16.7mm_14.6kV_850V_46V"] = 14.6;
  experiment_fields["220915_Pu_0.75atm_16.7mm_13.8kV_850V_46V"] = 13.8;
  experiment_fields["220915_Pu_0.75atm_16.7mm_12.3kV_850V_46V"] = 12.3;
  experiment_fields["220915_Pu_0.75atm_16.7mm_10.8kV_850V_46V"] = 10.8;
  experiment_fields["220915_Pu_0.75atm_16.7mm_10.0kV_850V_46V"] = 10.0;
  experiment_fields["220915_Pu_0.75atm_16.7mm_9.2kV_850V_46V"] = 9.2;
  experiment_fields["220915_Pu_0.75atm_16.7mm_8.4kV_850V_46V"] = 8.4;
  experiment_fields["220915_Pu_0.75atm_16.7mm_7.7kV_850V_46V"] = 7.7;
  experiment_fields["220915_Pu_0.75atm_16.7mm_6.9kV_850V_46V"] = 6.9;

  experiment_fields["220915_Pu_1.00atm_16.0mm_20kV_850V_46V"] = 20;
  experiment_fields["220915_Pu_1.00atm_16.0mm_19kV_850V_46V"] = 19;
  experiment_fields["220915_Pu_1.00atm_16.0mm_17kV_850V_46V"] = 17;
  experiment_fields["220915_Pu_1.00atm_16.0mm_16kV_850V_46V"] = 16;
  experiment_fields["220915_Pu_1.00atm_16.0mm_14kV_850V_46V"] = 14;
  experiment_fields["220915_Pu_1.00atm_16.0mm_13kV_850V_46V"] = 13;
  experiment_fields["220915_Pu_1.00atm_16.0mm_12kV_850V_46V"] = 12;
  experiment_fields["220915_Pu_1.00atm_16.0mm_11kV_850V_46V"] = 11;
  experiment_fields["220915_Pu_1.00atm_16.0mm_10kV_850V_46V"] = 10;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["220915_Pu_0.75atm_16.7mm_15.4kV_850V_46V"] = 1;
  experiment_runs["220915_Pu_0.75atm_16.7mm_14.6kV_850V_46V"] = 42;
  experiment_runs["220915_Pu_0.75atm_16.7mm_13.8kV_850V_46V"] = 83;
  experiment_runs["220915_Pu_0.75atm_16.7mm_12.3kV_850V_46V"] = 124;
  experiment_runs["220915_Pu_0.75atm_16.7mm_10.8kV_850V_46V"] = 165;
  experiment_runs["220915_Pu_0.75atm_16.7mm_10.0kV_850V_46V"] = 216;
  experiment_runs["220915_Pu_0.75atm_16.7mm_9.2kV_850V_46V"] = 267;
  experiment_runs["220915_Pu_0.75atm_16.7mm_8.4kV_850V_46V"] = 318;
  experiment_runs["220915_Pu_0.75atm_16.7mm_7.7kV_850V_46V"] = 359;
  experiment_runs["220915_Pu_0.75atm_16.7mm_6.9kV_850V_46V"] = 400;

  experiment_runs["220915_Pu_1.00atm_16.0mm_20kV_850V_46V"] = 699;
  experiment_runs["220915_Pu_1.00atm_16.0mm_19kV_850V_46V"] = 668;
  experiment_runs["220915_Pu_1.00atm_16.0mm_17kV_850V_46V"] = 637;
  experiment_runs["220915_Pu_1.00atm_16.0mm_16kV_850V_46V"] = 608;
  experiment_runs["220915_Pu_1.00atm_16.0mm_14kV_850V_46V"] = 577;
  experiment_runs["220915_Pu_1.00atm_16.0mm_13kV_850V_46V"] = 535;
  experiment_runs["220915_Pu_1.00atm_16.0mm_12kV_850V_46V"] = 504;
  experiment_runs["220915_Pu_1.00atm_16.0mm_11kV_850V_46V"] = 473;
  experiment_runs["220915_Pu_1.00atm_16.0mm_10kV_850V_46V"] = 441;

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
