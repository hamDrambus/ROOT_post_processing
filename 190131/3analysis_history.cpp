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
	ty(AStates::MPPC_tbNpe_sum);
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
	saveas(path + "slowPMTs_form_by_Npe");

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
		saveas(path + "slowPMTs_form_by_S");
	}
}

//Even it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
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
	x_y_regions = {0.035, 0.035, 0.0012, 0.2, 0.01, 0.1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_S_noise");
	if (aggressiveness ==1) {
		cut_A_S_upper(0.95, 0, 2, 0, display, channel, "rem_A>0.95");
		cut_A_S_upper(0.0, 0.7, 1, 0.9, display, channel, "rem_S>0.8");
	}
}
if (channel==2) {
    x_y_regions = {0.003, 0.0035, 0.000038, 0.008, 0.00007, 0.018};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.20, 0, 2, 0, display, channel, "rem_A>0.20");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==3) {
    x_y_regions = {0.0035, 0.0035, 0.000032, 0.012, 0.00013, 0.018};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.25, 0, 2, 0, display, channel, "rem_A>0.25");
    cut_A_S_upper(0.0, 0.01, 0.25, 0.025, display, channel, "rem_S>~0.01");
}
if (channel==4) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.02};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.5, 0, 2, 0, display, channel, "rem_A>0.5");
    cut_A_S_upper(0.0, 0.01, 0.5, 0.04, display, channel, "rem_S>~0.01");
}
if (channel==5) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.05};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.6, 0, 2, 0, display, channel, "rem_A>0.6");
    cut_A_S_upper(0.0, 0.01, 0.6, 0.046, display, channel, "rem_S>~0.01");
}
if (channel==7) {
    x_y_regions = {0.015, 0.015, 0.0013, 0.028, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.015, 0.015, 0.00065, 0.07, 0.005, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
    cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
    if (aggressiveness>=1) {//select only 1 photoelectron for calibration
        cut_A_S_upper(0.031, 0.0001, 1, 0.0001, display, channel, "2pe");
    }
	if (aggressiveness>=2) {
        cut_A_S_upper(0.015, 0.0016, 0.031, 0.0038, display, channel, "2pe_merged");
    }
}
}


void analysis_history(bool calibrate, int method = 0) {
//Created on 03.12.2019
//method = 0 - new event selection (suppressing events with large tails)
calibration_file = "190131/results/190131_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
trigger_version = TriggerVersion::trigger_v2;
data_output_path = "190131/results_v3/";
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190131_Cd_20kV_850V_46V_coll_2mm_th1.3V"] = std::pair<double, double> (23, 40);
S2_times["190131_Cd_20kV_850V_46V_coll_6mm_th1.3V"] = std::pair<double, double> (23, 40);
S2_times["190131_Cd_20kV_850V_46V_coll_2mm_th1.2V"] = std::pair<double, double> (23, 40);
S2_times["190131_Bkg_20kV_850V_46V_coll_2mm_th1.3V"] = std::pair<double, double> (23, 40);

std::map<std::string, std::string> exp_folders;
exp_folders["190131_Cd_20kV_850V_46V_coll_2mm_th1.3V"] = "Cd_46V_20kV_850V_2mm_th1.3V";
exp_folders["190131_Cd_20kV_850V_46V_coll_6mm_th1.3V"] = "Cd_46V_20kV_850V_6mm_th1.3V";
exp_folders["190131_Cd_20kV_850V_46V_coll_2mm_th1.2V"] = "Cd_46V_20kV_850V_2mm_th1.2V";
exp_folders["190131_Bkg_20kV_850V_46V_coll_2mm_th1.3V"] = "Bkg_46V_20kV_850V_2mm_th1.3V";

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
std::stringstream ss;
ss<<std::fixed<<std::setprecision(0)<<channel;
std::string ch_str = ss.str();
//parameters set by Cd_20kV_850V_46V, but valid for all fields. 
if (channel==7) {
ch(7);
    ty(AStates::MPPC_A_S);
    set_zoom(0, 0.072, 0, 0.012);
    set_bins(500);
    noise_cut(7, 1, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
    ty(AStates::MPPC_Ss);
    set_zoom(0, 0.009);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss");    
    noise_cut(7, 1, false);
    draw_limits(0.0005, 0.004);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_single_pe");
}
if (channel==0) {
ch(0);
    ty(AStates::PMT_A_S);
    set_zoom(0, 1.2, 0, 1);
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S");
    set_zoom(0, 0.15, 0, 0.03);        
    set_bins(100);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S_zoom");
    
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.15);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
    noise_cut(channel, 0, false);        
    draw_limits(0, 0.045);
    set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us_A_S_cuts");
}
if (channel==1) {
ch(1);
    ty(AStates::PMT_A_S);
    set_zoom(0, 1.2, 0, 1.2);
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S");    
    set_zoom(0, 0.15, 0, 0.03);        
    set_bins(200);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S_zoom");
    
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.15);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    noise_cut(channel, 0, false);        
    draw_limits(0.000, 0.045);
    set_use_mean();
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");
}
if (channel==2) {
ch(2);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.03, 0, 0.0005);        
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.0012);
    set_bins(400);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
}
if (channel==3) {
ch(3);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.03, 0, 0.0005);        
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.0016);
    set_bins(400);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
}
if (channel==4) {
ch(4);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.03, 0, 0.0007);        
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
}
if (channel==5) {
ch(5);
    ty(AStates::PMT_A_S);
    set_zoom(0, 0.03, 0, 0.0005);        
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_A_S");        
   
    ty(AStates::PMT_Ss);
    set_zoom(0, 0.003);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us");    
    noise_cut(channel, 0, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_0-"+S2_start+"us_A_S_cuts");
    cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false);
    saveas(data_output_path + folder+"/calibration/"+ch_str+"_Ss_w_"+S2_start+"-"+S2_finish+"us_A_S_cuts");
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

if (method == 0) { //Same for all experiments here
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
	off_ch(0); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
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
	off_ch(0); off_ch(2); off_ch(3); off_ch(4); off_ch(5);
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
	cut_x_y_right(13, 0, 250, 172, true, "1");
	set_titles("N_{pe} t#in["+S2_start+", 160] #mus", "N_{pe} t#in["+S2_start+", "+S2_finish+"] #mus");
	update();
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts+"_"+S2_start+"-"+S2_finish+"us_vs_"+S2_start+"-160us");
	set_as_run_cut("good_sPMTs_ZxZy");	cuts = "cuts_"+Num;
	Num = int_to_str(++no, 2);
	remcut(-1, "1");
	
set_corr(AStates::PMT_trigger_bNpeaks, AStates::PMT_trigger_bNpe, -1, -1);
	ty(AStates::Correlation);
	set_zoom(20, 45, 20, 45);
	set_bins(600);
	cut_x_y_right(30.62, 0, 30.62, 160, true, "1");
	cut_x_y_upper(0, 31.07, 160, 31.07, true, "2");
	cut_x_y_left(25.83, 0, 25.83, 160, true, "3");
	cut_x_y_lower(0, 26.16, 160, 26.16, true, "4");
	cut_x_y_right(24.75, 23.97, 32.81, 31.54, true, "5");
	cut_x_y_left(24.26, 25.57, 31.17, 32.60, true, "6");
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
	draw_limits(18, 23);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_right/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(18, 42);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	print_accepted_events(FOLDER + "forms_Cd_peak/"+Num+"_events_"+cuts+".txt", first_run);
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_Cd_peak/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());

ty(AStates::PMT_Npe_sum);
	draw_limits(60, 120);
	saveaspng(FOLDER + Num + "_slowPMTs_Npe_"+cuts);
	set_as_run_cut("En_spec");	cuts += "+" + Num;
	Num = int_to_str(++no, 2);

save_forms(FOLDER + "forms_cosmic/", false);
	unset_as_run_cut("En_spec");
	cuts.erase(cuts.end()-3, cuts.end());
}
//END OF FORMS
} 
