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
			cut_A_S_lower(0.020, 0.0035, 0.06, 0.0071, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.06, 0.0071, 1.4, 0.25, display, channel, "rem_largeA_smallS");
		}
		if (channel==2) {
			//cut_A_S_upper(0.005, 0.0, 0.023, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.0122, 0.0011, 0.055, 0.0036, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.055, 0.0036, 1.6, 0.27, display, channel, "rem_largeA_smallS");
		}
		if (channel==3) {
			//cut_A_S_upper(0.005, 0.0, 0.0139, 0.0, display, channel, "rem_smallA_largeS");
			cut_A_S_lower(0.01, 0.00135, 0.030, 0.0020, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.030, 0.0020, 1.2, 0.18, display, channel, "rem_largeA_smallS");
		}
		if (channel==4) {
			//cut_A_S_upper(0.01, 0.0, 0.0171, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.01, 0.00051, 0.035, 0.0023, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.035, 0.0023, 1.6, 0.22, display, channel, "rem_largeA_smallS");
		}
		if (channel==5) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.65, 0, 5, 0, display, channel, "rem_A>0.65");
				cut_A_S_upper(0.0, 0.030, 1.6, 0.030, display, channel, "rem_S>0.030");
			}
		}
		if (channel==6) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.50, 0, 5, 0, display, channel, "rem_A>0.50");
				cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
			}
		}
		if (channel==7) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.54, 0, 2, 0, display, channel, "rem_A>0.54");
				cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
			}
		}
		if (channel==8) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.54, 0, 2, 0, display, channel, "rem_A>0.54");
				cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
			}
		}
	}
	//qewr
	//The following are set by hand for 20kV individually, there is no other way.
	if (channel==32) {
		cut_A_S_lower(0.0062, 0.00106, 0.02, 0.00106, display, channel, "small_A-S_noise");
		x_y_regions = {0.0062, 0.0062, 0.00061, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0156, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0062, 0.00163, 0.0156, 0.00353, display, channel, "2pe_merged");
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
		cut_A_S_lower(0.0071, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
		x_y_regions = {0.0071, 0.0071, 0.00072, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0168, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0071, 0.00183, 0.0168, 0.00366, display, channel, "2pe_merged");
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
		x_y_regions = {0.0074, 0.0074, 0.00084, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0167, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00191, 0.0167, 0.00387, display, channel, "2pe_merged");
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
		cut_A_S_lower(0.0069, 0.00089, 0.02, 0.00089, display, channel, "small_A-S_noise");
		x_y_regions = {0.0069, 0.0069, 0.00040, 0.05, 0.0091, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0182, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0069, 0.00181, 0.0182, 0.00419, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0062, 0.00090, 0.02, 0.00090, display, channel, "small_A-S_noise");
		x_y_regions = {0.0062, 0.0066, 0.00060, 0.05, 0.0091, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0062, 0.00179, 0.0159, 0.00361, display, channel, "2pe_merged");
	}
	if (channel==40) {
		cut_A_S_lower(0.0066, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.0066, 0.00052, 0.05, 0.0089, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0165, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00191, 0.0165, 0.00377, display, channel, "2pe_merged");
	}
	if (channel==41) {
		cut_A_S_lower(0.0064, 0.00089, 0.02, 0.00089, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00055, 0.05, 0.0089, 1e3};
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
		cut_A_S_lower(0.0049, 0.00082, 0.02, 0.00082, display, channel, "small_A-S_noise");
		x_y_regions = {0.0049, 0.0053, 0.00058, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0125, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0049, 0.00142, 0.0125, 0.00285, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0060, 0.00087, 0.02, 0.00087, display, channel, "small_A-S_noise");
		x_y_regions = {0.0060, 0.0060, 0.00050, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0060, 0.00162, 0.0155, 0.00340, display, channel, "2pe_merged");
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
		cut_A_S_lower(0.0072, 0.00103, 0.02, 0.00103, display, channel, "small_A-S_noise");
		x_y_regions = {0.0072, 0.0072, 0.00058, 0.05, 0.0081, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0184, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0072, 0.00194, 0.0184, 0.00416, display, channel, "2pe_merged");
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
		cut_A_S_lower(0.0065, 0.00095, 0.02, 0.00095, display, channel, "small_A-S_noise");
		x_y_regions = {0.0065, 0.0065, 0.00047, 0.05, 0.0088, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0065, 0.00181, 0.0159, 0.00359, display, channel, "2pe_merged");
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
		cut_A_S_lower(0.0066, 0.00099, 0.02, 0.00099, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.0066, 0.00058, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0160, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00180, 0.0160, 0.00352, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0067, 0.00111, 0.02, 0.00111, display, channel, "small_A-S_noise");
		x_y_regions = {0.0067, 0.0067, 0.00057, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0164, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0067, 0.00180, 0.0164, 0.00360, display, channel, "2pe_merged");
	}

}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2022.09.26
//New standard analysis.
//Valid events and Pu peak events are selected from max Npe: SiPM matrix in this case
//Not using trigger adjustement or trigger-related cuts.
//Removed pre-trigger cut, using 2D cut on whole 0-160us range instead
//bacause previous approach (separate cut for pre-trigger region) may systematically increase or decrease long component.
//In order to determine its threshold more precisely, pre-trigger and long component
//areas must be treated in the same manner.
//For the same reason, Pu/Cd peaks are now selected using 0-160us. Otherwise, baseline (pre-trigger area of pulse-shape)
//for peak and low-E peak slope are diffent resulting in different long components.
//Same as 220915/7analysis_history.cpp

//Setup: ~18mm gap, alphas from 238Pu, 75% Electroconnect's THGEM used for cathode and THGEM1,
//28% CERN's THGEM as THGEM0 with 10 MOhm, all PMTs with high gain (3 old PMTs which we in use
//till ~ May 2019 and new PMTs as #4). 600 MOhm part of V0 divider is inside EL gap, covers PMT#1.
//No WLS. 1.25 atm pressures of argon in EL gap.

data_output_path = "220922/results_v7/";
calibration_file = "220922/results_v7/220922_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times; //adsf
S2_times["220922_Pu_1.25atm_16.3mm_22.0kV_850V_46V"] = std::pair<double, double> (25.0, 30.3);
S2_times["220922_Pu_1.25atm_16.3mm_20.9kV_850V_46V"] = std::pair<double, double> (25.0, 30.5);
S2_times["220922_Pu_1.25atm_16.3mm_20.2kV_850V_46V"] = std::pair<double, double> (25.0, 30.5);
S2_times["220922_Pu_1.25atm_16.3mm_19.6kV_850V_46V"] = std::pair<double, double> (25.0, 30.6);
S2_times["220922_Pu_1.25atm_16.3mm_19.0kV_850V_46V"] = std::pair<double, double> (25.0, 31.0);
S2_times["220922_Pu_1.25atm_16.3mm_18.4kV_850V_46V"] = std::pair<double, double> (25.0, 31.1);
S2_times["220922_Pu_1.25atm_16.3mm_17.2kV_850V_46V"] = std::pair<double, double> (25.0, 31.3);
S2_times["220922_Pu_1.25atm_16.3mm_16.0kV_850V_46V"] = std::pair<double, double> (24.5, 31.5);
S2_times["220922_Pu_1.25atm_16.3mm_14.7kV_850V_46V"] = std::pair<double, double> (24.5, 32.0);
S2_times["220922_Pu_1.25atm_16.3mm_13.5kV_850V_46V"] = std::pair<double, double> (24.5, 32.3);
S2_times["220922_Pu_1.25atm_16.3mm_12.3kV_850V_46V"] = std::pair<double, double> (24.5, 33.1);
S2_times["220922_Pu_1.25atm_16.3mm_11.0kV_850V_46V"] = std::pair<double, double> (24.0, 33.2);

std::map<std::string, std::string> exp_folders;
exp_folders["220922_Pu_1.25atm_16.3mm_22.0kV_850V_46V"] = "Pu_1.25atm_22.0kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_20.9kV_850V_46V"] = "Pu_1.25atm_20.9kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_20.2kV_850V_46V"] = "Pu_1.25atm_20.2kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_19.6kV_850V_46V"] = "Pu_1.25atm_19.6kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_19.0kV_850V_46V"] = "Pu_1.25atm_19.0kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_18.4kV_850V_46V"] = "Pu_1.25atm_18.4kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_17.2kV_850V_46V"] = "Pu_1.25atm_17.2kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_16.0kV_850V_46V"] = "Pu_1.25atm_16.0kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_14.7kV_850V_46V"] = "Pu_1.25atm_14.7kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_13.5kV_850V_46V"] = "Pu_1.25atm_13.5kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_12.3kV_850V_46V"] = "Pu_1.25atm_12.3kV_46V_850V";
exp_folders["220922_Pu_1.25atm_16.3mm_11.0kV_850V_46V"] = "Pu_1.25atm_11.0kV_46V_850V";

std::map<std::string, double> pre_trigger_max_S; //for calibration only
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_22.0kV_850V_46V"] = 0.086;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_20.9kV_850V_46V"] = 0.082;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_20.2kV_850V_46V"] = 0.069;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_19.6kV_850V_46V"] = 0.064;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_19.0kV_850V_46V"] = 0.059;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_18.4kV_850V_46V"] = 0.054;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_17.2kV_850V_46V"] = 0.050;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_16.0kV_850V_46V"] = 0.036;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_14.7kV_850V_46V"] = 0.035;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_13.5kV_850V_46V"] = 0.026;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_12.3kV_850V_46V"] = 0.025;
pre_trigger_max_S["220922_Pu_1.25atm_16.3mm_11.0kV_850V_46V"] = 0.025;

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
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

ty(AStates::MPPC_S_sum); //nex();
time_zoom_SiPMs(0, d_S2_start);
set_zoom(0, 0.3);
set_bins(500); set_log_y();
draw_limits(0, d_S_max);
saveaspng(data_output_path + "calibration/" + folder +"/SiPMs_pre-trigger_cut");
set_as_run_cut("small_pre-trigger");

for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) {
int channel = calib_channels[chi];
std::string ch_str = int_to_str(channel);
//parameters set by Pu_46V_20kV_850V, but valid for all fields.
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
		cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveaspng(data_output_path + "calibration/" + folder +"/"+ch_str+"_Ss_2_single_pe_w_0-"+S2_start+"us");
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
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.14);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.12);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.14);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.14);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
		set_bins(130, 300);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
  	set_zoom(0, 0.1, 0, 0.006);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(130, 400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0015);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
  	set_zoom(0, 0.1, 0, 0.006);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(60, 200);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.50, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.1, 0, 0.006);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(130, 500);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
if (exp == "220922_Pu_1.25atm_16.3mm_22.0kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	x_y_regions = {113, 0, 211, 45, 291, 66, 362, 111, 599, 190};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {50, 0, 218, 32, 280, 77, 399, 141};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(220, 268);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(220, 312);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_20.9kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	x_y_regions = {100, 0, 166, 40, 250, 62, 308, 109, 499, 171};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {50, 0, 182, 31, 248, 81, 399, 164};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(177, 223);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(177, 269);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_20.2kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	x_y_regions = {87, 0, 150, 38, 234, 64, 269, 101, 499, 180};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {50, 0, 171, 33, 399, 198};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(163, 202);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(163, 243);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_19.6kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {75, 0, 135, 38, 201, 55, 256, 100, 399, 150};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {50, 0, 159, 33, 399, 199};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(142, 182);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(142, 220);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_19.0kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {63, 0, 109, 27, 192, 62, 232, 102, 399, 164};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {50, 0, 155, 34, 399, 209};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(129, 167);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(129, 200);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_18.4kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {57, 0, 101, 26.0, 165, 54, 213, 96, 399, 187};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {39.5, 0, 147.4, 36.4, 299, 157};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(115, 149);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(115, 179);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_17.2kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {43.6, 0, 75.6, 20.9, 133.8, 52.0, 299, 171};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {39.5, 0, 127.1, 34.5, 299, 189};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(100, 122);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(100, 142.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_16.0kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {33.9, 0, 59.0, 19.9, 109.8, 47.6, 140.2, 80.7, 299, 183};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {25.3, 0, 106.8, 30.1, 199, 117};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(73, 98);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(73, 120);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_14.7kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {24.1, 0, 44.0, 18.4, 85.0, 40.8, 123.3, 80.7, 199, 131};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {25.3, 0, 94.2, 28.2, 199, 140};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(58, 80.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(58, 98.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_13.5kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {16.3, 0, 34.4, 15.8, 70.4, 37.6, 104.2, 76.2, 199, 151};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {40.6, 0, 90.0, 32.4, 199, 146};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(47, 65.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(47, 82.1);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_12.3kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {10.5, 0, 54.4, 29.8, 199, 171};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {40.6, 0, 74.7, 22.3, 199, 154};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(38, 54.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(38, 65.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "220922_Pu_1.25atm_16.3mm_11.0kV_850V_46V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {10.1, 0, 41.6, 21.0, 199, 181};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {49.9, 0, 85.2, 27.5, 199, 156.5};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(29, 46.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(29, 59.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
