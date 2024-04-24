{
if (false) {
    // sPMTs:
    // nch(); set_zoom(0, 0.24, 0, 0.08); set_bins(1500); set_log_z();
    // cut_t(80, 160, false, post_processor->current_channel); update();
    // fPMTs:
    // nch(); set_zoom(0, 0.1, 0, 0.004); set_bins(1500); set_log_z();
    // set_log_z(); set_bins(200, 400);
    // SiPM:
    // nch(); set_zoom(0, 0.05, 0, 0.01); set_bins(800); set_log_z();
    // set_log_z();
    int channel = 8;
    bool display = true;
    int aggressiveness = 2;
    cut_A_S_lower(0.0005, 0.00042, 0.0028, 0.0, display, channel, "rem_smallA_smallS");
    x_y_regions = {0.0014, 0.0014, 8.5e-5, 0.03, 0.00100, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
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
    // save_Npe_tables(data_output_path + "Npes_1ph_6mm_coll_Bkg.txt",
    //         data_output_path + "Npes_1ph_6mm_coll_S1.txt",
    //         data_output_path + "Npes_1ph_6mm_coll_S2.txt",
    //         data_output_path + "Npes_1ph_6mm_coll_S2_wo_S1_tail.txt");
}
}
