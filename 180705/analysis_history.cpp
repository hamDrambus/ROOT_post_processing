void time_zoom (double f_time, double to_time) {
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 8);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 9);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 10);
	cut_S_t_rect_select(f_time, to_time, 0, 1e5, false, 11);
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
//Even it is hard to read the whole analysis, it is better to move all A-S cuts for PMTs in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
if (channel==0) {
    if (aggressiveness !=1) {
    	cut_A_S_upper(1.4, 0, 1e2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 0.55, 1.4, 1.1, display, channel, "rem_S>~0.7");
	}
}
if (channel==1) {
	if (aggressiveness !=1) {
    	cut_A_S_upper(1.4, 0, 1e2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 1.2, 0.7, 1.5, display, channel, "rem_S>~1.3");
	}
}
if (channel==12) {
    x_y_regions = {0.0, 0.007, -0.000025, 0.05, 0.0007, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness !=1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 0.4, 0.1, display, channel, "rem_S>0.1");
	}
}
if (channel==11) {
    x_y_regions = {0.0, 0.007, 0.0, 0.05, 0.0007, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness !=1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 0.4, 0.1, display, channel, "rem_S>0.1");
	}
}
if (channel==10) {
    x_y_regions = {0.0, 0.006, 0.0, 0.05, 0.00074, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness !=1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 0.4, 0.1, display, channel, "rem_S>0.1");
	}
}
if (channel==9) {
    x_y_regions = {0.0, 0.006, 0.0, 0.05, 0.00074, 1};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness !=1) {
		cut_A_S_upper(0.4, 0, 10, 0, display, channel, "rem_A>0.4");
		cut_A_S_upper(0.0, 0.1, 1, 0.4, display, channel, "rem_S>0.1");
	}
}
if (channel==8) {
	cut_A_S_upper(0.002, 0.004, 0.0046, 0.004, display, channel, "small_A-S_noise");
	if (aggressiveness !=1) {
		cut_A_S_upper(0.1, 0, 2, 0, display, channel, "rem_A>0.1");
		cut_A_S_upper(0.0, 0.05, 0.1, 0.05, display, channel, "rem_S>0.05");
	}
}
//The following are set by hand for 20kV individually, there is no other way. 48V.
if (channel==32) {
    x_y_regions = {0.018, 0.018, 0.0010, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.046, 0.0064, display, channel, "2pe_merged");
}
if (channel==33) {
	x_y_regions = {0.016, 0.016, 0.0010, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.038, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0019, 0.038, 0.0050, display, channel, "2pe_merged");
}
if (channel==34) {
	x_y_regions = {0.016, 0.016, 0.0010, 0.03, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.048, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0019, 0.048, 0.0064, display, channel, "2pe_merged");
}
if (channel==35) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.049, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.049, 0.0068, display, channel, "2pe_merged");
}
if (channel==36) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.042, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.042, 0.0058, display, channel, "2pe_merged");
}
if (channel==37 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(37);
}
if (channel==38) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.046, 0.0063, display, channel, "2pe_merged");
}
if (channel==39) {
	x_y_regions = {0.018, 0.018, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.043, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.043, 0.0058, display, channel, "2pe_merged");
}
if (channel==40) {
	x_y_regions = {0.018, 0.018, 0.0012, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.047, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.047, 0.0066, display, channel, "2pe_merged");
}
if (channel==41) {
	x_y_regions = {0.020, 0.020, 0.00125, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.047, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0022, 0.047, 0.0066, display, channel, "2pe_merged");
}
if (channel==42) {
	x_y_regions = {0.018, 0.018, 0.00125, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.045, 0.0066, display, channel, "2pe_merged");
}
if (channel == 43) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0022, 0.045, 0.0066, display, channel, "2pe_merged");
}
if (channel==44 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(44);
}
if (channel==48) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.047, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.002, 0.047, 0.0062, display, channel, "2pe_merged");
}
if (channel==49) {
	x_y_regions = {0.015, 0.015, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.037, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.015, 0.0020, 0.037, 0.0047, display, channel, "2pe_merged");
}
if (channel==50) {
	x_y_regions = {0.015, 0.015, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.037, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.015, 0.0021, 0.037, 0.0052, display, channel, "2pe_merged");
}
if (channel==51) {
	x_y_regions = {0.016, 0.016, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.045, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.016, 0.0020, 0.045, 0.0060, display, channel, "2pe_merged");
}
if (channel==52) {
	x_y_regions = {0.017, 0.017, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.043, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.017, 0.0021, 0.043, 0.0064, display, channel, "2pe_merged");
}
if (channel==53) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.046, 0.0065, display, channel, "2pe_merged");
}
if (channel==54) {
	x_y_regions = {0.018, 0.018, 0.0010, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.042, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.042, 0.0058, display, channel, "2pe_merged");
}
if (channel==55) {
	x_y_regions = {0.018, 0.018, 0.0014, 0.04, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.04, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.018, 0.0021, 0.04, 0.0054, display, channel, "2pe_merged");
}
if (channel==56) {
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0022, 0.046, 0.0060, display, channel, "2pe_merged");
}
if (channel==57) {
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.046, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.046, 0.0061, display, channel, "2pe_merged");
}
if (channel==58) {
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.043, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.043, 0.0060, display, channel, "2pe_merged");
}
if (channel==59) {
	x_y_regions = {0.020, 0.020, 0.0012, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.044, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.044, 0.0061, display, channel, "2pe_merged");
}

}//noise_cut

void analysis_history(bool calibrate, int method = 0) {
//Created on 30.10.2019
//TODO 30.10.2019:
//  1) Move histogram stats function (top-left or top-right corner)
calibration_file = "180705/results/180705_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::map<std::string, std::pair<double, double> > S2_times;
S2_times["180705_Cd_20kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_18kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_16kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_14kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_13kV_800V_12bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_12kV_800V_6bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_11kV_800V_6bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_10kV_800V_6bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_9kV_800V_0bB_48V"] = std::pair<double, double> (25, 40);
S2_times["180705_Cd_8kV_800V_0bB_48V"] = std::pair<double, double> (25, 40);
std::map<std::string, std::string> exp_folders;
exp_folders["180705_Cd_20kV_800V_12bB_48V"] = "Cd_48V_20kV_800V";
exp_folders["180705_Cd_18kV_800V_12bB_48V"] = "Cd_48V_18kV_800V";
exp_folders["180705_Cd_16kV_800V_12bB_48V"] = "Cd_48V_16kV_800V";
exp_folders["180705_Cd_14kV_800V_12bB_48V"] = "Cd_48V_14kV_800V";
exp_folders["180705_Cd_13kV_800V_12bB_48V"] = "Cd_48V_13kV_800V";
exp_folders["180705_Cd_12kV_800V_6bB_48V"] = "Cd_48V_12kV_800V";
exp_folders["180705_Cd_11kV_800V_6bB_48V"] = "Cd_48V_11kV_800V";
exp_folders["180705_Cd_10kV_800V_6bB_48V"] = "Cd_48V_10kV_800V";
exp_folders["180705_Cd_9kV_800V_0bB_48V"] = "Cd_48V_9kV_800V";
exp_folders["180705_Cd_8kV_800V_0bB_48V"] = "Cd_48V_8kV_800V";

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
for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) { 
int channel = calib_channels[chi];
std::stringstream ss;
ss<<std::fixed<<std::setprecision(0)<<channel;
std::string ch_str = ss.str();
//parameters set by Cd_20kV_800V_48V.
if (channel>=32) {
ch(channel);
    ty(AStates::MPPC_A_S);
    set_zoom(0, 0.08, 0, 0.012);
    set_bins(500);
    noise_cut(channel, 1, true);
    saveas(data_output_path + folder + "/calibration/"+ch_str+"_A_S_zoom");
    ty(AStates::MPPC_Ss);
    set_zoom(0, 0.025);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss");    
    noise_cut(channel, 1, false);
    draw_limits(0.0005, 0.012);
    set_use_mean(); //overwrites old info from calibration file!
    unset_1peS(); //updates calibration if it was loaded from file
    update();
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_Ss_single_pe");
}

if (channel==8) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(1000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");    
	set_zoom(0, 0.02, 0, 0.05);
    set_bins(300);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==9) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");    
	set_zoom(0, 0.1, 0, 0.005);
    set_bins(600);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==10) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(1500);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");    
	set_zoom(0, 0.07, 0, 0.002);
    set_bins(300);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==11) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(2000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");    
	set_zoom(0, 0.07, 0, 0.002);
    set_bins(800);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
}
if (channel==12) {
ch(channel);
	ty(AStates::PMT_A_S);
    noise_cut(channel, 0, true);
	set_bins(3000);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S");    
	set_zoom(0, 0.07, 0, 0.002);
    set_bins(100);
    saveas(data_output_path + folder +"/calibration/"+ch_str+"_A_S_zoom");
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
if (!forms)
	return;
std::string aggr = "";
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
if (exp == "180705_Cd_20kV_800V_12bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 650; //Npes for full zoom (25-160us)
	int small_Npes = 300; //Npes for small zoom (25-~40us)
	int ch12_Npes = 100; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string str_small_Npes = "300";
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 0.2*small_Npes);
	draw_limits(0, 15);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 600);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);	
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////20kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(20, 115);
			npe_cut = "_20-115pe";
			Npe_tail_lims = std::pair<double, double> (0, 165);
			tail_bins = 300;
			break;			
		}
		case 1: {
			draw_limits(115, 150);
			npe_cut = "_115-150pe";
			Npe_tail_lims = std::pair<double, double> (0, 225);
			tail_bins = 350;
			break;			
		}
		case 2: {
			draw_limits(151, 175);
			npe_cut = "_151-175pe";
			Npe_tail_lims = std::pair<double, double> (0, 340);
			tail_bins = 500;
			break;
		}
		case 3: {
			draw_limits(151, 195);
			npe_cut = "_151-195pe";
			Npe_tail_lims = std::pair<double, double> (0, 355);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_18kV_800V_12bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 500; //Npes for full zoom (25-160us)
	int small_Npes = 280; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "280";
	int ch12_Npes = 80; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	central_SiPMs(true);
	set_bins(0, 300);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////18kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(50, 107);
			npe_cut = "_50-107pe";
			Npe_tail_lims = std::pair<double, double> (0, 140);
			tail_bins = 280;
			break;			
		}
		case 1: {
			draw_limits(108, 137);
			npe_cut = "_108-137pe";
			Npe_tail_lims = std::pair<double, double> (0, 190);
			tail_bins = 350;
			break;			
		}
		case 2: {
			draw_limits(138, 168);
			npe_cut = "_138-168pe";
			Npe_tail_lims = std::pair<double, double> (0, 285);
			tail_bins = 500;
			break;
		}
		case 3: {
			draw_limits(138, 188);
			npe_cut = "_138-188pe";
			Npe_tail_lims = std::pair<double, double> (0, 300);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_16kV_800V_12bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 500; //Npes for full zoom (25-160us)
	int small_Npes = 220; //Npes for small zoom (25-~40us)
	int ch12_Npes = 80; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string str_small_Npes = "220";
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 280);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////16kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(25, 70);
			npe_cut = "_25-70pe";
			Npe_tail_lims = std::pair<double, double> (0, 95);
			tail_bins = 220;
			break;			
		}
		case 1: {
			draw_limits(71, 115);
			npe_cut = "_71-115pe";
			Npe_tail_lims = std::pair<double, double> (0, 160);
			tail_bins = 300;
			break;			
		}
		case 2: {
			draw_limits(116, 151);
			npe_cut = "_116-151pe";
			Npe_tail_lims = std::pair<double, double> (0, 225);
			tail_bins = 500;
			break;
		}
		case 3: {
			draw_limits(116, 175);
			npe_cut = "_116-175pe";
			Npe_tail_lims = std::pair<double, double> (0, 250);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_14kV_800V_12bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 400; //Npes for full zoom (25-160us)
	int small_Npes = 220; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "220";
	int ch12_Npes = 70; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 280);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////14kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<3; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(20, 90);
			npe_cut = "_20-90pe";
			Npe_tail_lims = std::pair<double, double> (0, 120);
			tail_bins = 220;
			break;			
		}
		case 1: {
			draw_limits(91, 122);
			npe_cut = "_91-122pe";
			Npe_tail_lims = std::pair<double, double> (0, 165);
			tail_bins = 300;
			break;			
		}
		case 2: {
			draw_limits(91, 150);
			npe_cut = "_91-150pe";
			Npe_tail_lims = std::pair<double, double> (0, 195);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_13kV_800V_12bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 500; //Npes for full zoom (25-160us)
	int small_Npes = 220; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "220";
	int ch12_Npes = 70; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 280);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////13kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<3; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(15, 72);
			npe_cut = "_15-72pe";
			Npe_tail_lims = std::pair<double, double> (0, 91);
			tail_bins = 220;
			break;			
		}
		case 1: {
			draw_limits(73, 98);
			npe_cut = "_73-98pe";
			Npe_tail_lims = std::pair<double, double> (0, 132);
			tail_bins = 280;
			break;			
		}
		case 2: {
			draw_limits(73, 123);
			npe_cut = "_73-123pe";
			Npe_tail_lims = std::pair<double, double> (0, 160);
			tail_bins = 280;
			break;
		}
		case 3: {
			draw_limits(129, 170);
			npe_cut = "_129-170pe";
			Npe_tail_lims = std::pair<double, double> (0, 255);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_12kV_800V_6bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 400; //Npes for full zoom (25-160us)
	int small_Npes = 240; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "240";
	int ch12_Npes = 70; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 200);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////12kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(10, 85);
			npe_cut = "_10-85pe";
			Npe_tail_lims = std::pair<double, double> (0, 100);
			tail_bins = 200;
			break;			
		}
		case 1: {
			draw_limits(86, 108);
			npe_cut = "_86-108pe";
			Npe_tail_lims = std::pair<double, double> (0, 130);
			tail_bins = 240;
			break;			
		}
		case 2: {
			draw_limits(86, 140);
			npe_cut = "_86-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 162);
			tail_bins = 300;
			break;			
		}
		case 3: {
			draw_limits(148, 190);
			npe_cut = "_148-190pe";
			Npe_tail_lims = std::pair<double, double> (0, 250);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_11kV_800V_6bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 300; //Npes for full zoom (25-160us)
	int small_Npes = 220; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "220";
	int ch12_Npes = 70; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 200);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////11kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 30);
			npe_cut = "_0-30pe";
			Npe_tail_lims = std::pair<double, double> (0, 40);
			tail_bins = 100;
			break;			
		}
		case 1: {
			draw_limits(31, 52);
			npe_cut = "_31-52pe";
			Npe_tail_lims = std::pair<double, double> (0, 65);
			tail_bins = 160;
			break;			
		}
		case 2: {
			draw_limits(31, 73);
			npe_cut = "_31-73pe";
			Npe_tail_lims = std::pair<double, double> (0, 88);
			tail_bins = 180;
			break;			
		}
		case 3: {
			draw_limits(80, 140);
			npe_cut = "_80-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 200);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_10kV_800V_6bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 210; //Npes for full zoom (25-160us)
	int small_Npes = 210; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "210";
	int ch12_Npes = 60; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 180);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////10kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 13);
			npe_cut = "_0-13pe";
			Npe_tail_lims = std::pair<double, double> (0, 16);
			tail_bins = 80;
			break;			
		}
		case 1: {
			draw_limits(14, 28);
			npe_cut = "_14-28pe";
			Npe_tail_lims = std::pair<double, double> (0, 36);
			tail_bins = 90;
			break;			
		}
		case 2: {
			draw_limits(14, 45);
			npe_cut = "_14-45pe";
			Npe_tail_lims = std::pair<double, double> (0, 55);
			tail_bins = 120;
			break;			
		}
		case 3: {
			draw_limits(55, 85);
			npe_cut = "_55-85pe";
			Npe_tail_lims = std::pair<double, double> (0, 100);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_9kV_800V_0bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 120; //Npes for full zoom (25-160us)
	int small_Npes = 80; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "80";
	int ch12_Npes = 80; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 100);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////9kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 12);
			npe_cut = "_0-12pe";
			Npe_tail_lims = std::pair<double, double> (0, 15);
			tail_bins = 30;
			break;			
		}
		case 1: {
			draw_limits(13, 22);
			npe_cut = "_13-22pe";
			Npe_tail_lims = std::pair<double, double> (0, 25);
			tail_bins = 50;
			break;			
		}
		case 2: {
			draw_limits(13, 35);
			npe_cut = "_13-35pe";
			Npe_tail_lims = std::pair<double, double> (0, 39);
			tail_bins = 80;
			break;			
		}
		case 3: {
			draw_limits(42, 75);
			npe_cut = "_42-75pe";
			Npe_tail_lims = std::pair<double, double> (0, 90);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
if (exp == "180705_Cd_8kV_800V_0bB_48V" && method == 0) { 	
	ty(AStates::PMT_sum_N);
	int large_Npes = 100; //Npes for full zoom (25-160us)
	int small_Npes = 70; //Npes for small zoom (25-~40us)
	std::string str_small_Npes = "70";
	int ch12_Npes = 70; //Npes for fast PMT sum channel small zoom (25-~40us)
	std::string meth = "";
	off_ch(0); off_ch(1); off_ch(12);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	noise_cut(12, 0, false);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 8);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 9);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 10);
	cut_S_t_rect_select(0, d_S2_start, 0, 1e5, false, 11);
	set_bins(0, 20);
	draw_limits(0, 10);
	saveas(data_output_path + folder + "/fastPMTs_Npeaks_"+meth+"w_0-"+S2_start+"us");
	set_as_run_cut("noisy_pre_trigger");
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	set_bins(0, small_Npes);
	draw_limits(0, small_Npes);
	std::string npe_cut = "_0-" + str_small_Npes+ "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(12);
	unset_draw_limits();
	set_bins(0, ch12_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
    ch(8); on_ch(9); on_ch(10); on_ch(11);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
	set_bins(0, large_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
    ch(12);
	set_bins(0, small_Npes);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
	
	ty(AStates::MPPC_Npe_sum);
	set_bins(0, 80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbN_sum);
	central_SiPMs(true);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(1000);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	central_SiPMs(false);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
    ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");

	ty(AStates::PMT_tbS);
	ch(1);
	noise_cut(1, 1, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
////////////////////////////////////8kV///////////////////////////////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_sum_N);
		ch(8); on_ch(9); on_ch(10); on_ch(11);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 11);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_bins(0, small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 4);
			npe_cut = "_0-4pe";
			Npe_tail_lims = std::pair<double, double> (0, 6);
			tail_bins = 20;
			break;			
		}
		case 1: {
			draw_limits(5, 8);
			npe_cut = "_5-8pe";
			Npe_tail_lims = std::pair<double, double> (0, 11);
			tail_bins = 30;
			break;			
		}
		case 2: {
			draw_limits(5, 15);
			npe_cut = "_5-15pe";
			Npe_tail_lims = std::pair<double, double> (0, 17);
			tail_bins = 35;
			break;			
		}
		case 3: {
			draw_limits(20, 40);
			npe_cut = "_20-40pe";
			Npe_tail_lims = std::pair<double, double> (0, 45);
			tail_bins = large_Npes;
			break;
		}
		}
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 8);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 9);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 10);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 11);
		set_bins(0, tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		set_as_run_cut(tail_cut);
		ch(12);
		cut_S_t_rect_select(d_S2_start, 160, 0, 1e5, false, 12);
		unset_draw_limits();
		set_bins(0, large_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks");
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		set_bins(0, small_Npes);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_Npeaks_w"+S2_start+"-"+S2_finish+"us");
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		
		ty(AStates::MPPC_tbN_sum);
		central_SiPMs(true);
		set_bins(800);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
		central_SiPMs(false);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_edge_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_Npeaks");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_Npeaks");

		ty(AStates::PMT_tbS);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT8_form_by_S");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT9_form_by_S");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT10_form_by_S");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT11_form_by_S");
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMT12_form_by_S");
		
		ty(AStates::PMT_tbS);
		ch(1);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMT1_form_by_S");
	}
}
//END OF FORMS
} 
