void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_PMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
void time_zoom_PMTs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e);
void time_zoom_SiPMs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e);
std::string cuts_str(std::vector<int> cuts);
void central_SiPMs(bool on);
void fast_PMTs_only (void);
void slow_PMTs_only (void);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);

void time_zoom_PMTs (double f_time, double to_time) {
    // cut_t(f_time, to_time, false, 0);
    cut_t(f_time, to_time, false, 1);
    // cut_t(f_time, to_time, false, 2);
    cut_t(f_time, to_time, false, 3);
    cut_t(f_time, to_time, false, 4);

    cut_t(f_time, to_time, false, 5);
    // cut_t(f_time, to_time, false, 6);
    cut_t(f_time, to_time, false, 7);
    cut_t(f_time, to_time, false, 8);
}

void time_zoom_PMTs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e) {
    // cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 0);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 1);
    // cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 2);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 3);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 4);

    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 5);
    // cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 6);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 7);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 8);
}

void time_zoom_SiPMs (double f_time, double to_time) {
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        cut_t(f_time, to_time, false, chan);
    }
}

void time_zoom_SiPMs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e) {
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, chan);
    }
}

std::string cuts_str(std::vector<std::string> cuts) {
    std::string out = "no_cuts";
    if (!cuts.size())
        return out;
    out = "cuts_";
    for (std::size_t i = 0, i_end_ = cuts.size(); i!=i_end_; ++i) {
        out += cuts[i]+"+";
    }
    out.pop_back();
    return out;
}

void central_SiPMs(bool on) {
    if (on) {
        on_ch(50); on_ch(51); on_ch(36);
        on_ch(53); on_ch(38); on_ch(39);
        on_ch(40); on_ch(41); on_ch(56);
    } else {
        off_ch(50); off_ch(51); off_ch(36);
        off_ch(53); off_ch(38); off_ch(39);
        off_ch(40); off_ch(41); off_ch(56);
    }
}

void turn_on_all_SiPMs(void) {
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        on_ch(chan);
    }
}

void fast_PMTs_only (void) {
    on_ch(5);
    // on_ch(6);
    on_ch(7); on_ch(8);
    // off_ch(0);
    off_ch(1);
    // off_ch(2);
    off_ch(3); off_ch(4);
}

void slow_PMTs_only (void) {
    on_ch(1);
    // on_ch(2);
    on_ch(3); on_ch(4);
    // off_ch(0);
    off_ch(5);
    // off_ch(6);
    off_ch(7); off_ch(8);
}

void cut_PMT_noise(int device_condition)
{
    noise_cut(1, 0, device_condition, 0);
    // noise_cut(2, 0, device_condition, 0);
    noise_cut(3, 0, device_condition, 0);
    noise_cut(4, 0, device_condition, 0);

    noise_cut(5, 0, device_condition, 0);
    // noise_cut(6, 0, device_condition, 0);
    noise_cut(7, 0, device_condition, 0);
    noise_cut(8, 0, device_condition, 0);
}

void cut_SiPM_noise(int device_condition)
{
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        noise_cut(chan, 0, device_condition, 0);
    }
}

void save_forms(std::string path, bool N_only, int PMT_condition, int SiPM_condition)
{
    double zoom_l = 0, zoom_r = 160;
    std::size_t Nbins = 1600;
    ty(AStates::MPPC_tbNpe_sum);
    central_SiPMs(true);
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        noise_cut(chan, 0, SiPM_condition, false);
    }
    set_zoom(zoom_l, zoom_r);	set_log_y();
    set_bins(Nbins);
    saveas(path + "SiPMs_form_by_Npe");
    unset_log_y();
    saveaspng(path + "SiPMs_form_by_Npe_lin");
    central_SiPMs(false); set_log_y();
    update();
    saveas(path + "SiPMs_edge_form_by_Npe");
    unset_log_y();
    saveaspng(path + "SiPMs_edge_form_by_Npe_lin");

    ty(AStates::PMT_tbN);
    ch(5);
    noise_cut(5, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "5_form_by_Npeaks");
    // ch(6);
    // noise_cut(6, 1, PMT_condition, false);
    // set_zoom(zoom_l, zoom_r); set_log_y();
    // set_bins(Nbins);
    // saveas(path + "6_form_by_Npeaks");
    ch(7);
    noise_cut(7, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "7_form_by_Npeaks");
    ch(8);
    noise_cut(8, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "8_form_by_Npeaks");

    ty(AStates::PMT_tbNpe);
    ch(1);
    noise_cut(1, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "1_form_by_Npe");
    // ch(2);
    // noise_cut(2, 1, PMT_condition, false);
    // set_zoom(zoom_l, zoom_r); set_log_y();
    // set_bins(Nbins);
    // saveas(path + "2_form_by_Npe");
    ch(3);
    noise_cut(3, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "3_form_by_Npe");
    ch(4);
    noise_cut(4, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "4_form_by_Npe");
    ch(5);
    noise_cut(5, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "5_form_by_Npe");
    // ch(6);
    // noise_cut(6, 1, PMT_condition, false);
    // set_zoom(zoom_l, zoom_r); set_log_y();
    // set_bins(Nbins);
    // saveas(path + "6_form_by_Npe");
    ch(7);
    noise_cut(7, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "7_form_by_Npe");
    ch(8);
    noise_cut(8, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "8_form_by_Npe");

    if (!N_only) {
        ty(AStates::PMT_tbS);
        ch(1);
        noise_cut(1, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "1_form_by_S");
        // ch(2);
        // noise_cut(2, 1, PMT_condition, false);
        // set_zoom(zoom_l, zoom_r); set_log_y();
        // set_bins(Nbins);
        // saveas(path + "2_form_by_S");
        ch(3);
        noise_cut(3, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "3_form_by_S");
        ch(4);
        noise_cut(4, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "4_form_by_S");
        ch(5);
        noise_cut(5, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "5_form_by_S");
        // ch(6);
        // noise_cut(6, 1, PMT_condition, false);
        // set_zoom(zoom_l, zoom_r); set_log_y();
        // set_bins(Nbins);
        // saveas(path + "6_form_by_S");
        ch(7);
        noise_cut(7, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "7_form_by_S");
        ch(8);
        noise_cut(8, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "8_form_by_S");
    }
}

int get_PMT_state(const std::string& exp) {
    if (PMT_V[exp] == 850.0)
        return 0;
    if (PMT_V[exp] == 800.0)
        return 1;
    if (PMT_V[exp] == 650.0)
        return 2;
    return -1;
}

int get_SiPM_state(const std::string& exp) {
    if (MPPC_V[exp] == 46.0)
        return 0;
    if (MPPC_V[exp] == 48.0)
        return 1;
    return -1;
}

int get_PMT_state(void) {
    auto exp = post_processor->experiments[post_processor->current_exp_index];
    return get_PMT_state(exp);
}

int get_SiPM_state(void) {
    auto exp = post_processor->experiments[post_processor->current_exp_index];
    return get_SiPM_state(exp);
}

// The following photoelectron cuts are set by hand individually, there is no other way.
void noise_cut_PMT_850V(int channel, int aggressiveness, bool display) {
    if (channel==1) {
        cut_A_S_lower(0.020, 0.0029, 0.08, 0.0047, display, channel, "rem_smallA_smallS");
        cut_A_S_lower(0.08, 0.0047, 1.6, 0.16, display, channel, "rem_largeA_smallS");
    }
    if (channel==2) {
        // cut_A_S_lower(0.020, 0.0029, 0.08, 0.0047, display, channel, "rem_smallA_smallS");
        // cut_A_S_lower(0.08, 0.0047, 1.6, 0.16, display, channel, "rem_largeA_smallS");
    }
    if (channel==3) {
        cut_A_S_lower(0.010, 0.0015, 0.08, 0.0047, display, channel, "rem_smallA_smallS");
        cut_A_S_lower(0.08, 0.0047, 1.6, 0.16, display, channel, "rem_largeA_smallS");
    }
    if (channel==4) {
        cut_A_S_lower(0.010, 0.0015, 0.08, 0.0047, display, channel, "rem_smallA_smallS");
        cut_A_S_lower(0.08, 0.0047, 1.6, 0.16, display, channel, "rem_largeA_smallS");
    }
    if (channel==5) {
        cut_A_S_lower(0.0005, 0.00042, 0.0048, 0.0, display, channel, "rem_smallA_smallS");
        x_y_regions = {0.0019, 0.0020, 0.000120, 0.03, 0.00100, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
    }
    if (channel==6) {
        // cut_A_S_lower(0.0, 0.00032, 0.010, 0.0, display, channel, "small_A-S_noise");
        // x_y_regions = {0.0019, 0.0020, 0.000120, 0.03, 0.00100, 1e3};
        // cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
    }
    if (channel==7) {
        cut_A_S_lower(0.0005, 0.00042, 0.0028, 0.0, display, channel, "rem_smallA_smallS");
        x_y_regions = {0.0013, 0.0013, 8.7e-5, 0.03, 0.00094, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
    }
    if (channel==8) {
        cut_A_S_lower(0.0005, 0.00042, 0.0028, 0.0, display, channel, "rem_smallA_smallS");
        x_y_regions = {0.0014, 0.0014, 8.5e-5, 0.03, 0.00100, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
    }
}

void noise_cut_PMT_800V(int channel, int aggressiveness, bool display) {
    if (channel==1) {
        cut_A_S_lower(0.010, 0.0020, 10, 10*0.00563/0.1, display, channel, "rem_smallA_smallS");
    }
    if (channel==2) {
        // cut_A_S_lower(0.010, 0.0020, 10, 10*0.00563/0.1, display, channel, "rem_smallA_smallS");
    }
    if (channel==3) {
        cut_A_S_lower(0.010, 0.0016, 10, 10*0.00563/0.1, display, channel, "rem_smallA_smallS");
    }
    if (channel==4) {
        cut_A_S_lower(0.010, 0.00128, 10, 10*0.0070/0.1, display, channel, "rem_smallA_smallS");
    }
    if (channel==5) {
        cut_A_S_lower(0.0014, 8.3e-5, 10, 10*0.00037/0.01, display, channel, "rem_smallA_smallS");
        cut_A_S_lower(0.0005, 0.00029, 0.0052, 0.0, display, channel, "rem_smallA_smallS2");
        cut_A_S_upper(0.0001, 1e-6, 0.0014, 1e-6, display, channel, "rem_largeA_smallS");
    }
    if (channel==6) {
        // cut_A_S_lower(0.0014, 8.3e-5, 10, 10*0.00037/0.01, display, channel, "rem_smallA_smallS");
        // cut_A_S_upper(0.0001, 1e-6, 0.0014, 1e-6, display, channel, "rem_largeA_smallS");
    }
    if (channel==7) {
        cut_A_S_lower(0.0013, 7.9e-5, 10, 10*0.00033/0.01, display, channel, "rem_smallA_smallS");
        cut_A_S_lower(0.0005, 0.00029, 0.0034, 0.0, display, channel, "rem_smallA_smallS2");
        cut_A_S_upper(0.0001, 1e-6, 0.0013, 1e-6, display, channel, "rem_largeA_smallS");
    }
    if (channel==8) {
        cut_A_S_lower(0.0013, 7.9e-5, 10, 10*0.00033/0.01, display, channel, "rem_smallA_smallS");
        cut_A_S_lower(0.0005, 0.00026, 0.0028, 0.0, display, channel, "rem_smallA_smallS2");
        cut_A_S_upper(0.0001, 1e-6, 0.0013, 1e-6, display, channel, "rem_largeA_smallS");
    }
}

void noise_cut_PMT_650V(int channel, int aggressiveness, bool display) {
    if (channel==1) {
        cut_A_S_lower(0.01, 0.0022, 0.08, 0.0050, display, channel, "rem_smallA_smallS");
    }
    if (channel==2) {
        // cut_A_S_lower(0.01, 0.0021, 0.08, 0.0050, display, channel, "rem_smallA_smallS");
    }
    if (channel==3) {
        cut_A_S_lower(0.01, 0.0022, 0.08, 0.0050, display, channel, "rem_smallA_smallS");
    }
    if (channel==4) {
        cut_A_S_lower(0.01, 0.0026, 0.08, 0.0050, display, channel, "rem_smallA_smallS");
    }
    if (channel==5) {
        cut_A_S_lower(0.0, 0.00007, 10, 10*0.0012/0.035, display, channel, "rem_smallA_smallS");
    }
    if (channel==6) {
        // cut_A_S_lower(0.0, 0.00007, 10, 10*0.0012/0.035, display, channel, "rem_smallA_smallS");
    }
    if (channel==7) {
        cut_A_S_lower(0.0, 0.000044, 10, 10*0.0012/0.035, display, channel, "rem_smallA_smallS");
    }
    if (channel==8) {
        cut_A_S_lower(0.0, 0.000044, 10, 10*0.0012/0.035, display, channel, "rem_smallA_smallS");
    }
}

void noise_cut_SiPM_46V(int channel, int aggressiveness, bool display) {
    // qewr - for fast Ctrl+F
    if (channel==32) {
        cut_A_S_lower(0.0062, 0.00121, 0.02, 0.00121, display, channel, "small_A-S_noise");
        x_y_regions = {0.0062, 0.0062, 0.00061, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0147, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0062, 0.00163, 0.0147, 0.00329, display, channel, "2pe_merged");
    }
    if (channel==33) {
        cut_A_S_lower(0.0056, 0.00101, 0.02, 0.00101, display, channel, "small_A-S_noise");
        x_y_regions = {0.0056, 0.0056, 0.00072, 0.05, 0.0092, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0127, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0056, 0.00155, 0.0127, 0.00290, display, channel, "2pe_merged");
    }
    if (channel==34) {
        cut_A_S_lower(0.0055, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
        x_y_regions = {0.0055, 0.0055, 0.00064, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0160, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0055, 0.00152, 0.0160, 0.00335, display, channel, "2pe_merged");
    }
    if (channel==35) {
        cut_A_S_lower(0.0069, 0.00115, 0.02, 0.00115, display, channel, "small_A-S_noise");
        x_y_regions = {0.0069, 0.0069, 0.00083, 0.05, 0.0092, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0069, 0.00181, 0.0155, 0.00345, display, channel, "2pe_merged");
    }
    if (channel==36) {
        cut_A_S_lower(0.0070, 0.00132, 0.02, 0.00132, display, channel, "small_A-S_noise");
        x_y_regions = {0.0070, 0.0074, 0.00084, 0.05, 0.0092, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0162, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0070, 0.00182, 0.0162, 0.00366, display, channel, "2pe_merged");
    }
    if (channel==37) {
        cut_A_S_lower(0.0059, 0.00102, 0.02, 0.00102, display, channel, "small_A-S_noise");
        x_y_regions = {0.0059, 0.0059, 0.00057, 0.05, 0.0091, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0134, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0059, 0.00162, 0.0134, 0.00288, display, channel, "2pe_merged");
    }
    if (channel==38) {
        cut_A_S_lower(0.0064, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
        x_y_regions = {0.0064, 0.0064, 0.00035, 0.05, 0.0085, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0170, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0064, 0.00161, 0.0170, 0.0037, display, channel, "2pe_merged");
    }
    if (channel==39) {
        cut_A_S_lower(0.0059, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
        x_y_regions = {0.0059, 0.0059, 0.00028, 0.05, 0.0089, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0150, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0059, 0.00164, 0.0150, 0.00333, display, channel, "2pe_merged");
    }
    if (channel==40) {
        cut_A_S_lower(0.0057, 0.00090, 0.02, 0.00090, display, channel, "small_A-S_noise");
        x_y_regions = {0.0057, 0.0057, 0.00036, 0.05, 0.0085, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0148, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0057, 0.00160, 0.0148, 0.00329, display, channel, "2pe_merged");
    }
    if (channel==41) {
        cut_A_S_lower(0.0050, 0.00066, 0.015, 0.00138, display, channel, "small_A-S_noise");
        x_y_regions = {0.0050, 0.0050, 0.00025, 0.05, 0.0080, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0146, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0050, 0.00161, 0.0146, 0.00320, display, channel, "2pe_merged");
    }
    if (channel==42) {
        cut_A_S_lower(0.0064, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
        x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0152, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0064, 0.00191, 0.0152, 0.00340, display, channel, "2pe_merged");
    }
    if (channel==43) {
        cut_A_S_lower(0.0056, 0.00083, 0.015, 0.00154, display, channel, "small_A-S_noise");
        x_y_regions = {0.0056, 0.0056, 0.00025, 0.05, 0.0086, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0152, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0056, 0.00155, 0.0152, 0.00339, display, channel, "2pe_merged");
    }
    /*if (channel==44) {
        cut_A_S_lower(0.0055, 0.00102, 0.02, 0.00102, display, channel, "small_A-S_noise");
        x_y_regions = {0.0055, 0.0055, 0.00058, 0.05, 0.0083, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0140, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0055, 0.00165, 0.0140, 0.00303, display, channel, "2pe_merged");
        //if (post_processor->isMultichannel(post_processor->current_type)) {
        //	off_ch(channel);
        //}
    }*/
    if (channel==48) {
        cut_A_S_lower(0.0063, 0.0011, 0.015, 0.00139, display, channel, "small_A-S_noise");
        x_y_regions = {0.0063, 0.0063, 0.00059, 0.05, 0.0087, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0153, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0063, 0.00163, 0.0153, 0.00326, display, channel, "2pe_merged");
    }
    if (channel==49) {
        cut_A_S_lower(0.0051, 0.00092, 0.02, 0.00129, display, channel, "small_A-S_noise");
        x_y_regions = {0.0051, 0.0051, 0.00051, 0.05, 0.0082, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0119, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0051, 0.00137, 0.0119, 0.00256, display, channel, "2pe_merged");
    }
    if (channel==50) {
        cut_A_S_lower(0.0051, 0.00072, 0.015, 0.00156, display, channel, "small_A-S_noise");
        x_y_regions = {0.0051, 0.0051, 0.00049, 0.05, 0.0084, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0121, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0051, 0.00138, 0.0121, 0.0027, display, channel, "2pe_merged");
    }
    if (channel==51) {
        cut_A_S_lower(0.0060, 0.00103, 0.02, 0.00103, display, channel, "small_A-S_noise");
        x_y_regions = {0.0060, 0.0060, 0.00050, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0146, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0060, 0.00162, 0.0146, 0.00331, display, channel, "2pe_merged");
    }
    if (channel==52) {
        cut_A_S_lower(0.0068, 0.00121, 0.02, 0.00121, display, channel, "small_A-S_noise");
        x_y_regions = {0.0068, 0.0068, 0.00066, 0.05, 0.0086, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0157, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0068, 0.00175, 0.0157, 0.00354, display, channel, "2pe_merged");
    }
    if (channel==53) {
        cut_A_S_lower(0.0072, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
        x_y_regions = {0.0072, 0.0072, 0.00058, 0.05, 0.0081, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0172, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0072, 0.00194, 0.0172, 0.00372, display, channel, "2pe_merged");
    }
    if (channel==54) {
        cut_A_S_lower(0.0058, 0.00088, 0.015, 0.00105, display, channel, "small_A-S_noise");
        x_y_regions = {0.0058, 0.0058, 0.00047, 0.05, 0.0085, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0138, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0058, 0.00143, 0.0138, 0.00297, display, channel, "2pe_merged");
    }
    if (channel==55) {
        cut_A_S_lower(0.0061, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
        x_y_regions = {0.0061, 0.0061, 0.00046, 0.05, 0.0086, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0141, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0061, 0.00164, 0.0141, 0.00309, display, channel, "2pe_merged");
    }
    if (channel==56) {
        cut_A_S_lower(0.0052, 0.00064, 0.015, 0.00130, display, channel, "small_A-S_noise");
        x_y_regions = {0.0052, 0.0052, 0.00011, 0.05, 0.0083, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0149, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0052, 0.00150, 0.0149, 0.00333, display, channel, "2pe_merged");
    }
    if (channel==57) {
        cut_A_S_lower(0.0062, 0.0010, 0.02, 0.00100, display, channel, "small_A-S_noise");
        x_y_regions = {0.0062, 0.0062, 0.00056, 0.05, 0.0083, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0154, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0062, 0.00171, 0.0154, 0.00341, display, channel, "2pe_merged");
    }
    if (channel==58) {
        cut_A_S_lower(0.0050, 0.00068, 0.015, 0.00116, display, channel, "small_A-S_noise");
        x_y_regions = {0.0050, 0.0050, 0.00011, 0.05, 0.0080, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0147, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0050, 0.00166, 0.0147, 0.00324, display, channel, "2pe_merged");
    }
    if (channel==59) {
        cut_A_S_lower(0.0054, 0.00074, 0.015, 0.00123, display, channel, "small_A-S_noise");
        x_y_regions = {0.0054, 0.0054, 0.00018, 0.05, 0.0081, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0150, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0054, 0.00158, 0.0150, 0.00327, display, channel, "2pe_merged");
    }
}

// Even if it is harder to read the whole analysis, it is better to move all A-S cuts for PMT in one
// place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
// selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during
// plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
// which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, int device_condition, bool display) {
    //aggresiveness:
    //	PMT: 0 - cut only noise, 1 - remove peaks with too large A or S
    //	SiPM: 0 - cut only noise, 1 - select only single phe peaks (including merged peaks - same A but double S)
    //		2 - select only single phe peaks without merged ones.
    if (channel < 32) {
        switch (device_condition) {
            case 0: {
                noise_cut_PMT_850V(channel, aggressiveness, display);
                break;
            }
            case 1: {
                noise_cut_PMT_800V(channel, aggressiveness, display);
                break;
            }
            case 2: {
                noise_cut_PMT_650V(channel, aggressiveness, display);
                break;
            }
        }
    } else {
        if (0 == device_condition) {
            noise_cut_SiPM_46V(channel, aggressiveness, display);
        }
    }
    
}

// Call from x_script.cpp after analysis is finished
void save_Npe_table(std::string fname) {
    std::ofstream str;
    str.open(fname, std::ios_base::trunc);
    if (!str.is_open())
        return;
    str<<"V\t";
    for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
        int chan = chs_for_Npe[ich];
        str<<chan<<"\t";
    }
    str<<std::endl;
    std::vector<std::string> ref_candidates;
    for (auto i = experiment_fields.begin(); i != experiment_fields.end(); ++i)
        if (i->second == 0)
            ref_candidates.push_back(i->first);
    if (ref_candidates.empty()) {
        std::cerr<<"Error:save_Npe_table: Could not find reference experiment (0 field). Aborting."<<std::endl;
        return;
    }
    std::vector<std::size_t> ref_indices;
    for (std::size_t c = 0; c!=ref_candidates.size(); ++c) {
        for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i)
            if (exp_area.experiments[i] == ref_candidates[c]) {
                ref_indices.push_back(i);
                break;
            }
    }
    if (ref_indices.empty()) {
        std::cerr<<"Error:save_SiPM_Npe_table: Could not find index of reference experiment (0 field). Aborting."<<std::endl;
        return;
    }
    if (ref_indices.size() > 1) {
        std::cerr<<"Error:save_SiPM_Npe_table: Found several indices of reference experiment (0 field). Aborting."<<std::endl;
        return;
    }
    std::size_t ref_ind = ref_indices[0];

    struct reference_data {
        double ref_S1;
        double ref_S2;
    };
    channel_info<reference_data> ref_data;
    for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
        int chan = chs_for_Npe[ich];
        reference_data refernce;
        Npe_in_ranges_data data = (*g_Npe_data.info(chan))[ref_ind];
        refernce.ref_S1 = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
        refernce.ref_S2 = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
        ref_data.push(chan, refernce);
    }

    for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i) {
        double V = experiment_fields[exp_area.experiments[i]];
        str<<V<<"\t";
        for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
            int chan = chs_for_Npe[ich];
            reference_data ref = *ref_data.info(chan);
            Npe_in_ranges_data data = (*g_Npe_data.info(chan))[i];
            double S1_no_noise = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
            double S2_no_noise = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
            double S2_only = S2_no_noise - ref.ref_S2 * S1_no_noise / ref.ref_S1;
            str<<S2_only<<"\t";
        }
        str<<std::endl;
    }
}

// Call from x_script.cpp after analysis is finished
void save_Npe_tables(std::string fname_bkg, std::string fname_S1, std::string fname_S2, std::string fname_S2_wo_S1_tail) {
    std::ofstream str;
    // returns whether saving was successful
    auto save_header = [] (std::ofstream &str) {
        str<<"V\t";
        for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
        	int chan = chs_for_Npe[ich];
            str<<chan<<"\t";
        }
        str<<std::endl;
        return true;
    };

    auto save_bkg = [] (std::ofstream &str) {
        for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i) {
            double V = experiment_fields[exp_area.experiments[i]];
            str<<V<<"\t";
            for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
        		int chan = chs_for_Npe[ich];
                Npe_in_ranges_data data = (*g_Npe_data.info(chan))[i];
                str<<data.Npe_pre_trigger<<"\t";
            }
            str<<std::endl;
        }
        return true;
    };

    auto save_S1 = [] (std::ofstream &str) {
        for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i) {
            double V = experiment_fields[exp_area.experiments[i]];
            str<<V<<"\t";
            for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
        		int chan = chs_for_Npe[ich];
                Npe_in_ranges_data data = (*g_Npe_data.info(chan))[i];
                double S1_only = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
                str<<S1_only<<"\t";
            }
            str<<std::endl;
        }
        return true;
    };

    auto save_S2 = [] (std::ofstream &str) {
        for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i) {
            double V = experiment_fields[exp_area.experiments[i]];
            str<<V<<"\t";
            for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
        		int chan = chs_for_Npe[ich];
                Npe_in_ranges_data data = (*g_Npe_data.info(chan))[i];
                double S2_only = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
                str<<S2_only<<"\t";
            }
            str<<std::endl;
        }
        return true;
    };

    auto save_S2_wo_S1_tail = [] (std::ofstream &str) {
        std::vector<std::size_t> ref_indices;
        for (auto i = experiment_fields.begin(); i != experiment_fields.end(); ++i)
            if (i->second == 0) {
                for (std::size_t e = 0, e_end_ = exp_area.experiments.size(); e != e_end_; ++e) {
                    if (exp_area.experiments[e] == i->first) {
                        ref_indices.push_back(e);
                        break;
                    }
                }
            }
        if (ref_indices.empty()) {
            std::cerr<<"Error:save_SiPM_Npe_table: Could not find index of reference experiment (0 field). Aborting."<<std::endl;
            return false;
        }
        if (ref_indices.size() > 1) {
            std::cerr<<"Error:save_SiPM_Npe_table: Found several indices of reference experiment (0 field). Aborting."<<std::endl;
            return false;
        }
        std::size_t ref_ind = ref_indices[0];

        struct reference_data {
            double ref_S1;
            double ref_S2;
        };
        channel_info<reference_data> ref_data;
        for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
			int chan = chs_for_Npe[ich];
            reference_data refernce;
            Npe_in_ranges_data data = (*g_Npe_data.info(chan))[ref_ind];
            refernce.ref_S1 = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
            refernce.ref_S2 = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
            ref_data.push(chan, refernce);
        }

        for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i) {
            double V = experiment_fields[exp_area.experiments[i]];
            str<<V<<"\t";
            for (std::size_t ich = 0; ich != chs_for_Npe.size(); ++ich) {
				int chan = chs_for_Npe[ich];
                reference_data ref = *ref_data.info(chan);
                Npe_in_ranges_data data = (*g_Npe_data.info(chan))[i];
                double S1_no_noise = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
                double S2_no_noise = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
                double S2_only = S2_no_noise - ref.ref_S2 * S1_no_noise / ref.ref_S1;
                str<<S2_only<<"\t";
            }
            str<<std::endl;
        }
        return true;
    };

    str.open(fname_bkg, std::ios_base::trunc);
    if (!str.is_open())
        return;
    save_header(str);
    save_bkg(str);
    str.close();

    str.open(fname_S1, std::ios_base::trunc);
    if (!str.is_open())
        return;
    save_header(str);
    save_S1(str);
    str.close();

    str.open(fname_S2, std::ios_base::trunc);
    if (!str.is_open())
        return;
    save_header(str);
    save_S2(str);
    str.close();

    str.open(fname_S2_wo_S1_tail, std::ios_base::trunc);
    if (!str.is_open())
        return;
    save_header(str);
    save_S2_wo_S1_tail(str);
    str.close();
}

void analysis_history(bool calibrate, unsigned int method = 0) {
// Created on 2024.04.24
// Analyzing S1 and S2 (gas) in pure Ar.
// Standard pulsed X-ray analysis.
// Only printing Npe for PMTs and SiPMs vs event number here.

// Setup: double-phase, X-ray with different collimators,
// 75% Electroconnect's THGEM used for cathode and THGEM0,
// Polish standard thin GEM (PL 2022 #1) is used as GEM1, all PMTs with high gain (3 old PMTs which were in use
// till ~ May 2019 and new PMTs as #4). 600 MOhm part of V0 divider is inside EL gap, covers PMT#1 and PMT#4.
// PMT#2 has no signal, probably due to a mistake during reassembly.
// Instead of acrylic (PMMA) plate before SiPMs, there was a NIR (> 715 nm) FSQ-RG715 optical filter with FR4 support frame.
// No WLS. 1.0 atm pressure of argon.
// Setup was tilted by sin(angle) = 8mm/330mm => 1.4 degrees to avoid gas pocket under GEM1 for single-phase data.

data_output_path = "240215/results_v3/";
calibration_file = "240215/results_v1/240215_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
std::map<std::string, std::pair<double, double> > S1_times;
std::map<std::string, std::pair<double, double> > Bkg_times;
for (int e = 0; e != exp_area.experiments.size(); ++e) {
    std::string exp = exp_area.experiments[e];
    S1_times[exp] = std::pair<double, double> (31.6, 42);
    S2_times[exp] = std::pair<double, double> (42, 160);
    Bkg_times[exp] = std::pair<double, double> (2, 31.6);
}

std::map<std::string, std::string> exp_folders;
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_20kV_800V_46V"] = "X-ray_14mm_coll_filt3_20kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_18kV_800V_46V"] = "X-ray_14mm_coll_filt3_18kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_16kV_800V_46V"] = "X-ray_14mm_coll_filt3_16kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_14kV_800V_46V"] = "X-ray_14mm_coll_filt3_14kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_12kV_800V_46V"] = "X-ray_14mm_coll_filt3_12kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_10kV_800V_46V"] = "X-ray_14mm_coll_filt3_10kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_8kV_850V_46V"] = "X-ray_14mm_coll_filt3_08kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_filt3_0kV_850V_46V"] = "X-ray_14mm_coll_filt3_00kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_20kV_650V_46V"] = "X-ray_14mm_coll_20kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_18kV_650V_46V"] = "X-ray_14mm_coll_18kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_16kV_650V_46V"] = "X-ray_14mm_coll_16kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_14kV_650V_46V"] = "X-ray_14mm_coll_14kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_12kV_650V_46V"] = "X-ray_14mm_coll_12kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_10kV_650V_46V"] = "X-ray_14mm_coll_10kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_8kV_650V_46V"] = "X-ray_14mm_coll_08kV";
exp_folders["240215_Ar_2ph_X-ray_14mm_coll_0kV_650V_46V"] = "X-ray_14mm_coll_00kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_20kV_800V_46V"] = "X-ray_6mm_coll_20kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_18kV_800V_46V"] = "X-ray_6mm_coll_18kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_16kV_800V_46V"] = "X-ray_6mm_coll_16kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_14kV_850V_46V"] = "X-ray_6mm_coll_14kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_12kV_850V_46V"] = "X-ray_6mm_coll_12kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_10kV_850V_46V"] = "X-ray_6mm_coll_10kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_8kV_850V_46V"] = "X-ray_6mm_coll_08kV";
exp_folders["240215_Ar_2ph_X-ray_6mm_coll_0kV_850V_46V"] = "X-ray_6mm_coll_00kV";

bool forms = !calibrate;
if (calibrate) {
	std::cout<<"Use 1analysis_history.cpp for calibration! Doing nothing."<<std::endl;
	return;
}
//END OF CALIBRATION

//SIGNAL FORMS
if (!forms)
    return;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
double d_S2_start = S2_times.at(exp).first, d_S2_finish = S2_times.at(exp).second;
double d_S1_start = S1_times.at(exp).first, d_S1_finish = S1_times.at(exp).second;
double d_Bkg_start = Bkg_times.at(exp).first, d_Bkg_finish = Bkg_times.at(exp).second;
std::string folder = exp_folders.at(exp),
			S2_start = dbl_to_str(d_S2_start, 1),
			S2_finish = dbl_to_str(d_S2_finish, 1),
			S1_start = dbl_to_str(d_S1_start, 1),
			S1_finish = dbl_to_str(d_S1_finish, 1),
			Bkg_start = dbl_to_str(d_Bkg_start, 1),
			Bkg_finish = dbl_to_str(d_Bkg_finish, 1);
int first_run = experiment_runs.at(exp);
int PMT_state = get_PMT_state();
int SiPM_state = get_SiPM_state();
std::vector<std::string> cuts; // List of cuts in terms of figure numbers where cut is shown
int no = 0; // Figure number
std::string Num = int_to_str(++no, 2); // = "01" // Figure number as string
std::string FOLDER = data_output_path + folder + "/";

auto do_PMT_Npe_figures = [&] (unsigned int max_Npe) {
    // This comment is for working in interpreter
    // int max_Npe = 500;
    ty(AStates::PMT_Npe_sum);
    slow_PMTs_only();
    cut_PMT_noise(PMT_state);
    if (max_Npe > 1500) {
        set_zoom(0, max_Npe);
        set_bins(max_Npe/2);
    } else {
        set_bins(0, max_Npe);
    }
    saveaspng(FOLDER + Num+"_sPMTs_Npe_"+cuts_str(cuts)+"_0");
    set_log_y();
    saveaspng(FOLDER + Num+"_sPMTs_Npe_"+cuts_str(cuts)+"_1_log");
    unset_log_y();
    Num = int_to_str(++no, 2);
    return;
};

auto do_SiPM_Npe_figures = [&] (unsigned int max_Npe) {
    // This comment is for working in interpreter
    // int max_Npe = 400;
    ty(AStates::MPPC_Npe_sum);
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        noise_cut(chan, 0, SiPM_state, false);
    }
    if (max_Npe > 1500) {
        set_zoom(0, max_Npe);
        set_bins(max_Npe/2);
    } else {
        set_bins(0, max_Npe);
    }
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_0");
    set_log_y();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_1_log");
    unset_log_y();
    Num = int_to_str(++no, 2);
    return;
};

// Can not calibrate for 650V at PMTs, so plotting peak areas instead.
auto do_PMT_S_figures = [&] (double max_S, unsigned int Npe) {
    // This comment is for working in interpreter
    // double max_S = 5; \
    int Npe = 800;
    ty(AStates::PMT_S_sum);
    slow_PMTs_only();
    cut_PMT_noise(PMT_state);
    set_zoom(0, max_S);
    set_bins(Npe);
    saveaspng(FOLDER + Num+"_sPMTs_S_"+cuts_str(cuts)+"_0");
    set_log_y();
    saveaspng(FOLDER + Num+"_sPMTs_S_"+cuts_str(cuts)+"_1_log");
    unset_log_y();
    Num = int_to_str(++no, 2);
    return;
};

// This functions assumes that do_PMT_Npe_figures and do_SiPM_Npe_figures were called beforehand
auto do_SiPM_vs_PMT_Npe_figures = [&] (const std::pair<unsigned int, unsigned int> &max_Npe,
                                       const std::pair<unsigned int, unsigned int> &N_bins,
                                       const std::vector<double> &x_y_select_region) {
    // This comment is for working in interpreter
    // std::pair<unsigned int, unsigned int> max_Npe(100, 100); \
    std::pair<unsigned int, unsigned int> N_bins(0, 0); \
    std::vector<double> x_y_select_region = \
    {0, 0, 24, 0, 24, 3, 6.3, 10, 0, 10};
    set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0, max_Npe.first, 0, max_Npe.second);
    set_bins(N_bins.first ? N_bins.first : max_Npe.first, N_bins.second ? N_bins.second : max_Npe.second);
    cut_x_y_poly_select(x_y_select_region, true, "1");
    update();

    set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
    saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_0");
    set_log_z();
    saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_1_log");
    unset_log_z();
    
    set_as_run_cut("no_discharge"); cuts.push_back(Num);
    Num = int_to_str(++no, 2);
    return;
};

// This functions assumes that do_PMT_Npe_figures and do_SiPM_Npe_figures were called beforehand
auto do_SiPM_vs_PMT_Npe_figures_log = [&] (const std::pair<unsigned int, unsigned int> &max_Npe,
                                       const std::pair<unsigned int, unsigned int> &N_bins,
                                       const std::vector<double> &x_y_select_region) {
    // This comment is for working in interpreter
    // std::pair<unsigned int, unsigned int> max_Npe(10000, 1000); \
    std::pair<unsigned int, unsigned int> N_bins(500, 200); \
    std::vector<double> x_y_select_region = \
    {640, 65, 1094, 29, 1466, 96, 996, 132};
    set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0.9, max_Npe.first, 0.9, max_Npe.second);
    set_log_bins_xy(true, false);
    set_log_x();
    set_log_y();
    set_bins(N_bins.first ? N_bins.first : max_Npe.first, N_bins.second ? N_bins.second : max_Npe.second);
    set_hist_stats_pos("tl");

    cut_x_y_poly_select(x_y_select_region, true, "1");
    update();
    set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
    saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_0");
    set_log_z();
    saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_1_log");
    unset_log_z();

    set_hist_stats_pos("tr");
    unset_log_bins_xy(false);
    unset_log_x();
    unset_log_y();
    Num = int_to_str(++no, 2);
    return;
};

// This functions assumes that do_SiPM_Npe_figures was called beforehand
auto do_SiPM_Npe_vs_event_figure = [&] (unsigned int max_Npe, unsigned int N_bins) {
    // unsigned int max_Npe = 20000; \
    unsigned int N_bins = 500;
    set_corr(AStates::EventNumber, AStates::MPPC_Npe_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0, post_processor->events_number(), 0.1, max_Npe);
    set_bins(post_processor->events_number()/100, N_bins);
    unset_hist_stats();
    set_log_y();
    set_log_bins_y();
    update();

    set_titles("Event number", "N_{PE} SiPMs");
    saveaspng(FOLDER + Num + "_SiPMs_Npe_vs_time_"+cuts_str(cuts));
    unset_log_y();
    unset_log_bins_y();
    set_hist_stats("tr");
    Num = int_to_str(++no, 2);
    return;
};

// This functions assumes that do_PMT_Npe_figures was called beforehand
auto do_PMT_Npe_vs_event_figure = [&] (unsigned int max_Npe, unsigned int N_bins) {
    // unsigned int max_Npe = 5000; \
    unsigned int N_bins = 250;
    set_corr(AStates::EventNumber, AStates::PMT_Npe_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0, post_processor->events_number(), 1, max_Npe);
    set_bins(post_processor->events_number()/100, N_bins);
    unset_hist_stats();
    set_log_y();
    set_log_bins_y();
    update();
    set_titles("Event number", "N_{PE} PMTs");
    saveaspng(FOLDER + Num + "_slowPMTs_Npe_vs_time_"+cuts_str(cuts));
    
    unset_log_y();
    unset_log_bins_y();
    set_hist_stats("tr");
    Num = int_to_str(++no, 2);
    return;
};

// This functions assumes that do_PMT_S_figures was called beforehand
auto do_PMT_S_vs_event_figure = [&] (double min_S, double max_S, unsigned int N_bins) {
    // unsigned int max_Npe = 5000; \
    double min_S = 0.1; \
    double max_S = 500;
    set_corr(AStates::EventNumber, AStates::PMT_S_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0, post_processor->events_number(), min_S, max_S);
    set_bins(post_processor->events_number()/100, N_bins);
    unset_hist_stats();
    set_log_y();
    set_log_bins_y();
    update();
    set_titles("Event number", "S PMTs");
    saveaspng(FOLDER + Num + "_slowPMTs_S_vs_time_"+cuts_str(cuts));
    
    unset_log_y();
    unset_log_bins_y();
    set_hist_stats("tr");
    Num = int_to_str(++no, 2);
    return;
};

auto do_SiPM_Npe_Bkg_S1_S2_figures = [&] (unsigned int max_Npe_Bkg,
                                          unsigned int max_Npe_S1,
                                          unsigned int max_Npe_S2) {
    ty(AStates::MPPC_Npe_sum);
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        on_ch(chan);
        noise_cut(chan, 0, SiPM_state, false);
    }
    set_bins(0, max_Npe_Bkg);
    time_zoom_SiPMs(d_Bkg_start, d_Bkg_finish); update();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+Bkg_start+"-"+Bkg_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    set_bins(0, max_Npe_S1);
    time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    set_bins(0, max_Npe_S2);
    time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-"+S2_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);
    return;
};

auto do_PMT_Npe_Bkg_S1_S2_figures = [&] (unsigned int max_Npe_Bkg,
                                         unsigned int max_Npe_S1,
                                         unsigned int max_Npe_S2) {
    ty(AStates::PMT_Npe_sum);
    slow_PMTs_only();
    cut_PMT_noise(PMT_state);

    set_bins(0, max_Npe_Bkg);
    time_zoom_PMTs(d_Bkg_start, d_Bkg_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+Bkg_start+"-"+Bkg_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    set_bins(0, max_Npe_S1);
    time_zoom_PMTs(d_S1_start, d_S1_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    set_bins(0, max_Npe_S2);
    time_zoom_PMTs(d_S2_start, d_S2_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-"+S2_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);
    return;
};

// Assumes that photoelectron noise cuts were applied beforehand
auto do_SiPM_Npe_tables = [&] (unsigned int max_Npe_SiPM = 100u) {
    ty(AStates::MPPC_Npe_sum);
    set_bins(0, max_Npe_SiPM);
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        ch(chan);
        cut_t(d_Bkg_start, d_Bkg_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_pre_trigger = std::make_pair(d_Bkg_start, d_Bkg_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_pre_trigger = get_mean();
        cut_t(d_S1_start, d_S1_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S1 = std::make_pair(d_S1_start, d_S1_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S1 = get_mean();
        cut_t(d_S2_start, d_S2_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S2 = std::make_pair(d_S2_start, d_S2_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S2 = get_mean();
    }
};

// Assumes that photoelectron noise cuts were applied beforehand
auto do_PMT_Npe_tables = [&] (unsigned int max_Npe_PMT = 100u) {
    ty(AStates::PMT_Npe_sum);
    set_bins(0, max_Npe_PMT);
    for (int ich = 0; ich!= post_processor->PMT_channels.size(); ++ich) {
        int chan = post_processor->PMT_channels[ich];
        if (chan < 1)
            continue;
        ch(chan);
        cut_t(d_Bkg_start, d_Bkg_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_pre_trigger = std::make_pair(d_Bkg_start, d_Bkg_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_pre_trigger = get_mean();
        cut_t(d_S1_start, d_S1_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S1 = std::make_pair(d_S1_start, d_S1_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S1 = get_mean();
        cut_t(d_S2_start, d_S2_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S2 = std::make_pair(d_S2_start, d_S2_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S2 = get_mean();
    }
};

// Assumes that photoelectron noise cuts were applied beforehand
auto do_SiPM_PMT_Npe_tables = [&] (unsigned int max_Npe_SiPM = 100u,
                                   unsigned int max_Npe_PMT = 100u) {
    do_SiPM_Npe_tables(max_Npe_SiPM);
    do_PMT_Npe_tables(max_Npe_PMT);
};

//zcxv
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_20kV_800V_46V") {
    do_PMT_Npe_figures(2000u);
    do_SiPM_Npe_figures(200u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_18kV_800V_46V") {
    do_PMT_Npe_figures(2000u);
    do_SiPM_Npe_figures(200u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_16kV_800V_46V") {
    do_PMT_Npe_figures(2000u);
    do_SiPM_Npe_figures(150u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_14kV_800V_46V") {
    do_PMT_Npe_figures(1500u);
    do_SiPM_Npe_figures(150u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_12kV_800V_46V") {
    do_PMT_Npe_figures(1200u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_10kV_800V_46V") {
    do_PMT_Npe_figures(1000u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_8kV_850V_46V") {
    do_PMT_Npe_figures(1000u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_filt3_0kV_850V_46V") {
    do_PMT_Npe_figures(100u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_20kV_800V_46V") {
    do_PMT_Npe_figures(1500u);
    do_SiPM_Npe_figures(200u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_18kV_800V_46V") {
    do_PMT_Npe_figures(1500u);
    do_SiPM_Npe_figures(200u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_16kV_800V_46V") {
    do_PMT_Npe_figures(1200u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_14kV_850V_46V") {
    do_PMT_Npe_figures(1200u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_12kV_850V_46V") {
    do_PMT_Npe_figures(1000u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_10kV_850V_46V") {
    do_PMT_Npe_figures(800u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_8kV_850V_46V") {
    do_PMT_Npe_figures(500u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_6mm_coll_0kV_850V_46V") {
    do_PMT_Npe_figures(100u);
    do_SiPM_Npe_figures(100u);

    do_PMT_Npe_vs_event_figure(5000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}

// This functions assumes that do_PMT_Npe_figures and do_SiPM_Npe_figures were called beforehand
auto do_SiPM_Npe_vs_PMT_S_figures = [&] (const std::pair<double, unsigned int> &max_vals,
                                       const std::pair<unsigned int, unsigned int> &N_bins,
                                       const std::vector<double> &x_y_select_region) {
    // This comment is for working in interpreter
    // std::pair<double, unsigned int> max_vals(5, 400); \
    std::pair<unsigned int, unsigned int> N_bins(400, 200); \
    std::vector<double> x_y_select_region = \
    {2.02, 159, 3.42, 131, 3.76, 301, 2.39, 324};
    set_corr(AStates::PMT_S_sum, AStates::MPPC_Npe_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0, max_vals.first, 0, max_vals.second);
    set_bins(N_bins.first, N_bins.second ? N_bins.second : max_vals.second);
    cut_x_y_poly_select(x_y_select_region, true, "1");
    update();

    set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
    saveaspng(FOLDER + Num + "_SiPMs_Npe_vs_slowPMTs_S_"+cuts_str(cuts)+"_0");
    set_log_z();
    saveaspng(FOLDER + Num + "_SiPMs_Npe_vs_slowPMTs_S_"+cuts_str(cuts)+"_1_log");
    unset_log_z();
    
    set_as_run_cut("no_discharge"); cuts.push_back(Num);
    Num = int_to_str(++no, 2);
    return;
};

// This functions assumes that do_PMT_Npe_figures and do_SiPM_Npe_figures were called beforehand
auto do_SiPM_Npe_vs_PMT_S_figures_log = [&] (const std::pair<double, unsigned int> &max_vals,
                                       const std::pair<unsigned int, unsigned int> &N_bins,
                                       const std::vector<double> &x_y_select_region) {
    // This comment is for working in interpreter
    // std::pair<unsigned int, unsigned int> max_vals(100.0, 10000); \
    std::pair<unsigned int, unsigned int> N_bins(500, 1000); \
    std::vector<double> x_y_select_region = \
    {640, 65, 1094, 29, 1466, 96, 996, 132};
    set_corr(AStates::PMT_S_sum, AStates::MPPC_Npe_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0.0001, max_vals.first, 0.9, max_vals.second);
    set_log_bins_xy(true, false);
    set_log_x();
    set_log_y();
    set_bins(N_bins.first, N_bins.second ? N_bins.second : max_vals.second);
    set_hist_stats_pos("tl");

    cut_x_y_poly_select(x_y_select_region, true, "1");
    update();
    set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
    saveaspng(FOLDER + Num + "_SiPMs_Npe_vs_slowPMTs_S_"+cuts_str(cuts)+"_0");
    set_log_z();
    saveaspng(FOLDER + Num + "_SiPMs_Npe_vs_slowPMTs_S_"+cuts_str(cuts)+"_1_log");
    unset_log_z();

    set_hist_stats_pos("tr");
    unset_log_bins_xy(false);
    unset_log_x();
    unset_log_y();
    Num = int_to_str(++no, 2);
    return;
};

auto do_PMT_S_Bkg_S1_S2_figures = [&] (const std::pair<double, unsigned int> &Smax_Nbins_Bkg,
                                       const std::pair<double, unsigned int> &Smax_Nbins_S1,
                                       const std::pair<double, unsigned int> &Smax_Nbins_S2) {
    ty(AStates::PMT_S_sum);
    slow_PMTs_only();
    cut_PMT_noise(PMT_state);

    set_zoom(0.0, Smax_Nbins_Bkg.first);
    set_bins(Smax_Nbins_Bkg.second);
    time_zoom_PMTs(d_Bkg_start, d_Bkg_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_S_"+Bkg_start+"-"+Bkg_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    set_zoom(0.0, Smax_Nbins_S1.first);
    set_bins(Smax_Nbins_S1.second);
    time_zoom_PMTs(d_S1_start, d_S1_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_S_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    set_zoom(0.0, Smax_Nbins_S2.first);
    set_bins(Smax_Nbins_S2.second);
    time_zoom_PMTs(d_S2_start, d_S2_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_S_"+S2_start+"-"+S2_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);
    return;
};

// Assumes that photoelectron noise cuts were applied beforehand
auto do_PMT_S_tables = [&] (double max_S_PMT = 100u, unsigned int Nbins = 500u) {
    ty(AStates::PMT_S_sum);
    set_zoom(0, max_S_PMT);
    set_bins(Nbins);
    for (int ich = 0; ich!= post_processor->PMT_channels.size(); ++ich) {
        int chan = post_processor->PMT_channels[ich];
        if (chan < 1)
            continue;
        ch(chan);
        cut_t(d_Bkg_start, d_Bkg_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_pre_trigger = std::make_pair(d_Bkg_start, d_Bkg_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_pre_trigger = get_mean();
        cut_t(d_S1_start, d_S1_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S1 = std::make_pair(d_S1_start, d_S1_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S1 = get_mean();
        cut_t(d_S2_start, d_S2_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S2 = std::make_pair(d_S2_start, d_S2_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S2 = get_mean();
    }
};

// Assumes that photoelectron noise cuts were applied beforehand
auto do_SiPM_Npe_PMT_S_tables = [&] (unsigned int max_Npe_SiPM = 100u,
                                     double max_S_PMT = 100u, unsigned int Nbins = 500u) {
    do_SiPM_Npe_tables(max_Npe_SiPM);
    do_PMT_S_tables(max_S_PMT, Nbins);
};
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_20kV_650V_46V") {
    do_PMT_S_figures(15, 800u);
    do_SiPM_Npe_figures(2000u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_18kV_650V_46V") {
    do_PMT_S_figures(15, 800u);
    do_SiPM_Npe_figures(1500u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_16kV_650V_46V") {
    do_PMT_S_figures(15, 800u);
    do_SiPM_Npe_figures(1000u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_14kV_650V_46V") {
    do_PMT_S_figures(12, 800u);
    do_SiPM_Npe_figures(800u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_12kV_650V_46V") {
    do_PMT_S_figures(10, 800u);
    do_SiPM_Npe_figures(800u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_10kV_650V_46V") {
    do_PMT_S_figures(8, 800u);
    do_SiPM_Npe_figures(600u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_8kV_650V_46V") {
    do_PMT_S_figures(5, 800u);
    do_SiPM_Npe_figures(400u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}
if (exp == "240215_Ar_2ph_X-ray_14mm_coll_0kV_650V_46V") {
    do_PMT_S_figures(0.1, 500u);
    do_SiPM_Npe_figures(100u);

    do_PMT_S_vs_event_figure(0.1, 1000, 500);
    do_SiPM_Npe_vs_event_figure(5000, 500);
}

//END OF FORMS
}
