void noise_cut(int channel, int aggressiveness, int device_condition, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void time_zoom_SiPMs (double f_time, double to_time);
void central_SiPMs(bool on);
void save_forms (std::string path, bool N_only, int PMT_condition, int SiPM_condition);
int save_forms (std::string prefix, std::string output_file, int Nbins, int type);

void time_zoom_fPMTs (double f_time, double to_time) {
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
	ch(0);
	noise_cut(0, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "0_form_by_Npeaks");
	ch(1);
	noise_cut(1, 1, PMT_condition, false);
	set_zoom(zoom_l, zoom_r);
	set_bins(Nbins);
	saveas(path + "1_form_by_Npeaks");
ty(AStates::PMT_tbNpe);
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

	if (!N_only) {
		ty(AStates::PMT_tbS);
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
	}
}

//Calculate S1 and S2 amplitudes for each calibrated channel
//Assumes that noise cuts and event selection are already done!
//Times must be in correct order (t1>t0)!
//Changes ty(AStates::PMT_Npe_sum) and ty(AStates::MPPC_Npe_sum) channels and time constraints!
void print_amplitudes(std::string file, double bkg_t0, double bkg_t1, double S1_t0, double S1_t1, double S2_t0, double S2_t1) {
	std::ofstream str;
	open_output_file(file, str, std::ios_base::trunc);
	if (!str.is_open())
		return;
	str<<"//channel\tbkg("<<dbl_to_str(bkg_t0, 1)<<"-"<<dbl_to_str(bkg_t1, 1)<<"us)\tS1("<<dbl_to_str(S1_t0, 1)<<"-"<<dbl_to_str(S1_t1, 1)
			<<"us)\tS2("<<dbl_to_str(S2_t0, 1)<<"-"<<dbl_to_str(S2_t1, 1)<<"us)\tS1-bkg\tS2-bkg (all in photoelectrons)"<<std::endl;
	for (std::size_t chi = 0, chi_end_ = calib_channels.size(); chi!=chi_end_; ++chi) {
		double bkg_Npe = 0, S1_Npe = 0, S2_Npe = 0;
		int channel = calib_channels[chi];
		std::string ch_str = int_to_str(channel);
		if (channel>=32) {
			if (getIndex(post_processor->MPPC_channels, channel)<0)
				continue;
			ty(AStates::MPPC_Npe_sum);
		} else {
			if (getIndex(post_processor->PMT_channels, channel)<0)
				continue;
			ty(AStates::PMT_Npe_sum);
		}
		ch(channel);
		cut_t(bkg_t0, bkg_t1, false, channel); update();
		bkg_Npe = get_mean();
		cut_t(S1_t0, S1_t1, false, channel); update();
		S1_Npe = get_mean();
		cut_t(S2_t0, S2_t1, false, channel); update();
		S2_Npe = get_mean();
		str<<channel<<"\t"<<bkg_Npe<<"\t"<<S1_Npe<<"\t"<<S2_Npe<<"\t"<<S1_Npe - bkg_Npe*(S1_t1-S1_t0)/(bkg_t1-bkg_t0)
				<<"\t"<<S2_Npe - bkg_Npe*(S2_t1-S2_t0)/(bkg_t1-bkg_t0)<<std::endl;
	}
	double bkg_Npe = 0, S1_Npe = 0, S2_Npe = 0;
	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		on_ch(chan);
		cut_t(bkg_t0, bkg_t1, false, chan);
	}
	update();
	bkg_Npe = get_mean();
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich)
		cut_t(S1_t0, S1_t1, false, post_processor->MPPC_channels[ich]);
	update();
	S1_Npe = get_mean();
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich)
		cut_t(S2_t0, S2_t1, false, post_processor->MPPC_channels[ich]);
	update();
	S2_Npe = get_mean();
	str<<"SiPMs"<<"\t"<<bkg_Npe<<"\t"<<S1_Npe<<"\t"<<S2_Npe<<"\t"<<S1_Npe - bkg_Npe*(S1_t1-S1_t0)/(bkg_t1-bkg_t0)
			<<"\t"<<S2_Npe - bkg_Npe*(S2_t1-S2_t0)/(bkg_t1-bkg_t0)<<std::endl;
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
//	PMT: 0 - 650V, 12dB
//	SiPM: 0 - 46V, 1 - 48V

//The following are set by hand for 20kV individually, there is no other way.
if (channel==32) {
	cut_A_S_lower(0.0105, 0.00058, 0.037, 0.0016, display, channel, "small_A-S_noise");
	x_y_regions = {0.0105, 0.0123, 0.0, 0.06, 0.0045, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0273, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0105, 0.00132, 0.0273, 0.00321, display, channel, "2pe_merged");
}
if (channel==33) {
	cut_A_S_lower(0.01, 0.00058, 0.03, 0.00127, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, 0.0, 0.06, 0.0046, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0215, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.0011, 0.0215, 0.0024, display, channel, "2pe_merged");
}
if (channel==34) {
	cut_A_S_lower(0.0115, 0.00085, 0.0292, 0.00085, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0002, 0.06, 0.0046, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0292, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.00146, 0.0292, 0.0033, display, channel, "2pe_merged");
}
if (channel==35) {
	cut_A_S_lower(0.0110, 0.0007, 0.0296, 0.00114, display, channel, "small_A-S_noise");
	x_y_regions = {0.0110, 0.0110, 0.0, 0.06, 0.0046, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0296, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0110, 0.00127, 0.0296, 0.0035, display, channel, "2pe_merged");
}
if (channel==36) {
	cut_A_S_lower(0.0112, 0.00060, 0.0280, 0.0013, display, channel, "small_A-S_noise");
	x_y_regions = {0.0112, 0.0112, 0.0, 0.06, 0.00483, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0280, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0112, 0.0012, 0.0280, 0.0037, display, channel, "2pe_merged");
}
if (channel==37) {
	cut_A_S_lower(0.0095, 0.00054, 0.0243, 0.00110, display, channel, "small_A-S_noise");
	x_y_regions = {0.0095, 0.0095, 0.0001, 0.06, 0.0048, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0243, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0095, 0.0010, 0.0243, 0.0031, display, channel, "2pe_merged");
}
if (channel==38) {
	cut_A_S_lower(0.012, 0.00077, 0.0307, 0.0016, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0, 0.06, 0.0047, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0307, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00127, 0.0307, 0.0042, display, channel, "2pe_merged");
}
if (channel==39) {
	cut_A_S_lower(0.01, 0.00062, 0.0269, 0.00120, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, -0.00013, 0.06, 0.0047, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0269, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.0011, 0.0269, 0.00376, display, channel, "2pe_merged");
}
if (channel==40) {
	cut_A_S_lower(0.0104, 0.00059, 0.0289, 0.00131, display, channel, "small_A-S_noise");
	x_y_regions = {0.0104, 0.0104, -0.00011, 0.06, 0.0045, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0289, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0104, 0.00132, 0.0289, 0.00364, display, channel, "2pe_merged");
}
if (channel==41) {
	cut_A_S_lower(0.0110, 0.00058, 0.0287, 0.00132, display, channel, "small_A-S_noise");
	x_y_regions = {0.0110, 0.0110, -0.00032, 0.06, 0.0044, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0287, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0110, 0.00135, 0.0287, 0.0038, display, channel, "2pe_merged");
}
if (channel==42) {
	cut_A_S_lower(0.0112, 0.00061, 0.0280, 0.00122, display, channel, "small_A-S_noise");
	x_y_regions = {0.0112, 0.0112, 0.0001, 0.06, 0.0046, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0280, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0112, 0.00122, 0.0280, 0.0034, display, channel, "2pe_merged");
}
if (channel == 43) {
	if (post_processor->isMultichannel(post_processor->current_type))
		off_ch(channel);
	cut_A_S_lower(0.0112, 0.00125, 0.028, 0.0, display, channel, "small_A-S_noise");
	x_y_regions = {0.0112, 0.0112, 0.0005, 0.06, 0.00580, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0112, 0.00167, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel == 44) {
	cut_A_S_lower(0.0105, 0.00070, 0.0224, 0.00111, display, channel, "small_A-S_noise");
	x_y_regions = {0.0105, 0.0105, 0.0, 0.06, 0.0050, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0224, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0105, 0.00135, 0.0224, 0.0030, display, channel, "2pe_merged");
}
if (channel==48) {
	cut_A_S_lower(0.0113, 0.00055, 0.0288, 0.00130, display, channel, "small_A-S_noise");
	x_y_regions = {0.0113, 0.0113, 0.0, 0.06, 0.0045, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0288, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0113, 0.00127, 0.0288, 0.00355, display, channel, "2pe_merged");
}
if (channel==49) {
	cut_A_S_lower(0.01, 0.00050, 0.0215, 0.00084, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.0086, -0.0001, 0.06, 0.0047, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0215, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00113, 0.0215, 0.0024, display, channel, "2pe_merged");
}
if (channel==50) {
	cut_A_S_lower(0.01, 0.00061, 0.0212, 0.00093, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, -0.0002, 0.06, 0.0049, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0212, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00108, 0.0212, 0.0025, display, channel, "2pe_merged");
}
if (channel==51) {
	cut_A_S_lower(0.01, 0.00063, 0.0275, 0.00127, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, -0.00013, 0.06, 0.0045, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0275, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.0012, 0.0275, 0.00337, display, channel, "2pe_merged");
}
if (channel==52) {
	cut_A_S_lower(0.0107, 0.00066, 0.0287, 0.0012, display, channel, "small_A-S_noise");
	x_y_regions = {0.0107, 0.0107, 0.0, 0.06, 0.0048, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0287, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0107, 0.00135, 0.0287, 0.0036, display, channel, "2pe_merged");
}
if (channel==53) {
	cut_A_S_lower(0.015, 0.0010, 0.031, 0.00167, display, channel, "small_A-S_noise");
	x_y_regions = {0.015, 0.015, 0.00037, 0.06, 0.0047, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.031, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.015, 0.0016, 0.031, 0.00414, display, channel, "2pe_merged");
}
if (channel==54) {
	cut_A_S_lower(0.01, 0.00061, 0.0248, 0.00106, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, -0.0002, 0.06, 0.0048, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0248, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00114, 0.0248, 0.00305, display, channel, "2pe_merged");
}
if (channel==55) {
	cut_A_S_lower(0.01, 0.00055, 0.0250, 0.00116, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, -0.0002, 0.06, 0.00488, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0250, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00120, 0.0250, 0.0029, display, channel, "2pe_merged");
}
if (channel==56) {
	cut_A_S_lower(0.012, 0.00080, 0.0282, 0.00143, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0001, 0.06, 0.00455, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0282, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00143, 0.0282, 0.0038, display, channel, "2pe_merged");
}
if (channel==57) {
	cut_A_S_lower(0.0107, 0.00063, 0.0282, 0.00114, display, channel, "small_A-S_noise");
	x_y_regions = {0.0107, 0.0107, -0.0001, 0.06, 0.00475, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0282, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0107, 0.0013, 0.0282, 0.0037, display, channel, "2pe_merged");
}
if (channel==58) {
	cut_A_S_lower(0.0103, 0.00061, 0.0280, 0.00127, display, channel, "small_A-S_noise");
	x_y_regions = {0.0103, 0.0103, -0.0001, 0.06, 0.0046, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0280, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0103, 0.00116, 0.0280, 0.00367, display, channel, "2pe_merged");
}
if (channel==59) {
	cut_A_S_lower(0.0118, 0.00070, 0.0282, 0.00126, display, channel, "small_A-S_noise");
	x_y_regions = {0.0118, 0.0118, -0.0001, 0.06, 0.0046, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0282, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0118, 0.00140, 0.0282, 0.00364, display, channel, "2pe_merged");
}

}

void analysis_history(bool calibrate, unsigned int iteration = 0) {
//Created on 2020.09.22
//iteration = 0 - no trigger adjustment.
//iteration > 0 - use fast PMTs for trigger_fit (byNpe for large fields/byNpeaks for low) with
//experimental pulse-shape determined from previous iteration
data_output_path = "190307_Xray/results_v1/";
calibration_file = "190307_Xray/results_v1/190307_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::string iter_str = int_to_str((std::size_t)iteration, 2);

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190307_X-ray_20kV_650V_46V_coll_6mm"] = std::pair<double, double> (42, 70);
S2_times["190307_X-ray_18kV_650V_46V_coll_6mm"] = std::pair<double, double> (42, 70);
S2_times["190307_X-ray_16kV_650V_46V_coll_6mm"] = std::pair<double, double> (42, 70);
S2_times["190307_X-ray_14kV_650V_46V_coll_6mm"] = std::pair<double, double> (42, 70);
S2_times["190307_X-ray_12kV_650V_46V_coll_6mm"] = std::pair<double, double> (42, 70);
S2_times["190307_X-ray_10kV_650V_46V_coll_6mm"] = std::pair<double, double> (42, 70);
S2_times["190307_X-ray_9kV_650V_46V_coll_6mm"] = std::pair<double, double> (42, 70);

std::map<std::string, std::string> exp_folders;
exp_folders["190307_X-ray_20kV_650V_46V_coll_6mm"] = "Xray_46V_20kV_650V";
exp_folders["190307_X-ray_18kV_650V_46V_coll_6mm"] = "Xray_46V_18kV_650V";
exp_folders["190307_X-ray_16kV_650V_46V_coll_6mm"] = "Xray_46V_16kV_650V";
exp_folders["190307_X-ray_14kV_650V_46V_coll_6mm"] = "Xray_46V_14kV_650V";
exp_folders["190307_X-ray_12kV_650V_46V_coll_6mm"] = "Xray_46V_12kV_650V";
exp_folders["190307_X-ray_10kV_650V_46V_coll_6mm"] = "Xray_46V_10kV_650V";
exp_folders["190307_X-ray_9kV_650V_46V_coll_6mm"] = "Xray_46V_9kV_650V";

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
		cut_t(0, d_S2_start, false, channel);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    noise_cut(channel, 1, SiPM_state, false);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_single_pe_w_0-"+S2_start+"us");
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

if (true) { //do the same for all experiments
	std::string cuts = "no_cuts";
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

std::string form_n = "forms_X-ray/";
save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}
//END OF FORMS
}
