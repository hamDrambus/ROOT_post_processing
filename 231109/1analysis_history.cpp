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
    cut_t(f_time, to_time, false, 0);
    cut_t(f_time, to_time, false, 1);
    cut_t(f_time, to_time, false, 2);
    cut_t(f_time, to_time, false, 3);
    cut_t(f_time, to_time, false, 4);

    cut_t(f_time, to_time, false, 5);
    cut_t(f_time, to_time, false, 6);
    cut_t(f_time, to_time, false, 7);
    cut_t(f_time, to_time, false, 8);
}

void time_zoom_PMTs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e) {
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 0);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 1);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 2);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 3);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 4);

    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 5);
    cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 6);
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

void fast_PMTs_only (void) {
    on_ch(5); on_ch(6); on_ch(7); on_ch(8);
    off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
}

void slow_PMTs_only (void) {
    on_ch(1); on_ch(2); on_ch(3); on_ch(4);
    off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
}

void cut_PMT_noise(int device_condition)
{
    noise_cut(1, 0, device_condition, 0);
    noise_cut(2, 0, device_condition, 0);
    noise_cut(3, 0, device_condition, 0);
    noise_cut(4, 0, device_condition, 0);

    noise_cut(5, 0, device_condition, 0);
    noise_cut(6, 0, device_condition, 0);
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

void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition)
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
    ch(6);
    noise_cut(6, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "6_form_by_Npeaks");
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
    ch(2);
    noise_cut(2, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "2_form_by_Npe");
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
    ch(6);
    noise_cut(6, 1, PMT_condition, false);
    set_zoom(zoom_l, zoom_r); set_log_y();
    set_bins(Nbins);
    saveas(path + "6_form_by_Npe");
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
        ch(2);
        noise_cut(2, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "2_form_by_S");
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
        ch(6);
        noise_cut(6, 1, PMT_condition, false);
        set_zoom(zoom_l, zoom_r); set_log_y();
        set_bins(Nbins);
        saveas(path + "6_form_by_S");
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

//Even it is harder to read the whole analysis, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, int device_condition, bool display) {
    //aggresiveness:
    //	PMT: 0 - cut only noise, 1 - remove peaks with too large A or S
    //	SiPM: 0 - cut only noise, 1 - select only single phe peaks (including merged peaks - same A but double S)
    //		2 - select only single phe peaks without merged ones.
    //device_condition: (if out of range 0 is used)
    //	PMT: 0 - 850V, 12dB
    //	SiPM: 0 - 46V, 1 - 48V
    if (0 == device_condition) {
        if (channel==1) {
            //cut_A_S_upper(0.005, 0.0, 0.0132, 0.0, display, channel, "rem_smallA_largeS_1");
            cut_A_S_lower(0.020, 0.0038, 0.06, 0.0087, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.06, 0.0087, 1.4, 0.33, display, channel, "rem_largeA_smallS");
        }
        if (channel==2) {
            //cut_A_S_upper(0.005, 0.0, 0.018, 0.0, display, channel, "rem_smallA_largeS_1");
            cut_A_S_lower(0.0122, 0.0027, 0.040, 0.0047, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.040, 0.0047, 1.6, 0.37, display, channel, "rem_largeA_smallS");
        }
        if (channel==3) {
            //cut_A_S_upper(0.005, 0.0, 0.0139, 0.0, display, channel, "rem_smallA_largeS");
            cut_A_S_lower(0.01, 0.0029, 0.030, 0.0038, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.030, 0.0038, 1.2, 0.26, display, channel, "rem_largeA_smallS");
        }
        if (channel==4) {
            //cut_A_S_upper(0.01, 0.0, 0.0171, 0.0, display, channel, "rem_smallA_largeS_1");
            cut_A_S_lower(0.01, 0.0024, 0.030, 0.0037, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.030, 0.0037, 1.6, 0.32, display, channel, "rem_largeA_smallS");
        }
        if (channel==5) {
            //cut_A_S_upper(0.001, 0.0, 0.0084, 0.0, display, channel, "rem_smallA-S");
            if (aggressiveness == 1) {
                cut_A_S_upper(0.65, 0, 5, 0, display, channel, "rem_A>0.65");
                cut_A_S_upper(0.0, 0.030, 1.6, 0.030, display, channel, "rem_S>0.030");
            }
        }
        if (channel==6) {
            cut_A_S_upper(0.001, 0.0, 0.00503, 0.0, display, channel, "rem_smallA-S");
            if (aggressiveness == 1) {
                cut_A_S_upper(0.50, 0, 5, 0, display, channel, "rem_A>0.50");
                cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
            }
        }
        if (channel==7) {
            //cut_A_S_upper(0.001, 0.0, 0.0060, 0.0, display, channel, "rem_smallA-S");
            if (aggressiveness == 1) {
                cut_A_S_upper(0.54, 0, 2, 0, display, channel, "rem_A>0.54");
                cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
            }
        }
        if (channel==8) {
            cut_A_S_upper(0.001, 0.0, 0.00503, 0.0, display, channel, "rem_smallA-S");
            if (aggressiveness == 1) {
                cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
                cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
            }
        }
    }
    //qewr
    //The following are set by hand individually, there is no other way.
    if (channel==32) {
        cut_A_S_lower(0.0062, 0.00121, 0.02, 0.00121, display, channel, "small_A-S_noise");
        x_y_regions = {0.0062, 0.0062, 0.00061, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0152, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0062, 0.00163, 0.0152, 0.00342, display, channel, "2pe_merged");
    }
    if (channel==33) {
        cut_A_S_lower(0.0056, 0.00101, 0.02, 0.00101, display, channel, "small_A-S_noise");
        x_y_regions = {0.0056, 0.0056, 0.00072, 0.05, 0.0092, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0127, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0056, 0.00155, 0.0127, 0.00290, display, channel, "2pe_merged");
    }
    if (channel==34) {
        cut_A_S_lower(0.0055, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
        x_y_regions = {0.0055, 0.0055, 0.00064, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0125, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0055, 0.00152, 0.0125, 0.00280, display, channel, "2pe_merged");
    }
    if (channel==35) {
        cut_A_S_lower(0.0069, 0.00115, 0.02, 0.00115, display, channel, "small_A-S_noise");
        x_y_regions = {0.0069, 0.0069, 0.00083, 0.05, 0.0092, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0069, 0.00181, 0.0155, 0.00345, display, channel, "2pe_merged");
    }
    if (channel==36) {
        cut_A_S_lower(0.0074, 0.00132, 0.02, 0.00132, display, channel, "small_A-S_noise");
        x_y_regions = {0.0074, 0.0074, 0.00084, 0.05, 0.0092, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0162, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0074, 0.00191, 0.0162, 0.00366, display, channel, "2pe_merged");
    }
    if (channel==37) {
        cut_A_S_lower(0.0059, 0.00102, 0.02, 0.00102, display, channel, "small_A-S_noise");
        x_y_regions = {0.0059, 0.0059, 0.00076, 0.05, 0.0094, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0141, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0059, 0.00162, 0.0141, 0.00311, display, channel, "2pe_merged");
    }
    if (channel==38) {
        cut_A_S_lower(0.0064, 0.00116, 0.02, 0.00116, display, channel, "small_A-S_noise");
        x_y_regions = {0.0064, 0.0064, 0.00074, 0.05, 0.0088, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0141, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0064, 0.00161, 0.0141, 0.00316, display, channel, "2pe_merged");
    }
    if (channel==39) {
        cut_A_S_lower(0.0062, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
        x_y_regions = {0.0062, 0.0066, 0.00060, 0.05, 0.0091, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0150, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0062, 0.00179, 0.0150, 0.00333, display, channel, "2pe_merged");
    }
    if (channel==40) {
        cut_A_S_lower(0.0066, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
        x_y_regions = {0.0066, 0.0066, 0.00052, 0.05, 0.0089, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0154, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0066, 0.00191, 0.0154, 0.00358, display, channel, "2pe_merged");
    }
    if (channel==41) {
        cut_A_S_lower(0.0064, 0.00089, 0.02, 0.00089, display, channel, "small_A-S_noise");
        x_y_regions = {0.0064, 0.0064, 0.00055, 0.05, 0.0089, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0153, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0064, 0.00191, 0.0153, 0.00337, display, channel, "2pe_merged");
    }
    if (channel==42) {
        cut_A_S_lower(0.0064, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
        x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0152, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0064, 0.00191, 0.0152, 0.00340, display, channel, "2pe_merged");
    }
    if (channel==43) {
        cut_A_S_lower(0.0064, 0.00106, 0.02, 0.00106, display, channel, "small_A-S_noise");
        x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0064, 0.00191, 0.0155, 0.00353, display, channel, "2pe_merged");
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
        cut_A_S_lower(0.0067, 0.00123, 0.02, 0.00123, display, channel, "small_A-S_noise");
        x_y_regions = {0.0067, 0.0067, 0.00084, 0.05, 0.0089, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0067, 0.00163, 0.0145, 0.00308, display, channel, "2pe_merged");
    }
    if (channel==49) {
        cut_A_S_lower(0.0055, 0.00107, 0.02, 0.00162, display, channel, "small_A-S_noise");
        x_y_regions = {0.0055, 0.0055, 0.00066, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0121, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0055, 0.00146, 0.0121, 0.00275, display, channel, "2pe_merged");
    }
    if (channel==50) {
        cut_A_S_lower(0.0072, 0.00128, 0.02, 0.00128, display, channel, "small_A-S_noise");
        x_y_regions = {0.0072, 0.0072, 0.00095, 0.05, 0.0089, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0072, 0.00181, 0.0159, 0.00343, display, channel, "2pe_merged");
    }
    if (channel==51) {
        cut_A_S_lower(0.0060, 0.00113, 0.02, 0.00113, display, channel, "small_A-S_noise");
        x_y_regions = {0.0060, 0.0060, 0.00050, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0146, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0060, 0.00162, 0.0146, 0.00331, display, channel, "2pe_merged");
    }
    if (channel==52) {
        cut_A_S_lower(0.0071, 0.00121, 0.02, 0.00121, display, channel, "small_A-S_noise");
        x_y_regions = {0.0071, 0.0071, 0.00081, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0164, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0071, 0.00186, 0.0164, 0.00371, display, channel, "2pe_merged");
    }
    if (channel==53) {
        cut_A_S_lower(0.0072, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
        x_y_regions = {0.0072, 0.0072, 0.00058, 0.05, 0.0081, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0172, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0072, 0.00194, 0.0172, 0.00372, display, channel, "2pe_merged");
    }
    if (channel==54) {
        cut_A_S_lower(0.0078, 0.00131, 0.02, 0.00131, display, channel, "small_A-S_noise");
        x_y_regions = {0.0078, 0.0078, 0.00100, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0168, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0078, 0.00200, 0.0168, 0.00371, display, channel, "2pe_merged");
    }
    if (channel==55) {
        cut_A_S_lower(0.0061, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
        x_y_regions = {0.0061, 0.0061, 0.00066, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0061, 0.00164, 0.0145, 0.00320, display, channel, "2pe_merged");
    }
    if (channel==56) {
        cut_A_S_lower(0.0065, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
        x_y_regions = {0.0065, 0.0065, 0.00047, 0.05, 0.0088, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0153, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0065, 0.00181, 0.0153, 0.00342, display, channel, "2pe_merged");
    }
    if (channel==57) {
        cut_A_S_lower(0.0069, 0.00120, 0.02, 0.00120, display, channel, "small_A-S_noise");
        x_y_regions = {0.0069, 0.0069, 0.00066, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0069, 0.00183, 0.0159, 0.00348, display, channel, "2pe_merged");
    }
    if (channel==58) {
        cut_A_S_lower(0.0066, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
        x_y_regions = {0.0066, 0.0066, 0.00058, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0153, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0066, 0.00180, 0.0153, 0.00333, display, channel, "2pe_merged");
    }
    if (channel==59) {
        cut_A_S_lower(0.0067, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
        x_y_regions = {0.0067, 0.0067, 0.00057, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0157, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0067, 0.00180, 0.0157, 0.00340, display, channel, "2pe_merged");
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
// Created on 2023.11.16
// Analyzing only S1 in LAr + N2(~840 ppm). Analyzing S1+S2(gas) is in another file.
// Standard pulsed X-ray analysis. Some event selection to avoid events with discharge/turned off SiPMs or events with gas bubbles.
// Not using trigger adjustement or trigger-related cuts.
// Output is signal pulse-shapes and Npe for pre-trigger, S1 and potential S2 due to bubbles.

// Setup: double- and single-phase, X-ray with different collimators,
// 75% Electroconnect's THGEM used for cathode and THGEM0,
// Polish standard thin GEM (PL 2022 #1) is used as GEM1, all PMTs with high gain (3 old PMTs which were in use
// till ~ May 2019 and new PMTs as #4). 600 MOhm part of V0 divider is inside EL gap, covers PMT#1 and PMT#2(?).
// No WLS. 1.0 atm pressures of argon in EL gap.
// Setup was tilted by sin(angle) = 8mm/330mm => 1.4 degrees to avoid gas pocket under GEM1 for single-phase data
// (for 14mm and 14mm+filter#3 collimators only).

data_output_path = "231109/results_v1/";
calibration_file = "231109/results_v1/231109_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
std::map<std::string, std::pair<double, double> > S1_times;
std::map<std::string, std::pair<double, double> > Bkg_times;
for (int e = 0; e != exp_area.experiments.size(); ++e) {
    std::string exp = exp_area.experiments[e];
    S1_times[exp] = std::pair<double, double> (31.6, 44);
    S2_times[exp] = std::pair<double, double> (44, 85);
    Bkg_times[exp] = std::pair<double, double> (2, 31.6);
}

std::map<std::string, std::string> exp_folders;
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V"] = "X-ray_14mm_coll_filt3_20kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V"] = "X-ray_14mm_coll_filt3_18kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V"] = "X-ray_14mm_coll_filt3_16kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V"] = "X-ray_14mm_coll_filt3_14kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V"] = "X-ray_14mm_coll_filt3_12kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V"] = "X-ray_14mm_coll_filt3_10kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V"] = "X-ray_14mm_coll_filt3_08kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V"] = "X-ray_14mm_coll_filt3_00kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_20kV_850V_46V"] = "X-ray_14mm_coll_20kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_18kV_850V_46V"] = "X-ray_14mm_coll_18kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_16kV_850V_46V"] = "X-ray_14mm_coll_16kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_14kV_850V_46V"] = "X-ray_14mm_coll_14kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_12kV_850V_46V"] = "X-ray_14mm_coll_12kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_10kV_850V_46V"] = "X-ray_14mm_coll_10kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_8kV_850V_46V"] = "X-ray_14mm_coll_08kV";
exp_folders["231109_1ph_LArN2_X-ray_14mm_coll_0kV_850V_46V"] = "X-ray_14mm_coll_00kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V_1"] = "X-ray_6mm_coll_20kV_1";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V"] = "X-ray_6mm_coll_20kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_18kV_850V_46V"] = "X-ray_6mm_coll_18kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_16kV_850V_46V"] = "X-ray_6mm_coll_16kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_14kV_850V_46V"] = "X-ray_6mm_coll_14kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_12kV_850V_46V"] = "X-ray_6mm_coll_12kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_10kV_850V_46V"] = "X-ray_6mm_coll_10kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_8kV_850V_46V"] = "X-ray_6mm_coll_08kV";
exp_folders["231109_1ph_LArN2_X-ray_6mm_coll_0kV_850V_46V"] = "X-ray_6mm_coll_00kV";

std::map<std::string, double> pre_trigger_max_S; //for calibration only
for (int e = 0; e != exp_area.experiments.size(); ++e) {
    std::string exp = exp_area.experiments[e];
    pre_trigger_max_S[exp] = 0.06;
}

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string exp = post_processor->experiments[post_processor->current_exp_index];
double d_S2_start = S2_times.at(exp).first, d_S2_finish = S2_times.at(exp).second;
double d_S1_start = S1_times.at(exp).first, d_S1_finish = S1_times.at(exp).second;
double d_Bkg_start = Bkg_times.at(exp).first, d_Bkg_finish = Bkg_times.at(exp).second;
double d_S_max = pre_trigger_max_S.at(exp);
std::string folder = exp_folders.at(exp),
			S2_start = dbl_to_str(d_S2_start, 1),
			S2_finish = dbl_to_str(d_S2_finish, 1),
			S1_start = dbl_to_str(d_S1_start, 1),
			S1_finish = dbl_to_str(d_S1_finish, 1),
			Bkg_start = dbl_to_str(d_Bkg_start, 1),
			Bkg_finish = dbl_to_str(d_Bkg_finish, 1);
int PMT_state = 0; //850V, 12 dB
int SiPM_state = 0; //46 V

ty(AStates::MPPC_S_sum); //nex();
time_zoom_SiPMs(0, d_S1_start);
set_zoom(0, 0.3);
set_bins(500); set_log_y();
draw_limits(0, d_S_max);
saveaspng(data_output_path + "calibration/" + folder + "/SiPMs_pre-trigger");
set_as_run_cut("small_pre-trigger");

for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) {
int channel = calib_channels[chi];
std::string ch_str = int_to_str(channel);
//parameters set by Pu_46V_20kV_850V, but valid for all fields.
// adsf - for fast Ctrl+F
if (channel>=32) {
    if (getIndex(post_processor->MPPC_channels, channel)<0)
        continue;
ty(AStates::MPPC_A_S);
    ch(channel);
    set_zoom(0, 0.05, 0, 0.01);
    set_bins(800);
    noise_cut(channel, 1, SiPM_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/"+ch_str+"_A_S_1_zoom");
    set_log_z();
    saveaspng(data_output_path + "calibration/" + folder + "/"+ch_str+"_A_S_2_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveaspng(data_output_path + "calibration/" + folder +"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, SiPM_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveaspng(data_output_path + "calibration/" + folder +"/"+ch_str+"_Ss_2_single_pe_w_0-"+S1_start+"us");
}
if (channel==1) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.5, 0, 5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.24, 0, 0.08);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(500);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.20);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.14);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==2) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.3, 0, 3.5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.18, 0, 0.06);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
    unset_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_5_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.2);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.12);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==3) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.3, 0, 2.5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.14, 0, 0.05);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.20);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.14);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==4) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.2, 0, 2.5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.15, 0, 0.06);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.2);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.14);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==5) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.65, 0, 0.020);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.1, 0, 0.006);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(240, 300);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==6) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.54, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.1, 0, 0.003);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200, 400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.0015);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==7) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.54, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.1, 0, 0.003);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200, 400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.35, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.1, 0, 0.003);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
    set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200, 400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
}//for (channels)
nex();
}//for (experiments)
if (calibrate) {
    calib_save(calibration_file);
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
int PMT_state = 0; //850V, 12 dB
int SiPM_state = 0; //46 V
std::vector<std::string> cuts; // List of cuts in terms of figure numbers where cut is shown
int no = 0; // Figure number
std::string Num = int_to_str(++no, 2); // = "01" // Figure number as string
std::string FOLDER = data_output_path + folder + "/";
//zcxv
if (exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_20kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_18kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_16kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_14kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_12kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_10kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_8kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_14mm_coll_0kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V_1"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_18kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_16kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_14kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_12kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_10kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_8kV_850V_46V"
    || exp == "231109_1ph_LArN2_X-ray_6mm_coll_0kV_850V_46V") {
    std::vector<std::string> cuts;
    int no = 0; //picture number
    std::string Num = int_to_str(++no, 2); //="01" //picture number as string
    std::string FOLDER = data_output_path + folder + "/";

ty(AStates::PMT_Npe_sum);
    slow_PMTs_only();
    cut_PMT_noise(PMT_state);
    set_bins(0, 200);
    saveaspng(FOLDER + Num+"_sPMTs_Npe_"+cuts_str(cuts)+"_0");
    set_log_y();
    saveaspng(FOLDER + Num+"_sPMTs_Npe_"+cuts_str(cuts)+"_1_log");
    unset_log_y();
    Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        noise_cut(chan, 0, SiPM_state, false);
    }
    set_bins(0, 200);
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_0");
    set_log_y();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_1_log");
    unset_log_y();
    Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
    ty(AStates::Correlation);
    set_zoom(0, 300, 0, 100);
    set_bins(300, 100);
    x_y_regions = {0, 0, 200, 0, 200, 100, 0, 100};
    if (exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_20kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_18kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_16kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_14kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_12kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_10kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_8kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_filt3_0kV_850V_46V")
        x_y_regions = {0, 0, 40, 0, 40, 12, 0, 12};
    if (exp == "231109_1ph_LArN2_X-ray_14mm_coll_20kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_18kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_16kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_14kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_12kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_10kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_8kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_14mm_coll_0kV_850V_46V")
        x_y_regions = {0, 0, 80, 0, 80, 20, 0, 20};
    if (exp == "231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V_1"
        || exp == "231109_1ph_LArN2_X-ray_6mm_coll_18kV_850V_46V")
        x_y_regions = {0, 0, 40, 0, 40, 13, 0, 13};
    if (exp == "231109_1ph_LArN2_X-ray_6mm_coll_20kV_850V_46V") {
        set_zoom(0, 1000, 0, 300);
        set_bins(500, 300);
        x_y_regions = {0, 0, 400, 0, 400, 109, 0, 109};
    }
    if (exp == "231109_1ph_LArN2_X-ray_6mm_coll_16kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_6mm_coll_14kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_6mm_coll_12kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_6mm_coll_10kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_6mm_coll_8kV_850V_46V"
        || exp == "231109_1ph_LArN2_X-ray_6mm_coll_0kV_850V_46V")
        x_y_regions = {0, 0, 30, 0, 30, 13, 0, 13};
    cut_x_y_poly_select(x_y_regions, true, "1");
    update();
    set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
    saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_0");
    set_log_z();
    saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_1_log");
    unset_log_z();
    //if (exp == "231109_1ph_LArN2_X-ray_6mm_coll_18kV_850V_46V") {
        set_zoom(0, 1000, 0, 300);
        set_bins(500, 300);
        saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_2_zoom");
    //}
    set_as_run_cut("no_discharge"); cuts.push_back(Num);
    Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
    set_bins(0, 100);
    time_zoom_SiPMs(d_Bkg_start, d_Bkg_finish); update();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+Bkg_start+"-"+Bkg_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
    saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-"+S2_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
    slow_PMTs_only();
    cut_PMT_noise(PMT_state);
    set_bins(0, 100);

    time_zoom_PMTs(d_Bkg_start, d_Bkg_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+Bkg_start+"-"+Bkg_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    time_zoom_PMTs(d_S1_start, d_S1_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    time_zoom_PMTs(d_S2_start, d_S2_finish); update();
    saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-"+S2_finish+"us_"+cuts_str(cuts));
    Num = int_to_str(++no, 2);

    std::string form_n = "forms_X-ray/";
    save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
    print_accepted_events(FOLDER + form_n + "_events.txt", first_run);

    // Too many parameters for this to be worth made into function.
    ty(AStates::MPPC_Npe_sum);
    for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
        int chan = post_processor->MPPC_channels[ich];
        ch(chan);
        cut_t(d_Bkg_start, d_Bkg_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_pre_trigger = std::pair<double, double>(d_Bkg_start, d_Bkg_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_pre_trigger = get_mean();
        cut_t(d_S1_start, d_S1_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S1 = std::pair<double, double>(d_S1_start, d_S1_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S1 = get_mean();
        cut_t(d_S2_start, d_S2_finish, false, chan);
        update();
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].t_S2 = std::pair<double, double>(d_S2_start, d_S2_finish);
        (*g_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S2 = get_mean();
    }

    ty(AStates::PMT_Npe_sum);
    for (int chan = 1; chan != 9; ++chan) {
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
}
//END OF FORMS
}
