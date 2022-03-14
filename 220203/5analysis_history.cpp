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
	if (0 == device_condition) {
		if (channel==1) {
			//cut_A_S_upper(0.005, 0.0, 0.0132, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.010, 0.0004, 0.030, 0.0021, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.030, 0.0021, 1.1, 0.18, display, channel, "rem_largeA_smallS");
		}
		if (channel==2) {
			//cut_A_S_upper(0.005, 0.0, 0.023, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.022, 0.0, 1.6, 0.15, display, channel, "rem_smallA_smallS");
		}
		if (channel==3) {
			cut_A_S_upper(0.005, 0.0, 0.0112, 0.0, display, channel, "rem_smallA_largeS");
			cut_A_S_lower(0.01, 0.0003, 0.025, 0.0014, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.025, 0.0014, 1.2, 0.18, display, channel, "rem_largeA_smallS");
		}
		if (channel==4) {
			//cut_A_S_upper(0.01, 0.0, 0.0180, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.01, 0.00051, 0.035, 0.0012, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.035, 0.0012, 1.6, 0.15, display, channel, "rem_largeA_smallS");
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
	}
	//qewr
	//The following are set by hand for 20kV individually, there is no other way.
	if (channel==32) {
		cut_A_S_lower(0.0062, 0.00106, 0.02, 0.00106, display, channel, "small_A-S_noise");
		x_y_regions = {0.0062, 0.0062, 0.00077, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0153, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0062, 0.00163, 0.0153, 0.00353, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.0056, 0.00101, 0.02, 0.00101, display, channel, "small_A-S_noise");
		x_y_regions = {0.0056, 0.0056, 0.00072, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0056, 0.00155, 0.0132, 0.00304, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0074, 0.00128, 0.02, 0.00128, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, 0.00101, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0173, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00171, 0.0173, 0.00376, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0069, 0.00115, 0.02, 0.00115, display, channel, "small_A-S_noise");
		x_y_regions = {0.0069, 0.0069, 0.00083, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0164, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0069, 0.00181, 0.0164, 0.00365, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0074, 0.00132, 0.02, 0.00132, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, 0.00100, 0.05, 0.0094, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0173, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00191, 0.0173, 0.00387, display, channel, "2pe_merged");
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
		cut_A_S_lower(0.0079, 0.00136, 0.02, 0.00136, display, channel, "small_A-S_noise");
		x_y_regions = {0.0079, 0.0079, 0.00086, 0.05, 0.0095, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0183, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0079, 0.00214, 0.0183, 0.00407, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0066, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.0066, 0.00068, 0.05, 0.0094, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0163, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00191, 0.0163, 0.00370, display, channel, "2pe_merged");
	}
	if (channel==40) {
		cut_A_S_lower(0.0066, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.0066, 0.00068, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0165, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00191, 0.0165, 0.00377, display, channel, "2pe_merged");
	}
	if (channel==41) {
		cut_A_S_lower(0.0064, 0.00103, 0.02, 0.00103, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0161, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00191, 0.0161, 0.00371, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0064, 0.00103, 0.02, 0.00103, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0161, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00191, 0.0161, 0.00371, display, channel, "2pe_merged");
	}
	if (channel==43) {
		cut_A_S_lower(0.0064, 0.00106, 0.02, 0.00106, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0167, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00191, 0.0167, 0.00388, display, channel, "2pe_merged");
	}
	if (channel==44) {
		cut_A_S_lower(0.0064, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00084, 0.05, 0.0087, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0142, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00178, 0.0142, 0.00317, display, channel, "2pe_merged");
		//if (post_processor->isMultichannel(post_processor->current_type)) {
		//	off_ch(channel);
		//}
	}
	if (channel==48) {
		cut_A_S_lower(0.0070, 0.00123, 0.02, 0.00123, display, channel, "small_A-S_noise");
		x_y_regions = {0.0070, 0.0070, 0.00091, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0168, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0070, 0.00163, 0.0168, 0.00366, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0055, 0.00097, 0.02, 0.00097, display, channel, "small_A-S_noise");
		x_y_regions = {0.0055, 0.0055, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0125, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0055, 0.00146, 0.0125, 0.00285, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0053, 0.00097, 0.02, 0.00097, display, channel, "small_A-S_noise");
		x_y_regions = {0.0053, 0.0053, 0.00058, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0125, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0053, 0.00148, 0.0125, 0.00285, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0064, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00068, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00167, 0.0155, 0.00340, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0071, 0.00121, 0.02, 0.00121, display, channel, "small_A-S_noise");
		x_y_regions = {0.0071, 0.0071, 0.00081, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0170, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0071, 0.00186, 0.0170, 0.00384, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0083, 0.00131, 0.02, 0.00131, display, channel, "small_A-S_noise");
		x_y_regions = {0.0083, 0.0083, 0.00100, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0186, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0083, 0.00210, 0.0186, 0.00415, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0061, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
		x_y_regions = {0.0061, 0.0061, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0148, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0061, 0.00160, 0.0148, 0.00329, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0061, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
		x_y_regions = {0.0061, 0.0061, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0151, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0061, 0.00164, 0.0151, 0.00335, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.0066, 0.00115, 0.02, 0.00115, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.0066, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0165, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00186, 0.0165, 0.00369, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0069, 0.00120, 0.02, 0.00120, display, channel, "small_A-S_noise");
		x_y_regions = {0.0069, 0.0069, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0167, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0069, 0.00183, 0.0167, 0.00363, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0067, 0.00115, 0.02, 0.00115, display, channel, "small_A-S_noise");
		x_y_regions = {0.0067, 0.0067, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0160, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0067, 0.00180, 0.0160, 0.00353, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0067, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
		x_y_regions = {0.0067, 0.0067, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0164, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0067, 0.00180, 0.0164, 0.00360, display, channel, "2pe_merged");
	}
}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2022.02.07
//S1 + S2 in LAr in THGEM1
//Standard pulsed X-ray analysis, no event selection
//Not using trigger adjustement or trigger-related cuts.
//Output is signal pulse-shapes and Npe for S2
data_output_path = "220203/results_v5/";
calibration_file = "220203/results_v5/220203_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["220203_X-ray_20kV_850V_46V_12dB_6180V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_5993V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_5738V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_5297V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_4856V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_4413V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_3972V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_3531V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_3090V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_2648V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_2206V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_1765V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_0V_6180V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_0V_5736V"] = std::pair<double, double> (44, 78);
S2_times["220203_X-ray_20kV_850V_46V_12dB_0V"] = std::pair<double, double> (44, 78);

std::map<std::string, std::pair<double, double> > S1_times;
S1_times["220203_X-ray_20kV_850V_46V_12dB_6180V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_5993V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_5738V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_5297V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_4856V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_4413V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_3972V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_3531V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_3090V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_2648V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_2206V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_1765V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_0V_6180V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_0V_5736V"] = std::pair<double, double> (31.5, 44);
S1_times["220203_X-ray_20kV_850V_46V_12dB_0V"] = std::pair<double, double> (31.5, 44);

std::map<std::string, std::string> exp_folders;
exp_folders["220203_X-ray_20kV_850V_46V_12dB_6180V"] = "X-ray_46_20kV_850V_6180V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_5993V"] = "X-ray_46_20kV_850V_5993V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_5738V"] = "X-ray_46_20kV_850V_5738V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_5297V"] = "X-ray_46_20kV_850V_5297V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_4856V"] = "X-ray_46_20kV_850V_4856V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_4413V"] = "X-ray_46_20kV_850V_4413V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_3972V"] = "X-ray_46_20kV_850V_3972V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_3531V"] = "X-ray_46_20kV_850V_3531V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_3090V"] = "X-ray_46_20kV_850V_3090V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_2648V"] = "X-ray_46_20kV_850V_2648V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_2206V"] = "X-ray_46_20kV_850V_2206V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_1765V"] = "X-ray_46_20kV_850V_1765V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_0V_6180V"] = "X-ray_46_20kV_850V_0V_6180V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_0V_5736V"] = "X-ray_46_20kV_850V_0V_5736V";
exp_folders["220203_X-ray_20kV_850V_46V_12dB_0V"] = "X-ray_46_20kV_850V_0V";

std::map<std::string, double> pre_trigger_max_S; //for calibration only
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_6180V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_5993V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_5738V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_5297V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_4856V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_4413V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_3972V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_3531V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_3090V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_2648V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_2206V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_1765V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_0V_6180V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_0V_5736V"] = 0.06;
pre_trigger_max_S["220203_X-ray_20kV_850V_46V_12dB_0V"] = 0.06;


bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish, S1_start, S1_finish;
double d_S2_start, d_S2_finish;
double d_S1_start, d_S1_finish;
double d_S_max;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //850V, 12 dB
int SiPM_state = 0; //46V
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
		S2_start = dbl_to_str(d_S2_start, 1);
		S2_finish = dbl_to_str(d_S2_finish, 1);
} else {
    std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
    nex();
    continue;
}
auto S1_times_entry = S1_times.find(exp);
if (S1_times_entry != S1_times.end()) {
    d_S1_start = S1_times_entry->second.first;
    d_S1_finish = S1_times_entry->second.second;
		S1_start = dbl_to_str(d_S1_start, 1);
		S1_finish = dbl_to_str(d_S1_finish, 1);
} else {
    std::cout<<"Could not find S1 time limits for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
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
if (channel>=32) { //adsf
		if (getIndex(post_processor->MPPC_channels, channel)<0)
			continue;
ty(AStates::MPPC_A_S);
		ch(channel);
    set_zoom(0, 0.05, 0, 0.01);
    set_bins(500);
    noise_cut(channel, 1, SiPM_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_zoom");
		set_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, SiPM_state, false);
		cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_single_pe_w_0-"+S1_start+"us");
}
if (channel==1) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.1, 0, 2.5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.04); set_log_z();
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom_log");
		set_bins(500);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.040);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_3_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==2) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.2, 0, 2.5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
    set_zoom(0, 0.15, 0, 0.05);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
	 	set_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
    set_bins(200);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_4_zoom_log");
		unset_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_5_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.1);
    set_bins(600);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.08);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_3_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==3) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.2, 0, 2.5);
    set_bins(1000);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
		set_zoom(0, 0.12, 0, 0.04);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.03);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_3_w_0-"+S1_start+"us");
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
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.04);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
    saveas(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.08);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_3_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==5) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.35, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
    set_zoom(0, 0.06, 0, 0.003);
    saveas(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
		set_log_z();
    saveas(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
		set_bins(130, 300);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.001);
    set_bins(800);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.0004);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==6) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.35, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
  	set_zoom(0, 0.06, 0, 0.003);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
    set_bins(130, 400);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.0012);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_0-"+S1_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==7) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.35, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
  	set_zoom(0, 0.03, 0, 0.003);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
    set_bins(60, 200);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.001);
    set_bins(800);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.0005);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_0-"+S1_start+"us");
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
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_1_");
    set_zoom(0, 0.06, 0, 0.003);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_3_zoom_log");
    set_bins(130, 500);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S1_start, false, channel);
    draw_limits(0.0, 0.0016);
    saveaspng(data_output_path + "calibration/" + folder + "/" + ch_str+"_Ss_2_w_0-"+S1_start+"us");
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
std::string folder, S2_start, S2_finish, S1_start, S1_finish;;
double d_S1_start, d_S1_finish;
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
auto S1_times_entry = S1_times.find(exp);
if (S1_times_entry != S1_times.end()) {
		d_S1_start = S1_times_entry->second.first;
		d_S1_finish = S1_times_entry->second.second;
		S1_start = dbl_to_str(d_S1_start, 1);
		S1_finish = dbl_to_str(d_S1_finish, 1);
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
if (exp == "220203_X-ray_20kV_850V_46V_12dB_6180V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_5993V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_5738V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_5297V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_4856V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_4413V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_3972V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_3531V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_3090V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_2648V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_2206V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_1765V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_0V_6180V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_0V_5736V"
		|| exp == "220203_X-ray_20kV_850V_46V_12dB_0V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	if (exp == "220203_X-ray_20kV_850V_46V_12dB_6180V" || exp == "220203_X-ray_20kV_850V_46V_12dB_5993V") {
		ty(AStates::PMT_Npe_sum);
		slow_PMTs_only();
		cut_PMT_noise(PMT_state);
		set_bins(0, 200);
		time_zoom_sPMTs(1, d_S1_start); update();
		draw_limits(0, 22);
		saveaspng(FOLDER + Num+"_slowPMTs_Npe_1.0-"+S1_start+"us_"+cuts_str(cuts));
		set_as_run_cut("no_discharge"); cuts.push_back(Num);
		unset_draw_limits();
		Num = int_to_str(++no, 2);
	}

	std::string form_n = "forms_X-ray/";
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	print_accepted_events(FOLDER + form_n + "_events.txt", first_run);

ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	set_bins(0, 200);

	time_zoom_sPMTs(1, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_1.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-"+S2_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	set_bins(0, 300);

	time_zoom_SiPMs(1, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_1.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	if (exp == "220203_X-ray_20kV_850V_46V_12dB_6180V"
			|| exp == "220203_X-ray_20kV_850V_46V_12dB_5993V"
			|| exp == "220203_X-ray_20kV_850V_46V_12dB_5738V")
		set_bins(0, 400);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-"+S2_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
}

//END OF FORMS
}
