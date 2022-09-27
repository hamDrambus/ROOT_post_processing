{
if (false) {
	// nch(); set_zoom(0, 0.05, 0, 0.01); set_bins(800);
	// set_log_z();
	int channel = 7;
	bool display = true;
	int aggressiveness = 1;
	if (aggressiveness == 1) {
		cut_A_S_upper(0.54, 0, 2, 0, display, channel, "rem_A>0.54");
		cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
	}
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
}
}
