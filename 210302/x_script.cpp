{
if (true) {
	// nch(); set_zoom(0, 0.04, 0, 0.005); set_bins(1000); set_log_z();
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_lower(0.0061, 0.00057, 0.01, 0.00049, display, channel, "small_A-S_noise");
	x_y_regions = {0.0061, 0.0061, 0.00046, 0.04, 0.00253, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0061, 0.00134, 0.0145, 0.00162, display, channel, "2pe_merged");
}
if (false) { //Inspect SiPM en spectrum
	double d_S2_start = 22;
	int SiPM_state = 0;
	nex();
	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 800);
}
if (false) { //Inspect sPMTs en spectrum
	double d_S2_start = 22;
	int PMT_state = 0;
	nex();
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 400);
}
if (false) {
	nex();
	set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
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
	analysis_history(calibrate, method); //5.5
	nex();
	analysis_history(calibrate, method); //5.5
	nex();
}
}
