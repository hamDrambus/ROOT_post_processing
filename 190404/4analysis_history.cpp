void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void central_SiPMs(bool on);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);

void time_zoom_fPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 8);
	cut_t(f_time, to_time, false, 9);
	cut_t(f_time, to_time, false, 10);
	cut_t(f_time, to_time, false, 11);
}

void time_zoom_sPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 12);
	cut_t(f_time, to_time, false, 13);
	cut_t(f_time, to_time, false, 14);
	cut_t(f_time, to_time, false, 15);
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
	ch(8);
	noise_cut(8, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "12_form_by_Npe");
	ch(13);
	noise_cut(13, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "13_form_by_Npe");
	ch(14);
	noise_cut(14, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "14_form_by_Npe");
	ch(15);
	noise_cut(15, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "15_form_by_Npe");

	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(8);
		noise_cut(8, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "8_form_by_S");
		ch(9);
		noise_cut(9, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "9_form_by_S");
		ch(10);
		noise_cut(10, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "10_form_by_S");
		ch(11);
		noise_cut(11, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "11_form_by_S");
		ch(12);
		noise_cut(12, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "12_form_by_S");
		ch(13);
		noise_cut(13, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "13_form_by_S");
		ch(14);
		noise_cut(14, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "14_form_by_S");
		ch(15);
		noise_cut(15, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r);
		set_bins(Nbins);
		saveas(path + "15_form_by_S");
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
if (channel==15) {
	cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.0006, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.00824, 0.0395, 0.0825, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }  
}
if (channel==14) {
	cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.0006, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.0363, 0.0690, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }
}
if (channel==13) {
	cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.0006, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.035, 0.0680, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }
}
if (channel==12) {
    cut_A_S_upper(0.013, 0.0073, 0.026, 0.0089, display, channel, "rem_smallA_largeS");
	cut_A_S_lower(0.0165, 0.00082, 0.162, 0.0127, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.162, 0.0127, 1.6, 0.20, display, channel, "rem_largeA_smallS");
	cut_A_S_left(0.0, 0.025, 0.0540, 0.0, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 5.0, 1.6, 5.5, display, channel, "rem_S>~5.0");
    }
}
if (channel==11) {
	cut_A_S_upper(0.00362, 0.000112, 0.009, 0.000238, display, channel, "rem_smallA_largeS");
    if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (channel==10) {
	cut_A_S_left(0.00505, 0.0, 0.00505, 1e5, display, channel, "rem_smallA");
    if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (channel==9) {
	cut_A_S_left(0.0045, 0.00012, 0.0045, 1e5, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (channel==8) {
    cut_A_S_left(0.0045, 0, 0.0045, 1e5, display, channel, "rem_smallA");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
//The following are set by hand for 20kV individually, there is no other way.
if (channel==32) {
    cut_A_S_lower(0.011, 0.0011, 0.023, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.00065, 0.06, 0.00535, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0250, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0250, 0.00307, display, channel, "2pe_merged");
}
if (channel==33) {
	cut_A_S_lower(0.008, 0.00115, 0.0175, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.008, 0.008, 0.000528, 0.06, 0.00538, 1e3};
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
		cut_A_S_upper(0.0280, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.0280, 0.0036, display, channel, "2pe_merged");
}
if (channel==35) {
	cut_A_S_lower(0.0115, 0.0013, 0.0287, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.000740, 0.06, 0.0057, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04"); 
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.027, 0.004, display, channel, "2pe_merged");
}
if (channel==36) {
	cut_A_S_lower(0.0115, 0.00130, 0.027, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.000555, 0.06, 0.00623, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0019, 0.026, 0.004, display, channel, "2pe_merged");
}
if (channel==37) {
	cut_A_S_lower(0.0095, 0.00111, 0.030, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0095, 0.0095, 0.00058, 0.06, 0.00583, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0224, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0095, 0.00151, 0.0224, 0.00342, display, channel, "2pe_merged");
}
if (channel==38) {
	cut_A_S_lower(0.012, 0.00130, 0.03, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000687, 0.06, 0.00591, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00185, 0.0285, 0.0042, display, channel, "2pe_merged");
}
if (channel==39) {
	cut_A_S_lower(0.011, 0.00130, 0.032, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.000638, 0.06, 0.00617, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0247, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0247, 0.00376, display, channel, "2pe_merged");
}
if (channel==40) {
	cut_A_S_lower(0.0117, 0.00119, 0.03, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0117, 0.0117, 0.000744, 0.06, 0.00572, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0263, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0117, 0.00178, 0.0263, 0.00364, display, channel, "2pe_merged");
}
if (channel==41) {
	cut_A_S_lower(0.012, 0.00119, 0.024, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000638, 0.06, 0.00604, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00180, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==42) {
	cut_A_S_lower(0.0115, 0.00125, 0.024, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.00077, 0.06, 0.00558, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04"); 
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.00167, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel == 43 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==44) {
	cut_A_S_lower(0.01, 0.00112, 0.028, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, 0.000640, 0.06, 0.00577, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04"); 
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0225, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00157, 0.0225, 0.0030, display, channel, "2pe_merged");
}
if (channel==48) {
	cut_A_S_lower(0.012, 0.00125, 0.02, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000824, 0.06, 0.00595, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0270, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0016, 0.0270, 0.00375, display, channel, "2pe_merged");
}
if (channel==49) {
	cut_A_S_lower(0.0089, 0.0010, 0.026, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0089, 0.0089, 0.000611, 0.06, 0.00582, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0089, 0.00139, 0.02, 0.0028, display, channel, "2pe_merged");
}
if (channel==50) {
	cut_A_S_lower(0.0085, 0.00101, 0.022, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0085, 0.0085, 0.000425, 0.06, 0.006, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0196, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0085, 0.00149, 0.0196, 0.00279, display, channel, "2pe_merged");
}
if (channel==51) {
	cut_A_S_lower(0.0109, 0.00122, 0.036, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0109, 0.0109, 0.000691, 0.06, 0.00585, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0248, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0109, 0.00167, 0.0248, 0.00348, display, channel, "2pe_merged");
}
if (channel==52) {
	cut_A_S_lower(0.0115, 0.00127, 0.04, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.000665, 0.06, 0.00606, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0263, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0018, 0.0263, 0.0039, display, channel, "2pe_merged");
}
if (channel==53) {
	cut_A_S_lower(0.012, 0.00146, 0.028, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.000691, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00186, 0.028, 0.00436, display, channel, "2pe_merged");
}
if (channel==54) {
	cut_A_S_lower(0.01, 0.0012, 0.030, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, 0.000638, 0.06, 0.00587, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00157, 0.023, 0.00325, display, channel, "2pe_merged");
}
if (channel==55) {
	cut_A_S_lower(0.0107, 0.00117, 0.026, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0107, 0.0107, 0.000638, 0.06, 0.00625, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0235, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0107, 0.00165, 0.0235, 0.0034, display, channel, "2pe_merged");
}
if (channel==56) {
	cut_A_S_lower(0.0118, 0.00138, 0.023, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0118, 0.0118, 0.000851, 0.06, 0.0060, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0118, 0.00165, 0.0255, 0.0038, display, channel, "2pe_merged");
}
if (channel==57) {
	cut_A_S_lower(0.012, 0.00127, 0.040, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.00079, 0.06, 0.00596, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00162, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==58) {
	cut_A_S_lower(0.0113, 0.00133, 0.025, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0113, 0.0113, 0.000664, 0.06, 0.0062, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0246, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0113, 0.00162, 0.0246, 0.00367, display, channel, "2pe_merged");
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
//Created on 23.12.2019
//method = 0 - use fast PMTs for trigger_v2 (byNpe for large fields/byNpeaks for low) with
//optimal dt determined as
//dt_trigger_optimal[us] = 0.3923 + 0.9728*Tdrift(18mm)[us] (for previous analysis)
//method = 1 - same as 0 but not using trigger adjustment (raw signals, but with same event selection as 0)
data_output_path = "190404/results_v4/";
calibration_file = data_output_path + "190404_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2;
if (method==1)
	data_output_path = "190404/results_v1/";
if (method>1) {
	std::cout<<"There is no method > 1, quitting"<<std::endl;
	return;
}
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190404_Cd_20kV_850V_46V_th250mV_0"] = std::pair<double, double> (25, 40);
S2_times["190404_Cd_20kV_850V_46V_th250mV"] = std::pair<double, double> (25, 40);
S2_times["190404_Cd_18kV_850V_46V_th230mV"] = std::pair<double, double> (25, 40);
S2_times["190404_Cd_16kV_850V_46V_th210mV"] = std::pair<double, double> (25, 40);
S2_times["190404_Cd_14kV_850V_46V_th200mV"] = std::pair<double, double> (25, 40);
S2_times["190404_Cd_12kV_850V_46V_th160mV"] = std::pair<double, double> (25, 40);
S2_times["190404_Cd_10kV_850V_46V_th150mV"] = std::pair<double, double> (25, 40);
S2_times["190404_Cd_8kV_850V_46V_th140mV"] = std::pair<double, double> (22, 40); //!!!22, 40

std::map<std::string, std::string> exp_folders;
exp_folders["190404_Cd_20kV_850V_46V_th250mV_0"] = "Cd_46V_20kV_850V_0";
exp_folders["190404_Cd_20kV_850V_46V_th250mV"] = "Cd_46V_20kV_850V";
exp_folders["190404_Cd_18kV_850V_46V_th230mV"] = "Cd_46V_18kV_850V";
exp_folders["190404_Cd_16kV_850V_46V_th210mV"] = "Cd_46V_16kV_850V";
exp_folders["190404_Cd_14kV_850V_46V_th200mV"] = "Cd_46V_14kV_850V";
exp_folders["190404_Cd_12kV_850V_46V_th160mV"] = "Cd_46V_12kV_850V";
exp_folders["190404_Cd_10kV_850V_46V_th150mV"] = "Cd_46V_10kV_850V";
exp_folders["190404_Cd_8kV_850V_46V_th140mV"] = "Cd_46V_8kV_850V";

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
    set_bins(500);
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
if (channel==15) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.6, 0, 6);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.5, 0, 0.1);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	cut_A_S_left(0.0, 0.023, 0.044, 0.0, false, channel, "temp"); //Removes small noise for better display 
	set_bins(100);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.06);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==14) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.6, 0, 6);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.5, 0, 0.1);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	cut_A_S_left(0.0, 0.02, 0.04, 0.0, false, channel, "temp"); //Removes small noise for better display 
	set_bins(100);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.06);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==13) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.6, 0, 6);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.5, 0, 0.1);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	cut_A_S_left(0.0, 0.022, 0.04, 0.0, false, channel, "temp"); //Removes small noise for better display 
	set_bins(120);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.05);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==12) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 1.6, 0, 6);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.5, 0, 0.1);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	cut_A_S_left(0.0, 0.018, 0.032, 0.0, false, channel, "temp"); //Removes small noise for better display 
	set_bins(100);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_cuts");
	cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.05);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==11) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.6, 0, 1.0);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.1, 0, 0.0025);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.006);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.0015);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==10) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.6, 0, 1.0);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.1, 0, 0.0025);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.006);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.0015);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==9) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.6, 0, 1.0);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.1, 0, 0.0025);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.006);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.0015);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
	ch(channel);
	set_zoom(0, 0.6, 0, 1.0);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.1, 0, 0.0025);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
	set_bins(120);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
	set_zoom(0, 0.006);
	set_bins(800);
	saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 0, PMT_state, false);
	cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.0015);
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

if ((exp == "190404_Cd_20kV_850V_46V_th250mV" || exp == "190404_Cd_20kV_850V_46V_th250mV_0")) {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	cut_x_y_right(13, 0, 28.3, 9.9, true, "1");
	cut_x_y_right(28.3, 9.9, 50.7, 19.2, true, "2");
	cut_x_y_right(50.7, 19.2, 79.3, 45.7, true, "3");
	cut_x_y_right(79.3, 45.7, 250, 164, true, "4");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, 90);
	ty(AStates::Correlation);
	cut_x_y_right(11.7, 0, 30.0, 16, true, "1");
	cut_x_y_right(30.0, 16, 51, 31.6, true, "2");
	cut_x_y_right(51, 31.6, 84.2, 68.9, true, "3");
	cut_x_y_right(84.2, 68.9, 250, 212, true, "4");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-90us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(33, 0, 33, 160, true, "1");
	cut_x_y_upper(0, 33, 160, 33, true, "2");
	cut_x_y_left(24.13, 0, 24.13, 160, true, "3");
	cut_x_y_lower(0, 24.04, 160, 24.04, true, "4");
	cut_x_y_right(24.58, 22.32, 40.38, 37.97, true, "5");
	cut_x_y_left(22.20, 24.24, 38.54, 41.69, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe" + DT);
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
	draw_limits(0, 24);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(25, 44);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(25, 67);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(80, 130);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_18kV_850V_46V_th230mV") {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	cut_x_y_right(13, 0, 20.4, 7.4, true, "1");
	cut_x_y_right(20.4, 7.4, 40.8, 15.9, true, "2");
	cut_x_y_right(40.8, 15.9, 63.4, 39, true, "3");
	cut_x_y_right(63.4, 39, 200, 138, true, "4");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, 90);
	ty(AStates::Correlation);
	cut_x_y_right(10.7, 0, 22.3, 12, true, "1");
	cut_x_y_right(22.3, 12, 35.1, 20.6, true, "2");
	cut_x_y_right(35.1, 20.6, 72.9, 60, true, "3");
	cut_x_y_right(72.9, 60, 200, 175, true, "4");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-90us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(33, 0, 33, 160, true, "1");
	cut_x_y_upper(0, 33, 160, 33, true, "2");
	cut_x_y_left(25.38, 0, 25.38, 160, true, "3");
	cut_x_y_lower(0, 25.38, 160, 25.38, true, "4");
	cut_x_y_right(24.49, 23.11, 40.20, 38.77, true, "5");
	cut_x_y_left(22.83, 24.17, 39.08, 41.49, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe" + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
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
	draw_limits(8, 250);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 16);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(17, 32);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(17, 51);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(65, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_16kV_850V_46V_th210mV") {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	cut_x_y_right(11, 0, 17.9, 7.3, true, "1");
	cut_x_y_right(17.9, 7.3, 30.6, 15.7, true, "2");
	cut_x_y_right(30.6, 15.7, 50.9, 35.0, true, "3");
	cut_x_y_right(50.9, 35.0, 150, 111, true, "4");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, 90);
	ty(AStates::Correlation);
	cut_x_y_right(6.9, 0, 29.8, 19.6, true, "1");
	cut_x_y_right(29.8, 19.6, 52.6, 44.5, true, "2");
	cut_x_y_right(52.6, 44.5, 150, 131, true, "3");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-90us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(32, 0, 32, 160, true, "1");
	cut_x_y_upper(0, 32, 160, 32, true, "2");
	cut_x_y_left(25.38, 0, 25.38, 160, true, "3");
	cut_x_y_lower(0, 25.38, 160, 25.38, true, "4");
	cut_x_y_right(24.49, 23.11, 40.20, 38.77, true, "5");
	cut_x_y_left(22.83, 24.17, 39.08, 41.49, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe" + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
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
	draw_limits(8, 250);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 13);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(14, 24);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(14, 39);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(50, 100);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_14kV_850V_46V_th200mV") {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 120, 0, 120);
	set_bins(120);
	cut_x_y_right(7.4, 0, 13.1, 5.8, true, "1");
	cut_x_y_right(13.1, 5.8, 22.8, 12.3, true, "2");
	cut_x_y_right(22.8, 12.3, 41.8, 31.7, true, "3");
	cut_x_y_right(41.8, 31.7, 120, 100, true, "4");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, 100);
	ty(AStates::Correlation);
	cut_x_y_right(5, 0, 24.8, 17.8, true, "1");
	cut_x_y_right(24.8, 17.8, 47.7, 42.2, true, "2");
	cut_x_y_right(47.7, 42.2, 120, 111, true, "3");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-100us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(32, 0, 32, 160, true, "1");
	cut_x_y_upper(0, 32, 160, 32, true, "2");
	cut_x_y_left(24.66, 0, 24.66, 160, true, "3");
	cut_x_y_lower(0, 24.66, 160, 24.66, true, "4");
	cut_x_y_right(22.83, 22.25, 38.36, 37.97, true, "5");
	cut_x_y_left(22.43, 24.04, 38.59, 41.29, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe" + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
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
	draw_limits(7, 250);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 10);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(11, 18);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(11, 31);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(40, 80);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_12kV_850V_46V_th160mV") {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	cut_x_y_right(5.4, 0, 11.0, 5.3, true, "1");
	cut_x_y_right(11.0, 5.3, 18.8, 12.3, true, "2");
	cut_x_y_right(18.8, 12.3, 28.0, 22.2, true, "3");
	cut_x_y_right(28.0, 22.2, 100, 90, true, "4");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, 110);
	ty(AStates::Correlation);
	cut_x_y_right(4.14, 0, 100, 95, true, "1");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-110us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(31, 0, 31, 160, true, "1");
	cut_x_y_upper(0, 31, 160, 31, true, "2");
	cut_x_y_left(24.10, 0, 24.10, 160, true, "3");
	cut_x_y_lower(0, 24.10, 160, 24.10, true, "4");
	cut_x_y_right(22.83, 22.25, 38.36, 37.97, true, "5");
	cut_x_y_left(22.43, 24.04, 38.59, 41.29, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe" + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
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
	draw_limits(6, 250);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(8, 13);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(8, 23);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(30, 60);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_10kV_850V_46V_th150mV") {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	cut_x_y_right(3.7, 0, 100, 93.6, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(31, 0, 31, 160, true, "1");
	cut_x_y_upper(0, 31, 160, 31, true, "2");
	cut_x_y_left(23.70, 0, 23.70, 160, true, "3");
	cut_x_y_lower(0, 23.70, 160, 23.70, true, "4");
	cut_x_y_right(22.83, 22.25, 38.36, 37.97, true, "5");
	cut_x_y_left(21.26, 23.04, 37.42, 40.30, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe" + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
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
	draw_limits(5, 250);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(6, 9);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(6, 17);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(26, 50);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_8kV_850V_46V_th140mV") {
	double dt_shaping = 0.3923 + 0.9728*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, PMT_state, 0);
	noise_cut(13, 0, PMT_state, 0);
	noise_cut(14, 0, PMT_state, 0);
	noise_cut(15, 0, PMT_state, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged and >1phe peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because fast PMTs have bad calibration precision
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 100, 0, 100);
	set_bins(100);
	cut_x_y_right(3.7, 0, 100, 93.6, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(31, 0, 31, 160, true, "1");
	cut_x_y_upper(0, 31, 160, 31, true, "2");
	cut_x_y_left(23.70, 0, 23.70, 160, true, "3");
	cut_x_y_lower(0, 23.70, 160, 23.70, true, "4");
	cut_x_y_right(22.83, 22.25, 38.36, 37.97, true, "5");
	cut_x_y_left(21.26, 23.04, 37.42, 40.30, true, "6");
	update();
	set_titles("t by fastPMTs by N peaks " + DT, "t by fastPMTs by Npe" + DT);
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_Npeaks_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
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
	draw_limits(4, 250);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts + "_" + S2_start + "-" + S2_finish + "us");
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(5, 7);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(5, 14);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(20, 40);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/events.txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}
//END OF FORMS
} 
