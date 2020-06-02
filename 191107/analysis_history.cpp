void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void central_SiPMs(bool on);
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
	ty(AStates::MPPC_tbN_sum);
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

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 14.01.2020
//method = 0 - use fast PMTs for trigger_v2 (byNpe for large fields/byNpeaks for low) with
//optimal dt determined as
//dt_trigger_optimal[us] = 0.3923 + 0.9728*Tdrift(18mm)[us] (for previous analysis)
//method = 1 - same as 0 but not using trigger adjustment (raw signals, but with same event selection as 0)
data_output_path = "191107/results_v4/";
calibration_file = data_output_path + "191107_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2;
if (method==1)
	data_output_path = "191107/results_v1/";
if (method>1) {
	std::cout<<"There is no method > 1, quitting"<<std::endl;
	return;
}
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["191107_U_20kV_850V_46V_12dB"] = std::pair<double, double> (88, 110);
S2_times["191107_U_18kV_850V_46V_12dB"] = std::pair<double, double> (88, 107);
S2_times["191107_U_16kV_850V_46V_12dB"] = std::pair<double, double> (88, 105);
S2_times["191107_U_14kV_850V_46V_12dB"] = std::pair<double, double> (88, 103);
S2_times["191107_U_12kV_850V_46V_12dB"] = std::pair<double, double> (88, 103);
S2_times["191107_U_10kV_850V_46V_12dB"] = std::pair<double, double> (88, 100);
S2_times["191107_U_8kV_850V_46V_12dB"] = std::pair<double, double> (86, 98);
S2_times["191107_U_6kV_850V_46V_12dB"] = std::pair<double, double> (86, 98);

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
    noise_cut(channel, 2, SiPM_state, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss");    
    noise_cut(channel, 2, SiPM_state, false);
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
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 90;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//remove events with large Npe before S2 signal
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 200);
	draw_limits(0, 74);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	time_zoom_sPMTs(d_S2_start, 160);
	unset_draw_limits();
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
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(80, 100);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(80, 100);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(80, 100);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(41, 0, 69.7, 26.8, true, "1");
	cut_x_y_right(69.7, 26.8, 102.0, 46.8, true, "2");
	cut_x_y_right(102.0, 46.8, 135, 77, true, "3");
	cut_x_y_right(135, 77, 300, 203, true, "4");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(85, 100, 85, 100);
	set_bins(600);
	cut_x_y_right(94.0, 0, 94.0, 160, true, "1");
	cut_x_y_upper(0, 94.0, 160, 94.0, true, "2");
	cut_x_y_left(90.32, 0, 90.32, 160, true, "3");
	cut_x_y_lower(0, 90.52, 160, 90.52, true, "4");
	cut_x_y_right(86.32, 85.63, 96.85, 96.18, true, "5");
	cut_x_y_left(86.03, 86.55, 96.54, 97.34, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe " + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at);
		cuts += "+" + Num;
	}
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_sum_N);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	draw_limits(10, 250);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 60);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(61, 90);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_left/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_Cd_left/", false, PMT_state, SiPM_state);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(61, 117);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(130, 200);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "191107_U_8kV_850V_46V_12dB") {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 90;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//remove events with large Npe before S2 signal
	ty(AStates::PMT_Npe_sum);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	draw_limits(0, 40);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_0-"+S2_start+"us");
	set_as_run_cut("good_pre_trigger");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	time_zoom_sPMTs(d_S2_start, 160);
	unset_draw_limits();
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

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(80, 100);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(80, 100);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(80, 100);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	cut_x_y_right(6.84, 0, 100, 86.7, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(85, 100, 85, 100);
	set_bins(600);
	cut_x_y_right(94.46, 0, 94.46, 160, true, "1");
	cut_x_y_upper(0, 94.46, 160, 94.46, true, "2");
	cut_x_y_left(90.0, 0, 90.0, 160, true, "3");
	cut_x_y_lower(0, 90.0, 160, 90.0, true, "4");
	cut_x_y_right(86.32, 85.63, 96.85, 96.18, true, "5");
	cut_x_y_left(86.03, 86.66, 96.54, 97.34, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe " + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at);
		cuts += "+" + Num;
	}
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_sum_N);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	draw_limits(5, 100);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 8);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(9, 12);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_left/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_Cd_left/", false, PMT_state, SiPM_state);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(9, 20);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(22, 40);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);
save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

//END OF FORMS
} 
