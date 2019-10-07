//Even it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
if (channel==15) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.04, 0, 0.8, 0.105, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	    cut_A_S_fast_PMT(x_y_regions, display, 15, "large_A_noise");
	    x_y_regions = {-1, 0.020, 0.017, 0.1, 0.0, 1e3};//small S ch15 Cd_20kV_850V_46V. Best to determine using 0-23 us peaks
	    cut_A_S_fast_PMT(x_y_regions, display, 15, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 15, "A<0.8");
    }
}
if (channel==14) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.035, 0, 0.8, 0.108, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	    cut_A_S_fast_PMT(x_y_regions, display, 14, "large_A_noise");
	    x_y_regions = {-1, 0, 0.032, 0.13, 0.0, 1e3};	
	    cut_A_S_fast_PMT(x_y_regions, display, 14, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 14, "A<0.8");
    }
}
if (channel==13) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.025, 0, 0.8, 0.1, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
        cut_A_S_fast_PMT(x_y_regions, display, 13, "large_A_noise");
        x_y_regions = {-1, 0, 0.018, 0.046, 0.0, 1e3};//small S ch15 Cd_20kV_850V_46V. Best to determine using 0-23 us peaks
        cut_A_S_fast_PMT(x_y_regions, display, 13, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 13, "A<0.8");
    }
}
if (channel==12) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.025, 0, 0.45, 0.057, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 12, "large_A_noise");
	    x_y_regions = {-1, 0, 0.02, 0.054, 0.0, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 12, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 12, "A<0.8");
    }
}
if (channel==11) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.006, 0.000045, 0.4, 0.0062, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, 11, "large_A_noise");
	    cut_A_S_upper(0.005, 0.00018, 0.6, 0.019, display, 11);
    }
}
if (channel==10) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.006, 0.000045, 0.4, 0.0065, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 10, "large_A_noise");
	    cut_A_S_upper(0.003, 0.000118, 0.6, 0.0195, display, 10);
    }
}
if (channel==9) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.0042, 0, 0.4, 0.0065, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 9, "large_A_noise");
	    cut_A_S_upper(0.003, 0.00013, 0.6, 0.021, display, 9);
    }
}
if (channel==8) {
    if (aggressiveness > 0) {
        x_y_regions = {-1, 0.003, 0.000008, 0.4, 0.0069, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 8, "large_A_noise");
	    cut_A_S_upper(0.003, 0.00012, 0.6, 0.021, display, 8);
    }
}

std::map<int, std::pair<double, double> > cut_vals;
//SiPM (MPPC) channel - (Smin, Smax). Set by hand for each channel, but for all experiments (fields)
//for 46V on SiPM		
cut_vals[32] = std::pair<double, double> (0.0009, 0.007);
cut_vals[33] = std::pair<double, double> (0.00075, 0.006);
cut_vals[34] = std::pair<double, double> (0.0009, 0.01);
cut_vals[35] = std::pair<double, double> (0.001, 0.008);
cut_vals[36] = std::pair<double, double> (0.001, 0.009);
cut_vals[37] = std::pair<double, double> (0.00085, 0.005);
cut_vals[38] = std::pair<double, double> (0.001, 0.01);
cut_vals[39] = std::pair<double, double> (0.001, 0.007);
cut_vals[40] = std::pair<double, double> (0.0009, 0.009);
cut_vals[41] = std::pair<double, double> (0.0009, 0.009);
cut_vals[42] = std::pair<double, double> (0.001, 0.007);
//cut_vals[43] = std::pair<double, double> (-1.0, 0.0); //no contact for this channel, ignore it
cut_vals[44] = std::pair<double, double> (0.0008, 0.007);
cut_vals[48] = std::pair<double, double> (0.0009, 0.008);
cut_vals[49] = std::pair<double, double> (0.0007, 0.007);
cut_vals[50] = std::pair<double, double> (0.0007, 0.006);
cut_vals[51] = std::pair<double, double> (0.0008, 0.007);
cut_vals[52] = std::pair<double, double> (0.001, 0.01);
cut_vals[53] = std::pair<double, double> (0.001, 0.01);
cut_vals[54] = std::pair<double, double> (0.0008, 0.008);
cut_vals[55] = std::pair<double, double> (0.0009, 0.008);
cut_vals[56] = std::pair<double, double> (0.00095, 0.008);
cut_vals[57] = std::pair<double, double> (0.001, 0.01);
cut_vals[58] = std::pair<double, double> (0.001, 0.008);
cut_vals[59] = std::pair<double, double> (0.001, 0.009);

auto entry = cut_vals.find(channel);
if (entry != cut_vals.end()) {
	cut_S(entry->second.first, entry->second.second, display, channel);
}
if (channel == 43 && post_processor->isMultichannel(post_processor->current_type))
	off_ch(channel);
}

void analysis_history(void) {
//Created on 23.09.2019
//TODO 23.09.2019:
//  1) Move histogram stats function (top-left or top-right corner)
//  2) Pause after saving histogram. There is an option to wait for click on histogram
//  3) Parameters for this script to run different analyzes (calibration, signal forms without cuts, signal forms for low Npe, for Cd peak, for high Npe, with/without A-S cuts on PMT (slow+fast) and SiPM selections, with Npe weights, with S weights).

std::map<std::string, std::pair<double, double> > S2_times;
S2_times["190404_Cd_20kV_850V_46V_th250mV_0"] = std::pair<double, double> (25, 70);
S2_times["190404_Cd_20kV_850V_46V_th250mV"] = std::pair<double, double> (25, 70);
S2_times["190404_Cd_18kV_850V_46V_th230mV"] = std::pair<double, double> (25, 70);
S2_times["190404_Cd_16kV_850V_46V_th210mV"] = std::pair<double, double> (25, 70);
S2_times["190404_Cd_14kV_850V_46V_th200mV"] = std::pair<double, double> (25, 70);
S2_times["190404_Cd_12kV_850V_46V_th160mV"] = std::pair<double, double> (25, 70);
S2_times["190404_Cd_10kV_850V_46V_th150mV"] = std::pair<double, double> (25, 80);
S2_times["190404_Cd_8kV_850V_46V_th140mV"] = std::pair<double, double> (25, 80);

//Ultimately I have to choose single procedure for signal forms among them for all data.
//TODO: experiments loop (need long strings whens saving pictures). Currently I have to run this calibration script for each nex();  
//TODO: move S2 times from cut_S2_times.cpp here. Use them instead of 23-65us
int aggressive_PMT_cuts = 0; //0 - no cuts at all. 1 - moderate cuts, very low A-S exclusion, also exclude high A low S. 2 - most aggressive cuts on low A and S (only for ch 14). Calibration as well as Npe and signal forms must be plotted from scratch for every agrressiveness level.
bool calibrate = false;
bool forms = true;
for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) { 
int channel = calib_channels[chi];

//CALIBRATION (slow PMT and SiPMs)
//parameters set by Cd_20kV_850V_46V, but valid for all fields. 
if (channel==15) {
ch(15);
	ty(AnalysisStates::PMT_A_S);
    if (aggressive_PMT_cuts == 0) { //no A-S cuts
	    set_zoom(0, 0.3, 0, 0.04);
	    set_bins(60);
	    cut_S_t_rect_exclude(23, 65, 0, 1e5, false); //exclude signal region for calibration. May be even better to use 0-23 us selectoin only
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us_zoom");//w = with, wo = without
        set_zoom(0, 1.3, 0, 0.5);
        set_bins(300);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us");
        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.15);
        set_bins(200);
        cut_S_t_rect_exclude(23, 65, 0, 1e5, false);
        draw_limits(0.0, 0.055); //uncertainty 0.05-0.06 => S1pe 0.0224467-0.0251765 V*us
        set_use_mean(); //overwrites old info from calibration file!
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_Ss_wo_23-65us");
    }
    if (aggressive_PMT_cuts > 0) {
	    set_zoom(0, 0.3, 0, 0.04);
	    set_bins(60);
	    cut_S_t_rect_exclude(23, 65, 0, 1e5, false); //exclude signal region for calibration. May be even better to use 0-23 us selectoin only
        noise_cut(15, aggressive_PMT_cuts, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us_aggr=1_zoom");//w = with, wo = without
        set_zoom(0, 1.3, 0, 0.5);
        set_bins(300);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us_aggr=1");
        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.15);
        set_bins(200);
        cut_S_t_rect_exclude(23, 65, 0, 1e5, false);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_Ss_wo_23-65us");
        noise_cut(15, aggressive_PMT_cuts, false);
        draw_limits(0.0, 0.055); //uncertainty 0.05-0.06 => S1pe 0.0224467-0.0251765 V*us
        set_use_mean(); //overwrites old info from calibration file!
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_Ss_wo_23-65us_aggr=1");
    }
}
if (channel==14) {
ch(14);
    ty(AnalysisStates::PMT_A_S);
    if (aggressive_PMT_cuts == 0) { //no cuts on A-S
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(300);
        noise_cut(14, aggressive_PMT_cuts, true);
        cut_S_t_rect_select(23, 65, 0, 1e5, false);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_w_23-65us");
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(100);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_w_23-65us_zoom");

        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.12);
        noise_cut(14, aggressive_PMT_cuts, false);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(200);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_Ss_w_0-23us");//just showing that there are 2 distinct peaks here. Other pictures do not show it.
        cut_S_t_rect_select(23, 65, 0, 1e5, false);        
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_Ss_w_23-65us");
        cut_S_t_rect_select(65, 160, 0, 1e5, false);        
        draw_limits(0, 0.035);
        set_use_mean();
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_Ss_w_23-160us");
    }
    if (aggressive_PMT_cuts > 0) { //such cuts on A-S that Ss histogram for 0-23us peaks shows only 1 peak
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(300);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        noise_cut(14, aggressive_PMT_cuts, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_w_0-23us_aggr=1");        
        set_zoom(0, 0.4, 0, 0.07);
        set_bins(60);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_w_0-23us_aggr=1_zoom");
        
        remcut_S_t_rect_select();
        cut_S_t_rect_exclude(23, 65, 0, 1e5, false);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_wo_23-65us_aggr=1_zoom");
        set_zoom(0, 0.8, 0, 0.2);
        set_bins(300);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_wo_23-65us_aggr=1");

        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.12);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(200);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/Ss_14_w_0-23us");
        noise_cut(14, aggressive_PMT_cuts, false);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_Ss_w_0-23us_aggr=1");
        cut_S_t_rect_select(65, 160, 0, 1e5, false);        
        draw_limits(0, 0.043);
        set_use_mean();
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_Ss_w_65-160us_aggr=1");
    }
}
if (channel==13) {
ch(13);
    ty(AnalysisStates::PMT_A_S);
    if (aggressive_PMT_cuts == 0) { //no cuts on A-S
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(400);
        noise_cut(13, aggressive_PMT_cuts, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S");        
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(120);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S_zoom");

        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.1);
        noise_cut(13, aggressive_PMT_cuts, false);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(200);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_Ss_w_0-23us");
        cut_S_t_rect_select(23, 65, 0, 1e5, false);
        set_bins(300);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_Ss_w_23-65us");
        cut_S_t_rect_select(65, 160, 0, 1e5, false);        
        draw_limits(0, 0.033);
        set_use_mean();
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_Ss_w_23-160us");
    }
    if (aggressive_PMT_cuts > 0) {
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(400);
        noise_cut(13, aggressive_PMT_cuts, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S_aggr=1");        
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(120);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S_aggr=1_zoom");

        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.1);
        noise_cut(13, aggressive_PMT_cuts, false);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(200);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_Ss_w_0-23us_aggr=1");
        cut_S_t_rect_select(23, 65, 0, 1e5, false);
        set_bins(300);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_Ss_w_23-65us_aggr=1");
        cut_S_t_rect_select(65, 160, 0, 1e5, false);        
        draw_limits(0, 0.033);
        set_use_mean();
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_Ss_w_23-160us_aggr=1");
    }
}
if (channel==12) {
ch(12);
    ty(AnalysisStates::PMT_A_S);
    if (aggressive_PMT_cuts == 0) { //no cuts on A-S
        set_zoom(0, 0.5, 0, 0.1);        
        set_bins(400);
        noise_cut(12, aggressive_PMT_cuts, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S");        
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(150);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S_zoom");

        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.06);
        noise_cut(12, aggressive_PMT_cuts, false);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(200);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_Ss_w_0-23us");
        cut_S_t_rect_select(23, 65, 0, 1e5, false);
        set_bins(400);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_Ss_w_23-65us");
        cut_S_t_rect_select(65, 160, 0, 1e5, false);        
        draw_limits(0, 0.022);
        set_use_mean();
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_Ss_w_23-160us");
    }
    if (aggressive_PMT_cuts > 0) {
        set_zoom(0, 0.5, 0, 0.1);        
        set_bins(400);
        noise_cut(12, aggressive_PMT_cuts, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S_aggr=1");        
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(150);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S_aggr=1_zoom");
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(60);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S_w_0-23us_aggr=1_zoom");
        cut_S_t_rect_select(23, 65, 0, 1e5, false);
        set_bins(120);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S_w_23-65us_aggr=1_zoom");
        cut_S_t_rect_select(65, 160, 0, 1e5, false);
        set_bins(100);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S_w_65-160us_aggr=1_zoom");

        ty(AnalysisStates::PMT_Ss);
        set_zoom(0, 0.06);
        noise_cut(12, aggressive_PMT_cuts, false);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(200);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_Ss_w_0-23us_aggr=1");
        cut_S_t_rect_select(23, 65, 0, 1e5, false);
        set_bins(400);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_Ss_w_23-65us_aggr=1");
        cut_S_t_rect_select(65, 160, 0, 1e5, false);        
        draw_limits(0, 0.022);
        set_use_mean();
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_Ss_w_23-160us_aggr=1");
    }
}
if (channel==11 && aggressive_PMT_cuts > 0) {
ch(11);
    ty(AnalysisStates::PMT_A_S);
    noise_cut(11, aggressive_PMT_cuts, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/11_A_S_aggr=1");
    set_zoom(0, 0.08, 0, 0.003);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/11_A_S_aggr=1_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas("190404/results/Cd_46V_20kV_850V/calibration/11_A_S_aggr=1_zoom1");
}
if (channel==10 && aggressive_PMT_cuts > 0) {
ch(10);
    ty(AnalysisStates::PMT_A_S);
    noise_cut(10, aggressive_PMT_cuts, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/10_A_S_aggr=1");
    set_zoom(0, 0.08, 0, 0.003);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/10_A_S_aggr=1_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas("190404/results/Cd_46V_20kV_850V/calibration/10_A_S_aggr=1_zoom1");
}
if (channel==9 && aggressive_PMT_cuts > 0) {
ch(9);
    ty(AnalysisStates::PMT_A_S);
    noise_cut(9, aggressive_PMT_cuts, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/9_A_S_aggr=1");
    set_zoom(0, 0.08, 0, 0.003);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/9_A_S_aggr=1_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas("190404/results/Cd_46V_20kV_850V/calibration/9_A_S_aggr=1_zoom1");
}
if (channel==8 && aggressive_PMT_cuts > 0) {
ch(8);
    ty(AnalysisStates::PMT_A_S);
    noise_cut(8, aggressive_PMT_cuts, true);
    set_bins(3000);
    set_zoom(0, 0.4, 0, 0.018);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/8_A_S_aggr=1");
    set_zoom(0, 0.08, 0, 0.003);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/8_A_S_aggr=1_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas("190404/results/Cd_46V_20kV_850V/calibration/8_A_S_aggr=1_zoom1");
}
}//for (channels)
if (calibrate) {
	if (aggressive_PMT_cuts == 0) { //no A-S cuts
		calib_save("190404/results/190404_calibration.dat");
	}
	if (aggressive_PMT_cuts == 1) { //see above or pictures.
		calib_save("190404/results/190404_calibration_aggr=1.dat");
	}
	if (aggressive_PMT_cuts == 2) { //the only difference of 2 from 1 is 14th channel - more strict cuts
		calib_save("190404/results/190404_calibration_aggr=2.dat");
	}
}
//SiPMs were calibrated before the creation of this file so they are omitted. They will be present in analysis_history.cpp for other experimental days.
//END OF CALIBRATION

//FORMS NO CUTS/PEAK-NOISE SELECTIONS
//First are signal forms without cuts at all, even Npe, aggressiveness = 0. There are time windows for Npe though.
std::string aggr = "";
if (aggressive_PMT_cuts == 0)
	aggr = "";
if (aggressive_PMT_cuts == 1)
	aggr = "_aggr=1";
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
if (exp == "190404_Cd_20kV_850V_46V_th250mV" && aggressive_PMT_cuts == 1 && forms) { 	
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
////////////////////////////////////20kV///////0-23pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	draw_limits(0, 23);
	npe_cut = "_0-23pe";
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
////////////////////////////////////20kV///////23-39pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(23, 39);
	npe_cut = "_23-39pe";
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
////////////////////////////////////20kV///////23-65pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(23, 65);
	npe_cut = "_23-65pe";
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
////////////////////////////////////20kV///////80-160pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(80, 160);
	npe_cut = "_80-160pe";
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
}
//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18//18
if (exp == "190404_Cd_18kV_850V_46V_th230mV" && aggressive_PMT_cuts == 0 && forms) { 	
	std::string folder = "Cd_46V_18kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 140);
	set_bins(140);
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
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////18kV///////0-21pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 21);
	npe_cut = "_0-21pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////18kV///////21-40pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(21, 40);
	npe_cut = "_21-40pe";
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

////////////////////////////////////18kV///////21-62pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(21, 62);
	npe_cut = "_21-62pe";
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

////////////////////////////////////18kV///////70-160pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(70, 160);
	npe_cut = "_70-160pe";
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
if (exp == "190404_Cd_18kV_850V_46V_th230mV" && aggressive_PMT_cuts == 1 && forms) { 	
	std::string folder = "Cd_46V_18kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 140);
	set_bins(140);
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
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////18kV///////0-18pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 18);
	npe_cut = "_0-18pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////18kV///////18-32pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 32);
	npe_cut = "_18-32pe";
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

////////////////////////////////////18kV///////18-50pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 50);
	npe_cut = "_18-50pe";
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

////////////////////////////////////18kV///////60-160pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(60, 160);
	npe_cut = "_60-160pe";
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
if (exp == "190404_Cd_16kV_850V_46V_th210mV" && aggressive_PMT_cuts == 0 && forms) { 	
	std::string folder = "Cd_46V_16kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 100);
	set_bins(100);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 140);
	std::string npe_cut = "_0-140pe";
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
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////16kV///////0-18pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 18);
	npe_cut = "_0-18pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////16kV///////18-32pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 32);
	npe_cut = "_18-32pe";
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

////////////////////////////////////16kV///////18-48pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 48);
	npe_cut = "_18-48pe";
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

////////////////////////////////////16kV///////60-140pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(60, 140);
	npe_cut = "_60-140pe";
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
if (exp == "190404_Cd_16kV_850V_46V_th210mV" && aggressive_PMT_cuts == 1 && forms) { 	
	std::string folder = "Cd_46V_16kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 100);
	set_bins(100);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 140);
	std::string npe_cut = "_0-140pe";
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
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////16kV///////0-14pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 14);
	npe_cut = "_0-14pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////16kV///////14-26pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(14, 26);
	npe_cut = "_14-26pe";
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

////////////////////////////////////16kV///////14-39pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(14, 39);
	npe_cut = "_14-39pe";
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

////////////////////////////////////16kV///////48-140pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(48, 140);
	npe_cut = "_48-140pe";
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
if (exp == "190404_Cd_14kV_850V_46V_th200mV" && aggressive_PMT_cuts == 0 && forms) { 	
	std::string folder = "Cd_46V_14kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 120);
	std::string npe_cut = "_0-120pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 60);
	set_bins(60);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////14kV///////0-13pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 13);
	npe_cut = "_0-13pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////14kV///////13-25pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(13, 25);
	npe_cut = "_13-25pe";
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

////////////////////////////////////14kV///////13-40pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(13, 40);
	npe_cut = "_13-40pe";
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

////////////////////////////////////14kV///////48-120pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(48, 120);
	npe_cut = "_48-120pe";
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
if (exp == "190404_Cd_14kV_850V_46V_th200mV" && aggressive_PMT_cuts == 1 && forms) { 	
	std::string folder = "Cd_46V_14kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 120);
	std::string npe_cut = "_0-120pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 60);
	set_bins(60);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////14kV///////0-10pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 10);
	npe_cut = "_0-10pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////14kV///////10-20pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 20);
	npe_cut = "_10-20pe";
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

////////////////////////////////////14kV///////10-34pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 34);
	npe_cut = "_10-34pe";
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

////////////////////////////////////14kV///////40-120pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(40, 120);
	npe_cut = "_40-120pe";
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
if (exp == "190404_Cd_12kV_850V_46V_th160mV" && aggressive_PMT_cuts == 0 && forms) { 	
	std::string folder = "Cd_46V_12kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 100);
	std::string npe_cut = "_0-100pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 60);
	set_bins(60);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////12kV///////0-10pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 10);
	npe_cut = "_0-10pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////12kV///////10-19pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 19);
	npe_cut = "_10-19pe";
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

////////////////////////////////////12kV///////10-32pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 32);
	npe_cut = "_10-32pe";
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

////////////////////////////////////12kV///////40-100pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(40, 100);
	npe_cut = "_40-100pe";
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
if (exp == "190404_Cd_12kV_850V_46V_th160mV" && aggressive_PMT_cuts == 1 && forms) { 	
	std::string folder = "Cd_46V_12kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 100);
	std::string npe_cut = "_0-100pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 60);
	set_bins(60);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////12kV///////0-8pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 8);
	npe_cut = "_0-8pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////12kV///////8-15pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 15);
	npe_cut = "_8-15pe";
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

////////////////////////////////////12kV///////8-24pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 24);
	npe_cut = "_8-24pe";
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

////////////////////////////////////12kV///////30-100pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(30, 100);
	npe_cut = "_30-100pe";
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
if (exp == "190404_Cd_10kV_850V_46V_th150mV" && aggressive_PMT_cuts == 0 && forms) { 	
	std::string folder = "Cd_46V_10kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 70);
	std::string npe_cut = "_0-70pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 50);
	set_bins(50);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////10kV///////0-8pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 8);
	npe_cut = "_0-8pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////10kV///////8-13pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 13);
	npe_cut = "_8-13pe";
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

////////////////////////////////////10kV///////8-23pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 23);
	npe_cut = "_8-23pe";
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

////////////////////////////////////10kV///////30-70pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(30, 70);
	npe_cut = "_30-70pe";
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
if (exp == "190404_Cd_10kV_850V_46V_th150mV" && aggressive_PMT_cuts == 1 && forms) { 	
	std::string folder = "Cd_46V_10kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 70);
	std::string npe_cut = "_0-70pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 50);
	set_bins(50);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////10kV///////0-6pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 6);
	npe_cut = "_0-6pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////10kV///////6-10pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(6, 10);
	npe_cut = "_6-10pe";
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

////////////////////////////////////10kV///////6-18pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(6, 18);
	npe_cut = "_6-18pe";
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

////////////////////////////////////10kV///////24-70pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(24, 70);
	npe_cut = "_24-70pe";
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
if (exp == "190404_Cd_8kV_850V_46V_th140mV" && aggressive_PMT_cuts == 0 && forms) { 	
	std::string folder = "Cd_8V_10kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 70);
	std::string npe_cut = "_0-70pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 50);
	set_bins(50);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////8kV///////0-5pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 5);
	npe_cut = "_0-5pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////8kV///////5-10pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(5, 10);
	npe_cut = "_5-10pe";
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

////////////////////////////////////8kV///////5-20pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(5, 20);
	npe_cut = "_5-20pe";
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

////////////////////////////////////8kV///////25-70pe/////////aggr==0/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(25, 70);
	npe_cut = "_25-70pe";
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
if (exp == "190404_Cd_8kV_850V_46V_th140mV" && aggressive_PMT_cuts == 1 && forms) { 	
	std::string folder = "Cd_46V_8kV_850V";
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AnalysisStates::PMT_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, aggressive_PMT_cuts, false);
	noise_cut(14, aggressive_PMT_cuts, false);
	noise_cut(13, aggressive_PMT_cuts, false);
	noise_cut(12, aggressive_PMT_cuts, false);
	draw_limits(0, 70);
	std::string npe_cut = "_0-70pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	
	ty(AnalysisStates::PMT_sum_N);
	set_zoom(0, 50);
	set_bins(50);
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
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AnalysisStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AnalysisStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AnalysisStates::PMT_tbN);
	ch(15);
	noise_cut(15, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, aggressive_PMT_cuts, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////8kV///////0-4pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 4);
	npe_cut = "_0-4pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AnalysisStates::MPPC_tbN_sum);
	set_bins(400);
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

////////////////////////////////////8kV///////4-7pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(4, 7);
	npe_cut = "_4-7pe";
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

////////////////////////////////////8kV///////4-16pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(4, 16);
	npe_cut = "_4-16pe";
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

////////////////////////////////////8kV///////20-70pe/////////aggr==1/////////////////////////////////////
	ty(AnalysisStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(20, 70);
	npe_cut = "_20-70pe";
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
