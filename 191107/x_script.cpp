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
if (true) {
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_lower(0.0107, 0.00124, 0.04, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0107, 0.0107, 0.00040, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0107, 0.00172, 0.0255, 0.00364, display, channel, "2pe_merged");
}
if (false) {
	bool calibrate = false;
	unsigned int method = 1;
	analysis_history(calibrate, method); //20
	nex();
	analysis_history(calibrate, method); //20
	nex();
	analysis_history(calibrate, method); //18
	nex();
	analysis_history(calibrate, method); //16
	nex();
	analysis_history(calibrate, method); //14
	nex();
	analysis_history(calibrate, method); //12
	nex();
	analysis_history(calibrate, method);	//10
	nex();
	analysis_history(calibrate, method);	//8
	nex();
}
} 
