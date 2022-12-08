void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom (double f_time, double to_time);
void central_SiPMs(bool on);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);

void time_zoom (double f_time, double to_time) {
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 8);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 9);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 10);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 11);
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
	ty(AStates::MPPC_tbNpe_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_condition, false);
	}
	set_zoom(20, 160);
	set_bins(1000);
	saveas(path + "SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(path + "SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, PMT_condition, false);
	set_zoom(20, 160);
	set_bins(800);
	saveas(path + "8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, PMT_condition, false);
	set_zoom(20, 160);
	set_bins(800);
	saveas(path + "9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, PMT_condition, false);
	set_zoom(20, 160);
	set_bins(800);
	saveas(path + "10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, PMT_condition, false);
	set_zoom(20, 160);
	set_bins(800);
	saveas(path + "11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, PMT_condition, false);
	set_zoom(20, 160);
	set_bins(800);
	saveas(path + "12_form_by_Npeaks");

	ch(1);
	noise_cut(1, 1, PMT_condition, false);
	set_zoom(20, 160);
	set_bins(800);
	saveas(path + "1_form_by_Npe");

	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(8);
		noise_cut(8, 1, PMT_condition, false);
		set_zoom(20, 160);
		set_bins(800);
		saveas(path + "8_form_by_S");
		ch(9);
		noise_cut(9, 1, PMT_condition, false);
		set_zoom(20, 160);
		set_bins(800);
		saveas(path + "9_form_by_S");
		ch(10);
		noise_cut(10, 1, PMT_condition, false);
		set_zoom(20, 160);
		set_bins(800);
		saveas(path + "10_form_by_S");
		ch(11);
		noise_cut(11, 1, PMT_condition, false);
		set_zoom(20, 160);
		set_bins(800);
		saveas(path + "11_form_by_S");
		ch(12);
		noise_cut(12, 1, PMT_condition, false);
		set_zoom(20, 160);
		set_bins(800);
		saveas(path + "12_form_by_S");

		ch(1);
		noise_cut(1, 1, PMT_condition, false);
		set_zoom(20, 160);
		set_bins(800);
		saveas(path + "1_form_by_S");
	}
}

//Even it is hard to read the whole analysis, it is better to move all A-S cuts for PMTs in one
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
//	PMT: 0 - 800V, 12dB (20kV), 1 - 800V, 6 dB (10-12kV), 2 - 800V, 0 dB (8-9kV)
//	SiPM: 0 - 48V
if (channel==0) {
	switch (device_condition) {
	case 1: {
		cut_A_S_upper(0.048, 0.01, 0.0565, 0.0112, display, channel, "rem_smallA_largeS");
		x_y_regions = {0.05, 0.05, 0.00180, 0.093, 0.00256, 0.093};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "rem_smallA_smallS");
		break;
	}
	case 2:  {
		cut_A_S_upper(0.08, 0.0157, 0.102, 0.020, display, channel, "rem_smallA_largeS");
		x_y_regions = {0.08, 0.08, 0.00287, 0.171, 0.00462, 0.171};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "rem_smallA_smallS");
		break;
	}
	default : {
		cut_A_S_upper(0.035, 0.009, 0.042, 0.010, display, channel, "rem_smallA_largeS");
		x_y_regions = {0.035, 0.035, 0.00123, 0.065, 0.00173, 0.065};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "rem_smallA_smallS");
	}
	}
}
if (channel==1) {
	cut_A_S_upper(0.06, 0.0234, 0.0758, 0.0278, display, channel, "rem_smallA_largeS");
	x_y_regions = {0.06, 0.06, 0.00230, 0.134, 0.00344, 0.2};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "rem_smallA_smallS");
}
if (channel==12) {
	if (aggressiveness ==1) {
		cut_A_S_upper(1.5, 0, 10, 0, display, channel, "rem_A>1.5");
	}
}
if (channel==11) {
	if (aggressiveness ==1) {
		cut_A_S_upper(1.5, 0, 10, 0, display, channel, "rem_A>1.5");
	}
}
if (channel==10) {
    if (aggressiveness ==1) {
		cut_A_S_upper(1.5, 0, 10, 0, display, channel, "rem_A>1.5");
	}
}
if (channel==9) {
    if (aggressiveness ==1) {
		cut_A_S_upper(1.5, 0, 10, 0, display, channel, "rem_A>1.5");
	}
}
if (channel==8) {
	if (aggressiveness ==1) {
		cut_A_S_upper(1.5, 0, 10, 0, display, channel, "rem_A>1.5");
	}
}
//The following are set by hand for 20kV individually, there is no other way. 48V.
if (channel==32) {
   x_y_regions = {0.018, 0.018, 0.0010, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.046, 0.0059, display, channel, "2pe_merged");
}
if (channel==33) {
	x_y_regions = {0.0177, 0.0177, 0.00115, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.038, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0019, 0.038, 0.0050, display, channel, "2pe_merged");
}
if (channel==34) {
	x_y_regions = {0.02, 0.02, 0.0014, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.048, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0019, 0.048, 0.0064, display, channel, "2pe_merged");
}
if (channel==35) {
	x_y_regions = {0.020, 0.020, 0.0011, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.049, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.049, 0.0068, display, channel, "2pe_merged");
}
if (channel==36) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.042, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.042, 0.0058, display, channel, "2pe_merged");
}
if (channel==37 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(37);
}
if (channel==38) {
	x_y_regions = {0.02, 0.02, 0.0012, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.046, 0.0063, display, channel, "2pe_merged");
}
if (channel==39) {
	x_y_regions = {0.0185, 0.0185, 0.00138, 0.09, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.043, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.043, 0.0058, display, channel, "2pe_merged");
}
if (channel==40) {
	x_y_regions = {0.018, 0.018, 0.0012, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.047, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.047, 0.0066, display, channel, "2pe_merged");
}
if (channel==41) {
	x_y_regions = {0.0160, 0.0160, 0.00107, 0.07, 0.0060, 0.07};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	cut_A_S_upper(0.0160, 0.00154, 0.0212, 0.00330, display, channel, "rem_smallA_largeS");
	if (aggressiveness>=1) {//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0423, 0.0001, 1, 0.0001, display, channel, "2pe");
		cut_A_S_right(0.0350, 0.00254, 0.0486, 0.00648, display, channel, "2pe_1");
	}
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0160, 0.0026, 0.044, 0.0070, display, channel, "2pe_merged");
}
if (channel==42) {
	x_y_regions = {0.018, 0.018, 0.00125, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.00183, 0.045, 0.00597, display, channel, "2pe_merged");
}
if (channel == 43) {
	x_y_regions = {0.020, 0.020, 0.00115, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.045, 0.0060, display, channel, "2pe_merged");
}
if (channel==44 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(44);
}
if (channel==48) {
	x_y_regions = {0.020, 0.020, 0.00128, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.047, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.002, 0.047, 0.0062, display, channel, "2pe_merged");
}
if (channel==49) {
	x_y_regions = {0.0163, 0.0163, 0.00103, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.037, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.015, 0.0020, 0.037, 0.0047, display, channel, "2pe_merged");
}
if (channel==50) {
	x_y_regions = {0.015, 0.015, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.037, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.015, 0.0021, 0.037, 0.0052, display, channel, "2pe_merged");
}
if (channel==51) {
	x_y_regions = {0.0186, 0.0186, 0.00103, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0020, 0.045, 0.0060, display, channel, "2pe_merged");
}
if (channel==52) {
	x_y_regions = {0.0188, 0.0188, 0.00103, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.043, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.017, 0.0021, 0.043, 0.0064, display, channel, "2pe_merged");
}
if (channel==53) {
	x_y_regions = {0.020, 0.020, 0.00119, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.046, 0.0066, display, channel, "2pe_merged");
}
if (channel==54) {
	x_y_regions = {0.019, 0.019, 0.0015, 0.06, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0406, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.0406, 0.0056, display, channel, "2pe_merged");
}
if (channel==55) {
	x_y_regions = {0.0193, 0.0193, 0.00123, 0.09, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.04, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.00194, 0.04, 0.0057, display, channel, "2pe_merged");
}
if (channel==56) {
	x_y_regions = {0.0161, 0.0161, 0.00120, 0.07, 0.00624, 0.07};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	cut_A_S_left(0.014, 0.00063, 0.0224, 0.0038, display, channel, "rem_smallA_largeS");
	if (aggressiveness>=1) {//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0426, 0.0001, 1, 0.0001, display, channel, "2pe");
		cut_A_S_right(0.0355, 0.0021, 0.0498, 0.0088, display, channel, "2pe_1");
	}
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0161, 0.00246, 0.0464, 0.0070, display, channel, "2pe_merged");
}
if (channel==57) {
	x_y_regions = {0.0169, 0.0169, 0.0012, 0.07, 0.0046, 0.07};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.042, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0169, 0.00246, 0.042, 0.0060, display, channel, "2pe_merged");
}
if (channel==58) {
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.043, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.043, 0.0060, display, channel, "2pe_merged");
}
if (channel==59) {
	x_y_regions = {0.021, 0.021, 0.0015, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.044, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.044, 0.0061, display, channel, "2pe_merged");
}

}//noise_cut

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 17.12.2019
//method = 0 - use fast PMTs for trigger_v2 (byNpe for large fields/byNpeaks for low) with
//optimal dt determined as
//dt_trigger_optimal[us] = 0.3923 + 0.9728*Tdrift(18mm)[us] (for previous analysis)
//method = 1 - same as 0 but not using trigger adjustment (raw signals, but with same event selection as 0)
data_output_path = "180705/results_v4/";
calibration_file = data_output_path+"180705_calibration.dat";
trigger_version = TriggerVersion::trigger_v2;
if (method==1)
	data_output_path = "180705/results_v1/";
if (method>1) {
	std::cout<<"There is no method > 1, quitting"<<std::endl;
	return;
}

post_processor->calibr_info.Load(calibration_file);
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["180705_Cd_20kV_800V_12bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_18kV_800V_12bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_16kV_800V_12bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_14kV_800V_12bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_13kV_800V_12bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_12kV_800V_6bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_11kV_800V_6bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_10kV_800V_6bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_9kV_800V_0bB_48V"] = std::pair<double, double> (20, 38);
S2_times["180705_Cd_8kV_800V_0bB_48V"] = std::pair<double, double> (20, 38);
std::map<std::string, std::string> exp_folders;
exp_folders["180705_Cd_20kV_800V_12bB_48V"] = "Cd_48V_20kV_800V";
exp_folders["180705_Cd_18kV_800V_12bB_48V"] = "Cd_48V_18kV_800V";
exp_folders["180705_Cd_16kV_800V_12bB_48V"] = "Cd_48V_16kV_800V";
exp_folders["180705_Cd_14kV_800V_12bB_48V"] = "Cd_48V_14kV_800V";
exp_folders["180705_Cd_13kV_800V_12bB_48V"] = "Cd_48V_13kV_800V";
exp_folders["180705_Cd_12kV_800V_6bB_48V"] = "Cd_48V_12kV_800V";
exp_folders["180705_Cd_11kV_800V_6bB_48V"] = "Cd_48V_11kV_800V";
exp_folders["180705_Cd_10kV_800V_6bB_48V"] = "Cd_48V_10kV_800V";
exp_folders["180705_Cd_9kV_800V_0bB_48V"] = "Cd_48V_9kV_800V";
exp_folders["180705_Cd_8kV_800V_0bB_48V"] = "Cd_48V_8kV_800V";

bool forms = !calibrate;

//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
int PMT_state = 0; //800V, 12 dB
int SiPM_state = 0; //48V
std::string exp = post_processor->experiments[post_processor->current_exp_index];
if (exp == "180705_Cd_12kV_800V_6bB_48V"
	|| exp == "180705_Cd_11kV_800V_6bB_48V"
	|| exp == "180705_Cd_10kV_800V_6bB_48V")
	PMT_state = 1;
if (exp == "180705_Cd_9kV_800V_0bB_48V"
	|| exp == "180705_Cd_8kV_800V_0bB_48V")
	PMT_state = 2;
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
//parameters set by Cd_20kV_800V_48V.
if (channel>=32) {
	if (getIndex(post_processor->MPPC_channels, channel)<0)
		continue;
ty(AStates::MPPC_A_S);
	ch(channel);
    set_zoom(0, 0.10, 0, 0.015);
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

if (channel==0) {
	ty(AStates::PMT_A_S);
	ch(channel);
  noise_cut(channel, 0, PMT_state, true);
	set_bins(1000);
  saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	if (2 == PMT_state)
		set_zoom(0, 0.35, 0, 0.06);
	else
		set_zoom(0, 0.25, 0, 0.04);
  set_bins(1000);
  saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
	ty(AStates::PMT_Ss);
  set_zoom(0, 0.2);
  noise_cut(channel, 0, PMT_state, false);
  set_bins(600);
  saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
  cut_t(0, d_S2_start, false, channel);
	if (0 == PMT_state)
 		draw_limits(0.0015, 0.04);
	if (1 == PMT_state)
		draw_limits(0.0, 0.06);
	if (2 == PMT_state)
		draw_limits(0.0, 0.03);
	set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}

if (channel==1) {
	ty(AStates::PMT_A_S);
	ch(channel);
    noise_cut(channel, 0, PMT_state, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0.06, 0.25, 0, 0.06);
    set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.15);
    noise_cut(channel, 0, PMT_state, false);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.08);
	set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}

if (channel==8) {
	ty(AStates::PMT_A_S);
	ch(channel);
    noise_cut(channel, 1, PMT_state, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.15, 0, 0.01);
    set_bins(500);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    noise_cut(channel, 0, PMT_state, false);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.001);
	set_use_mean(); //Not for actual Npe, but for correct trigger adjustment using fastPMT channels (correct weights for channels)
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}
if (channel==9) {
	ty(AStates::PMT_A_S);
	ch(channel);
    noise_cut(channel, 1, PMT_state, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.15, 0, 0.01);
    set_bins(500);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    noise_cut(channel, 0, PMT_state, false);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.001);
	set_use_mean(); //Not for actual Npe, but for correct trigger adjustment using fastPMT channels (correct weights for channels)
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}
if (channel==10) {
	ty(AStates::PMT_A_S);
	ch(channel);
    noise_cut(channel, 1, PMT_state, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.15, 0, 0.01);
    set_bins(500);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    noise_cut(channel, 0, PMT_state, false);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.001);
	set_use_mean(); //Not for actual Npe, but for correct trigger adjustment using fastPMT channels (correct weights for channels)
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}
if (channel==11) {
ty(AStates::PMT_A_S);
	ch(channel);
    noise_cut(channel, 1, PMT_state, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.15, 0, 0.01);
    set_bins(500);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    noise_cut(channel, 0, PMT_state, false);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.001);
	set_use_mean(); //Not for actual Npe, but for correct trigger adjustment using fastPMT channels (correct weights for channels)
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}
if (channel==12) {
ty(AStates::PMT_A_S);
	ch(channel);
    noise_cut(channel, 1, PMT_state, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.15, 0, 0.01);
    set_bins(500);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    noise_cut(channel, 0, PMT_state, false);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
	draw_limits(0.0, 0.001);
	set_use_mean(); //Not for actual Npe, but for correct trigger adjustment using fastPMT channels (correct weights for channels)
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}
}//for (channels)
nex();
}//for (experiments)
if (calibrate) {
	calib_save(calibration_file);
}
//END OF CALIBRATION

//FORMS NO CUTS/PEAK-NOISE SELECTIONS
//First are signal forms without cuts at all, even Npe, aggressiveness = 0. There are time windows for Npe though.
if (!forms)
	return;
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
int first_run = 0;
int PMT_state = 0; //800V, 12 dB
int SiPM_state = 0; //48V
std::string exp = post_processor->experiments[post_processor->current_exp_index];
if (exp == "180705_Cd_12kV_800V_6bB_48V"
	|| exp == "180705_Cd_11kV_800V_6bB_48V"
	|| exp == "180705_Cd_10kV_800V_6bB_48V")
	PMT_state = 1;
if (exp == "180705_Cd_9kV_800V_0bB_48V"
	|| exp == "180705_Cd_8kV_800V_0bB_48V")
	PMT_state = 2;
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

if (exp == "180705_Cd_20kV_800V_12bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 1200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 650);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Very poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with larges gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 1200, 0, 1200);
	set_bins(1200);
	cut_x_y_right(25.8, 0, 548, 390, true, "1");
	cut_x_y_right(548, 390, 728, 485, true, "2");
	cut_x_y_right(728, 485, 848, 654, true, "3");
	cut_x_y_right(848, 654, 1200, 871, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 1200, 0, 1200);
	set_bins(1200);
	cut_x_y_right(10.3, 0, 580.6, 554.3, true, "1");
	cut_x_y_right(580.6, 554.3, 819.2, 783, true, "2");
	cut_x_y_right(819.2, 783, 1200, 970.6, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 37, 27, 37);
	set_bins(600);
	cut_x_y_right(35.69, 0, 35.69, 160, true, "1");
	cut_x_y_upper(0, 35.88, 160, 35.88, true, "2");
	cut_x_y_left(30.53, 0, 30.53, 160, true, "3");
	cut_x_y_lower(0, 30.47, 160, 30.47, true, "4");
	cut_x_y_left(27.13, 27.26, 36.39, 36.68, true, "5");
	cut_x_y_right(27.72, 27.58, 36.40, 36.18, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by S" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_S_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(130, 230);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_left/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(231, 400);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(550, 705);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(550, 780);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_18kV_800V_12bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 1200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 650);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with larges gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 1200, 0, 1200);
	set_bins(1200);
	cut_x_y_right(9.22, 0, 479, 357, true, "1");
	cut_x_y_right(479, 357, 613, 430, true, "2");
	cut_x_y_right(613, 430, 754, 594, true, "3");
	cut_x_y_right(754, 594, 1200, 907, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 1200, 0, 1200);
	set_bins(1200);
	cut_x_y_right(15.1, 0, 573.4, 545.1, true, "1");
	cut_x_y_right(573.4, 545.1, 720.4, 698, true, "2");
	cut_x_y_right(720.4, 698, 1200, 990, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 37, 27, 37);
	set_bins(600);
	cut_x_y_right(35.69, 0, 35.69, 160, true, "1");
	cut_x_y_upper(0, 35.88, 160, 35.88, true, "2");
	cut_x_y_left(30.44, 0, 30.44, 160, true, "3");
	cut_x_y_lower(0, 30.47, 160, 30.47, true, "4");
	cut_x_y_left(27.54, 27.68, 36.07, 36.36, true, "5");
	cut_x_y_right(27.76, 27.50, 36.42, 36.13, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by S" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_S_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(130, 210);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_left/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(211, 370);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(440, 585);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(440, 690);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_16kV_800V_12bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 800);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 550);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 800, 0, 800);
	set_bins(800);
	cut_x_y_right(9.22, 0, 351, 261, true, "1");
	cut_x_y_right(351, 261, 489, 354, true, "2");
	cut_x_y_right(489, 354, 609, 473, true, "3");
	cut_x_y_right(609, 473, 800, 619, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 800, 0, 800);
	set_bins(800);
	cut_x_y_right(7.5, 4, 412, 399.5, true, "1");
	cut_x_y_right(412, 399.5, 582, 568.5, true, "2");
	cut_x_y_right(582, 568.5, 800, 772, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 37, 27, 37);
	set_bins(600);
	cut_x_y_right(35.69, 0, 35.69, 160, true, "1");
	cut_x_y_upper(0, 35.88, 160, 35.88, true, "2");
	cut_x_y_left(30.33, 0, 30.33, 160, true, "3");
	cut_x_y_lower(0, 30.26, 160, 30.26, true, "4");
	cut_x_y_left(27.54, 27.72, 36.07, 36.40, true, "5");
	cut_x_y_right(27.76, 27.50, 36.42, 36.13, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by S" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_S_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(60, 130);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_left/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_left/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(131, 200);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(330, 455);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(330, 560);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_14kV_800V_12bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 650);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 450);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 650, 0, 650);
	set_bins(650);
	cut_x_y_right(3.2, 0, 157.5, 121, true, "1");
	cut_x_y_right(157.5, 121, 288.7, 214.8, true, "2");
	cut_x_y_right(288.7, 214.8, 478, 376, true, "3");
	cut_x_y_right(478, 376, 650, 508, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 650, 0, 650);
	set_bins(650);
	cut_x_y_right(7.5, 4, 315, 303, true, "1");
	cut_x_y_right(315, 303, 427.7, 417.5, true, "2");
	cut_x_y_right(427.7, 417.5, 650, 630, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 37, 27, 37);
	set_bins(600);
	cut_x_y_right(35.69, 0, 35.69, 160, true, "1");
	cut_x_y_upper(0, 35.88, 160, 35.88, true, "2");
	cut_x_y_left(30.33, 0, 30.33, 160, true, "3");
	cut_x_y_lower(0, 30.26, 160, 30.26, true, "4");
	cut_x_y_left(27.54, 27.80, 36.07, 36.48, true, "5");
	cut_x_y_right(27.76, 27.50, 36.42, 36.13, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by S" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_S_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(70, 150);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(195, 280);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(195, 365);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(420, 650);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_13kV_800V_12bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 450);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	cut_x_y_right(3.2, 0, 131.3, 95.4, true, "1");
	cut_x_y_right(131.3, 95.4, 234.5, 172.8, true, "2");
	cut_x_y_right(234.5, 172.8, 327.9, 261.0, true, "3");
	cut_x_y_right(327.9, 261.0, 600, 471, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	cut_x_y_right(7.5, 4, 195.9, 186.4, true, "1");
	cut_x_y_right(195.9, 186.4, 335.1, 326.2, true, "2");
	cut_x_y_right(335.1, 326.2, 600, 587, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 37, 27, 37);
	set_bins(600);
	cut_x_y_right(35.63, 0, 35.63, 160, true, "1");
	cut_x_y_upper(0, 35.65, 160, 35.65, true, "2");
	cut_x_y_left(30.58, 0, 30.58, 160, true, "3");
	cut_x_y_lower(0, 30.53, 160, 30.53, true, "4");
	cut_x_y_left(27.54, 27.95, 36.07, 36.63, true, "5");
	cut_x_y_right(27.76, 27.50, 36.42, 36.13, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by S" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_S_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(30, 100);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(115, 200);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(115, 290);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(330, 600);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_12kV_800V_6bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 450);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	cut_x_y_right(1.8, 0, 75.5, 58.6, true, "1");
	cut_x_y_right(75.5, 58.6, 135.5, 96.6, true, "2");
	cut_x_y_right(135.5, 96.6, 230.9, 182.5, true, "3");
	cut_x_y_right(230.9, 182.5, 500, 403, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	cut_x_y_right(7.5, 4, 150.9, 142.6, true, "1");
	cut_x_y_right(150.9, 142.6, 237.5, 232.9, true, "2");
	cut_x_y_right(237.5, 232.9, 500, 487, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 40, 27, 40);
	set_bins(600);
	cut_x_y_right(35.13, 0, 35.13, 160, true, "1");
	cut_x_y_upper(0, 35.19, 160, 35.19, true, "2");
	cut_x_y_left(30.34, 0, 30.34, 160, true, "3");
	cut_x_y_lower(0, 30.22, 160, 30.22, true, "4");
	cut_x_y_left(27.41, 28.04, 35.92, 36.68, true, "5");
	cut_x_y_right(28.08, 27.45, 36.73, 36.10, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by Npeaks" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(20, 73);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_W_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_W_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(83, 129);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(83, 190);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(220, 400);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_11kV_800V_6bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 350);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 350);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
	cut_x_y_right(1.8, 0, 59.7, 45.2, true, "1");
	cut_x_y_right(59.7, 45.2, 114.4, 86.3, true, "2");
	cut_x_y_right(114.4, 86.3, 168.6, 136.5, true, "3");
	cut_x_y_right(168.6, 136.5, 350, 278, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
	cut_x_y_right(3.15, 0, 80.3, 73.3, true, "1");
	cut_x_y_right(80.3, 73.3, 153.1, 148.6, true, "2");
	cut_x_y_right(153.1, 148.6, 350, 342, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 40, 27, 40);
	set_bins(600);
	cut_x_y_right(35.13, 0, 35.13, 160, true, "1");
	cut_x_y_upper(0, 35.19, 160, 35.19, true, "2");
	cut_x_y_left(29.82, 0, 29.82, 160, true, "3");
	cut_x_y_lower(0, 29.59, 160, 29.59, true, "4");
	cut_x_y_left(27.41, 28.34, 35.78, 37.10, true, "5");
	cut_x_y_right(28.08, 27.10, 36.73, 35.75, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by Npeaks" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(15, 50);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(55, 89);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(55, 135);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(150, 300);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_10kV_800V_6bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	cut_x_y_right(4.5, 0, 29.2, 22.1, true, "1");
	cut_x_y_right(29.2, 22.1, 55.3, 41.6, true, "2");
	cut_x_y_right(55.3, 41.6, 86.8, 71.6, true, "3");
	cut_x_y_right(86.8, 71.6, 250, 204, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	cut_x_y_right(1.48, 0, 52.3, 46.8, true, "1");
	cut_x_y_right(52.3, 46.8, 110.7, 107.2, true, "2");
	cut_x_y_right(110.7, 107.2, 250, 243, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 40, 27, 40);
	set_bins(600);
	cut_x_y_right(34.62, 0, 34.62, 160, true, "1");
	cut_x_y_upper(0, 34.68, 160, 34.68, true, "2");
	cut_x_y_left(29.87, 0, 29.87, 160, true, "3");
	cut_x_y_lower(0, 29.80, 160, 29.80, true, "4");
	cut_x_y_left(27.62, 28.11, 35.99, 36.93, true, "5");
	cut_x_y_right(27.84, 27.11, 36.55, 35.82, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by Npeaks" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 26);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(28, 50);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(28, 78);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(90, 200);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_9kV_800V_0bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	cut_x_y_right(4.5, 0, 150, 129.6, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	cut_x_y_right(3.89, 0, 150, 147, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 40, 27, 40);
	set_bins(600);
	cut_x_y_right(34.62, 0, 34.62, 160, true, "1");
	cut_x_y_upper(0, 34.68, 160, 34.68, true, "2");
	cut_x_y_left(29.87, 0, 29.87, 160, true, "3");
	cut_x_y_lower(0, 29.80, 160, 29.80, true, "4");
	cut_x_y_left(28.03, 28.66, 35.83, 37.06, true, "5");
	cut_x_y_right(28.76, 27.73, 36.64, 35.71, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by Npeaks" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method)
		set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 15);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_small_Npes/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(16, 25);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(16, 39);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(50, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "180705_Cd_8kV_800V_0bB_48V") {
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
	ch(0); on_ch(1); noise_cut(0, 0, PMT_state, 0); noise_cut(1, 0, PMT_state, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //Poor results because it favours PMT channel with largest gain (S of 1phe)
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //Poor results because fast PMTs have bad calibration precision
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, PMT_state, 0);
	noise_cut(9, 0, PMT_state, 0);
	noise_cut(10, 0, PMT_state, 0);
	noise_cut(11, 0, PMT_state, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	cut_x_y_right(5.0, 0, 150, 138, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::Correlation_y);
	double T2 = 60;
	std::string str_T2 = dbl_to_str(T2, 0);
	cut_t(d_S2_start, T2, false, 0);
	cut_t(d_S2_start, T2, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	cut_x_y_right(3.89, 0, 150, 147, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+str_T2+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+str_T2+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy2");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(27, 40, 27, 40);
	set_bins(600);
	cut_x_y_right(34.64, 0, 34.64, 160, true, "1");
	cut_x_y_upper(0, 35.02, 160, 35.02, true, "2");
	cut_x_y_left(29.71, 0, 29.71, 160, true, "3");
	cut_x_y_lower(0, 29.49, 160, 29.49, true, "4");
	cut_x_y_left(28.17, 28.49, 35.66, 36.96, true, "5");
	cut_x_y_right(28.87, 27.83, 36.22, 35.68, true, "6");
	set_titles("t by fastPMTs by Npe " + DT, "t by fastPMTs by Npeaks" + DT);
	update();
	set_as_run_cut("good_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_vs_Npe_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks); //Different from higher fields! trgger by N peaks is used here. (Poor calibration of fPMTs)
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	if (0==method) {
		set_trigger_offsets(trigger_at);
		cuts += "+" + Num;
	}
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_fastPMTs_Npetrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bS);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_sum_N);
	time_zoom(d_S2_start, d_S2_finish);
	draw_limits(6, 150);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_w_"+S2_start+"-"+S2_finish+"us_"+cuts);
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(10, 16);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_right/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(10, 28);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(40, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_cosmic/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}
//END OF FORMS
}
