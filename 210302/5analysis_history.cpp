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
	//	PMT: 0 - 800V, 12dB, 1 - 700V, 12 dB
	//	SiPM: 0 - 46.0V (132K), 1 - 46.0V (131K), 2 - 46V (120K)
	if (0 == device_condition) {
		if (channel==1) {
			cut_A_S_lower(0.01, 0.0013, 0.019, 0.0013, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.019, 0.0013, 1.1, 0.24, display, channel, "rem_largeA_smallS");
		}
		if (channel==2) {
			cut_A_S_lower(0.02, 0.00136, 0.075, 0.0054, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.075, 0.0054, 1.6, 0.26, display, channel, "rem_largeA_smallS");
		}
		if (channel==3) {
			cut_A_S_lower(0.013, 0.00076, 0.038, 0.0023, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.038, 0.0023, 1.2, 0.22, display, channel, "rem_largeA_smallS");
		}
		if (channel==4) {
			cut_A_S_lower(0.02, 0.00135, 0.057, 0.0029, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.057, 0.0029, 1.6, 0.23, display, channel, "rem_largeA_smallS");
		}
		if (channel==5) {
		}
		if (channel==6) {
		}
		if (channel==7) {
		}
		if (channel==8) {
		}
		if (channel==9) {
			cut_A_S_lower(0.077, 0.0, 1.28, 3.05, display, channel, "rem_smallA_smallS");
			cut_A_S_upper(0.077, 0.22, 0.17, 0.80, display, channel, "rem_smallA_largeS");
		}
	}
	if (channel==9 && 1 == device_condition) {
		cut_A_S_lower(0.02, 0.045, 0.026, 0.055, display, channel, "rem_smallA_smallS");
	}

	//qewr
	if (2 == device_condition) {
	if (channel==32) {
		cut_A_S_lower(0.0068, 0.00061, 0.02, 0.0005, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.0068, 0.00007, 0.04, 0.00366, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0164, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00113, 0.0164, 0.00222, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.0055, 0.00052, 0.02, 0.00037, display, channel, "small_A-S_noise");
		x_y_regions = {0.0055, 0.0055, 0.000064, 0.04, 0.0039, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0140, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0055, 0.00097, 0.0140, 0.00189, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0081, 0.00076, 0.02, 0.00050, display, channel, "small_A-S_noise");
		x_y_regions = {0.0081, 0.0081, 0.00022, 0.04, 0.0036, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0192, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0081, 0.00130, 0.0192, 0.00247, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0077, 0.00074, 0.02, 0.00021, display, channel, "small_A-S_noise");
		x_y_regions = {0.0077, 0.0077, 0.00026, 0.04, 0.00366, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0194, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0077, 0.00124, 0.0194, 0.00252, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0081, 0.00077, 0.015, 0.00035, display, channel, "small_A-S_noise");
		x_y_regions = {0.0081, 0.0081, 0.00026, 0.04, 0.00376, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0195, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0081, 0.00133, 0.0195, 0.00262, display, channel, "2pe_merged");
	}
	if (channel==37) {
		cut_A_S_lower(0.0060, 0.00068, 0.01, 0.00022, display, channel, "small_A-S_noise");
		x_y_regions = {0.0060, 0.0060, 0.00017, 0.04, 0.0038, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0154, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0060, 0.00117, 0.0154, 0.00222, display, channel, "2pe_merged");
	}
	if (channel==38) {
		cut_A_S_lower(0.0084, 0.00072, 0.015, 0.00014, display, channel, "small_A-S_noise");
		x_y_regions = {0.0084, 0.0084, 0.00041, 0.04, 0.00375, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.021, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0084, 0.0018, 0.021, 0.00243, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0073, 0.00068, 0.010, 0.00041, display, channel, "small_A-S_noise");
		x_y_regions = {0.0073, 0.0073, 0.000355, 0.04, 0.00382, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.018, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0073, 0.00155, 0.018, 0.00210, display, channel, "2pe_merged");
	}
	if (channel==40) {
		cut_A_S_lower(0.0073, 0.00068, 0.010, 0.00041, display, channel, "small_A-S_noise");
		x_y_regions = {0.0073, 0.0073, 0.00026, 0.04, 0.00375, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0187, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0073, 0.00143, 0.0187, 0.0022, display, channel, "2pe_merged");
	}
	if (channel==41) {
		cut_A_S_lower(0.0074, 0.00069, 0.010, 0.00041, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, 0.00033, 0.04, 0.00380, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0183, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00150, 0.0183, 0.00214, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0075, 0.00072, 0.010, 0.00047, display, channel, "small_A-S_noise");
		x_y_regions = {0.0075, 0.0075, 0.00023, 0.04, 0.00370, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0183, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0075, 0.00137, 0.0183, 0.00209, display, channel, "2pe_merged");
	}
	if (channel == 43) {
		cut_A_S_lower(0.0075, 0.00072, 0.010, 0.00047, display, channel, "small_A-S_noise");
		x_y_regions = {0.0075, 0.0075, 0.00025, 0.04, 0.00370, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0188, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0075, 0.00136, 0.0188, 0.00219, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0054, 0.000566, 0.010, 0.000266, display, channel, "small_A-S_noise");
		x_y_regions = {0.0054, 0.0054, 0.000015, 0.04, 0.00385, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0130, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0054, 0.00106, 0.0130, 0.00154, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0054, 0.000566, 0.010, 0.000266, display, channel, "small_A-S_noise");
		x_y_regions = {0.0054, 0.0054, 0.000015, 0.04, 0.00385, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0134, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0054, 0.00114, 0.0134, 0.00156, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0070, 0.000712, 0.015, 0.000153, display, channel, "small_A-S_noise");
		x_y_regions = {0.0070, 0.0070, 0.000153, 0.04, 0.00370, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0176, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0070, 0.00143, 0.0176, 0.00194, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0080, 0.00076, 0.015, 0.000355, display, channel, "small_A-S_noise");
		x_y_regions = {0.0080, 0.0080, 0.00032, 0.04, 0.00370, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0196, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0080, 0.00167, 0.0196, 0.00229, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0092, 0.00082, 0.015, 0.00040, display, channel, "small_A-S_noise");
		x_y_regions = {0.0092, 0.0092, 0.00045, 0.04, 0.00370, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0213, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0092, 0.00181, 0.0213, 0.00249, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0068, 0.00063, 0.010, 0.00040, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.0068, 0.00030, 0.04, 0.00380, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0165, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00142, 0.0165, 0.00194, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0068, 0.00063, 0.010, 0.00040, display, channel, "small_A-S_noise");
		x_y_regions = {0.0068, 0.0068, 0.00025, 0.04, 0.00384, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0168, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0068, 0.00139, 0.0168, 0.00196, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.0076, 0.00063, 0.010, 0.00040, display, channel, "small_A-S_noise");
		x_y_regions = {0.0076, 0.0076, 0.000388, 0.04, 0.00380, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0186, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0076, 0.00159, 0.0186, 0.00211, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0081, 0.00071, 0.010, 0.00057, display, channel, "small_A-S_noise");
		x_y_regions = {0.0081, 0.0081, 0.00046, 0.04, 0.00380, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0192, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0081, 0.00156, 0.0192, 0.00222, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0073, 0.00071, 0.015, 0.00027, display, channel, "small_A-S_noise");
		x_y_regions = {0.0073, 0.0073, 0.00024, 0.04, 0.00381, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0184, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0073, 0.00147, 0.0184, 0.00215, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0075, 0.00071, 0.015, 0.00027, display, channel, "small_A-S_noise");
		x_y_regions = {0.0075, 0.0075, 0.000186, 0.04, 0.00372, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0189, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0075, 0.00151, 0.0189, 0.00220, display, channel, "2pe_merged");
	}
}//2 == device_condition
if (1 == device_condition) {
if (channel==32) {
	cut_A_S_lower(0.0056, 0.00056, 0.01, 0.00042, display, channel, "small_A-S_noise");
	x_y_regions = {0.0056, 0.0056, 0.00041, 0.04, 0.00279, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0137, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0056, 0.00117, 0.0137, 0.00152, display, channel, "2pe_merged");
}
if (channel==33) {
	cut_A_S_lower(0.0050, 0.00040, 0.01, 0.00021, display, channel, "small_A-S_noise");
	x_y_regions = {0.0050, 0.0050, 0.00036, 0.04, 0.00241, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0103, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0050, 0.00101, 0.0103, 0.00113, display, channel, "2pe_merged");
}
if (channel==34) {
	cut_A_S_lower(0.0065, 0.00065, 0.01, 0.00051, display, channel, "small_A-S_noise");
	x_y_regions = {0.0065, 0.0065, 0.00053, 0.04, 0.00223, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0156, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0065, 0.00143, 0.0156, 0.00160, display, channel, "2pe_merged");
}
if (channel==35) {
	cut_A_S_lower(0.0062, 0.00066, 0.01, 0.00035, display, channel, "small_A-S_noise");
	x_y_regions = {0.0062, 0.0062, 0.00051, 0.04, 0.00164, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0150, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0062, 0.00139, 0.0150, 0.00159, display, channel, "2pe_merged");
}
if (channel==36) {
	cut_A_S_lower(0.0067, 0.00063, 0.01, 0.00031, display, channel, "small_A-S_noise");
	x_y_regions = {0.0067, 0.0067, 0.00048, 0.04, 0.00259, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0067, 0.00157, 0.0159, 0.00180, display, channel, "2pe_merged");
}
if (channel==37) {
	cut_A_S_lower(0.0053, 0.00055, 0.01, 0.00009, display, channel, "small_A-S_noise");
	x_y_regions = {0.0053, 0.0053, 0.00041, 0.04, 0.00212, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0121, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0053, 0.00122, 0.0121, 0.00134, display, channel, "2pe_merged");
}
if (channel==38) {
	cut_A_S_lower(0.0071, 0.00068, 0.01, 0.00034, display, channel, "small_A-S_noise");
	x_y_regions = {0.0071, 0.0071, 0.00049, 0.04, 0.00285, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0169, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0071, 0.00161, 0.0169, 0.00188, display, channel, "2pe_merged");
}
if (channel==39) {
	cut_A_S_lower(0.0056, 0.00057, 0.01, 0.00031, display, channel, "small_A-S_noise");
	x_y_regions = {0.0056, 0.0056, 0.00038, 0.04, 0.00247, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0056, 0.00137, 0.0145, 0.00160, display, channel, "2pe_merged");
}
if (channel==40) {
	cut_A_S_lower(0.0059, 0.00053, 0.01, 0.00030, display, channel, "small_A-S_noise");
	x_y_regions = {0.0059, 0.0059, 0.00043, 0.04, 0.00237, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0146, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0059, 0.00137, 0.0146, 0.00160, display, channel, "2pe_merged");
}
if (channel==41) {
	cut_A_S_lower(0.0060, 0.00057, 0.01, 0.00040, display, channel, "small_A-S_noise");
	x_y_regions = {0.0060, 0.0060, 0.00046, 0.04, 0.00237, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0143, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0060, 0.00136, 0.0143, 0.00155, display, channel, "2pe_merged");
}
if (channel==42) {
	cut_A_S_lower(0.0060, 0.00057, 0.01, 0.00040, display, channel, "small_A-S_noise");
	x_y_regions = {0.0060, 0.0060, 0.00046, 0.04, 0.00237, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0143, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0060, 0.00136, 0.0143, 0.00155, display, channel, "2pe_merged");
}
if (channel==43) {
	cut_A_S_lower(0.0060, 0.00058, 0.01, 0.00041, display, channel, "small_A-S_noise");
	x_y_regions = {0.0060, 0.0060, 0.00044, 0.04, 0.00252, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0146, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0060, 0.00135, 0.0146, 0.00160, display, channel, "2pe_merged");
}
if (channel==49) {
	cut_A_S_lower(0.0050, 0.00039, 0.01, 0.00040, display, channel, "small_A-S_noise");
	x_y_regions = {0.0050, 0.0050, 0.00032, 0.04, 0.00194, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0097, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0050, 0.00094, 0.0097, 0.00106, display, channel, "2pe_merged");
}
if (channel==50) {
	cut_A_S_lower(0.0050, 0.00039, 0.01, 0.00039, display, channel, "small_A-S_noise");
	x_y_regions = {0.0050, 0.0050, 0.00034, 0.04, 0.00158, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0098, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0050, 0.00102, 0.0098, 0.00106, display, channel, "2pe_merged");
}
if (channel==51) {
	cut_A_S_lower(0.0055, 0.00054, 0.01, 0.00038, display, channel, "small_A-S_noise");
	x_y_regions = {0.0055, 0.0055, 0.00044, 0.04, 0.00192, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0130, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0055, 0.00125, 0.0130, 0.00136, display, channel, "2pe_merged");
}
if (channel==52) {
	cut_A_S_lower(0.0066, 0.00065, 0.01, 0.00056, display, channel, "small_A-S_noise");
	x_y_regions = {0.0066, 0.0066, 0.00049, 0.04, 0.00244, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0066, 0.00152, 0.0155, 0.00181, display, channel, "2pe_merged");
}
if (channel==53) {
	cut_A_S_lower(0.0075, 0.00065, 0.01, 0.00057, display, channel, "small_A-S_noise");
	x_y_regions = {0.0075, 0.0075, 0.00053, 0.04, 0.00271, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0178, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0075, 0.00165, 0.0178, 0.00198, display, channel, "2pe_merged");
}
if (channel==54) {
	cut_A_S_lower(0.0053, 0.00054, 0.01, 0.00038, display, channel, "small_A-S_noise");
	x_y_regions = {0.0053, 0.0053, 0.00040, 0.04, 0.00231, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0125, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0053, 0.00127, 0.0125, 0.00142, display, channel, "2pe_merged");
}
if (channel==55) {
	cut_A_S_lower(0.0053, 0.00054, 0.01, 0.00047, display, channel, "small_A-S_noise");
	x_y_regions = {0.0053, 0.0053, 0.00040, 0.04, 0.00231, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0129, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0053, 0.00127, 0.0129, 0.00148, display, channel, "2pe_merged");
}
if (channel==56) {
	cut_A_S_lower(0.0062, 0.00055, 0.01, 0.00048, display, channel, "small_A-S_noise");
	x_y_regions = {0.0062, 0.0062, 0.00043, 0.04, 0.00269, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0146, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0062, 0.00131, 0.0146, 0.00168, display, channel, "2pe_merged");
}
if (channel==57) {
	cut_A_S_lower(0.0065, 0.00057, 0.01, 0.00049, display, channel, "small_A-S_noise");
	x_y_regions = {0.0065, 0.0065, 0.00048, 0.04, 0.00268, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0149, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0065, 0.00137, 0.0149, 0.00174, display, channel, "2pe_merged");
}
if (channel==58) {
	cut_A_S_lower(0.0061, 0.00053, 0.01, 0.00046, display, channel, "small_A-S_noise");
	x_y_regions = {0.0061, 0.0061, 0.00044, 0.04, 0.00257, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0144, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0061, 0.00135, 0.0144, 0.00163, display, channel, "2pe_merged");
}
if (channel==59) {
	cut_A_S_lower(0.0061, 0.00057, 0.01, 0.00049, display, channel, "small_A-S_noise");
	x_y_regions = {0.0061, 0.0061, 0.00046, 0.04, 0.00253, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0061, 0.00134, 0.0145, 0.00162, display, channel, "2pe_merged");
}
}//1==device_condition

}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2021.04.06
//Same as 210316/4analysis_history.cpp but using fPMTs for pulse-shape fit
data_output_path = "210302/results_v5/";
calibration_file = "210302/results_v1/210302_calibration.dat";
post_processor->calibr_info.Load(calibration_file);

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["210302_Pu_7.6kV_700V_46V_12dB_132K"] = std::pair<double, double> (24.9, 62);
S2_times["210302_Pu_7.6kV_800V_46V_12dB_131K"] = std::pair<double, double> (26.3, 58);
S2_times["210302_Pu_7.6kV_800V_46V_12dB_120K"] = std::pair<double, double> (42.7, 78);

std::map<std::string, std::string> exp_folders;
exp_folders["210302_Pu_7.6kV_700V_46V_12dB_132K"] = "Pu_46V_7.6kV_700V_132K";
exp_folders["210302_Pu_7.6kV_800V_46V_12dB_131K"] = "Pu_46V_7.6kV_800V_131K";
exp_folders["210302_Pu_7.6kV_800V_46V_12dB_120K"] = "Pu_46V_7.6kV_800V_120K";

std::map<std::string, double> pre_trigger_max_S; //for calibration
pre_trigger_max_S["210302_Pu_7.6kV_700V_46V_12dB_132K"] = 0.047;
pre_trigger_max_S["210302_Pu_7.6kV_800V_46V_12dB_131K"] = 0.041;
pre_trigger_max_S["210302_Pu_7.6kV_800V_46V_12dB_120K"] = 0.094;

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
if (calibrate) {
	std::cout<<"Use 1analysis_history.cpp for calibration! Doing nothing."<<std::endl;
	return;
}
//END OF CALIBRATION

//SIGNAL FORMS
if (!forms)
	return;
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
int first_run = 0;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //800V, 12 dB
int SiPM_state = 0; //46V
if (exp == "210302_Pu_7.6kV_700V_46V_12dB_132K") {
	PMT_state = 1; //700V, 12 dB
	SiPM_state = 0; //SiPMs are sensitive to temperature
}
if (exp == "210302_Pu_7.6kV_800V_46V_12dB_131K") {
	PMT_state = 0; //800V, 12 dB
	SiPM_state = 1;
}
if (exp == "210302_Pu_7.6kV_800V_46V_12dB_120K") {
	PMT_state = 0; //800V, 12 dB
	SiPM_state = 2;
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
if (exp == "210302_Pu_7.6kV_800V_46V_12dB_120K") {
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
	set_zoom(0, 1500, 0, 1500);
	set_bins(1500);
	x_y_regions = {148.8, 0, 1499, 1206};
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
	x_y_regions = {157, 0, 999, 757};
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
	ShapeFitData::SetBound(1, 58, 70);
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
	ShapeFitData::SetBound(1, 58, 70);
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
	set_zoom(0, 1400, 0, 6);
	set_bins(1400, 500);
	set_titles("N_{pe} SiPMs t#in[0, 160] #mus", "fPMTs fitted width [#mus]");
	x_y_regions = {802, 1.27, 527, 1.27, 477, 2.15, 849, 2.15};
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
	x_y_regions = {886, 3.14, 656, 3.14, 784, 4.63, 876, 4.63};
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
	x_y_regions = {857, 2.26, 490, 2.26, 490, 2.97, 896, 2.97};
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
if (exp == "210302_Pu_7.6kV_800V_46V_12dB_131K") {
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
	set_zoom(0, 1500, 0, 1500);
	set_bins(1500);
	x_y_regions = {95, 0, 1499, 1257};
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
	x_y_regions = {104, 0, 999, 831};
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
	set_zoom(0, 1000, 0, 6);
	set_bins(1000, 500);
	set_titles("N_{pe} SiPMs t#in[0, 160] #mus", "fPMTs fitted width [#mus]");
	x_y_regions = {622, 1.33, 423, 1.33, 381, 2.08, 688, 2.08};
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
	x_y_regions = {701, 2.94, 489, 2.94, 592, 3.98, 690, 3.98};
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
	x_y_regions = {669, 2.16, 402, 2.16, 491, 2.82, 701, 2.82};
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
//END OF FORMS
}
