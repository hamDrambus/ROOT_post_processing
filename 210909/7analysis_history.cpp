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
	//cut_t(f_time, to_time, false, 5);
	cut_t(f_time, to_time, false, 6);
	cut_t(f_time, to_time, false, 7);
	cut_t(f_time, to_time, false, 8);
}

void time_zoom_sPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 0);
	//cut_t(f_time, to_time, false, 1);
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
	/*ch(5);
	noise_cut(5, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "5_form_by_Npeaks");*/
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
	/*ch(1);
	noise_cut(1, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "1_form_by_Npe");*/
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
  /*ch(5);
	noise_cut(5, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "5_form_by_Npe");*/
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
		/*ch(1);
		noise_cut(1, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "1_form_by_S");*/
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
		/*ch(5);
		noise_cut(5, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "5_form_by_S");*/
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
		/*if (channel==1) {
			cut_A_S_upper(0.005, 0.0, 0.0132, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.010, 0.0007, 0.030, 0.0021, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.030, 0.0021, 1.1, 0.20, display, channel, "rem_largeA_smallS");
		}*/
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
		/*if (channel==5) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
				cut_A_S_upper(0.0, 0.015, 1.6, 0.015, display, channel, "rem_S>0.015");
			}
		}*/
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
		cut_A_S_lower(0.0089, 0.00060, 0.03, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00016, 0.06, 0.0050, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0244, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00136, 0.0244, 0.00324, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.008, 0.00053, 0.023, 0.00053, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.008, 0.00006, 0.06, 0.00565, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0201, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.008, 0.0012, 0.0201, 0.00262, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0101, 0.00061, 0.025, 0.00147, display, channel, "small_A-S_noise");
		x_y_regions = {0.0101, 0.0101, 0.00039, 0.06, 0.0053, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0269, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0101, 0.00153, 0.0269, 0.00348, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0095, 0.00044, 0.025, 0.00044, display, channel, "small_A-S_noise");
		x_y_regions = {0.0095, 0.0095, 0.00019, 0.06, 0.0052, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0267, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0095, 0.00159, 0.0267, 0.00359, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0100, 0.00035, 0.024, 0.00106, display, channel, "small_A-S_noise");
		x_y_regions = {0.0100, 0.0100, 0.00006, 0.06, 0.00522, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0253, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0100, 0.00162, 0.0253, 0.00393, display, channel, "2pe_merged");
	}
	if (channel==37) {
		cut_A_S_lower(0.0084, 0.00052, 0.030, 0.00139, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00001, 0.06, 0.00540, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0216, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.00165, 0.0216, 0.00293, display, channel, "2pe_merged");
	}
	if (channel==38) {
		cut_A_S_lower(0.0112, 0.00030, 0.024, 0.00030, display, channel, "small_A-S_noise");
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
		x_y_regions = {0.0101, 0.0101, 0.00029, 0.06, 0.00363, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0241, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0101, 0.00173, 0.0241, 0.00353, display, channel, "2pe_merged");
	}
	if (channel==40) {
		//cut_A_S_lower(0.0104, 0.00122, 0.029, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0112, 0.00037, 0.06, 0.0040, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0263, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0112, 0.00170, 0.0263, 0.00366, display, channel, "2pe_merged");
	}
	if (channel==41) {
		//cut_A_S_lower(0.0107, 0.00108, 0.038, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0107, 0.0107, 0.00037, 0.06, 0.0035, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0107, 0.00173, 0.0255, 0.0038, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0096, 0.00061, 0.014, 0.00076, display, channel, "small_A-S_noise");
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
	}
	if (channel==48) {
		//cut_A_S_lower(0.0101, 0.00113, 0.027, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0101, 0.0101, 0.00039, 0.06, 0.0050, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0260, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0101, 0.00131, 0.0260, 0.0035, display, channel, "2pe_merged");
	}
	if (channel==49) {
		//cut_A_S_lower(0.0080, 0.00095, 0.026, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.0080, 0.00052, 0.06, 0.0033, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0195, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0080, 0.00124, 0.0195, 0.0027, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0080, 0.00040, 0.02, 0.00058, display, channel, "small_A-S_noise");
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
		x_y_regions = {0.0100, 0.0100, 0.00031, 0.06, 0.0038, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0248, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0100, 0.00163, 0.0248, 0.00337, display, channel, "2pe_merged");
	}
	if (channel==52) {
		//cut_A_S_lower(0.0107, 0.00120, 0.040, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0099, 0.0099, 0.00032, 0.06, 0.00418, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0254, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0099, 0.00168, 0.0254, 0.00374, display, channel, "2pe_merged");
	}
	if (channel==53) {
		//cut_A_S_lower(0.0114, 0.00132, 0.022, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0114, 0.0114, 0.00040, 0.06, 0.0035, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0114, 0.00184, 0.028, 0.00421, display, channel, "2pe_merged");
	}
	if (channel==54) {
		//cut_A_S_lower(0.0086, 0.00103, 0.02, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0086, 0.0086, 0.00025, 0.06, 0.0048, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0221, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0086, 0.00153, 0.0221, 0.0030, display, channel, "2pe_merged");
	}
	if (channel==55) {
		//cut_A_S_lower(0.0094, 0.00140, 0.0172, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00027, 0.06, 0.0050, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0227, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00156, 0.0227, 0.00314, display, channel, "2pe_merged");
	}
	if (channel==56) {
		//cut_A_S_lower(0.0116, 0.00108, 0.020, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.010, 0.010, 0.00037, 0.06, 0.00405, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0249, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.010, 0.00159, 0.0249, 0.00356, display, channel, "2pe_merged");
	}
	if (channel==57) {
		//cut_A_S_lower(0.0090, 0.00124, 0.029, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0090, 0.0090, 0.00034, 0.06, 0.00508, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0090, 0.00137, 0.0255, 0.0035, display, channel, "2pe_merged");
	}
	if (channel==58) {
		//cut_A_S_lower(0.0103, 0.00119, 0.04, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0088, 0.0088, 0.00031, 0.06, 0.00480, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0246, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0088, 0.00153, 0.0246, 0.00367, display, channel, "2pe_merged");
	}
	if (channel==59) {
		//cut_A_S_lower(0.0088, 0.00119, 0.022, 0.0, display, channel, "small_A-S_noise");
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
//Created on 2021.10.15
//Checking new idea:
//trapped electrons should have the same position as not trapped ones.
//Hence comparing SiPM-matrix coordinate using fast only conponent (60% of PE) vs slow+long only (40% of PE)
//will test that slow components are not causes by something like VUV-related electron emission and are
//caused by the same electrons as those produced in primary ionization.
data_output_path = "210909/results_v7/";
calibration_file = "210909/results_v5/210909_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["210909_Pu_17.3kV_850V_46V_12dB"] = std::pair<double, double> (23.5, 30.3);

std::map<std::string, std::string> exp_folders;
exp_folders["210909_Pu_13.8kV_850V_46V_12dB_2588V"] = "Pu_46V_13.8kV_850V_2588V";
exp_folders["210909_Pu_13.8kV_850V_46V_12dB_2465V"] = "Pu_46V_13.8kV_850V_2465V";
exp_folders["210909_Pu_13.8kV_850V_46V_12dB_2300V"] = "Pu_46V_13.8kV_850V_2300V";
exp_folders["210909_Pu_17.3kV_850V_46V_12dB"] = "Pu_46V_17.3kV_850V";
exp_folders["210909_Pu_15.6kV_850V_46V_12dB"] = "Pu_46V_15.6kV_850V";
exp_folders["210909_Pu_13.8kV_850V_46V_12dB"] = "Pu_46V_13.8kV_850V";
exp_folders["210909_Pu_12.1kV_850V_46V_12dB"] = "Pu_46V_12.1kV_850V";
exp_folders["210909_Pu_11.2kV_850V_46V_12dB"] = "Pu_46V_11.2kV_850V";
exp_folders["210909_Pu_10.4kV_850V_46V_12dB"] = "Pu_46V_10.4kV_850V";
exp_folders["210909_Pu_9.5kV_850V_46V_12dB"] = "Pu_46V_09.5kV_850V";
exp_folders["210909_Pu_8.6kV_850V_46V_12dB"] = "Pu_46V_08.6kV_850V";
exp_folders["210909_Pu_7.8kV_850V_46V_12dB"] = "Pu_46V_07.8kV_850V";
exp_folders["210909_Pu_6.9kV_850V_46V_12dB"] = "Pu_46V_06.9kV_850V";

std::map<std::string, double> pre_trigger_max_S; //for calibration only

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
if (calibrate) {
	std::cout<<"Use 5analysis_history.cpp for calibration! Doing nothing." <<std::endl;
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
if (exp == "210909_Pu_17.3kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_zoom(0, 250);
	set_bins(250);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {31, 0, 100.5, 17.8, 122.5, 46.3, 199, 95.0};
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

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, 160);
	draw_limits(62, 97.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts));
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_finish, 160);
	unset_draw_limits();
	update();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_finish+"-160us");
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	update();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	set_corr(AStates::MPPC_coord_x, AStates::MPPC_coord_x, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_finish, 160);
	ty(AStates::Correlation);
	set_zoom(-10, 10, -10, 10);
	set_bins(300);
	set_titles("X coord. by t#in["+S2_start+", "+S2_finish+"] #mus", "X coord. by t#in["+S2_finish+", 160] #mus");
	update();
	saveaspng(FOLDER + Num + "_SiPMs_X_correlation_"+cuts_str(cuts)+"_"+S2_finish+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	set_corr(AStates::MPPC_coord_y, AStates::MPPC_coord_y, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_finish, 160);
	ty(AStates::Correlation);
	set_zoom(-10, 10, -10, 10);
	set_bins(300);
	set_titles("Y coord. by t#in["+S2_start+", "+S2_finish+"] #mus", "Y coord. by t#in["+S2_finish+", 160] #mus");
	update();
	saveaspng(FOLDER + Num + "_SiPMs_Y_correlation_"+cuts_str(cuts)+"_"+S2_finish+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);
}
//END OF FORMS
}
