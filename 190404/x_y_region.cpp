{
	//This file is for substituting typing into console.
	//All parameters/actions are chosen during analysis and can't be reproduced by running dedicated
	//script as in case of "cut_Ss.cpp" or "cut_S2_times.cpp"

	//Cd_20kV_850V_46V_0, 18kV, 16kV, 14kV, 12kV, 10kV, 8kV
	//x_y_regions = {-1, 0.04, 0, 0.8, 0.105, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	//cut_A_S_fast_PMT(x_y_regions, false, 15, "large_A_noise");
	//x_y_regions = {-1, 0.020, 0.017, 0.1, 0.0, 1e3};//small S ch15 Cd_20kV_850V_46V
	//cut_A_S_fast_PMT(x_y_regions, false, 15, "small_S_noise");
	x_y_regions = {-1, 0.003, 0.000008, 0.4, 0.0069, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	cut_A_S_fast_PMT(x_y_regions, 1, 8, "large_A_noise");
	//cut_A_S_upper(0.005, 0.00018, 0.025, 0.00077, true);
	cut_A_S_upper(0.003, 0.00012, 0.6, 0.021, true);
	//x_y_regions = {-1, 0, 0.02, 0.054, 0.0, 1e3};//small S ch15 Cd_20kV_850V_46V. Best to determine using 0-23 us peaks
	//x_y_regions = {-1, 0, 0.012, 0.038, 0.0, 1e3};	
	//cut_A_S_fast_PMT(x_y_regions, 0, 12, "small_S_noise");

	//Cd_20kV_850V_46V
	//x_y_regions = {29, 39, 0, 1e5}; //ch16 t-S 

	//Cd_20kV_850V_46V_0, 18kV
	//x_y_regions = {29, 39, 0, 1e5}; //ch16 t-S 

	//Cd_16kV_850V_46V
	//x_y_regions = {29, 41, 0, 1e5}; //ch16 t-S 
	//x_y_regions = {0, 0.17, 0, 0.7}; //ch16 A-S exclude 

	//Cd_14kV_850V_46V
	//x_y_regions = {29, 40.5, 0, 1e5}; //ch16 t-S 

	//Cd_12kV_850V_46V
	//x_y_regions = {29.5, 39, 0, 1e5}; //ch16 t-S 

	//Cd_10kV_850V_46V
	//x_y_regions = {29, 39, 0, 1e5}; //ch16 t-S
	//x_y_regions = {0.06, 0.11, 0.1, 1e5}; //ch16 A-S
	//cut_A_S_rect_exclude(x_y_regions, false, -1, "1");
	//x_y_regions = {0.06, 0.13, 0.7, 1e5}; //ch16 A-S
	//cut_A_S_rect_exclude(x_y_regions, false, -1, "2");
	//x_y_regions = {0.06, 0.14, 1.2, 1e5}; //ch16 A-S
	//cut_A_S_rect_exclude(x_y_regions, false, -1, "3");
	//x_y_regions = {0.06, 0.18, 1.4, 1e5}; //ch16 A-S
	//cut_A_S_rect_exclude(x_y_regions, false, -1, "4");
	//x_y_regions = {0.06, 0.25, 1.8, 1e5}; //ch16 A-S
	//cut_A_S_rect_exclude(x_y_regions, false, -1, "5");

	//Cd_8kV_850V_46V
	//x_y_regions = {29, 39, 0, 1e5}; //ch16 t-S
	/*
	for (int chn = 32; chn<=44; ++chn) {
		ch(chn);
		std::string name = "190404/results/Cd_46V_8kV_850V/NpeSoS1pe_"+std::to_string(chn)+"_25-80us_Cd_peak";
		saveas(name);
	} //!!!ch 43 does not work (bad contact on 190404 day)
	for (int chn = 48; chn<=59; ++chn) {
		ch(chn);
		std::string name = "190404/results/Cd_46V_8kV_850V/NpeSoS1pe_"+std::to_string(chn)+"_25-80us_Cd_peak";
		saveas(name);
	}*/
	/*
	for (int chn = 8; chn<=15; ++chn) {
		ch(chn);
		set_bins(400);
		set_zoom(20, 80);
		std::string name = "190404/results/Cd_46V_8kV_850V/formN_"+std::to_string(chn)+"_Cd_peak";
		saveas(name);
	}*/
} 
