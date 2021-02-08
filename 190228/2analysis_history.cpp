void noise_cut(int channel, int aggressiveness, bool display);

void time_zoom(double from, double to) //fast PMTs
{
	cut_t(from, to, false, 2);
	cut_t(from, to, false, 3);
	cut_t(from, to, false, 4);
	cut_t(from, to, false, 5);
}

void save_forms (std::string path, bool N_only)
{
	ty(AStates::MPPC_tbNpe_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "SiPM38_form_by_Npe");

	ty(AStates::PMT_tbN);
	set_bins(800);
	ch(1);
	set_zoom(20, 90);
	set_bins(800);
	noise_cut(1, 0, false);
	saveas(path + "slowPMTs_form_by_Npe");
	ch(0);
	set_zoom(20, 90);
	set_bins(800);
	noise_cut(0, 0, false);
	saveas(path + "slowPMT2,3,4_form_by_Npe");
	ch(2);
	set_zoom(20, 90);
	set_bins(800);
	noise_cut(2, 0, false);
	saveas(path + "2_form_by_Npeaks");
    ch(3);
	set_zoom(20, 90);
	set_bins(800);
	noise_cut(3, 0, false);
	saveas(path + "3_form_by_Npeaks");
    ch(4);
	set_zoom(20, 90);
	set_bins(800);
	noise_cut(4, 0, false);
	saveas(path + "4_form_by_Npeaks");
    ch(5);
	set_zoom(20, 90);
	set_bins(800);
	noise_cut(5, 0, false);
	saveas(path + "5_form_by_Npeaks");
	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(1);
		set_zoom(20, 90);
		set_bins(800);
		noise_cut(1, 0, false);
		saveas(path + "slowPMTs_form_by_S");
		ch(0);
		set_zoom(20, 90);
		set_bins(800);
		noise_cut(0, 0, false);
		saveas(path + "slowPMT2,3,4_form_by_S");
		ch(2);
		set_zoom(20, 90);
		set_bins(800);
		noise_cut(2, 0, false);
		saveas(path + "2_form_by_S");
		ch(3);
		set_zoom(20, 90);
		set_bins(800);
		noise_cut(3, 0, false);
		saveas(path + "3_form_by_S");
		ch(4);
		set_zoom(20, 90);
		set_bins(800);
		noise_cut(4, 0, false);
		saveas(path + "4_form_by_S");
		ch(5);
		set_zoom(20, 90);
		set_bins(800);
		noise_cut(5, 0, false);
		saveas(path + "5_form_by_S");
	}
}

//Even if it is hard to read the whole analysis, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) {
std::vector<double> x_y_regions;
if (channel==0) {
    x_y_regions = {0.035, 0.035, 0.0012, 0.2, 0.005, 0.5};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_S_noise");
	if (aggressiveness ==1) {
    	cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.6, 1, 0.8, display, channel, "rem_S>~0.7");
	}
}
if (channel==1) {
	x_y_regions = {0.035, 0.035, 0.0012, 0.2, 0.01, 0.5};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_S_noise");
	if (aggressiveness ==1) {
		cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.7, 1, 0.9, display, channel, "rem_S>0.8");
	}
}
if (channel==2) {
    x_y_regions = {0.003, 0.0035, 0.00003, 0.008, 0.00007, 0.018};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.25, 0, 2, 0, display, channel, "rem_A>0.25");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==3) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.018};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.25, 0, 2, 0, display, channel, "rem_A>0.25");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==4) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.02};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.25, 0, 2, 0, display, channel, "rem_A>0.25");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==5) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.05};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.25, 0, 2, 0, display, channel, "rem_A>0.25");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==7) {
    x_y_regions = {0.015, 0.015, 0.0013, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.015, 0.015, 0.00065, 0.07, 0.005, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
    cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04"); //removes non-uniform noise from signal forms
    if (aggressiveness>=1) {//select only 1 photoelectron for calibration
        cut_A_S_upper(0.031, 0.0001, 1, 0.0001, display, channel, "2pe");
    }
	if (aggressiveness>=2) {//remove merger photoelectorns
        cut_A_S_upper(0.015, 0.0016, 0.031, 0.0038, display, channel, "2pe_merged");
	}
}
}

void analysis_history(bool calibrate, int method) {
//Created on 20.11.2019
//method = 0 - detailed analysis of 20kV 500mV: different combination of steps, how cuts on trigger affect Npe 2D distribution in two time windows (25-40us)vs(25-160us) and vise-verse + printing event examples (thier run-subrun) for different histogram areas. In addition, different signal forms are made for different time window of trigger adjustment algoritm for slowPMT and fastPMT.
//!!! This was done for SignalOperations::find_trigger_v1 returning left front of found cluster. It introduces artifacts, so I changed it to return weighted by S average t of the peaks. Use method = 1 for the new code version.
//method = 1 - same as method 0, but with new trigger adjustment algorithm (TriggerVersion::trigger_v2)
//method = 2 - TriggerVersion::trigger_v3
calibration_file = data_output_path+"190228_calibration.dat";
switch(method) {
case 0: {
	trigger_version = TriggerVersion::trigger_v1;	
	data_output_path = "190228/results_v2/";
	break;
}
case 1: {
	trigger_version = TriggerVersion::trigger_v2;
	data_output_path = "190228/results_v3/";
	break;
}
case 2: {
	trigger_version = TriggerVersion::trigger_v3;
	data_output_path = "190228/results_v4/";
	break;
}
default: {
	std::cerr<<"method="<<method<<" is not supported, exiting."<<std::endl;
	return;
}
}
post_processor->calibr_info.Load(calibration_file);
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190228_Cd_20kV_850V_46V_th500mV"] = std::pair<double, double> (22, 42);
S2_times["190228_Cd_20kV_850V_46V_th600mV"] = std::pair<double, double> (22, 42);
S2_times["190228_Cd_18kV_850V_46V_th500mV"] = std::pair<double, double> (22, 42);
S2_times["190228_Cd_16kV_850V_46V_th500mV"] = std::pair<double, double> (22, 42);
S2_times["190228_Bkg_20kV_850V_46V_th500mV"] = std::pair<double, double> (22, 42);

std::map<std::string, std::string> exp_folders;
exp_folders["190228_Cd_20kV_850V_46V_th500mV"] = "Cd_46V_20kV_850V_th500mV";
exp_folders["190228_Cd_20kV_850V_46V_th600mV"] = "Cd_46V_20kV_850V_th600mV";
exp_folders["190228_Cd_18kV_850V_46V_th500mV"] = "Cd_46V_18kV_850V";
exp_folders["190228_Cd_16kV_850V_46V_th500mV"] = "Cd_46V_16kV_850V";
exp_folders["190228_Bkg_20kV_850V_46V_th500mV"] = "Bkg_46V_20kV_850V";

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
for (std::size_t chi = 0, chi_end_ = calib_channels.size(); chi!=chi_end_; ++chi) { 
int channel = calib_channels[chi];
//parameters set by Cd_20kV_850V_46V, but valid for all fields. 
if (channel==7) {
ch(7);
    ty(AStates::MPPC_A_S);
    set_zoom(0, 0.072, 0, 0.012);
    set_bins(500);
    noise_cut(7, 1, true);
    saveas(data_output_path + folder + "/calibration/7_A_S_zoom");
    ty(AStates::MPPC_Ss);
    set_zoom(0, 0.009);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/7_Ss");    
    noise_cut(7, 1, false);
    draw_limits(0.0005, 0.004);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/7_Ss_single_pe");
}
if (channel==0) {
ch(0);
    ty(AStates::PMT_A_S);
    set_zoom(0, 1.2, 0, 1);
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/0_A_S");
    set_zoom(0, 0.15, 0, 0.04);        
    set_bins(100);
    saveas(data_output_path + folder+"/calibration/0_A_S_zoom");
    
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.15);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/0_Ss_w_0-"+S2_start+"us");
    noise_cut(channel, 0, false);        
    draw_limits(0, 0.040);
    set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder+"/calibration/0_Ss_w_0-"+S2_start+"us_A_S_cuts");
}
if (channel==1) {
ch(1);
    ty(AStates::PMT_A_S);
    set_zoom(0, 1.2, 0, 1.2);
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/1_A_S");    
    set_zoom(0, 0.8, 0, 0.2);        
    set_bins(600);
    saveas(data_output_path + folder+"/calibration/1_A_S_zoom");
    
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.15);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    noise_cut(channel, 0, false);        
    draw_limits(0.002, 0.042);
    set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder+"/calibration/1_Ss_w_0-"+S2_start+"us");
}
if (channel==2) {
ch(2);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.02, 0, 0.0005);        
    set_bins(300);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/2_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.0012);
    set_bins(400);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/2_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/2_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/2_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
}
if (channel==3) {
ch(3);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.02, 0, 0.0005);        
    set_bins(300);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/3_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.0016);
    set_bins(400);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/3_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/3_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/3_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
}
if (channel==4) {
ch(4);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.02, 0, 0.0007);        
    set_bins(400);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/4_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/4_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/4_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/4_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
}
if (channel==5) {
ch(5);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.02, 0, 0.0005);        
    set_bins(300);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/5_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/5_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/5_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/5_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
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

if (exp == "190228_Cd_20kV_850V_46V_th500mV" && forms && 0==method) {
	std::string meth = "";
	std::string path = "";
	ty(AStates::PMT_Npe_sum);
	ch(1);
	noise_cut(0, 0, false);
	noise_cut(1, 0, false);
	cut_t(d_S2_start, 160, false, 1);
	set_bins(0, 300);
	saveaspng(data_output_path + folder + "/different_methods/01_slowPMTs_Npe_no_cuts_w_"+S2_start+"-160us");
	cut_t(0, d_S2_start, false, 1);
	set_bins(0, 60);
	saveaspng(data_output_path + folder + "/different_methods/02_slowPMTs_Npe_no_cuts_w_0-"+S2_start+"us");
	cut_t(d_S2_start, d_S2_finish, false, 1);
	set_bins(0, 120);
	saveaspng(data_output_path + folder + "/different_methods/03_slowPMTs_Npe_no_cuts_w_"+S2_start+"-"+S2_finish+"us");
	cut_t(d_S2_start, 160, false, 1); //for future correlation
	set_bins(0, 300);	

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	time_zoom(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(data_output_path + folder + "/different_methods/04_fastPMTs_Npe_no_cuts_w_"+S2_start+"-160us");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/05_slowPMTs_Npe_no_cuts_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cut_x_y_right_select(80, 30, 150, 80, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/05_events_sPMTs_right_noise/events_sPMTs_right_noise";
	saveaspng(path);
	print_accepted_events(path+ ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");

	cut_x_y_lower_select(40, 5, 60, 20, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/05_events_sPMTs_lower_noise/events_sPMTs_lower_noise";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");

	cut_x_y_upper_select(58, 26, 110, 65, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/05_events_sPMTs_upper_signal/events_sPMTs_upper_signal";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/06_fastPMTs_Npe_no_cuts_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");

ty(AStates::PMT_trigger_bNpe);
	set_zoom(0, 160);
	set_bins(640);
	ch(1); noise_cut(1, 0, false);
	set_trigger_shaping(4);
	set_X_title("t [#mus]");
	saveaspng(data_output_path + folder + "/different_methods/06_slowPMTs_trigger_no_cuts_dt=4us");
	
	draw_limits(5, 15);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/06_events_sPMTs_early/events_sPMTs_early";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	draw_limits(40, 60);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/06_events_sPMTs_late/events_sPMTs_late";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");

	set_zoom(20, 40);
	set_bins(400);
	draw_limits(27, 32);
	saveaspng(data_output_path + folder + "/different_methods/07_slowPMTs_trigger_no_cuts_dt=4us_zoom");
	draw_limits(28, 29);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/07_events_sPMTs_28-29us/events_sPMTs_28-29us";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	draw_limits(31.8, 32);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/07_events_sPMTs_32us/events_sPMTs_32us";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	draw_limits(27, 32);
	set_as_run_cut("valid_trigger");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/08_fastPMTs_Npe_cut_07_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/09_slowPMTs_Npe_cut_07_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cut_x_y_right_select(70, 25, 145, 80, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/09_events_sPMTs_right_noise/events_sPMTs_right_noise";
	saveaspng(path);
	print_accepted_events(path+ ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");
	cut_x_y_right(50, 18, 300, 195, true, "1");
	cut_x_y_right(38, 1, 42, 18, true, "2");
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/10_slowPMTs_Npe_cut_07_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	remcut(-1, "1");
	remcut(-1, "2");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	cut_x_y_right_select(60, 15, 300, 160, true, "1");
	update();
	saveaspng(data_output_path + folder + "/different_methods/11_fastPMTs_Npe_cut_07+10_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/11_events_fPMTs_right_noise/events_fPMTs_right_noise";
	saveaspng(path);
	print_accepted_events(path+ ".txt", 32, 1000);
	unset_as_run_cut("temp");
	cut_x_y_right(60, 15, 300, 160, true, "1");
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	remcut(-1, "1");

ty(AStates::PMT_Npe_sum);
	cut_t(d_S2_start, 160, false, 1);
	set_bins(0, 300);
	saveaspng(data_output_path + folder + "/different_methods/12_slowPMTs_Npe_cut_07+10+11_w_"+S2_start+"-160us");
	cut_t(0, d_S2_start, false, 1);
	set_bins(0, 60);
	saveaspng(data_output_path + folder + "/different_methods/13_slowPMTs_Npe_cut_07+10+11_w_0-"+S2_start+"us");
	cut_t(d_S2_start, d_S2_finish, false, 1);
	set_bins(0, 120);
	saveaspng(data_output_path + folder + "/different_methods/14_slowPMTs_Npe_cut_07+10+11_w_"+S2_start+"-"+S2_finish+"us");
	cut_t(d_S2_start, 160, false, 1); //for future correlation
	set_bins(0, 300);	

ty(AStates::PMT_sum_N);
	saveaspng(data_output_path + folder + "/different_methods/15_fastPMTs_Npe_cut_07+10+11_w_"+S2_start+"-160us");

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/16_slowPMTs_trigger_cut_07+10+11_dt=4us");

ty(AStates::PMT_trigger_bNpeaks);
	set_zoom(0, 160);
	set_bins(640);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_trigger_shaping(4);
	set_X_title("t [#mus]");
	saveaspng(data_output_path + folder + "/different_methods/17_fastPMTs_trigger_cut_07+10+11_dt=4us");
	set_zoom(20, 40);
	set_bins(400);
	saveaspng(data_output_path + folder + "/different_methods/18_fastPMTs_trigger_cut_07+10+11_dt=4us_zoom");

ty(AStates::PMT_trigger_bNpe);
	set_trigger_offsets(30);
	saveaspng(data_output_path + folder + "/different_methods/19_slowPMTs_trigger_cut_07+10+11_dt=4us_zoom");
ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(data_output_path + folder + "/different_methods/20_fastPMTs_trigger_cut_07+10+11_dt=4us_zoom");

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/21_slowPMTs_Npe_cut_07+10+11+19_w_"+S2_start+"-160us");
ty(AStates::PMT_sum_N);
	saveaspng(data_output_path + folder + "/different_methods/22_fastPMTs_Npe_cut_07+10+11+19+21_w_"+S2_start+"-160us");
ty(AStates::MPPC_Npe_sum);
	cut_t(d_S2_start, 160, false, 7);
	noise_cut(7, 0, false);
	set_bins(0, 50);
	saveaspng(data_output_path + folder + "/different_methods/23_SiPM_Npe_cut_07+10+11+19+21_w_"+S2_start+"-160us");

	path = data_output_path + folder + "/different_methods/21_sPMT_trigger_4us_cut_07+10+11+19+21/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpeaks);
	set_zoom(28, 34);
	draw_limits(29.85, 30.0);
	set_as_run_cut("fastPMT_trigger");
	saveaspng(data_output_path + folder + "/different_methods/24_fastPMTs_trigger_cut_07+10+11+19+21_dt=4us_zoom");

ty(AStates::PMT_tbN);
	path = data_output_path + folder + "/different_methods/24_sPMT_trigger_4us_cut_07+10+11+19+21+24/";
save_forms(path, false);

	//unset_as_run_cut("valid_trigger");
	//Not finished as I realized there is issue with trigger adjustment algorithm (will always introduce peak at the start of fast component for devices (channels) from which trigger was calculated).
}

if (exp == "190228_Cd_20kV_850V_46V_th500mV" && forms && 1==method) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping = 4;
	std::string dt = "_dt=4us";
	std::string cuts = "no_cuts";
//Loot at spectra in different time windows without cuts
ty(AStates::PMT_Npe_sum);
	ch(1);
	noise_cut(0, 0, false);
	noise_cut(1, 0, false);
	cut_t(d_S2_start, 160, false, 1);
	set_bins(0, 300);
	saveaspng(data_output_path + folder + "/different_methods/01_slowPMTs_Npe_no_cuts_w_"+S2_start+"-160us");
	cut_t(0, d_S2_start, false, 1);
	set_bins(0, 60);
	saveaspng(data_output_path + folder + "/different_methods/02_slowPMTs_Npe_no_cuts_w_0-"+S2_start+"us");
	cut_t(d_S2_start, d_S2_finish, false, 1);
	set_bins(0, 120);
	saveaspng(data_output_path + folder + "/different_methods/03_slowPMTs_Npe_no_cuts_w_"+S2_start+"-"+S2_finish+"us");
	cut_t(d_S2_start, 160, false, 1); //for future correlation
	set_bins(0, 300);	

ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	time_zoom(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(data_output_path + folder + "/different_methods/04_fastPMTs_Npe_no_cuts_w_"+S2_start+"-160us");
//Look at fPMTs and sPMT S2 in time window correlation. Print event examples
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/05_slowPMTs_Npe_no_cuts_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cut_x_y_right_select(80, 30, 150, 80, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/05_events_sPMTs_right_noise/events_sPMTs_right_noise";
	saveaspng(path);
	print_accepted_events(path+ ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");

	cut_x_y_lower_select(40, 5, 60, 20, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/05_events_sPMTs_lower_noise/events_sPMTs_lower_noise";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");

	cut_x_y_upper_select(58, 26, 110, 65, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/05_events_sPMTs_upper_signal/events_sPMTs_upper_signal";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/06_fastPMTs_Npe_no_cuts_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
//Look at slowPMTs trigger, print event examples.
//Select events with valid trigger only
ty(AStates::PMT_trigger_bNpe);
	set_zoom(0, 160);
	set_bins(640);
	ch(1); noise_cut(1, 0, false);
	set_trigger_shaping(4);
	set_X_title("t [#mus]");
	saveaspng(data_output_path + folder + "/different_methods/06_slowPMTs_trigger_no_cuts_dt=4us");
	
	draw_limits(5, 25);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/06_events_sPMTs_early/events_sPMTs_early";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	draw_limits(40, 90);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/06_events_sPMTs_late/events_sPMTs_late";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");

	set_zoom(15, 40);
	set_bins(400);
	draw_limits(29.5, 35);
	saveaspng(data_output_path + folder + "/different_methods/07_slowPMTs_trigger_no_cuts_dt=4us_zoom");
	draw_limits(30.5, 31);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/07_events_sPMTs_28-29us/events_sPMTs_28-29us";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	draw_limits(31.9, 32.1);
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/07_events_sPMTs_32us/events_sPMTs_32us";
	saveaspng(path);
	print_accepted_events(path + ".txt", 32, 1000);
	unset_as_run_cut("temp");
	draw_limits(29.5, 35);
	set_as_run_cut("valid_trigger_sPMTs");
//Look at fPMTs S2 time window correlation with valid sPMT trigger
set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/08_fastPMTs_Npe_cut_07_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
//Look at fPMTs S2 time window correlation with valid sPMT trigger
//Select valid events here. Print event examples
set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	saveaspng(data_output_path + folder + "/different_methods/09_slowPMTs_Npe_cut_07_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cut_x_y_right_select(70, 25, 145, 80, true, "1");
	update();
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/09_events_sPMTs_right_noise/events_sPMTs_right_noise";
	saveaspng(path);
	print_accepted_events(path+ ".txt", 32, 1000);
	unset_as_run_cut("temp");
	remcut(-1, "1");
	cut_x_y_right(50, 18, 300, 195, true, "1");
	cut_x_y_right(38, 1, 42, 18, true, "2");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/10_slowPMTs_Npe_cut_07_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	remcut(-1, "1");
	remcut(-1, "2");
//Look at fPMTs S2 time window correlation with valid sPMT trigger and valid sPMT Npe[22,42us]vsNpe[22,160us]
//Select valid events, print example of invalid ones.
set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	cut_x_y_right_select(60, 15, 300, 160, true, "1");
	update();
	saveaspng(data_output_path + folder + "/different_methods/11_fastPMTs_Npe_cut_07+10_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	set_as_run_cut("temp");
	path = data_output_path + folder + "/different_methods/11_events_fPMTs_right_noise/events_fPMTs_right_noise";
	saveaspng(path);
	print_accepted_events(path+ ".txt", 32, 1000);
	unset_as_run_cut("temp");
	cut_x_y_right(60, 15, 300, 160, true, "1");
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	remcut(-1, "1");
//Look at the effect of sPMT trigger, sPMTs Npe[22,42us]vsNpe[22,160us] and fPMTs Npeaks[22,42us]vsNpe[22,160us] cuts
//on Npe spectra.
ty(AStates::PMT_Npe_sum);
	cut_t(d_S2_start, 160, false, 1);
	set_bins(0, 300);
	saveaspng(data_output_path + folder + "/different_methods/12_slowPMTs_Npe_cut_07+10+11_w_"+S2_start+"-160us");
	cut_t(0, d_S2_start, false, 1);
	set_bins(0, 60);
	saveaspng(data_output_path + folder + "/different_methods/13_slowPMTs_Npe_cut_07+10+11_w_0-"+S2_start+"us");
	cut_t(d_S2_start, d_S2_finish, false, 1);
	set_bins(0, 120);
	saveaspng(data_output_path + folder + "/different_methods/14_slowPMTs_Npe_cut_07+10+11_w_"+S2_start+"-"+S2_finish+"us");
	cut_t(d_S2_start, 160, false, 1); //for future correlation
	set_bins(0, 300);	

ty(AStates::PMT_sum_N);
	saveaspng(data_output_path + folder + "/different_methods/15_fastPMTs_Npe_cut_07+10+11_w_"+S2_start+"-160us");
//Look at sPMTs trigger
ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/16_slowPMTs_trigger_cut_07+10+11_dt=4us");
//Look at fPMTs trigger
ty(AStates::PMT_trigger_bNpeaks);
	set_zoom(0, 160);
	set_bins(640);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_trigger_shaping(4);
	set_X_title("t [#mus]");
	saveaspng(data_output_path + folder + "/different_methods/17_fastPMTs_trigger_cut_07+10+11_dt=4us");
	set_zoom(15, 40);
	set_bins(400);
	saveaspng(data_output_path + folder + "/different_methods/18_fastPMTs_trigger_cut_07+10+11_dt=4us_zoom");
//Apply sPMTs trigger adjustment
ty(AStates::PMT_trigger_bNpe);
	set_trigger_offsets(25);
	saveaspng(data_output_path + folder + "/different_methods/19_slowPMTs_trigger_cut_07+10+11_dt=4us_zoom");
//Look at fPMTs trigger with sPMTs trigger adjustment
ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(data_output_path + folder + "/different_methods/20_fastPMTs_trigger_cut_07+10+11_dt=4us_zoom");
//Select Cd events by sPMTs
ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/21_slowPMTs_Npe_cut_07+10+11+19_w_"+S2_start+"-160us");
//Look at Npe for Cd events for fPMTs and SiPM
ty(AStates::PMT_sum_N);
	saveaspng(data_output_path + folder + "/different_methods/22_fastPMTs_Npe_cut_07+10+11+19+21_w_"+S2_start+"-160us");
ty(AStates::MPPC_Npe_sum);
	cut_t(d_S2_start, 160, false, 7);
	noise_cut(7, 0, false);
	set_bins(0, 50);
	saveaspng(data_output_path + folder + "/different_methods/23_SiPM_Npe_cut_07+10+11+19+21_w_"+S2_start+"-160us");
//Print forms for sPMT trigger adjusment and all cuts 
	path = data_output_path + folder + "/different_methods/21_sPMT_trigger_4us_cut_07+10+11+19+21/";
save_forms(path, false);
//Apply cut on fPMTs trigger with sPMTs adjustment
ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(23, 27.2);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/24_fastPMTs_trigger_cut_07+10+11+19+21_dt=4us_zoom");

//Print forms for sPMT trigger adjusment and all cuts 
	path = data_output_path + folder + "/different_methods/24_sPMT_trigger_4us_cut_07+10+11+19+21+24/";
save_forms(path, false);

	//NOW USE fastPMTs 4US FOR TRIGGER CORRECTION
	//Since sPMTs and fPMTs t 22-42 vs 22-160 us cuts were created after trigger cut,
	//for correct procedure comparison all cuts must be redone after fPMT trigger cut
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(29, 35);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/25_fastPMTs_no_cuts_dt=4us_zoom");

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/26_slowPMTs_Npe_cut_25_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 8, 300, 170, true, "1");
	cut_x_y_right(25, 0, 30, 8, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/27_fastPMTs_Npe_cut_25+26_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/28_slowPMTs_trigger_cut_25+26+27_dt=4us");

ty(AStates::PMT_trigger_bNpeaks);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/29_fastPMTs_trigger_cut_25+26+27_dt=4us");
	set_trigger_offsets(25);

ty(AStates::PMT_trigger_bNpe);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/30_slowPMTs_trigger_cut_25+26+27+29_dt=4us");

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/31_slowPMTs_Npe_cut_25+26+27+29_w_"+S2_start+"-160us");

	path = data_output_path + folder + "/different_methods/31_fPMT_trigger_4us_cut_25+26+27+29+31/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(21, 29);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/32_slowPMTs_trigger_cut_25+26+27+29+31_dt=4us_zoom");

ty(AStates::PMT_tbN);
	path = data_output_path + folder + "/different_methods/32_fPMT_trigger_4us_cut_25+26+27+29+31+32/";
save_forms(path, false);

	//NOW USE slowPMTs 3US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_trigger_offsets();
	dt = "_dt=3us";
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpe);
	set_trigger_shaping(3);
	draw_limits(30, 35);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/33_slowPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_33";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/34_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+34";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(40, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/35_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+35";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	set_trigger_shaping(3);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/36_fastPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/37_slowPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+37";

ty(AStates::PMT_trigger_bNpeaks);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/38_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/39_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+39";

	path = data_output_path + folder + "/different_methods/39_sPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(23.5, 26.4);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/40_fastPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+40";

	path = data_output_path + folder + "/different_methods/40_sPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

	//NOW USE fastPMTs 3US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	dt = "_dt=3us";
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpeaks);
	set_trigger_shaping(3);
	draw_limits(30, 34);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/41_fastPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_41";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/42_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+42";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(40, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/43_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+43";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/44_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpeaks);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/45_fastPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+45";

ty(AStates::PMT_trigger_bNpe);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/46_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/47_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+47";

	path = data_output_path + folder + "/different_methods/47_fPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(23, 27);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/48_slowPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+48";

	path = data_output_path + folder + "/different_methods/48_fPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

	//NOW USE slowPMTs 2US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_trigger_offsets();
	dt = "_dt=2us";
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpe);
	set_trigger_shaping(2);
	draw_limits(30.5, 34);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/49_slowPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_49";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/50_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+50";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(40, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/51_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+51";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	set_trigger_shaping(2);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/52_fastPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/53_slowPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+53";

ty(AStates::PMT_trigger_bNpeaks);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/54_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/55_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+55";

	path = data_output_path + folder + "/different_methods/55_sPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(23.3, 26.7);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/56_fastPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+56";

	path = data_output_path + folder + "/different_methods/56_sPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

	//NOW USE fastPMTs 2US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	dt = "_dt=2us";
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(30.4, 33.6);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/57_fastPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_57";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/58_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+58";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(40, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/59_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+61";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/59_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpeaks);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/60_fastPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+63";

ty(AStates::PMT_trigger_bNpe);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/61_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/62_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+65";

	path = data_output_path + folder + "/different_methods/62_fPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(23.3, 26.5);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/63_slowPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+66";

	path = data_output_path + folder + "/different_methods/63_fPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

	//NOW USE slowPMTs 1US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_trigger_offsets();
	dt_shaping = 1;
	dt = "_dt=1us";
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpe);
	set_trigger_shaping(dt_shaping);
	draw_limits(30.5, 34);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/64_slowPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_64";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/65_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+65";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/66_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+66";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	set_trigger_shaping(dt_shaping);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/67_fastPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/68_slowPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+68";

ty(AStates::PMT_trigger_bNpeaks);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/69_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/70_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+70";

	path = data_output_path + folder + "/different_methods/70_sPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(23.3, 26);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/71_fastPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+71";

	path = data_output_path + folder + "/different_methods/71_sPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

	//NOW USE fastPMTs 1US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(30.0, 33.8);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/72_fastPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_72";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/73_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+73";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/74_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+74";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/75_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpeaks);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/76_fastPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+76";

ty(AStates::PMT_trigger_bNpe);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/77_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/78_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+78";

	path = data_output_path + folder + "/different_methods/78_fPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(24, 26.5);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/79_slowPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+79";

	path = data_output_path + folder + "/different_methods/79_fPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

	//NOW USE slowPMTs 5US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_trigger_offsets();
	dt_shaping = 5;
	dt = "_dt=5us";
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpe);
	set_trigger_shaping(dt_shaping);
	draw_limits(29, 35);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/80_slowPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_80";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/81_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+81";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/82_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+82";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	set_trigger_shaping(dt_shaping);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/83_fastPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/84_slowPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+84";

ty(AStates::PMT_trigger_bNpeaks);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/85_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/86_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+86";

	path = data_output_path + folder + "/different_methods/86_sPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(23, 26.5);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/87_fastPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+87";

	path = data_output_path + folder + "/different_methods/87_sPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

	//NOW USE fastPMTs 5US FOR TRIGGER CORRECTION
	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	cuts = "no_cuts";

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(28.5, 35);
	set_as_run_cut("valid_trigger_fPMTs");
	saveaspng(data_output_path + folder + "/different_methods/88_fastPMTs_"+cuts+""+dt+"_zoom");
	cuts = "cut_88";

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/89_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+89";
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms");
	saveaspng(data_output_path + folder + "/different_methods/90_fastPMTs_Npe_"+cuts+"_"+S2_start+"-160us_vs_"+S2_start+"-"+S2_finish+"us");
	cuts += "+90";
	remcut(-1, "1");
	remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/91_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_trigger_bNpeaks);
	unset_draw_limits();
	saveaspng(data_output_path + folder + "/different_methods/92_fastPMTs_trigger_"+cuts+""+dt);
	set_trigger_offsets(25);
	cuts += "+92";

ty(AStates::PMT_trigger_bNpe);
	//Added cut to this histogram below
	saveaspng(data_output_path + folder + "/different_methods/93_slowPMTs_trigger_"+cuts+""+dt);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 95);
	set_as_run_cut("Cd_peak");
	saveaspng(data_output_path + folder + "/different_methods/94_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	cuts += "+94";

	path = data_output_path + folder + "/different_methods/94_fPMT_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(23, 27);
	set_as_run_cut("valid_trigger_sPMTs");
	saveaspng(data_output_path + folder + "/different_methods/95_slowPMTs_trigger_"+cuts+""+dt+"_zoom");
	cuts += "+95";

	path = data_output_path + folder + "/different_methods/95_fPMTs_trigger"+dt+"_"+cuts+"/";
save_forms(path, false);
}


if (exp == "190228_Cd_20kV_850V_46V_th500mV" && forms && 2==method) {
	std::string meth = "";
	std::string path = "";
	double dt_shaping = 1;
	std::string dt = "dt=1us";
	std::string DT = "dt=1#mus";
	std::string cuts = "no_cuts";
	int no = 0; //number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/different_methods/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	ch(1); noise_cut(1, 0, 0); noise_cut(0, 0, 0);
	cut_t(d_S2_start, 160, false, 1);
	cut_t(d_S2_start, 160, false, 0);
	set_bins(18, 300);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0);
	noise_cut(3, 0, 0);
	noise_cut(4, 0, 0);
	noise_cut(5, 0, 0);
	time_zoom(d_S2_start, 160);
	set_bins(0, 200);

	//Set AStates::PMT_trigger_bNpe and AStates::PMT_trigger_bNpeaks for future correlations
	ty(AStates::PMT_trigger_bNpe);
	ch(1); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	ty(AStates::PMT_trigger_bNpeaks);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0);
	noise_cut(3, 0, 0);
	noise_cut(4, 0, 0);
	noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);

	//USE slowPMTs 1US FOR TRIGGER CORRECTION trigger_v3
set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(600);
	set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt);
	Num = int_to_str(++no, 2);
	set_zoom(25, 45, 25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(30.5, 34);
	set_as_run_cut("valid_trigger_sPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(27, 30);
	set_as_run_cut("valid_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_slowPMTs_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	cuts = "no_cuts";

	//NOW USE fastPMTs 1US FOR TRIGGER CORRECTION

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(29.8, 34);
	set_as_run_cut("valid_trigger_fPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_trigger"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_falstPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(28, 30.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_fastPMT_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();

	//USE 2us TRIGGERS
	dt_shaping = 2;
	dt = "dt=2us";
	DT = "dt=2#mus";
	cuts = "no_cuts";
	
	//Set AStates::PMT_trigger_bNpe and AStates::PMT_trigger_bNpeaks for future correlations
	ty(AStates::PMT_trigger_bNpe);
	ch(1); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	ty(AStates::PMT_trigger_bNpeaks);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0);
	noise_cut(3, 0, 0);
	noise_cut(4, 0, 0);
	noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	//USE slowPMTs 2US FOR TRIGGER CORRECTION trigger_v3

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(600);
	set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt);
	Num = int_to_str(++no, 2);
	set_zoom(25, 45, 25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(30.5, 34);
	set_as_run_cut("valid_trigger_sPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(27.5, 30.5);
	set_as_run_cut("valid_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_slowPMTs_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	cuts = "no_cuts";

	//NOW USE fastPMTs 2US FOR TRIGGER CORRECTION

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(30.5, 33.5);
	set_as_run_cut("valid_trigger_fPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_trigger"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_falstPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(27, 31.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_fastPMT_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();

	//USE 3us TRIGGERS
	dt_shaping = 3;
	dt = "dt=3us";
	DT = "dt=3#mus";
	cuts = "no_cuts";
	
	//Set AStates::PMT_trigger_bNpe and AStates::PMT_trigger_bNpeaks for future correlations
	ty(AStates::PMT_trigger_bNpe);
	ch(1); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	ty(AStates::PMT_trigger_bNpeaks);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0);
	noise_cut(3, 0, 0);
	noise_cut(4, 0, 0);
	noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	//USE slowPMTs 2US FOR TRIGGER CORRECTION trigger_v3

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(600);
	set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt);
	Num = int_to_str(++no, 2);
	set_zoom(25, 45, 25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(30.5, 34.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(27.5, 30.5);
	set_as_run_cut("valid_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_slowPMTs_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	cuts = "no_cuts";

	//NOW USE fastPMTs 3US FOR TRIGGER CORRECTION

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(30.5, 33.5);
	set_as_run_cut("valid_trigger_fPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_trigger"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_falstPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(27.5, 30.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_fastPMT_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();

	//USE 4us TRIGGERS
	dt_shaping = 4;
	DT = "dt=4#mus";
	dt = "dt=4us";
	cuts = "no_cuts";
	
	//Set AStates::PMT_trigger_bNpe and AStates::PMT_trigger_bNpeaks for future correlations
	ty(AStates::PMT_trigger_bNpe);
	ch(1); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	ty(AStates::PMT_trigger_bNpeaks);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0);
	noise_cut(3, 0, 0);
	noise_cut(4, 0, 0);
	noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	//USE slowPMTs 4US FOR TRIGGER CORRECTION trigger_v3

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(600);
	set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt);
	Num = int_to_str(++no, 2);
	set_zoom(25, 45, 25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(30.5, 34.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(27.0, 31.5);
	set_as_run_cut("valid_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_slowPMTs_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	cuts = "no_cuts";

	//NOW USE fastPMTs 4US FOR TRIGGER CORRECTION

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(29.8, 35.0);
	set_as_run_cut("valid_trigger_fPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_trigger"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_falstPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(27.5, 30.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_fastPMT_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();

	//USE 4us TRIGGERS
	dt_shaping = 5;
	DT = "dt=5#mus";
	dt = "dt=5us";
	cuts = "no_cuts";
	
	//Set AStates::PMT_trigger_bNpe and AStates::PMT_trigger_bNpeaks for future correlations
	ty(AStates::PMT_trigger_bNpe);
	ch(1); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	ty(AStates::PMT_trigger_bNpeaks);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0);
	noise_cut(3, 0, 0);
	noise_cut(4, 0, 0);
	noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	
	//USE slowPMTs 5US FOR TRIGGER CORRECTION trigger_v3

set_corr(AStates::PMT_trigger_bNpe, AStates::PMT_trigger_bNpeaks, -1, -1);
	ty(AStates::Correlation);
	set_zoom(0, 160, 0, 160);
	set_bins(600);
	set_titles("t by slowPMTs " + DT, "t by fastPMTs " + DT);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt);
	Num = int_to_str(++no, 2);
	set_zoom(25, 45, 25, 45);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_vs_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(30.5, 34.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	set_zoom(0, 300, 0, 300);
	set_bins(300);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(27.0, 31.0);
	set_as_run_cut("valid_trigger_fPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_slowPMTs_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
	cuts = "no_cuts";

	//NOW USE fastPMTs 5US FOR TRIGGER CORRECTION

ty(AStates::PMT_trigger_bNpeaks);
	draw_limits(29.3, 35.5);
	set_as_run_cut("valid_trigger_fPMTs"); cuts = "cut_" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_trigger"+cuts+"_"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	cut_t(d_S2_start, d_S2_finish, false, 1);
	ty(AStates::Correlation);
	cut_x_y_right(30, 5, 300, 195, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_sPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1");

set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	cut_x_y_right(40, 15, 300, 170, true, "1");
	cut_x_y_right(30, 0, 40, 15, true, "2");
	set_titles("N_{peaks} t#in["+S2_start+", 160] #mus", "N_{peaks} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	set_as_run_cut("good_fPMTs_Npe_t_zooms"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2");

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num + "_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num + "_falstPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(29); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	draw_limits(27.5, 30.5);
	set_as_run_cut("valid_trigger_sPMTs"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_fastPMTs_trigger_"+cuts+"+"+dt+"_zoom");
	unset_draw_limits();
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(45, 100);
	set_as_run_cut("Cd_peak"); cuts += "+" + Num;
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-160us");
	path = FOLDER + Num + "_fastPMT_trigger_"+dt+"_"+cuts+"/";
save_forms(path, false);
	Num = int_to_str(++no, 2);

	unset_as_run_cut("valid_trigger_sPMTs");
	unset_as_run_cut("good_sPMTs_Npe_t_zooms");
	unset_as_run_cut("good_fPMTs_Npe_t_zooms");
	unset_as_run_cut("Cd_peak");
	unset_as_run_cut("valid_trigger_fPMTs");
	unset_trigger_offsets();
}

//END OF FORMS
} 
