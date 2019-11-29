{
if (false) {
	int offset = -2;
	cut_x_y_lower(25, 0 + offset, 48, 18+ offset, true, "1");
	cut_x_y_lower(48, 18+ offset, 68, 28+ offset, true, "2");
	cut_x_y_lower(68, 28+ offset, 103, 54+ offset, true, "3");
	cut_x_y_lower(103, 54+ offset, 120, 70+ offset, true, "4");
	cut_x_y_lower(120, 70+ offset, 300, 190+ offset, true, "5");
	update();
}
if (false) {
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;	
	x_y_regions = {0.011, 0.011, 0.0012, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.026, 0.0038, display, channel, "2pe_merged");
	}
if (true) {
	analysis_history(false, 0); //20
	nex();
	analysis_history(false, 0); //18
	nex();
	analysis_history(false, 0); //16
	nex();
	analysis_history(false, 0); //14
	nex();
	analysis_history(false, 0); //12
	nex();
	analysis_history(false, 0);	//10
	nex();
	analysis_history(false, 0);	//8
	nex();
}
if (false) {
	analysis_history(true, 2); //20
	nex();
	analysis_history(true, 2); //18
	nex();
	analysis_history(true, 2); //16
	nex();
	analysis_history(true, 2); //14
	nex();
	analysis_history(true, 2); //12
	nex();
	analysis_history(true, 2);	//10
	nex();
	analysis_history(true, 2);	//8
	nex();
}

} 
