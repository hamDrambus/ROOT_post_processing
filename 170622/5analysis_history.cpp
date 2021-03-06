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
	ty(AStates::MPPC_tbNpe_sum);
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
//	PMT: 0 - 700V, 0dB
//	SiPM: 0 - 46V
if (channel==0) {
	cut_A_S_upper(0.05, 0.0, 0.074, 0.0, display, channel, "rem_smallA_largeS_1");
	cut_A_S_upper(0.074, 0.0093, 0.083, 0.015, display, channel, "rem_smallA_largeS_2");
	cut_A_S_lower(0.06, 0.0020, 0.135, 0.0034, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.12, 0.003, 1.4, 0.175, display, channel, "rem_largeA_smallS");
	if (aggressiveness == 1) {
	  cut_A_S_upper(1.4, 0, 2, 0, display, channel, "rem_A>1.4");
	  cut_A_S_upper(0.0, 6, 1.6, 6, display, channel, "rem_S>~6.0");
  }
}
if (channel==1) {
	cut_A_S_upper(0.05, 0.013, 0.061, 0.026, display, channel, "rem_smallA_largeS_2");
	cut_A_S_lower(0.05, 0.0034, 0.1, 0.0051, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.08, 0.0036, 0.8, 0.16, display, channel, "rem_largeA_smallS");
	if (aggressiveness == 1) {
	  cut_A_S_upper(0.8, 0, 2, 0, display, channel, "rem_A>0.8");
	  cut_A_S_upper(0.0, 2, 1.6, 2, display, channel, "rem_S>~2.0");
  }
}
//The following are set by hand for 20kV individually, there is no other way.
if (channel==32) {
	cut_A_S_lower(0.01, 0.00052, 0.03, 0.00134, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.012, 0.0001, 0.06, 0.0044, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0287, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00103, 0.0287, 0.00313, display, channel, "2pe_merged");
}
if (channel==33) {
	cut_A_S_lower(0.01, 0.00052, 0.03, 0.00131, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.012, 0.0001, 0.06, 0.0044, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0237, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.00103, 0.0237, 0.00249, display, channel, "2pe_merged");
}
if (channel==34) {
	cut_A_S_lower(0.012, 0.00076, 0.03, 0.00156, display, channel, "small_A-S_noise");
	x_y_regions = {0.0155, 0.012, 0.0001, 0.06, 0.0044, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0305, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00108, 0.0305, 0.00342, display, channel, "2pe_merged");
}
if (channel==35 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==36) {
	cut_A_S_lower(0.012, 0.00087, 0.03, 0.00105, display, channel, "small_A-S_noise");
	x_y_regions = {0.0145, 0.012, 0.00029, 0.06, 0.0047, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0283, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00108, 0.0283, 0.00366, display, channel, "2pe_merged");
}
if (channel==37 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==38) {
	cut_A_S_lower(0.012, 0.00070, 0.03, 0.00169, display, channel, "small_A-S_noise");
	x_y_regions = {0.0150, 0.012, 0.00029, 0.06, 0.0047, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.03, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00108, 0.03, 0.00392, display, channel, "2pe_merged");
}
if (channel==39 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==40) {
	cut_A_S_lower(0.012, 0.00050, 0.03, 0.00153, display, channel, "small_A-S_noise");
	x_y_regions = {0.0150, 0.012, 0.00029, 0.06, 0.00455, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0294, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00108, 0.0294, 0.00376, display, channel, "2pe_merged");
}
if (channel==41 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==42) {
	cut_A_S_lower(0.012, 0.00058, 0.03, 0.00138, display, channel, "small_A-S_noise");
	x_y_regions = {0.0150, 0.012, 0.00029, 0.06, 0.00455, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0286, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00108, 0.0286, 0.00368, display, channel, "2pe_merged");
}
if (channel==43) {
	cut_A_S_lower(0.012, 0.00057, 0.03, 0.00137, display, channel, "small_A-S_noise");
	x_y_regions = {0.0150, 0.012, 0.00029, 0.06, 0.00455, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0286, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00108, 0.0286, 0.00368, display, channel, "2pe_merged");
}
if (channel==44 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==48) {
	cut_A_S_lower(0.012, 0.00056, 0.03, 0.00132, display, channel, "small_A-S_noise");
	x_y_regions = {0.0150, 0.012, 0.00018, 0.06, 0.00437, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.03, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00108, 0.03, 0.00318, display, channel, "2pe_merged");
}
if (channel==49 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==50 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==51 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==52 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==53) {
	cut_A_S_lower(0.012, 0.00087, 0.03, 0.00151, display, channel, "small_A-S_noise");
	x_y_regions = {0.0160, 0.012, 0.00030, 0.06, 0.0046, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0312, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00100, 0.0312, 0.00408, display, channel, "2pe_merged");
}
if (channel==54 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==55 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==56 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==57 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==58) {
	cut_A_S_lower(0.012, 0.00075, 0.03, 0.00154, display, channel, "small_A-S_noise");
	x_y_regions = {0.0145, 0.012, 0.00020, 0.06, 0.0045, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0280, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00100, 0.0280, 0.00355, display, channel, "2pe_merged");
}
if (channel==59) {
	cut_A_S_lower(0.012, 0.00079, 0.03, 0.00154, display, channel, "small_A-S_noise");
	x_y_regions = {0.0145, 0.012, 0.00020, 0.06, 0.0045, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0286, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00105, 0.0286, 0.00352, display, channel, "2pe_merged");
}

}

void analysis_history(bool calibrate, unsigned int iteration = 0) {
//Created on 2020.07.22
//iteration = 0 - no trigger adjustment.
//iteration > 0 - use fast PMTs for trigger_fit (byNpe for large fields/byNpeaks for low) with
//experimental pulse-shape determined from previous iteration
data_output_path = "170622/results_v5/";
calibration_file = "170622/results_v5/170622_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::string iter_str = int_to_str((std::size_t)iteration, 2);

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["event_Cd_20_th400mV"] = std::pair<double, double> (24, 36);
S2_times["event_Cd_18_th300mV"] = std::pair<double, double> (24, 36);
S2_times["event_Cd_18_th190mV"] = std::pair<double, double> (24, 36);
S2_times["event_Cd_16_thmV"] = std::pair<double, double> (24, 36);
S2_times["event_Cd_14_thmV"] = std::pair<double, double> (24, 36);

std::map<std::string, std::string> exp_folders;
exp_folders["event_Cd_20_th400mV"] = "Cd_46V_20kV_700V";
exp_folders["event_Cd_18_th300mV"] = "Cd_46V_18kV_700V_th300mV";
exp_folders["event_Cd_18_th190mV"] = "Cd_46V_18kV_700V_th190mV";
exp_folders["event_Cd_16_thmV"] = "Cd_46V_16kV_700V";
exp_folders["event_Cd_14_thmV"] = "Cd_46V_14kV_700V";

std::map<std::string, std::pair<double, double> > S2_pre_trigger_cut;
S2_pre_trigger_cut["event_Cd_20_th400mV"] = std::pair<double, double> (0, 0.15);
S2_pre_trigger_cut["event_Cd_18_th300mV"] = std::pair<double, double> (0, 0.12);
S2_pre_trigger_cut["event_Cd_18_th190mV"] = std::pair<double, double> (0, 0.12);
S2_pre_trigger_cut["event_Cd_16_thmV"] = std::pair<double, double> (0, 0.10);
S2_pre_trigger_cut["event_Cd_14_thmV"] = std::pair<double, double> (0, 0.10);

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //700V, 0 dB
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
double d_S2_min_pre_trigger, d_S2_max_pre_trigger;
auto S2_cut_entry = S2_pre_trigger_cut.find(exp);
if (S2_cut_entry != S2_pre_trigger_cut.end()) {
	  d_S2_min_pre_trigger = S2_cut_entry->second.first;
	  d_S2_max_pre_trigger = S2_cut_entry->second.second;
} else {
    std::cout<<"Could not find S2 cut limits for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
    nex();
    continue;
}
//Pre-trigger region is used for calibration.
//Remove pileup S2 signals from it.
ty(AStates::PMT_S2_S);
ch(0);
cut_t(0, d_S2_start, false, 0);
set_zoom(0.003, 0.4);
set_bins(200);
set_titles("4PMT (slow) S t#in[0, "+S2_start+"] #mus", "Counts");
draw_limits(d_S2_min_pre_trigger, d_S2_max_pre_trigger);
saveaspng(data_output_path + folder + "/calibration/" + "0_Area_w_0-"+S2_start+"us");
set_as_run_cut("pre-trigger");

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
		update();
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
		cut_t(0, d_S2_start, false, channel);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    noise_cut(channel, 0, SiPM_state, false); //!Do not remove "2pe" peaks (use 0) - in pre-trigger region they are from cross-talks and afterpulses
    draw_limits(0.0001, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_no_noise_w_0-"+S2_start+"us");
}
if (channel==0) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.5, 0, 10);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.3, 0, 0.1);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		set_bins(300);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
		update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		noise_cut(channel, 0, PMT_state, false);
		draw_limits(0.0, 0.08);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us_w_cuts");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==1) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.5, 0, 10);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.3, 0, 0.1);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom");
		set_bins(300);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_zoom2");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss");
    cut_t(0, d_S2_start, false, channel);
		update();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
		noise_cut(channel, 0, PMT_state, false);
		draw_limits(0.0, 0.08);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us_w_cuts");
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

if (exp == "event_Cd_20_th400mV") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	draw_limits(0, 10);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_w_0-"+S2_start+"us_"+cuts);
	set_as_run_cut("good_PMTs_pretrigger");	cuts = Num;
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 1000);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_peak_no_cuts/";
ty(AStates::PMT_Npe_sum);
	draw_limits(95, 243);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	unset_draw_limits();
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	ty(AStates::PMT_sum_N);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npeaks_"+cuts);
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1000, 0, 1000);
	set_bins(1000);
	x_y_regions = {55, 0, 213, 120, 300, 200, 999, 764};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy");	cuts += "+"+Num;
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(95, 170);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(95, 243);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(288, 600);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

}
if (exp == "event_Cd_18_th300mV") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	draw_limits(0, 10);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_w_0-"+S2_start+"us_"+cuts);
	set_as_run_cut("good_PMTs_pretrigger");	cuts = Num;
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 800);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_peak_no_cuts/";
ty(AStates::PMT_Npe_sum);
	draw_limits(70, 208);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	unset_draw_limits();
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	ty(AStates::PMT_sum_N);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npeaks_"+cuts);
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

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 800, 0, 800);
	set_bins(800);
	x_y_regions = {43, 0, 799, 588};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy");	cuts += "+"+Num;
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(70, 126);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(70, 208);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(278, 500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

}
if (exp == "event_Cd_18_th190mV") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	draw_limits(0, 9);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_w_0-"+S2_start+"us_"+cuts);
	set_as_run_cut("good_PMTs_pretrigger");	cuts = Num;
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 800);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_peak_no_cuts/";
ty(AStates::PMT_Npe_sum);
	draw_limits(70, 190);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	unset_draw_limits();
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	ty(AStates::PMT_sum_N);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich = 0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 800, 0, 800);
	set_bins(800);
	x_y_regions = {18, 0, 125, 58, 799, 640};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy");	cuts += "+"+Num;
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(70, 126);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(70, 190);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(250, 500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

}
if (exp == "event_Cd_16_thmV") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	draw_limits(0, 9);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_w_0-"+S2_start+"us_"+cuts);
	set_as_run_cut("good_PMTs_pretrigger");	cuts = Num;
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 600);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_peak_no_cuts/";
ty(AStates::PMT_Npe_sum);
	draw_limits(40, 118);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	unset_draw_limits();
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	ty(AStates::PMT_sum_N);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich = 0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 600, 0, 600);
	set_bins(600);
	x_y_regions = {26, 0, 599, 420};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy");	cuts += "+"+Num;
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(40, 70);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(40, 118);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(250, 500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

}
if (exp == "event_Cd_14_thmV") {
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/iteration_" + iter_str + "/";

	//Set AStates::PMT_Npe_sum, AStates::PMT_sum_N and AStates::MPPC_Npe_sum cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_sPMTs(0, d_S2_start);
	set_bins(0, 100);
	draw_limits(0, 9);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_w_0-"+S2_start+"us_"+cuts);
	set_as_run_cut("good_PMTs_pretrigger");	cuts = Num;
	unset_draw_limits();
	Num = int_to_str(++no, 2);

	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

std::string form_n = "forms_Cd_peak_no_cuts/";
ty(AStates::PMT_Npe_sum);
	draw_limits(17, 53);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	unset_draw_limits();
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

	ty(AStates::PMT_sum_N);
	noise_cut(0, 0, PMT_state, 0);
	noise_cut(1, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 300);
	saveaspng(FOLDER + Num+"_slowPMTs_Npeaks_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::MPPC_Npe_sum);
	for (int ich = 0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 100);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	x_y_regions = {14, 0, 299, 213};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_PMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy");	cuts += "+"+Num;
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");


form_n = "forms_Cd_left/";
ty(AStates::PMT_Npe_sum);
	draw_limits(17, 32);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_Cd_peak/";
ty(AStates::PMT_Npe_sum);
	draw_limits(17, 53);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

form_n = "forms_cosmic/";
ty(AStates::PMT_Npe_sum);
	draw_limits(70, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + form_n + "events.txt", first_run, 10);
	Num = int_to_str(++no, 2);

ty(AStates::MPPC_Npe_sum);
	saveaspng(FOLDER + Num + "_SiPMs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

}
//END OF FORMS
}

#define PAIR std::pair<double, double>

double integrate(TH1D *hist, double from, double to) {
  double Int = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
      Int += hist->GetBinContent(bin) * hist->GetBinWidth(bin);
    }
  }
  return Int;
}

double integrate(TH1D *hist, double from, double to, double baseline) {
  double Int = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
      double val = std::max(hist->GetBinContent(bin) - baseline, 0.0);
      Int += val * hist->GetBinWidth(bin);
    }
  }
  return Int;
}

double average(TH1D *hist, double from, double to) {
  double baseline = 0;
  int counts = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
      baseline += hist->GetBinContent(bin);
      ++counts;
    }
  }
  return baseline/counts;
}

void subtract_baseline(TH1D *hist, double baseline) {
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    hist->SetBinContent(bin, std::max(hist->GetBinContent(bin) - baseline, 0.0));
  }
}

void print_hist(TH1D *hist, std::ofstream &str) {
  if (!str.is_open() || hist == NULL) {
    std::cout<<"Error: print_hist: invalid parameters"<<std::endl;
    return;
  }
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    str<<hist->GetBinCenter(bin)<<"\t"<<hist->GetBinContent(bin)<<std::endl;
  }
}

void read_hist (TH1D *hist, std::string fname) {
  std::ifstream str;
  str.open(fname, std::ios_base::binary);
  if (!str.is_open()) {
    std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;
    return;
  }
  std::size_t real_size = 0;
  str.read((char*)&real_size, sizeof(std::size_t));
  std::cout<<"File '"<<fname<<"': "<<real_size<<" events"<<std::endl;
  double val;
  while (!str.eof()) {
    str.read((char*)&val, sizeof(double));
    hist->Fill(val);
  }
  str.close();
}

void read_hist_w (TH1D *hist, std::string fname, double offset = 0) {
  std::ifstream str;
  str.open(fname, std::ios_base::binary);
  if (!str.is_open()) {
    std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;
    return;
  }
  std::size_t real_size = 0;
  str.read((char*)&real_size, sizeof(std::size_t));
  std::cout<<"File '"<<fname<<"': "<<real_size<<" events"<<std::endl;
  double val1;
  double val2;
  while (!str.eof()) {
    str.read((char*)&val1, sizeof(double));
    if (str.eof())
      break;
    str.read((char*)&val2, sizeof(double));
    hist->Fill(val1 + offset, val2);
  }
  str.close();
}

int save_forms (std::string prefix, std::string output_file, int Nbins, int type) {
	//TODO: update
  //type: 1 - 4PMT by Npeaks, 2 - 4PMT by Npe, 3 - 4PMT by S
  //4 - SiPM-matrix by Npeaks, 5 - SiPM-matrix by Npe, 6 - SiPM-matrix by S
	//7 - slow 4PMT by Npeaks, 8 - slow 4PMT by Npe, 9 - slow 4PMT by S
  std::string output_data = output_file + ".dat";
  std::string output_fig = output_file + ".png";
  std::ofstream output;
  open_output_file(output_data, output, std::ios_base::trunc);
  if (!output.is_open()) {
    std::cout<<"Error: save_forms: could not open output file \""<<output_file<<"\""<<std::endl;
    return 1;
  }
  if (type<1 || type>9) {
    std::cout<<"Error: save_forms: input int type="<<type<<" is out of range [1, 9]"<<std::endl;
    return 1;
  }
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  int DEF_W = 1300, DEF_H = 700;
  std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
  std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};

  bool linear = 1;

  bool subtact_baseline = true;
  bool renormalize = true;
  double time_pretrigger_left = 5, time_pretrigger_right = 20;
  double time_left = 0, time_right = 160;//us
  double max_val = 0;
  double y_min = 1e-4;

  std::string device = type > 3 ? (type > 6 ? "s4PMT" : "SiPM-matrix") : "4PMT";
  std::string weight_type;
  switch ((type - 1) % 3) {
  case 0: {
    weight_type = "by Npeaks";
    break;
  }
  case 1: {
    weight_type = "by Npe";
    break;
  }
  case 2: {
    weight_type = "by S";
    break;
  }
  }

  std::string framename = prefix + " " + device + " " + weight_type;
  std::string histname = std::string("hist_") + prefix + " " + device + " " + weight_type;
  TH1D *hist = new TH1D (histname.c_str(), histname.c_str(), Nbins, time_left, time_right);

  std::vector<std::string> files_to_read;
  if (type > 3 && type < 7) {
    std::string fname = "SiPMs_form_by_";
    switch ((type - 1) % 3) {
    case 0: {
      fname += "Npeaks";
      break;
    }
    case 1: {
      fname += "Npe";
      break;
    }
    case 2: {
      fname += "S";
      break;
    }
    }
    fname += ".hdata";
    fname = prefix + fname;
    files_to_read.push_back(fname);
  } else {
		if (type >=7)
			files_to_read = {"1_form_by_", "2_form_by_", "3_form_by_", "4_form_by_"};
		else
    	files_to_read = {"5_form_by_", "6_form_by_", "7_form_by_", "8_form_by_"};
    for (std::size_t f = 0, f_end_ = files_to_read.size(); f!=f_end_; ++f) {
      switch ((type - 1) % 3) {
      case 0: {
        files_to_read[f] += "Npeaks";
        break;
      }
      case 1: {
        files_to_read[f] += "Npe";
        break;
      }
      case 2: {
        files_to_read[f] += "S";
        break;
      }
      }
      files_to_read[f] += ".hdata";
      files_to_read[f] = prefix + files_to_read[f];
    }
  }

  for (std::size_t f = 0, f_end_ = files_to_read.size(); f!=f_end_; ++f)
    read_hist_w (hist, files_to_read[f], 0.0);

  double baseline = average(hist, time_pretrigger_left, time_pretrigger_right);
  if (subtact_baseline) {
    subtract_baseline(hist, baseline);
    baseline = 0;
  }
  if (renormalize) {
    double integral = integrate(hist, time_pretrigger_left, time_pretrigger_right);
    hist->Scale(1.0/integral);
    baseline *= 1.0/integral;
  }
  max_val = hist->GetBinContent(hist->GetMaximumBin());
  max_val*= linear ? 1.2 : 2;

  gStyle->SetGridStyle(3);
  gStyle->SetGridColor(14);
  gStyle->SetGridWidth(1);
  gStyle->SetOptStat("");
  TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
  c_->SetGrid(); c_->SetTicks(); c_->ToggleEventStatus(); c_->ToggleToolBar();
  if (!linear)
    c_->SetLogy();
  TLegend *legend = new TLegend(0.35, 0.7, 0.9, 0.9);
  //legend->SetHeader("");
  legend->SetMargin(0.25);
  TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : y_min, max_val);
  frame->GetXaxis()->SetTitle("Time [#mus]");
  frame->GetXaxis()->SetRangeUser(0, 60);
  frame->GetYaxis()->SetTitle("PE peak counts");
  frame->Draw();

  hist->SetLineWidth(2);
  hist->SetLineColor(palette_major[0]);
  hist->Draw("hist Lsame");

  legend->AddEntry(hist, framename.c_str(), "l");

  frame->Draw("sameaxis");
  legend->Draw("same");
  c_->Update();

  print_hist(hist, output);

  ensure_file(output_fig);
	c_->SaveAs(output_fig.c_str(), "png");
  delete c_;
  return 0;
}
