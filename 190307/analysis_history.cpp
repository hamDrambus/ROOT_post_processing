//Even if it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
std::vector<double> x_y_regions;
if (channel==0) {
    cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
    cut_A_S_upper(0, 0, 0.029, 0, display, channel, "small_A");
}
if (channel==1) {
    cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
}
if (channel==2) {
    x_y_regions = {0.003, 0.0035, 0.00003, 0.008, 0.00007, 0.008};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.2, 0, 2, 0, display, channel, "rem_A>0.2");
}
if (channel==3) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.012};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.2, 0, 2, 0, display, channel, "rem_A>0.2");
}
if (channel==4) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.05};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.2, 0, 2, 0, display, channel, "rem_A>0.2");
}
if (channel==5) {
    x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.05};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.2, 0, 2, 0, display, channel, "rem_A>0.2");
}
if (channel==7) {
    x_y_regions = {0.0144, 0.014, 0.0010, 0.025, 0.0, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    if (aggressiveness>0) {//select only 1 photoelectron for calibration
        cut_A_S_upper(0.031, 0.0001, 1, 0.0001, display, channel, "2pe");
        cut_A_S_upper(0.0144, 0.0017, 0.031, 0.0039, display, channel, "2pe_merged");
    }
}
}

void analysis_history(void) {
//Created on 10.10.2019
//TODO 10.10.2019:
//  1) Move histogram stats function (top-left or top-right corner)
//  2) Pause after saving histogram. There is an option to wait for click on histogram
//  3) Parameters for this script to run different analyzes (calibration, signal forms without cuts, signal forms for low Npe, for Cd peak, for high Npe, with/without A-S cuts on PMT (slow+fast) and SiPM selections, with Npe weights, with S weights).

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = std::pair<double, double> (22, 80);
S2_times["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = std::pair<double, double> (22, 80);

std::map<std::string, std::string> exp_folders;
exp_folders["190307_Cd_20kV_850V_46V_coll6mm_th430mV"] = "Cd_46V_20kV_850V";
exp_folders["190307_Cd_18kV_850V_46V_coll6mm_th430mV"] = "Cd_46V_18kV_850V";
exp_folders["190307_Cd_16kV_850V_46V_coll6mm_th400mV"] = "Cd_46V_16kV_850V";
exp_folders["190307_Cd_14kV_850V_46V_coll6mm_th350mV"] = "Cd_46V_14kV_850V";
exp_folders["190307_Bkg_20kV_850V_46V_coll6mm_th430mV"] = "Bkg_46V_20kV_850V";

bool calibrate = true;
bool forms = false;
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
    d_S2_start = S2_times_entry->second->first;
    d_S2_finish = S2_times_entry->second->second;
    std::stringstream ss;
    ss<<std::setprecision(1)<<d_S2_start;
    S2_start = ss.str();
    ss.str("");
    ss<<std::setprecision(1)<<d_S2_finish;
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
    set_zoom(0, 0.15, 0, 0.04);        
    set_bins(100);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/0_A_S_zoom");
    
    ty(AnalysisStates::PMT_Ss);
    set_zoom(0, 0.15);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false)
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
    set_zoom(0, 0.8, 0, 0.2);        
    set_bins(600);
    noise_cut(channel, 0, true);
    saveas(data_output_path + folder+"/calibration/1_A_S_zoom");
    
    ty(AnalysisStates::PMT_Ss);
    set_zoom(0, 0.15);
    set_bins(600);
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false)
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
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false)
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
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false)
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
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false)
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
    cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false)
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
std::string exp = post_processor->experiments[post_processor->current_exp_index];
if (exp == "190404_Cd_20kV_850V_46V_th250mV" && aggressive_PMT_cuts == 0 && forms) { 	
	std::string folder = "Cd_46V_20kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 160);
	set_bins(160);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/slowPMTs_Npe");
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, aggressive_PMT_cuts, false);
	noise_cut(9, aggressive_PMT_cuts, false);
	noise_cut(10, aggressive_PMT_cuts, false);
	noise_cut(11, aggressive_PMT_cuts, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_S");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_S");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_S");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_S");
////////////////////////////////////20kV///////0-28pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 28);
	npe_cut = "_0-28pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////28-48pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 48);
	npe_cut = "_28-48pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////28-80pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 80);
	npe_cut = "_28-80pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////90-160pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(90, 160);
	npe_cut = "_90-160pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AnalysisStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

	ty(AnalysisStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////21-62pe///As 18 kV////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(21, 62);
	npe_cut = "_x_21-62pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");
////////////////////////////////////20kV///////18-48pe///As 16 kV////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 48);
	npe_cut = "_x_18-48pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");
////////////////////////////////////20kV///////13-40pe///As 14 kV////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(13, 40);
	npe_cut = "_x_13-40pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");
////////////////////////////////////20kV///////10-32pe///As 12 kV////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 32);
	npe_cut = "_x_10-32pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");
////////////////////////////////////20kV///////8-23pe///As 10 kV////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 23);
	npe_cut = "_x_8-23pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");
////////////////////////////////////20kV///////5-20pe///As 8 kV////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(5, 20);
	npe_cut = "_x_5-20pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AnalysisStates::PMT_tbN);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");
}
//END OF FORMS
} 
