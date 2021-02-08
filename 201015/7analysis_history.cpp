void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
std::string cuts_str(std::vector<int> cuts);
void central_SiPMs(bool on);
void fast_PMTs_only (void);
void slow_PMTs_only (void);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);
int save_forms (std::string prefix, std::string output_file, int Nbins, int type);

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

void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition)
{
	double zoom_l = 10, zoom_r = 80;
	std::size_t Nbins = 800;
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
		cut_A_S_upper(0.01, 0.0, 0.019, 0.0, display, channel, "rem_smallA_largeS_1");
		cut_A_S_lower(0.015, 0.00052, 0.055, 0.004, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.055, 0.004, 1.1, 0.186, display, channel, "rem_largeA_smallS");
		if (aggressiveness == 1) {
			cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
			cut_A_S_upper(0.0, 2.0, 1.6, 2, display, channel, "rem_S>~2.0");
		}
	}
	if (channel==2) {
		cut_A_S_upper(0.013, 0.0, 0.030, 0.0, display, channel, "rem_smallA_largeS_1");
		cut_A_S_lower(0.0170, 0.0012, 0.075, 0.0032, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.075, 0.0032, 1.6, 0.28, display, channel, "rem_largeA_smallS");
		if (aggressiveness == 1) {
		  cut_A_S_upper(1.05, 0, 2, 0, display, channel, "rem_A>1.05");
		  cut_A_S_upper(0.0, 2.0, 1.6, 2, display, channel, "rem_S>~2.0");
	  }
	}
	if (channel==3) {
		cut_A_S_upper(0.013, 0.0, 0.020, 0.0, display, channel, "rem_smallA_largeS");
		cut_A_S_lower(0.013, 0.00059, 0.055, 0.0028, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.055, 0.0028, 1.2, 0.20, display, channel, "rem_largeA_smallS");
		if (aggressiveness == 1) {
			cut_A_S_upper(1.2, 0, 2, 0, display, channel, "rem_A>1.2");
			cut_A_S_upper(0.0, 2.0, 1.6, 2, display, channel, "rem_S>2.0");
		}
	}
	if (channel==4) {
		cut_A_S_upper(0.013, 0.0, 0.0195, 0.0, display, channel, "rem_smallA_largeS_1");
		cut_A_S_lower(0.0165, 0.00082, 0.055, 0.0030, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.055, 0.0030, 1.6, 0.27, display, channel, "rem_largeA_smallS");
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
		cut_A_S_upper(0.003, 0.00019, 0.0050, 0.00038, display, channel, "rem_smallA_largeS");
		if (aggressiveness == 1) {
			cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
			cut_A_S_upper(0.0, 0.015, 1.6, 0.015, display, channel, "rem_S>0.015");
		}
	}
	//qewr
	//The following are set by hand for 20kV individually, there is no other way.
	if (channel==32) {
	    cut_A_S_lower(0.0105, 0.00118, 0.05, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0105, 0.0105, 0.00037, 0.06, 0.00575, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0253, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0105, 0.00162, 0.0253, 0.00337, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.008, 0.00115, 0.023, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0085, 0.008, 0.00042, 0.06, 0.006, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0205, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.008, 0.0013, 0.0205, 0.0029, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0115, 0.00150, 0.027, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0115, 0.0115, 0.00071, 0.06, 0.00562, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0274, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0115, 0.00175, 0.0274, 0.0036, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0110, 0.00126, 0.06, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0110, 0.0110, 0.00018, 0.06, 0.00565, 1e3};
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
		cut_A_S_lower(0.012, 0.00130, 0.03, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.012, 0.012, 0.00034, 0.06, 0.00591, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.012, 0.002, 0.0285, 0.0042, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0094, 0.00114, 0.06, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0094, 0.0094, 0.000077, 0.06, 0.0059, 1e3};
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
		cut_A_S_lower(0.0110, 0.00121, 0.038, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0110, 0.0097, 0.00002, 0.06, 0.00576, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0110, 0.00161, 0.0255, 0.0037, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0112, 0.00125, 0.028, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0112, 0.0005, 0.06, 0.00580, 1e3};
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
		cut_A_S_lower(0.0112, 0.00125, 0.028, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0112, 0.0112, 0.0005, 0.06, 0.00580, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0112, 0.00167, 0.0255, 0.0037, display, channel, "2pe_merged");
	}
	if (channel == 44) {
		cut_A_S_lower(0.0105, 0.0011, 0.03, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0105, 0.0105, 0.0006, 0.06, 0.00580, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0221, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0105, 0.00145, 0.0221, 0.0033, display, channel, "2pe_merged");
	}
	if (channel==48) {
		cut_A_S_lower(0.0113, 0.00132, 0.027, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0113, 0.0113, 0.00058, 0.06, 0.00595, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0270, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0113, 0.00174, 0.0270, 0.00375, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0086, 0.0010, 0.026, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0086, 0.0086, 0.00042, 0.06, 0.006, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0086, 0.00137, 0.02, 0.0028, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0090, 0.0010, 0.036, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0090, 0.0085, 0.00021, 0.06, 0.0061, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0196, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0090, 0.00145, 0.0196, 0.00279, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0097, 0.00106, 0.032, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0097, 0.0097, 0.00021, 0.06, 0.0058, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0248, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0097, 0.00172, 0.0248, 0.00337, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0107, 0.00127, 0.042, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0107, 0.0107, 0.00055, 0.06, 0.0061, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0258, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0107, 0.00185, 0.0258, 0.00384, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0114, 0.00132, 0.033, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0114, 0.0114, 0.00032, 0.06, 0.00595, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0114, 0.00196, 0.028, 0.00436, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0092, 0.00114, 0.044, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0092, 0.0092, 0.00016, 0.06, 0.0061, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0092, 0.00177, 0.023, 0.00325, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0094, 0.00159, 0.0172, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0094, 0.0094, 0.00040, 0.06, 0.0057, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0235, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0094, 0.00174, 0.0235, 0.0034, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.0119, 0.00132, 0.0214, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0119, 0.0119, 0.00082, 0.06, 0.0057, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0119, 0.00170, 0.0255, 0.0038, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0107, 0.00124, 0.060, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0107, 0.0107, 0.000316, 0.06, 0.006, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0107, 0.00191, 0.0255, 0.0037, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0103, 0.00122, 0.04, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0103, 0.0103, 0.000336, 0.06, 0.0062, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0246, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0103, 0.00182, 0.0246, 0.00367, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0118, 0.00133, 0.024, 0.0, display, channel, "small_A-S_noise");
		x_y_regions = {0.0118, 0.0118, 0.00077, 0.06, 0.00575, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0118, 0.00167, 0.0255, 0.00364, display, channel, "2pe_merged");
	}

}

void analysis_history(bool calibrate, unsigned int iteration = 0) {
//Created on 10.11.2020
//Trigger adjustment using SiPM-matrix form fit. The rest is based on 5analysis_history.cpp
clearAll();
data_output_path = "201015/results_v7/";
calibration_file = "201015/results_v5/201015_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::string iter_str = int_to_str((std::size_t)iteration, 2);

gStyle->SetOptStat("emr");

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["201015_Pu_20kV_850V_46V_12dB"] = std::pair<double, double> (23, 40);
S2_times["201015_Pu_18kV_850V_46V_12dB"] = std::pair<double, double> (23, 40);
S2_times["201015_Pu_16kV_850V_46V_12dB"] = std::pair<double, double> (23, 38);
S2_times["201015_Pu_14kV_850V_46V_12dB"] = std::pair<double, double> (23, 38);
S2_times["201015_Pu_12kV_850V_46V_12dB"] = std::pair<double, double> (23, 34);
S2_times["201015_Pu_10kV_850V_46V_12dB"] = std::pair<double, double> (23, 34);

std::map<std::string, std::string> exp_folders;
exp_folders["201015_Pu_20kV_850V_46V_12dB"] = "Pu_46V_20kV_850V";
exp_folders["201015_Pu_18kV_850V_46V_12dB"] = "Pu_46V_18kV_850V";
exp_folders["201015_Pu_16kV_850V_46V_12dB"] = "Pu_46V_16kV_850V";
exp_folders["201015_Pu_14kV_850V_46V_12dB"] = "Pu_46V_14kV_850V";
exp_folders["201015_Pu_12kV_850V_46V_12dB"] = "Pu_46V_12kV_850V";
exp_folders["201015_Pu_10kV_850V_46V_12dB"] = "Pu_46V_10kV_850V";

std::map<std::string, double> pre_trigger_max_S;
pre_trigger_max_S["201015_Pu_20kV_850V_46V_12dB"] = 0.4;
pre_trigger_max_S["201015_Pu_18kV_850V_46V_12dB"] = 0.3;
pre_trigger_max_S["201015_Pu_16kV_850V_46V_12dB"] = 0.23;
pre_trigger_max_S["201015_Pu_14kV_850V_46V_12dB"] = 0.18;
pre_trigger_max_S["201015_Pu_12kV_850V_46V_12dB"] = 0.13;
pre_trigger_max_S["201015_Pu_10kV_850V_46V_12dB"] = 0.12;

bool forms = !calibrate;

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
if (exp == "201015_Pu_20kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, chan);
	}
	set_bins(0, 150);
	set_log_y();
	draw_limits(0, 57);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	if (iteration == 0) {
		ty(AStates::PMT_Npe_sum);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 350);
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
		set_bins(0, 350);
		saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);

		ty(AStates::PMT_sum_N);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 350);
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);
	}

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::MPPC_trigger_fit); //May have poor results because of merged and >1phe peaks
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, SiPM_state, false);
		}
		set_zoom(-20, 20);
		set_bins(300);
		draw_limits(-5, 5);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		TriggerFitData::SetTriggerFirstScant(1.0);
		TriggerFitData::SetTriggerPrecision(0.2);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_SiPMs_by_Npe.dat");
		saveaspng(FOLDER + Num + "_SiPMs_Npe_trigger_"+cuts_str(cuts)+"_zoom");
		set_as_run_cut("good_trigger"); cuts.push_back(Num);
		set_trigger_offsets(0.0);
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	x_y_regions = {137, 0, 599.9, 340};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	if (iteration == 0) {
		set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
		set_zoom(0, 600, 0, 600);
		set_bins(600);
		set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
		set_log_z();
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
		unset_log_z();
		Num = int_to_str(++no, 2);
	}

std::string form_n = "forms_Alpha_left/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(280, 345);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(280, 408);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npe", 1200, 2); //For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 1200, 5); //For next iteration trigger fit
}
if (exp == "201015_Pu_18kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, chan);
	}
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 49);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	if (iteration == 0) {
		ty(AStates::PMT_Npe_sum);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 350);
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
		set_bins(0, 350);
		saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);

		ty(AStates::PMT_sum_N);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 350);
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);
	}

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::MPPC_trigger_fit); //May have poor results because of merged and >1phe peaks
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, SiPM_state, false);
		}
		set_zoom(-20, 20);
		set_bins(300);
		draw_limits(-5, 5);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.2);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_SiPMs_by_Npe.dat");
		saveaspng(FOLDER + Num + "_SiPMs_Npe_trigger_"+cuts_str(cuts)+"_zoom");
		set_as_run_cut("good_trigger"); cuts.push_back(Num);
		set_trigger_offsets(0.0);
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	x_y_regions = {114, 0, 499.9, 290};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	if (iteration == 0) {
		set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
		set_zoom(0, 500, 0, 500);
		set_bins(500);
		set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
		set_log_z();
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
		unset_log_z();
		Num = int_to_str(++no, 2);
	}

std::string form_n = "forms_Alpha_left/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(216, 266);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(216, 309);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npe", 1200, 2); //For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 1200, 5); //For next iteration trigger fit
}
if (exp == "201015_Pu_16kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, chan);
	}
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 32);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	if (iteration == 0) {
		ty(AStates::PMT_Npe_sum);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 250);
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
		set_bins(0, 250);
		saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);

		ty(AStates::PMT_sum_N);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 250);
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);
	}

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::MPPC_trigger_fit); //May have poor results because of merged and >1phe peaks
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, SiPM_state, false);
		}
		set_zoom(-20, 20);
		set_bins(300);
		draw_limits(-5, 5);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.2);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_SiPMs_by_Npe.dat");
		saveaspng(FOLDER + Num + "_SiPMs_Npe_trigger_"+cuts_str(cuts)+"_zoom");
		set_as_run_cut("good_trigger"); cuts.push_back(Num);
		set_trigger_offsets(0.0);
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {76, 0, 399.9, 230};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	if (iteration == 0) {
		set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
		set_zoom(0, 400, 0, 400);
		set_bins(400);
		set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
		set_log_z();
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
		unset_log_z();
		Num = int_to_str(++no, 2);
	}

std::string form_n = "forms_Alpha_left/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(150, 191);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(150, 223);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npe", 1200, 2); //For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 1200, 5); //For next iteration trigger fit
}
if (exp == "201015_Pu_14kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, chan);
	}
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 26);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	if (iteration == 0) {
		ty(AStates::PMT_Npe_sum);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 200);
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
		set_bins(0, 200);
		saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);

		ty(AStates::PMT_sum_N);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 200);
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);
	}

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::MPPC_trigger_fit); //May have poor results because of merged and >1phe peaks
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, SiPM_state, false);
		}
		set_zoom(-20, 20);
		set_bins(300);
		draw_limits(-5, 5);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.2);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_SiPMs_by_Npe.dat");
		saveaspng(FOLDER + Num + "_SiPMs_Npe_trigger_"+cuts_str(cuts)+"_zoom");
		set_as_run_cut("good_trigger"); cuts.push_back(Num);
		set_trigger_offsets(0.0);
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {54.6, 0, 299.9, 190};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	if (iteration == 0) {
		set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
		set_zoom(0, 300, 0, 300);
		set_bins(300);
		set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
		set_log_z();
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
		unset_log_z();
		Num = int_to_str(++no, 2);
	}

std::string form_n = "forms_Alpha_left/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(100, 128);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(100, 157);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npe", 1200, 2); //For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 1200, 5); //For next iteration trigger fit
}
if (exp == "201015_Pu_12kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, chan);
	}
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 20);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	if (iteration == 0) {
		ty(AStates::PMT_Npe_sum);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 200);
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
		set_bins(0, 200);
		saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);

		ty(AStates::PMT_sum_N);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 200);
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);
	}

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::MPPC_trigger_fit); //May have poor results because of merged and >1phe peaks
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, SiPM_state, false);
		}
		set_zoom(-20, 20);
		set_bins(300);
		draw_limits(-5, 5);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.2);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_SiPMs_by_Npe.dat");
		saveaspng(FOLDER + Num + "_SiPMs_Npe_trigger_"+cuts_str(cuts)+"_zoom");
		set_as_run_cut("good_trigger"); cuts.push_back(Num);
		set_trigger_offsets(0.0);
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {35, 0, 199.9, 133};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	if (iteration == 0) {
		set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
		set_zoom(0, 200, 0, 200);
		set_bins(200);
		set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
		set_log_z();
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
		unset_log_z();
		Num = int_to_str(++no, 2);
	}

std::string form_n = "forms_Alpha_left/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(55, 81);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(55, 102);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npe", 1200, 2); //For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 1200, 5); //For next iteration trigger fit
}
if (exp == "201015_Pu_10kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, chan);
	}
	set_bins(0, 100);
	set_log_y();
	draw_limits(0, 14);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-"+S2_start+"us");
	set_as_run_cut("good_pre-trigger");	cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	unset_log_y();
	unset_draw_limits();

	if (iteration == 0) {
		ty(AStates::PMT_Npe_sum);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 200);
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
		set_bins(0, 200);
		saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);

		ty(AStates::PMT_sum_N);
		fast_PMTs_only();
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		time_zoom_fPMTs(d_S2_start, 160);
		set_bins(0, 200);
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
		Num = int_to_str(++no, 2);
	}

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::MPPC_trigger_fit); //May have poor results because of merged and >1phe peaks
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, SiPM_state, false);
		}
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.2);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_SiPMs_by_Npe.dat");
		draw_limits(-5, 5);
		saveaspng(FOLDER + Num + "_SiPMs_Npe_trigger_"+cuts_str(cuts)+"_zoom");
		set_as_run_cut("good_trigger"); cuts.push_back(Num);
		set_trigger_offsets(0.0);
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {20, 0, 199.9, 164};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	if (iteration == 0) {
		set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
		set_zoom(0, 200, 0, 200);
		set_bins(200);
		set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts));
		set_log_z();
		saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_log");
		unset_log_z();
		Num = int_to_str(++no, 2);
	}

std::string form_n = "forms_Alpha_left/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(30, 46);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(30, 60);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);
	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npe", 1200, 2); //For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 1200, 5); //For next iteration trigger fit
}
//END OF FORMS
}
#define PAIR std::pair<double, double>

double integrate(TH1D *hist, double from, double to) {
  double Int = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
      Int += hist->GetBinContent(bin) * hist->GetBinWidth(bin);
    }
  }
  return Int;
}

double integrate(TH1D *hist, double from, double to, double baseline) {
  double Int = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
      double val = std::max(hist->GetBinContent(bin) - baseline, 0.0);
      Int += val * hist->GetBinWidth(bin);
    }
  }
  return Int;
}

double average(TH1D *hist, double from, double to) {
  double baseline = 0;
  int counts = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
      baseline += hist->GetBinContent(bin);
      ++counts;
    }
  }
  return baseline/counts;
}

void subtract_baseline(TH1D *hist, double baseline) {
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    hist->SetBinContent(bin, std::max(hist->GetBinContent(bin) - baseline, 0.0));
  }
}

void print_hist(TH1D *hist, std::ofstream &str) {
  if (!str.is_open() || hist == NULL) {
    std::cout<<"Error: print_hist: invalid parameters"<<std::endl;
    return;
  }
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    str<<hist->GetBinCenter(bin)<<"\t"<<hist->GetBinContent(bin)<<std::endl;
  }
}

void read_hist (TH1D *hist, std::string fname) {
  std::ifstream str;
  str.open(fname, std::ios_base::binary);
  if (!str.is_open()) {
    std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;
    return;
  }
  std::size_t real_size = 0;
  str.read((char*)&real_size, sizeof(std::size_t));
  std::cout<<"File '"<<fname<<"': "<<real_size<<" events"<<std::endl;
  double val;
  while (!str.eof()) {
    str.read((char*)&val, sizeof(double));
    hist->Fill(val);
  }
  str.close();
}

void read_hist_w (TH1D *hist, std::string fname, double offset = 0) {
  std::ifstream str;
  str.open(fname, std::ios_base::binary);
  if (!str.is_open()) {
    std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;
    return;
  }
  std::size_t real_size = 0;
  str.read((char*)&real_size, sizeof(std::size_t));
  std::cout<<"File '"<<fname<<"': "<<real_size<<" events"<<std::endl;
  double val1;
  double val2;
  while (!str.eof()) {
    str.read((char*)&val1, sizeof(double));
    if (str.eof())
      break;
    str.read((char*)&val2, sizeof(double));
    hist->Fill(val1 + offset, val2);
  }
  str.close();
}
// Created on 2020.11.10
// Designed to be called from analysis_history to create file which
// will be loaded in TriggerFitData::SetPulseShape in order to
// iteratively adjust the trigger. This means this function will
// be called after each analysis_history pass for PMTs/SiPMs for
// all electric fields.
int save_forms (std::string prefix, std::string output_file, int Nbins, int type) {
  //type: 1 - 4PMT by Npeaks, 2 - 4PMT by Npe, 3 - 4PMT by S
  //4 - SiPM-matrix by Npeaks, 5 - SiPM-matrix by Npe, 6 - SiPM-matrix by S
  std::string output_data = output_file + ".dat";
  std::string output_fig = output_file + ".png";
  std::ofstream output;
  open_output_file(output_data, output, std::ios_base::trunc);
  if (!output.is_open()) {
    std::cout<<"Error: save_forms: could not open output file \""<<output_file<<"\""<<std::endl;
    return 1;
  }
  if (type<1 || type>6) {
    std::cout<<"Error: save_forms: input int type="<<type<<" is out of range [1, 6]"<<std::endl;
    return 1;
  }
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  int DEF_W = 1300, DEF_H = 700;
  std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
  std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};

  bool linear = 1;

  bool subtact_baseline = false;
  bool renormalize = true;
  double time_pretrigger_left = 5, time_pretrigger_right = 15;
  double time_left = 0, time_right = 160;//us
  double max_val = 0;
  double y_min = 1e-4;

  std::string device = type > 3 ? "SiPM-matrix" : "4PMT";
  std::string weight_type;
  switch ((type - 1) % 3) {
  case 0: {
    weight_type = "by Npeaks";
    break;
  }
  case 1: {
    weight_type = "by Npe";
    break;
  }
  case 2: {
    weight_type = "by S";
    break;
  }
  }

  std::string framename = prefix + " " + device + " " + weight_type;
  std::string histname = std::string("hist_") + prefix + " " + device + " " + weight_type;
  TH1D *hist = new TH1D (histname.c_str(), histname.c_str(), Nbins, time_left, time_right);

  std::vector<std::string> files_to_read;
  if (type > 3) {
    std::string fname = "SiPMs_form_by_";
    switch ((type - 1) % 3) {
    case 0: {
      fname += "Npeaks";
      break;
    }
    case 1: {
      fname += "Npe";
      break;
    }
    case 2: {
      fname += "S";
      break;
    }
    }
    fname += ".hdata";
    fname = prefix + fname;
    files_to_read.push_back(fname);
  } else {
    files_to_read = {"5_form_by_", "6_form_by_", "7_form_by_", "8_form_by_"};
    for (std::size_t f = 0, f_end_ = files_to_read.size(); f!=f_end_; ++f) {
      switch ((type - 1) % 3) {
      case 0: {
        files_to_read[f] += "Npeaks";
        break;
      }
      case 1: {
        files_to_read[f] += "Npe";
        break;
      }
      case 2: {
        files_to_read[f] += "S";
        break;
      }
      }
      files_to_read[f] += ".hdata";
      files_to_read[f] = prefix + files_to_read[f];
    }
  }

  for (std::size_t f = 0, f_end_ = files_to_read.size(); f!=f_end_; ++f)
    read_hist_w (hist, files_to_read[f], 0.0);

  double baseline = average(hist, time_pretrigger_left, time_pretrigger_right);
  if (subtact_baseline) {
    subtract_baseline(hist, baseline);
    baseline = 0;
  }
  if (renormalize) {
    double integral = integrate(hist, time_left, time_right);
    hist->Scale(1.0/integral);
    baseline *= 1.0/integral;
  }
  max_val = hist->GetBinContent(hist->GetMaximumBin());
  max_val*= linear ? 1.2 : 2;

  gStyle->SetGridStyle(3);
  gStyle->SetGridColor(14);
  gStyle->SetGridWidth(1);
  gStyle->SetOptStat("");
  TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
  c_->SetGrid(); c_->SetTicks(); c_->ToggleEventStatus(); c_->ToggleToolBar();
  if (!linear)
    c_->SetLogy();
  TLegend *legend = new TLegend(0.35, 0.7, 0.9, 0.9);
  //legend->SetHeader("");
  legend->SetMargin(0.25);
  TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : y_min, max_val);
  frame->GetXaxis()->SetTitle("Time [#mus]");
  frame->GetXaxis()->SetRangeUser(20, 60);
  frame->GetYaxis()->SetTitle("PE peak counts");
  frame->Draw();

  hist->SetLineWidth(2);
  hist->SetLineColor(palette_major[0]);
  hist->Draw("hist Lsame");

  legend->AddEntry(hist, framename.c_str(), "l");

  frame->Draw("sameaxis");
  legend->Draw("same");
  c_->Update();

  print_hist(hist, output);

  ensure_file(output_fig);
	c_->SaveAs(output_fig.c_str(), "png");
  delete c_;
  return 0;
}
