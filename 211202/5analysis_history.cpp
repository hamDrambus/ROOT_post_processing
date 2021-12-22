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
	/*on_ch(5);*/ on_ch(6); on_ch(7); on_ch(8);
	off_ch(0); /*off_ch(1);*/ off_ch(2); off_ch(3); off_ch(4);
}

void slow_PMTs_only (void) {
	/*on_ch(1);*/ on_ch(2); on_ch(3); on_ch(4);
	off_ch(0); /*off_ch(5);*/ off_ch(6); off_ch(7); off_ch(8);
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
			cut_A_S_upper(0.005, 0.0, 0.0132, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.010, 0.0007, 0.030, 0.0021, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.030, 0.0021, 1.1, 0.20, display, channel, "rem_largeA_smallS");
		}
		if (channel==2) {
			cut_A_S_upper(0.005, 0.0, 0.023, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.0122, 0.00057, 0.055, 0.0036, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.055, 0.0036, 1.6, 0.27, display, channel, "rem_largeA_smallS");
		}
		if (channel==3) {
			cut_A_S_upper(0.005, 0.0, 0.0139, 0.0, display, channel, "rem_smallA_largeS");
			cut_A_S_lower(0.01, 0.00135, 0.030, 0.0022, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.030, 0.0022, 1.2, 0.24, display, channel, "rem_largeA_smallS");
		}
		if (channel==4) {
			cut_A_S_upper(0.01, 0.0, 0.0171, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.01, 0.00051, 0.035, 0.0023, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.035, 0.0023, 1.6, 0.26, display, channel, "rem_largeA_smallS");
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
		cut_A_S_lower(0.0089, 0.00106, 0.03, 0.00053, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00016, 0.06, 0.0050, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0244, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00136, 0.0244, 0.00324, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.008, 0.00079, 0.023, 0.00079, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.008, 0.00006, 0.06, 0.00565, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0201, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.008, 0.0012, 0.0201, 0.00262, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0101, 0.00080, 0.025, 0.00147, display, channel, "small_A-S_noise");
		x_y_regions = {0.0101, 0.0101, 0.00039, 0.06, 0.0053, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0269, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0101, 0.00153, 0.0269, 0.00348, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0095, 0.00077, 0.025, 0.00077, display, channel, "small_A-S_noise");
		x_y_regions = {0.0095, 0.0095, 0.00019, 0.06, 0.0052, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0267, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0095, 0.00159, 0.0267, 0.00359, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0100, 0.00092, 0.024, 0.00106, display, channel, "small_A-S_noise");
		x_y_regions = {0.0100, 0.0100, 0.00006, 0.06, 0.00522, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0253, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0100, 0.00162, 0.0253, 0.00393, display, channel, "2pe_merged");
	}
	if (channel==37) {
		cut_A_S_lower(0.0084, 0.00115, 0.020, 0.00053, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00001, 0.06, 0.00540, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0216, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.00165, 0.0216, 0.00293, display, channel, "2pe_merged");
	}
	if (channel==38) {
		cut_A_S_lower(0.0112, 0.00100, 0.024, 0.00100, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0112, 0.00029, 0.06, 0.0033, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0282, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0112, 0.00181, 0.0282, 0.00426, display, channel, "2pe_merged");
	}
	if (channel==39) {
		//cut_A_S_lower(0.0094, 0.00027, 0.06, 0.00027, display, channel, "small_A-S_noise");
		x_y_regions = {0.0101, 0.0101, 0.00061, 0.06, 0.00393, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0241, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0101, 0.00173, 0.0241, 0.00353, display, channel, "2pe_merged");
	}
	if (channel==40) {
		//cut_A_S_lower(0.0104, 0.00122, 0.029, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0112, 0.00055, 0.06, 0.0043, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0263, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0112, 0.00170, 0.0263, 0.00366, display, channel, "2pe_merged");
	}
	if (channel==41) {
		//cut_A_S_lower(0.0107, 0.00108, 0.038, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0107, 0.0107, 0.00058, 0.06, 0.0040, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0107, 0.00173, 0.0255, 0.0038, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0096, 0.00081, 0.020, 0.00095, display, channel, "small_A-S_noise");
		x_y_regions = {0.0096, 0.0096, 0.00037, 0.06, 0.00528, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0253, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0096, 0.00157, 0.0253, 0.00351, display, channel, "2pe_merged");
	}
	if (channel == 44) {
		//cut_A_S_lower(0.0089, 0.0010, 0.05, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00066, 0.06, 0.00272, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0214, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00165, 0.0214, 0.00312, display, channel, "2pe_merged");
		if (post_processor->isMultichannel(post_processor->current_type)) {
			off_ch(channel);
		}
	}
	if (channel==48) {
		//cut_A_S_lower(0.0101, 0.00113, 0.027, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0101, 0.0101, 0.00050, 0.06, 0.0050, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0260, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0101, 0.00131, 0.0260, 0.0035, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0080, 0.00095, 0.026, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.0080, 0.00052, 0.06, 0.0033, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0195, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0080, 0.00124, 0.0195, 0.0027, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0080, 0.00070, 0.02, 0.00087, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.0080, 0.00014, 0.06, 0.00460, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0192, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0080, 0.00152, 0.0192, 0.00259, display, channel, "2pe_merged");
	}
	if (channel==51) {
		//cut_A_S_lower(0.0097, 0.00106, 0.032, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0100, 0.0100, 0.00060, 0.06, 0.0038, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0248, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0100, 0.00163, 0.0248, 0.00337, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0114, 0.00114, 0.024, 0.00111, display, channel, "small_A-S_noise");
		x_y_regions = {0.0114, 0.0114, 0.00056, 0.06, 0.0053, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0258, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0114, 0.00176, 0.0258, 0.00380, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0114, 0.00100, 0.024, 0.00100, display, channel, "small_A-S_noise");
		x_y_regions = {0.0114, 0.0114, 0.00040, 0.06, 0.0035, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0114, 0.00184, 0.028, 0.00421, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0086, 0.00103, 0.02, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0086, 0.0086, 0.00025, 0.06, 0.0048, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0221, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0086, 0.00153, 0.0221, 0.0030, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0089, 0.00088, 0.02, 0.001, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00027, 0.06, 0.0050, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0227, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00156, 0.0227, 0.00314, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.010, 0.00084, 0.020, 0.00084, display, channel, "small_A-S_noise");
		x_y_regions = {0.010, 0.010, 0.00037, 0.06, 0.00405, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0249, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.010, 0.00159, 0.0249, 0.00356, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0090, 0.00124, 0.029, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0090, 0.0090, 0.00034, 0.06, 0.00508, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0090, 0.00137, 0.0255, 0.0035, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0088, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
		x_y_regions = {0.0088, 0.0088, 0.00031, 0.06, 0.00480, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0246, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0088, 0.00153, 0.0246, 0.00367, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0088, 0.0010, 0.02, 0.0010, display, channel, "small_A-S_noise");
		x_y_regions = {0.0088, 0.0088, 0.00027, 0.06, 0.00505, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0088, 0.00152, 0.0255, 0.00364, display, channel, "2pe_merged");
	}

}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2021.12.06
//Almost standard analysis. Event selection using both SiPM-matrix and slow PMTs since the Npe is low
//Pu peak is selected from max Npe: slow PMTs in this case.
//(cold finger + 75% THGEM before SiPMs -> low Npe for SiPM matrix in this detector configuration)
//Not using trigger adjustement or trigger-related cuts.
//Removed pre-trigger cut, using 2D cut on whole 0-160us range instead
//bacause previous approach (separate cut for pre-trigger region) may systematically increase or decrease long component.
//In order to determine its threshold more precisely, pre-trigger and long component
//areas must be treated in the same manner.
//Similar to 211028/5analysis_history.cpp

//!!!! There is slight error in this analysis: PMT#1 is turned off for event selection.
data_output_path = "211202/results_v5/";
calibration_file = "211202/results_v5/211202_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times; //adsf
S2_times["211202_Pu_16kV_850V_46V_12dB_16mm"] = std::pair<double, double> (24, 31.2);
S2_times["211202_Pu_15kV_850V_46V_12dB_16mm"] = std::pair<double, double> (24, 31.2);
S2_times["211202_Pu_15kV_850V_46V_12dB_18mm"] = std::pair<double, double> (24, 31.8);
S2_times["211202_Pu_14kV_850V_46V_12dB_18mm"] = std::pair<double, double> (24, 32.4);
S2_times["211202_Pu_13kV_850V_46V_12dB_18mm"] = std::pair<double, double> (24, 32.7);
S2_times["211202_Pu_12kV_850V_46V_12dB_18mm"] = std::pair<double, double> (24, 32.8);
S2_times["211202_Pu_11kV_850V_46V_12dB_18mm"] = std::pair<double, double> (24, 33.2);
S2_times["211202_Pu_10kV_850V_46V_12dB_18mm"] = std::pair<double, double> (24, 33.7);
S2_times["211202_Pu_9kV_850V_46V_12dB_18mm"] = std::pair<double, double> (23.5, 33.8);
S2_times["211202_Pu_8kV_850V_46V_12dB_18mm"] = std::pair<double, double> (23.5, 34.0);
S2_times["211202_Pu_17.3kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.5, 30.2);
S2_times["211202_Pu_15.6kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.5, 30.3);
S2_times["211202_Pu_13.8kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.5, 30.5);
S2_times["211202_Pu_12.1kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.5, 30.9);
S2_times["211202_Pu_11.2kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.0, 31.0);
S2_times["211202_Pu_10.4kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.0, 31.3);
S2_times["211202_Pu_9.5kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.0, 31.4);
S2_times["211202_Pu_8.6kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.0, 31.5);
S2_times["211202_Pu_7.8kV_850V_46V_12dB_10mm"] = std::pair<double, double> (25.0, 31.5);

std::map<std::string, std::string> exp_folders;
exp_folders["211202_Pu_16kV_850V_46V_12dB_16mm"] = "Pu_16mm_46V_16kV_850V";
exp_folders["211202_Pu_15kV_850V_46V_12dB_16mm"] = "Pu_16mm_46V_15kV_850V";
exp_folders["211202_Pu_15kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_15kV_850V";
exp_folders["211202_Pu_14kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_14kV_850V";
exp_folders["211202_Pu_13kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_13kV_850V";
exp_folders["211202_Pu_12kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_12kV_850V";
exp_folders["211202_Pu_11kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_11kV_850V";
exp_folders["211202_Pu_10kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_10kV_850V";
exp_folders["211202_Pu_9kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_09kV_850V";
exp_folders["211202_Pu_8kV_850V_46V_12dB_18mm"] = "Pu_18mm_46V_08kV_850V";
exp_folders["211202_Pu_17.3kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_17.3kV_850V";
exp_folders["211202_Pu_15.6kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_15.6kV_850V";
exp_folders["211202_Pu_13.8kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_13.8kV_850V";
exp_folders["211202_Pu_12.1kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_12.1kV_850V";
exp_folders["211202_Pu_11.2kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_11.2kV_850V";
exp_folders["211202_Pu_10.4kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_10.4kV_850V";
exp_folders["211202_Pu_9.5kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_09.5kV_850V";
exp_folders["211202_Pu_8.6kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_08.6kV_850V";
exp_folders["211202_Pu_7.8kV_850V_46V_12dB_10mm"] = "Pu_10mm_46V_07.8kV_850V";

std::map<std::string, double> pre_trigger_max_S; //for calibration only
pre_trigger_max_S["211202_Pu_16kV_850V_46V_12dB_16mm"] = 0.027;
pre_trigger_max_S["211202_Pu_15kV_850V_46V_12dB_16mm"] = 0.027;
pre_trigger_max_S["211202_Pu_15kV_850V_46V_12dB_18mm"] = 0.020;
pre_trigger_max_S["211202_Pu_14kV_850V_46V_12dB_18mm"] = 0.024;
pre_trigger_max_S["211202_Pu_13kV_850V_46V_12dB_18mm"] = 0.023;
pre_trigger_max_S["211202_Pu_12kV_850V_46V_12dB_18mm"] = 0.019;
pre_trigger_max_S["211202_Pu_11kV_850V_46V_12dB_18mm"] = 0.016;
pre_trigger_max_S["211202_Pu_10kV_850V_46V_12dB_18mm"] = 0.016;
pre_trigger_max_S["211202_Pu_9kV_850V_46V_12dB_18mm"] = 0.016;
pre_trigger_max_S["211202_Pu_8kV_850V_46V_12dB_18mm"] = 0.016;
pre_trigger_max_S["211202_Pu_17.3kV_850V_46V_12dB_10mm"] = 0.035;
pre_trigger_max_S["211202_Pu_15.6kV_850V_46V_12dB_10mm"] = 0.029;
pre_trigger_max_S["211202_Pu_13.8kV_850V_46V_12dB_10mm"] = 0.024;
pre_trigger_max_S["211202_Pu_12.1kV_850V_46V_12dB_10mm"] = 0.023;
pre_trigger_max_S["211202_Pu_11.2kV_850V_46V_12dB_10mm"] = 0.022;
pre_trigger_max_S["211202_Pu_10.4kV_850V_46V_12dB_10mm"] = 0.022;
pre_trigger_max_S["211202_Pu_9.5kV_850V_46V_12dB_10mm"] = 0.018;
pre_trigger_max_S["211202_Pu_8.6kV_850V_46V_12dB_10mm"] = 0.018;
pre_trigger_max_S["211202_Pu_7.8kV_850V_46V_12dB_10mm"] = 0.018;

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
saveaspng(data_output_path + folder +"/calibration/SiPMs_pre-trigger_cut");
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
    set_zoom(0, 0.06, 0, 0.01);
    set_bins(500);
    noise_cut(channel, 1, SiPM_state, true);
    saveaspng(data_output_path + folder + "/calibration/"+ch_str+"_A_S_1_zoom");
		set_log_z();
		saveaspng(data_output_path + folder + "/calibration/"+ch_str+"_A_S_2_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveaspng(data_output_path + folder +"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveaspng(data_output_path + folder +"/calibration/"+ch_str+"_Ss_2_single_pe_w_0-"+S2_start+"us");
}
if (channel==1) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.1, 0, 2.5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.04); set_log_z();
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom_log");
		set_bins(500);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.030);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.15, 0, 0.05);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
	 	set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		unset_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.1);
    set_bins(600);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.08);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
		set_zoom(0, 0.12, 0, 0.04);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.03);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.04);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.08);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
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
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.06, 0, 0.003);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
		set_bins(130, 300);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.001);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0004);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.06, 0, 0.003);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(130, 400);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0012);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.03, 0, 0.003);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(60, 200);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.001);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0005);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.06, 0, 0.003);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(130, 500);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0016);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
if (exp == "211202_Pu_16kV_850V_46V_12dB_16mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {40, 0, 113, 35, 167.5, 57, 399, 251};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {5, 0, 44, 6, 119, 60};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(113, 145);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(113, 177);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_15kV_850V_46V_12dB_16mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {40, 0, 108, 37, 151, 46.6, 299, 180};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {5, 0, 34.5, 5.0, 119, 66};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(103, 134);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(103, 167);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_15kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
	x_y_regions = {57, 0, 205, 70, 349, 193};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {5, 0, 33.8, 6.6, 119, 68};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(136, 185);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(136, 226);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_14kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
	x_y_regions = {26, 0, 165, 79, 349, 250};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {7.5, 0, 28.9, 5.2, 119, 81.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(110, 148);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(110, 180);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_13kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
	x_y_regions = {23, 0, 143.5, 77, 349, 270};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {7.5, 0, 26.6, 5.2, 119, 87.1};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(90, 125);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(90, 154);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_12kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {20.6, 0, 121, 70, 299, 259};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {5.1, 0, 20.0, 2.9, 119, 95.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(72, 104);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(72, 131);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_11kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	x_y_regions = {10.4, 0, 97, 58, 249, 228};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {3.0, 0, 16.5, 1.7, 119, 104.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(61, 86);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(61, 110);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_10kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {7.3, 0, 77.4, 43.7, 199, 173};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0.6, 0, 13.2, 1.3, 119, 104.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(48, 74);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(48, 94);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_9kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 2, 66.0, 36.6, 199, 181};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0.6, 0, 13.2, 1.3, 119, 104.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(41, 63);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(41, 81);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_8kV_850V_46V_12dB_18mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 2, 57.4, 31.4, 199, 188};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0.6, 0, 13.2, 1.3, 119, 104.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(34, 54.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(34, 73);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_17.3kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	x_y_regions = {11.3, 0, 120.6, 21.0, 249, 145};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0.6, 0, 8.7, 2.3, 45.1, 3.7, 119, 66.7};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(83, 105);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(83, 128.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_15.6kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	x_y_regions = {11.3, 0, 89.3, 21.8, 249, 133.3};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0.6, 0, 3.8, 2.3, 34.4, 2.3, 119, 67.3};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(66, 86.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(66, 108.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_13.8kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 6.5, 77.0, 22.7, 199, 132};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0.6, 0, 1.9, 1.9, 24.1, 1.9, 119, 74.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(53, 71);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(53, 89.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_12.1kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 4.8, 60.0, 20.1, 199, 146.5};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0, 0, 1.9, 1.9, 18.2, 1.9, 119, 91};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(43, 60.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(43, 78.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_11.2kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 4.8, 52.9, 18.8, 199, 153.0};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0, 0, 1.9, 1.9, 16.3, 1.9, 119, 95};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(38, 53.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(38, 69.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_10.4kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 4.8, 43.1, 15.5, 199, 161.0};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0, 0, 1.9, 1.9, 14.3, 1.9, 119, 100.3};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(33, 48.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(33, 66);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_9.5kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	x_y_regions = {0, 4.0, 42.5, 18.5, 149, 120.6};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0, 0, 1.9, 1.9, 14.3, 1.9, 119, 107.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(31, 46);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(31, 61.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_8.6kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	x_y_regions = {0, 4.0, 37.2, 13.3, 149, 121.5};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0, 0, 1, 1.5, 12.6, 1.5, 119, 109.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(26, 41.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(26, 56.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "211202_Pu_7.8kV_850V_46V_12dB_10mm") {
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
	set_zoom(0, 200);
	set_bins(200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	x_y_regions = {0, 4.0, 34.0, 12.1, 149, 124.7};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_sPMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	x_y_regions = {0, 0, 1, 1.5, 12.6, 1.5, 119, 109.5};
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
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(22, 35.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(22, 51.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
