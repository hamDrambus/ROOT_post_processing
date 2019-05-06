{
  //From global parameters:
  data_prefix_path = "test_data/";
  calibration_file = "PMT_SiPM_test.dat";
  data_output_path = "test_data/";
  DATA_MPPC_VERSION = ".";
  DATA_PMT_VERSION = ".";

  OUTPUT_MPPCS_PICS = "./MPPCs_";
  OUTPUT_PMT_PICS = "./PMT_";
  OUTPUT_MPPCS = "MPPC_";

  exp_area.experiments.clear();
  exp_area.experiments.push_back("10V_exp");
  exp_area.experiments.push_back("15V_exp");
  
  PMT_V.clear();
  MPPC_V.clear();
  PMT_dB.clear();
  for (auto i = exp_area.experiments.begin(), i_end_ = exp_area.experiments.end(); i != i_end_; ++i) {
	  PMT_V[*i] = 850;
	  PMT_dB[*i] = 1.0; //ratio, not actual dB. 6dB = 2.0, 12dB = 3.98
	  MPPC_V[*i] = 46;
  }

  experiment_fields.clear();
  experiment_fields["10V_exp"] = 10;
  experiment_fields["15V_exp"] = 15;
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
  areas_to_draw.back().sub_runs.push_pair(0, 0);
  areas_to_draw.back().channels.push_pair(2, 6);
  areas_to_draw.back().channels.push_pair(32, 64);

  exp_area.channels.erase();
  exp_area.runs.erase();
  exp_area.sub_runs.erase();
  exp_area.channels.push_pair(2, 6);//will load only present channels
  exp_area.channels.push_pair(32, 62);//will load only present channels
  exp_area.runs.push_pair(0, 0);
  exp_area.sub_runs.push_pair(0, 0);
  //end of global parameters
}
