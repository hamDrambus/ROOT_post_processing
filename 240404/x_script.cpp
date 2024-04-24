{
if (false) {
    std::vector<double> xy_points;
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
    bool display = true;
    int aggressiveness = 1;
    // cut_A_S_lower(0.0, 0.00032, 0.010, 0.0, display, channel, "small_A-S_noise");
    xy_points = {0.0020, 0.0020, 0.000149, 0.03, 0.00148, 1e3};
    cut_A_S_fast_PMT(xy_points, display, channel, "small_A-S_noise2");
    if (aggressiveness == 1) {
        cut_A_S_upper(0.65, 0, 5, 0, display, channel, "rem_A>0.65");
        cut_A_S_upper(0.0, 0.030, 1.6, 0.030, display, channel, "rem_S>0.030");
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
    save_Npe_tables_raw(data_output_path + "Npes_raw_Bkg.txt",
            data_output_path + "Npes_raw_S1.txt",
            data_output_path + "Npes_raw_S2.txt");
}
}
