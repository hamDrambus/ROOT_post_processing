void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
std::string cuts_str(std::vector<int> cuts);
void central_SiPMs(bool on);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);

void time_zoom_fPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 8);
	cut_t(f_time, to_time, false, 9);
	cut_t(f_time, to_time, false, 10);
	cut_t(f_time, to_time, false, 11);
}

void time_zoom_sPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 12);
	cut_t(f_time, to_time, false, 13);
	cut_t(f_time, to_time, false, 14);
	cut_t(f_time, to_time, false, 15);
}

void time_zoom_SiPMs (double f_time, double to_time) {
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		cut_t(f_time, to_time, false, chan);
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

void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition)
{
	double zoom_l = 0, zoom_r = 160;
	std::size_t Nbins = 1600;
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_condition, false);
	}
	set_zoom(zoom_l, zoom_r);	set_log_y();
	set_bins(Nbins);
	saveas(path + "SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(path + "SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "10_form_by_Npeaks");
  ch(11);
	noise_cut(11, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "11_form_by_Npeaks");

	ty(AStates::PMT_tbNpe);
	ch(8);
	noise_cut(8, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "8_form_by_Npe");
	ch(9);
	noise_cut(9, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "9_form_by_Npe");
	ch(10);
	noise_cut(10, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "10_form_by_Npe");
  ch(11);
	noise_cut(11, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "11_form_by_Npe");
  ch(12);
	noise_cut(12, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "12_form_by_Npe");
	ch(13);
	noise_cut(13, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "13_form_by_Npe");
	ch(14);
	noise_cut(14, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "14_form_by_Npe");
	ch(15);
	noise_cut(15, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "15_form_by_Npe");

	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(8);
		noise_cut(8, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "8_form_by_S");
		ch(9);
		noise_cut(9, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "9_form_by_S");
		ch(10);
		noise_cut(10, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "10_form_by_S");
		ch(11);
		noise_cut(11, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "11_form_by_S");
		ch(12);
		noise_cut(12, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "12_form_by_S");
		ch(13);
		noise_cut(13, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "13_form_by_S");
		ch(14);
		noise_cut(14, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "14_form_by_S");
		ch(15);
		noise_cut(15, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "15_form_by_S");
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
if (channel==15) {
	cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.0006, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.00824, 0.0395, 0.0825, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }
}
if (channel==14) {
	cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.0006, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.0363, 0.0690, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }
}
if (channel==13) {
	cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.0006, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.035, 0.0680, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }
}
if (channel==12) {
    cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.00082, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.025, 0.0540, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }
}
if (channel==11) {
	cut_A_S_upper(0.00362, 0.000112, 0.009, 0.000238, display, channel, "rem_smallA_largeS");
    if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (channel==10) {
	cut_A_S_left(0.00505, 0.0, 0.00505, 1e5, display, channel, "rem_smallA");
    if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (channel==9) {
	cut_A_S_left(0.0045, 0.00012, 0.0045, 1e5, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (channel==8) {
    cut_A_S_left(0.0045, 0, 0.0045, 1e5, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
//The following are set by hand for 20kV individually, there is no other way.
if (device_condition == 0) { //46V on SiPMs
if (channel==32) {
    cut_A_S_lower(0.011, 0.0011, 0.023, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.00065, 0.06, 0.00535, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0250, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0250, 0.00307, display, channel, "2pe_merged");
}
if (channel==33) {
	cut_A_S_lower(0.008, 0.00115, 0.0175, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.008, 0.008, 0.000528, 0.06, 0.00538, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0205, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.008, 0.0013, 0.0205, 0.0029, display, channel, "2pe_merged");
}
if (channel==34) {
	cut_A_S_lower(0.0115, 0.00150, 0.027, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.00071, 0.06, 0.00562, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0280, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.0280, 0.0036, display, channel, "2pe_merged");
}
if (channel==35) {
	cut_A_S_lower(0.0115, 0.0013, 0.0287, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.000740, 0.06, 0.0057, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.027, 0.004, display, channel, "2pe_merged");
}
if (channel==36) {
	cut_A_S_lower(0.0115, 0.00130, 0.027, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.000555, 0.06, 0.00623, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0019, 0.026, 0.004, display, channel, "2pe_merged");
}
if (channel==37) {
	cut_A_S_lower(0.0095, 0.00111, 0.030, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0095, 0.0095, 0.00058, 0.06, 0.00583, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0224, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0095, 0.00151, 0.0224, 0.00342, display, channel, "2pe_merged");
}
if (channel==38) {
	cut_A_S_lower(0.012, 0.00130, 0.03, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000687, 0.06, 0.00591, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00185, 0.0285, 0.0042, display, channel, "2pe_merged");
}
if (channel==39) {
	cut_A_S_lower(0.011, 0.00130, 0.032, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.000638, 0.06, 0.00617, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0247, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0247, 0.00376, display, channel, "2pe_merged");
}
if (channel==40) {
	cut_A_S_lower(0.0117, 0.00119, 0.03, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0117, 0.0117, 0.000744, 0.06, 0.00572, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0263, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0117, 0.00178, 0.0263, 0.00364, display, channel, "2pe_merged");
}
if (channel==41) {
	cut_A_S_lower(0.012, 0.00119, 0.024, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000638, 0.06, 0.00604, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00180, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==42) {
	cut_A_S_lower(0.0115, 0.00125, 0.024, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.00077, 0.06, 0.00558, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.00167, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel == 43 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==44) {
	cut_A_S_lower(0.01, 0.00112, 0.028, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, 0.000640, 0.06, 0.00577, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0225, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00157, 0.0225, 0.0030, display, channel, "2pe_merged");
}
if (channel==48) {
	cut_A_S_lower(0.012, 0.00125, 0.02, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000824, 0.06, 0.00595, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");;
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0270, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0016, 0.0270, 0.00375, display, channel, "2pe_merged");
}
if (channel==49) {
	cut_A_S_lower(0.0089, 0.0010, 0.026, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0089, 0.0089, 0.000611, 0.06, 0.00582, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0089, 0.00139, 0.02, 0.0028, display, channel, "2pe_merged");
}
if (channel==50) {
	cut_A_S_lower(0.0085, 0.00101, 0.022, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0085, 0.0085, 0.000425, 0.06, 0.006, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0196, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0085, 0.00149, 0.0196, 0.00279, display, channel, "2pe_merged");
}
if (channel==51) {
	cut_A_S_lower(0.0109, 0.00122, 0.036, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0109, 0.0109, 0.000691, 0.06, 0.00585, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0248, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0109, 0.00167, 0.0248, 0.00348, display, channel, "2pe_merged");
}
if (channel==52) {
	cut_A_S_lower(0.0115, 0.00127, 0.04, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.000665, 0.06, 0.00606, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0263, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0018, 0.0263, 0.0039, display, channel, "2pe_merged");
}
if (channel==53) {
	cut_A_S_lower(0.012, 0.00146, 0.028, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000691, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00186, 0.028, 0.00436, display, channel, "2pe_merged");
}
if (channel==54) {
	cut_A_S_lower(0.01, 0.0012, 0.030, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, 0.000638, 0.06, 0.00587, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00157, 0.023, 0.00325, display, channel, "2pe_merged");
}
if (channel==55) {
	cut_A_S_lower(0.0107, 0.00117, 0.026, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0107, 0.0107, 0.000638, 0.06, 0.00625, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0235, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0107, 0.00165, 0.0235, 0.0034, display, channel, "2pe_merged");
}
if (channel==56) {
	cut_A_S_lower(0.0118, 0.00138, 0.023, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0118, 0.0118, 0.000851, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0118, 0.00165, 0.0255, 0.0038, display, channel, "2pe_merged");
}
if (channel==57) {
	cut_A_S_lower(0.012, 0.00127, 0.040, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.00079, 0.06, 0.00596, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00162, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==58) {
	cut_A_S_lower(0.0113, 0.00133, 0.025, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0113, 0.0113, 0.000664, 0.06, 0.0062, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0246, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0113, 0.00162, 0.0246, 0.00367, display, channel, "2pe_merged");
}
if (channel==59) {
	cut_A_S_lower(0.0118, 0.00133, 0.024, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0118, 0.0118, 0.00077, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0118, 0.00167, 0.0255, 0.00364, display, channel, "2pe_merged");
}
}
//qewr
if (device_condition == 1) { //48V on SiPMs
if (channel==32) {
	x_y_regions = {0.0185, 0.0185, 0.00133, 0.08, 0.0075, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0412, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0185, 0.0023, 0.0412, 0.0053, display, channel, "2pe_merged");
}
if (channel==33) {
	x_y_regions = {0.0165, 0.0165, 0.00130, 0.08, 0.0079, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0358, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0165, 0.0022, 0.0358, 0.0048, display, channel, "2pe_merged");
}
if (channel==34) {
	x_y_regions = {0.02, 0.02, 0.00165, 0.08, 0.0077, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0447, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.02, 0.00232, 0.0447, 0.0060, display, channel, "2pe_merged");
}
if (channel==35) {
	x_y_regions = {0.02, 0.02, 0.00153, 0.08, 0.0076, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0437, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.02, 0.00241, 0.0437, 0.0059, display, channel, "2pe_merged");
}
if (channel==36) {
	x_y_regions = {0.0195, 0.0195, 0.00149, 0.08, 0.0082, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.04, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0195, 0.0026, 0.04, 0.0059, display, channel, "2pe_merged");
}
if (channel==37) {
	x_y_regions = {0.0174, 0.0174, 0.00124, 0.08, 0.0080, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.037, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0174, 0.00232, 0.037, 0.0051, display, channel, "2pe_merged");
}
if (channel==38) {
	x_y_regions = {0.0205, 0.0205, 0.00155, 0.08, 0.0080, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0444, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0205, 0.0025, 0.0444, 0.0067, display, channel, "2pe_merged");
}
if (channel==39) {
	x_y_regions = {0.0189, 0.0189, 0.00140, 0.08, 0.0082, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.04, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0189, 0.00245, 0.04, 0.0057, display, channel, "2pe_merged");
}
if (channel==40) {
	x_y_regions = {0.0195, 0.0195, 0.00154, 0.08, 0.0078, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0433, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0195, 0.00232, 0.0433, 0.0063, display, channel, "2pe_merged");
}
if (channel==41) {
	x_y_regions = {0.0195, 0.0195, 0.00154, 0.08, 0.0078, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0412, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0195, 0.00241, 0.0412, 0.0059, display, channel, "2pe_merged");
}
if (channel==42) {
	x_y_regions = {0.0195, 0.0195, 0.00154, 0.08, 0.0078, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0424, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0195, 0.00241, 0.0424, 0.0056, display, channel, "2pe_merged");
}
if (channel == 43) {
	if (post_processor->isMultichannel(post_processor->current_type))
		off_ch(channel);
	x_y_regions = {0.0195, 0.0195, 0.00154, 0.08, 0.0080, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0420, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0195, 0.00241, 0.0420, 0.0058, display, channel, "2pe_merged");
}
if (channel==44) {
	x_y_regions = {0.0182, 0.0182, 0.00136, 0.08, 0.0078, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0378, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0182, 0.00226, 0.0378, 0.0049, display, channel, "2pe_merged");
}
if (channel==48) {
	x_y_regions = {0.02, 0.02, 0.00165, 0.08, 0.0078, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0428, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.02, 0.00235, 0.0428, 0.0058, display, channel, "2pe_merged");
}
if (channel==49) {
	x_y_regions = {0.0165, 0.0165, 0.00127, 0.08, 0.0081, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0348, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0165, 0.00210, 0.0348, 0.0049, display, channel, "2pe_merged");
}
if (channel==50) {
	x_y_regions = {0.0165, 0.0165, 0.00121, 0.08, 0.0082, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0348, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0165, 0.00225, 0.0348, 0.0049, display, channel, "2pe_merged");
}
if (channel==51) {
	x_y_regions = {0.0188, 0.0188, 0.00146, 0.08, 0.00775, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0408, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0188, 0.00226, 0.0408, 0.0054, display, channel, "2pe_merged");
}
if (channel==52) {
	x_y_regions = {0.0188, 0.0188, 0.00146, 0.08, 0.00815, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0408, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0188, 0.00254, 0.0408, 0.0058, display, channel, "2pe_merged");
}
if (channel==53) {
	x_y_regions = {0.02, 0.02, 0.00146, 0.08, 0.0081, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0441, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.02, 0.00257, 0.0441, 0.0065, display, channel, "2pe_merged");
}
if (channel==54) {
	x_y_regions = {0.0179, 0.0179, 0.00140, 0.08, 0.0081, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0381, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0179, 0.00226, 0.0381, 0.0053, display, channel, "2pe_merged");
}
if (channel==55) {
	x_y_regions = {0.0179, 0.0179, 0.00133, 0.08, 0.0081, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0381, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0179, 0.00235, 0.0381, 0.0054, display, channel, "2pe_merged");
}
if (channel==56) {
	x_y_regions = {0.0193, 0.0193, 0.00156, 0.08, 0.00805, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0407, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0193, 0.00235, 0.0407, 0.0057, display, channel, "2pe_merged");
}
if (channel==57) {
	x_y_regions = {0.0196, 0.0196, 0.00165, 0.08, 0.00785, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0418, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0196, 0.00235, 0.0418, 0.0056, display, channel, "2pe_merged");
}
if (channel==58) {
	x_y_regions = {0.0190, 0.0190, 0.00146, 0.08, 0.0080, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.04, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0190, 0.00241, 0.04, 0.0055, display, channel, "2pe_merged");
}
if (channel==59) {
	x_y_regions = {0.0193, 0.0193, 0.00150, 0.08, 0.00795, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0412, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0193, 0.00241, 0.0412, 0.0057, display, channel, "2pe_merged");
}
}

}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 04.10.2020
//Analysing both 46 and 48 V, slight adjustements so that the analysis is the same as of the later dates.
//method = 0 - do not use trigger adjustement or trigger-related cuts.
//Otherwise the analysis and calibrations are standard (as in 191107, 200910, etc. (5analysis_history.cpp))
data_output_path = "190404/results_v6/";
calibration_file = data_output_path + "190404_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190404_Cd_20kV_850V_46V_th250mV_0"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_20kV_850V_46V_th250mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_18kV_850V_46V_th230mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_16kV_850V_46V_th210mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_14kV_850V_46V_th200mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_12kV_850V_46V_th160mV"] = std::pair<double, double> (23, 34);
S2_times["190404_Cd_10kV_850V_46V_th150mV"] = std::pair<double, double> (23, 34);
S2_times["190404_Cd_8kV_850V_46V_th140mV"] = std::pair<double, double> (21, 34);

S2_times["190404_Cd_20kV_850V_48V_th230mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_18kV_850V_48V_th210mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_16kV_850V_48V_th200mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_14kV_850V_48V_th160mV"] = std::pair<double, double> (23, 40);
S2_times["190404_Cd_12kV_850V_48V_th150mV"] = std::pair<double, double> (23, 34);
S2_times["190404_Cd_10kV_850V_48V_th150mV"] = std::pair<double, double> (23, 34);
S2_times["190404_Cd_8kV_850V_48V_th140mV"] = std::pair<double, double> (21, 34);

std::map<std::string, std::string> exp_folders;
exp_folders["190404_Cd_20kV_850V_46V_th250mV_0"] = "Cd_46V_20kV_850V_0";
exp_folders["190404_Cd_20kV_850V_46V_th250mV"] = "Cd_46V_20kV_850V";
exp_folders["190404_Cd_18kV_850V_46V_th230mV"] = "Cd_46V_18kV_850V";
exp_folders["190404_Cd_16kV_850V_46V_th210mV"] = "Cd_46V_16kV_850V";
exp_folders["190404_Cd_14kV_850V_46V_th200mV"] = "Cd_46V_14kV_850V";
exp_folders["190404_Cd_12kV_850V_46V_th160mV"] = "Cd_46V_12kV_850V";
exp_folders["190404_Cd_10kV_850V_46V_th150mV"] = "Cd_46V_10kV_850V";
exp_folders["190404_Cd_8kV_850V_46V_th140mV"] = "Cd_46V_08kV_850V";

exp_folders["190404_Cd_20kV_850V_48V_th230mV"] = "Cd_48V_20kV_850V";
exp_folders["190404_Cd_18kV_850V_48V_th210mV"] = "Cd_48V_18kV_850V";
exp_folders["190404_Cd_16kV_850V_48V_th200mV"] = "Cd_48V_16kV_850V";
exp_folders["190404_Cd_14kV_850V_48V_th160mV"] = "Cd_48V_14kV_850V";
exp_folders["190404_Cd_12kV_850V_48V_th150mV"] = "Cd_48V_12kV_850V";
exp_folders["190404_Cd_10kV_850V_48V_th150mV"] = "Cd_48V_10kV_850V";
exp_folders["190404_Cd_8kV_850V_48V_th140mV"] = "Cd_48V_08kV_850V";

std::map<std::string, double> pre_trigger_max_S;
pre_trigger_max_S["190404_Cd_20kV_850V_46V_th250mV_0"] = 0.24;
pre_trigger_max_S["190404_Cd_20kV_850V_46V_th250mV"] = 0.24;
pre_trigger_max_S["190404_Cd_18kV_850V_46V_th230mV"] = 0.2;
pre_trigger_max_S["190404_Cd_16kV_850V_46V_th210mV"] = 0.19;
pre_trigger_max_S["190404_Cd_14kV_850V_46V_th200mV"] = 0.14;
pre_trigger_max_S["190404_Cd_12kV_850V_46V_th160mV"] = 0.14;
pre_trigger_max_S["190404_Cd_10kV_850V_46V_th150mV"] = 0.14;
pre_trigger_max_S["190404_Cd_8kV_850V_46V_th140mV"] = 0.14;

pre_trigger_max_S["190404_Cd_20kV_850V_48V_th230mV"] = 0.2;
pre_trigger_max_S["190404_Cd_18kV_850V_48V_th210mV"] = 0.18;
pre_trigger_max_S["190404_Cd_16kV_850V_48V_th200mV"] = 0.17;
pre_trigger_max_S["190404_Cd_14kV_850V_48V_th160mV"] = 0.14;
pre_trigger_max_S["190404_Cd_12kV_850V_48V_th150mV"] = 0.13;
pre_trigger_max_S["190404_Cd_10kV_850V_48V_th150mV"] = 0.13;
pre_trigger_max_S["190404_Cd_8kV_850V_48V_th140mV"] = 0.13;

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
double d_S_max;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //850V, 12 dB
int SiPM_state = exp_ind < 8 ? 0 : 1; //46 or 48 V
auto folder_entry = exp_folders.find(exp);
if (folder_entry != exp_folders.end())
    folder = folder_entry->second;
else {
    std::cout<<"Could not find output folder for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
    nex();
    continue;
}
auto S2_times_entry = S2_times.find(exp);
if (S2_times_entry != S2_times.end()) {
    d_S2_start = S2_times_entry->second.first;
    d_S2_finish = S2_times_entry->second.second;
		S2_start = dbl_to_str(d_S2_start);
		S2_finish = dbl_to_str(d_S2_finish);
} else {
    std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
    nex();
    continue;
}
auto S_max_entry = pre_trigger_max_S.find(exp);
if (S_max_entry != pre_trigger_max_S.end()) {
    d_S_max = S_max_entry->second;
} else {
    std::cout<<"Could not find pre-trigger S limits for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
    nex();
    continue;
}

ty(AStates::PMT_S_sum); //nex();
off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
time_zoom_sPMTs(0, d_S2_start);
set_zoom(0, 2);
set_bins(500); set_log_y();
draw_limits(0, d_S_max);
saveas(data_output_path + folder +"/calibration/sPMTs_pre-trigger_cut");
set_as_run_cut("small_pre-trigger");

for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) {
int channel = calib_channels[chi];
std::string ch_str = int_to_str(channel);
//parameters set by Cd_20kV_850V_46V, but valid for all fields.
if (channel>=32) { //adsf
		if (getIndex(post_processor->MPPC_channels, channel)<0)
			continue;
if (SiPM_state==0) {
ty(AStates::MPPC_A_S);
		ch(channel);
    set_zoom(0, 0.06, 0, 0.01);
    set_bins(500);
    noise_cut(channel, 1, SiPM_state, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
}
if (SiPM_state==1) {
ty(AStates::MPPC_A_S);
		ch(channel);
    set_zoom(0, 0.08, 0, 0.012);
    set_bins(500);
    noise_cut(channel, 1, SiPM_state, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
}
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 1, SiPM_state, false);
		cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_single_pe_w_0-"+S2_start+"us");
}
if (channel==15) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 1.6, 0, 6);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.5, 0, 0.1);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		cut_A_S_left(0.0, 0.023, 0.044, 0.0, false, channel, "temp"); //Removes small noise for better display
		set_bins(100);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.16);
		set_bins(600);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.06);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		set_use_mean(); //overwrites old info from calibration file!
		unset_1peS(); //updates calibration if it was loaded from file
		update();
}
if (channel==14) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 1.6, 0, 6);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.5, 0, 0.1);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		cut_A_S_left(0.0, 0.02, 0.04, 0.0, false, channel, "temp"); //Removes small noise for better display
		set_bins(100);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.16);
		set_bins(600);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.06);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		set_use_mean(); //overwrites old info from calibration file!
		unset_1peS(); //updates calibration if it was loaded from file
		update();
}
if (channel==13) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 1.6, 0, 6);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.5, 0, 0.1);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		cut_A_S_left(0.0, 0.022, 0.04, 0.0, false, channel, "temp"); //Removes small noise for better display
		set_bins(120);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.16);
		set_bins(600);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.05);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		set_use_mean(); //overwrites old info from calibration file!
		unset_1peS(); //updates calibration if it was loaded from file
		update();
}
if (channel==12) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 1.6, 0, 6);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.5, 0, 0.1);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		cut_A_S_left(0.0, 0.018, 0.032, 0.0, false, channel, "temp"); //Removes small noise for better display
		set_bins(100);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.16);
		set_bins(600);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.05);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		set_use_mean(); //overwrites old info from calibration file!
		unset_1peS(); //updates calibration if it was loaded from file
		update();
}
if (channel==11) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 0.6, 0, 1.0);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.1, 0, 0.0025);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		set_bins(120);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.006);
		set_bins(800);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.0015);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		set_use_mean(); //overwrites old info from calibration file!
		unset_1peS(); //updates calibration if it was loaded from file
		update();
}
if (channel==10) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 0.6, 0, 1.0);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.1, 0, 0.0025);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		set_bins(120);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.006);
		set_bins(800);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.0015);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		set_use_mean(); //overwrites old info from calibration file!
		unset_1peS(); //updates calibration if it was loaded from file
		update();
}
if (channel==9) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 0.6, 0, 1.0);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.1, 0, 0.0025);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		set_bins(120);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.006);
		set_bins(800);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.0015);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		set_use_mean(); //overwrites old info from calibration file!
		unset_1peS(); //updates calibration if it was loaded from file
		update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
		ch(channel);
		set_zoom(0, 0.6, 0, 1.0);
		set_bins(1500);
		noise_cut(channel, 1, PMT_state, true);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
		set_zoom(0, 0.1, 0, 0.0025);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		set_bins(120);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
		set_zoom(0, 0.006);
		set_bins(800);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
		noise_cut(channel, 0, PMT_state, false);
		cut_t(0, d_S2_start, false, channel);
		draw_limits(0.0, 0.0015);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
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
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
int first_run = 0;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //850V, 12 dB
int SiPM_state = post_processor->current_exp_index < 8 ? 0 : 1; //46 or 48 V
auto folder_entry = exp_folders.find(exp);
if (folder_entry != exp_folders.end())
    folder = folder_entry->second;
else {
    std::cout<<"Could not find output folder for '"<<exp<<"'! Skipping building signal forms"<<std::endl;
    return;
}
auto S2_times_entry = S2_times.find(exp);
if (S2_times_entry != S2_times.end()) {
		d_S2_start = S2_times_entry->second.first;
		d_S2_finish = S2_times_entry->second.second;
		S2_start = dbl_to_str(d_S2_start);
		S2_finish = dbl_to_str(d_S2_finish);
} else {
    std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping building signal forms"<<std::endl;
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
if (exp == "190404_Cd_20kV_850V_46V_th250mV" || exp == "190404_Cd_20kV_850V_46V_th250mV_0" || exp == "190404_Cd_20kV_850V_48V_th230mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 13);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	ty(AStates::PMT_Npe_sum);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	x_y_regions = {12.2, 0, 26.9, 8.6, 52.9, 21.8, 77.5, 43.1, 249.9, 161};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_small_Npes/";
ty(AStates::PMT_Npe_sum);
	draw_limits(10, 25);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(29, 45);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(29, 60);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(75, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak_v2/";
ty(AStates::Correlation); //!!! Uses last correlation
	if (exp == "190404_Cd_20kV_850V_48V_th230mV")
		x_y_regions = {13.1, 31.8, 39.0, 11.9, 65.0, 44.4, 69.5, 80.9, 44.4, 102.8};
	else
		x_y_regions = {14.0, 25.1, 35.0, 5.2, 55.1, 16.5, 78.9, 45.7, 49.3, 70.26};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left_v2/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_20kV_850V_48V_th230mV")
		x_y_regions = {13.1, 31.8, 39.0, 11.9, 61.0, 41.7, 25.6, 64.3};
	else
		x_y_regions = {14.0, 25.1, 35.0, 5.2, 55.6, 20.5, 28.3, 44.4};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = Num + "_strange_events/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_20kV_850V_48V_th230mV")
		x_y_regions = {61.0, 11.9, 39.0, 11.9, 61.0, 41.7};
	else
		x_y_regions = {69.5, 4.6, 31.0, 4.6, 49.8, 8.6, 69.5, 34.4};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "190404_Cd_18kV_850V_46V_th230mV" || exp == "190404_Cd_18kV_850V_48V_th210mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 13);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	ty(AStates::PMT_Npe_sum);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	x_y_regions = {12.2, 0, 24.0, 6.7, 46.2, 21.4, 62.9, 37.8, 149.9, 103};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_small_Npes/";
ty(AStates::PMT_Npe_sum);
	draw_limits(6, 17);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(22, 33);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(22, 45);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(65, 110);
	saveaspng(FOLDER + Num + "_slowPMTs_Np2e_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak_v2/";
ty(AStates::Correlation); //!!! Uses last correlation
	if (exp == "190404_Cd_18kV_850V_48V_th210mV")
		x_y_regions = {12.9, 22.2, 29.1, 10.7, 48.4, 22.2, 57.0, 56.9, 40.1, 76.0, 21.5, 63.3};
	else
		x_y_regions = {10.0, 16.7, 28.3, 0.4, 50.3, 8.7, 61.3, 34.6, 46.3, 53.3, 22.3, 44.9};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left_v2/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_18kV_850V_48V_th210mV")
		x_y_regions = {12.9, 22.2, 29.1, 10.7, 48.4, 22.2, 49.2, 28.2, 17.5, 50.1};
	else
		x_y_regions = {10.0, 16.7, 28.3, 0.4, 50.5, 9.1, 19.9, 40.6};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "190404_Cd_16kV_850V_46V_th210mV" || exp == "190404_Cd_16kV_850V_48V_th200mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 13);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	ty(AStates::PMT_Npe_sum);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {12.2, 0, 20.9, 7.3, 38.5, 20.0, 52.9, 34.7, 119.9, 89};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_small_Npes/";
ty(AStates::PMT_Npe_sum);
	draw_limits(4, 13);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(16, 24);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(16, 34);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(50, 100);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak_v2/";
ty(AStates::Correlation); //!!! Uses last correlation
	if (exp == "190404_Cd_16kV_850V_48V_th200mV")
		x_y_regions = {9.1, 17.2, 23.9, 7.6, 38.9, 21.6, 41.9, 54.7, 26.7, 63.0, 13.1, 43.6};
	else
		x_y_regions = {6.5, 14.0, 20.5, 0.0, 36.6, 6.3, 46.9, 28.9, 32.1, 41.1, 16.8, 35.0};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left_v2/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_16kV_850V_48V_th200mV")
		x_y_regions = {9.1, 17.2, 23.9, 7.6, 38.9, 21.6, 12.9, 41.4};
	else
		x_y_regions = {6.5, 14.0, 20.5, 0.0, 36.6, 6.3, 15.1, 31.8};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "190404_Cd_14kV_850V_46V_th200mV" || exp == "190404_Cd_14kV_850V_48V_th160mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 10);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	ty(AStates::PMT_Npe_sum);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	x_y_regions = {7.4, 0, 17.6, 7.7, 28.3, 17.0, 38.0, 27.0, 99.9, 79};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_small_Npes/";
ty(AStates::PMT_Npe_sum);
	draw_limits(4, 9);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(11, 18);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(11, 25);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(40, 90);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak_v2/";
ty(AStates::Correlation); //!!! Uses last correlation
	if (exp == "190404_Cd_14kV_850V_48V_th160mV")
		x_y_regions = {5.0, 11.9, 19.9, 1.6, 32.3, 15.1, 35.0, 39.0, 21.2, 48.3, 8.6, 35.0};
	else
		x_y_regions = {5.2, 10.6, 16.5, 0.0, 20.8, 0.0, 32.6, 6.3, 34.6, 19.1, 28.1, 30.5, 12.9, 27.8};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left_v2/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_14kV_850V_48V_th160mV")
		x_y_regions = {5.0, 11.9, 19.9, 1.6, 32.3, 15.1, 7.7, 32.3};
	else
		x_y_regions = {5.2, 10.6, 16.5, 0.0, 20.8, 0.0, 28.1, 3.9, 10.6, 21.7};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "190404_Cd_12kV_850V_46V_th160mV" || exp == "190404_Cd_12kV_850V_48V_th150mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 9);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	ty(AStates::PMT_Npe_sum);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	x_y_regions = {6.1, 0, 24.7, 16.2, 99.9, 83};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_small_Npes/";
ty(AStates::PMT_Npe_sum);
	draw_limits(3, 6);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(8, 13);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(8, 18);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(29, 60);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak_v2/";
ty(AStates::Correlation); //!!! Uses last correlation
	if (exp == "190404_Cd_12kV_850V_48V_th150mV")
		x_y_regions = {2.4, 9.5, 13.3, 0, 21.2, 3.7, 28.7, 23.3, 20.6, 37.4, 7.9, 32.4};
	else
		x_y_regions = {3.1, 7.4, 11.7, 0.0, 15.6, 0.0, 23.0, 3.2, 26.9, 17.5, 16.5, 26.0, 6.3, 19.9};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left_v2/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_12kV_850V_48V_th150mV")
		x_y_regions = {2.4, 9.5, 13.3, 0, 21.2, 3.7, 23.5, 9.3, 5.1, 24.1};
	else
		x_y_regions = {3.1, 7.4, 11.7, 0.0, 15.6, 0.0, 23.0, 3.2, 6.3, 19.9};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "190404_Cd_10kV_850V_46V_th150mV" || exp == "190404_Cd_10kV_850V_48V_th150mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 9);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	ty(AStates::PMT_Npe_sum);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	x_y_regions = {3.8, 0, 99.9, 89};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_small_Npes/";
ty(AStates::PMT_Npe_sum);
	draw_limits(0, 4);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(5, 8);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(5, 12);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(23, 50);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak_v2/";
ty(AStates::Correlation); //!!! Uses last correlation
	if (exp == "190404_Cd_10kV_850V_48V_th150mV")
		x_y_regions = {2.7, 6.3, 9.6, 0.0, 12.9, 0.0, 19.9, 7.3, 18.3, 24.4, 7.6, 28.5, 2.7, 19.0};
	else
		x_y_regions = {1.8, 5.6, 9.9, 0.0, 12.8, 0.0, 20.1, 3.5, 21.0, 16.5, 12.2, 21.8, 3.1, 16.5};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left_v2/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_10kV_850V_48V_th150mV")
		x_y_regions = {2.7, 6.3, 9.6, 0.0, 12.9, 0.0, 16.6, 3.8, 2.8, 16.8};
	else
		x_y_regions = {1.8, 5.6, 9.9, 0.0, 12.8, 0.0, 18.4, 2.6, 2.6, 14.0};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "190404_Cd_8kV_850V_46V_th140mV" || exp == "190404_Cd_8kV_850V_48V_th140mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 9);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	ty(AStates::PMT_Npe_sum);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	on_ch(12); on_ch(13); on_ch(14); on_ch(15);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	x_y_regions = {3.8, 0, 99.9, 89};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_small_Npes/";
ty(AStates::PMT_Npe_sum);
	draw_limits(0, 2);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(3, 6.5);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(3, 11);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(20, 40);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak_v2/";
ty(AStates::Correlation); //!!! Uses last correlation
	if (exp == "190404_Cd_8kV_850V_48V_th140mV")
		x_y_regions = {2.1, 4.9, 7.0, 0.0, 13.5, 0.0, 16.0, 14.9, 9.9, 23.2, 3.9, 19.8};
	else
		x_y_regions = {1.8, 5.4, 6.1, 0.0, 13.6, 0.0, 16.5, 9.9, 10.8, 16.1, 2.6, 12.5};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_left_v2/";
ty(AStates::Correlation);
	if (exp == "190404_Cd_8kV_850V_48V_th140mV")
		x_y_regions = {2.1, 4.9, 7.0, 0.0, 13.5, 0.0, 13.9, 2.4, 2.4, 9.0};
	else
		x_y_regions = {1.8, 5.4, 6.1, 0.0, 10.3, 0.0, 1.8, 10.5};
	cut_x_y_poly_select(x_y_regions, true, "1"); update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_log");
	unset_log_z();
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::PMT_sum_N);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
