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

//Even it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
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
            cut_A_S_upper(0.005, 0.0, 0.0230, 0.0, display, channel, "rem_smallA_largeS_1");
            cut_A_S_lower(0.020, 0.0039, 0.06, 0.0069, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.06, 0.0069, 1.4, 0.31, display, channel, "rem_largeA_smallS");
        }
        if (channel==2) {
            cut_A_S_upper(0.005, 0.0, 0.018, 0.0, display, channel, "rem_smallA_largeS_1");
            cut_A_S_lower(0.0122, 0.00226, 0.055, 0.0044, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.055, 0.0044, 1.6, 0.27, display, channel, "rem_largeA_smallS");
        }
        if (channel==3) {
            //cut_A_S_upper(0.005, 0.0, 0.0139, 0.0, display, channel, "rem_smallA_largeS");
            cut_A_S_lower(0.01, 0.0028, 0.050, 0.0040, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.050, 0.0040, 1.2, 0.22, display, channel, "rem_largeA_smallS");
        }
        if (channel==4) {
            //cut_A_S_upper(0.01, 0.0, 0.0171, 0.0, display, channel, "rem_smallA_largeS_1");
            cut_A_S_lower(0.01, 0.0021, 0.055, 0.0044, display, channel, "rem_smallA_smallS");
            cut_A_S_lower(0.055, 0.0044, 1.6, 0.26, display, channel, "rem_largeA_smallS");
        }
        if (channel==5) {
            cut_A_S_upper(0.001, 0.0, 0.0084, 0.0, display, channel, "rem_smallA-S");
            if (aggressiveness == 1) {
                cut_A_S_upper(0.65, 0, 5, 0, display, channel, "rem_A>0.65");
                cut_A_S_upper(0.0, 0.030, 1.6, 0.030, display, channel, "rem_S>0.030");
            }
        }
        if (channel==6) {
            cut_A_S_upper(0.001, 0.0, 0.0062, 0.0, display, channel, "rem_smallA-S");
            if (aggressiveness == 1) {
                cut_A_S_upper(0.50, 0, 5, 0, display, channel, "rem_A>0.50");
                cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
            }
        }
        if (channel==7) {
            cut_A_S_upper(0.001, 0.0, 0.0060, 0.0, display, channel, "rem_smallA-S");
            if (aggressiveness == 1) {
                cut_A_S_upper(0.54, 0, 2, 0, display, channel, "rem_A>0.54");
                cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
            }
        }
        if (channel==8) {
            if (aggressiveness == 1) {
                cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
                cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
            }
        }
    }
    //qewr
    //The following are set by hand for 20kV individually, there is no other way.
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
            cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0069, 0.00181, 0.0159, 0.00356, display, channel, "2pe_merged");
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
        cut_A_S_lower(0.0055, 0.00097, 0.02, 0.00097, display, channel, "small_A-S_noise");
        x_y_regions = {0.0055, 0.0055, 0.00058, 0.05, 0.0090, 1e3};
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
        cut_A_S_lower(0.0060, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
        x_y_regions = {0.0060, 0.0060, 0.00050, 0.05, 0.0090, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
        cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
        if (aggressiveness>=1)//select only 1 photoelectron for calibration
            cut_A_S_upper(0.0149, 0.0001, 1, 0.0001, display, channel, "2pe");
        if (aggressiveness>=2)//remove afterimpulses
            cut_A_S_upper(0.0060, 0.00162, 0.0149, 0.00335, display, channel, "2pe_merged");
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

//call from x_script.cpp after analysis is finished
void save_SiPM_Npe_table(std::string fname) {
	std::ofstream str;
	str.open(fname, std::ios_base::trunc);
	if (!str.is_open())
		return;
	str<<"V\t";
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		str<<chan<<"\t";
	}
	str<<std::endl;
	std::vector<std::string> ref_candidates;
	for (auto i = experiment_fields.begin(); i != experiment_fields.end(); ++i)
		if (i->second == 0)
			ref_candidates.push_back(i->first);
	if (ref_candidates.empty()) {
		std::cerr<<"Error:save_SiPM_Npe_table: Could not find reference experiment (0 field). Aborting."<<std::endl;
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
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		reference_data refernce;
		SiPM_Npe_data data = (*gSiPM_Npe_data.info(chan))[ref_ind];
		refernce.ref_S1 = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
		refernce.ref_S2 = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
		ref_data.push(chan, refernce);
	}

	for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i) {
		double V = experiment_fields[exp_area.experiments[i]];
		str<<V<<"\t";
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			reference_data ref = *ref_data.info(chan);
			SiPM_Npe_data data = (*gSiPM_Npe_data.info(chan))[i];
			double S1_no_noise = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
			double S2_no_noise = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
			double S2_only = S2_no_noise - ref.ref_S2 * S1_no_noise / ref.ref_S1;
			str<<S2_only<<"\t";
		}
		str<<std::endl;
	}
}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2022.12.07
//S1 + S2 in LAr in thin GEM1
//Almost standard pulsed X-ray analysis. Some event selection to avoid events with discharge/turned off SiPMs or events with gas bubbles.
//To check GEM1 charge-up, only first 1k of events is selected.
//Not using trigger adjustement or trigger-related cuts.
//Output is signal pulse-shapes and Npe for S2
//Based on 221124/1analysis_history.cpp

//Setup: single-phase, X-ray with 14-mm collimator and copper filter (plate) #3,
//75% Electroconnect's THGEM used for cathode and THGEM0,
//Polish standard thin GEM (PL 2022 #1) is used as GEM1, all PMTs with high gain (3 old PMTs which we in use
//till ~ May 2019 and new PMTs as #4). 600 MOhm part of V0 divider is inside EL gap, covers PMT#1 and PMT#2(?).
//No WLS. 1.0 atm pressures of argon in EL gap.
//Setup was tilted by sin(angle) = 8mm/330mm => 1.4 degrees to avoid gas pocket under GEM1

//adsf
data_output_path = "221124/results_v2/";
calibration_file = "221124/results_v1/221124_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
std::map<std::string, std::pair<double, double> > S1_times;
std::map<std::string, std::pair<double, double> > Bkg_times;
for (int e = 0; e != exp_area.experiments.size(); ++e) {
	std::string exp = exp_area.experiments[e];
	S1_times[exp] = std::pair<double, double> (31.6, 43);
	S2_times[exp] = std::pair<double, double> (43, 72);
	Bkg_times[exp] = std::pair<double, double> (1, 31.6);
}

std::map<std::string, std::string> exp_folders;
exp_folders["221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3_2"] = "X-ray_20kV_785V_2";
exp_folders["221124_X-ray_S2_LAr_20kV_831V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_831V";
exp_folders["221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_785V";
exp_folders["221124_X-ray_S2_LAr_20kV_739V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_739V";
exp_folders["221124_X-ray_S2_LAr_20kV_693V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_693V";
exp_folders["221124_X-ray_S2_LAr_20kV_646V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_646V";
exp_folders["221124_X-ray_S2_LAr_20kV_600V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_600V";
exp_folders["221124_X-ray_S2_LAr_20kV_554V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_554V";
exp_folders["221124_X-ray_S2_LAr_20kV_508V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_508V";
exp_folders["221124_X-ray_S2_LAr_20kV_462V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_462V";
exp_folders["221124_X-ray_S2_LAr_20kV_416V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_416V";
exp_folders["221124_X-ray_S2_LAr_20kV_369V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_369V";
exp_folders["221124_X-ray_S2_LAr_20kV_323V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_323V";
exp_folders["221124_X-ray_S2_LAr_20kV_277V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_277V";
exp_folders["221124_X-ray_S2_LAr_20kV_231V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_231V";
exp_folders["221124_X-ray_S2_LAr_20kV_185V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_185V";
exp_folders["221124_X-ray_S2_LAr_20kV_139V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_139V";
exp_folders["221124_X-ray_S2_LAr_20kV_92V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_092V";
exp_folders["221124_X-ray_S2_LAr_20kV_46V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_046V";
exp_folders["221124_X-ray_S2_LAr_20kV_0V_850V_46V_14mm_coll_filt3"] = "X-ray_20kV_000V";

std::map<std::string, double> pre_trigger_max_S; //for calibration only
for (int e = 0; e != exp_area.experiments.size(); ++e) {
	std::string exp = exp_area.experiments[e];
	pre_trigger_max_S[exp] = 0.06;
}

bool forms = !calibrate;
if (calibrate) {
	std::cout<<"Use 1analysis_history.cpp for calibration! Doing nothing."<<std::endl;
	return;
}

//SIGNAL FORMS
if (!forms)
	return;
std::string folder, S2_start, S2_finish, S1_start, S1_finish, Bkg_start, Bkg_finish;
double d_Bkg_start, d_Bkg_finish;
int first_run = 0;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //850V, 12 dB
int SiPM_state = 0; //46 V
auto folder_entry = exp_folders.find(exp);
if (folder_entry != exp_folders.end())
    folder = folder_entry->second;
else {
    std::cout<<"Could not find output folder for '"<<exp<<"'! Skipping building signal forms"<<std::endl;
    return;
}
auto S2_times_entry = S2_times.find(exp);
if (S2_times_entry != S2_times.end()) {
    d_S2_start = S2_times_entry->second.first; d_S2_finish = S2_times_entry->second.second;
		S2_start = dbl_to_str(d_S2_start, 1); S2_finish = dbl_to_str(d_S2_finish, 1);
} else {
    std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping this experiment"<<std::endl;
    return;
}
auto S1_times_entry = S1_times.find(exp);
if (S1_times_entry != S1_times.end()) {
    d_S1_start = S1_times_entry->second.first; d_S1_finish = S1_times_entry->second.second;
		S1_start = dbl_to_str(d_S1_start, 1); S1_finish = dbl_to_str(d_S1_finish, 1);
} else {
    std::cout<<"Could not find S1 time limits for '"<<exp<<"'! Skipping this experiment"<<std::endl;
    return;
}
auto Bkg_times_entry = Bkg_times.find(exp);
if (Bkg_times_entry != Bkg_times.end()) {
    d_Bkg_start = Bkg_times_entry->second.first; d_Bkg_finish = Bkg_times_entry->second.second;
		Bkg_start = dbl_to_str(d_Bkg_start, 1); Bkg_finish = dbl_to_str(d_Bkg_finish, 1);
} else {
    std::cout<<"Could not find Background time limits for '"<<exp<<"'! Skipping this experiment"<<std::endl;
  return;
}
auto first_run_entry = experiment_runs.find(exp);
if (first_run_entry != experiment_runs.end())
    first_run = first_run_entry->second;
else {
    std::cout<<"Could not find starting run index for '"<<exp<<"'! Will print invalid event indexes."<<std::endl;
    first_run = -10000;
}
//zcxv
if (exp == "221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3_2"
	|| exp =="221124_X-ray_S2_LAr_20kV_831V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_739V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_693V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_646V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_600V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_554V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_508V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_462V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_416V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_369V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_323V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_277V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_231V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_185V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_139V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_92V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_46V_850V_46V_14mm_coll_filt3"
	|| exp =="221124_X-ray_S2_LAr_20kV_0V_850V_46V_14mm_coll_filt3") {
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
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 0, 100, 0, 100, 100, 0, 100};
	if (exp == "221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3_2"
		|| exp == "221124_X-ray_S2_LAr_20kV_831V_850V_46V_14mm_coll_filt3")
		x_y_regions = {0, 0, 50, 0, 50, 50, 0, 50};
	if (exp =="221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_739V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_693V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_646V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_600V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_554V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_508V_850V_46V_14mm_coll_filt3")
		x_y_regions = {0, 0, 25, 0, 25, 22, 0, 22};
	if (exp =="221124_X-ray_S2_LAr_20kV_462V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_416V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_369V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_323V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_277V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_231V_850V_46V_14mm_coll_filt3")
		x_y_regions = {0, 0, 26, 0, 26, 20, 0, 20};
	if (exp =="221124_X-ray_S2_LAr_20kV_185V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_139V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_92V_850V_46V_14mm_coll_filt3"
		|| exp =="221124_X-ray_S2_LAr_20kV_46V_850V_46V_14mm_coll_filt3")
		x_y_regions = {0, 0, 27, 0, 27, 18, 0, 18};
	if (exp == "221124_X-ray_S2_LAr_20kV_0V_850V_46V_14mm_coll_filt3")
		x_y_regions = {0, 0, 28, 0, 28, 18, 1.3, 18, 0, 4.1};

	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_0");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_1_log");
	unset_log_z();
	if (exp == "221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3_2"
		|| exp == "221124_X-ray_S2_LAr_20kV_831V_850V_46V_14mm_coll_filt3"
		|| exp == "221124_X-ray_S2_LAr_20kV_785V_850V_46V_14mm_coll_filt3") {
		set_zoom(0, 8000, 0, 8000);
		saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_2_zoom");
	}
	set_as_run_cut("no_discharge"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);

	cut_runs(1000, "first_events");
	set_as_run_cut("first_events");

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
		(*gSiPM_Npe_data.info(chan))[post_processor->current_exp_index].t_pre_trigger = std::pair<double, double>(d_Bkg_start, d_Bkg_finish);
		(*gSiPM_Npe_data.info(chan))[post_processor->current_exp_index].Npe_pre_trigger = get_mean();
		cut_t(d_S1_start, d_S1_finish, false, chan);
		update();
		(*gSiPM_Npe_data.info(chan))[post_processor->current_exp_index].t_S1 = std::pair<double, double>(d_S1_start, d_S1_finish);
		(*gSiPM_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S1 = get_mean();
		cut_t(d_S2_start, d_S2_finish, false, chan);
		update();
		(*gSiPM_Npe_data.info(chan))[post_processor->current_exp_index].t_S2 = std::pair<double, double>(d_S2_start, d_S2_finish);
		(*gSiPM_Npe_data.info(chan))[post_processor->current_exp_index].Npe_S2 = get_mean();
	}
}
//END OF FORMS
}
