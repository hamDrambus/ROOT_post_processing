void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_PMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
void time_zoom_PMTs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e);
void time_zoom_SiPMs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e);
std::string cuts_str(std::vector<int> cuts);
void central_SiPMs(bool on);
void fast_PMTs_only (void);
void slow_PMTs_only (void);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);

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

void time_zoom_PMTs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e) {
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 0);
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 1);
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 2);
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 3);
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 4);

	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 5);
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 6);
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 7);
	cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, 8);
}

void time_zoom_SiPMs (double f_time, double to_time) {
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		cut_t(f_time, to_time, false, chan);
	}
}

void time_zoom_SiPMs_S1_S2 (double S1_s, double S1_e, double S2_s, double S2_e) {
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		cut_t_S1_S2(S1_s, S1_e, S2_s, S2_e, false, chan);
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

//call from x_script.cpp after analysis is finished
void save_SiPM_Npe_table(std::string fname) {
	std::ofstream str;
	str.open(fname, std::ios_base::trunc);
	if (!str.is_open())
		return;
	str<<"V\t";
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		str<<chan<<"\t";
	}
	str<<std::endl;

	for (std::size_t i = 0, i_end_ = exp_area.experiments.size(); i != i_end_; ++i) {
		double V = experiment_fields[exp_area.experiments[i]];
		str<<V<<"\t";
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			SiPM_Npe_data data = (*gSiPM_Npe_data.info(chan))[i];
			double S1_no_noise = data.Npe_S1 - data.Npe_pre_trigger * (data.t_S1.second - data.t_S1.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
			double S2_no_noise = data.Npe_S2 - data.Npe_pre_trigger * (data.t_S2.second - data.t_S2.first) / (data.t_pre_trigger.second - data.t_pre_trigger.first);
			str<<S2_no_noise<<"\t";
		}
		str<<std::endl;
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
	if (0 == device_condition) {
		if (channel==1) {
			//cut_A_S_upper(0.005, 0.0, 0.0132, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.020, 0.0019, 0.06, 0.0054, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.06, 0.0054, 1.4, 0.25, display, channel, "rem_largeA_smallS");
		}
		if (channel==2) {
			//cut_A_S_upper(0.005, 0.0, 0.023, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.0122, 0.0014, 0.055, 0.0036, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.055, 0.0036, 1.6, 0.27, display, channel, "rem_largeA_smallS");
		}
		if (channel==3) {
			//cut_A_S_upper(0.005, 0.0, 0.0139, 0.0, display, channel, "rem_smallA_largeS");
			cut_A_S_lower(0.01, 0.0017, 0.030, 0.0025, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.030, 0.0025, 1.2, 0.14, display, channel, "rem_largeA_smallS");
		}
		if (channel==4) {
			//cut_A_S_upper(0.01, 0.0, 0.0171, 0.0, display, channel, "rem_smallA_largeS_1");
			cut_A_S_lower(0.01, 0.0016, 0.055, 0.0037, display, channel, "rem_smallA_smallS");
			cut_A_S_lower(0.055, 0.0037, 1.6, 0.16, display, channel, "rem_largeA_smallS");
		}
		if (channel==5) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.65, 0, 5, 0, display, channel, "rem_A>0.65");
				cut_A_S_upper(0.0, 0.030, 1.6, 0.030, display, channel, "rem_S>0.030");
			}
		}
		if (channel==6) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.50, 0, 5, 0, display, channel, "rem_A>0.50");
				cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
			}
		}
		if (channel==7) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.54, 0, 2, 0, display, channel, "rem_A>0.54");
				cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
			}
		}
		if (channel==8) {
			if (aggressiveness == 1) {
				cut_A_S_upper(0.54, 0, 2, 0, display, channel, "rem_A>0.54");
				cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
			}
		}
	}
	//qewr
	//The following are set by hand for 20kV individually, there is no other way.
	if (channel==32) {
		cut_A_S_lower(0.0062, 0.00106, 0.02, 0.00106, display, channel, "small_A-S_noise");
		x_y_regions = {0.0062, 0.0062, 0.00061, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0147, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0062, 0.00163, 0.0147, 0.00325, display, channel, "2pe_merged");
	}
	if (channel==33) {
		cut_A_S_lower(0.0056, 0.00101, 0.02, 0.00101, display, channel, "small_A-S_noise");
		x_y_regions = {0.0056, 0.0056, 0.00072, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0127, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0056, 0.00155, 0.0127, 0.00290, display, channel, "2pe_merged");
	}
	if (channel==34) {
		cut_A_S_lower(0.0071, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
		x_y_regions = {0.0071, 0.0071, 0.00072, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0162, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0071, 0.00183, 0.0162, 0.00348, display, channel, "2pe_merged");
	}
	if (channel==35) {
		cut_A_S_lower(0.0069, 0.00115, 0.02, 0.00115, display, channel, "small_A-S_noise");
		x_y_regions = {0.0069, 0.0069, 0.00083, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0069, 0.00181, 0.0159, 0.00356, display, channel, "2pe_merged");
	}
	if (channel==36) {
		cut_A_S_lower(0.0074, 0.00132, 0.02, 0.00132, display, channel, "small_A-S_noise");
		x_y_regions = {0.0074, 0.0074, 0.00084, 0.05, 0.0092, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0162, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0074, 0.00191, 0.0162, 0.00366, display, channel, "2pe_merged");
	}
	if (channel==37) {
		cut_A_S_lower(0.0059, 0.00102, 0.02, 0.00102, display, channel, "small_A-S_noise");
		x_y_regions = {0.0059, 0.0059, 0.00076, 0.05, 0.0094, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0141, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0059, 0.00162, 0.0141, 0.00311, display, channel, "2pe_merged");
	}
	if (channel==38) {
		cut_A_S_lower(0.0069, 0.00111, 0.02, 0.00111, display, channel, "small_A-S_noise");
		x_y_regions = {0.0069, 0.0069, 0.00040, 0.05, 0.0091, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0169, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0069, 0.00181, 0.0169, 0.00395, display, channel, "2pe_merged");
	}
	if (channel==39) {
		cut_A_S_lower(0.0062, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
		x_y_regions = {0.0062, 0.0066, 0.00060, 0.05, 0.0091, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0150, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0062, 0.00179, 0.0150, 0.00333, display, channel, "2pe_merged");
	}
	if (channel==40) {
		cut_A_S_lower(0.0066, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.0066, 0.00052, 0.05, 0.0089, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0154, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00191, 0.0154, 0.00358, display, channel, "2pe_merged");
	}
	if (channel==41) {
		cut_A_S_lower(0.0064, 0.00089, 0.02, 0.00089, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00055, 0.05, 0.0089, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0153, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00191, 0.0153, 0.00337, display, channel, "2pe_merged");
	}
	if (channel==42) {
		cut_A_S_lower(0.0064, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0152, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00191, 0.0152, 0.00340, display, channel, "2pe_merged");
	}
	if (channel==43) {
		cut_A_S_lower(0.0064, 0.00106, 0.02, 0.00106, display, channel, "small_A-S_noise");
		x_y_regions = {0.0064, 0.0064, 0.00060, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0155, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0064, 0.00191, 0.0155, 0.00353, display, channel, "2pe_merged");
	}
	if (channel==44) {
		cut_A_S_lower(0.0055, 0.00102, 0.02, 0.00102, display, channel, "small_A-S_noise");
		x_y_regions = {0.0055, 0.0055, 0.00058, 0.05, 0.0083, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0140, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0055, 0.00165, 0.0140, 0.00303, display, channel, "2pe_merged");
		//if (post_processor->isMultichannel(post_processor->current_type)) {
		//	off_ch(channel);
		//}
	}
	if (channel==48) {
		cut_A_S_lower(0.0070, 0.00123, 0.02, 0.00123, display, channel, "small_A-S_noise");
		x_y_regions = {0.0070, 0.0070, 0.00091, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0154, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0070, 0.00171, 0.0154, 0.00332, display, channel, "2pe_merged");
	}
	if (channel==49) {
		cut_A_S_lower(0.0055, 0.00097, 0.02, 0.00097, display, channel, "small_A-S_noise");
		x_y_regions = {0.0055, 0.0055, 0.00058, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0121, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0055, 0.00146, 0.0121, 0.00275, display, channel, "2pe_merged");
	}
	if (channel==50) {
		cut_A_S_lower(0.0049, 0.00082, 0.02, 0.00082, display, channel, "small_A-S_noise");
		x_y_regions = {0.0049, 0.0053, 0.00058, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0123, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0049, 0.00142, 0.0123, 0.00280, display, channel, "2pe_merged");
	}
	if (channel==51) {
		cut_A_S_lower(0.0060, 0.00100, 0.02, 0.00100, display, channel, "small_A-S_noise");
		x_y_regions = {0.0060, 0.0060, 0.00050, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0144, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0060, 0.00162, 0.0144, 0.00329, display, channel, "2pe_merged");
	}
	if (channel==52) {
		cut_A_S_lower(0.0071, 0.00121, 0.02, 0.00121, display, channel, "small_A-S_noise");
		x_y_regions = {0.0071, 0.0071, 0.00081, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0164, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0071, 0.00186, 0.0164, 0.00371, display, channel, "2pe_merged");
	}
	if (channel==53) {
		cut_A_S_lower(0.0072, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
		x_y_regions = {0.0072, 0.0072, 0.00058, 0.05, 0.0081, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0172, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0072, 0.00194, 0.0172, 0.00372, display, channel, "2pe_merged");
	}
	if (channel==54) {
		cut_A_S_lower(0.0061, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
		x_y_regions = {0.0061, 0.0061, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0141, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0061, 0.00160, 0.0141, 0.00316, display, channel, "2pe_merged");
	}
	if (channel==55) {
		cut_A_S_lower(0.0061, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
		x_y_regions = {0.0061, 0.0061, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0061, 0.00164, 0.0145, 0.00320, display, channel, "2pe_merged");
	}
	if (channel==56) {
		cut_A_S_lower(0.0065, 0.00107, 0.02, 0.00107, display, channel, "small_A-S_noise");
		x_y_regions = {0.0065, 0.0065, 0.00047, 0.05, 0.0088, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0150, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0065, 0.00181, 0.0150, 0.00340, display, channel, "2pe_merged");
	}
	if (channel==57) {
		cut_A_S_lower(0.0069, 0.00120, 0.02, 0.00120, display, channel, "small_A-S_noise");
		x_y_regions = {0.0069, 0.0069, 0.00066, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0159, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0069, 0.00183, 0.0159, 0.00348, display, channel, "2pe_merged");
	}
	if (channel==58) {
		cut_A_S_lower(0.0066, 0.00108, 0.02, 0.00108, display, channel, "small_A-S_noise");
		x_y_regions = {0.0066, 0.0066, 0.00058, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0153, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0066, 0.00180, 0.0153, 0.00333, display, channel, "2pe_merged");
	}
	if (channel==59) {
		cut_A_S_lower(0.0067, 0.00118, 0.02, 0.00118, display, channel, "small_A-S_noise");
		x_y_regions = {0.0067, 0.0067, 0.00057, 0.05, 0.0090, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
		cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
		if (aggressiveness>=1)//select only 1 photoelectron for calibration
			cut_A_S_upper(0.0157, 0.0001, 1, 0.0001, display, channel, "2pe");
		if (aggressiveness>=2)//remove afterimpulses
			cut_A_S_upper(0.0067, 0.00180, 0.0157, 0.00340, display, channel, "2pe_merged");
	}

}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2022.12.16
//New standard-ish analysis for NBrS in LAr measurements
//Valid events and Pu peak events are selected from both SiPM matrix and 4 sPMTs
//Not using trigger adjustement or trigger-related cuts.
//Removed pre-trigger cut, using 2D cut on whole 0-160us range instead vs S1+S2 range
//Also cut on S1 vs S2 is present.
//bacause previous approach (separate cut for pre-trigger region) may systematically increase or decrease baseline.
//Thus all non-signal areas must be treated in the same manner.
//For the same reason, Pu/Cd peaks are now selected using 0-160us.
//Based on 220922/7analysis_history.cpp

//1analysis_history.cpp was to get S2 in LAr photoelectrons and pulse-shape
//This script is to study X-Y S2 (gas) distribution, light response function and source position.

//Setup: ~6mm gap, alphas from 238Pu, 75% Electroconnect's THGEM used for cathode and THGEM1,
//28% CERN's THGEM as THGEM0 with 260 MOhm, all PMTs with high gain (3 old PMTs which we in use
//till ~ May 2019 and new PMTs as #4). 240 MOhm part of V0 divider is inside EL gap, covers PMT#2.
//All PMTs have WLS. 1.0 atm pressures of argon in EL gap.

data_output_path = "221020/results_v2/";
calibration_file = "221020/results_v1/221020_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times; //adsf
S2_times["221020_S2_LAr_Pu_WLS_18.5kV_0V"] = std::pair<double, double> (85.0, 90.0);
S2_times["221020_S2_LAr_Pu_WLS_18.0kV_338V"] = std::pair<double, double> (85.0, 90.0);
S2_times["221020_S2_LAr_Pu_WLS_17.5kV_563V"] = std::pair<double, double> (85.4, 90.5);
S2_times["221020_S2_LAr_Pu_WLS_17.0kV_917V"] = std::pair<double, double> (85.5, 90.5);
S2_times["221020_S2_LAr_Pu_WLS_16.5kV_1238V"] = std::pair<double, double> (85.7, 90.6);
S2_times["221020_S2_LAr_Pu_WLS_16.0kV_1238V"] = std::pair<double, double> (86.0, 91.0);
S2_times["221020_S2_LAr_Pu_WLS_15.5kV_1238V"] = std::pair<double, double> (86.3, 91.3);
S2_times["221020_S2_LAr_Pu_WLS_15.0kV_1238V"] = std::pair<double, double> (86.7, 91.3);
S2_times["221020_S2_LAr_Pu_WLS_14.0kV_1688V"] = std::pair<double, double> (87.3, 92.0);
S2_times["221020_S2_LAr_Pu_WLS_13.0kV_2250V"] = std::pair<double, double> (88.0, 92.6);
S2_times["221020_S2_LAr_Pu_WLS_12.0kV_2250V"] = std::pair<double, double> (89.2, 94.0);
S2_times["221020_S2_LAr_Pu_WLS_11.0kV_2025V"] = std::pair<double, double> (90.3, 95.2);

std::map<std::string, std::pair<double, double> > S2_times_for_XY;
S2_times_for_XY["221020_S2_LAr_Pu_WLS_18.5kV_0V"] = std::pair<double, double> (85.0, 100);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_18.0kV_338V"] = std::pair<double, double> (85.0, 100);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_17.5kV_563V"] = std::pair<double, double> (85.4, 100);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_17.0kV_917V"] = std::pair<double, double> (85.5, 100);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_16.5kV_1238V"] = std::pair<double, double> (85.7, 100);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_16.0kV_1238V"] = std::pair<double, double> (86.0, 101);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_15.5kV_1238V"] = std::pair<double, double> (86.3, 102);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_15.0kV_1238V"] = std::pair<double, double> (86.7, 102);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_14.0kV_1688V"] = std::pair<double, double> (87.3, 105);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_13.0kV_2250V"] = std::pair<double, double> (88.0, 104);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_12.0kV_2250V"] = std::pair<double, double> (89.2, 110);
S2_times_for_XY["221020_S2_LAr_Pu_WLS_11.0kV_2025V"] = std::pair<double, double> (90.3, 116);

std::map<std::string, std::pair<double, double> > S1_times;
S1_times["221020_S2_LAr_Pu_WLS_18.5kV_0V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_18.0kV_338V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_17.5kV_563V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_17.0kV_917V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_16.5kV_1238V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_16.0kV_1238V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_15.5kV_1238V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_15.0kV_1238V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_14.0kV_1688V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_13.0kV_2250V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_12.0kV_2250V"] = std::pair<double, double> (58, 63.5);
S1_times["221020_S2_LAr_Pu_WLS_11.0kV_2025V"] = std::pair<double, double> (58, 63.5);

std::map<std::string, std::pair<double, double> > Bkg_times;
Bkg_times["221020_S2_LAr_Pu_WLS_18.5kV_0V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_18.0kV_338V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_17.5kV_563V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_17.0kV_917V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_16.5kV_1238V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_16.0kV_1238V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_15.5kV_1238V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_15.0kV_1238V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_14.0kV_1688V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_13.0kV_2250V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_12.0kV_2250V"] = std::pair<double, double> (10, 55);
Bkg_times["221020_S2_LAr_Pu_WLS_11.0kV_2025V"] = std::pair<double, double> (10, 55);

std::map<std::string, std::string> exp_folders;
exp_folders["221020_S2_LAr_Pu_WLS_18.5kV_0V"] = "Pu_18.5kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_18.0kV_338V"] = "Pu_18.0kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_17.5kV_563V"] = "Pu_17.5kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_17.0kV_917V"] = "Pu_17.0kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_16.5kV_1238V"] = "Pu_16.5kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_16.0kV_1238V"] = "Pu_16.0kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_15.5kV_1238V"] = "Pu_15.5kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_15.0kV_1238V"] = "Pu_15.0kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_14.0kV_1688V"] = "Pu_14.0kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_13.0kV_2250V"] = "Pu_13.0kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_12.0kV_2250V"] = "Pu_12.0kV_46V_850V";
exp_folders["221020_S2_LAr_Pu_WLS_11.0kV_2025V"] = "Pu_11.0kV_46V_850V";

std::map<std::string, double> pre_trigger_max_S; //for calibration only
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_18.5kV_0V"] = 0.70;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_18.0kV_338V"] = 0.70;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_17.5kV_563V"] = 0.73;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_17.0kV_917V"] = 0.65;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_16.5kV_1238V"] = 0.78;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_16.0kV_1238V"] = 0.36;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_15.5kV_1238V"] = 0.40;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_15.0kV_1238V"] = 0.54;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_14.0kV_1688V"] = 0.23;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_13.0kV_2250V"] = 0.21;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_12.0kV_2250V"] = 0.30;
pre_trigger_max_S["221020_S2_LAr_Pu_WLS_11.0kV_2025V"] = 0.30;

bool forms = !calibrate;
if (calibrate) {
	std::cout<<"Use 5analysis_history.cpp for calibration! Doing nothing."<<std::endl;
}

//SIGNAL FORMS
if (!forms)
	return;
std::string folder, S2_start, S2_finish, S1_start, S1_finish, S2_XY_start, S2_XY_finish, Bkg_start, Bkg_finish;
double d_S2_XY_start, d_S2_XY_finish, d_Bkg_start, d_Bkg_finish;
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
    d_S2_start = S2_times_entry->second.first; d_S2_finish = S2_times_entry->second.second;
		S2_start = dbl_to_str(d_S2_start, 1); S2_finish = dbl_to_str(d_S2_finish, 1);
} else {
    std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping this experiment"<<std::endl;
    return;
}
auto S1_times_entry = S1_times.find(exp);
if (S1_times_entry != S1_times.end()) {
    d_S1_start = S1_times_entry->second.first; d_S1_finish = S1_times_entry->second.second;
		S1_start = dbl_to_str(d_S1_start, 1); S1_finish = dbl_to_str(d_S1_finish, 1);
} else {
    std::cout<<"Could not find S1 time limits for '"<<exp<<"'! Skipping this experiment"<<std::endl;
    return;
}
auto S2_XY_times_entry = S2_times_for_XY.find(exp);
if (S2_XY_times_entry != S2_times_for_XY.end()) {
    d_S2_XY_start = S2_XY_times_entry->second.first; d_S2_XY_finish = S2_XY_times_entry->second.second;
		S2_XY_start = dbl_to_str(d_S2_XY_start, 1); S2_XY_finish = dbl_to_str(d_S2_XY_finish, 1);
} else {
    std::cout<<"Could not find S2 XY time limits for '"<<exp<<"'! Skipping this experiment"<<std::endl;
  return;
}
auto Bkg_times_entry = Bkg_times.find(exp);
if (Bkg_times_entry != Bkg_times.end()) {
    d_Bkg_start = Bkg_times_entry->second.first; d_Bkg_finish = Bkg_times_entry->second.second;
		Bkg_start = dbl_to_str(d_Bkg_start, 1); Bkg_finish = dbl_to_str(d_Bkg_finish, 1);
} else {
    std::cout<<"Could not find Background time limits for '"<<exp<<"'! Skipping this experiment"<<std::endl;
  return;
}
auto first_run_entry = experiment_runs.find(exp);
if (first_run_entry != experiment_runs.end())
    first_run = first_run_entry->second;
else {
    std::cout<<"Warning: Could not find starting run index for '"<<exp<<"'! Will print invalid event indexes."<<std::endl;
    first_run = -10000;
}
// These lambda functions are to shorten the code only.
auto print_SiPMs_xy = [=](std::string& FOLDER, std::string& Num, int& no, std::vector<std::string>& cuts) {
	ty(AStates::MPPC_coord);
		time_zoom_SiPMs(d_S2_XY_start, d_S2_XY_finish);
		set_zoom(-25, 25, -25, 25); set_bins(180);
		update();
		saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_"+S2_XY_start+"-"+S2_XY_finish+"us");
		Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_profile_x);
		time_zoom_SiPMs(d_S2_XY_start, d_S2_XY_finish);
		update();
		saveaspng(FOLDER + Num + "_SiPMs_S2_x_profile_"+cuts_str(cuts)+"_"+S2_XY_start+"-"+S2_XY_finish+"us");
		Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_profile_y);
		time_zoom_SiPMs(d_S2_XY_start, d_S2_XY_finish);
		update();
		saveaspng(FOLDER + Num + "_SiPMs_S2_y_profile_"+cuts_str(cuts)+"_"+S2_XY_start+"-"+S2_XY_finish+"us");
		Num = int_to_str(++no, 2);

	ty(AStates::MPPC_coord);
		x_y_regions = {-2, -2, 2, -2, 2, 2, -2, 2};
		cut_x_y_poly_select(x_y_regions, true, "1");
		update();
		saveaspng(FOLDER + Num + "_SiPMs_xy_"+cuts_str(cuts)+"_"+S2_XY_start+"-"+S2_XY_finish+"us");
		set_as_run_cut("Central"); cuts.push_back(Num);
		print_accepted_events(FOLDER + Num + "_central_events.txt", first_run);
		Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_profile_x);
		time_zoom_SiPMs(d_S2_XY_start, d_S2_XY_finish);
		update();
		saveaspng(FOLDER + Num + "_SiPMs_S2_x_profile_"+cuts_str(cuts)+"_"+S2_XY_start+"-"+S2_XY_finish+"us");
		Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_profile_y);
		time_zoom_SiPMs(d_S2_XY_start, d_S2_XY_finish);
		update();
		saveaspng(FOLDER + Num + "_SiPMs_S2_y_profile_"+cuts_str(cuts)+"_"+S2_XY_start+"-"+S2_XY_finish+"us");
		Num = int_to_str(++no, 2);
};
//zcxv
if (exp == "221020_S2_LAr_Pu_WLS_18.5kV_0V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {58.7, 0, 185.5, 88.8, 399, 222};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {25.8, 0, 57.7, 21.4, 199, 145};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 80, 9, 80, 52, 16.3, 52, 0, 13.0};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {97.4, 13.6, 149, 56, 111, 109, 62, 62};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_18.0kV_338V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {55.2, 0, 141.9, 47.9, 399, 219};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {25.8, 0, 64.2, 24.0, 199, 137};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 80, 9, 80, 52, 16.3, 52, 0, 13.0};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {113.5, 15.0, 160.9, 50.0, 119.9, 111.8, 77.1, 71.0};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_17.5kV_563V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {54.2, 0, 133.4, 42.7, 399, 231};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {27.1, 0, 68.9, 25.6, 199, 142};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 80, 9, 80, 44, 14.3, 44, 0, 15.2};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {117.3, 19.9, 167.3, 46.6, 123.7, 113.8, 81.2, 73.9};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_17.0kV_917V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {54.2, 0, 123.3, 34.3, 399, 236};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {21.3, 0, 70.2, 28.5, 199, 157};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 80, 9, 80, 44, 14.3, 44, 0, 15.2};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {109.0, 12.1, 166.5, 36.9, 129.3, 109.4, 81.2, 69.5};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_16.5kV_1238V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	x_y_regions = {54.2, 0, 142.9, 40.1, 399, 237};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {28.6, 0, 70.4, 28.2, 199, 158};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 80, 9, 80, 44, 14.3, 44, 0, 15.2};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {121.0, 17.0, 183.1, 43.2, 145.5, 115.2, 93.2, 73.9};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_16.0kV_1238V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {20.4, 0, 94.0, 40.3, 299, 194};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {14.3, 0, 52.4, 23.5, 199, 161.4};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 75, 9, 75, 48, 11.6, 48, 0, 19.1};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {75.6, 4.8, 130.1, 24.7, 101.9, 95.8, 52.7, 57.8};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_15.5kV_1238V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {20.4, 0, 90.6, 37.4, 299, 194};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {14.3, 0, 44.6, 18.4, 199, 167};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 65, 9, 65, 48, 11.6, 48, 0, 20.7};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {69.2, 2.9, 123.8, 22.7, 95.5, 93.7, 46.4, 55.7};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_15.0kV_1238V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {60.2, 0, 299, 175};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {15.8, 0, 42.1, 14.3, 199, 164};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {0, 9, 50, 9, 50, 48, 11.6, 48, 0, 21.0};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {69.2, 2.9, 123.8, 22.7, 95.5, 93.7, 46.4, 55.7};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_14.0kV_1688V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {11.3, 0, 63.6, 30.1, 299, 227};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {15.8, 0, 38.9, 17.5, 199, 175};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {9.6, 8, 48, 8, 48, 51, 2, 51, 2, 18.5};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {50.1, 2.6, 99.5, 14.9, 70.9, 74.9, 30.4, 43.1};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_13.0kV_2250V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {11.3, 0, 63.6, 27.7, 299, 227};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {15.1, 0, 38.4, 17.5, 199, 175};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {12.8, 8, 48, 8, 48, 51, 2, 51, 2, 22.3};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {50.0, 2.6, 95.2, 13.3, 74.2, 77.5, 30.1, 43.4};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_12.0kV_2250V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {14.7, 0, 54.5, 26.7, 299, 236};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {10.5, 0, 30.6, 13.6, 199, 176};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {25.8, 8, 46, 8, 46, 58, 2, 58, 2, 22.7};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {49.6, 2.2, 86.5, 10.7, 75.2, 62.9, 32.4, 40.2};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
if (exp == "221020_S2_LAr_Pu_WLS_11.0kV_2025V") {
	std::vector<std::string> cuts;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	ty(AStates::PMT_Npe_sum);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts)+"_w_0-160us");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	cut_PMT_noise(PMT_state);
	time_zoom_PMTs(0, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts)+"_w_0-160us");
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {15.1, 0, 44.0, 19.9, 299, 236};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::MPPC_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_SiPMs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {10.8, 0, 23.8, 7.7, 199, 166};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts_str(cuts)+"_S1_S2_vs_all_log");
	unset_log_z();
	set_as_run_cut("good_SiPMs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_x);
	time_zoom_PMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation_y);
	time_zoom_PMTs(d_S1_start, d_S1_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {19.6, 9, 34, 9, 34, 58, 1, 58, 1, 21.7};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} S2, t#in["+S2_start+", "+S2_finish+"] #mus", "N_{PE} S1, t#in["+S1_start+", "+S1_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2");
	set_log_z();
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts_str(cuts)+"_S1_vs_S2_log");
	unset_log_z();
	set_as_run_cut("good_PMTs_S1_S2"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_Npe_sum, AStates::MPPC_Npe_sum, -1, -1);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	x_y_regions = {40.6, 0.9, 74.7, 9.4, 65.6, 57.0, 25.8, 35.3};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	set_titles("N_{PE} PMTs", "N_{PE} SiPM-matrix");
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_log_z();
	saveaspng(FOLDER + Num + "_SiPMs_vs_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events_log");
	unset_log_z();

	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + Num + "_events.txt", first_run);
	Num = int_to_str(++no, 2);

	print_SiPMs_xy(FOLDER, Num, no, cuts);
}
//END OF FORMS
}
