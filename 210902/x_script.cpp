{
if (false) {
	// nch(); set_zoom(0, 0.08, 0, 0.012); set_bins(500);
	int channel = 4;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_upper(0.01, 0.0, 0.0171, 0.0, display, channel, "rem_smallA_largeS_1");
	cut_A_S_lower(0.01, 0.00051, 0.035, 0.0023, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.035, 0.0023, 1.6, 0.26, display, channel, "rem_largeA_smallS");
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
}
}
