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
	if (aggressiveness !=1) {
    	cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.6, 1, 0.8, display, channel, "rem_S>~0.7");
	}
}
if (channel==1) {
	x_y_regions = {0.035, 0.035, 0.0012, 0.2, 0.01, 0.5};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_S_noise");
	if (aggressiveness !=1) {
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
    cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");  
    if (aggressiveness>0) {//select only 1 photoelectron for calibration
        cut_A_S_upper(0.031, 0.0001, 1, 0.0001, display, channel, "2pe");
        cut_A_S_upper(0.015, 0.0016, 0.031, 0.0038, display, channel, "2pe_merged");
    }
}
}

void analysis_history(bool calibrate, int method = 0) {
//Created on 21.10.2019
//TODO:
//  1) Move histogram stats function (top-left or top-right corner)
//  2) Issue with drawing cuts: drawn on top of stat box and cropping issue when border is the same as line to draw
calibration_file = data_output_path+"190228_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190228_Cd_20kV_850V_46V_th500mV"] = std::pair<double, double> (25, 40);
S2_times["190228_Cd_20kV_850V_46V_th600mV"] = std::pair<double, double> (25, 40);
S2_times["190228_Cd_18kV_850V_46V_th500mV"] = std::pair<double, double> (25, 40);
S2_times["190228_Cd_16kV_850V_46V_th500mV"] = std::pair<double, double> (25, 40);
S2_times["190228_Bkg_20kV_850V_46V_th500mV"] = std::pair<double, double> (25, 40);

std::map<std::string, std::string> exp_folders;
exp_folders["190228_Cd_20kV_850V_46V_th500mV"] = "Cd_46V_20kV_850V";
exp_folders["190228_Cd_20kV_850V_46V_th600mV"] = "Cd_46V_20kV_850V_1";
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

if (exp == "190228_Cd_20kV_850V_46V_th500mV" && forms && 0==method) { 	
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 270; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 150; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "150";
	std::string meth = "";
	ch(1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 1);
	set_bins(0, slowPMT_small_Npes/2);
	draw_limits(0, 15);
	saveas(data_output_path + folder + "/slowPMTs_Npe_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	set_bins(18, slowPMT_small_Npes);
	draw_limits(18, slowPMT_small_Npes);
	std::string npe_cut = "_18-" + str_slowPMT_small_Npes+ "pe";
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
	set_bins(0, slowPMT_large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_bins(0, 200);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	
////////////////////////////////////20kV///////600mV thresh/////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(18, slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(18, 30);
			npe_cut = "_18-30pe";
			Npe_tail_lims = std::pair<double, double> (0, 80);
			tail_bins = 140;			
			break;			
		}
		case 1: {
			draw_limits(31, 51);
			npe_cut = "_31-51pe";
			Npe_tail_lims = std::pair<double, double> (0, 105);
			tail_bins = 200;
			break;			
		}
		case 2: {
			draw_limits(31, 72);
			npe_cut = "_31-72pe";
			Npe_tail_lims = std::pair<double, double> (0, 124);
			tail_bins = 240;
			break;
		}
		case 3: {
			draw_limits(85, 140);
			npe_cut = "_85-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 220);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_bins(0, tail_bins);
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_bins(0, slowPMT_large_Npes);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

		ty(AStates::PMT_tbN);
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
if (exp == "190228_Cd_20kV_850V_46V_th600mV" && forms && 0==method) { 	
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 270; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 150; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "150";
	std::string meth = "";
	ch(1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 1);
	set_bins(0, slowPMT_small_Npes);
	draw_limits(0, 15);
	saveas(data_output_path + folder + "/slowPMTs_Npe_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	set_bins(18, slowPMT_small_Npes);
	draw_limits(18, slowPMT_small_Npes);
	std::string npe_cut = "_18-" + str_slowPMT_small_Npes+ "pe";
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
	set_bins(0, slowPMT_large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_bins(0, 200);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	
////////////////////////////////////20kV///////600mV thresh/////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(18, slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(18, 35);
			npe_cut = "_18-35pe";
			Npe_tail_lims = std::pair<double, double> (0, 84);
			tail_bins = 140;			
			break;			
		}
		case 1: {
			draw_limits(36, 52);
			npe_cut = "_36-52pe";
			Npe_tail_lims = std::pair<double, double> (0, 102);
			tail_bins = 200;
			break;			
		}
		case 2: {
			draw_limits(36, 72);
			npe_cut = "_36-72pe";
			Npe_tail_lims = std::pair<double, double> (0, 120);
			tail_bins = 240;
			break;
		}
		case 3: {
			draw_limits(85, 140);
			npe_cut = "_85-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 225);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_bins(0, slowPMT_large_Npes);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

		ty(AStates::PMT_tbN);
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
if (exp == "190228_Bkg_20kV_850V_46V_th500mV" && forms && 0==method) { 	
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 270; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 150; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "150";
	std::string meth = "";
	ch(1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 1);
	set_bins(0, slowPMT_small_Npes);
	draw_limits(0, 15);
	saveas(data_output_path + folder + "/slowPMTs_Npe_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	set_bins(18, slowPMT_small_Npes);
	draw_limits(18, slowPMT_small_Npes);
	std::string npe_cut = "_18-" + str_slowPMT_small_Npes+ "pe";
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
	set_bins(0, slowPMT_large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_bins(0, 200);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	
////////////////////////////////////20kV///////600mV thresh/////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(18, slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(18, 30);
			npe_cut = "_18-30pe";
			Npe_tail_lims = std::pair<double, double> (0, 80);
			tail_bins = 140;			
			break;			
		}
		case 1: {
			draw_limits(31, 51);
			npe_cut = "_31-51pe";
			Npe_tail_lims = std::pair<double, double> (0, 105);
			tail_bins = 200;
			break;			
		}
		case 2: {
			draw_limits(31, 72);
			npe_cut = "_31-72pe";
			Npe_tail_lims = std::pair<double, double> (0, 124);
			tail_bins = 240;
			break;
		}
		case 3: {
			draw_limits(85, 140);
			npe_cut = "_85-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 220);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_bins(0, tail_bins);
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_bins(0, slowPMT_large_Npes);	
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

		ty(AStates::PMT_tbN);
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
if (exp == "190228_Cd_18kV_850V_46V_th500mV" && forms && 0==method) { 	
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 270; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 150; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "150";
	std::string meth = "";
	ch(1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 1);
	set_bins(0, slowPMT_small_Npes);
	draw_limits(0, 15);
	saveas(data_output_path + folder + "/slowPMTs_Npe_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	set_bins(16, slowPMT_small_Npes);
	draw_limits(16, slowPMT_small_Npes);
	std::string npe_cut = "_16-" + str_slowPMT_small_Npes+ "pe";
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
	set_bins(0, slowPMT_large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_bins(0, 200);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	
////////////////////////////////////18kV////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(16, slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(16, 27);
			npe_cut = "_16-27pe";
			Npe_tail_lims = std::pair<double, double> (0, 60);
			tail_bins = 150;			
			break;			
		}
		case 1: {
			draw_limits(30, 46);
			npe_cut = "_30-46pe";
			Npe_tail_lims = std::pair<double, double> (0, 82);
			tail_bins = 150;
			break;			
		}
		case 2: {
			draw_limits(30, 65);
			npe_cut = "_30-65pe";
			Npe_tail_lims = std::pair<double, double> (0, 100);
			tail_bins = 220;
			break;			
		}
		case 3: {
			draw_limits(72, 140);
			npe_cut = "_72-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 180);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_bins(0, tail_bins);
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_bins(0, slowPMT_large_Npes);		
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

		ty(AStates::PMT_tbN);
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
if (exp == "190228_Cd_16kV_850V_46V_th500mV" && forms && 0==method) { 	
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 250; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 150; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "150";
	std::string meth = "";
	ch(1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 1);
	set_bins(0, slowPMT_small_Npes);
	draw_limits(0, 15);
	saveas(data_output_path + folder + "/slowPMTs_Npe_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 0);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	set_bins(12, slowPMT_small_Npes);
	draw_limits(12, slowPMT_small_Npes);
	std::string npe_cut = "_12-" + str_slowPMT_small_Npes+ "pe";
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
	set_bins(0, slowPMT_large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
    ch(0);
    update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(0); off_ch(1);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 2);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 3);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 4);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 5);
	noise_cut(2, 0, false);
	noise_cut(3, 0, false);
	noise_cut(4, 0, false);
	noise_cut(5, 0, false);
	set_bins(0, 200);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 30);
	noise_cut(7, 0, false);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 7);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	noise_cut(7, 0, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	
////////////////////////////////////16kV////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		ch(1);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 1);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(12, slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(12, 22);
			npe_cut = "_12-22pe";
			Npe_tail_lims = std::pair<double, double> (0, 40);
			tail_bins = 150;			
			break;			
		}
		case 1: {
			draw_limits(23, 40);
			npe_cut = "_23-40pe";
			Npe_tail_lims = std::pair<double, double> (0, 70);
			tail_bins = 150;
			break;			
		}
		case 2: {
			draw_limits(23, 57);
			npe_cut = "_23-57pe";
			Npe_tail_lims = std::pair<double, double> (0, 85);
			tail_bins = 200;
			break;			
		}
		case 3: {
			draw_limits(65, 140);
			npe_cut = "_65-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 190);
			tail_bins = slowPMT_small_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 1);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		ch(0);
		unset_draw_limits();
		set_bins(0, slowPMT_large_Npes);		
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT2,3,4_Npe");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbN_sum);
		set_bins(400);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPM38_form_by_Npe");

		ty(AStates::PMT_tbN);
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
