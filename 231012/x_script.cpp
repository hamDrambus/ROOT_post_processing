{
if (false) {
	// nch(); set_zoom(0, 0.05, 0, 0.01); set_bins(800);
	// set_log_z();
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_lower(0.0067, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
	x_y_regions = {0.0067, 0.0067, 0.00057, 0.05, 0.0090, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0157, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0067, 0.00180, 0.0157, 0.00340, display, channel, "2pe_merged");
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
	save_SiPM_Npe_table(data_output_path + "SiPM_Npes.txt");
}
}
