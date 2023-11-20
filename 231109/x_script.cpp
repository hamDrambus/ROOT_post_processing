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
    int channel = 32;
    bool display = true;
    int aggressiveness = 2;
    cut_A_S_lower(0.0062, 0.00121, 0.02, 0.00121, display, channel, "small_A-S_noise");
    x_y_regions = {0.0062, 0.0062, 0.00061, 0.05, 0.0090, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
    cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
    if (aggressiveness>=1)//select only 1 photoelectron for calibration
        cut_A_S_upper(0.0152, 0.0001, 1, 0.0001, display, channel, "2pe");
    if (aggressiveness>=2)//remove afterimpulses
        cut_A_S_upper(0.0062, 0.00163, 0.0152, 0.00342, display, channel, "2pe_merged");
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
    save_SiPM_Npe_tables(data_output_path + "SiPM_Bkg_Npes.txt",
            data_output_path + "SiPM_S1_Npes.txt",
            data_output_path + "SiPM_S2_Npes.txt",
            data_output_path + "SiPM_S2_wo_S1_tail_Npes.txt");
}
}
