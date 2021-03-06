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
	//	PMT: 0 - 800V, 12dB
	//	SiPM: 0 - 50.0V (200K)
	if (channel==1) {
		cut_A_S_lower(0.01, 0.0011, 0.019, 0.0013, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.019, 0.0013, 1.1, 0.24, display, channel, "rem_largeA_smallS");
	}
	if (channel==2) {
		cut_A_S_lower(0.02, 0.00111, 0.075, 0.0056, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.075, 0.0056, 1.6, 0.26, display, channel, "rem_largeA_smallS");
	}
	if (channel==3) {
		cut_A_S_lower(0.013, 0.00080, 0.038, 0.0023, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.038, 0.0023, 1.2, 0.22, display, channel, "rem_largeA_smallS");
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
		cut_A_S_lower(0.077, 0.0, 1.28, 3.05, display, channel, "rem_smallA_smallS");
		cut_A_S_upper(0.077, 0.32, 0.186, 0.91, display, channel, "rem_smallA_largeS");
	}
	//qewr
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
}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2021.03.12
//Analysis of cryogenic temperature gaseous Ar alpha data with trigger cuts/adjustment
//Event cuts as 210302/2analysis_history.cpp and event selection as 210302/1analysis_history.cpp
data_output_path = "210311/results_v1/";
calibration_file = "210311/results_v1/210311_calibration.dat";
post_processor->calibr_info.Load(calibration_file);

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["210311_Pu_7.6kV_800V_50V_12dB_200K"] = std::pair<double, double> (26.4, 58.0);

std::map<std::string, std::string> exp_folders;
exp_folders["210311_Pu_7.6kV_800V_50V_12dB_200K"] = "Pu_50V_7.6kV_800V_200K";

std::map<std::string, double> pre_trigger_max_S; //for calibration
pre_trigger_max_S["210311_Pu_7.6kV_800V_50V_12dB_200K"] = 0.049;

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
double d_S_max;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //800V, 12 dB
int SiPM_state = 0; //50V
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
		S2_start = dbl_to_str(d_S2_start, 1);
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

ty(AStates::MPPC_S_sum); //nex();
time_zoom_SiPMs(0, d_S2_start);
set_zoom(0, 0.4);
set_bins(500); set_log_y();
draw_limits(0, d_S_max);
saveaspng(data_output_path + folder +"/calibration/SiPMs_pre-trigger_cut");
set_as_run_cut("small_pre-trigger");

for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) {
int channel = calib_channels[chi];
std::string ch_str = int_to_str(channel);
//parameters set for 210218_Pu_5.5kV_850V_0V_12dB_th2V, but valid for all fields.
//adsf
if (channel>=32) { //adsf
		if (getIndex(post_processor->MPPC_channels, channel)<0)
			continue;
ty(AStates::MPPC_A_S);
		ch(channel);
    set_zoom(0, 0.04, 0, 0.005); set_bins(1000);
    noise_cut(channel, 1, SiPM_state, true);
    saveaspng(data_output_path + folder + "/calibration/"+ch_str+"_A_S_1_zoom");
		set_log_z();
		saveaspng(data_output_path + folder + "/calibration/"+ch_str+"_A_S_2_zoom_log");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.015);
    set_bins(800);
    saveaspng(data_output_path + folder +"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 1, SiPM_state, false);
		cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0002, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveaspng(data_output_path + folder +"/calibration/"+ch_str+"_Ss_single_pe_w_0-"+S2_start+"us");
}
if (getIndex(post_processor->PMT_channels, channel)<0)
	continue;
if (channel==1) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.7, 0, 3);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.1, 0, 0.025);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
		set_bins(500);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(1500);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.04);
    set_bins(600);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.030);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==2) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.4, 0, 4);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.03);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
	 	set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(1500);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.07);
    set_bins(600);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.06);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==3) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.5, 0, 2.3);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
		set_zoom(0, 0.09, 0, 0.025);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(1500);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.04);
    set_bins(600);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.03);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==4) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.2, 0, 8);
    set_bins(2000);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.04);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(2000);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.08);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==5) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.08, 0, 0.006);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.03, 0, 0.0016);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(160);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.0012);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0010);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==6) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.2, 0, 0.01);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.06, 0, 0.004);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0013);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==7) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.04, 0, 0.003);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.02, 0, 0.001);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(80);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.001);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0007);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.2, 0, 0.018);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.05, 0, 0.0025);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0016);
    saveaspng(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
int PMT_state = 0; //800V, 12 dB
int SiPM_state = 0; //50V
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
if (exp == "210311_Pu_7.6kV_800V_50V_12dB_200K") {
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
	x_y_regions = {45.5, 0, 1499, 1293};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_zoom(200, 1000, 200, 1000);
	set_bins(800);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_zoom");
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 1000);
	set_bins(1000);
	x_y_regions = {67.8, 0, 999, 875};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

	ty(AStates::PMT_T_sum);
	ch(9);
	noise_cut(9, 0, PMT_state, 0);
	set_zoom(0, 15); set_bins(500);
	saveaspng(FOLDER + Num+"_trigger_Tsum_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_trigger_avg);
	TriggerAvgTData::SetTriggerType(TriggerAvgTData::tbNpe);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_zoom(40, 70); set_bins(1000);
	draw_limits(49.5, 55);
	saveaspng(FOLDER + Num+"_SiPMs_trigger_avg_"+cuts_str(cuts));
	set_as_run_cut("SiPMs_trigger"); cuts.push_back(Num);
	post_processor->set_trigger_offsets(52.1);
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Alpha_peak_v1/";
set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 1500, 0, 1500);
	set_bins(1500);
	set_titles("N_{pe} sPMTs", "N_{pe} SiPM-matrix");
	x_y_regions = {530, 379, 353, 501, 479, 683, 600, 518};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_peak_v2/";
	ty(AStates::Correlation);
	x_y_regions = {344, 583, 263, 732, 337, 846, 425, 724};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_vs_SiPMs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
