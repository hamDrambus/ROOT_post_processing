{
  //From global parameters:
  data_prefix_path = "../Data/CdTime/180705/results/";
  calibration_file = "PMT_SiPM_48V_180705.dat";
  data_output_path = "../Data/CdTime/180705/results/";
  DATA_MPPC_VERSION = "MPPCs_v1";
  DATA_PMT_VERSION = "PMT_v2";

  OUTPUT_MPPCS_PICS = "MPPCs_v1/MPPCs_";
  OUTPUT_PMT_PICS = "PMT_v2/PMT_";
  OUTPUT_MPPCS = "MPPC_";
  
  PMT_V.clear();
  PMT_V["180705_Cd_10kV_800V_6bB_48V"] = 800;
  PMT_V["180705_Cd_11kV_800V_6bB_48V"] = 800;
  PMT_V["180705_Cd_12kV_800V_6bB_48V"] = 800;


  PMT_dB.clear();
  PMT_dB["180705_Cd_10kV_800V_6bB_48V"] = 2.0; //ratio, not actual dB. 6dB = 2.0, 12dB = 3.98
  PMT_dB["180705_Cd_11kV_800V_6bB_48V"] = 2.0;
  PMT_dB["180705_Cd_12kV_800V_6bB_48V"] = 2.0;

  experiment_fields.clear();
  experiment_fields["180705_Cd_10kV_800V_6bB_48V"] = 10;
  experiment_fields["180705_Cd_11kV_800V_6bB_48V"] = 11;
  experiment_fields["180705_Cd_12kV_800V_6bB_48V"] = 12;
  
  {
    double coeff = (600 / 804.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
    for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
      j->second *= coeff;
  }
   
  exp_area.experiments.clear();
  //exp_area.experiments.push_back("coll6_20kV_850V_12dB_46V");
  exp_area.experiments.push_back("180705_Cd_10kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_11kV_800V_6bB_48V");
  exp_area.experiments.push_back("180705_Cd_12kV_800V_6bB_48V");
 
  if (areas_to_draw.empty())
	areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().channels.erase();
  areas_to_draw.back().runs.erase();
  areas_to_draw.back().sub_runs.erase();
  areas_to_draw.back().runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(0, 1);
  areas_to_draw.back().channels.push_pair(8, 12);
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(0, 1);//will load only present channels
  exp_area.channels.push_pair(8, 12);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
}
