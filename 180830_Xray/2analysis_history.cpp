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

}

void time_zoom_sPMTs (double f_time, double to_time) {

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

}

void slow_PMTs_only (void) {

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

	if (!N_only) {
		ty(AStates::MPPC_tbS_sum);
		central_SiPMs(true);
		for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
			int chan = post_processor->MPPC_channels[ich];
			noise_cut(chan, 0, SiPM_condition, false);
		}
		set_zoom(zoom_l, zoom_r);	set_log_y();
		set_bins(Nbins);
		saveas(path + "SiPMs_form_by_S");
		central_SiPMs(false);
		update();
		saveas(path + "SiPMs_edge_form_by_S");
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
	//	PMT: 0 - 600V, 12dB
	//	SiPM: 0 - 49V
	//qewr
	//The following are set by hand for 20kV individually, there is no other way.
	if (channel==32) {
		x_y_regions = {0.0134, 0.0134, 0.00170, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==33) {
		x_y_regions = {0.0118, 0.0118, 0.00157, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==34) {
		x_y_regions = {0.0140, 0.0140, 0.00206, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==35) {
		x_y_regions = {0.0140, 0.0140, 0.00206, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==36) {
		x_y_regions = {0.0140, 0.0140, 0.00206, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==37) {
		x_y_regions = {0.0123, 0.0123, 0.00162, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==38) {
		x_y_regions = {0.0143, 0.0143, 0.00207, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==39) {
		x_y_regions = {0.0131, 0.0131, 0.00178, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==40) {
		x_y_regions = {0.0134, 0.0134, 0.00183, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==41) {
		x_y_regions = {0.0134, 0.0134, 0.00183, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==42) {
		x_y_regions = {0.0134, 0.0134, 0.00183, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel == 43) {
		if (post_processor->isMultichannel(post_processor->current_type))
			off_ch(channel);
	}
	if (channel == 44) {
		if (post_processor->isMultichannel(post_processor->current_type))
			off_ch(channel);
		x_y_regions = {0.0128, 0.0128, 0.00170, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==48) {
		x_y_regions = {0.0138, 0.0138, 0.00193, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==49) {
		x_y_regions = {0.0116, 0.0116, 0.00159, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==50) {
		x_y_regions = {0.0116, 0.0116, 0.00159, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==51) {
		x_y_regions = {0.0131, 0.0131, 0.00188, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==52) {
		x_y_regions = {0.0138, 0.0138, 0.00201, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==53) {
		x_y_regions = {0.0145, 0.0145, 0.00215, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==54) {
		x_y_regions = {0.0129, 0.0129, 0.00181, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==55) {
		x_y_regions = {0.0129, 0.0129, 0.00181, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==56) {
		x_y_regions = {0.0138, 0.0138, 0.00202, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==57) {
		x_y_regions = {0.0138, 0.0138, 0.00202, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==58) {
		x_y_regions = {0.0135, 0.0135, 0.00200, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}
	if (channel==59) {
		x_y_regions = {0.0133, 0.0133, 0.00191, 0.06, 0.00950, 1e3};
		cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	}

}

void analysis_history(bool calibrate, unsigned int method = 0) {
//Created on 2021.04.20
//Analysis to calculate S1 signal PE for SiPM-matrix, for Vlad.
//Do not select 1 pe for SiPM calibration (should be corect after-impulses correction)
//Same as 201015_Xray/2analysis_history.cpp but no PMTs at all
data_output_path = "180830_Xray/results_v2/";
calibration_file = data_output_path + "180830_Xray_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2; //not used

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["180830_Xray_0kV_600V_12bB_49V_1ph_35mm_coll"] = std::pair<double, double> (31.3, 43); //not the same as Vlad's!

std::map<std::string, std::string> exp_folders;
exp_folders["180830_Xray_0kV_600V_12bB_49V_1ph_35mm_coll"] = "Xray_49V_0kV_600V_35mm_coll";

bool forms = !calibrate;
//CALIBRATION (slow PMT and SiPMs)
for (std::size_t exp_ind = 0; calibrate && (exp_ind!= exp_area.experiments.size()); ++exp_ind) {
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
std::string exp = post_processor->experiments[post_processor->current_exp_index];
int PMT_state = 0; //600V, 12 dB
int SiPM_state = 0; //49V
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
//parameters set by Xray_49V_0kV_600V_35mm_coll
if (channel>=32) { //adsf
		if (getIndex(post_processor->MPPC_channels, channel)<0)
			continue;
ty(AStates::MPPC_A_S);
		ch(channel);
    set_zoom(0, 0.06, 0, 0.01);
    set_bins(500);
    noise_cut(channel, 0, SiPM_state, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_01_A_S_zoom");
ty(AStates::MPPC_Ss);
    set_zoom(0, 0.04);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_02_Ss");
    noise_cut(channel, 0, SiPM_state, false);
		cut_t(0, d_S2_start, false, channel);
    draw_limits(0.0005, 0.035);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_03_Ss_single_pe_w_0-"+S2_start+"us");
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
//zcxv
if (exp == "180830_Xray_0kV_600V_12bB_49V_1ph_35mm_coll") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

	save_forms(FOLDER + "forms/", false, PMT_state, SiPM_state);

	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	time_zoom_SiPMs(32, 38);
	set_bins(0, 400);
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_32-38us");
	Num = int_to_str(++no, 2);

	time_zoom_SiPMs(1, 30);
	update();
	saveaspng(FOLDER + Num+"_SiPMs_Npe_"+cuts_str(cuts)+"_w_1-30us");
	Num = int_to_str(++no, 2);
}
//END OF FORMS
}
