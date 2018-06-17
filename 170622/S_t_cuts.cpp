{
  //From global parameters:
  data_prefix_path = "../Data/170622/results/";
  calibration_file = "PMT_SiPM_46V_170622.dat";
  data_output_path = "../Data/170622/results/";
  
  PMT_V.clear();
  PMT_V["event_x-ray_4_thmV"] = 700;
  PMT_V["event_x-ray_5_thmV"] = 700;
  PMT_V["event_x-ray_6_thmV"] = 700;
  PMT_V["event_x-ray_7_thmV"] = 700;
  PMT_V["event_x-ray_8_thmV"] = 700;
  PMT_V["event_x-ray_9_thmV"] = 700;
  PMT_V["event_x-ray_10_thmV_recalib"] = 700;
  PMT_V["event_x-ray_10_thmV"] = 550;
  PMT_V["event_x-ray_12_thmV"] = 550;
  PMT_V["event_x-ray_14_thmV"] = 550;
  PMT_V["event_x-ray_16_thmV"] = 550;
  PMT_V["event_x-ray_18_thmV"] = 550;
  PMT_V["event_x-ray_20_thmV"] = 550;

  PMT_dB.clear();
  PMT_dB["event_x-ray_10_thmV_recalib"] = 2; //ratio, not actual dB
  PMT_dB["event_x-ray_16_thmV"] = 3.98;
  PMT_dB["event_x-ray_18_thmV"] = 3.98;
  PMT_dB["event_x-ray_20_thmV"] = 3.98;

  experiment_fields.clear();
  experiment_fields["event_x-ray_4_thmV"] = 4;
  experiment_fields["event_x-ray_5_thmV"] = 5;
  experiment_fields["event_x-ray_6_thmV"] = 6;
  experiment_fields["event_x-ray_7_thmV"] = 7;
  experiment_fields["event_x-ray_8_thmV"] = 8;
  experiment_fields["event_x-ray_9_thmV"] = 9;
  experiment_fields["event_x-ray_10_thmV_recalib"] = 9.9;
  experiment_fields["event_x-ray_10_thmV"] = 10.1;
  experiment_fields["event_x-ray_12_thmV"] = 12;
  experiment_fields["event_x-ray_14_thmV"] = 14;
  experiment_fields["event_x-ray_16_thmV"] = 16;
  experiment_fields["event_x-ray_18_thmV"] = 18;
  experiment_fields["event_x-ray_20_thmV"] = 20;
  {
    double coeff = (600 / 804.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
    for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
      j->second *= coeff;
  }

  if (areas_to_draw.empty())
    areas_to_draw.push_back(experiment_area());
  areas_to_draw.back().experiments.clear();
  areas_to_draw.back().experiments.push_back("event_x-ray_4_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_5_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_6_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_7_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_8_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_9_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_10_thmV_recalib");
  areas_to_draw.back().experiments.push_back("event_x-ray_10_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_12_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_14_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_16_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_18_thmV");
  areas_to_draw.back().experiments.push_back("event_x-ray_20_thmV");
  
  exp_area.experiments.clear();
  exp_area.experiments.push_back("event_x-ray_4_thmV");
  exp_area.experiments.push_back("event_x-ray_5_thmV");
  exp_area.experiments.push_back("event_x-ray_6_thmV");
  exp_area.experiments.push_back("event_x-ray_7_thmV");
  exp_area.experiments.push_back("event_x-ray_8_thmV");
  exp_area.experiments.push_back("event_x-ray_9_thmV");
  exp_area.experiments.push_back("event_x-ray_10_thmV_recalib");
  exp_area.experiments.push_back("event_x-ray_10_thmV");
  exp_area.experiments.push_back("event_x-ray_12_thmV");
  exp_area.experiments.push_back("event_x-ray_14_thmV");
  exp_area.experiments.push_back("event_x-ray_16_thmV");
  exp_area.experiments.push_back("event_x-ray_18_thmV");
  exp_area.experiments.push_back("event_x-ray_20_thmV");
  //end of global parameters

  S_T_to_exclude.resize(24);
  S_min_thresholds.resize(24);
  S_min_thresholds = {0.00065, 0.0005, 0.00075, 0.00075, 0.00075, 0.0006, 0.00055, 0.00055, 0.00055, 0.00045, 0.00065, 0.0005,
		  1, 0.0006, 0.0005, 0.0005, 0.0005, 0.0006, 0.0006, 0.0006, 0.0006, 0.0006, 0.00055, 0.0006 };
	  
  /*32*/S_T_to_exclude[0] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*33*/S_T_to_exclude[1] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*34*/S_T_to_exclude[2] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*35*/S_T_to_exclude[3] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*36*/S_T_to_exclude[4] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*37*/S_T_to_exclude[5] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*38*/S_T_to_exclude[6] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*39*/S_T_to_exclude[7] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*40*/S_T_to_exclude[8] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*41*/S_T_to_exclude[9] = {32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*42*/S_T_to_exclude[10] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*43*/S_T_to_exclude[11] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};  
  /*44*/S_T_to_exclude[12] ={32, 35, 0, 1, 45, 48, 0, 0.0005, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0005, 137, 140, 0, 0.0005};
  /*48*/S_T_to_exclude[13] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*49*/S_T_to_exclude[14] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*50*/S_T_to_exclude[15] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*51*/S_T_to_exclude[16] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*52*/S_T_to_exclude[17] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*53*/S_T_to_exclude[18] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*54*/S_T_to_exclude[19] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*55*/S_T_to_exclude[20] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*56S_T_to_exclude[21] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};*/
  /*57*/S_T_to_exclude[21] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*58*/S_T_to_exclude[22] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  /*59*/S_T_to_exclude[23] ={32, 35, 0, 1, 45, 48, 0, 0.0006, 75.5, 77, 0, 0.0006, 109, 112.5, 0, 0.0006, 137, 140, 0, 0.0005};
  
  S_T_to_exclude[12] = {0, 160, 0, 2};//44
  //Temporary remove central MPPCs
  /*S_T_to_exclude[4] = {0, 160, 0, 2};//36
  S_T_to_exclude[6] = {0, 160, 0, 2};//38
  S_T_to_exclude[7] = {0, 160, 0, 2};//39
  S_T_to_exclude[8] = {0, 160, 0, 2};//40
  S_T_to_exclude[9] = {0, 160, 0, 2};//41
  S_T_to_exclude[15] ={0, 160, 0, 2};//50
  S_T_to_exclude[16] ={0, 160, 0, 2};//51
  S_T_to_exclude[18] ={0, 160, 0, 2};//53
  S_T_to_exclude[21] ={0, 160, 0, 2};//56
  */
  S2_times.resize(13);
  S2_times[0]= std::pair<double,double>(77, 160);//4
  S2_times[1]= std::pair<double,double>(60, 150);//5
  S2_times[2]= std::pair<double,double>(55, 125);//6
  S2_times[3]= std::pair<double,double>(53, 115);//7
  S2_times[4]= std::pair<double,double>(52, 102);//8
  S2_times[5]= std::pair<double,double>(50, 95);//9
  S2_times[6]= std::pair<double,double>(47, 110);//10 rec
  S2_times[7]= std::pair<double,double>(45, 90);//10
  S2_times[8]= std::pair<double,double>(45, 85);//12
  S2_times[9]= std::pair<double,double>(43, 88);//14
  S2_times[10]= std::pair<double,double>(43, 87);//16
  S2_times[11]= std::pair<double,double>(43, 87);//18
  S2_times[12]= std::pair<double,double>(43, 86);//20
}