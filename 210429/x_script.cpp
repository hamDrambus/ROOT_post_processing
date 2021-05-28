{
if (false) {
	// nch(); set_zoom(0, 0.08, 0, 0.012); set_bins(500);
	int channel = 32;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_lower(0.0105, 0.00111, 0.05, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0105, 0.0105, 0.00063, 0.06, 0.00575, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0253, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0105, 0.00162, 0.0253, 0.00337, display, channel, "2pe_merged");
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
}
}
