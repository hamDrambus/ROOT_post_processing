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
if (true) {
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
if (false) {
	bool calibrate = false;
	unsigned int method = 0;
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
	analysis_history(calibrate, method); //10
	nex();
}
}
