{
  //From global parameters:
  data_prefix_path = "../Data/180705/results/";
  calibration_file = "180705/results/180705_calibration.dat";
  data_output_path = "180705/results/";
  DATA_MPPC_VERSION = "MPPCs_v1";
  DATA_PMT_VERSION = "PMT_v1";

  OUTPUT_MPPCS_PICS = "MPPCs_v1/MPPCs_";
  OUTPUT_PMT_PICS = "PMT_v1/PMT_";
  OUTPUT_MPPCS = "MPPC_";

  exp_area.experiments.clear();
  exp_area.experiments.push_back("180705_Cd_20kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_18kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_16kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_14kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_13kV_800V_12bB_48V");
  exp_area.experiments.push_back("180705_Cd_12kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_11kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_10kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_9kV_800V_0bB_48V");
  exp_area.experiments.push_back("180705_Cd_8kV_800V_0bB_48V");
  
  PMT_V.clear();
  MPPC_V.clear();
  PMT_dB.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 800;
	  PMT_dB[*i] = 1.0; //ratio, not actual dB. 6dB = 2.0, 12dB = 3.98
	  MPPC_V[*i] = 48;
  }
  PMT_dB["180705_Cd_20kV_800V_12bB_48V"] = 3.98; //ratio, not actual dB. 6dB = 2.0, 12dB = 3.98
  PMT_dB["180705_Cd_18kV_800V_12bB_48V"] = 3.98;
  PMT_dB["180705_Cd_16kV_800V_12bB_48V"] = 3.98;
  PMT_dB["180705_Cd_14kV_800V_12bB_48V"] = 3.98;
  PMT_dB["180705_Cd_12kV_800V_6bB_48V"] = 3.98;
  PMT_dB["180705_Cd_12kV_800V_6bB_48V"] = 2;
  PMT_dB["180705_Cd_11kV_800V_6bB_48V"] = 2;
  PMT_dB["180705_Cd_10kV_800V_6bB_48V"] = 2;

  experiment_fields.clear();
  experiment_fields["180705_Cd_20kV_800V_12bB_48V"] = 20;
  experiment_fields["180705_Cd_18kV_800V_12bB_48V"] = 18;
  experiment_fields["180705_Cd_16kV_800V_12bB_48V"] = 16;
  experiment_fields["180705_Cd_14kV_800V_12bB_48V"] = 14;
  experiment_fields["180705_Cd_13kV_800V_12bB_48V"] = 13;
  experiment_fields["180705_Cd_12kV_800V_6bB_48V"] = 12;
  experiment_fields["180705_Cd_11kV_800V_6bB_48V"] = 11;
  experiment_fields["180705_Cd_10kV_800V_6bB_48V"] = 10;
  experiment_fields["180705_Cd_9kV_800V_0bB_48V"] = 9;
  experiment_fields["180705_Cd_8kV_800V_0bB_48V"] = 8;
  
  {
    double coeff = (600 / 804.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
    for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
      j->second *= coeff;
  }
 
  if (areas_to_draw.empty())
	areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(0, 1); //slow PMTs #2+3+4 (with WLS) and slow PMT#1 (no WLS)
  areas_to_draw.back().channels.push_pair(8, 12); //fast PMTs no. 1-4 and sum
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(0, 16);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
  Initialize();
  //Following variables are required for analysis_history.cpp
  std::vector<int> calib_channels = {0, 1, 8, 9, 10, 11, 12};
  for (int ch=32; ch!=64; ++ch) {
    calib_channels.push_back(ch);
  }
}
