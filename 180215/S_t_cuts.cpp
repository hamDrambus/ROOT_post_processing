{
  //From global parameters:
  data_prefix_path = "../Data/180215/results/";
  calibration_file = "PMT_SiPM_48V_180215.dat";
  data_output_path = "../Data/180215/results/";
  
  PMT_V.clear();
  PMT_V["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
  PMT_V["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
  PMT_V["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
  PMT_V["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
  PMT_V["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
  PMT_V["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;

  PMT_dB.clear();
  PMT_dB["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 3.98; //ratio, not actual dB
  PMT_dB["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 3.98;

  experiment_fields.clear();
  experiment_fields["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 6;
  experiment_fields["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 8;
  experiment_fields["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 10;
  experiment_fields["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 11;
  experiment_fields["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 12;
  experiment_fields["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 14;
  
  {
    double coeff = (600 / 804.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
    for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
      j->second *= coeff;
  }

  if (areas_to_draw.empty())
    areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().experiments.clear();
  areas_to_draw.back().experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  areas_to_draw.back().experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  areas_to_draw.back().experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  areas_to_draw.back().experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  areas_to_draw.back().experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  areas_to_draw.back().experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");

  exp_area.experiments.clear();
  exp_area.experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  exp_area.experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  exp_area.experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  exp_area.experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  exp_area.experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  exp_area.experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
  //End of global parameters
  
  S_T_to_exclude.resize(25);
  S_min_thresholds.resize(25);
  S_min_thresholds = {0.0015, 0.0013, 0.0016, 0.0014, 0.0015, 0.0014, 0.0013, 0.0013, 0.0013, 0.0012, 0.0014,
		0.0014, 0.0012, 0.0014, 0.0013, 0.0012, 0.0013, 0.0014, 0.0014, 0.0012, 0.0014, 0.0012, 0.0014, 0.0014, 0.0014};
	    
	  
  /*32*/S_T_to_exclude[0] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*33*/S_T_to_exclude[1] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*34*/S_T_to_exclude[2] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*35*/S_T_to_exclude[3] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*36*/S_T_to_exclude[4] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*37*/S_T_to_exclude[5] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*38*/S_T_to_exclude[6] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*39*/S_T_to_exclude[7] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*40*/S_T_to_exclude[8] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*41*/S_T_to_exclude[9] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*42*/S_T_to_exclude[10] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*43*/S_T_to_exclude[11] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};  
  /*44*/S_T_to_exclude[12] ={32, 34.5, 0, 1, 45, 48, 0, 0.001, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.001, 137, 140, 0, 0.0005};
  /*48*/S_T_to_exclude[13] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*49*/S_T_to_exclude[14] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*50*/S_T_to_exclude[15] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*51*/S_T_to_exclude[16] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*52*/S_T_to_exclude[17] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*53*/S_T_to_exclude[18] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*54*/S_T_to_exclude[19] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*55*/S_T_to_exclude[20] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*56*/S_T_to_exclude[21] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*57*/S_T_to_exclude[22] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*58*/S_T_to_exclude[23] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  /*59*/S_T_to_exclude[24] ={32, 34.5, 0, 1, 45, 48, 0, 0.0008, 75.5, 77, 0, 0.0006, 107, 111, 0, 0.0009, 137, 140, 0, 0.0005};
  
  //Temporary remove central MPPCs
  S_T_to_exclude[4] = {0, 160, 0, 2};//36
  S_T_to_exclude[6] = {0, 160, 0, 2};//38
  S_T_to_exclude[7] = {0, 160, 0, 2};//39
  S_T_to_exclude[8] = {0, 160, 0, 2};//40
  S_T_to_exclude[9] = {0, 160, 0, 2};//41
  S_T_to_exclude[15] ={0, 160, 0, 2};//50
  S_T_to_exclude[16] ={0, 160, 0, 2};//51
  S_T_to_exclude[18] ={0, 160, 0, 2};//53
  S_T_to_exclude[21] ={0, 160, 0, 2};//56
  
  
  S2_times.resize(6);
  S2_times[0]= std::pair<double,double>(50, 140);//6
  S2_times[1]= std::pair<double,double>(48, 110);//8
  S2_times[2]= std::pair<double,double>(40, 85);//10
  S2_times[3]= std::pair<double,double>(40, 83);//11
  S2_times[4]= std::pair<double,double>(40, 83);//12
  S2_times[5]= std::pair<double,double>(40, 83);//14
}
