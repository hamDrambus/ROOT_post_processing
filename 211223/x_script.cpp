{
if (false) {
	// nch(); set_zoom(0, 0.06, 0, 0.01); set_bins(500); set_log_z();
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	//cut_A_S_lower(0.0068, 0.00061, 0.02, 0.00110, display, channel, "small_A-S_noise");
	x_y_regions = {0.0091, 0.0091, 0.00056, 0.06, 0.0069, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0207, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0091, 0.00175, 0.0207, 0.00318, display, channel, "2pe_merged");
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
}
}
