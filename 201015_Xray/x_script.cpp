{
if (false) {
    // sPMTs:
    // nch(); set_zoom(0, 0.24, 0, 0.08); set_bins(1500);
    // cut_t(80, 160, false, post_processor->current_channel); update();
    // fPMTs:
    // nch(); set_zoom(0, 0.1, 0, 0.004); set_bins(1500);
    // set_log_z(); set_bins(200, 400);
    // SiPM:
    // nch(); set_zoom(0, 0.05, 0, 0.01); set_bins(800);
    // set_log_z();
    int channel = 8;
    bool display = false;
    int aggressiveness = 1;
	cut_A_S_upper(0.001, 0.0, 0.00376, 0.0, display, channel, "rem_smallA-S");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
		cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
	}
}
if (false) {
    ty(AStates::Correlation_x);
    slow_PMTs_only();
    cut_PMT_noise(0);
    ty(AStates::Correlation_y);
    cut_SiPM_noise(0);
    ty(AStates::Correlation);
    set_zoom(0, 300, 0, 100);
    set_bins(300, 100);
}
if (true) {
    for (int e = 0; e != exp_area.experiments.size(); ++e) {
        analysis_history(0);
        nex();
    }
    save_Npe_tables(data_output_path + "Npes_Bkg.txt",
            data_output_path + "Npes_S1.txt",
            data_output_path + "Npes_S2.txt",
            data_output_path + "Npes_S2_wo_S1_tail.txt");
}
}