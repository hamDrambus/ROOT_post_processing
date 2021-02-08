{
if (false) {
	// nch(); set_zoom(0, 0.08, 0, 0.012); set_bins(500);
	int channel = 8;
	bool display = true;
	int aggressiveness = 1;
	cut_A_S_upper(0.003, 0.00019, 0.0050, 0.00038, display, channel, "rem_smallA_largeS");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
		cut_A_S_upper(0.0, 0.015, 1.6, 0.015, display, channel, "rem_S>0.015");
	}
}
if (false) {
	unsigned int max_iter = 5;
	for (int i = 1; i!=(max_iter+1); ++i)
		analysis_history(false, i);
}
if (false) {
	unsigned int max_iter = 5;
	unsigned int exps = post_processor->experiments.size();
	for (int e = 0; e!=exps; ++e) {
		for (int i = 0; i!=(max_iter+1); ++i)
			analysis_history(false, i);
		nex();
	}
}
if (true) {
	bool calibrate = false;
	unsigned int method = 0;
	analysis_history(calibrate, method); //16.3
	nex();
	analysis_history(calibrate, method); //14.6
	nex();
	analysis_history(calibrate, method); //13.0
	nex();
	analysis_history(calibrate, method); //11.4
	nex();
	analysis_history(calibrate, method); //10.6
	nex();
	analysis_history(calibrate, method); //9.7
	nex();
	analysis_history(calibrate, method); //8.9
	nex();
	analysis_history(calibrate, method); //8.1
	nex();
	analysis_history(calibrate, method); //6.5
	nex();
}
}
