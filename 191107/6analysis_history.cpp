void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
void central_SiPMs(bool on);
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
	double zoom_l = 20, zoom_r = 160;
	std::size_t Nbins = 1600;
	ty(AStates::MPPC_tbNpe_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_condition, false);
	}
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(path + "SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(5);
	noise_cut(5, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "5_form_by_Npeaks");
	ch(6);
	noise_cut(6, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "6_form_by_Npeaks");
    ch(7);
	noise_cut(7, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "7_form_by_Npeaks");
    ch(8);
	noise_cut(8, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "8_form_by_Npeaks");
	ch(0);
	noise_cut(0, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "0_form_by_Npe");
	ch(1);
	noise_cut(1, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "1_form_by_Npe");
	ch(2);
	noise_cut(2, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "2_form_by_Npe");
	ch(3);
	noise_cut(3, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "3_form_by_Npe");
	ch(4);
	noise_cut(4, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "4_form_by_Npe");

	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(5);
		noise_cut(5, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "5_form_by_S");
		ch(6);
		noise_cut(6, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "6_form_by_S");
		ch(7);
		noise_cut(7, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "7_form_by_S");
		ch(8);
		noise_cut(8, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "8_form_by_S");
		ch(0);
		noise_cut(0, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "0_form_by_S");
		ch(1);
		noise_cut(1, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "1_form_by_S");
		ch(2);
		noise_cut(2, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "2_form_by_S");
		ch(3);
		noise_cut(3, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "3_form_by_S");
		ch(4);
		noise_cut(4, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "4_form_by_S");
	}
}

//Even if it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
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
	cut_A_S_upper(0.01, 0.0, 0.021, 0.0, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.0006, 0.055, 0.0026, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.055, 0.0026, 1.1, 0.22, display, channel, "rem_largeA_smallS");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 2.0, 1.6, 2, display, channel, "rem_S>~2.0");
    }
}
if (channel==2) {
	cut_A_S_upper(0.013, 0.0, 0.026, 0.0, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0170, 0.0010, 0.057, 0.0035, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.057, 0.0035, 1.6, 0.28, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.027, 0.0330, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.05, 0, 2, 0, display, channel, "rem_A>1.05");
		cut_A_S_upper(0.0, 3.0, 1.6, 3, display, channel, "rem_S>~3.0");
    }
}
if (channel==3) {
    cut_A_S_upper(0.013, 0.0073, 0.022, 0.0083, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.00082, 0.055, 0.0037, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.055, 0.0037, 1.6, 0.3, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.0284, 0.0255, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.2, 0, 2, 0, display, channel, "rem_A>1.2");
		cut_A_S_upper(0.0, 3.0, 1.6, 3, display, channel, "rem_S>3.0");
    }
}
if (channel==4) {
	cut_A_S_upper(0.013, 0.0073, 0.022, 0.0083, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.00082, 0.055, 0.0040, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.055, 0.0040, 1.6, 0.27, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.0284, 0.0255, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.2, 0, 2, 0, display, channel, "rem_A>1.2");
		cut_A_S_upper(0.0, 5.0, 1.6, 5, display, channel, "rem_S>5.0");
    }
}
if (channel==5) {
	if (aggressiveness == 1) {
		cut_A_S_upper(0.18, 0, 2, 0, display, channel, "rem_A>0.18");
		cut_A_S_upper(0.0, 0.006, 1.6, 0.006, display, channel, "rem_S>0.006");
	}
}
if (channel==6) {
	cut_A_S_upper(0.002, 0.0003, 0.0044, 0.00038, display, channel, "rem_smallA_largeS");
    if (aggressiveness == 1) {
		cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
		cut_A_S_upper(0.0, 0.012, 1.6, 0.012, display, channel, "rem_S>0.012");
	}
}
if (channel==7) {
	if (aggressiveness == 1) {
		cut_A_S_upper(0.12, 0, 2, 0, display, channel, "rem_A>0.12");
		cut_A_S_upper(0.0, 0.006, 1.6, 0.006, display, channel, "rem_S>0.006");
	}
}
if (channel==8) {
   cut_A_S_upper(0.003, 0.000257, 0.0052, 0.000356, display, channel, "rem_smallA_largeS");
    if (aggressiveness == 1) {
		cut_A_S_upper(0.4, 0, 2, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.025, 1.6, 0.025, display, channel, "rem_S>0.025");
	}
}
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
	cut_A_S_lower(0.0097, 0.00103, 0.046, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0097, 0.0097, 0.00002, 0.06, 0.00576, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0097, 0.00177, 0.0255, 0.0037, display, channel, "2pe_merged");
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
if (channel == 43 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel == 44 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
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
	cut_A_S_lower(0.0085, 0.0008, 0.04, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0085, 0.0085, 0.00021, 0.06, 0.0061, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0196, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0085, 0.00167, 0.0196, 0.00279, display, channel, "2pe_merged");
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
	cut_A_S_lower(0.0094, 0.0009, 0.035, 0.00193, display, channel, "small_A-S_noise");
	cut_A_S_lower(0.0094, 0.00137, 0.016, 0.0, display, channel, "small_A-S_noise3");
	x_y_regions = {0.0094, 0.0094, 0.000236, 0.06, 0.0061, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0235, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0094, 0.00174, 0.0235, 0.0034, display, channel, "2pe_merged");
}
if (channel==56) {
	cut_A_S_lower(0.0099, 0.00114, 0.034, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0099, 0.0099, 0.00013, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0099, 0.00170, 0.0255, 0.0038, display, channel, "2pe_merged");
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
	x_y_regions = {0.0118, 0.0118, 0.00077, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0118, 0.00167, 0.0255, 0.00364, display, channel, "2pe_merged");
}

}

void analysis_history(bool calibrate, unsigned int iteration = 0) {
//Created on 2020.06.28
//iteration = 0 - no trigger adjustment.
//This analysis focuses on S1 & S2 amplitudes to compare with Vlad's results.
//Based on 5analysis_history.cpp
data_output_path = "191107/results_v6/";
calibration_file = "191107/results_v5/191107_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::string iter_str = int_to_str((std::size_t)iteration, 2);

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["191107_U_20kV_850V_46V_12dB"] = std::pair<double, double> (85, 110);
S2_times["191107_U_18kV_850V_46V_12dB"] = std::pair<double, double> (87, 107);
S2_times["191107_U_16kV_850V_46V_12dB"] = std::pair<double, double> (87, 105);
S2_times["191107_U_14kV_850V_46V_12dB"] = std::pair<double, double> (87, 103);
S2_times["191107_U_12kV_850V_46V_12dB"] = std::pair<double, double> (87, 103);
S2_times["191107_U_10kV_850V_46V_12dB"] = std::pair<double, double> (87, 100);
S2_times["191107_U_8kV_850V_46V_12dB"] = std::pair<double, double> (85, 98);
S2_times["191107_U_6kV_850V_46V_12dB"] = std::pair<double, double> (85, 98);

std::map<std::string, std::string> exp_folders;
exp_folders["191107_U_20kV_850V_46V_12dB"] = "Alpha_46V_20kV_850V";
exp_folders["191107_U_18kV_850V_46V_12dB"] = "Alpha_46V_18kV_850V";
exp_folders["191107_U_16kV_850V_46V_12dB"] = "Alpha_46V_16kV_850V";
exp_folders["191107_U_14kV_850V_46V_12dB"] = "Alpha_46V_14kV_850V";
exp_folders["191107_U_12kV_850V_46V_12dB"] = "Alpha_46V_12kV_850V";
exp_folders["191107_U_10kV_850V_46V_12dB"] = "Alpha_46V_10kV_850V";
exp_folders["191107_U_8kV_850V_46V_12dB"] = "Alpha_46V_8kV_850V";
exp_folders["191107_U_6kV_850V_46V_12dB"] = "Alpha_46V_6kV_850V";

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
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
for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) {
int channel = calib_channels[chi];
std::string ch_str = int_to_str(channel);
//parameters set by Cd_20kV_850V_46V, but valid for all fields.
if (channel>=32) {
	if (getIndex(post_processor->MPPC_channels, channel)<0)
		continue;
ty(AStates::MPPC_A_S);
	ch(channel);
    set_zoom(0, 0.06, 0, 0.01);
    set_bins(1000);
    noise_cut(channel, 1, SiPM_state, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 1, SiPM_state, false);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_single_pe");
}
if (channel==1) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.1, 0, 2);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.2, 0, 0.07);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.014);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==2) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.6, 0, 6);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.2, 0, 0.07);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(100);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.1);
    set_bins(600);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.025);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==3) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.6, 0, 6);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.2, 0, 0.07);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(100);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.012);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==4) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.6, 0, 6);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.2, 0, 0.07);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(100);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.03);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==5) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.2, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
  	set_zoom(0, 0.05, 0, 0.01);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.001);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.00026);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==6) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.4, 0, 0.02);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
  	set_zoom(0, 0.1, 0, 0.01);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.002);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.00037);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==7) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.14, 0, 0.02);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
  	set_zoom(0, 0.03, 0, 0.003);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.001);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.00024);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.5, 0, 0.07);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.1, 0, 0.006);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.002);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.0005);
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
gStyle->SetOptStat("emr");
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
int first_run = 0;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //850V, 12 dB
int SiPM_state = 0; //46V
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

if (exp == "191107_U_20kV_850V_46V_12dB") {
	double d_S2_start = 85, d_S2_finish = 110;
	double d_S1_start = 62.4, d_S1_finish = 64.8;
	double d_bkg1_start = 52.4, d_bkg1_finish = 54.8;
	double d_bkg2_start = 10, d_bkg2_finish = 60;
	std::string S2_start = "85", S2_finish = "110";
	std::string S1_start = "62.4", S1_finish = "64.8";
	std::string bkg1_start = "52.4", bkg1_finish = "54.8";
	std::string bkg2_start = "10", bkg2_finish = "60";

	std::string cuts = "no_cuts";
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich = 0; ich != post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_N_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_N_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 110);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	x_y_regions = {44, 0, 75.7, 11.9, 86.2, 30.3, 172, 104, 241, 153, 256, 176, 501, 360};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_w_"+S2_start + "-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_N_sum);
	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npeaks_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npeaks_"+cuts+"_w_"+S2_start + "-160us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, d_S2_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S2_start, 160);
	draw_limits(164, 250);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_S1_start, d_S1_finish);
	unset_draw_limits();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_w_"+S1_start+"-"+S1_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_bkg1_start, d_bkg1_finish);
	unset_draw_limits();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_w_"+bkg1_start+"-"+bkg1_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(d_bkg2_start, d_bkg2_finish);
	unset_draw_limits();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_w_"+bkg2_start+"-"+bkg2_finish+"us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_N_sum);
	time_zoom_SiPMs(d_bkg2_start, d_bkg2_finish);
	unset_draw_limits();
	saveaspng(FOLDER + Num+"_SiPMs_Npeaks_"+cuts+"_w_"+bkg2_start+"-"+bkg2_finish+"us");
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	unset_draw_limits();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_w_"+S2_start+"-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S1_start, d_S1_finish);
	ch(1);
	saveaspng(FOLDER + Num+"_slowPMT1_Npe_"+cuts+"_w_"+S1_start + "-"+S1_finish+"us");
	ch(2);
	saveaspng(FOLDER + Num+"_slowPMT2_Npe_"+cuts+"_w_"+S1_start + "-"+S1_finish+"us");
	ch(3);
	saveaspng(FOLDER + Num+"_slowPMT3_Npe_"+cuts+"_w_"+S1_start + "-"+S1_finish+"us");
	ch(4);
	saveaspng(FOLDER + Num+"_slowPMT4_Npe_"+cuts+"_w_"+S1_start + "-"+S1_finish+"us");
	on_ch(1); on_ch(2); on_ch(3); on_ch(4); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_w_"+S1_start + "-"+S1_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_bkg1_start, d_bkg1_finish);
	ch(1);
	saveaspng(FOLDER + Num+"_slowPMT1_Npe_"+cuts+"_w_"+bkg1_start + "-"+bkg1_finish+"us");
	ch(2);
	saveaspng(FOLDER + Num+"_slowPMT2_Npe_"+cuts+"_w_"+bkg1_start + "-"+bkg1_finish+"us");
	ch(3);
	saveaspng(FOLDER + Num+"_slowPMT3_Npe_"+cuts+"_w_"+bkg1_start + "-"+bkg1_finish+"us");
	ch(4);
	saveaspng(FOLDER + Num+"_slowPMT4_Npe_"+cuts+"_w_"+bkg1_start + "-"+bkg1_finish+"us");
	on_ch(1); on_ch(2); on_ch(3); on_ch(4); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_w_"+bkg1_start + "-"+bkg1_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_bkg2_start, d_bkg2_finish);
	ch(1);
	saveaspng(FOLDER + Num+"_slowPMT1_Npe_"+cuts+"_w_"+bkg2_start + "-"+bkg2_finish+"us");
	ch(2);
	saveaspng(FOLDER + Num+"_slowPMT2_Npe_"+cuts+"_w_"+bkg2_start + "-"+bkg2_finish+"us");
	ch(3);
	saveaspng(FOLDER + Num+"_slowPMT3_Npe_"+cuts+"_w_"+bkg2_start + "-"+bkg2_finish+"us");
	ch(4);
	saveaspng(FOLDER + Num+"_slowPMT4_Npe_"+cuts+"_w_"+bkg2_start + "-"+bkg2_finish+"us");
	on_ch(1); on_ch(2); on_ch(3); on_ch(4); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_w_"+bkg2_start + "-"+bkg2_finish+"us");
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ch(1);
	saveaspng(FOLDER + Num+"_slowPMT1_Npe_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	ch(2);
	saveaspng(FOLDER + Num+"_slowPMT2_Npe_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	ch(3);
	saveaspng(FOLDER + Num+"_slowPMT3_Npe_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	ch(4);
	saveaspng(FOLDER + Num+"_slowPMT4_Npe_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	on_ch(1); on_ch(2); on_ch(3); on_ch(4); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_w_"+S2_start + "-"+S2_finish+"us");
	Num = int_to_str(++no, 2);

}
if (exp == "191107_U_18kV_850V_46V_12dB") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 90);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::PMT_trigger_fit); //May have poor results because of merged and >1phe peaks
		off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpeaks);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.1);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_4PMT_fast_by_S.dat");
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	x_y_regions = {40, 0, 58.3, 11.3, 63.4, 19.7, 136, 79.6, 182.6, 109.7, 221.0, 150.7, 501, 368};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


	if (iteration > 0) {
	ty(AStates::PMT_trigger_fit);
		draw_limits(-3, 2);
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		set_as_run_cut("good_trigger"); cuts += "+" + Num;
		set_trigger_offsets(0.0);
		print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
		Num = int_to_str(++no, 2);

		//TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		//saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		//Num = int_to_str(++no, 2);
	}

ty(AStates::MPPC_Npe_sum);
	draw_limits(0, 87);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(128, 159);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_left/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(128, 194);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(232, 338);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npeaks", 800, 1);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_S", 800, 3);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 800, 5);//For next iteration trigger fit
}
if (exp == "191107_U_16kV_850V_46V_12dB") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 350);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 64);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::PMT_trigger_fit); //May have poor results because of merged and >1phe peaks
		off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpeaks);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.1);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_4PMT_fast_by_S.dat");
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
	x_y_regions = {27, 0, 40.9, 2.8, 41.7, 11.4, 89.5, 50.7, 126.1, 77.0, 157.8, 108.3, 351, 257};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


	if (iteration > 0) {
	ty(AStates::PMT_trigger_fit);
		draw_limits(-3, 2);
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		set_as_run_cut("good_trigger"); cuts += "+" + Num;
		set_trigger_offsets(0.0);
		print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
		Num = int_to_str(++no, 2);

		//TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		//saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		//Num = int_to_str(++no, 2);
	}

ty(AStates::MPPC_Npe_sum);
	draw_limits(0, 64);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(86, 114);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_left/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(86, 143);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(167, 250);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npeaks", 800, 1);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_S", 800, 3);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 800, 5);//For next iteration trigger fit
}
if (exp == "191107_U_14kV_850V_46V_12dB") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 44);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::PMT_trigger_fit); //May have poor results because of merged and >1phe peaks
		off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpeaks);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.1);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_4PMT_fast_by_S.dat");
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	x_y_regions = {22.6, 0, 27.7, 8.0, 103.7, 64.5, 120.2, 87.9, 249, 188};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


	if (iteration > 0) {
	ty(AStates::PMT_trigger_fit);
		draw_limits(-3, 2);
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		set_as_run_cut("good_trigger"); cuts += "+" + Num;
		set_trigger_offsets(0.0);
		print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
		Num = int_to_str(++no, 2);

		//TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		//saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		//Num = int_to_str(++no, 2);
	}

ty(AStates::MPPC_Npe_sum);
	draw_limits(0, 42);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(61, 82);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_left/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(61, 103);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(120, 190);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npeaks", 800, 1);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_S", 800, 3);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 800, 5);//For next iteration trigger fit
}
if (exp == "191107_U_12kV_850V_46V_12dB") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 27);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::PMT_trigger_fit); //May have poor results because of merged and >1phe peaks
		off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpeaks);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.1);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_4PMT_fast_by_S.dat");
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {12, 0, 25.8, 12.7, 67.4, 45.1, 78.1, 60.4, 199, 165};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


	if (iteration > 0) {
	ty(AStates::PMT_trigger_fit);
		draw_limits(-3, 2);
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		set_as_run_cut("good_trigger"); cuts += "+" + Num;
		set_trigger_offsets(0.0);
		print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
		Num = int_to_str(++no, 2);

		//TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		//saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		//Num = int_to_str(++no, 2);
	}

ty(AStates::MPPC_Npe_sum);
	draw_limits(0, 25);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(37, 54);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_left/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(37, 71);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(88, 135);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npeaks", 800, 1);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_S", 800, 3);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 800, 5);//For next iteration trigger fit
}
if (exp == "191107_U_10kV_850V_46V_12dB") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 17);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::PMT_trigger_fit); //May have poor results because of merged and >1phe peaks
		off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpeaks);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.1);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_4PMT_fast_by_S.dat");
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	cut_x_y_lower(0, 10, 4, 10, false, "2");
	x_y_regions = {5.4, 0, 14.6, 8.3, 41.7, 27.4, 60.2, 49.3, 149, 132};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	remcut(-1, "2");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


	if (iteration > 0) {
	ty(AStates::PMT_trigger_fit);
		draw_limits(-3, 2);
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		set_as_run_cut("good_trigger"); cuts += "+" + Num;
		set_trigger_offsets(0.0);
		print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
		Num = int_to_str(++no, 2);

		//TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		//saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		//Num = int_to_str(++no, 2);
	}

ty(AStates::MPPC_Npe_sum);
	draw_limits(0, 14);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(20, 33);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_left/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(20, 46);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(60, 90);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npeaks", 800, 1);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_S", 800, 3);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 800, 5);//For next iteration trigger fit
}
if (exp == "191107_U_8kV_850V_46V_12dB") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 12);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::PMT_trigger_fit); //May have poor results because of merged and >1phe peaks
		off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpeaks);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.1);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_4PMT_fast_by_S.dat");
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	cut_x_y_lower(0, 10, 2, 10, false, "2");
	x_y_regions = {3.1, 0, 8.8, 5.0, 22.7, 13.4, 40.5, 34.5, 99, 87};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	remcut(-1, "2");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


	if (iteration > 0) {
	ty(AStates::PMT_trigger_fit);
		draw_limits(-3, 2);
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		set_as_run_cut("good_trigger"); cuts += "+" + Num;
		set_trigger_offsets(0.0);
		print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
		Num = int_to_str(++no, 2);

		//TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		//saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		//Num = int_to_str(++no, 2);
	}

ty(AStates::MPPC_Npe_sum);
	draw_limits(0, 9);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(10, 19);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_left/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(10, 30);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(38, 60);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npeaks", 800, 1);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_S", 800, 3);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 800, 5);//For next iteration trigger fit
}
if (exp == "191107_U_6kV_850V_46V_12dB") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//remove events with large Npe before S2 signal (pre-trigger, S1 is very small)
	time_zoom_SiPMs(0, d_S2_start);
	draw_limits(0, 20);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger_SiPMs");	cuts = "cuts_"+Num;
	time_zoom_SiPMs(d_S2_start, 160);
	unset_draw_limits();

	if (iteration > 0) {
		std::string PREV_FOLDER = data_output_path + folder + "/iteration_" + int_to_str((std::size_t)(iteration - 1), 2) + "/";
		ty(AStates::PMT_trigger_fit); //May have poor results because of merged and >1phe peaks
		off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
		noise_cut(5, 0, PMT_state, 0);
		noise_cut(6, 0, PMT_state, 0);
		noise_cut(7, 0, PMT_state, 0);
		noise_cut(8, 0, PMT_state, 0);
		set_zoom(-20, 20);
		set_bins(300);
		TriggerFitData::SetTriggerType(TriggerFitData::tbNpeaks);
		TriggerFitData::SetTriggerFirstScant(1.6);
		TriggerFitData::SetTriggerPrecision(0.1);
		TriggerFitData::SetPulseShape(PREV_FOLDER + "A_4PMT_fast_by_S.dat");
		saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);

		TriggerFitData::SetTriggerType(TriggerFitData::tbNpe);
		update();
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		Num = int_to_str(++no, 2);
	}

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	cut_x_y_lower(0, 10, 2, 10, false, "2");
	x_y_regions = {3.1, 0, 4.7, 2.1, 13.1, 4.2, 31.4, 23.6, 99, 76};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	remcut(-1, "2");
	set_as_run_cut("good_SiPMs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


	if (iteration > 0) {
	ty(AStates::PMT_trigger_fit);
		draw_limits(-3, 2);
		saveaspng(FOLDER + Num+"_fastPMTs_Npe_trigger_"+cuts+"_zoom");
		set_as_run_cut("good_trigger"); cuts += "+" + Num;
		set_trigger_offsets(0.0);
		print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
		Num = int_to_str(++no, 2);

		//TriggerFitData::SetTriggerType(TriggerFitData::tbS);
		//saveaspng(FOLDER + Num+"_fastPMTs_S_trigger_"+cuts+"_zoom");
		//Num = int_to_str(++no, 2);
	}

ty(AStates::MPPC_Npe_sum);
	draw_limits(0, 4);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(6, 11);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_left/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(6, 19);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Alpha_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Alpha_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::MPPC_Npe_sum);
	draw_limits(25, 35);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_Npeaks", 800, 1);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_4PMT_fast_by_S", 800, 3);//For next iteration trigger fit
	save_forms(FOLDER + "forms_Alpha_peak/", FOLDER + "A_SiPMs_by_Npe", 800, 5);//For next iteration trigger fit
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

  bool subtact_baseline = true;
  bool renormalize = true;
  double time_pretrigger_left = 5, time_pretrigger_right = 20;
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
    double integral = integrate(hist, time_pretrigger_left, time_pretrigger_right);
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
  frame->GetXaxis()->SetRangeUser(85, 130);
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
