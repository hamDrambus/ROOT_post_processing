void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
std::string cuts_str(std::vector<int> cuts);
void central_SiPMs(bool on);
void fast_PMTs_only (void);
void fast_PMTs_WLS_only (void);
void slow_PMTs_only (void);
void slow_PMTs_WLS_only (void);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);

void time_zoom_fPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 8);
	cut_t(f_time, to_time, false, 9);
	cut_t(f_time, to_time, false, 10);
	cut_t(f_time, to_time, false, 11);
	cut_t(f_time, to_time, false, 12);
}

void time_zoom_sPMTs (double f_time, double to_time) {
	cut_t(f_time, to_time, false, 0);
	cut_t(f_time, to_time, false, 1);
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
	on_ch(8); on_ch(9); on_ch(10); on_ch(11);
	off_ch(0); off_ch(1); off_ch(12);
}

void fast_PMTs_WLS_only (void)
{
	on_ch(9); on_ch(10); on_ch(11);
	off_ch(0); off_ch(1); off_ch(8); off_ch(12);
}

void slow_PMTs_only (void) {
	on_ch(0); on_ch(1);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(12);
}

void slow_PMTs_WLS_only (void) {
	on_ch(0);
	off_ch(1); off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(12);
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
	ch(8);
	noise_cut(8, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "10_form_by_Npeaks");
  ch(11);
	noise_cut(11, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "12_form_by_Npeaks");

	ch(1);
	noise_cut(1, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "1_form_by_Npeaks");

	ty(AStates::PMT_tbNpe);
	ch(1);
	noise_cut(1, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "1_form_by_Npe");
	ch(8);
	noise_cut(8, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "8_form_by_Npe");
	ch(9);
	noise_cut(9, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "9_form_by_Npe");
  ch(10);
	noise_cut(10, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "10_form_by_Npe");
  ch(11);
	noise_cut(11, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "11_form_by_Npe");
	ch(12);
	noise_cut(12, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r); set_log_y();
	set_bins(Nbins);
	saveas(path + "12_form_by_Npe");

	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(1);
		noise_cut(1, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "1_form_by_S");
		ch(8);
		noise_cut(8, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "8_form_by_S");
		ch(9);
		noise_cut(9, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "9_form_by_S");
		ch(10);
		noise_cut(10, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "10_form_by_S");
		ch(11);
		noise_cut(11, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "11_form_by_S");
		ch(12);
		noise_cut(12, 1, PMT_condition, false);
		set_zoom(zoom_l, zoom_r); set_log_y();
		set_bins(Nbins);
		saveas(path + "12_form_by_S");

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
//Created on 2022.10.11
//New standard analysis. Redoing 180705 analysis to get number of photoelectrons
//Valid events and Pu peak events are selected from max Npe: SiPM matrix in this case
//Not using trigger adjustement or trigger-related cuts.
//Removed pre-trigger cut, using 2D cut on whole 0-160us range instead
//bacause previous approach (separate cut for pre-trigger region) may systematically increase or decrease long component.
//In order to determine its threshold more precisely, pre-trigger and long component
//areas must be treated in the same manner.
//For the same reason, Pu/Cd peaks are now selected using 0-160us. Otherwise, baseline (pre-trigger area of pulse-shape)
//for peak and low-E peak slope are diffent resulting in different long components.
//Same as 220922/7analysis_history.cpp

//Setup: ~18mm gap, gammas from 109Cd, 28% Electroconnect's THGEM used for cathode and THGEM1.
//61% transparency of THGEM0 for electrons!
//28% CERN's THGEM as THGEM0 with 4 MOhm, all PMTs with high gain
//With WLS before 3PMTs (ch0 and ch9,10,11). 1 atm pressures of argon in EL gap.

data_output_path = "180705/results_v7/";
calibration_file = data_output_path + "180705_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used
//adsf
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["180705_Cd_20kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_18kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_16kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_14kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_13kV_800V_12bB_48V"] = std::pair<double, double> (25, 41);
S2_times["180705_Cd_12kV_800V_6bB_48V"] = std::pair<double, double> (25, 41);
S2_times["180705_Cd_11kV_800V_6bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_10kV_800V_6bB_48V"] = std::pair<double, double> (25, 39);
S2_times["180705_Cd_9kV_800V_0bB_48V"] = std::pair<double, double> (25, 39);
S2_times["180705_Cd_8kV_800V_0bB_48V"] = std::pair<double, double> (25, 39);

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

std::map<std::string, double> pre_trigger_max_S; //for calibration only
pre_trigger_max_S["180705_Cd_20kV_800V_12bB_48V"] = 0.12;
pre_trigger_max_S["180705_Cd_18kV_800V_12bB_48V"] = 0.12;
pre_trigger_max_S["180705_Cd_16kV_800V_12bB_48V"] = 0.11;
pre_trigger_max_S["180705_Cd_14kV_800V_12bB_48V"] = 0.10;
pre_trigger_max_S["180705_Cd_13kV_800V_12bB_48V"] = 0.10;
pre_trigger_max_S["180705_Cd_12kV_800V_6bB_48V"] = 0.11;
pre_trigger_max_S["180705_Cd_11kV_800V_6bB_48V"] = 0.11;
pre_trigger_max_S["180705_Cd_10kV_800V_6bB_48V"] = 0.10;
pre_trigger_max_S["180705_Cd_9kV_800V_0bB_48V"] = 0.14;
pre_trigger_max_S["180705_Cd_8kV_800V_0bB_48V"] = 0.16;

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
double d_S_max;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //850V, 12 dB
int SiPM_state = 0; //46V
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
auto S_max_entry = pre_trigger_max_S.find(exp);
if (S_max_entry != pre_trigger_max_S.end()) {
    d_S_max = S_max_entry->second;
} else {
    std::cout<<"Could not find pre-trigger S limits for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
    nex();
    continue;
}

ty(AStates::PMT_S_sum); //nex();
slow_PMTs_only();
time_zoom_sPMTs(0, d_S2_start);
set_zoom(0, 2.5);
set_bins(500); set_log_y();
draw_limits(0, d_S_max);
saveaspng(data_output_path + "calibration/" + folder +"/sPMTs_pre-trigger_cut");
set_as_run_cut("small_pre-trigger");

for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) {
int channel = calib_channels[chi];
std::string ch_str = int_to_str(channel);
//parameters set by Pu_46V_20kV_850V, but valid for all fields.
if (channel>=32) {
		if (getIndex(post_processor->MPPC_channels, channel)<0)
			continue;
ty(AStates::MPPC_A_S);
		ch(channel);
		set_zoom(0, 0.10, 0, 0.015);
    set_bins(800);
    noise_cut(channel, 1, SiPM_state, true);
    saveaspng(data_output_path + "calibration/" + folder + "/"+ch_str+"_A_S_1_zoom");
		set_log_z();
		saveaspng(data_output_path + "calibration/" + folder + "/"+ch_str+"_A_S_2_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveaspng(data_output_path + "calibration/" + folder +"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveaspng(data_output_path + "calibration/" + folder +"/"+ch_str+"_Ss_2_single_pe_w_0-"+S2_start+"us");
}
if (channel==0) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.5, 0, 5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
		if (2 == PMT_state)
			set_zoom(0, 0.35, 0, 0.12);
		else
			set_zoom(0, 0.25, 0, 0.08);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
		set_bins(500);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
		unset_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_5_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.20);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
		if (0 == PMT_state)
	 		draw_limits(0.0015, 0.12);
		if (1 == PMT_state)
			draw_limits(0.0, 0.14);
		if (2 == PMT_state)
			draw_limits(0.0, 0.10);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==1) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.3, 0, 3.5);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0.0, 0.25, 0, 0.06);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
	 	set_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
		unset_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_5_zoom");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.2);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.12);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.65, 0, 0.020);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.15, 0, 0.01);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
		set_bins(400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==9) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.54, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.15, 0, 0.01);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==10) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.54, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.15, 0, 0.01);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==11) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.50, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.15, 0, 0.01);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==12) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.50, 0, 0.015);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.15, 0, 0.01);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_3_zoom_log");
    set_bins(400);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_A_S_4_zoom_log");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.005);
    set_bins(1000);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.002);
    saveaspng(data_output_path+"calibration/"+folder+"/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
int first_run = 0;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //850V, 12 dB
int SiPM_state = 0; //46 V
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
if (exp == "180705_Cd_20kV_800V_12bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_zoom(0, 3000);
	set_bins(1000);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 1200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 1200);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 3000, 0, 3000);
	set_bins(1000);
	x_y_regions = {50, 0, 2999, 2232};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {18, 0, 44, 12, 199, 127};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 1000, 0, 1000);
	set_bins(1000);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	set_bins(400);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	draw_limits(591, 676);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(591, 746);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	set_zoom(0, 3000);
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_18kV_800V_12bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_zoom(0, 3000);
	set_bins(1000);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 1000);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 1000);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 3000, 0, 3000);
	set_bins(1000);
	x_y_regions = {30, 0, 2999, 2314};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {13, 0, 38, 9.4, 199, 133};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 800, 0, 800);
	set_bins(800);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	set_bins(500);
	draw_limits(466, 559);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(466, 631);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	set_zoom(0, 2000);
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_16kV_800V_12bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_zoom(0, 1500);
	set_bins(500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 1000);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 1000);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 2000, 0, 2000);
	set_bins(1000);
	x_y_regions = {30, 0, 1999, 1579};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {13, 0, 31, 8.7, 199, 147};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 800, 0, 800);
	set_bins(800);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	set_bins(500);
	draw_limits(360, 440);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(360, 504);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	set_zoom(0, 1200);
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_14kV_800V_12bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_zoom(0, 1000);
	set_bins(500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 600);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 1000);
	set_bins(1000);
	x_y_regions = {20, 0, 999, 809};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {10, 0, 27.3, 8.4, 199, 150};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	set_bins(300);
	draw_limits(206, 273);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(206, 335);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_13kV_800V_12bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 600);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 1000);
	set_bins(1000);
	x_y_regions = {16.5, 0, 999, 851};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {11.8, 0, 199, 159};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	draw_limits(133, 196);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(133, 250);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_12kV_800V_6bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 400);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	x_y_regions = {12.3, 0, 599, 510};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {11.8, 0, 199, 159};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	draw_limits(81, 126);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(81, 165);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_11kV_800V_6bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 300);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {12.3, 0, 399, 332};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {11.8, 0, 199, 159};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	draw_limits(62, 87);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(62, 111);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_10kV_800V_6bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {10.6, 0, 299, 241};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {4.3, 0, 15.1, 2.2, 199, 161};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	draw_limits(31, 48);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(31, 63);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_9kV_800V_0bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {9.8, 0, 199, 165};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {4.3, 0, 13.6, 1.3, 199, 163};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	draw_limits(16, 25.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(16, 33.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "180705_Cd_8kV_800V_0bB_48V") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_sPMTs(0, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_fPMTs(0, 160);
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

ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_fPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {12.1, 0, 199, 173};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_log_z();
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {4.3, 0, 14.6, 1.3, 199, 170};
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

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{pe} PMTs", "N_{pe} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts));
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_log");
	unset_log_z();
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
  slow_PMTs_only();
	time_zoom_sPMTs(0, 160);
	draw_limits(10, 16.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(10, 22.5);
	saveaspng(FOLDER + Num + "_sPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	slow_PMTs_WLS_only();
	time_zoom_sPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_sPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_slow3PMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	fast_PMTs_WLS_only();
	time_zoom_fPMTs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_fPMTs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_fastPMTsWLS_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	time_zoom_SiPMs(4, d_S2_start); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_4-"+S2_start+"us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);
	time_zoom_SiPMs(d_S2_start, 160); update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+S2_start+"-160us_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
