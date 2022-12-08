void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_PMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
void central_SiPMs(bool on);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);
int save_forms (std::string prefix, std::string output_file, int Nbins, int type);

void time_zoom_PMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 0);
	cut_t(f_time, to_time, false, 1);
	cut_t(f_time, to_time, false, 2);
	cut_t(f_time, to_time, false, 3);
	cut_t(f_time, to_time, false, 4);

	cut_t(f_time, to_time, false, 5);
	cut_t(f_time, to_time, false, 6);
	cut_t(f_time, to_time, false, 7);
	cut_t(f_time, to_time, false, 8);
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

void fast_PMTs_only (void) {
	on_ch(5); on_ch(6); on_ch(7); on_ch(8);
	off_ch(0); off_ch(1); off_ch(2); off_ch(3); off_ch(4);
}

void slow_PMTs_only (void) {
	on_ch(1); on_ch(2); on_ch(3); on_ch(4);
	off_ch(0); off_ch(5); off_ch(6); off_ch(7); off_ch(8);
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
	unset_log_y();
	saveaspng(path + "SiPMs_form_by_Npe_lin");
	central_SiPMs(false); set_log_y();
	update();
	saveas(path + "SiPMs_edge_form_by_Npe");
	unset_log_y();
	saveaspng(path + "SiPMs_edge_form_by_Npe_lin");

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

	ty(AStates::PMT_tbNpe);
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
//Created on 2022.03.15. Finished on 20220.12.05.
//Re-analysed 20kV data using the same algorithm as 201015/S1_20kV and >2111.. data (pre-trigger and slow components are not treated separately)
data_output_path = "191107/results_v7/";
calibration_file = "191107/results_v5/191107_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::string iter_str = int_to_str((std::size_t)iteration, 2);

std::map<std::string, std::pair<double, double> > S2_times; //adsf - for fast Ctrl+F
S2_times["191107_U_20kV_850V_46V_12dB"] = std::pair<double, double> (90, 94.8);
S2_times["191107_U_18kV_850V_46V_12dB"] = std::pair<double, double> (89.9, 94.8);
S2_times["191107_U_16kV_850V_46V_12dB"] = std::pair<double, double> (89.7, 95.1);
S2_times["191107_U_14kV_850V_46V_12dB"] = std::pair<double, double> (89.6, 95.4);
S2_times["191107_U_12kV_850V_46V_12dB"] = std::pair<double, double> (89.2, 95.9);
S2_times["191107_U_10kV_850V_46V_12dB"] = std::pair<double, double> (88.9, 96.1);
S2_times["191107_U_8kV_850V_46V_12dB"] = std::pair<double, double> (88.1, 96.4);
S2_times["191107_U_6kV_850V_46V_12dB"] = std::pair<double, double> (87.4, 97.3);

std::map<std::string, std::pair<double, double> > S1_times;
S1_times["191107_U_20kV_850V_46V_12dB"] = std::pair<double, double> (62.8, 64.7);
S1_times["191107_U_18kV_850V_46V_12dB"] = std::pair<double, double> (61.5, 63.6);
S1_times["191107_U_16kV_850V_46V_12dB"] = std::pair<double, double> (59.9, 61.9);
S1_times["191107_U_14kV_850V_46V_12dB"] = std::pair<double, double> (57.5, 60.5);
S1_times["191107_U_12kV_850V_46V_12dB"] = std::pair<double, double> (54.7, 57.9);
S1_times["191107_U_10kV_850V_46V_12dB"] = std::pair<double, double> (50.9, 54.4);
S1_times["191107_U_8kV_850V_46V_12dB"] = std::pair<double, double> (44.6, 49.1);
S1_times["191107_U_6kV_850V_46V_12dB"] = std::pair<double, double> (34.3, 39.6);

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
if (calibrate) {
	std::cout<<"Warning! Use 5analysis_history.cpp for calibration! Doing nothing."<<std::endl;
	return;
}
//END OF CALIBRATION

//SIGNAL FORMS
if (!forms)
	return;
gStyle->SetOptStat("emr");
std::string folder, S2_start, S2_finish, S1_start, S1_finish;
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
	S2_start = dbl_to_str(d_S2_start, 1);
	S2_finish = dbl_to_str(d_S2_finish, 1);
} else {
  std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping building signal forms"<<std::endl;
  return;
}
auto S1_times_entry = S1_times.find(exp);
if (S1_times_entry != S1_times.end()) {
	d_S1_start = S1_times_entry->second.first;
	d_S1_finish = S1_times_entry->second.second;
	S1_start = dbl_to_str(d_S1_start, 1);
	S1_finish = dbl_to_str(d_S1_finish, 1);
} else {
  std::cout<<"Could not find S1 time limits for '"<<exp<<"'! Skipping building signal forms"<<std::endl;
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
if (exp == "191107_U_20kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	x_y_regions = {120, 0, 218, 36, 315, 66, 499, 216};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {100, 0, 183, 19, 299, 96};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(220, 269);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(220, 314);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "191107_U_18kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	x_y_regions = {89, 0, 162, 26, 256, 56, 288, 94, 499, 169};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {69, 0, 146, 19, 299, 124};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(169, 204);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(169, 243);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "191107_U_16kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {69, 0, 122, 25, 181, 46, 207, 82, 399, 164};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {57, 0, 107, 17, 199, 84};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(115, 145);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(115, 173);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "191107_U_14kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {39, 0, 122, 36, 159, 75, 299, 149};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {40, 0, 78, 14, 199, 115};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(76, 101);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(76, 125.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "191107_U_12kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {26, 0, 80, 27.5, 106, 61, 199, 132};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {30, 0, 199, 93};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(46, 64.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(46, 80.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "191107_U_10kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {12.9, 0, 41.4, 16.0, 74.0, 50.4, 199, 147};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {21, 0, 47, 9.4, 55.5, 31.7, 199, 157.5};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(23, 39.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(23, 53.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "191107_U_8kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {5.4, 0, 13.1, 2.8, 18.0, 3.2, 56.1, 39.0, 199, 155};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {15.5, 0, 33.7, 6.0, 43.0, 21.2, 199, 160};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(12, 23.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(12, 34.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "191107_U_6kV_850V_46V_12dB") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(0, 160, false, chan);
	}
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {4.9, 0, 10.7, 1.8, 17.4, 2.1, 46.0, 25.8, 199, 125};
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {15.3, 0, 33.4, 9.4, 38.0, 18.5, 199, 143};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Pu_left/";
ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(0, 160);
	draw_limits(7, 13.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Pu_peak/";
ty(AStates::MPPC_Npe_sum);
	draw_limits(7, 21.5);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_PMTs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_PMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	ty(AStates::MPPC_Npe_sum);
	unset_draw_limits();
	time_zoom_SiPMs(4, d_S1_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4.0-"+S1_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S1_start, d_S1_finish); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S1_start+"-"+S1_finish+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
