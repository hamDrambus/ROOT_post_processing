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
	//	PMT: 0 - 850V, 12dB
	//	SiPM: 0 - 46V, 1 - 48V
	if (channel==1) {
		cut_A_S_lower(0.012, 0.0005, 0.046, 0.0051, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.046, 0.0051, 1.1, 0.27, display, channel, "rem_largeA_smallS");
	}
	if (channel==2) {
		cut_A_S_lower(0.02, 0.00096, 0.075, 0.0054, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.075, 0.0054, 1.6, 0.26, display, channel, "rem_largeA_smallS");
	}
	if (channel==3) {
		cut_A_S_lower(0.013, 0.00076, 0.042, 0.0024, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.042, 0.0024, 1.2, 0.20, display, channel, "rem_largeA_smallS");
	}
	if (channel==4) {
		cut_A_S_upper(0.013, 0.0, 0.0238, 0.0, display, channel, "rem_smallA_largeS_1");
		cut_A_S_lower(0.0165, 0.00220, 0.038, 0.00070, display, channel, "rem_smallA_smallS");
		cut_A_S_lower(0.038, 0.00070, 1.6, 0.12, display, channel, "rem_largeA_smallS");
	}
	if (channel==5) {

	}
	if (channel==6) {

	}
	if (channel==7) {

	}
	if (channel==8) {

	}
	//qewr
}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2021.02.23
//First analysis of room temperature alpha data
data_output_path = "210218/results_v1/";
calibration_file = "210218/results_v1/210218_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["210218_Pu_5.5kV_850V_0V_12dB_th2V"] = std::pair<double, double> (9.3, 43);
S2_times["210218_Pu_5.5kV_850V_0V_12dB_th250mV"] = std::pair<double, double> (11.5, 49);

std::map<std::string, std::string> exp_folders;
exp_folders["210218_Pu_5.5kV_850V_0V_12dB_th2V"] = "Pu_54V_5.5kV_850V_high_thr";
exp_folders["210218_Pu_5.5kV_850V_0V_12dB_th250mV"] = "Pu_54V_5.5kV_850V_low_thr";

std::map<std::string, double> pre_trigger_max_S; //for calibration
pre_trigger_max_S["210218_Pu_5.5kV_850V_0V_12dB_th2V"] = 0.075;
pre_trigger_max_S["210218_Pu_5.5kV_850V_0V_12dB_th250mV"] = 0.15;

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
double d_S_max;
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

ty(AStates::PMT_S_sum); //nex();
slow_PMTs_only();
time_zoom_sPMTs(0, d_S2_start);
set_zoom(0, 2);
set_bins(500); set_log_y();
draw_limits(0, d_S_max);
saveas(data_output_path + folder +"/calibration/sPMTs_pre-trigger_cut");
set_as_run_cut("small_pre-trigger");

for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) {
int channel = calib_channels[chi];
std::string ch_str = int_to_str(channel);
//parameters set for 210218_Pu_5.5kV_850V_0V_12dB_th2V, but valid for all fields.
//adsf
if (channel==1) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.2, 0, 8);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.1, 0, 0.025);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
		set_bins(500);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(1500);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.030);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==2) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.4, 0, 8);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.03);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
	 	set_log_z();
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(1500);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.1);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.08);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==3) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.2, 0, 8);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
		set_zoom(0, 0.09, 0, 0.025);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(1500);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.08);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.03);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==4) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 1.25, 0, 12);
    set_bins(2000);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.12, 0, 0.04);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(300);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		unset_log_z(); set_bins(2000);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.16);
    set_bins(600);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_cuts");
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.08);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_3_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==5) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.2, 0, 0.05);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.06, 0, 0.006);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(120);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.001);
    set_bins(800);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0004);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==6) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.3, 0, 0.02);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.06, 0, 0.004);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0012);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==7) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.08, 0, 0.010);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
  	set_zoom(0, 0.03, 0, 0.003);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.001);
    set_bins(800);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0005);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
}
if (channel==8) {
ty(AStates::PMT_A_S);
    ch(channel);
    set_zoom(0, 0.5, 0, 0.04);
    set_bins(1500);
    noise_cut(channel, 1, PMT_state, true);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_1_");
    set_zoom(0, 0.05, 0, 0.0025);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_2_zoom");
		set_log_z();
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_3_zoom_log");
    set_bins(200);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_4_zoom_log");
		cut_t(0, d_S2_start, false, channel);
		set_bins(1500);
		saveas(data_output_path+folder+"/calibration/"+ch_str+"_A_S_5_zoom_log_w_0-"+S2_start+"us");
ty(AStates::PMT_Ss);
    set_zoom(0, 0.002);
    set_bins(800);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_1_");
    noise_cut(channel, 0, PMT_state, false);
    cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0, 0.0016);
    saveas(data_output_path+folder+"/calibration/"+ch_str+"_Ss_2_w_0-"+S2_start+"us");
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
if (exp == "210218_Pu_5.5kV_850V_0V_12dB_th2V") {
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
	set_bins(0, 2500);
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
	set_bins(0, 2500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 2500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(0, 160);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 2500, 0, 2500);
	set_bins(2500);
	x_y_regions = {57, 0, 2334, 2203};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

std::string form_n = "forms_Alpha_peak/";
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(358, 575);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_right_slope/";
ty(AStates::PMT_Npe_sum);
	draw_limits(624, 827);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_right_tail/";
ty(AStates::PMT_Npe_sum);
	draw_limits(895, 1500);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
if (exp == "210218_Pu_5.5kV_850V_0V_12dB_th250mV") {
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
	set_bins(0, 2500);
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
	set_bins(0, 2500);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	fast_PMTs_only();
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 2500);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(0, 160);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 2500, 0, 2500);
	set_bins(2500);
	x_y_regions = {57, 0, 2334, 2203};
	cut_x_y_right(x_y_regions, true, "1");
	update();
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_PMTs_ZxZy"); cuts.push_back(Num);
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

std::string form_n = "forms_Alpha_peak/";
ty(AStates::PMT_Npe_sum);
	time_zoom_sPMTs(d_S2_start, 160);
	draw_limits(280, 395);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_right_slope/";
ty(AStates::PMT_Npe_sum);
	draw_limits(624, 827);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();

form_n = "forms_Alpha_right_tail/";
ty(AStates::PMT_Npe_sum);
	draw_limits(895, 1500);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_N="+int_to_str(post_processor->numOfFills(true))+"events");
	set_as_run_cut("En_spec"); cuts.push_back(Num);
	print_accepted_events(FOLDER + form_n + "events.txt", first_run);
	Num = int_to_str(++no, 2);

	save_forms(FOLDER + form_n, false, PMT_state, SiPM_state);
	unset_as_run_cut("En_spec");
	cuts.pop_back();
}
//END OF FORMS
}
