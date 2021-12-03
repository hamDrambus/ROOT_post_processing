{
if (false) {
	// nch(); set_zoom(0, 0.06, 0, 0.01); set_bins(500); set_log_z();
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	//cut_A_S_lower(0.0088, 0.00119, 0.022, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0088, 0.0088, 0.00027, 0.06, 0.00505, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0088, 0.00152, 0.0255, 0.00364, display, channel, "2pe_merged");
}
if (false) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
}
if (true) {
	std::string exp = post_processor->experiments[post_processor->current_exp_index];
	if (exp == "211007_Pu_19.8kV_800V_46V_12dB_2500V") {
		ty(AStates::MPPC_Npe_sum);
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, 0, false);
			cut_t(0, 160, false, chan);
		}

		set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
		ty(AStates::Correlation_y);
		time_zoom_SiPMs(22.0, 29.7);
		ty(AStates::Correlation);
		set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in[22.0, 29.7] #mus");
		set_zoom(0, 4000, 0, 4000);
		set_bins(500);
		x_y_regions = {1213, 200, 1339, 200, 1339, 284, 1213, 284};
		cut_x_y_poly_select(x_y_regions, true, "1");
		update();
		saveaspng("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/01_SiPMs_Npe_22-29.7us_vs_0-160us");
		set_as_run_cut("SiPMs_ZxZy");
		print_accepted_events("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/01_events.txt", experiment_runs[exp]);
		remcut(-1, "1");
		unset_as_run_cut("SiPMs_ZxZy");

		x_y_regions = {1574, 200, 1739, 200, 1739, 310, 1574, 310};
		cut_x_y_poly_select(x_y_regions, true, "1");
		update();
		saveaspng("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/02_SiPMs_Npe_22-29.7us_vs_0-160us");
		set_as_run_cut("SiPMs_ZxZy");
		print_accepted_events("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/02_events.txt", experiment_runs[exp]);
		remcut(-1, "1");
		unset_as_run_cut("SiPMs_ZxZy");

		x_y_regions = {1976, 167, 2260, 167, 2260, 323, 1976, 323};
		cut_x_y_poly_select(x_y_regions, true, "1");
		update();
		saveaspng("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/03_SiPMs_Npe_22-29.7us_vs_0-160us");
		set_as_run_cut("SiPMs_ZxZy");
		print_accepted_events("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/03_events.txt", experiment_runs[exp]);
		remcut(-1, "1");
		unset_as_run_cut("SiPMs_ZxZy");

		x_y_regions = {1438, 362, 1599, 388, 1564, 479, 1424, 447};
		cut_x_y_poly_select(x_y_regions, true, "1");
		update();
		saveaspng("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/04_SiPMs_Npe_22-29.7us_vs_0-160us");
		set_as_run_cut("SiPMs_ZxZy");
		print_accepted_events("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/04_events.txt", experiment_runs[exp]);
		remcut(-1, "1");
		unset_as_run_cut("SiPMs_ZxZy");

		x_y_regions = {1829, 370, 2110, 370, 2110, 505, 1829, 505};
		cut_x_y_poly_select(x_y_regions, true, "1");
		update();
		saveaspng("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/05_SiPMs_Npe_22-29.7us_vs_0-160us");
		set_as_run_cut("SiPMs_ZxZy");
		print_accepted_events("211007/results_v5/Pu_46V_19.8kV_800V_2500V/events/05_events.txt", experiment_runs[exp]);
		remcut(-1, "1");
		unset_as_run_cut("SiPMs_ZxZy");
	}
}
}
