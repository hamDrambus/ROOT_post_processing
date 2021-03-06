{
  //From global parameters:
  data_prefix_path = "../Data/190307/results/";
  calibration_file = "190307/results/190307_calibration.dat";
  data_output_path = "190307/results/";
  DATA_MPPC_VERSION = "MPPCs_v1";
  DATA_PMT_VERSION = "PMT_v1";

  OUTPUT_MPPCS_PICS = "MPPCs_v1/MPPCs_";
  OUTPUT_PMT_PICS = "PMT_v1/PMT_";
  OUTPUT_MPPCS = "MPPC_";

  exp_area.experiments.clear();
  exp_area.experiments.push_back("190307_Cd_20kV_850V_46V_coll6mm_th430mV");
  exp_area.experiments.push_back("190307_Cd_18kV_850V_46V_coll6mm_th430mV");
  exp_area.experiments.push_back("190307_Cd_16kV_850V_46V_coll6mm_th400mV");
  exp_area.experiments.push_back("190307_Cd_14kV_850V_46V_coll6mm_th350mV");
  exp_area.experiments.push_back("190307_Bkg_20kV_850V_46V_coll6mm_th430mV");
  
  PMT_V.clear();
  MPPC_V.clear();
  dBs.clear();
  channel_info<dB_info> atten0;
  atten0.push(0, dB_info(12)); //decibells, not ratio
  atten0.push(1, dB_info(12));
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 850;
	  dBs[*i] = atten0;
	  MPPC_V[*i] = 46;
  }

  experiment_fields.clear();
  experiment_fields["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = 20;
  experiment_fields["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = 20;
  experiment_fields["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = 18;
  experiment_fields["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = 16;
  experiment_fields["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = 14;

  std::map<std::string, int> experiment_runs; //required for printing accpeted/rejected events
  experiment_runs["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = 1;
  experiment_runs["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = 118;
  experiment_runs["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = 32;
  experiment_runs["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = 63;
  experiment_runs["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = 94;
 
  if (areas_to_draw.empty())
	areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(0, 7);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(0, 7);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
  Initialize();
  //Following variables are required for analysis_history.cpp
  std::vector<int> calib_channels = {7, 0, 1};
}
