void noise_cut(int channel, int aggressiveness, bool display);
void time_zoom_fPMTs (double f_time, double to_time);
void time_zoom_sPMTs (double f_time, double to_time);
void save_forms (std::string path, bool N_only);

void time_zoom_fPMTs (double f_time, double to_time) {
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 2);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 3);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 4);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 5);
}

void time_zoom_sPMTs (double f_time, double to_time) {
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 0);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 1);
}

void save_forms (std::string path, bool N_only)
{
	ty(AStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(1000);
	saveas(path + "SiPM38_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(2);
	noise_cut(2, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "2_form_by_Npeaks");
	ch(3);
	noise_cut(3, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "3_form_by_Npeaks");
	ch(4);
	noise_cut(4, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "5_form_by_Npeaks");
    ch(0);
	noise_cut(0, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "slowPMT2,3,4_form_by_Npe");
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(path + "slowPMT1_form_by_Npe");

	if (!N_only) {
		ty(AStates::PMT_tbS);
		ch(2);
		noise_cut(2, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "2_form_by_S");
		ch(3);
		noise_cut(3, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "3_form_by_S");
		ch(4);
		noise_cut(4, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "4_form_by_S");
		ch(5);
		noise_cut(5, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "5_form_by_S");
		ch(0);
		noise_cut(0, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "slowPMT2,3,4_form_by_S");
		ch(1);
		noise_cut(1, 1, false);
		set_zoom(20, 90);
		set_bins(800);
		saveas(path + "slowPMT1_form_by_S");
	}
}

//Even it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
std::vector<double> x_y_regions;
if (channel==0) {
    cut_A_S_upper(0, 0, 0.029, 0, display, channel, "small_A");
	if (aggressiveness ==1) {
    	cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.25, 1, 0.4, display, channel, "rem_S>~0.3");
	}
}
if (channel==1) {
	if (aggressiveness ==1) {
		cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.6, 1, 0.6, display, channel, "rem_S>0.6");
	}
}
if (channel==2) {
    x_y_regions = {0.003, 0.0035, 0.00003, 0.008, 0.00007, 0.008};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.25, 0, 2, 0, display, channel, "rem_A>0.25");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==3) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.012};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.25, 0, 2, 0, display, channel, "rem_A>0.25");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==4) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.05};
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
    x_y_regions = {0.0144, 0.014, 0.0010, 0.025, 0.0, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");    
    if (aggressiveness>=1) {//select only 1 photoelectron for calibration
        cut_A_S_upper(0.031, 0.0001, 1, 0.0001, display, channel, "2pe");
    }
	if (aggressiveness>=2) {//remove merged peaks
        cut_A_S_upper(0.0144, 0.0017, 0.031, 0.0039, display, channel, "2pe_merged");
	}
}
}

void analysis_history(bool calibrate, int method = 0) {
//Created on 18.11.2019
//method = 0 - new event selection (suppressing events with large tails)
calibration_file = "190307/results/190307_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2;
data_output_path = "190307/results_v3/";
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (23, 40);
S2_times["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (23, 40);
S2_times["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = std::pair<double, double> (23, 40);
S2_times["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = std::pair<double, double> (23, 40);
S2_times["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (23, 40);

std::map<std::string, std::string> exp_folders;
exp_folders["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = "Cd_46V_20kV_850V";
exp_folders["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = "Cd_46V_18kV_850V";
exp_folders["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = "Cd_46V_16kV_850V";
exp_folders["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = "Cd_46V_14kV_850V";
exp_folders["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = "Bkg_46V_20kV_850V";


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
    std::stringstream ss;
    ss<<std::fixed<<std::setprecision(0)<<d_S2_start;
    S2_start = ss.str();
    ss.str("");
    ss<<std::fixed<<std::setprecision(0)<<d_S2_finish;
    S2_finish = ss.str();
} else {
    std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping calibration of this experiment"<<std::endl;
    nex();
    continue;
}
for (std::size_t chi = 0, chi_end_ = calib_channels.size(); chi!=chi_end_; ++chi) { 
int channel = calib_channels[chi];
//parameters set by Cd_20kV_850V_46V, but valid for all fields. 
if (channel==7) {
    ty(AStates::MPPC_A_S);
	ch(7);
    set_zoom(0, 0.1, 0, 0.012);
    set_bins(400);
    noise_cut(7, 1, true);
    saveas(data_output_path + folder + "/calibration/7_A_S_zoom");
    ty(AStates::MPPC_Ss);
    set_zoom(0, 0.01);
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
    ty(AStates::PMT_A_S);
	ch(0);
    set_zoom(0, 1.2, 0, 1);
    set_bins(400);
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
    ty(AStates::PMT_A_S);
	ch(1);
    set_zoom(0, 1.2, 0, 1);
    set_bins(400);
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
    ty(AStates::PMT_A_S);
	ch(2);
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
    ty(AStates::PMT_A_S);
	ch(3);
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
    ty(AStates::PMT_A_S);
	ch(4);
    set_zoom(0, 0.03, 0, 0.0007);        
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
    ty(AStates::PMT_A_S);
	ch(5);
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

//SIGNAL FORMS
if (!forms)
	return;
std::string folder, S2_start, S2_finish;
double d_S2_start, d_S2_finish;
int first_run = 0;
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
auto first_run_entry = experiment_runs.find(exp);
if (first_run_entry != experiment_runs.end())
    first_run = first_run_entry->second;
else {
    std::cout<<"Could not find starting run index for '"<<exp<<"'! Will print invalid event indexes."<<std::endl;
    first_run = -10000;
}

if ((exp == "190307_Bkg_20kV_850V_46V_coll6mm_th430mV" || exp == "190307_Cd_20kV_850V_46V_coll6mm_th430mV") && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 350);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 350, 0, 350);
	set_bins(350);
	cut_x_y_right(23, 0, 40, 12.7, true, "1");
	cut_x_y_right(40, 12.7, 68, 27.6, true, "2");
	cut_x_y_right(68, 27.6, 89.7, 52, true, "3");
	cut_x_y_right(89.7, 52, 350, 215, true, "4");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.26, 0, 30.26, 160, true, "1");
	cut_x_y_upper(0, 30.7, 160, 30.7, true, "2");
	cut_x_y_left(25.83, 0, 25.83, 160, true, "3");
	cut_x_y_lower(0, 26.0, 160, 26.0, true, "4");
	cut_x_y_right(26.18, 25.16, 31.60, 29.81, true, "5");
	cut_x_y_left(24.93, 26.36, 29.41, 31.07, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 30);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(31, 48);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(31, 80);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(90, 200);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190307_Cd_18kV_850V_46V_coll6mm_th430mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 250);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 250, 0, 250);
	set_bins(250);
	cut_x_y_right(16, 0, 56.3, 23.0, true, "1");
	cut_x_y_right(56.3, 23.0, 72, 44.8, true, "2");
	cut_x_y_right(72, 44.8, 250.0, 180, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.53, 0, 30.53, 160, true, "1");
	cut_x_y_upper(0, 30.87, 160, 30.87, true, "2");
	cut_x_y_left(25.78, 0, 25.78, 160, true, "3");
	cut_x_y_lower(0, 26.0, 160, 26.0, true, "4");
	cut_x_y_right(26.18, 25.16, 31.60, 29.81, true, "5");
	cut_x_y_left(24.93, 26.36, 29.41, 31.07, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 23);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(24, 39);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(24, 68);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(78, 150);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190307_Cd_16kV_850V_46V_coll6mm_th400mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 200);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 200, 0, 200);
	set_bins(200);
	cut_x_y_right(16, 0, 42.4, 20.1, true, "1");
	cut_x_y_right(42.4, 20.1, 57, 38.2, true, "2");
	cut_x_y_right(57, 38.2, 200, 150, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.53, 0, 30.53, 160, true, "1");
	cut_x_y_upper(0, 30.87, 160, 30.87, true, "2");
	cut_x_y_left(25.78, 0, 25.78, 160, true, "3");
	cut_x_y_lower(0, 26.0, 160, 26.0, true, "4");
	cut_x_y_right(26.18, 25.16, 31.60, 29.81, true, "5");
	cut_x_y_left(24.93, 26.36, 29.95, 31.60, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 17);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(18, 29);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(18, 52);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(65, 140);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

if (exp == "190307_Cd_14kV_850V_46V_coll6mm_th350mV" && method == 0) {
	double dt_shaping = 0.81 + 0.8228*drift_time_from_kV(experiment_fields[exp], 1.8);
	std::string dt = "dt="+dbl_to_str(dt_shaping, 1)+"us";
	std::string DT = "dt="+dbl_to_str(dt_shaping, 1)+"#mus"; //ROOT supports greek letters
	std::string cuts = "no_cuts";
	std::string zoomx = "Npe" + dbl_to_str(160-d_S2_start, 0);
	std::string zoomy = "Npe" + dbl_to_str(d_S2_finish-d_S2_start, 0);
	double trigger_at = 32;
	int no = 0; //picture number
	std::string Num = int_to_str(++no, 2); //="01"
	std::string FOLDER = data_output_path + folder + "/";

	//Set AStates::PMT_Npe_sum and AStates::PMT_sum_N cuts for future correlations
	ty(AStates::PMT_Npe_sum);
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	time_zoom_sPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_slowPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	time_zoom_fPMTs(d_S2_start, 160);
	set_bins(0, 150);
	saveaspng(FOLDER + Num+"_fastPMTs_Npe_"+cuts);
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpeaks); //May have poor results because of merged peaks
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bS); //May have poor results because it gives preference to single channel (to PMT with largest gain)
	off_ch(0); off_ch(1);
	noise_cut(2, 0, 0); noise_cut(3, 0, 0); noise_cut(4, 0, 0); noise_cut(5, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_fastPMTs_Strigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

	ty(AStates::PMT_trigger_bNpe); //May have poor results because of merged peaks
	off_ch(1); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
	noise_cut(0, 0, 0); noise_cut(1, 0, 0);
	set_trigger_shaping(dt_shaping);
	set_zoom(20, 40);
	set_bins(300);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

set_corr(AStates::PMT_Npe_sum, AStates::PMT_Npe_sum, -1, -1);
	ty(AStates::Correlation_y);
	time_zoom_sPMTs(d_S2_start, d_S2_finish);
	ty(AStates::Correlation);
	set_zoom(0, 150, 0, 150);
	set_bins(150);
	cut_x_y_right(10, 0, 32.8, 18.2, true, "1");
	cut_x_y_right(32.8, 18.2, 45.7, 33.0, true, "2");
	cut_x_y_right(45.7, 33.0, 150, 116, true, "3");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.53, 0, 30.53, 160, true, "1");
	cut_x_y_upper(0, 30.87, 160, 30.87, true, "2");
	cut_x_y_left(25.78, 0, 25.78, 160, true, "3");
	cut_x_y_lower(0, 26.0, 160, 26.0, true, "4");
	cut_x_y_right(26.18, 25.16, 31.60, 29.81, true, "5");
	cut_x_y_left(24.93, 26.36, 29.90, 31.87, true, "6");
	set_titles("t by fastPMTs by N " + DT, "t by slowPMTs by Npe" + DT);
	update();
	set_as_run_cut("good_trigger"); cuts += "+" + Num;
	saveaspng(FOLDER + Num+"_fastPMTs_N_vs_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	print_rejected_events(FOLDER + Num + "_rejected_events.txt", first_run);
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5");  remcut(-1, "6");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpeaks);
	saveaspng(FOLDER + Num+"_fastPMTs_Ntrigger_"+cuts+"_"+dt+"_zoom");
	set_trigger_offsets(trigger_at); cuts += "+" + Num;
	Num = int_to_str(++no, 2);

ty(AStates::PMT_trigger_bNpe);
	saveaspng(FOLDER + Num+"_slowPMTs_trigger_"+cuts+"_"+dt+"_zoom");
	Num = int_to_str(++no, 2);

ty(AStates::PMT_Npe_sum);
	draw_limits(0, 13);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_small_Npes/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(14, 22);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(14, 42);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(50, 100);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}

//END OF FORMS
} 
