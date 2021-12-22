void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
std::string cuts_str(std::vector<int> cuts);
void central_SiPMs(bool on);
void fast_PMTs_only (void);
void slow_PMTs_only (void);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);

void time_zoom_fPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 5);
	cut_t(f_time, to_time, false, 6);
	cut_t(f_time, to_time, false, 7);
	cut_t(f_time, to_time, false, 8);
}

void time_zoom_sPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 0);
	cut_t(f_time, to_time, false, 1);
	cut_t(f_time, to_time, false, 2);
	cut_t(f_time, to_time, false, 3);
	cut_t(f_time, to_time, false, 4);
}

void time_zoom_SiPMs (double f_time, double to_time) {
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		cut_t(f_time, to_time, false, chan);
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
	ty(AStates::MPPC_tbNpe_sum);
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
	if (channel==1) {
		cut_A_S_upper(0.005, 0.0, 0.0146, 0.0, display, channel, "rem_smallA_largeS_1");
		cut_A_S_lower(0.011, 0.0005, 0.046, 0.0059, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.046, 0.0059, 1.1, 0.29, display, channel, "rem_largeA_smallS");
		if (aggressiveness == 1) {
			cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
			cut_A_S_upper(0.0, 2.0, 1.6, 2, display, channel, "rem_S>~2.0");
		}
	}
	if (channel==2) {
		cut_A_S_upper(0.013, 0.0, 0.028, 0.0, display, channel, "rem_smallA_largeS_1");
		cut_A_S_lower(0.0170, 0.0012, 0.075, 0.0052, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.075, 0.0052, 1.6, 0.26, display, channel, "rem_largeA_smallS");
		if (aggressiveness == 1) {
			cut_A_S_upper(1.05, 0, 2, 0, display, channel, "rem_A>1.05");
			cut_A_S_upper(0.0, 2.0, 1.6, 2, display, channel, "rem_S>~2.0");
		}
	}
	if (channel==3) {
		cut_A_S_upper(0.013, 0.0, 0.020, 0.0, display, channel, "rem_smallA_largeS");
		cut_A_S_lower(0.013, 0.00059, 0.055, 0.0041, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.055, 0.0041, 1.2, 0.20, display, channel, "rem_largeA_smallS");
		if (aggressiveness == 1) {
			cut_A_S_upper(1.2, 0, 2, 0, display, channel, "rem_A>1.2");
			cut_A_S_upper(0.0, 2.0, 1.6, 2, display, channel, "rem_S>2.0");
		}
	}
	if (channel==4) {
		cut_A_S_upper(0.013, 0.0, 0.0196, 0.0, display, channel, "rem_smallA_largeS_1");
		cut_A_S_lower(0.0165, 0.00105, 0.055, 0.0034, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.055, 0.0034, 1.6, 0.26, display, channel, "rem_largeA_smallS");
		if (aggressiveness == 1) {
			cut_A_S_upper(1.15, 0, 2, 0, display, channel, "rem_A>1.15");
			cut_A_S_upper(0.0, 2.0, 1.6, 2.0, display, channel, "rem_S>2.0");
		}
	}
	if (channel==5) {
		if (aggressiveness == 1) {
			cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
			cut_A_S_upper(0.0, 0.015, 1.6, 0.015, display, channel, "rem_S>0.015");
		}
	}
	if (channel==6) {
		if (aggressiveness == 1) {
			cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
			cut_A_S_upper(0.0, 0.015, 1.6, 0.015, display, channel, "rem_S>0.015");
		}
	}
	if (channel==7) {
		if (aggressiveness == 1) {
			cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
			cut_A_S_upper(0.0, 0.015, 1.6, 0.015, display, channel, "rem_S>0.015");
		}
	}
	if (channel==8) {
		if (aggressiveness == 1) {
			cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
			cut_A_S_upper(0.0, 0.015, 1.6, 0.015, display, channel, "rem_S>0.015");
		}
	}
	//qewr
	//The following are set by hand for 20kV individually, there is no other way.
	if (channel==32) {
		cut_A_S_lower(0.0105, 0.00111, 0.05, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0105, 0.0105, 0.00063, 0.06, 0.00575, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0253, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0105, 0.00162, 0.0253, 0.00337, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.008, 0.00103, 0.023, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0085, 0.008, 0.00042, 0.06, 0.006, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0205, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.008, 0.0013, 0.0205, 0.0029, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0115, 0.00138, 0.027, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0115, 0.0115, 0.00071, 0.06, 0.00562, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0274, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0115, 0.00175, 0.0274, 0.0036, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0110, 0.00116, 0.06, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0110, 0.0110, 0.00050, 0.06, 0.00565, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0110, 0.00185, 0.027, 0.004, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0112, 0.00120, 0.027, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0115, 0.000555, 0.06, 0.00623, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0112, 0.00185, 0.026, 0.004, display, channel, "2pe_merged");
	}
	if (channel==37) {
		cut_A_S_lower(0.0095, 0.00111, 0.030, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0095, 0.0095, 0.00058, 0.06, 0.00583, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0216, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0095, 0.0017, 0.0216, 0.00342, display, channel, "2pe_merged");
	}
	if (channel==38) {
		cut_A_S_lower(0.012, 0.00129, 0.03, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.012, 0.012, 0.00034, 0.06, 0.00591, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.012, 0.002, 0.0285, 0.0042, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0094, 0.00111, 0.06, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0094, 0.0094, 0.00044, 0.06, 0.0059, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0247, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0094, 0.0016, 0.0247, 0.00376, display, channel, "2pe_merged");
	}
	if (channel==40) {
		cut_A_S_lower(0.0104, 0.00122, 0.029, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0104, 0.0104, 0.00034, 0.06, 0.0058, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0263, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0104, 0.00177, 0.0263, 0.00364, display, channel, "2pe_merged");
	}
	if (channel==41) {
		cut_A_S_lower(0.0110, 0.00108, 0.038, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0110, 0.0097, 0.00052, 0.06, 0.00576, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0110, 0.00161, 0.0255, 0.0037, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0112, 0.00113, 0.028, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0112, 0.00071, 0.06, 0.00580, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0112, 0.00167, 0.0255, 0.0037, display, channel, "2pe_merged");
	}
	if (channel == 43) {
		if (post_processor->isMultichannel(post_processor->current_type))
			off_ch(channel);
		cut_A_S_lower(0.0112, 0.00116, 0.028, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0112, 0.00071, 0.06, 0.00580, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0112, 0.00167, 0.0255, 0.0037, display, channel, "2pe_merged");
	}
	if (channel == 44) {
		cut_A_S_lower(0.0095, 0.0010, 0.05, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0095, 0.0095, 0.00036, 0.06, 0.00570, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0221, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0095, 0.00159, 0.0221, 0.0033, display, channel, "2pe_merged");
	}
	if (channel==48) {
		cut_A_S_lower(0.0113, 0.00113, 0.027, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0113, 0.0113, 0.00058, 0.06, 0.00595, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0270, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0113, 0.00174, 0.0270, 0.00375, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0086, 0.00095, 0.026, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0086, 0.0086, 0.00050, 0.06, 0.006, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0086, 0.00137, 0.02, 0.0028, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0085, 0.00087, 0.06, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0085, 0.0085, 0.00042, 0.06, 0.00575, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0196, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0085, 0.00153, 0.0196, 0.00279, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0097, 0.00106, 0.032, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0097, 0.0097, 0.00045, 0.06, 0.00575, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0248, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0097, 0.00172, 0.0248, 0.00337, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0107, 0.00120, 0.040, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0107, 0.0107, 0.00055, 0.06, 0.0061, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0258, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0107, 0.00185, 0.0258, 0.00384, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0114, 0.00132, 0.022, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0114, 0.0114, 0.00032, 0.06, 0.00595, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0114, 0.00196, 0.028, 0.00436, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0100, 0.00103, 0.02, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0100, 0.0100, 0.00063, 0.06, 0.0058, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0100, 0.00161, 0.023, 0.00325, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0094, 0.00140, 0.0172, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0102, 0.0094, 0.00040, 0.06, 0.0057, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0235, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0094, 0.00174, 0.0235, 0.0034, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.0116, 0.00108, 0.020, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0116, 0.0116, 0.00077, 0.06, 0.0057, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0116, 0.00170, 0.0255, 0.0038, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0107, 0.00124, 0.029, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0107, 0.0107, 0.00066, 0.06, 0.00595, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0107, 0.00151, 0.0255, 0.0035, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0103, 0.00119, 0.04, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0103, 0.0103, 0.000554, 0.06, 0.00607, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0246, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0103, 0.00182, 0.0246, 0.00367, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0114, 0.00119, 0.022, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0114, 0.0114, 0.00074, 0.06, 0.00575, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0114, 0.00167, 0.0255, 0.00364, display, channel, "2pe_merged");
	}

}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2021.12.16
//Event selection using only SiPM-matrix (forms_Alpha_peak)
//Not using trigger adjustement or trigger-related cuts.
//Modified 5analysis_history.cpp
//Studing x-y coordinate distribution and Npe profile for SiPM-matrix for alpha events
//Studing these distributions for fast only, slow+long only, slow only and pre-trigger photoelectons
data_output_path = "210121/results_v6/";
calibration_file = "210121/results_v5/210121_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times; //adsf
S2_times["210121_Pu_20kV_850V_46V_12dB"] = std::pair<double, double> (25.0, 30.2);
S2_times["210121_Pu_18kV_850V_46V_12dB"] = std::pair<double, double> (22, 33);
S2_times["210121_Pu_16kV_850V_46V_12dB"] = std::pair<double, double> (22, 33);
S2_times["210121_Pu_15kV_850V_46V_12dB"] = std::pair<double, double> (22, 33);
S2_times["210121_Pu_14kV_850V_46V_12dB"] = std::pair<double, double> (22, 32);
S2_times["210121_Pu_13kV_850V_46V_12dB"] = std::pair<double, double> (22, 32);
S2_times["210121_Pu_12kV_850V_46V_12dB"] = std::pair<double, double> (22, 32);
S2_times["210121_Pu_11kV_850V_46V_12dB"] = std::pair<double, double> (22, 32);
S2_times["210121_Pu_10kV_850V_46V_12dB"] = std::pair<double, double> (22, 32.5);
S2_times["210121_Pu_8.9kV_850V_46V_12dB"] = std::pair<double, double> (22, 33);

std::map<std::string, std::string> exp_folders;
exp_folders["210121_Pu_20kV_850V_46V_12dB"] = "Pu_46V_20kV_850V";
exp_folders["210121_Pu_18kV_850V_46V_12dB"] = "Pu_46V_18kV_850V";
exp_folders["210121_Pu_16kV_850V_46V_12dB"] = "Pu_46V_16kV_850V";
exp_folders["210121_Pu_15kV_850V_46V_12dB"] = "Pu_46V_15kV_850V";
exp_folders["210121_Pu_14kV_850V_46V_12dB"] = "Pu_46V_14kV_850V";
exp_folders["210121_Pu_13kV_850V_46V_12dB"] = "Pu_46V_13kV_850V";
exp_folders["210121_Pu_12kV_850V_46V_12dB"] = "Pu_46V_12kV_850V";
exp_folders["210121_Pu_11kV_850V_46V_12dB"] = "Pu_46V_11kV_850V";
exp_folders["210121_Pu_10kV_850V_46V_12dB"] = "Pu_46V_10kV_850V";
exp_folders["210121_Pu_8.9kV_850V_46V_12dB"] = "Pu_46V_8.9kV_850V";

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
if (calibrate) {
	std::cout<<"Use 5analysis_history.cpp for calibration! Doing nothing."<<std::endl;
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
		d_S2_start = S2_times_entry->second.first;
		d_S2_finish = S2_times_entry->second.second;
		S2_start = dbl_to_str(d_S2_start, 1);
		S2_finish = dbl_to_str(d_S2_finish, 1);
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
if (exp == "210121_Pu_20kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only(); cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only(); cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only(); cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(0, 160);
	set_bins(0, 1000);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 800, 0, 800);
	set_bins(800);
	x_y_regions = {207, 0, 384, 69, 486, 94, 799, 273};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	double slow_cutoff = 45;//us
	std::string s_slow_cutoff = dbl_to_str(slow_cutoff, 0);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160);
	draw_limits(347, 490);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-160us_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update(); //Slow component only
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_coord);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-15, 15, -15, 15);	set_bins(99);
	saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_coord_x);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-15, 15); set_bins(149);
	saveaspng(FOLDER + Num + "_SiPMs_x_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_x_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_x_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_x_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_x_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_coord_y);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-15, 15);	set_bins(149);
	saveaspng(FOLDER + Num + "_SiPMs_y_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_y_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_y_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_y_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_y_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_profile);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-22.5, 22.5, -22.5, 22.5); set_bins(9, 9);
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_profile_x);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-22.5, 22.5); set_bins(9);
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_profile_y);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-22.5, 22.5); set_bins(9);
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_coord);
	time_zoom_SiPMs(d_S2_start, slow_cutoff);
	x_y_regions = {-0.90, -1.80, 0.90, -1.80, 1.60, 0, 0.90, 1.80, -0.90, 1.80, -1.60, 0.0};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_"+S2_start+"-"+s_slow_cutoff+"us");
	set_as_run_cut("SiPMs_0_xy_coordinate"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::MPPC_Npe_profile);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-22.5, 22.5, -22.5, 22.5); set_bins(9, 9);
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_xy_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_profile_x);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-22.5, 22.5); set_bins(9);
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_x_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_profile_y);
	cut_SiPM_noise(SiPM_state);
	time_zoom_SiPMs(d_S2_start, 160);
	set_zoom(-22.5, 22.5); set_bins(9);
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_4-"+S2_start+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, slow_cutoff); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_finish+"-"+s_slow_cutoff+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160); update();
	saveaspng(FOLDER + Num + "_SiPMs_profile_y_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);

}
//END OF FORMS
}
