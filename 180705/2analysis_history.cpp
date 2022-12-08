void noise_cut(int channel, int aggressiveness, bool display);
void time_zoom (double f_time, double to_time);
void central_SiPMs(bool on);
void save_forms (std::string path, bool N_only);

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

void save_forms (std::string path, bool N_only)
{
	ty(AStates::MPPC_tbNpe_sum);
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
	saveas(path + "12_form_by_Npeaks");

	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "1_form_by_Npe");

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

		ch(1);
		noise_cut(1, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "1_form_by_S");
	}
}

//Even it is hard to read the whole analysis, it is better to move all A-S cuts for PMTs in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
if (channel==0) {
    if (aggressiveness ==1) {
    	cut_A_S_upper(1.4, 0, 1e2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 0.55, 1.4, 1.1, display, channel, "rem_S>~0.7");
	}
}
if (channel==1) {
	if (aggressiveness ==1) {
    	cut_A_S_upper(1.4, 0, 1e2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 1.2, 0.7, 1.5, display, channel, "rem_S>~1.3");
	}
}
if (channel==12) {
    x_y_regions = {0.0, 0.007, -0.000025, 0.05, 0.0007, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness ==1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 0.4, 0.1, display, channel, "rem_S>0.1");
	}
}
if (channel==11) {
    x_y_regions = {0.0, 0.007, 0.0, 0.05, 0.0007, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness ==1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 0.4, 0.1, display, channel, "rem_S>0.1");
	}
}
if (channel==10) {
    x_y_regions = {0.0, 0.006, 0.0, 0.05, 0.00074, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness ==1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 0.4, 0.1, display, channel, "rem_S>0.1");
	}
}
if (channel==9) {
    x_y_regions = {0.0, 0.006, 0.0, 0.05, 0.00074, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness ==1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 1, 0.4, display, channel, "rem_S>0.1");
	}
}
if (channel==8) {
	cut_A_S_upper(0.002, 0.004, 0.0046, 0.004, display, channel, "small_A-S_noise");
	if (aggressiveness ==1) {
		cut_A_S_upper(0.1, 0, 2, 0, display, channel, "rem_A>0.1");
		cut_A_S_upper(0.0, 0.05, 0.1, 0.05, display, channel, "rem_S>0.05");
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
		cut_A_S_upper(0.018, 0.0022, 0.046, 0.0064, display, channel, "2pe_merged");
}
if (channel==33) {
	x_y_regions = {0.016, 0.016, 0.0010, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.038, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0019, 0.038, 0.0050, display, channel, "2pe_merged");
}
if (channel==34) {
	x_y_regions = {0.016, 0.016, 0.0010, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.048, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0019, 0.048, 0.0064, display, channel, "2pe_merged");
}
if (channel==35) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
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
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.046, 0.0063, display, channel, "2pe_merged");
}
if (channel==39) {
	x_y_regions = {0.018, 0.018, 0.0012, 0.05, 0.0, 1e3};
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
	x_y_regions = {0.020, 0.020, 0.00125, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.047, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0022, 0.047, 0.0066, display, channel, "2pe_merged");
}
if (channel==42) {
	x_y_regions = {0.018, 0.018, 0.00125, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.045, 0.0066, display, channel, "2pe_merged");
}
if (channel == 43) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.045, 0.0066, display, channel, "2pe_merged");
}
if (channel==44 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(44);
}
if (channel==48) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.047, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.002, 0.047, 0.0062, display, channel, "2pe_merged");
}
if (channel==49) {
	x_y_regions = {0.015, 0.015, 0.0010, 0.04, 0.0, 1e3};
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
	x_y_regions = {0.016, 0.016, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0020, 0.045, 0.0060, display, channel, "2pe_merged");
}
if (channel==52) {
	x_y_regions = {0.017, 0.017, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.043, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.017, 0.0021, 0.043, 0.0064, display, channel, "2pe_merged");
}
if (channel==53) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.046, 0.0065, display, channel, "2pe_merged");
}
if (channel==54) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.042, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.042, 0.0058, display, channel, "2pe_merged");
}
if (channel==55) {
	x_y_regions = {0.018, 0.018, 0.0014, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.04, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.04, 0.0054, display, channel, "2pe_merged");
}
if (channel==56) {
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0022, 0.046, 0.0060, display, channel, "2pe_merged");
}
if (channel==57) {
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.046, 0.0061, display, channel, "2pe_merged");
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
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.044, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.044, 0.0061, display, channel, "2pe_merged");
}

}//noise_cut

void analysis_history(bool calibrate, int method = 1) {
//Created on 22.11.2019
//method = 1 - use trigger_v2, with detailed analysis at 20kV, 16kV and 10kV
//(sPMTs/fPMTs for trigger {1.5, 2, 2.5, 3, 3.5us})
double trigger_at = 32;
calibration_file = data_output_path+"180705_calibration.dat";
data_output_path = "180705/results_v2/";
trigger_version = TriggerVersion::trigger_v2;

post_processor->calibr_info.Load(calibration_file);
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["180705_Cd_20kV_800V_12bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_18kV_800V_12bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_16kV_800V_12bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_14kV_800V_12bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_13kV_800V_12bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_12kV_800V_6bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_11kV_800V_6bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_10kV_800V_6bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_9kV_800V_0bB_48V"] = std::pair<double, double> (25, 38);
S2_times["180705_Cd_8kV_800V_0bB_48V"] = std::pair<double, double> (25, 38);
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
std::string ch_str = int_to_str(channel);
//parameters set by Cd_20kV_800V_48V.
if (channel>=32) {
ch(channel);
    ty(AStates::MPPC_A_S);
    set_zoom(0, 0.08, 0, 0.012);
    set_bins(500);
    noise_cut(channel, 1, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
    ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss");
    noise_cut(channel, 1, false);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_single_pe");
}

if (channel==8) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(1000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.02, 0, 0.05);
    set_bins(300);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==9) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.1, 0, 0.005);
    set_bins(600);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==10) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(1500);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.07, 0, 0.002);
    set_bins(300);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==11) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.07, 0, 0.002);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==12) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(3000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");
	set_zoom(0, 0.07, 0, 0.002);
    set_bins(100);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
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
std::string aggr = "";
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
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
if (exp == "180705_Cd_20kV_800V_12bB_48V" && method == 1) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping = 1.5;
	std::string dt = "dt=1.5us";
	std::string DT = "dt=1.5#mus";
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(0); noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 1200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 650);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_trigger_bNpe and AStates::PMT_trigger_bNpeaks for future correlations
	ty(AStates::PMT_trigger_bNpe);
	ch(0); noise_cut(0, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Very poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	ty(AStates::Correlation);
	set_zoom(0, 1200, 0, 1200);
	set_bins(1200);
	cut_x_y_right_select(570, 420, 720, 530, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("temp");
	path = FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_right_noise/events_sPMTs_right_noise";
	saveaspng(path);
	print_accepted_events(path+".txt", 360);
	unset_as_run_cut("temp");
	cut_x_y_right_select(570, 420, 720, 530, true, "1");
	update();
	set_as_run_cut("temp");
	path = FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us_right_noise/events_sPMTs_right_noise2";
	saveaspng(path);
	print_accepted_events(path+".txt", 360);
	unset_as_run_cut("temp");
	cut_x_y_right(20, 0, 540, 420, true, "1");
	cut_x_y_right(570, 420, 720, 530, true, "2");
	cut_x_y_right(720, 530, 760, 650, true, "3");
	cut_x_y_right(760, 650, 1200, 1050, true, "4");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(545, 725);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("Cd_peak");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	//Now apply different trigger adjustments for valid events and Cd peak
	std::vector<std::pair<double, double> > fPMT_trigger_lims;
	std::vector<std::pair<double, double> > sPMT_trigger_lims;
	std::vector<double> shapinngs = {1.5, 2.0, 2.5, 3.0, 3.5, 4.5};
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //1.5 us shaping
	sPMT_trigger_lims.push_back(std::pair<double, double> (33.5, 36.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //2.0 us shaping
	sPMT_trigger_lims.push_back(std::pair<double, double> (33.5, 36.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //2.5 us shaping
	sPMT_trigger_lims.push_back(std::pair<double, double> (33.5, 36.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //3.0 us shaping
	sPMT_trigger_lims.push_back(std::pair<double, double> (33.5, 36.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (33.5, 36.5)); //3.5 us shaping
	sPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //4.5 us shaping
	sPMT_trigger_lims.push_back(std::pair<double, double> (33.5, 36.5));
	for (std::size_t sh = 0, sh_end_ = shapinngs.size(); sh != sh_end_; ++sh) {
		dt_shaping = shapinngs[sh];
		dt = "dt="+dbl_to_str(dt_shaping, 1) + "us";
		DT = "dt="+dbl_to_str(dt_shaping, 1) + "#mus";
		//DRAW SLOW-FAST PMT TRIGGER CORRELATION FOR Cd PEAK FOR 1.5 us
	set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
		ty(AStates::Correlation_x);
		set_trigger_shaping(dt_shaping);
		ty(AStates::Correlation_y);
		set_trigger_shaping(dt_shaping);
		ty(AStates::Correlation);
		set_zoom(20, 45, 20, 45);
		set_bins(600);
		set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
		saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
		Num = int_to_str(++no, 2);

		//APPLY slowPMTs 1.5us TRIGGER CORRECTION trigger_v2
	ty(AStates::PMT_trigger_bS);
		set_trigger_shaping(dt_shaping);
		saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
		Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe);
		set_trigger_shaping(dt_shaping);
		draw_limits(sPMT_trigger_lims[sh].first, sPMT_trigger_lims[sh].second);
		set_as_run_cut("valid_trigger_sPMTs"); cuts += "+" + Num;
		saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
		unset_draw_limits();
		set_trigger_offsets(30);
		Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS);
		saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
		Num = int_to_str(++no, 2);

	save_forms(FOLDER + Num + "_slowPMTs_trigger_"+dt+"_"+cuts+"/", false);

		//APPLY fastPMTs 1.5us (by peak area S) TRIGGER CORRECTION trigger_v2
		unset_trigger_offsets();
		unset_as_run_cut("valid_trigger_sPMTs");
		cuts.erase(cuts.end()-3, cuts.end());

	ty(AStates::PMT_trigger_bS);
		draw_limits(fPMT_trigger_lims[sh].first, fPMT_trigger_lims[sh].second);
		set_as_run_cut("valid_trigger_fPMTs"); cuts += "+" + Num;
		saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
		unset_draw_limits();
		set_trigger_offsets(30);
		Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe);
		saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
		Num = int_to_str(++no, 2);

	save_forms(FOLDER + Num + "_fastPMTs_Strigger_"+dt+"_"+cuts+"/", false);

		unset_trigger_offsets();
		unset_as_run_cut("valid_trigger_fPMTs");
		cuts.erase(cuts.end()-3, cuts.end());
	}
}

if (exp == "180705_Cd_16kV_800V_12bB_48V" && method == 1) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping = 3.0;
	std::string dt = "dt=3.0us";
	std::string DT = "dt=3.0#mus";
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(0); noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 1000);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 550);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	//Set AStates::PMT_trigger_bNpe and AStates::PMT_trigger_bNpeaks for future correlations
	ty(AStates::PMT_trigger_bNpe);
	ch(0); noise_cut(0, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //Poor results because of merged peaks
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	ty(AStates::Correlation);
	set_zoom(0, 1100, 0, 1100);
	set_bins(1100);
	cut_x_y_right_select(570, 420, 720, 530, true, "1");
	cut_x_y_right(15, 0, 660, 530, true, "1");
	cut_x_y_right(660, 530, 760, 650, true, "2");
	cut_x_y_right(760, 650, 1200, 1050, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(290, 520);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("Cd_peak");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	//Now apply different trigger adjustments for valid events and Cd peak
	std::vector<std::pair<double, double> > fPMT_trigger_lims;
	std::vector<double> shapinngs = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5};
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //2.0 us shaping
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //2.5 us shaping
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //3.0 us shaping
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //3.5 us shaping
	fPMT_trigger_lims.push_back(std::pair<double, double> (32, 35.5)); //4.0 us shaping
	fPMT_trigger_lims.push_back(std::pair<double, double> (32.5, 36.5)); //4.5 us shaping
	for (std::size_t sh = 0, sh_end_ = shapinngs.size(); sh != sh_end_; ++sh) {
		dt_shaping = shapinngs[sh];
		dt = "dt="+dbl_to_str(dt_shaping, 1) + "us";
		DT = "dt="+dbl_to_str(dt_shaping, 1) + "#mus";
		//DRAW SLOW-FAST PMT TRIGGER CORRELATION FOR Cd PEAK FOR 1.5 us
	set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bS, -1, -1);
		ty(AStates::Correlation_x);
		set_trigger_shaping(dt_shaping);
		ty(AStates::Correlation_y);
		set_trigger_shaping(dt_shaping);
		ty(AStates::Correlation);
		set_zoom(20, 45, 20, 45);
		set_bins(600);
		set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
		saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
		Num = int_to_str(++no, 2);

		//ONLY APPLY fastPMTs (by peak area S) TRIGGER CORRECTION trigger_v2
	ty(AStates::PMT_trigger_bS);
		set_trigger_shaping(dt_shaping);
		draw_limits(fPMT_trigger_lims[sh].first, fPMT_trigger_lims[sh].second);
		set_as_run_cut("valid_trigger_fPMTs"); cuts += "+" + Num;
		saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
		unset_draw_limits();
		set_trigger_offsets(30);
		Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe);
		set_trigger_shaping(dt_shaping);
		saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
		Num = int_to_str(++no, 2);

	save_forms(FOLDER + Num + "_fastPMTs_Strigger_"+dt+"_"+cuts+"/", false);
	print_accepted_events(FOLDER + Num + "_fastPMTs_Strigger_"+dt+"_"+cuts+"/events.txt", 338);

		unset_trigger_offsets();
		unset_as_run_cut("valid_trigger_fPMTs");
		cuts.erase(cuts.end()-3, cuts.end());
	}
}

if (exp == "180705_Cd_12kV_800V_6bB_48V" && method == 1) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping;
	std::string dt;
	std::string DT;
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(0); noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 700);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	ty(AStates::Correlation);
	set_zoom(0, 700, 0, 700);
	set_bins(700);
	cut_x_y_right(10, 0, 700, 546, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::PMT_Npe_sum);
	time_zoom(d_S2_start, 160);
	draw_limits(61, 180);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("Cd_peak");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_sum_N);
	time_zoom(d_S2_start, d_S2_finish);
	draw_limits(5, 300);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts);
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	//Now apply different trigger adjustments for valid events and Cd peak
	std::vector<std::pair<double, double> > fPMT_trigger_lims;
	std::vector<double> shapinngs = {3.0, 3.5, 4.0, 4.5, 5.0};
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.3, 35.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.3, 35.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.3, 35.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.3, 35.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.3, 35.5));

	for (std::size_t sh = 0, sh_end_ = shapinngs.size(); sh != sh_end_; ++sh) {
		dt_shaping = shapinngs[sh];
		dt = "dt="+dbl_to_str(dt_shaping, 1) + "us";
		DT = "dt="+dbl_to_str(dt_shaping, 1) + "#mus";

	ty(AStates::PMT_trigger_bS);
		off_ch(0); off_ch(1); off_ch(12);
		noise_cut(8, 0, 0);
		noise_cut(9, 0, 0);
		noise_cut(10, 0, 0);
		noise_cut(11, 0, 0);
		set_zoom(25, 45);
		set_bins(300);
		set_trigger_shaping(dt_shaping);
		draw_limits(fPMT_trigger_lims[sh].first, fPMT_trigger_lims[sh].second);
		saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+dt+"_"+cuts+"_zoom");
		set_as_run_cut("good_trigger");	cuts += "+" + Num;
		set_trigger_offsets(trigger_at);

	save_forms(FOLDER + Num + "_fastPMTs_Strigger_"+dt+"_"+cuts+"/", false);
		Num = int_to_str(++no, 2);

		unset_trigger_offsets();
		unset_as_run_cut("good_trigger");
		cuts.erase(cuts.end()-3, cuts.end());
	}
}

if (exp == "180705_Cd_11kV_800V_6bB_48V" && method == 1) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping;
	std::string dt;
	std::string DT;
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(0); noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	ty(AStates::Correlation);
	set_zoom(0, 500, 0, 500);
	set_bins(500);
	cut_x_y_right(10, 0, 500, 400, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::PMT_Npe_sum);
	time_zoom(d_S2_start, 160);
	draw_limits(47, 130);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("Cd_peak");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_sum_N);
	time_zoom(d_S2_start, d_S2_finish);
	draw_limits(5, 300);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts);
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	//Now apply different trigger adjustments for valid events and Cd peak
	std::vector<std::pair<double, double> > fPMT_trigger_lims;
	std::vector<double> shapinngs = {3.0, 3.5, 4.0, 4.5, 5.0};
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.0, 35.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.0, 35.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.7, 34.3));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.6, 34.2));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.5, 34.5));

	for (std::size_t sh = 0, sh_end_ = shapinngs.size(); sh != sh_end_; ++sh) {
		dt_shaping = shapinngs[sh];
		dt = "dt="+dbl_to_str(dt_shaping, 1) + "us";
		DT = "dt="+dbl_to_str(dt_shaping, 1) + "#mus";

	ty(AStates::PMT_trigger_bS);
		off_ch(0); off_ch(1); off_ch(12);
		noise_cut(8, 0, 0);
		noise_cut(9, 0, 0);
		noise_cut(10, 0, 0);
		noise_cut(11, 0, 0);
		set_zoom(25, 45);
		set_bins(300);
		set_trigger_shaping(dt_shaping);
		draw_limits(fPMT_trigger_lims[sh].first, fPMT_trigger_lims[sh].second);
		saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+dt+"_"+cuts+"_zoom");
		set_as_run_cut("good_trigger");	cuts += "+" + Num;
		set_trigger_offsets(trigger_at);

	save_forms(FOLDER + Num + "_fastPMTs_Strigger_"+dt+"_"+cuts+"/", false);
		Num = int_to_str(++no, 2);

		unset_trigger_offsets();
		unset_as_run_cut("good_trigger");
		cuts.erase(cuts.end()-3, cuts.end());
	}
}

if (exp == "180705_Cd_10kV_800V_6bB_48V" && method == 1) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping;
	std::string dt;
	std::string DT;
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(0); noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	ty(AStates::Correlation);
	set_zoom(0, 400, 0, 400);
	set_bins(400);
	cut_x_y_right(8, 0, 400, 330, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::PMT_Npe_sum);
	time_zoom(d_S2_start, 160);
	draw_limits(24, 70);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("Cd_peak");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_sum_N);
	time_zoom(d_S2_start, d_S2_finish);
	draw_limits(5, 100);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts);
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	//Now apply different trigger adjustments for valid events and Cd peak
	std::vector<std::pair<double, double> > fPMT_trigger_lims;
	std::vector<double> shapinngs = {3.0, 3.5, 4.0, 4.5, 5.0};
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.2, 33.7));
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.0, 33.9));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.5, 34.2));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.5, 34.3));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.6, 34.5));

	for (std::size_t sh = 0, sh_end_ = shapinngs.size(); sh != sh_end_; ++sh) {
		dt_shaping = shapinngs[sh];
		dt = "dt="+dbl_to_str(dt_shaping, 1) + "us";
		DT = "dt="+dbl_to_str(dt_shaping, 1) + "#mus";

	ty(AStates::PMT_trigger_bNpeaks);
		off_ch(0); off_ch(1); off_ch(12);
		noise_cut(8, 0, 0);
		noise_cut(9, 0, 0);
		noise_cut(10, 0, 0);
		noise_cut(11, 0, 0);
		set_zoom(25, 45);
		set_bins(300);
		set_trigger_shaping(dt_shaping);
		draw_limits(fPMT_trigger_lims[sh].first, fPMT_trigger_lims[sh].second);
		saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+dt+"_"+cuts+"_zoom");
		set_as_run_cut("good_trigger");	cuts += "+" + Num;
		set_trigger_offsets(trigger_at);

	save_forms(FOLDER + Num + "_fastPMTs_Ntrigger_"+dt+"_"+cuts+"/", false);
		Num = int_to_str(++no, 2);

		unset_trigger_offsets();
		unset_as_run_cut("good_trigger");
		cuts.erase(cuts.end()-3, cuts.end());
	}
}

if (exp == "180705_Cd_9kV_800V_0bB_48V" && method == 1) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping;
	std::string dt;
	std::string DT;
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(0); noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	cut_x_y_right(8, 0, 200, 160, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::PMT_Npe_sum);
	time_zoom(d_S2_start, 160);
	draw_limits(13, 35);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("Cd_peak");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_sum_N);
	time_zoom(d_S2_start, d_S2_finish);
	draw_limits(5, 100);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts);
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	//Now apply different trigger adjustments for valid events and Cd peak
	std::vector<std::pair<double, double> > fPMT_trigger_lims;
	std::vector<double> shapinngs = {3.5, 4.0, 4.5, 5.0, 5.5};
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.0, 34.0));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.7, 34.2));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.5, 34.3));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.6, 34.6));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.6, 34.7));

	for (std::size_t sh = 0, sh_end_ = shapinngs.size(); sh != sh_end_; ++sh) {
		dt_shaping = shapinngs[sh];
		dt = "dt="+dbl_to_str(dt_shaping, 1) + "us";
		DT = "dt="+dbl_to_str(dt_shaping, 1) + "#mus";

	ty(AStates::PMT_trigger_bNpeaks);
		off_ch(0); off_ch(1); off_ch(12);
		noise_cut(8, 0, 0);
		noise_cut(9, 0, 0);
		noise_cut(10, 0, 0);
		noise_cut(11, 0, 0);
		set_zoom(25, 45);
		set_bins(300);
		set_trigger_shaping(dt_shaping);
		draw_limits(fPMT_trigger_lims[sh].first, fPMT_trigger_lims[sh].second);
		saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+dt+"_"+cuts+"_zoom");
		set_as_run_cut("good_trigger");	cuts += "+" + Num;
		set_trigger_offsets(trigger_at);

	save_forms(FOLDER + Num + "_fastPMTs_Ntrigger_"+dt+"_"+cuts+"/", false);
		Num = int_to_str(++no, 2);

		unset_trigger_offsets();
		unset_as_run_cut("good_trigger");
		cuts.erase(cuts.end()-3, cuts.end());
	}
}

if (exp == "180705_Cd_8kV_800V_0bB_48V" && method == 1) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping;
	std::string dt;
	std::string DT;
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(0); noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, 0);
	noise_cut(9, 0, 0);
	noise_cut(10, 0, 0);
	noise_cut(11, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 0);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	cut_x_y_right(8, 0, 200, 170, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

ty(AStates::PMT_sum_N);
	time_zoom(d_S2_start, 160);
	draw_limits(6, 17);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts);
	set_as_run_cut("Cd_peak");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	time_zoom(d_S2_start, d_S2_finish);
	draw_limits(5, 40);
	saveaspng(FOLDER + Num + "_fastPMTs_Npeaks_"+cuts);
	set_as_run_cut("N_peaks");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

	//Now apply different trigger adjustments for valid events and Cd peak
	std::vector<std::pair<double, double> > fPMT_trigger_lims;
	std::vector<double> shapinngs = {3.5, 4.0, 4.5, 5.0, 5.5};
	fPMT_trigger_lims.push_back(std::pair<double, double> (30.1, 34.0));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.5, 34.3));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.5, 34.5));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.0, 35.0));
	fPMT_trigger_lims.push_back(std::pair<double, double> (29.0, 35.0));

	for (std::size_t sh = 0, sh_end_ = shapinngs.size(); sh != sh_end_; ++sh) {
		dt_shaping = shapinngs[sh];
		dt = "dt="+dbl_to_str(dt_shaping, 1) + "us";
		DT = "dt="+dbl_to_str(dt_shaping, 1) + "#mus";

	ty(AStates::PMT_trigger_bNpeaks);
		off_ch(0); off_ch(1); off_ch(12);
		noise_cut(8, 0, 0);
		noise_cut(9, 0, 0);
		noise_cut(10, 0, 0);
		noise_cut(11, 0, 0);
		set_zoom(25, 45);
		set_bins(300);
		set_trigger_shaping(dt_shaping);
		draw_limits(fPMT_trigger_lims[sh].first, fPMT_trigger_lims[sh].second);
		saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+dt+"_"+cuts+"_zoom");
		set_as_run_cut("good_trigger");	cuts += "+" + Num;
		set_trigger_offsets(trigger_at);

	save_forms(FOLDER + Num + "_fastPMTs_Ntrigger_"+dt+"_"+cuts+"/", false);
		Num = int_to_str(++no, 2);

		unset_trigger_offsets();
		unset_as_run_cut("good_trigger");
		cuts.erase(cuts.end()-3, cuts.end());
	}
}
//END OF FORMS
}
