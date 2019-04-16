{
  //From global parameters:
  data_prefix_path = "../Data/190404/results/";
  calibration_file = "PMT_SiPM_46V_190404.dat";
  data_output_path = "../Data/190404/results/";
  DATA_MPPC_VERSION = "MPPCs_v1";
  DATA_PMT_VERSION = "PMT_v1";

  OUTPUT_MPPCS_PICS = "MPPCs_v1/MPPCs_";
  OUTPUT_PMT_PICS = "PMT_v1/PMT_";
  OUTPUT_MPPCS = "MPPC_";

  exp_area.experiments.clear();
  exp_area.experiments.push_back("190404_Cd_20kV_850V_46V_th250mV");
  exp_area.experiments.push_back("190404_Cd_20kV_850V_46V_th250mV_0");
  exp_area.experiments.push_back("190404_Cd_18kV_850V_46V_th230mV");
  //exp_area.experiments.push_back("190404_Cd_16kV_850V_46V_th210mV");
  
  PMT_V.clear();
  MPPC_V.clear();
  PMT_dB.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 850;
	  PMT_dB[*i] = 1.0; //ratio, not actual dB. 6dB = 2.0, 12dB = 3.98
	  MPPC_V[*i] = 46;
  }

  experiment_fields.clear();
  experiment_fields["190404_Cd_20kV_850V_46V_th250mV_0"] = 20;
  experiment_fields["190404_Cd_20kV_850V_46V_th250mV"] = 20;
  experiment_fields["190404_Cd_18kV_850V_46V_th230mV"] = 18;
  experiment_fields["190404_Cd_16kV_850V_46V_th210mV"] = 16;
  
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
  areas_to_draw.back().channels.push_pair(8, 11); //fast PMTs no. 1-4
  areas_to_draw.back().channels.push_pair(12, 15); //slow PMTs no. 1-4
  areas_to_draw.back().channels.push_pair(16, 16); //Sum of slow PMTs after shaping amplifier
  areas_to_draw.back().channels.push_pair(32, 62);
  areas_to_draw.back().sub_runs.push_pair(0, 0);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(8, 16);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
}
