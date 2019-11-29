void noise_cut(int channel, int aggressiveness, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void central_SiPMs(bool on);
void save_forms (std::string path, bool N_only);

void time_zoom_fPMTs (double f_time, double to_time) {
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 8);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 9);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 10);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 11);
}

void time_zoom_sPMTs (double f_time, double to_time) {
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 12);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 13);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 14);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 15);
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

void save_forms (std::string path, bool N_only)
{
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(path + "SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(path + "SiPMs_edge_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "12_form_by_Npe");
	ch(13);
	noise_cut(13, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "13_form_by_Npe");
	ch(14);
	noise_cut(14, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "14_form_by_Npe");
	ch(15);
	noise_cut(15, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "15_form_by_Npe");

	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(8);
		noise_cut(8, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "8_form_by_S");
		ch(9);
		noise_cut(9, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "9_form_by_S");
		ch(10);
		noise_cut(10, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "10_form_by_S");
		ch(11);
		noise_cut(11, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "11_form_by_S");
		ch(12);
		noise_cut(12, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "12_form_by_S");
		ch(13);
		noise_cut(13, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "13_form_by_S");
		ch(14);
		noise_cut(14, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "14_form_by_S");
		ch(15);
		noise_cut(15, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "15_form_by_S");
	}
}

//Even it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
if (channel==15) {
	if (aggressiveness == 1) {
        cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
		cut_A_S_upper(0.0, 0.4, 0.9, 0.5, display, channel, "rem_S>~0.4");
    }    
}
if (channel==14) {
	if (aggressiveness == 1) {
        cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
		cut_A_S_upper(0.0, 0.4, 1.0, 0.55, display, channel, "rem_S>~0.4");
    }
}
if (channel==13) {
	if (aggressiveness == 1) {
        cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.4, 1.0, 0.55, display, channel, "rem_S>~0.4");
    }
}
if (channel==12) {
    x_y_regions = {-1, 0.025, 0, 0.45, 0.057, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, 12, "large_A_noise");
    x_y_regions = {-1, 0, 0.02, 0.054, 0.0, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, 12, "small_S_noise");
	if (aggressiveness == 1) {
        cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.4, 1.0, 0.55, display, channel, "rem_S>~0.4");
    }
}
if (channel==11) {
    x_y_regions = {-1, 0.006, 0.000045, 0.4, 0.0062, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, 11, "large_A_noise");
    cut_A_S_upper(0.005, 0.00018, 0.6, 0.019, display, 11);
}
if (channel==10) {
    x_y_regions = {-1, 0.006, 0.000045, 0.4, 0.0065, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, 10, "large_A_noise");
    cut_A_S_upper(0.003, 0.000118, 0.6, 0.0195, display, 10);
}
if (channel==9) {
    x_y_regions = {-1, 0.0042, 0, 0.4, 0.0065, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, 9, "large_A_noise");
    cut_A_S_upper(0.003, 0.00013, 0.6, 0.021, display, 9);
}
if (channel==8) {
    x_y_regions = {-1, 0.003, 0.000008, 0.4, 0.0069, 1e3};
    cut_A_S_fast_PMT(x_y_regions, display, 8, "large_A_noise");
    cut_A_S_upper(0.003, 0.00012, 0.6, 0.021, display, 8);
}
//The following are set by hand for 20kV individually, there is no other way.
if (channel==32) {
    x_y_regions = {0.011, 0.011, 0.0011, 0.023, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.00065, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0255, 0.003, display, channel, "2pe_merged");
}
if (channel==33) {
	x_y_regions = {0.008, 0.008, 0.00115, 0.0175, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.008, 0.008, 0.00042, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0215, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.008, 0.0013, 0.0215, 0.0031, display, channel, "2pe_merged");
}
if (channel==34) {
	x_y_regions = {0.0115, 0.0115, 0.0013, 0.024, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.0285, 0.0037, display, channel, "2pe_merged");
}
if (channel==35) {
	x_y_regions = {0.0115, 0.0115, 0.0011, 0.024, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04"); 
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.027, 0.004, display, channel, "2pe_merged");
}
if (channel==36) {
	x_y_regions = {0.0115, 0.0115, 0.00125, 0.026, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.026, 0.004, display, channel, "2pe_merged");
}
if (channel==37) {
	x_y_regions = {0.009, 0.009, 0.00115, 0.028, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.009, 0.009, 0.0005, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.009, 0.0012, 0.023, 0.0036, display, channel, "2pe_merged");
}
if (channel==38) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.025, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0285, 0.0042, display, channel, "2pe_merged");
}
if (channel==39) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.025, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.025, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.025, 0.0038, display, channel, "2pe_merged");
}
if (channel==40) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.023, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.027, 0.0037, display, channel, "2pe_merged");
}
if (channel==41) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.021, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==42) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.021, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04"); 
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel == 43 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==44) {
	x_y_regions = {0.009, 0.009, 0.00115, 0.019, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.009, 0.009, 0.0004, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04"); 
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0225, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.009, 0.0013, 0.0225, 0.0032, display, channel, "2pe_merged");
}
if (channel==48) {
	x_y_regions = {0.012, 0.012, 0.0012, 0.02, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0275, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0016, 0.0275, 0.0038, display, channel, "2pe_merged");
}
if (channel==49) {
	x_y_regions = {0.0085, 0.0085, 0.0010, 0.02, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0085, 0.0085, 0.0004, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0085, 0.0013, 0.02, 0.0028, display, channel, "2pe_merged");
}
if (channel==50) {
	x_y_regions = {0.0085, 0.0085, 0.0009, 0.02, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0085, 0.0085, 0.0003, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0085, 0.0013, 0.02, 0.0028, display, channel, "2pe_merged");
}
if (channel==51) {
	x_y_regions = {0.0105, 0.0105, 0.00105, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0105, 0.0105, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0105, 0.0016, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==52) {
	x_y_regions = {0.0115, 0.0115, 0.00105, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0018, 0.027, 0.0041, display, channel, "2pe_merged");
}
if (channel==53) {
	x_y_regions = {0.012, 0.012, 0.0012, 0.027, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0018, 0.028, 0.0043, display, channel, "2pe_merged");
}
if (channel==54) {
	x_y_regions = {0.009, 0.009, 0.0011, 0.032, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.009, 0.009, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.009, 0.0014, 0.023, 0.0034, display, channel, "2pe_merged");
}
if (channel==55) {
	x_y_regions = {0.01, 0.01, 0.00105, 0.030, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0235, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.0015, 0.0235, 0.0036, display, channel, "2pe_merged");
}
if (channel==56) {
	x_y_regions = {0.0115, 0.0115, 0.0011, 0.032, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0007, 0.03, 0.0024, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0014, 0.026, 0.004, display, channel, "2pe_merged");
}
if (channel==57) {
	x_y_regions = {0.012, 0.012, 0.00115, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0007, 0.03, 0.0024, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0265, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0015, 0.0265, 0.004, display, channel, "2pe_merged");
}
if (channel==58) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.025, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.025, 0.0038, display, channel, "2pe_merged");
}
if (channel==59) {
	x_y_regions = {0.011, 0.011, 0.0012, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.026, 0.0038, display, channel, "2pe_merged");
}

}

void analysis_history(bool calibrate, int method = 0) {
//Created on 18.11.2019
//method = 0 - new event selection (suppressing events with large tails)
calibration_file = "190404/results/190404_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
data_output_path = "190404/results_v3/";
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

//Ultimately I have to choose single procedure for signal forms among them for all data.
//TODO: experiments loop (need long strings whens saving pictures). Currently I have to run this calibration script for each nex();  
//TODO: move S2 times from cut_S2_times.cpp here. Use them instead of 23-65us
bool forms = !calibrate;
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
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

//CALIBRATION (slow PMT and SiPMs)
//parameters set by Cd_20kV_850V_46V, but valid for all fields. 
if (channel==15) {
ch(15);
	ty(AStates::PMT_A_S);
    set_zoom(0, 0.3, 0, 0.04);
    set_bins(60);
    cut_S_t_rect_exclude(23, 65, 0, 1e5, false); //exclude signal region for calibration. May be even better to use 0-23 us selectoin only
    noise_cut(15, 1, true);
    saveas(data_output_path+folder+"/calibration/15_A_S_wo_23-65us_zoom");//w = with, wo = without
    set_zoom(0, 1.3, 0, 0.5);
    set_bins(300);
    saveas(data_output_path+folder+"/calibration/15_A_S_wo_23-65us");
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.15);
    set_bins(200);
    cut_S_t_rect_exclude(23, 65, 0, 1e5, false);
    saveas(data_output_path+folder+"/calibration/15_Ss_wo_23-65us");
    noise_cut(15, 1, false);
    draw_limits(0.0, 0.055); //uncertainty 0.05-0.06 => S1pe 0.0224467-0.0251765 V*us
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/15_Ss_wo_23-65us");
}
if (channel==14) {
ch(14);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.8, 0, 0.2);        
    set_bins(300);
    cut_S_t_rect_select(0, 23, 0, 1e5, false);
    noise_cut(14, 1, true);
    saveas(data_output_path+folder+"/calibration/14_A_S_w_0-23us");        
    set_zoom(0, 0.4, 0, 0.07);
    set_bins(60);
    saveas(data_output_path+folder+"/calibration/14_A_S_w_0-23us_zoom");
    
    remcut_S_t_rect_select();
    cut_S_t_rect_exclude(23, 65, 0, 1e5, false);
    saveas(data_output_path+folder+"/calibration/14_A_S_wo_23-65us_zoom");
    set_zoom(0, 0.8, 0, 0.2);
    set_bins(300);
    saveas(data_output_path+folder+"/calibration/14_A_S_wo_23-65us");

    ty(AStates::PMT_Ss);
    set_zoom(0, 0.12);
    cut_S_t_rect_select(0, 23, 0, 1e5, false);
    noise_cut(14, 1, false);
    set_bins(200);
    saveas(data_output_path+folder+"/calibration/14_Ss_w_0-23us");
    cut_S_t_rect_select(65, 160, 0, 1e5, false);        
    draw_limits(0, 0.043);
    set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/14_Ss_w_65-160us");
}
if (channel==13) {
ch(13);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.8, 0, 0.2);        
    set_bins(400);
    noise_cut(13, 1, true);
    saveas(data_output_path+folder+"/calibration/13_A_S");        
    set_zoom(0, 0.2, 0, 0.035);
    set_bins(120);
    saveas(data_output_path+folder+"/calibration/13_A_S_zoom");

    ty(AStates::PMT_Ss);
    set_zoom(0, 0.1);
    noise_cut(13, 1, false);
    cut_S_t_rect_select(0, 23, 0, 1e5, false);
    set_bins(200);
    saveas(data_output_path+folder+"/calibration/13_Ss_w_0-23us");
    cut_S_t_rect_select(23, 65, 0, 1e5, false);
    set_bins(300);
    saveas(data_output_path+folder+"/calibration/13_Ss_w_23-65us");
    cut_S_t_rect_select(65, 160, 0, 1e5, false);        
    draw_limits(0, 0.033);
    set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/13_Ss_w_23-160us");
}
if (channel==12) {
ch(12);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.5, 0, 0.1);        
    set_bins(400);
    noise_cut(12, 1, true);
    saveas(data_output_path+folder+"/calibration/12_A_S");        
    set_zoom(0, 0.2, 0, 0.035);
    set_bins(150);
    saveas(data_output_path+folder+"/calibration/12_A_S_zoom");
    cut_S_t_rect_select(0, 23, 0, 1e5, false);
    set_bins(60);
    saveas(data_output_path+folder+"/calibration/12_A_S_w_0-23us_zoom");
    cut_S_t_rect_select(23, 65, 0, 1e5, false);
    set_bins(120);
    saveas(data_output_path+folder+"/calibration/12_A_S_w_23-65us_zoom");
    cut_S_t_rect_select(65, 160, 0, 1e5, false);
    set_bins(100);
    saveas(data_output_path+folder+"/calibration/12_A_S_w_65-160us_zoom");

    ty(AStates::PMT_Ss);
    set_zoom(0, 0.06);
    noise_cut(12, 1, false);
    cut_S_t_rect_select(0, 23, 0, 1e5, false);
    set_bins(200);
    saveas(data_output_path+folder+"/calibration/12_Ss_w_0-23us");
    cut_S_t_rect_select(23, 65, 0, 1e5, false);
    set_bins(400);
    saveas(data_output_path+folder+"/calibration/12_Ss_w_23-65us");
    cut_S_t_rect_select(65, 160, 0, 1e5, false);        
    draw_limits(0, 0.022);
    set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/12_Ss_w_23-160us");
}
if (channel==11) {
ch(11);
    ty(AStates::PMT_A_S);
    noise_cut(11, 1, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas(data_output_path+folder+"/calibration/11_A_S");
    set_zoom(0, 0.08, 0, 0.003);
    saveas(data_output_path+folder+"/calibration/11_A_S_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas(data_output_path+folder+"/calibration/11_A_S_zoom1");
}
if (channel==10) {
ch(10);
    ty(AStates::PMT_A_S);
    noise_cut(10, 1, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas(data_output_path+folder+"/calibration/10_A_S");
    set_zoom(0, 0.08, 0, 0.003);
    saveas(data_output_path+folder+"/calibration/10_A_S_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas(data_output_path+folder+"/calibration/10_A_S_zoom1");
}
if (channel==9) {
ch(9);
    ty(AStates::PMT_A_S);
    noise_cut(9, 1, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas(data_output_path+folder+"/calibration/9_A_S");
    set_zoom(0, 0.08, 0, 0.003);
    saveas(data_output_path+folder+"/calibration/9_A_S_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas(data_output_path+folder+"/calibration/9_A_S_zoom1");
}
if (channel==8) {
ch(8);
    ty(AStates::PMT_A_S);
    noise_cut(8, 1, true);
    set_bins(3000);
    set_zoom(0, 0.4, 0, 0.018);
    saveas(data_output_path+folder+"/calibration/8_A_S");
    set_zoom(0, 0.08, 0, 0.003);
    saveas(data_output_path+folder+"/calibration/8_A_S_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas(data_output_path+folder+"/calibration/8_A_S_zoom1");
}
}//for (channels)
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

if ((exp == "190404_Cd_20kV_850V_46V_th250mV" || exp == "190404_Cd_20kV_850V_46V_th250mV_0") && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
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
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	cut_x_y_right(35, 0, 47, 17, true, "1");
	cut_x_y_right(47, 17, 90, 36, true, "2");
	cut_x_y_right(90, 36, 122, 66, true, "3");
	cut_x_y_right(122, 66, 400, 244, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.5, 0, 30.5, 160, true, "1");
	cut_x_y_upper(0, 30.7, 160, 30.7, true, "2");
	cut_x_y_left(25.9, 0, 25.9, 160, true, "3");
	cut_x_y_lower(0, 26.0, 160, 26.0, true, "4");
	cut_x_y_right(26.09, 25.63, 31.51, 30.27, true, "5");
	cut_x_y_left(25.30, 26.20, 29.6, 31.15, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 40);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(41, 70);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(41, 105);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(130, 220);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_18kV_850V_46V_th230mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
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
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 160);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(20, 0, 36, 14, true, "1");
	cut_x_y_right(36, 14, 60, 24, true, "2");
	cut_x_y_right(60, 24, 103, 62, true, "3");
	cut_x_y_right(103, 62, 300, 200, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.75, 0, 30.75, 160, true, "1");
	cut_x_y_upper(0, 30.8, 160, 30.8, true, "2");
	cut_x_y_left(25.7, 0, 25.7, 160, true, "3");
	cut_x_y_lower(0, 26.0, 160, 26.0, true, "4");
	cut_x_y_right(26.10, 25.50, 31.50, 30.20, true, "5");
	cut_x_y_left(25.34, 26.36, 29.27, 31.14, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 29);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(30, 52);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(30, 80);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(100, 200);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_16kV_850V_46V_th210mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
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
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 160);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
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
	cut_x_y_right(17, 0, 28, 11, true, "1");
	cut_x_y_right(28, 11, 40, 16, true, "2");
	cut_x_y_right(40, 16, 87, 56, true, "3");
	cut_x_y_right(87, 56, 250, 180, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.6, 0, 30.6, 160, true, "1");
	cut_x_y_upper(0, 30.8, 160, 30.8, true, "2");
	cut_x_y_left(26.14, 0, 26.14, 160, true, "3");
	cut_x_y_lower(0, 26.16, 160, 26.16, true, "4");
	cut_x_y_right(26.10, 25.50, 31.50, 30.20, true, "5");
	cut_x_y_left(25.34, 26.36, 29.27, 31.14, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 21);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(22, 37);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(22, 65);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(80, 150);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_14kV_850V_46V_th200mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
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
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
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
	cut_x_y_right(12, 0, 22, 10, true, "1");
	cut_x_y_right(22, 10, 42, 22, true, "2");
	cut_x_y_right(42, 22, 59, 42, true, "3");
	cut_x_y_right(59, 42, 200, 157, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.84, 0, 30.84, 160, true, "1");
	cut_x_y_upper(0, 31.14, 160, 31.14, true, "2");
	cut_x_y_left(26.14, 0, 26.14, 160, true, "3");
	cut_x_y_lower(0, 26.16, 160, 26.16, true, "4");
	cut_x_y_right(26.10, 25.50, 31.50, 30.20, true, "5");
	cut_x_y_left(25.34, 26.36, 29.27, 31.14, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 14);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(15, 28);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(15, 50);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(65, 140);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_12kV_850V_46V_th160mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
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
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 160);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(160);
	cut_x_y_right(6.5, 0, 28.8, 17.8, true, "1");
	cut_x_y_right(28.8, 17.8, 42.2, 33.4, true, "2");
	cut_x_y_right(42.2, 33.4, 160, 130, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(31.33, 0, 31.33, 160, true, "1");
	cut_x_y_upper(0, 31.47, 160, 31.47, true, "2");
	cut_x_y_left(26.14, 0, 26.14, 160, true, "3");
	cut_x_y_lower(0, 26.16, 160, 26.16, true, "4");
	cut_x_y_right(25.92, 25.23, 31.51, 30.28, true, "5");
	cut_x_y_left(25.47, 26.76, 30.0, 31.74, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe); //DIFFERENT FROM HIGHER FIELDS!
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 9);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(10, 20);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(10, 37);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_10kV_850V_46V_th150mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
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
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 160);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(160);
	cut_x_y_right(7.5, 0, 160, 140, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(31.51, 0, 31.51, 160, true, "1");
	cut_x_y_upper(0, 31.8, 160, 31.8, true, "2");
	cut_x_y_left(26.14, 0, 26.14, 160, true, "3");
	cut_x_y_lower(0, 26.16, 160, 26.16, true, "4");
	cut_x_y_right(26.41, 25.37, 31.87, 30.34, true, "5");
	cut_x_y_left(25.24, 26.83, 29.77, 31.80, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe); //DIFFERENT FROM HIGHER FIELDS!
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

//ty(AStates::PMT_Npe_sum);
//	draw_limits(0, 0);
//	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
//	set_as_run_cut("En_spec");	cuts += "+" + Num;
//	Num = int_to_str(++no, 2);
//
//save_forms(FOLDER + "forms_small_Npes/", false);
//	unset_as_run_cut("En_spec");
//	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(4, 13);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(4, 30);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(40, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190404_Cd_8kV_850V_46V_th140mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
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
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 160);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 120);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	noise_cut(8, 0, 0); noise_cut(9, 0, 0); noise_cut(10, 0, 0); noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	noise_cut(12, 0, 0); noise_cut(13, 0, 0); noise_cut(14, 0, 0); noise_cut(15, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(160);
	cut_x_y_right(7.5, 0, 160, 140, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(32.10, 0, 32.10, 160, true, "1");
	cut_x_y_upper(0, 32.33, 160, 32.33, true, "2");
	cut_x_y_left(25.38, 0, 25.38, 160, true, "3");
	cut_x_y_lower(0, 25.63, 160, 25.63, true, "4");
	cut_x_y_right(25.56, 24.17, 32.95, 31.07, true, "5");
	cut_x_y_left(22.83, 24.44, 31.92, 34.72, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe); //DIFFERENT FROM HIGHER FIELDS!
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

//ty(AStates::PMT_Npe_sum);
//	draw_limits(0, 0);
//	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
//	set_as_run_cut("En_spec");	cuts += "+" + Num;
//	Num = int_to_str(++no, 2);
//
//save_forms(FOLDER + "forms_small_Npes/", false);
//	unset_as_run_cut("En_spec");
//	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(3, 10);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(3, 22);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(30, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}
//END OF FORMS
} 
