{
if (true) {
	// nch(); set_zoom(0, 0.04, 0, 0.009); set_bins(1000); set_log_z();
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	x_y_regions = {0.0030, 0.00041, 0.0086, 0.00073, 0.0104, 0.00073, 0.0141, 0.00098, 0.0141, 0.00157, 1, 0.18};
	cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0030, 0.00106, 0.0132, 0.00261, display, channel, "2pe_merged");
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
