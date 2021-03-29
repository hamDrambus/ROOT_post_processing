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
//Introducing new histogram type: signal width (PMT_T_sum),
//calculated as a total width of all peaks average for active channels
data_output_path = "210218/results_v2/";
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
if (calibrate) {
	std::cout<<"Use 1analysis_history.cpp for calibration! Doing nothing."<<std::endl;
	return;
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
if (exp == "210218_Pu_5.5kV_850V_0V_12dB_th250mV") {
	std::vector<std::string> cuts;
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01" //picture number as string
	std::string FOLDER = data_output_path + folder + "/";

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

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
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

	time_zoom_fPMTs(0, 160);

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

	ty(AStates::PMT_T_sum);
	slow_PMTs_only();
	noise_cut(1, 0, PMT_state, 0);
	noise_cut(2, 0, PMT_state, 0);
	noise_cut(3, 0, PMT_state, 0);
	noise_cut(4, 0, PMT_state, 0);
	time_zoom_sPMTs(0, 160);
	set_zoom(0, 50);
	set_bins(500);
	saveaspng(FOLDER + Num+"_slowPMTs_Tsum_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_T_sum);
	fast_PMTs_only();
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	time_zoom_fPMTs(0, 160);
	set_zoom(0, 15);
	set_bins(500);
	saveaspng(FOLDER + Num+"_fastPMTs_Tsum_"+cuts_str(cuts));
	Num = int_to_str(++no, 2);

if (0 == method) {
	set_corr(AStates::PMT_Npe_sum, AStates::PMT_T_sum, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 2500, 0, 15);
	set_bins(2500, 500);
	set_titles("N_{pe} t#in[0, 160] #mus", "Signal width, [#mus]");
	saveaspng(FOLDER + Num+"_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));

	std::string cut_folder = "T_vs_Npe_v1/cut1/";
	x_y_regions = {251, 1.00, 207, 1.46, 285, 1.89, 339, 1.38};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "1");

	cut_folder = "T_vs_Npe_v1/cut2/";
	x_y_regions = {401, 1.75, 358, 2.19, 395, 2.41, 436, 2.01};
	cut_x_y_poly_select(x_y_regions, true, "2");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "2");

	cut_folder = "T_vs_Npe_v1/cut3/";
	x_y_regions = {467, 2.14, 430, 2.55, 461, 2.70, 502, 2.31};
	cut_x_y_poly_select(x_y_regions, true, "3");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "3");

	cut_folder = "T_vs_Npe_v1/cut4/";
	x_y_regions = {548, 2.62, 520, 2.96, 580, 3.16, 608, 2.82};
	cut_x_y_poly_select(x_y_regions, true, "4");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "4");

	cut_folder = "T_vs_Npe_v1/cut5/";
	x_y_regions = {721, 3.16, 668, 3.57, 733, 3.89, 790, 3.40};
	cut_x_y_poly_select(x_y_regions, true, "5");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "5");

	cut_folder = "T_vs_Npe_v1/cut6/";
	x_y_regions = {912, 4.06, 846, 4.69, 949, 5.18, 1024, 4.50};
	cut_x_y_poly_select(x_y_regions, true, "6");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "6");

	cut_folder = "T_vs_Npe_v1/cut7/";
	x_y_regions = {1275, 5.49, 1178, 6.27, 1400, 7.34, 1507, 6.37};
	cut_x_y_poly_select(x_y_regions, true, "7");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "7");

	cut_folder = "T_vs_Npe_v1/cut8/";
	x_y_regions = {639, 2.25, 605, 2.43, 671, 2.77, 718, 2.53};
	cut_x_y_poly_select(x_y_regions, true, "8");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "8");

	cut_folder = "T_vs_Npe_v1/cut9/";
	x_y_regions = {783, 2.60, 740, 3.06, 808, 3.33, 862, 2.82};
	cut_x_y_poly_select(x_y_regions, true, "9");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "9");

	cut_folder = "T_vs_Npe_v1/cut10/";
	x_y_regions = {1046, 2.99, 1012, 3.72, 1375, 4.37, 1400, 3.43};
	cut_x_y_poly_select(x_y_regions, true, "10");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "10");

	cut_folder = "T_vs_Npe_v1/cut11/";
	x_y_regions = {1692, 3.35, 1657, 4.67, 2133, 5.13, 2192, 3.62};
	cut_x_y_poly_select(x_y_regions, true, "11");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "11");

	cut_folder = "T_vs_Npe_v1/cut12/";
	x_y_regions = {1328, 4.59, 1297, 5.25, 1519, 6.08, 1579, 4.96};
	cut_x_y_poly_select(x_y_regions, true, "12");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "12");

	cut_folder = "T_vs_Npe_v1/cut13/";
	x_y_regions = {2014, 5.49, 1832, 9.92, 2289, 11.86, 2424, 6.20};
	cut_x_y_poly_select(x_y_regions, true, "13");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "13");

	cut_folder = "T_vs_Npe_v1/";
	x_y_regions = {251, 1.00, 207, 1.46, 285, 1.89, 339, 1.38};
	cut_x_y_poly_select(x_y_regions, true, "1");
	x_y_regions = {401, 1.75, 358, 2.19, 395, 2.41, 436, 2.01};
	cut_x_y_poly_select(x_y_regions, true, "2");
	x_y_regions = {467, 2.14, 430, 2.55, 461, 2.70, 502, 2.31};
	cut_x_y_poly_select(x_y_regions, true, "3");
	x_y_regions = {548, 2.62, 520, 2.96, 580, 3.16, 608, 2.82};
	cut_x_y_poly_select(x_y_regions, true, "4");
	x_y_regions = {721, 3.16, 668, 3.57, 733, 3.89, 790, 3.40};
	cut_x_y_poly_select(x_y_regions, true, "5");
	x_y_regions = {912, 4.06, 846, 4.69, 949, 5.18, 1024, 4.50};
	cut_x_y_poly_select(x_y_regions, true, "6");
	x_y_regions = {1275, 5.49, 1178, 6.27, 1400, 7.34, 1507, 6.37};
	cut_x_y_poly_select(x_y_regions, true, "7");
	x_y_regions = {639, 2.25, 605, 2.43, 671, 2.77, 718, 2.53};
	cut_x_y_poly_select(x_y_regions, true, "8");
	x_y_regions = {783, 2.60, 740, 3.06, 808, 3.33, 862, 2.82};
	cut_x_y_poly_select(x_y_regions, true, "9");
	x_y_regions = {1046, 2.99, 1012, 3.72, 1375, 4.37, 1400, 3.43};
	cut_x_y_poly_select(x_y_regions, true, "10");
	x_y_regions = {1692, 3.35, 1657, 4.67, 2133, 5.13, 2192, 3.62};
	cut_x_y_poly_select(x_y_regions, true, "11");
	x_y_regions = {1328, 4.59, 1297, 5.25, 1519, 6.08, 1579, 4.96};
	cut_x_y_poly_select(x_y_regions, true, "12");
	x_y_regions = {2014, 5.49, 1832, 9.92, 2289, 11.86, 2424, 6.20};
	cut_x_y_poly_select(x_y_regions, true, "13");
	update();
	saveaspng(FOLDER + cut_folder + "all_cuts");
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	remcut(-1, "5"); remcut(-1, "6"); remcut(-1, "7"); remcut(-1, "8");
	remcut(-1, "9"); remcut(-1, "10"); remcut(-1, "11"); remcut(-1, "12");
	remcut(-1, "13");
	Num = int_to_str(++no, 2);
} //method == 0
if (1 == method) {
	slow_PMTs_only();
	set_zoom(0, 50);
	set_bins(500);

	set_corr(AStates::PMT_Npe_sum, AStates::PMT_T_sum, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 2500, 0, 50);
	set_bins(2500, 500);
	set_titles("N_{pe} t#in[0, 160] #mus", "Signal width, [#mus]");
	saveaspng(FOLDER + Num+"_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));

	std::string cut_folder = "T_vs_Npe_v2/cut1/";
	x_y_regions = {361, 6.07, 239, 8.91, 317, 11.02, 455, 7.53};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "1");

	cut_folder = "T_vs_Npe_v2/cut2/";
	x_y_regions = {445, 11.18, 392, 12.23, 442, 13.69, 495, 12.56};
	cut_x_y_poly_select(x_y_regions, true, "2");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "2");

	cut_folder = "T_vs_Npe_v2/cut3/";
	x_y_regions = {530, 10.53, 530, 11.50, 608, 11.58, 608, 10.69};
	cut_x_y_poly_select(x_y_regions, true, "3");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "3");

	cut_folder = "T_vs_Npe_v2/cut4/";
	x_y_regions = {530, 10.53, 530, 11.50, 608, 11.58, 608, 10.69};
	cut_x_y_poly_select(x_y_regions, true, "4");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "4");

	cut_folder = "T_vs_Npe_v2/cut5/";
	x_y_regions = {508, 8.42, 508, 9.72, 617, 10.12, 624, 8.67};
	cut_x_y_poly_select(x_y_regions, true, "5");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "5");

	cut_folder = "T_vs_Npe_v2/cut6/";
	x_y_regions = {677, 10.77, 661, 12.07, 740, 12.64, 758, 11.18};
	cut_x_y_poly_select(x_y_regions, true, "6");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "6");

	cut_folder = "T_vs_Npe_v2/cut7/";
	x_y_regions = {940, 11.02, 934, 14.26, 1178, 15.07, 1191, 10.94};
	cut_x_y_poly_select(x_y_regions, true, "7");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "7");

	cut_folder = "T_vs_Npe_v2/cut8/";
	x_y_regions = {686, 15.15, 520, 18.39, 674, 22.93, 990, 16.45};
	cut_x_y_poly_select(x_y_regions, true, "8");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "8");

	cut_folder = "T_vs_Npe_v2/cut9/";
	x_y_regions = {1006, 19.20, 808, 22.61, 931, 26.34, 1131, 22.53};
	cut_x_y_poly_select(x_y_regions, true, "9");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Tsum_vs_fastPMTs_Npe_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "9");

	cut_folder = "T_vs_Npe_v2/";
	x_y_regions = {361, 6.07, 239, 8.91, 317, 11.02, 455, 7.53};
	cut_x_y_poly_select(x_y_regions, true, "1");
	x_y_regions = {445, 11.18, 392, 12.23, 442, 13.69, 495, 12.56};
	cut_x_y_poly_select(x_y_regions, true, "2");
	x_y_regions = {530, 10.53, 530, 11.50, 608, 11.58, 608, 10.69};
	cut_x_y_poly_select(x_y_regions, true, "3");
	x_y_regions = {530, 10.53, 530, 11.50, 608, 11.58, 608, 10.69};
	cut_x_y_poly_select(x_y_regions, true, "4");
	x_y_regions = {508, 8.42, 508, 9.72, 617, 10.12, 624, 8.67};
	cut_x_y_poly_select(x_y_regions, true, "5");
	x_y_regions = {677, 10.77, 661, 12.07, 740, 12.64, 758, 11.18};
	cut_x_y_poly_select(x_y_regions, true, "6");
	x_y_regions = {940, 11.02, 934, 14.26, 1178, 15.07, 1191, 10.94};
	cut_x_y_poly_select(x_y_regions, true, "7");
	x_y_regions = {686, 15.15, 520, 18.39, 674, 22.93, 990, 16.45};
	cut_x_y_poly_select(x_y_regions, true, "8");
	x_y_regions = {1006, 19.20, 808, 22.61, 931, 26.34, 1131, 22.53};
	cut_x_y_poly_select(x_y_regions, true, "9");
	update();
	saveaspng(FOLDER + cut_folder + "all_cuts");
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	remcut(-1, "5"); remcut(-1, "6"); remcut(-1, "7"); remcut(-1, "8");
	remcut(-1, "9");
	Num = int_to_str(++no, 2);
} //method == 1
if (2 == method) {
	ty(AStates::PMT_sum_N);
	time_zoom_fPMTs(0, 160);
	set_bins(0, 1000);
	saveaspng(FOLDER + Num+"_fastPMTs_Npeaks_"+cuts_str(cuts));

	std::string cut_folder = "Npeaks_v1/cut1/";
	draw_limits(120, 200);
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");

	cut_folder = "Npeaks_v1/cut2/";
	draw_limits(204, 325);
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");

	cut_folder = "Npeaks_v1/cut3/";
	draw_limits(340, 400);
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");

	cut_folder = "Npeaks_v1/cut4/";
	draw_limits(405, 540);
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	unset_draw_limits();

	cut_folder = "Npeaks_v1/cut5/";
	draw_limits(560, 800);
	saveaspng(FOLDER + cut_folder + Num + "_fastPMTs_Npeaks_"+cuts_str(cuts));
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	unset_draw_limits();

	Num = int_to_str(++no, 2);
}
if (3 == method) {
	ty(AStates::PMT_Npe_sum);
	slow_PMTs_only();
	set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 1500, 0, 1500);
	set_bins(1500);
	set_titles("N_{pe} t#in[0, 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");

	std::string cut_folder = "sPMTs_Fr_v1/cut1/";
	x_y_regions = {200, 0, 200, 140, 800, 200, 800, 0};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "1");

	cut_folder = "sPMTs_Fr_v1/cut2/";
	x_y_regions = {658, 221, 577, 301, 827, 552, 896, 452};
	cut_x_y_poly_select(x_y_regions, true, "2");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "2");

	cut_folder = "sPMTs_Fr_v1/cut3/";
	x_y_regions = {515, 253, 372, 275, 742, 639, 818, 557};
	cut_x_y_poly_select(x_y_regions, true, "3");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "3");

	cut_folder = "sPMTs_Fr_v1/cut4/";
	set_zoom(180, 800, 180, 800);
	set_bins(800-180);
	x_y_regions = {260, 231.5, 252.6, 243.4, 418.5, 404.0, 427.6, 391.1};
	cut_x_y_poly_select(x_y_regions, true, "4");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "4");

	cut_folder = "sPMTs_Fr_v1/cut5/";
	x_y_regions = {274.1, 265.3, 268.7, 271.4, 405.4, 409.1, 413.1, 400.0};
	cut_x_y_poly_select(x_y_regions, true, "5");
	update();
	saveaspng(FOLDER + cut_folder + Num + "_slowPMTs_Npe_"+cuts_str(cuts)+"_"+S2_start+"-"+S2_finish+"us_vs_0-160us");
	set_as_run_cut("t");
	print_accepted_events(FOLDER + cut_folder + "events.txt", first_run);
	unset_as_run_cut("t");
	remcut(-1, "5");

	set_zoom(0, 1500, 0, 1500);
	set_bins(1500);
	cut_folder = "sPMTs_Fr_v1/";
	x_y_regions = {200, 0, 200, 140, 800, 200, 800, 0};
	cut_x_y_poly_select(x_y_regions, true, "1");
	x_y_regions = {658, 221, 577, 301, 827, 552, 896, 452};
	cut_x_y_poly_select(x_y_regions, true, "2");
	x_y_regions = {515, 253, 372, 275, 742, 639, 818, 557};
	cut_x_y_poly_select(x_y_regions, true, "3");
	x_y_regions = {260, 231.5, 252.6, 243.4, 418.5, 404.0, 427.6, 391.1};
	cut_x_y_poly_select(x_y_regions, true, "4");
	x_y_regions = {274.1, 265.3, 268.7, 271.4, 405.4, 409.1, 413.1, 400.0};
	cut_x_y_poly_select(x_y_regions, true, "5");
	update();
	saveaspng(FOLDER + cut_folder + "all_cuts");
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	remcut(-1, "5");
	Num = int_to_str(++no, 2);
}
}
//END OF FORMS
}
