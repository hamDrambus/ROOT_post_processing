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
	off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	off_ch(0); off_ch(9);
}

void slow_PMTs_only (void) {
	on_ch(1); on_ch(2); on_ch(3); on_ch(4);
	off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	off_ch(0); off_ch(9);
}

void trigger_only (void) {
	off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	off_ch(0); on_ch(9);
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
	//	PMT: 0 - 850V, 12dB, 1 - 800V,  12dB
	//	SiPM: 0 - 55V (300K), 1 - 54V  (256K), 2 - 50.0V (200K)
	if (0 == device_condition) {
		if (channel==1) {
			cut_A_S_lower(0.01, 0.00089, 0.027, 0.0023, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.027, 0.0023, 1.1, 0.22, display, channel, "rem_largeA_smallS");
		}
		if (channel==2) {
			cut_A_S_lower(0.02, 0.00111, 0.058, 0.0028, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.058, 0.0028, 1.6, 0.24, display, channel, "rem_largeA_smallS");
		}
		if (channel==3) {
			cut_A_S_lower(0.01, 0.00117, 0.0215, 0.00185, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.0215, 0.00185, 1.2, 0.23, display, channel, "rem_largeA_smallS");
		}
		if (channel==4) {
			cut_A_S_upper(0.01, 0.0, 0.0236, 0.0, display, channel, "rem_smallA");
			cut_A_S_lower(0.02, 0.00085, 0.057, 0.0024, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.057, 0.0024, 1.6, 0.20, display, channel, "rem_largeA_smallS");
		}
		if (channel==5) {
		}
		if (channel==6) {
		}
		if (channel==7) {
		}
		if (channel==8) {
		}
		if (channel==9) { //May be incorrect
			cut_A_S_lower(0.075, 0.0, 0.984, 3.11, display, channel, "rem_smallA_smallS");
			cut_A_S_upper(0.079, 0.570, 0.554, 2.33, display, channel, "rem_smallA_largeS");
		}
	} //0 == device_condition
	if (1 == device_condition) {
		if (channel==1) {
			cut_A_S_upper(0.01, 0.0, 0.0126, 0.0, display, channel, "rem_smallA");
			cut_A_S_lower(0.01, 0.0011, 0.019, 0.0013, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.019, 0.0013, 1.1, 0.24, display, channel, "rem_largeA_smallS");
		}
		if (channel==2) {
			cut_A_S_lower(0.02, 0.00111, 0.075, 0.0056, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.075, 0.0056, 1.6, 0.26, display, channel, "rem_largeA_smallS");
		}
		if (channel==3) {
			cut_A_S_lower(0.01, 0.00117, 0.0215, 0.00210, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.0215, 0.00210, 1.2, 0.25, display, channel, "rem_largeA_smallS");
		}
		if (channel==4) {
			cut_A_S_upper(0.01, 0.0, 0.0236, 0.0, display, channel, "rem_smallA");
			cut_A_S_lower(0.02, 0.00116, 0.057, 0.0034, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.057, 0.0034, 1.6, 0.23, display, channel, "rem_largeA_smallS");
		}
		if (channel==5) {
		}
		if (channel==6) {
		}
		if (channel==7) {
		}
		if (channel==8) {
		}
		if (channel==9) { //May be incorrect
			cut_A_S_right(0.069, 0.0, 1.2, 4.00, display, channel, "rem_smallA_smallS");
			cut_A_S_upper(0.077, 0.29, 0.189, 0.799, display, channel, "rem_smallA_largeS");
		}
	} //1 == device_condition
	//qewr
	if (0 == device_condition) {
	if (channel==32) {
		x_y_regions = {0.0030, 0.00036, 0.0080, 0.00060, 0.010, 0.00060, 0.0135, 0.00089, 0.0135, 0.00145, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0110, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00115, 0.0110, 0.00215, display, channel, "2pe_merged");
	}
	if (channel==33) {
		x_y_regions = {0.0030, 0.00033, 0.0067, 0.00051, 0.0082, 0.00048, 0.0109, 0.00073, 0.0109, 0.00122, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0096, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00106, 0.0096, 0.00184, display, channel, "2pe_merged");
	}
	if (channel==34) {
		x_y_regions = {0.0030, 0.00041, 0.0080, 0.00073, 0.0106, 0.00067, 0.0147, 0.00101, 0.0147, 0.00160, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0110, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00115, 0.0110, 0.00221, display, channel, "2pe_merged");
	}
	if (channel==35) {
		x_y_regions = {0.0030, 0.00041, 0.0080, 0.00073, 0.0106, 0.00067, 0.0147, 0.00101, 0.0147, 0.00160, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0115, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00115, 0.0115, 0.00232, display, channel, "2pe_merged");
	}
	if (channel==36) {
		x_y_regions = {0.0030, 0.00038, 0.0078, 0.00073, 0.0100, 0.00073, 0.0135, 0.00107, 0.0135, 0.00163, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0143, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00127, 0.0143, 0.00291, display, channel, "2pe_merged");
	}
	if (channel==37) {
		x_y_regions = {0.0030, 0.00037, 0.0073, 0.00060, 0.0110, 0.00071, 0.0135, 0.00086, 0.0135, 0.00162, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0111, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00103, 0.0111, 0.00225, display, channel, "2pe_merged");
	}
	if (channel==38) {
		x_y_regions = {0.0030, 0.00036, 0.0076, 0.00068, 0.0097, 0.00070, 0.0125, 0.00098, 0.0135, 0.00161, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0147, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00123, 0.0147, 0.00299, display, channel, "2pe_merged");
	}
	if (channel==39) {
		x_y_regions = {0.0030, 0.00042, 0.0076, 0.00068, 0.0097, 0.00070, 0.0125, 0.00098, 0.0135, 0.00161, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0127, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00111, 0.0127, 0.00261, display, channel, "2pe_merged");
	}
	if (channel==40) {
		x_y_regions = {0.0030, 0.00042, 0.0076, 0.00068, 0.0097, 0.00070, 0.0125, 0.00098, 0.0135, 0.00161, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0127, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00111, 0.0127, 0.00261, display, channel, "2pe_merged");
	}
	if (channel==41) {
		x_y_regions = {0.0030, 0.00042, 0.0076, 0.00068, 0.0097, 0.00070, 0.0125, 0.00098, 0.0135, 0.00161, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0127, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00111, 0.0127, 0.00261, display, channel, "2pe_merged");
	}
	if (channel==42) {
		x_y_regions = {0.0030, 0.00042, 0.0076, 0.00068, 0.0097, 0.00070, 0.0125, 0.00098, 0.0135, 0.00161, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0127, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00111, 0.0127, 0.00261, display, channel, "2pe_merged");
	}
	if (channel==43) {
		x_y_regions = {0.0030, 0.00042, 0.0076, 0.00068, 0.0097, 0.00070, 0.0125, 0.00098, 0.0135, 0.00161, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0120, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00107, 0.0120, 0.00234, display, channel, "2pe_merged");
	}
	if (channel==48) {
		x_y_regions = {0.0030, 0.00042, 0.0076, 0.00068, 0.0097, 0.00070, 0.0125, 0.00098, 0.0135, 0.00161, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0120, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00107, 0.0120, 0.00234, display, channel, "2pe_merged");
	}
	if (channel==49) {
		x_y_regions = {0.0030, 0.00041, 0.0055, 0.00053, 0.0084, 0.00044, 0.0116, 0.00072, 0.0116, 0.00122, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0084, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00107, 0.0084, 0.00164, display, channel, "2pe_merged");
	}
	if (channel==50) {
		x_y_regions = {0.0030, 0.00041, 0.0055, 0.00053, 0.0084, 0.00044, 0.0133, 0.00078, 0.0133, 0.00141, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0092, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00116, 0.0092, 0.00182, display, channel, "2pe_merged");
	}
	if (channel==51) {
		x_y_regions = {0.0030, 0.00041, 0.0068, 0.00062, 0.0104, 0.00064, 0.0133, 0.00087, 0.0133, 0.00141, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0107, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00115, 0.0107, 0.00212, display, channel, "2pe_merged");
	}
	if (channel==52) {
		x_y_regions = {0.0030, 0.00041, 0.0082, 0.00071, 0.0116, 0.00067, 0.0150, 0.00101, 0.0150, 0.00162, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00121, 0.0132, 0.00268, display, channel, "2pe_merged");
	}
	if (channel==53) {
		x_y_regions = {0.0030, 0.00041, 0.0086, 0.00077, 0.0111, 0.00076, 0.0149, 0.00109, 0.0149, 0.00162, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00124, 0.0145, 0.00292, display, channel, "2pe_merged");
	}
	if (channel==54) {
		x_y_regions = {0.0030, 0.00041, 0.0074, 0.00065, 0.0094, 0.00065, 0.0123, 0.00094, 0.0123, 0.00142, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0116, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00111, 0.0116, 0.00231, display, channel, "2pe_merged");
	}
	if (channel==55) {
		x_y_regions = {0.0030, 0.00041, 0.0075, 0.00067, 0.0103, 0.00066, 0.0127, 0.00090, 0.0127, 0.00143, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0116, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00111, 0.0116, 0.00231, display, channel, "2pe_merged");
	}
	if (channel==56) {
		x_y_regions = {0.0030, 0.00041, 0.0086, 0.00073, 0.0108, 0.00101, 0.0114, 0.00135, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00106, 0.0132, 0.00261, display, channel, "2pe_merged");
	}
	if (channel==57) {
		x_y_regions = {0.0030, 0.00041, 0.0086, 0.00073, 0.0108, 0.00101, 0.0114, 0.00135, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00106, 0.0132, 0.00261, display, channel, "2pe_merged");
	}
	if (channel==58) {
		x_y_regions = {0.0030, 0.00041, 0.0086, 0.00073, 0.0104, 0.00073, 0.0129, 0.00093, 0.0129, 0.00152, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00106, 0.0132, 0.00261, display, channel, "2pe_merged");
	}
	if (channel==59) {
		x_y_regions = {0.0030, 0.00041, 0.0086, 0.00073, 0.0104, 0.00073, 0.0141, 0.00098, 0.0141, 0.00157, 1, 0.18};
		cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0030, 0.00106, 0.0132, 0.00261, display, channel, "2pe_merged");
	}
	} // 0== device_condition
	if (1 == device_condition) {
	if (channel==32) {
		cut_A_S_lower(0.0069, 0.000961, 0.02, 0.00189, display, channel, "small_A-S_noise");
		x_y_regions = {0.0069, 0.0069, -0.000118, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0069, 0.00127, 0.0090, 0.00239, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0171, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0069, 0.00281, 0.0171, 0.00395, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.0061, 0.000830, 0.02, 0.00198, display, channel, "small_A-S_noise");
		x_y_regions = {0.0061, 0.0061, 0.000071, 0.04, 0.0072, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0061, 0.00115, 0.0079, 0.00198, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0061, 0.00255, 0.0155, 0.00364, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0074, 0.000962, 0.02, 0.00200, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, -0.000089, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0074, 0.00137, 0.0095, 0.00242, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0187, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00330, 0.0187, 0.00426, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0074, 0.000962, 0.02, 0.00200, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, -0.000089, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0074, 0.00137, 0.0095, 0.00242, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0187, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00330, 0.0187, 0.00426, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0074, 0.000962, 0.02, 0.00200, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, -0.000089, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0074, 0.00137, 0.0097, 0.00238, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0198, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00341, 0.0198, 0.00470, display, channel, "2pe_merged");
	}
	if (channel==37) {
		cut_A_S_lower(0.0063, 0.000801, 0.02, 0.00202, display, channel, "small_A-S_noise");
		x_y_regions = {0.0063, 0.0074, -0.000089, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0063, 0.00121, 0.0086, 0.00233, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0171, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0063, 0.00268, 0.0171, 0.00405, display, channel, "2pe_merged");
	}
	if (channel==38) {
		cut_A_S_lower(0.0068, 0.000801, 0.02, 0.00202, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00164, 0.04, 0.0069, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00112, 0.0096, 0.00226, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0200, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00329, 0.0200, 0.00465, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0068, 0.000786, 0.02, 0.00216, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00177, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00128, 0.0089, 0.00233, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0188, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00293, 0.0188, 0.004366, display, channel, "2pe_merged");
	}
	if (channel==40) {
		cut_A_S_lower(0.0068, 0.000801, 0.02, 0.00230, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00182, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00124, 0.0090, 0.00223, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0188, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00313, 0.0188, 0.00436, display, channel, "2pe_merged");
	}
	if (channel==41) {
		cut_A_S_lower(0.0068, 0.000801, 0.02, 0.00230, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00182, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00132, 0.0090, 0.00232, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0184, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00302, 0.0184, 0.00429, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0068, 0.00100, 0.02, 0.00201, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00180, 0.04, 0.0072, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00116, 0.0090, 0.00217, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0184, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00316, 0.0184, 0.00439, display, channel, "2pe_merged");
	}
	if (channel==43) {
		cut_A_S_lower(0.0068, 0.00090, 0.02, 0.00204, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00180, 0.04, 0.0072, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00121, 0.0091, 0.00232, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0188, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00290, 0.0188, 0.00453, display, channel, "2pe_merged");
	}
	if (channel==48) {
		cut_A_S_lower(0.0068, 0.00090, 0.02, 0.00204, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00180, 0.04, 0.0072, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00121, 0.0091, 0.00232, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0188, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00290, 0.0188, 0.00453, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0062, 0.00086, 0.02, 0.00182, display, channel, "small_A-S_noise");
		x_y_regions = {0.0062, 0.015, 0.00179, 0.04, 0.0074, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0062, 0.00118, 0.0079, 0.00207, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0152, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0062, 0.00249, 0.0152, 0.00354, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0062, 0.00086, 0.02, 0.00187, display, channel, "small_A-S_noise");
		x_y_regions = {0.0062, 0.015, 0.00163, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0062, 0.00120, 0.0079, 0.00208, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0158, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0062, 0.00233, 0.0158, 0.00383, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0066, 0.00083, 0.02, 0.00203, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.015, 0.00163, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0066, 0.00122, 0.0086, 0.00213, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0176, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00267, 0.0176, 0.00417, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0073, 0.00097, 0.02, 0.00208, display, channel, "small_A-S_noise");
		x_y_regions = {0.0073, 0.015, 0.00142, 0.04, 0.0071, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0073, 0.00133, 0.0098, 0.00258, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0195, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0073, 0.00303, 0.0195, 0.00473, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0073, 0.00097, 0.02, 0.00208, display, channel, "small_A-S_noise");
		x_y_regions = {0.0073, 0.015, 0.00148, 0.04, 0.0068, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0073, 0.00133, 0.0098, 0.00241, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0205, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0073, 0.00341, 0.0205, 0.00474, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0068, 0.00087, 0.02, 0.00206, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00172, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00131, 0.0090, 0.00244, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0178, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00289, 0.0178, 0.00420, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0068, 0.00087, 0.02, 0.00206, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00172, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00131, 0.0090, 0.00244, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0178, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00289, 0.0178, 0.00420, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.0068, 0.00087, 0.02, 0.00206, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00177, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00122, 0.0091, 0.00230, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0190, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00310, 0.0190, 0.00452, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0068, 0.00094, 0.02, 0.00219, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00184, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00101, 0.0096, 0.00239, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0198, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00322, 0.0198, 0.00467, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0068, 0.00084, 0.02, 0.00207, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.015, 0.00176, 0.04, 0.0070, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0068, 0.00112, 0.0094, 0.00242, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0193, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00299, 0.0193, 0.00452, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0074, 0.00093, 0.02, 0.00202, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.015, 0.00160, 0.04, 0.0068, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0.0074, 0.00135, 0.0095, 0.00246, display, channel, "smallA-largeS");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0193, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00315, 0.0193, 0.00450, display, channel, "2pe_merged");
	}
	} // 1== device_condition
	if (2 == device_condition) { //TODO
	if (channel==32) {
		cut_A_S_lower(0.0080, 0.000607, 0.02, 0.00116, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.0080, 0.000177, 0.04, 0.00350, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0193, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0080, 0.00178, 0.0193, 0.00222, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.0068, 0.00054, 0.02, 0.00083, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.0068, 0.000088, 0.04, 0.00378, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0157, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00161, 0.0157, 0.00189, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0089, 0.00070, 0.02, 0.00131, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00035, 0.04, 0.00366, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0211, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00195, 0.0211, 0.00235, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0089, 0.00070, 0.02, 0.00131, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00035, 0.04, 0.00359, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0207, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00195, 0.0207, 0.00240, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0089, 0.00060, 0.02, 0.00136, display, channel, "small_A-S_noise");
		x_y_regions = {0.0089, 0.0089, 0.00035, 0.04, 0.00359, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0219, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0089, 0.00213, 0.0219, 0.00279, display, channel, "2pe_merged");
	}
	if (channel==37) {
		cut_A_S_lower(0.0074, 0.00052, 0.02, 0.00128, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, 0.00024, 0.04, 0.00358, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0181, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00177, 0.0181, 0.00222, display, channel, "2pe_merged");
	}
	if (channel==38) {
		cut_A_S_lower(0.0099, 0.00072, 0.02, 0.00134, display, channel, "small_A-S_noise");
		x_y_regions = {0.0099, 0.0099, 0.00050, 0.04, 0.00355, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0226, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0099, 0.00228, 0.0226, 0.00281, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0084, 0.00057, 0.02, 0.00134, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00042, 0.04, 0.00377, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0198, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.00197, 0.0198, 0.00258, display, channel, "2pe_merged");
	}
	if (channel==40) {
		cut_A_S_lower(0.0084, 0.00057, 0.02, 0.00139, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00039, 0.04, 0.00369, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0203, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.00197, 0.0203, 0.00265, display, channel, "2pe_merged");
	}
	if (channel==41) {
		cut_A_S_lower(0.0084, 0.00057, 0.02, 0.00139, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00039, 0.04, 0.00369, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0203, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.00197, 0.0203, 0.00265, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0084, 0.00061, 0.02, 0.00139, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00039, 0.04, 0.00369, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0203, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.00175, 0.0203, 0.00265, display, channel, "2pe_merged");
	}
	if (channel==43) {
		cut_A_S_lower(0.0084, 0.00057, 0.02, 0.00134, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00039, 0.04, 0.00369, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0203, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.00173, 0.0203, 0.00267, display, channel, "2pe_merged");
	}
	if (channel==48) {
		cut_A_S_lower(0.0088, 0.00067, 0.02, 0.00122, display, channel, "small_A-S_noise");
		x_y_regions = {0.0088, 0.0088, 0.00036, 0.04, 0.00360, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0203, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0088, 0.00175, 0.0203, 0.00250, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0065, 0.00052, 0.02, 0.00074, display, channel, "small_A-S_noise");
		x_y_regions = {0.0065, 0.0065, 0.000072, 0.04, 0.00377, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0150, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0065, 0.00150, 0.0150, 0.00180, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0067, 0.00053, 0.02, 0.00119, display, channel, "small_A-S_noise");
		x_y_regions = {0.0067, 0.0067, 0.000104, 0.04, 0.00380, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0157, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0067, 0.00156, 0.0157, 0.00194, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0080, 0.00060, 0.02, 0.00121, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.0080, 0.000194, 0.04, 0.00366, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0190, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0080, 0.00177, 0.0190, 0.00224, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0091, 0.00067, 0.02, 0.00128, display, channel, "small_A-S_noise");
		x_y_regions = {0.0091, 0.0091, 0.000267, 0.04, 0.00355, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0216, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0091, 0.00193, 0.0216, 0.00276, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0094, 0.00066, 0.02, 0.00136, display, channel, "small_A-S_noise");
		x_y_regions = {0.0094, 0.0094, 0.000307, 0.04, 0.00356, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0233, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0094, 0.00210, 0.0233, 0.00292, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0081, 0.00062, 0.02, 0.00134, display, channel, "small_A-S_noise");
		x_y_regions = {0.0081, 0.0081, 0.000364, 0.04, 0.00376, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0189, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0081, 0.00173, 0.0189, 0.00242, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0079, 0.00054, 0.02, 0.00133, display, channel, "small_A-S_noise");
		x_y_regions = {0.0079, 0.0079, 0.000275, 0.04, 0.00377, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0190, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0079, 0.00181, 0.0190, 0.00258, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.0088, 0.00057, 0.02, 0.00143, display, channel, "small_A-S_noise");
		x_y_regions = {0.0088, 0.0088, 0.000429, 0.04, 0.00377, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0205, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0088, 0.00186, 0.0205, 0.00284, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0093, 0.00069, 0.02, 0.00143, display, channel, "small_A-S_noise");
		x_y_regions = {0.0093, 0.0093, 0.000477, 0.04, 0.00378, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0207, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0093, 0.00204, 0.0207, 0.00273, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0086, 0.00063, 0.02, 0.00131, display, channel, "small_A-S_noise");
		x_y_regions = {0.0086, 0.0086, 0.000331, 0.04, 0.00368, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0202, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0086, 0.00190, 0.0202, 0.00259, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0086, 0.00063, 0.02, 0.00131, display, channel, "small_A-S_noise");
		x_y_regions = {0.0086, 0.0086, 0.000331, 0.04, 0.00368, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0202, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0086, 0.00181, 0.0202, 0.00270, display, channel, "2pe_merged");
	}
	} // 2== device_condition
}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2021.04.03
//Same as 3analysis_history.cpp, but trying to select tracks by fitting fast PMT peaks instead of SiPM
data_output_path = "210316/results_v4/";
calibration_file = "210316/results_v1/210316_calibration.dat";
post_processor->calibr_info.Load(calibration_file);

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["210316_Pu_7.6kV_850V_55V_12dB_300K"] = std::pair<double, double> (26.6, 56.0);
S2_times["210316_Pu_7.6kV_800V_54V_12dB_256K"] = std::pair<double, double> (26.3, 57.0);
S2_times["210316_Pu_7.6kV_800V_50V_12dB_200K"] = std::pair<double, double> (26.4, 58.0);//TODO

std::map<std::string, std::string> exp_folders;
exp_folders["210316_Pu_7.6kV_850V_55V_12dB_300K"] = "Pu_55V_7.6kV_850V_300K";
exp_folders["210316_Pu_7.6kV_800V_54V_12dB_256K"] = "Pu_54V_7.6kV_800V_256K";
exp_folders["210316_Pu_7.6kV_800V_50V_12dB_200K"] = "Pu_50V_7.6kV_800V_200K";

std::map<std::string, double> pre_trigger_max_S; //for calibration
pre_trigger_max_S["210316_Pu_7.6kV_850V_55V_12dB_300K"] = 1.61;
pre_trigger_max_S["210316_Pu_7.6kV_800V_54V_12dB_256K"] = 0.2;
pre_trigger_max_S["210316_Pu_7.6kV_800V_50V_12dB_200K"] = 0.049;//TODO

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
if (calibrate) {
	std::cout<<"Use 1analysis_history.cpp for calibration! Doing nothing."<<std::endl;
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
int SiPM_state = 0; //55V, 300K
if (exp == "210316_Pu_7.6kV_800V_54V_12dB_256K") {
	PMT_state = 1;
	SiPM_state = 1; //54V, 256K
}
if (exp == "210316_Pu_7.6kV_800V_50V_12dB_200K") {
	PMT_state = 1;
	SiPM_state = 2; //50V, 200K
}
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
if (exp == "210316_Pu_7.6kV_800V_54V_12dB_256K") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 1500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 1500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(0, 160);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 1500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 2000);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(0, 160);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1800, 0, 1800);
	set_bins(1800);
	x_y_regions = {266, 0, 1799, 1382};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 1000);
	set_bins(1000);
	x_y_regions = {42.0, 0, 999, 922};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	set_zoom(0, 6); set_bins(500);
	ShapeFitData::SetPeakType(ShapeFitData::ptNpe);
	ShapeFitData::SetNPars(7);
	ShapeFitData::SetBound(0, 0.1, 5);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 42, 55);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.55);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "BkgFr");
	ShapeFitData::SetBound(3, 0, 0);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 160, 160);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetBound(5, 1.5, 5.0);
	ShapeFitData::SetPrecision(5, 0.02);
	ShapeFitData::SetParameterName(5, "W2/W1");
	ShapeFitData::SetBound(6, 0, 0.45);
	ShapeFitData::SetPrecision(6, 0.01);
	ShapeFitData::SetParameterName(6, "Gauss2Fr");
	ShapeFitData::SetFitFunction(gauss_gauss_bkg_pdf);
	ShapeFitData::SetPlotParameter(0); //Select for which parameter histogram will be plotted.
	update();
	saveaspng(FOLDER + Num+"_SiPMs_fitted_width_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_shape_fit);
	fast_PMTs_only(); //slow PMTs' peaks are merging which worsens fit
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_zoom(0, 6); set_bins(500);
	ShapeFitData::SetPeakType(ShapeFitData::ptNpe);
	ShapeFitData::SetNPars(7);
	ShapeFitData::SetBound(0, 0.1, 5);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 42, 55);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.55);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "BkgFr");
	ShapeFitData::SetBound(3, 0, 0);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 160, 160);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetBound(5, 1.5, 5.0);
	ShapeFitData::SetPrecision(5, 0.02);
	ShapeFitData::SetParameterName(5, "W2/W1");
	ShapeFitData::SetBound(6, 0, 0.45);
	ShapeFitData::SetPrecision(6, 0.01);
	ShapeFitData::SetParameterName(6, "Gauss2Fr");
	ShapeFitData::SetFitFunction(gauss_gauss_bkg_pdf);
	ShapeFitData::SetPlotParameter(0); //Select for which parameter histogram will be plotted.
	update();
	saveaspng(FOLDER + Num+"_fPMTs_fitted_width_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_shape_fit, AStates::PMT_shape_fit, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 6, 0, 6);
	set_bins(500);
	set_titles("SiPMs fitted width [#mus]", "fPMTs fitted width [#mus]");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_SiPMs_width_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_shape_fit, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 6);
	set_bins(1000, 500);
	set_titles("N_{pe} sPMTs t#in[0, 160] #mus", "fPMTs fitted width [#mus]");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_sPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Alpha_peak_v1/"; //v1 - orthogonal tracks, v2 - longitudial
set_corr(AStates::MPPC_Npe_sum, AStates::PMT_shape_fit, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 1800, 0, 6);
	set_bins(1800, 500);
	set_titles("N_{pe} SiPMs t#in[0, 160] #mus", "fPMTs fitted width [#mus]");
	x_y_regions = {1008, 1.28, 783, 1.28, 643, 2.12, 1171, 2.12};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak_v2/"; //v1 - orthogonal tracks, v2 - longitudial
ty(AStates::Correlation);
	x_y_regions = {1189, 3.16, 916, 3.16, 1051, 4.32, 1200, 4.32};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak_v3/"; //v3 - middle case
ty(AStates::Correlation);
	x_y_regions = {1140, 2.20, 632, 2.20, 580, 3.00, 1211, 3.00};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "210316_Pu_7.6kV_850V_55V_12dB_300K") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 1500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 1500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(0, 160);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 1500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(3000, 7000);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(0, 160);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(5500, 7000, 1000, 2000);
	set_bins(1500, 2000);
	x_y_regions = {5740, 1000, 6999, 1880};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 1000);
	set_bins(1000);
	x_y_regions = {22.8, 0, 999, 951};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	//Does not work! ty(AStates::MPPC_shape_fit);
ty(AStates::PMT_shape_fit);
	fast_PMTs_only(); //slow PMTs' peaks are merging which worsens fit
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_zoom(0, 6); set_bins(500);
	ShapeFitData::SetPeakType(ShapeFitData::ptNpe);
	ShapeFitData::SetNPars(7);
	ShapeFitData::SetBound(0, 0.1, 5);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 42, 55);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.55);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "BkgFr");
	ShapeFitData::SetBound(3, 0, 0);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 160, 160);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetBound(5, 1.5, 5.0);
	ShapeFitData::SetPrecision(5, 0.02);
	ShapeFitData::SetParameterName(5, "W2/W1");
	ShapeFitData::SetBound(6, 0, 0.45);
	ShapeFitData::SetPrecision(6, 0.01);
	ShapeFitData::SetParameterName(6, "Gauss2Fr");
	ShapeFitData::SetFitFunction(gauss_gauss_bkg_pdf);
	ShapeFitData::SetPlotParameter(0); //Select for which parameter histogram will be plotted.
	update();
	saveaspng(FOLDER + Num+"_fPMTs_fitted_width_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Alpha_peak_v1/"; //v1 - orthogonal tracks, v2 - longitudial
set_corr(AStates::PMT_Npe_sum, AStates::PMT_shape_fit, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 6);
	set_bins(1000, 500);
	set_titles("N_{pe} sPMTs t#in[0, 160] #mus", "fPMTs fitted width [#mus]");
	x_y_regions = {700, 1.66, 486, 1.66, 401, 2.35, 703, 2.35};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak_v2/"; //v1 - orthogonal tracks, v2 - longitudial
ty(AStates::Correlation);
	x_y_regions = {490, 3.07, 343, 3.07, 368, 4.12, 432, 4.12};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak_v3/"; //v3 - middle case
ty(AStates::Correlation);
	x_y_regions = {572, 2.47, 402, 2.47, 355, 3.00, 512, 3.00};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_fPMTs_width_vs_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
