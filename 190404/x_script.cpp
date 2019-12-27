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
	int channel = 11;
	bool display = false;
	int aggressiveness = 1;
	cut_A_S_upper(0.00362, 0.000112, 0.009, 0.000238, display, channel, "rem_smallA_largeS");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (true) {
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
