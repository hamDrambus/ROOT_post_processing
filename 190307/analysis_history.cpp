//Even if it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
std::vector<double> x_y_regions;
if (channel==0) {
    cut_A_S_upper(0, 0, 0.029, 0, display, channel, "small_A");
	if (aggressiveness !=1) {
    	cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.25, 1, 0.4, display, channel, "rem_S>~0.3");
	}
}
if (channel==1) {
	if (aggressiveness !=1) {
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
    if (aggressiveness>0) {//select only 1 photoelectron for calibration
        cut_A_S_upper(0.031, 0.0001, 1, 0.0001, display, channel, "2pe");
        cut_A_S_upper(0.0144, 0.0017, 0.031, 0.0039, display, channel, "2pe_merged");
    }
}
}

void analysis_history(bool calibrate, int method = 0) {
//Created on 10.10.2019
//TODO 10.10.2019:
//  1) Move histogram stats function (top-left or top-right corner)
//  DEPR. 2) Pause after saving histogram. There is an option to wait for click on histogram
//  DEPR. 3) Parameters for this script to run different analyzes (calibration, signal forms without cuts, signal forms for low Npe, for Cd peak, for high Npe, with/without A-S cuts on PMT (slow+fast) and SiPM selections, with Npe weights, with S weights).

std::map<std::string, std::pair<double, double> > S2_times;
if (0==method) {
S2_times["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (22, 80);
} else {
	S2_times["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (25, 40);
	S2_times["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (25, 40);
	S2_times["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = std::pair<double, double> (25, 40);
	S2_times["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = std::pair<double, double> (25, 40);
	S2_times["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (25, 40);
}

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
ch(7);
    ty(AnalysisStates::MPPC_A_S);
    set_zoom(0, 0.1, 0, 0.012);
    set_bins(400);
    noise_cut(7, 1, true);
    saveas(data_output_path + folder + "/calibration/7_A_S_zoom");
    ty(AnalysisStates::MPPC_Ss);
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
ch(0);
    ty(AnalysisStates::PMT_A_S);
    set_zoom(0, 1.2, 0, 1);
    set_bins(400);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/0_A_S");
    set_zoom(0, 0.15, 0, 0.04);        
    set_bins(100);
    saveas(data_output_path + folder+"/calibration/0_A_S_zoom");
    
    ty(AnalysisStates::PMT_Ss);
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
    ty(AnalysisStates::PMT_A_S);
    set_zoom(0, 1.2, 0, 1);
    set_bins(400);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/1_A_S");    
    set_zoom(0, 0.8, 0, 0.2);        
    set_bins(600);
    saveas(data_output_path + folder+"/calibration/1_A_S_zoom");
    
    ty(AnalysisStates::PMT_Ss);
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
    ty(AnalysisStates::PMT_A_S);
    set_zoom(0, 0.02, 0, 0.0005);        
    set_bins(300);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/2_A_S");        
   
    ty(AnalysisStates::PMT_Ss);
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
    ty(AnalysisStates::PMT_A_S);
    set_zoom(0, 0.02, 0, 0.0005);        
    set_bins(300);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/3_A_S");        
   
    ty(AnalysisStates::PMT_Ss);
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
    ty(AnalysisStates::PMT_A_S);
    set_zoom(0, 0.03, 0, 0.0007);        
    set_bins(400);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/4_A_S");        
   
    ty(AnalysisStates::PMT_Ss);
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
    ty(AnalysisStates::PMT_A_S);
    set_zoom(0, 0.02, 0, 0.0005);        
    set_bins(300);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/5_A_S");        
   
    ty(AnalysisStates::PMT_Ss);
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
    std::stringstream ss;
    ss<<std::fixed<<std::setprecision(0)<<d_S2_start;
    S2_start = ss.str();
    ss.str("");
    ss<<std::fixed<<std::setprecision(0)<<d_S2_finish;
    S2_finish = ss.str();
} else {
    std::cout<<"Could not find S2 time limits for '"<<exp<<"'! Skipping building signal forms"<<std::endl;
    return;
}
if (exp == "190307_Cd_20kV_850V_46V_coll6mm_th430mV" && forms && 0==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 160);
	set_bins(160);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 0, false);
	noise_cut(1, 0, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ch(1);
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 100);
	set_bins(100);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 25);
	set_bins(25);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
	
	ty(AnalysisStates::PMT_tbS);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////20kV///////0-28pe/////////////////////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 28);
	npe_cut = "_0-28pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
    set_bins(400);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    set_bins(400);	
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////20kV///////28-46pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 46);
	npe_cut = "_28-46pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////20kV///////28-67pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 67);
	npe_cut = "_28-67pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////20kV///////85-160pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(85, 160);
	npe_cut = "_85-160pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
}
if (exp == "190307_Bkg_20kV_850V_46V_coll6mm_th430mV" && forms && 0==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 160);
	set_bins(160);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 0, false);
	noise_cut(1, 0, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ch(1);
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 100);
	set_bins(100);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 25);
	set_bins(25);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
/////////////////Background/////////20kV///////0-28pe/////////////////////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 28);
	npe_cut = "_0-28pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
    set_bins(400);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

////////////////////////////////////20kV///////28-46pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 46);
	npe_cut = "_28-46pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

////////////////////////////////////20kV///////28-67pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 67);
	npe_cut = "_28-67pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

////////////////////////////////////20kV///////85-160pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(85, 160);
	npe_cut = "_85-160pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
}
if (exp == "190307_Cd_20kV_850V_46V_coll6mm_th430mV" && forms && 1==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	std::string meth = "_v1";
	set_zoom(0, 160);
	set_bins(160);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	return;
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
    ch(1);
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 100);
	set_bins(100);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 25);
	set_bins(25);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	
////////////////////////////////////20kV///////0-25pe/////version 1//////////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 25);
	npe_cut = "_0-25pe";
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
    set_bins(400);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    set_bins(400);	
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////20kV///////25-46pe/////version 1//////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(25, 46);
	npe_cut = "_25-46pe";
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");

////////////////////////////////////20kV///////25-70pe/////version 1////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(25, 70);
	npe_cut = "_25-70pe";
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");

////////////////////////////////////20kV///////85-160pe/////version 1//////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(85, 160);
	npe_cut = "_85-160pe";
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");

}
if (exp == "190307_Cd_18kV_850V_46V_coll6mm_th430mV" && forms && 0==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 140);
	set_bins(140);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 0, false);
	noise_cut(1, 0, false);
	draw_limits(0, 140);
	std::string npe_cut = "_0-140pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ch(1);
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 100);
	set_bins(100);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 25);
	set_bins(25);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////18kV///////0-22pe/////////////////////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 22);
	npe_cut = "_0-22pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
    set_bins(400);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////18kV///////22-40pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(22, 40);
	npe_cut = "_22-40pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////18kV///////22-58pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(22, 58);
	npe_cut = "_22-58pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////18kV///////75-140pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(75, 140);
	npe_cut = "_75-140pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
}
if (exp == "190307_Cd_16kV_850V_46V_coll6mm_th400mV" && forms && 0==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 120);
	set_bins(120);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 0, false);
	noise_cut(1, 0, false);
	draw_limits(0, 120);
	std::string npe_cut = "_0-120pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ch(1);
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 100);
	set_bins(100);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 25);
	set_bins(25);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////16kV///////0-16pe/////////////////////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 16);
	npe_cut = "_0-16pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
    set_bins(400);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////16kV///////16-32pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(16, 32);
	npe_cut = "_16-32pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////16kV///////16-48pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(16, 48);
	npe_cut = "_16-48pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
////////////////////////////////////16kV///////60-120pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(60, 120);
	npe_cut = "_60-120pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
}
if (exp == "190307_Cd_14kV_850V_46V_coll6mm_th350mV" && forms && 0==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 90);
	set_bins(90);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 0, false);
	noise_cut(1, 0, false);
	draw_limits(0, 90);
	std::string npe_cut = "_0-90pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ch(1);
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 80);
	set_bins(80);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 15);
	set_bins(15);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");
	
	ty(AnalysisStates::PMT_tbS);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////14kV///////0-14pe/////////////////////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 14);
	npe_cut = "_0-14pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
    set_bins(400);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    set_bins(400);	
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////14kV///////14-24pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(14, 24);
	npe_cut = "_14-24pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////14kV///////14-42pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(14, 42);
	npe_cut = "_14-42pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
////////////////////////////////////14kV///////50-90pe/////////////////////////////////////////////////////
    ty(AnalysisStates::PMT_Npe_sum);
    ch(1);
	unset_as_run_cut(npe_cut);
	draw_limits(50, 90);
	npe_cut = "_50-90pe";
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);
    ch(0);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_Npe");
    ty(AnalysisStates::MPPC_Npe_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_Npe");
    ty(AnalysisStates::PMT_sum_N);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/fastPMTs_Npeaks");
	
    ty(AnalysisStates::MPPC_tbN_sum);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
    saveas(data_output_path + folder + "/forms"+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_Npe");
	
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_Npeaks");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_Npeaks");
	ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_Npeaks");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(0);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMT2,3,4_form_by_S");
	ch(1);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/slowPMTs_form_by_S");
	ch(2);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/2_form_by_S");
	ch(3);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/3_form_by_S");
    ch(4);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/4_form_by_S");
	ch(5);
	saveas(data_output_path + folder + "/forms"+npe_cut+"/5_form_by_S");
}
if (exp == "190307_Cd_20kV_850V_46V_coll6mm_th430mV" && forms && 2==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	std::string meth = "_v2";
	set_zoom(0, 140);
	set_bins(140);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	draw_limits(0, 140);
	std::string npe_cut = "_0-140pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(0);
	unset_draw_limits();
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
	set_zoom(0, 250);
	set_bins(250);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 160);
	set_bins(160);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 30);
	set_bins(30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	
////////////////////////////////////20kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins = 140;	
		ty(AnalysisStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, 140);
		set_bins(140);
		switch (i) {
		case 0: {
			draw_limits(0, 20);
			npe_cut = "_0-20pe";
			Npe_tail_lims = std::pair<double, double> (0, 50);
			tail_bins = 140;			
			break;			
		}
		case 1: {
			draw_limits(20, 36);
			npe_cut = "_20-36pe";
			Npe_tail_lims = std::pair<double, double> (0, 77);
			tail_bins = 150;
			break;			
		}
		case 2: {
			draw_limits(20, 55);
			npe_cut = "_20-55pe";
			Npe_tail_lims = std::pair<double, double> (0, 103);
			tail_bins = 160;
			break;			
		}
		case 3: {
			draw_limits(68, 140);
			npe_cut = "_68-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 215);
			tail_bins = 250;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_zoom(0, 250);
		set_bins(250);		
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AnalysisStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AnalysisStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AnalysisStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
		ty(AnalysisStates::MPPC_tbS_sum);
		set_bins(400);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

		ty(AnalysisStates::PMT_tbN);
		ch(0);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
		
		ch(2);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
		ch(3);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
		ch(4);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
		ch(5);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	}
}
if (exp == "190307_Bkg_20kV_850V_46V_coll6mm_th430mV" && forms && 2==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	std::string meth = "_v2";
	set_zoom(0, 140);
	set_bins(140);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	draw_limits(0, 140);
	std::string npe_cut = "_0-140pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(0);
	unset_draw_limits();
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
	set_zoom(0, 250);
	set_bins(250);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 160);
	set_bins(160);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 30);
	set_bins(30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	
////////////////////////////////////20kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins = 140;	
		ty(AnalysisStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, 140);
		set_bins(140);
		switch (i) {
		case 0: {
			draw_limits(0, 20);
			npe_cut = "_0-20pe";
			Npe_tail_lims = std::pair<double, double> (0, 50);
			tail_bins = 140;			
			break;			
		}
		case 1: {
			draw_limits(20, 36);
			npe_cut = "_20-36pe";
			Npe_tail_lims = std::pair<double, double> (0, 77);
			tail_bins = 150;
			break;			
		}
		case 2: {
			draw_limits(20, 55);
			npe_cut = "_20-55pe";
			Npe_tail_lims = std::pair<double, double> (0, 103);
			tail_bins = 160;
			break;			
		}
		case 3: {
			draw_limits(68, 140);
			npe_cut = "_68-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 215);
			tail_bins = 250;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_zoom(0, 250);
		set_bins(250);		
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AnalysisStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AnalysisStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AnalysisStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
		ty(AnalysisStates::MPPC_tbS_sum);
		set_bins(400);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

		ty(AnalysisStates::PMT_tbN);
		ch(0);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
		
		ch(2);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
		ch(3);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
		ch(4);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
		ch(5);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	}
}
if (exp == "190307_Cd_18kV_850V_46V_coll6mm_th430mV" && forms && 2==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	std::string meth = "_v2";
	set_zoom(0, 120);
	set_bins(120);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	draw_limits(0, 120);
	std::string npe_cut = "_0-120pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(0);
	unset_draw_limits();
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
	set_zoom(0, 200);
	set_bins(200);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 150);
	set_bins(150);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 30);
	set_bins(30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	
////////////////////////////////////18kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AnalysisStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, 120);
		set_bins(120);
		switch (i) {
		case 0: {
			draw_limits(0, 15);
			npe_cut = "_0-15pe";
			Npe_tail_lims = std::pair<double, double> (0, 40);
			tail_bins = 120;			
			break;			
		}
		case 1: {
			draw_limits(15, 32);
			npe_cut = "_15-32pe";
			Npe_tail_lims = std::pair<double, double> (0, 65);
			tail_bins = 120;
			break;			
		}
		case 2: {
			draw_limits(15, 48);
			npe_cut = "_15-48pe";
			Npe_tail_lims = std::pair<double, double> (0, 81);
			tail_bins = 140;
			break;			
		}
		case 3: {
			draw_limits(63, 120);
			npe_cut = "_63-120pe";
			Npe_tail_lims = std::pair<double, double> (0, 190);
			tail_bins = 200;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_zoom(0, 200);
		set_bins(200);		
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AnalysisStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AnalysisStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AnalysisStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
		ty(AnalysisStates::MPPC_tbS_sum);
		set_bins(400);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

		ty(AnalysisStates::PMT_tbN);
		ch(0);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
		
		ch(2);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
		ch(3);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
		ch(4);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
		ch(5);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	}
}
if (exp == "190307_Cd_16kV_850V_46V_coll6mm_th400mV" && forms && 2==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	std::string meth = "_v2";
	set_zoom(0, 120);
	set_bins(120);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	draw_limits(0, 120);
	std::string npe_cut = "_0-120pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(0);
	unset_draw_limits();
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
	set_zoom(0, 180);
	set_bins(180);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 140);
	set_bins(140);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 25);
	set_bins(25);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	
////////////////////////////////////16kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AnalysisStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		switch (i) {
		case 0: {
			draw_limits(0, 13);
			npe_cut = "_0-13pe";
			Npe_tail_lims = std::pair<double, double> (0, 30);
			tail_bins = 120;			
			break;			
		}
		case 1: {
			draw_limits(13, 26);
			npe_cut = "_13-26pe";
			Npe_tail_lims = std::pair<double, double> (0, 48);
			tail_bins = 120;
			break;			
		}
		case 2: {
			draw_limits(13, 42);
			npe_cut = "_13-42pe";
			Npe_tail_lims = std::pair<double, double> (0, 65);
			tail_bins = 120;
			break;			
		}
		case 3: {
			draw_limits(55, 120);
			npe_cut = "_55-120pe";
			Npe_tail_lims = std::pair<double, double> (0, 180);
			tail_bins = 180;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-160us");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_zoom(0, 180);
		set_bins(180);		
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AnalysisStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AnalysisStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AnalysisStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
		ty(AnalysisStates::MPPC_tbS_sum);
		set_bins(400);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

		ty(AnalysisStates::PMT_tbN);
		ch(0);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
		
		ch(2);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
		ch(3);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
		ch(4);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
		ch(5);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	}
}
if (exp == "190307_Cd_14kV_850V_46V_coll6mm_th350mV" && forms && 2==method) { 	
	ty(AnalysisStates::PMT_Npe_sum);
	std::string meth = "_v2";
	set_zoom(0, 100);
	set_bins(100);
	ch(1);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	draw_limits(0, 100);
	std::string npe_cut = "_0-100pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
    ch(0);
	unset_draw_limits();
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe"+S2_start+"-"+S2_finish+"us");
    ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
	set_zoom(0, 140);
	set_bins(140);
	unset_draw_limits();
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_zoom(0, 100);
	set_bins(100);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 25);
	set_bins(25);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(1);
	noise_cut(1, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
	ch(0);
	noise_cut(0, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
	ch(2);
	noise_cut(2, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
    ch(3);
	noise_cut(3, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
    ch(4);
	noise_cut(4, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
    ch(5);
	noise_cut(5, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	
////////////////////////////////////14kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins = 140;	
		ty(AnalysisStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		switch (i) {
		case 0: {
			draw_limits(0, 12);
			npe_cut = "_0-12pe";
			Npe_tail_lims = std::pair<double, double> (0, 25);
			tail_bins = 100;			
			break;			
		}
		case 1: {
			draw_limits(12, 20);
			npe_cut = "_12-20pe";
			Npe_tail_lims = std::pair<double, double> (0, 35);
			tail_bins = 100;
			break;			
		}
		case 2: {
			draw_limits(12, 33);
			npe_cut = "_12-33pe";
			Npe_tail_lims = std::pair<double, double> (0, 48);
			tail_bins = 100;
			break;			
		}
		case 3: {
			draw_limits(45, 100);
			npe_cut = "_45-100pe";
			Npe_tail_lims = std::pair<double, double> (0, 125);
			tail_bins = 140;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-160us");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_zoom(0, 140);
		set_bins(140);		
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AnalysisStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AnalysisStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AnalysisStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");
		ty(AnalysisStates::MPPC_tbS_sum);
		set_bins(400);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_S");

		ty(AnalysisStates::PMT_tbN);
		ch(0);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_form_by_Npe");
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_form_by_Npe");
		
		ch(2);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/2_form_by_Npeaks");
		ch(3);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/3_form_by_Npeaks");
		ch(4);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/4_form_by_Npeaks");
		ch(5);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/5_form_by_Npeaks");
	}
}
//END OF FORMS
} 
