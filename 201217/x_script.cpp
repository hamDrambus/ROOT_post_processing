{
if (true) {
	// nch(); set_zoom(0, 0.08, 0, 0.012); set_bins(500);
	int channel = 9;
	bool display = true;
	int aggressiveness = 1;
	cut_A_S_lower(0.02, 0.045, 0.038, 0.00070, display, channel, "rem_smallA_smallS");
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
if (false) {
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
