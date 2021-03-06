//Even it is hard to read the whole analysis because of it, it is better to move all A-S cuts for PMT in one
//place. These cuts are used multiple times: during calibration - for A-S histogram and for Ss, the parameters are
//selected at that stage at 20kV; during Npe plots (for Cd peak selection, which is required during signal forms) and during 
//plotting PMT signal forms themselves. Without this function changing cut parameters would have to take place in several places
//which is very error-prone. Besides, it is easier to re-use this file for other analyzes.
void noise_cut(int channel, int aggressiveness, bool display) { //Same as
if (channel==15) {
	if (aggressiveness == 2) {
        cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
		cut_A_S_upper(0.0, 0.4, 0.9, 0.5, display, channel, "rem_S>~0.4");
    }    
	if (aggressiveness == 1) {
        x_y_regions = {-1, 0.04, 0, 0.8, 0.105, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	    cut_A_S_fast_PMT(x_y_regions, display, 15, "large_A_noise");
	    x_y_regions = {-1, 0.020, 0.017, 0.1, 0.0, 1e3};//small S ch15 Cd_20kV_850V_46V. Best to determine using 0-23 us peaks
	    cut_A_S_fast_PMT(x_y_regions, display, 15, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 15, "A<0.8");
    }
}
if (channel==14) {
	if (aggressiveness == 2) {
        cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
		cut_A_S_upper(0.0, 0.4, 1.0, 0.55, display, channel, "rem_S>~0.4");
    }  
    if (aggressiveness == 1) {
        x_y_regions = {-1, 0.035, 0, 0.8, 0.108, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	    cut_A_S_fast_PMT(x_y_regions, display, 14, "large_A_noise");
	    x_y_regions = {-1, 0, 0.032, 0.13, 0.0, 1e3};	
	    cut_A_S_fast_PMT(x_y_regions, display, 14, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 14, "A<0.8");
    }
}
if (channel==13) {
	if (aggressiveness == 2) {
        cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.4, 1.0, 0.55, display, channel, "rem_S>~0.4");
    }
    if (aggressiveness == 1) {
        x_y_regions = {-1, 0.025, 0, 0.8, 0.1, 1e3};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
        cut_A_S_fast_PMT(x_y_regions, display, 13, "large_A_noise");
        x_y_regions = {-1, 0, 0.018, 0.046, 0.0, 1e3};//small S ch15 Cd_20kV_850V_46V. Best to determine using 0-23 us peaks
        cut_A_S_fast_PMT(x_y_regions, display, 13, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 13, "A<0.8");
    }
}
if (channel==12) {
	if (aggressiveness == 2) {
        cut_A_S_upper(1.0, 0, 2, 0, display, channel, "rem_A>1.0");
		cut_A_S_upper(0.0, 0.4, 1.0, 0.55, display, channel, "rem_S>~0.4");
    }
    if (aggressiveness == 1) {
        x_y_regions = {-1, 0.025, 0, 0.45, 0.057, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 12, "large_A_noise");
	    x_y_regions = {-1, 0, 0.02, 0.054, 0.0, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 12, "small_S_noise");
		cut_A_S_upper(0.8, 0, 2, 0, display, 12, "A<0.8");
    }
}
if (channel==11) {
    if (aggressiveness == 1) {
        x_y_regions = {-1, 0.006, 0.000045, 0.4, 0.0062, 1e3};
        cut_A_S_fast_PMT(x_y_regions, display, 11, "large_A_noise");
	    cut_A_S_upper(0.005, 0.00018, 0.6, 0.019, display, 11);
    }
}
if (channel==10) {
    if (aggressiveness == 1) {
        x_y_regions = {-1, 0.006, 0.000045, 0.4, 0.0065, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 10, "large_A_noise");
	    cut_A_S_upper(0.003, 0.000118, 0.6, 0.0195, display, 10);
    }
}
if (channel==9) {
    if (aggressiveness == 1) {
        x_y_regions = {-1, 0.0042, 0, 0.4, 0.0065, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 9, "large_A_noise");
	    cut_A_S_upper(0.003, 0.00013, 0.6, 0.021, display, 9);
    }
}
if (channel==8) {
    if (aggressiveness == 1) {
        x_y_regions = {-1, 0.003, 0.000008, 0.4, 0.0069, 1e3};
	    cut_A_S_fast_PMT(x_y_regions, display, 8, "large_A_noise");
	    cut_A_S_upper(0.003, 0.00012, 0.6, 0.021, display, 8);
    }
}
//The following are set by hand for 20kV individually, there is no other way.
if (channel==32) {
    x_y_regions = {0.011, 0.011, 0.0011, 0.023, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.00065, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0255, 0.003, display, channel, "2pe_merged");
}
if (channel==33) {
	x_y_regions = {0.008, 0.008, 0.00115, 0.0175, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.008, 0.008, 0.00042, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0215, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.008, 0.0013, 0.0215, 0.0031, display, channel, "2pe_merged");
}
if (channel==34) {
	x_y_regions = {0.0115, 0.0115, 0.0013, 0.024, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.0285, 0.0037, display, channel, "2pe_merged");
}
if (channel==35) {
	x_y_regions = {0.0115, 0.0115, 0.0011, 0.024, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.027, 0.004, display, channel, "2pe_merged");
}
if (channel==36) {
	x_y_regions = {0.0115, 0.0115, 0.00125, 0.026, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0016, 0.026, 0.004, display, channel, "2pe_merged");
}
if (channel==37) {
	x_y_regions = {0.009, 0.009, 0.00115, 0.028, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.009, 0.009, 0.0005, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.009, 0.0012, 0.023, 0.0036, display, channel, "2pe_merged");
}
if (channel==38) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.025, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0285, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.0285, 0.0042, display, channel, "2pe_merged");
}
if (channel==39) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.025, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.025, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0017, 0.025, 0.0038, display, channel, "2pe_merged");
}
if (channel==40) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.023, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.027, 0.0037, display, channel, "2pe_merged");
}
if (channel==41) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.021, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==42) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.021, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0006, 0.03, 0.0021, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel == 43 && post_processor->isMultichannel(post_processor->current_type)) {
	off_ch(channel);
}
if (channel==44) {
	x_y_regions = {0.009, 0.009, 0.00115, 0.019, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.009, 0.009, 0.0004, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0225, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.009, 0.0013, 0.0225, 0.0032, display, channel, "2pe_merged");
}
if (channel==48) {
	x_y_regions = {0.012, 0.012, 0.0012, 0.02, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0006, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0275, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0016, 0.0275, 0.0038, display, channel, "2pe_merged");
}
if (channel==49) {
	x_y_regions = {0.0085, 0.0085, 0.0010, 0.02, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0085, 0.0085, 0.0004, 0.03, 0.0023, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0085, 0.0013, 0.02, 0.0028, display, channel, "2pe_merged");
}
if (channel==50) {
	x_y_regions = {0.0085, 0.0085, 0.0009, 0.02, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0085, 0.0085, 0.0003, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.02, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0085, 0.0013, 0.02, 0.0028, display, channel, "2pe_merged");
}
if (channel==51) {
	x_y_regions = {0.0105, 0.0105, 0.00105, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0105, 0.0105, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0255, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0105, 0.0016, 0.0255, 0.0037, display, channel, "2pe_merged");
}
if (channel==52) {
	x_y_regions = {0.0115, 0.0115, 0.00105, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.027, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0018, 0.027, 0.0041, display, channel, "2pe_merged");
}
if (channel==53) {
	x_y_regions = {0.012, 0.012, 0.0012, 0.027, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.028, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0018, 0.028, 0.0043, display, channel, "2pe_merged");
}
if (channel==54) {
	x_y_regions = {0.009, 0.009, 0.0011, 0.032, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.009, 0.009, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.023, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.009, 0.0014, 0.023, 0.0034, display, channel, "2pe_merged");
}
if (channel==55) {
	x_y_regions = {0.01, 0.01, 0.00105, 0.030, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.01, 0.01, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0235, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.01, 0.0015, 0.0235, 0.0036, display, channel, "2pe_merged");
}
if (channel==56) {
	x_y_regions = {0.0115, 0.0115, 0.0011, 0.032, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.0115, 0.0115, 0.0007, 0.03, 0.0024, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0115, 0.0014, 0.026, 0.004, display, channel, "2pe_merged");
}
if (channel==57) {
	x_y_regions = {0.012, 0.012, 0.00115, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.012, 0.012, 0.0007, 0.03, 0.0024, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0265, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.0015, 0.0265, 0.004, display, channel, "2pe_merged");
}
if (channel==58) {
	x_y_regions = {0.011, 0.011, 0.00115, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.025, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.025, 0.0038, display, channel, "2pe_merged");
}
if (channel==59) {
	x_y_regions = {0.011, 0.011, 0.0012, 0.035, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	x_y_regions = {0.011, 0.011, 0.0005, 0.03, 0.0022, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.01, 1, 0.01, display, channel, "rem_S>0.01");  
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.026, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.011, 0.0015, 0.026, 0.0038, display, channel, "2pe_merged");
}

}

void analysis_history(bool calibrate, int method = 0) {
//Created on 23.09.2019
//TODO 23.09.2019:
//  1) Move histogram stats function (top-left or top-right corner)

//method = 0 - no cuts at all, only PMT15 (original analysis of 08.2019).
//1 - very low A-S exclusion, also exclude high A low S. Calibration as well as Npe and signal forms must be plotted from scratch for every agrressiveness level.
//2 - new event selection (suppressing tails from muons, or just events with large tails)
if (method==1)
	calibration_file = "190404/results/190404_calibration_aggr=1.dat";
else
	calibration_file = "190404/results/190404_calibration.dat";
post_processor->calibr_info.Load(calibration_file);
std::map<std::string, std::pair<double, double> > S2_times;
if (0==method) {
	S2_times["190404_Cd_20kV_850V_46V_th250mV_0"] = std::pair<double, double> (25, 70);
	S2_times["190404_Cd_20kV_850V_46V_th250mV"] = std::pair<double, double> (25, 70);
	S2_times["190404_Cd_18kV_850V_46V_th230mV"] = std::pair<double, double> (25, 70);
	S2_times["190404_Cd_16kV_850V_46V_th210mV"] = std::pair<double, double> (25, 70);
	S2_times["190404_Cd_14kV_850V_46V_th200mV"] = std::pair<double, double> (25, 70);
	S2_times["190404_Cd_12kV_850V_46V_th160mV"] = std::pair<double, double> (25, 70);
	S2_times["190404_Cd_10kV_850V_46V_th150mV"] = std::pair<double, double> (25, 80);
	S2_times["190404_Cd_8kV_850V_46V_th140mV"] = std::pair<double, double> (25, 80);
} else {
	S2_times["190404_Cd_20kV_850V_46V_th250mV_0"] = std::pair<double, double> (25, 40);
	S2_times["190404_Cd_20kV_850V_46V_th250mV"] = std::pair<double, double> (25, 40);
	S2_times["190404_Cd_18kV_850V_46V_th230mV"] = std::pair<double, double> (25, 40);
	S2_times["190404_Cd_16kV_850V_46V_th210mV"] = std::pair<double, double> (25, 40);
	S2_times["190404_Cd_14kV_850V_46V_th200mV"] = std::pair<double, double> (25, 40);
	S2_times["190404_Cd_12kV_850V_46V_th160mV"] = std::pair<double, double> (25, 40);
	S2_times["190404_Cd_10kV_850V_46V_th150mV"] = std::pair<double, double> (25, 40);
	S2_times["190404_Cd_8kV_850V_46V_th140mV"] = std::pair<double, double> (25, 40);
}
std::map<std::string, std::string> exp_folders;
exp_folders["190404_Cd_20kV_850V_46V_th250mV_0"] = "Cd_46V_20kV_850V_0";
exp_folders["190404_Cd_20kV_850V_46V_th250mV"] = "Cd_46V_20kV_850V";
exp_folders["190404_Cd_18kV_850V_46V_th230mV"] = "Cd_46V_18kV_850V";
exp_folders["190404_Cd_16kV_850V_46V_th210mV"] = "Cd_46V_16kV_850V";
exp_folders["190404_Cd_14kV_850V_46V_th200mV"] = "Cd_46V_14kV_850V";
exp_folders["190404_Cd_12kV_850V_46V_th160mV"] = "Cd_46V_12kV_850V";
exp_folders["190404_Cd_10kV_850V_46V_th150mV"] = "Cd_46V_10kV_850V";
exp_folders["190404_Cd_8kV_850V_46V_th140mV"] = "Cd_46V_8kV_850V";

//Ultimately I have to choose single procedure for signal forms among them for all data.
//TODO: experiments loop (need long strings whens saving pictures). Currently I have to run this calibration script for each nex();  
//TODO: move S2 times from cut_S2_times.cpp here. Use them instead of 23-65us
bool forms = !calibrate;
for (std::size_t chi = 0, chi_end_ = calib_channels.size(); calibrate && chi!=chi_end_; ++chi) { 
int channel = calib_channels[chi];

//CALIBRATION (slow PMT and SiPMs)
//parameters set by Cd_20kV_850V_46V, but valid for all fields. 
if (channel==15) {
ch(15);
	ty(AStates::PMT_A_S);
    if (method != 1) { //no A-S cuts
	    set_zoom(0, 0.3, 0, 0.04);
	    set_bins(60);
	    cut_S_t_rect_exclude(23, 65, 0, 1e5, false); //exclude signal region for calibration. May be even better to use 0-23 us selectoin only
		noise_cut(15, 0, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us_zoom");//w = with, wo = without
        set_zoom(0, 1.3, 0, 0.5);
        set_bins(300);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us");
        ty(AStates::PMT_Ss);
		noise_cut(15, 0, true);        
		set_zoom(0, 0.15);
        set_bins(200);
        cut_S_t_rect_exclude(23, 65, 0, 1e5, false);
        draw_limits(0.0, 0.055); //uncertainty 0.05-0.06 => S1pe 0.0224467-0.0251765 V*us
        set_use_mean(); //overwrites old info from calibration file!
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_Ss_wo_23-65us");
    }
    if (method == 1) {
	    set_zoom(0, 0.3, 0, 0.04);
	    set_bins(60);
	    cut_S_t_rect_exclude(23, 65, 0, 1e5, false); //exclude signal region for calibration. May be even better to use 0-23 us selectoin only
        noise_cut(15, 1, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us_aggr=1_zoom");//w = with, wo = without
        set_zoom(0, 1.3, 0, 0.5);
        set_bins(300);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_A_S_wo_23-65us_aggr=1");
        ty(AStates::PMT_Ss);
        set_zoom(0, 0.15);
        set_bins(200);
        cut_S_t_rect_exclude(23, 65, 0, 1e5, false);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_Ss_wo_23-65us");
        noise_cut(15, 1, false);
        draw_limits(0.0, 0.055); //uncertainty 0.05-0.06 => S1pe 0.0224467-0.0251765 V*us
        set_use_mean(); //overwrites old info from calibration file!
        unset_1peS(); //updates calibration if it was loaded from file
        update();
        saveas("190404/results/Cd_46V_20kV_850V/calibration/15_Ss_wo_23-65us_aggr=1");
    }
}
if (channel==14) {
ch(14);
    ty(AStates::PMT_A_S);
    if (method != 1) { //no cuts on A-S
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(300);
        noise_cut(14, 0, true);
        cut_S_t_rect_select(23, 65, 0, 1e5, false);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_w_23-65us");
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(100);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/14_A_S_w_23-65us_zoom");

        ty(AStates::PMT_Ss);
        set_zoom(0, 0.12);
        noise_cut(14, 0, false);
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
    if (1 == method) { //such cuts on A-S that Ss histogram for 0-23us peaks shows only 1 peak
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(300);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        noise_cut(14, 1, true);
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

        ty(AStates::PMT_Ss);
        set_zoom(0, 0.12);
        cut_S_t_rect_select(0, 23, 0, 1e5, false);
        set_bins(200);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/Ss_14_w_0-23us");
        noise_cut(14, 1, false);
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
    ty(AStates::PMT_A_S);
    if (method != 1) { //no cuts on A-S
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(400);
        noise_cut(13, 0, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S");        
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(120);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S_zoom");

        ty(AStates::PMT_Ss);
        set_zoom(0, 0.1);
        noise_cut(13, 0, false);
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
    if (method == 1) {
        set_zoom(0, 0.8, 0, 0.2);        
        set_bins(400);
        noise_cut(13, 1, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S_aggr=1");        
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(120);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/13_A_S_aggr=1_zoom");

        ty(AStates::PMT_Ss);
        set_zoom(0, 0.1);
        noise_cut(13, 1, false);
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
    ty(AStates::PMT_A_S);
    if (method != 1) { //no cuts on A-S
        set_zoom(0, 0.5, 0, 0.1);        
        set_bins(400);
        noise_cut(12, 0, true);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S");        
        set_zoom(0, 0.2, 0, 0.035);
        set_bins(150);
        saveas("190404/results/Cd_46V_20kV_850V/calibration/12_A_S_zoom");

        ty(AStates::PMT_Ss);
        set_zoom(0, 0.06);
        noise_cut(12, 0, false);
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
    if (method == 1) {
        set_zoom(0, 0.5, 0, 0.1);        
        set_bins(400);
        noise_cut(12, 1, true);
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

        ty(AStates::PMT_Ss);
        set_zoom(0, 0.06);
        noise_cut(12, 1, false);
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
if (channel==11 && method == 1) {
ch(11);
    ty(AStates::PMT_A_S);
    noise_cut(11, 1, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/11_A_S_aggr=1");
    set_zoom(0, 0.08, 0, 0.003);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/11_A_S_aggr=1_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas("190404/results/Cd_46V_20kV_850V/calibration/11_A_S_aggr=1_zoom1");
}
if (channel==10 && method == 1) {
ch(10);
    ty(AStates::PMT_A_S);
    noise_cut(10, 1, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/10_A_S_aggr=1");
    set_zoom(0, 0.08, 0, 0.003);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/10_A_S_aggr=1_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas("190404/results/Cd_46V_20kV_850V/calibration/10_A_S_aggr=1_zoom1");
}
if (channel==9 && method == 1) {
ch(9);
    ty(AStates::PMT_A_S);
    noise_cut(9, 1, true);
    set_bins(3000);
    set_zoom(0, 0.55, 0, 0.02);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/9_A_S_aggr=1");
    set_zoom(0, 0.08, 0, 0.003);
    saveas("190404/results/Cd_46V_20kV_850V/calibration/9_A_S_aggr=1_zoom");
    set_zoom(0, 0.05, 0, 0.001);        
    saveas("190404/results/Cd_46V_20kV_850V/calibration/9_A_S_aggr=1_zoom1");
}
if (channel==8 && method == 1) {
ch(8);
    ty(AStates::PMT_A_S);
    noise_cut(8, 1, true);
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
	calib_save(calibration_file);
}
//SiPMs were calibrated before the creation of this file so they are omitted. They will be present in analysis_history.cpp for other experimental days.
//END OF CALIBRATION

//FORMS NO CUTS/PEAK-NOISE SELECTIONS
//First are signal forms without cuts at all, even Npe, aggressiveness = 0. There are time windows for Npe though.
if (!forms)
	return;
std::string aggr = "";
if (method == 0)
	aggr = "";
if (method == 1)
	aggr = "_aggr=1";
if (method == 2)
	aggr = "_v2";
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
if (exp == "190404_Cd_20kV_850V_46V_th250mV" && method == 0 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 160);
	set_bins(160);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/slowPMTs_Npe");
	ty(AStates::PMT_sum_N);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, aggressive_PMT_cuts, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(15);
	noise_cut(15, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_S");
	ch(14);
	noise_cut(14, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_S");
	ch(13);
	noise_cut(13, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_S");
	ch(12);
	noise_cut(12, 0, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_S");
////////////////////////////////////20kV///////0-28pe/////////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 28);
	npe_cut = "_0-28pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////28-48pe/////////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 48);
	npe_cut = "_28-48pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////28-80pe/////////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(28, 80);
	npe_cut = "_28-80pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////90-160pe/////////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(90, 160);
	npe_cut = "_90-160pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////21-62pe///As 18 kV////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(21, 62);
	npe_cut = "_x_21-62pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 48);
	npe_cut = "_x_18-48pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(13, 40);
	npe_cut = "_x_13-40pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 32);
	npe_cut = "_x_10-32pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 23);
	npe_cut = "_x_8-23pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(5, 20);
	npe_cut = "_x_5-20pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_20kV_850V_46V_th250mV" && method == 1 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 160);
	set_bins(160);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 1, false);
	noise_cut(14, 1, false);
	noise_cut(13, 1, false);
	noise_cut(12, 1, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/slowPMTs_Npe");
	ty(AStates::PMT_sum_N);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 1, false);
	noise_cut(9, 1, false);
	noise_cut(10, 1, false);
	noise_cut(11, 1, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

	ty(AStates::PMT_tbS);
	ch(15);
	noise_cut(15, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_S");
	ch(14);
	noise_cut(14, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_S");
	ch(13);
	noise_cut(13, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_S");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 90);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_S");
////////////////////////////////////20kV///////0-23pe/////////aggr==1/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	draw_limits(0, 23);
	npe_cut = "_0-23pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////23-39pe/////////aggr==1/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(23, 39);
	npe_cut = "_23-39pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////23-65pe/////////aggr==1/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(23, 65);
	npe_cut = "_23-65pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
	ch(15);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_S");
	ch(14);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_S");
	ch(13);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_S");
	ch(12);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_S");
////////////////////////////////////20kV///////80-160pe/////////aggr==1/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(80, 160);
	npe_cut = "_80-160pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");
	ty(AStates::MPPC_tbS_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_S");

	ty(AStates::PMT_tbN);
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

	ty(AStates::PMT_tbS);
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
if (exp == "190404_Cd_18kV_850V_46V_th230mV" && method == 0 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 140);
	set_bins(140);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/slowPMTs_Npe");
	ty(AStates::PMT_sum_N);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////18kV///////0-21pe/////////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 21);
	npe_cut = "_0-21pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(21, 40);
	npe_cut = "_21-40pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(21, 62);
	npe_cut = "_21-62pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(70, 160);
	npe_cut = "_70-160pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_18kV_850V_46V_th230mV" && method == 1 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 140);
	set_bins(140);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 1, false);
	noise_cut(14, 1, false);
	noise_cut(13, 1, false);
	noise_cut(12, 1, false);
	draw_limits(0, 160);
	std::string npe_cut = "_0-160pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 1, false);
	noise_cut(9, 1, false);
	noise_cut(10, 1, false);
	noise_cut(11, 1, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////18kV///////0-18pe/////////aggr==1/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 18);
	npe_cut = "_0-18pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 32);
	npe_cut = "_18-32pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 50);
	npe_cut = "_18-50pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(60, 160);
	npe_cut = "_60-160pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_16kV_850V_46V_th210mV" && method == 0 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 100);
	set_bins(100);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, 140);
	std::string npe_cut = "_0-140pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/slowPMTs_Npe");
	ty(AStates::PMT_sum_N);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////16kV///////0-18pe/////////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 18);
	npe_cut = "_0-18pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 32);
	npe_cut = "_18-32pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(18, 48);
	npe_cut = "_18-48pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(60, 140);
	npe_cut = "_60-140pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_16kV_850V_46V_th210mV" && method == 1 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 100);
	set_bins(100);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 1, false);
	noise_cut(14, 1, false);
	noise_cut(13, 1, false);
	noise_cut(12, 1, false);
	draw_limits(0, 140);
	std::string npe_cut = "_0-140pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 1, false);
	noise_cut(9, 1, false);
	noise_cut(10, 1, false);
	noise_cut(11, 1, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+"/8_form_by_Npeaks");

////////////////////////////////////16kV///////0-14pe/////////aggr==1/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 14);
	npe_cut = "_0-14pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(14, 26);
	npe_cut = "_14-26pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(14, 39);
	npe_cut = "_14-39pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(48, 140);
	npe_cut = "_48-140pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_14kV_850V_46V_th200mV" && method == 0 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, 120);
	std::string npe_cut = "_0-120pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	ty(AStates::PMT_sum_N);
	set_zoom(0, 60);
	set_bins(60);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 0, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////14kV///////0-13pe/////////aggr==0/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 13);
	npe_cut = "_0-13pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(13, 25);
	npe_cut = "_13-25pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(13, 40);
	npe_cut = "_13-40pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(48, 120);
	npe_cut = "_48-120pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_14kV_850V_46V_th200mV" && method == 1 && forms) { 	
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 12);
	noise_cut(15, 1, false);
	noise_cut(14, 1, false);
	noise_cut(13, 1, false);
	noise_cut(12, 1, false);
	draw_limits(0, 120);
	std::string npe_cut = "_0-120pe";
	set_as_run_cut(npe_cut);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	set_zoom(0, 60);
	set_bins(60);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 8);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 9);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 10);
	cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, 11);
	noise_cut(8, 1, false);
	noise_cut(9, 1, false);
	noise_cut(10, 1, false);
	noise_cut(11, 1, false);
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/fastPMTs_Npeaks");
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 1, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
	ch(15);
	noise_cut(15, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/15_form_by_Npe");
	ch(14);
	noise_cut(14, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/14_form_by_Npe");
	ch(13);
	noise_cut(13, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/13_form_by_Npe");
	ch(12);
	noise_cut(12, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/12_form_by_Npe");

	ch(11);
	noise_cut(11, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/11_form_by_Npeaks");
	ch(10);
	noise_cut(10, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/10_form_by_Npeaks");
	ch(9);
	noise_cut(9, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/9_form_by_Npeaks");
	ch(8);
	noise_cut(8, 1, false);
	set_zoom(20, 70);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/8_form_by_Npeaks");

////////////////////////////////////14kV///////0-10pe/////////aggr==1/////////////////////////////////////
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 10);
	npe_cut = "_0-10pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 20);
	npe_cut = "_10-20pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 34);
	npe_cut = "_10-34pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(40, 120);
	npe_cut = "_40-120pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_12kV_850V_46V_th160mV" && method == 0 && forms) { 	
	int aggressive_PMT_cuts = 0;
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
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
	ty(AStates::PMT_sum_N);
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
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 10);
	npe_cut = "_0-10pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 19);
	npe_cut = "_10-19pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(10, 32);
	npe_cut = "_10-32pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(40, 100);
	npe_cut = "_40-100pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_12kV_850V_46V_th160mV" && method == 1 && forms) { 	
	int aggressive_PMT_cuts = 1;
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
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
	
	ty(AStates::PMT_sum_N);
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
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 8);
	npe_cut = "_0-8pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 15);
	npe_cut = "_8-15pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 24);
	npe_cut = "_8-24pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(30, 100);
	npe_cut = "_30-100pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_10kV_850V_46V_th150mV" && method == 0 && forms) { 	
	int aggressive_PMT_cuts = 0;
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
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
	ty(AStates::PMT_sum_N);
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
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 8);
	npe_cut = "_0-8pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 13);
	npe_cut = "_8-13pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(8, 23);
	npe_cut = "_8-23pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(30, 70);
	npe_cut = "_30-70pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_10kV_850V_46V_th150mV" && method == 1 && forms) { 	
	int aggressive_PMT_cuts = 1;
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
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
	
	ty(AStates::PMT_sum_N);
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
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 6);
	npe_cut = "_0-6pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(6, 10);
	npe_cut = "_6-10pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(6, 18);
	npe_cut = "_6-18pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(24, 70);
	npe_cut = "_24-70pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_8kV_850V_46V_th140mV" && method == 0 && forms) { 	
	int aggressive_PMT_cuts = 0;
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
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
	ty(AStates::PMT_sum_N);
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
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 5);
	npe_cut = "_0-5pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(5, 10);
	npe_cut = "_5-10pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(5, 20);
	npe_cut = "_5-20pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(25, 70);
	npe_cut = "_25-70pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_8kV_850V_46V_th140mV" && method == 1 && forms) { 	
	int aggressive_PMT_cuts = 1;
	double S2_t_start = S2_times.find(exp)->second.first, S2_t_finish = S2_times.find(exp)->second.second;
	ty(AStates::PMT_Npe_sum);
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
	
	ty(AStates::PMT_sum_N);
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
	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 50);
	set_bins(50);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
		cut_S_t_rect_select(S2_t_start, S2_t_finish, 0, 1e5, false, chan);
	}
	update();
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, aggressive_PMT_cuts, false);
	}
	set_zoom(20, 70);
	set_bins(800);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(0, 4);
	npe_cut = "_0-4pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(400);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(4, 7);
	npe_cut = "_4-7pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(4, 16);
	npe_cut = "_4-16pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
	ty(AStates::PMT_Npe_sum);
	unset_as_run_cut(npe_cut);
	draw_limits(20, 70);
	npe_cut = "_20-70pe";
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/slowPMTs_Npe");
	set_as_run_cut(npe_cut);

	ty(AStates::MPPC_tbNpe_sum);
	set_bins(600);
	saveas(std::string("190404/results/") + folder + "/forms"+aggr+npe_cut+"/SiPMs_form_by_Npe");

	ty(AStates::PMT_tbN);
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
if (exp == "190404_Cd_20kV_850V_46V_th250mV" && forms && 2==method) { 	
	std::string meth = "_v2";
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 250; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 150; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "150";
	set_zoom(0, slowPMT_small_Npes); 
	set_bins(slowPMT_small_Npes);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, slowPMT_small_Npes);
	std::string npe_cut = "_0-" + str_slowPMT_small_Npes + "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w_"+S2_start+"-"+S2_finish+"us");
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
	unset_draw_limits();
    set_zoom(0, slowPMT_large_Npes); 
	set_bins(slowPMT_large_Npes);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	set_zoom(0, 140);
	set_bins(140);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");

	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 120);
	set_bins(120);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 2, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
	ch(13);
	noise_cut(13, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
	ch(14);
	noise_cut(14, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
	ch(15);
	noise_cut(15, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
////////////////////////////////////20kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, slowPMT_small_Npes);
		set_bins(slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 20);
			npe_cut = "_0-20pe";
			Npe_tail_lims = std::pair<double, double> (0, 55);
			tail_bins = 150;			
			break;			
		}
		case 1: {
			draw_limits(20, 41);
			npe_cut = "_20-41pe";
			Npe_tail_lims = std::pair<double, double> (0, 85);
			tail_bins = 160;
			break;			
		}
		case 2: {
			draw_limits(20, 65);
			npe_cut = "_20-65pe";
			Npe_tail_lims = std::pair<double, double> (0, 115);
			tail_bins = 170;
			break;			
		}
		case 3: {
			draw_limits(80, 150);
			npe_cut = "_80-150pe";
			Npe_tail_lims = std::pair<double, double> (0, 215);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbNpe_sum);
		set_bins(600);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
	
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
		ch(13);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
		ch(14);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
		ch(15);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
	}
}
if (exp == "190404_Cd_18kV_850V_46V_th230mV" && forms && 2==method) { 	
	std::string meth = "_v2";
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 200; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 140; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "140";
	set_zoom(0, slowPMT_small_Npes); 
	set_bins(slowPMT_small_Npes);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, slowPMT_small_Npes);
	std::string npe_cut = "_0-" + str_slowPMT_small_Npes + "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w_"+S2_start+"-"+S2_finish+"us");
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
	unset_draw_limits();
    set_zoom(0, slowPMT_large_Npes); 
	set_bins(slowPMT_large_Npes);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	set_zoom(0, 110);
	set_bins(110);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");

	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 120);
	set_bins(120);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 2, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
	ch(13);
	noise_cut(13, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
	ch(14);
	noise_cut(14, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
	ch(15);
	noise_cut(15, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
////////////////////////////////////18kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, slowPMT_small_Npes);
		set_bins(slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 16);
			npe_cut = "_0-16pe";
			Npe_tail_lims = std::pair<double, double> (0, 37);
			tail_bins = 140;			
			break;			
		}
		case 1: {
			draw_limits(16, 33);
			npe_cut = "_16-33pe";
			Npe_tail_lims = std::pair<double, double> (0, 65);
			tail_bins = 140;
			break;			
		}
		case 2: {
			draw_limits(16, 53);
			npe_cut = "_16-53pe";
			Npe_tail_lims = std::pair<double, double> (0, 84);
			tail_bins = 150;
			break;			
		}
		case 3: {
			draw_limits(65, 140);
			npe_cut = "_65-140pe";
			Npe_tail_lims = std::pair<double, double> (0, 173);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbNpe_sum);
		set_bins(600);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
	
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
		ch(13);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
		ch(14);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
		ch(15);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
	}
}
if (exp == "190404_Cd_16kV_850V_46V_th210mV" && forms && 2==method) { 	
	std::string meth = "_v2";
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 180; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 120; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "120";
	set_zoom(0, slowPMT_small_Npes); 
	set_bins(slowPMT_small_Npes);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, slowPMT_small_Npes);
	std::string npe_cut = "_0-" + str_slowPMT_small_Npes + "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w_"+S2_start+"-"+S2_finish+"us");
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
	unset_draw_limits();
    set_zoom(0, slowPMT_large_Npes); 
	set_bins(slowPMT_large_Npes);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	set_zoom(0, 100);
	set_bins(100);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");

	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 2, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
	ch(13);
	noise_cut(13, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
	ch(14);
	noise_cut(14, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
	ch(15);
	noise_cut(15, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
////////////////////////////////////16kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, slowPMT_small_Npes);
		set_bins(slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 14);
			npe_cut = "_0-14pe";
			Npe_tail_lims = std::pair<double, double> (0, 32);
			tail_bins = 120;			
			break;			
		}
		case 1: {
			draw_limits(14, 29);
			npe_cut = "_14-29pe";
			Npe_tail_lims = std::pair<double, double> (0, 50);
			tail_bins = 120;
			break;			
		}
		case 2: {
			draw_limits(14, 45);
			npe_cut = "_14-45pe";
			Npe_tail_lims = std::pair<double, double> (0, 67);
			tail_bins = 120;
			break;			
		}
		case 3: {
			draw_limits(55, 120);
			npe_cut = "_55-120pe";
			Npe_tail_lims = std::pair<double, double> (0, 150);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbNpe_sum);
		set_bins(600);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
	
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
		ch(13);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
		ch(14);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
		ch(15);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
	}
}
if (exp == "190404_Cd_14kV_850V_46V_th200mV" && forms && 2==method) { 	
	std::string meth = "_v2";
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 140; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 110; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "110";
	set_zoom(0, slowPMT_small_Npes); 
	set_bins(slowPMT_small_Npes);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, slowPMT_small_Npes);
	std::string npe_cut = "_0-" + str_slowPMT_small_Npes + "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w_"+S2_start+"-"+S2_finish+"us");
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
	unset_draw_limits();
    set_zoom(0, slowPMT_large_Npes); 
	set_bins(slowPMT_large_Npes);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	set_zoom(0, 80);
	set_bins(80);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");

	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 80);
	set_bins(80);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 2, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
	ch(13);
	noise_cut(13, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
	ch(14);
	noise_cut(14, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
	ch(15);
	noise_cut(15, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
////////////////////////////////////14kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, slowPMT_small_Npes);
		set_bins(slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 11);
			npe_cut = "_0-11pe";
			Npe_tail_lims = std::pair<double, double> (0, 22);
			tail_bins = 110;			
			break;			
		}
		case 1: {
			draw_limits(11, 22);
			npe_cut = "_11-22pe";
			Npe_tail_lims = std::pair<double, double> (0, 36);
			tail_bins = 110;
			break;			
		}
		case 2: {
			draw_limits(11, 37);
			npe_cut = "_11-37pe";
			Npe_tail_lims = std::pair<double, double> (0, 50);
			tail_bins = 110;
			break;			
		}
		case 3: {
			draw_limits(45, 110);
			npe_cut = "_45-110pe";
			Npe_tail_lims = std::pair<double, double> (0, 140);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbNpe_sum);
		set_bins(600);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
	
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
		ch(13);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
		ch(14);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
		ch(15);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
	}
}
if (exp == "190404_Cd_12kV_850V_46V_th160mV" && forms && 2==method) { 	
	std::string meth = "_v2";
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 120; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 100; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "100";
	set_zoom(0, slowPMT_small_Npes); 
	set_bins(slowPMT_small_Npes);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, slowPMT_small_Npes);
	std::string npe_cut = "_0-" + str_slowPMT_small_Npes + "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w_"+S2_start+"-"+S2_finish+"us");
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
	unset_draw_limits();
    set_zoom(0, slowPMT_large_Npes); 
	set_bins(slowPMT_large_Npes);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	set_zoom(0, 80);
	set_bins(80);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");

	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 2, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
	ch(13);
	noise_cut(13, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
	ch(14);
	noise_cut(14, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
	ch(15);
	noise_cut(15, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
////////////////////////////////////12kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, slowPMT_small_Npes);
		set_bins(slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 9);
			npe_cut = "_0-9pe";
			Npe_tail_lims = std::pair<double, double> (0, 17);
			tail_bins = 100;			
			break;			
		}
		case 1: {
			draw_limits(9, 17);
			npe_cut = "_9-17pe";
			Npe_tail_lims = std::pair<double, double> (0, 25);
			tail_bins = 100;
			break;			
		}
		case 2: {
			draw_limits(9, 30);
			npe_cut = "_9-30pe";
			Npe_tail_lims = std::pair<double, double> (0, 38);
			tail_bins = 100;
			break;			
		}
		case 3: {
			draw_limits(39, 100);
			npe_cut = "_39-100pe";
			Npe_tail_lims = std::pair<double, double> (0, 111);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbNpe_sum);
		set_bins(600);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
	
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
		ch(13);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
		ch(14);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
		ch(15);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
	}
}
if (exp == "190404_Cd_10kV_850V_46V_th150mV" && forms && 2==method) { 	
	std::string meth = "_v2";
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 110; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 80; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "80";
	set_zoom(0, slowPMT_small_Npes); 
	set_bins(slowPMT_small_Npes);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, slowPMT_small_Npes);
	std::string npe_cut = "_0-" + str_slowPMT_small_Npes + "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w_"+S2_start+"-"+S2_finish+"us");
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
	unset_draw_limits();
    set_zoom(0, slowPMT_large_Npes); 
	set_bins(slowPMT_large_Npes);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	set_zoom(0, 60);
	set_bins(60);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");

	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 70);
	set_bins(70);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 2, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
	ch(13);
	noise_cut(13, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
	ch(14);
	noise_cut(14, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
	ch(15);
	noise_cut(15, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
////////////////////////////////////10kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, slowPMT_small_Npes);
		set_bins(slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 7);
			npe_cut = "_0-7pe";
			Npe_tail_lims = std::pair<double, double> (0, 12);
			tail_bins = 80;			
			break;			
		}
		case 1: {
			draw_limits(8, 12);
			npe_cut = "_8-12pe";
			Npe_tail_lims = std::pair<double, double> (0, 18);
			tail_bins = 80;
			break;			
		}
		case 2: {
			draw_limits(8, 24);
			npe_cut = "_8-24pe";
			Npe_tail_lims = std::pair<double, double> (0, 28);
			tail_bins = 80;
			break;			
		}
		case 3: {
			draw_limits(30, 80);
			npe_cut = "_30-80pe";
			Npe_tail_lims = std::pair<double, double> (0, 100);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbNpe_sum);
		set_bins(600);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
	
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
		ch(13);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
		ch(14);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
		ch(15);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
	}
}
if (exp == "190404_Cd_8kV_850V_46V_th140mV" && forms && 2==method) { 	
	std::string meth = "_v2";
	ty(AStates::PMT_Npe_sum);
	int slowPMT_large_Npes = 100; //Npes for full zoom (25-160us)
	int slowPMT_small_Npes = 70; //Npes for small zoom (25-~40us)
	std::string str_slowPMT_small_Npes = "70";
	set_zoom(0, slowPMT_small_Npes); 
	set_bins(slowPMT_small_Npes);
	off_ch(8); off_ch(9); off_ch(10); off_ch(11); off_ch(16);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
	noise_cut(15, 0, false);
	noise_cut(14, 0, false);
	noise_cut(13, 0, false);
	noise_cut(12, 0, false);
	draw_limits(0, slowPMT_small_Npes);
	std::string npe_cut = "_0-" + str_slowPMT_small_Npes + "pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w_"+S2_start+"-"+S2_finish+"us");
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
	unset_draw_limits();
    set_zoom(0, slowPMT_large_Npes); 
	set_bins(slowPMT_large_Npes);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
	
	ty(AStates::PMT_sum_N);
	off_ch(12); off_ch(13); off_ch(14); off_ch(15); off_ch(16);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 8);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 9);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 10);
	cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 11);
	noise_cut(8, 0, false);
	noise_cut(9, 0, false);
	noise_cut(10, 0, false);
	noise_cut(11, 0, false);
	set_zoom(0, 60);
	set_bins(60);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");

	ty(AStates::MPPC_Npe_sum);
	set_zoom(0, 60);
	set_bins(60);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, false);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, chan);
	}
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
	
	ty(AStates::MPPC_tbNpe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 2, false);
	}
	set_zoom(20, 90);
	set_bins(800);
	update();
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");
	
	ty(AStates::PMT_tbN);
	ch(8);
	noise_cut(8, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
	ch(9);
	noise_cut(9, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
	ch(10);
	noise_cut(10, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
    ch(11);
	noise_cut(11, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
    ch(12);
	noise_cut(12, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
	ch(13);
	noise_cut(13, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
	ch(14);
	noise_cut(14, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
	ch(15);
	noise_cut(15, 2, false);
	set_zoom(20, 90);
	set_bins(800);
	saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
////////////////////////////////////8kV///////////////////version 2//////////////////////////////////////////
	for (int i = 0; i<4; ++i) {
		std::pair<double, double> Npe_tail_lims;
		int tail_bins;	
		ty(AStates::PMT_Npe_sum);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, d_S2_finish, 0, 1e5, false, 15);
		unset_as_run_cut(npe_cut);
		unset_as_run_cut(tail_cut);
		set_zoom(0, slowPMT_small_Npes);
		set_bins(slowPMT_small_Npes);
		switch (i) {
		case 0: {
			draw_limits(0, 5.5);
			npe_cut = "_0-5pe";
			Npe_tail_lims = std::pair<double, double> (0, 9.5);
			tail_bins = 70;			
			break;			
		}
		case 1: {
			draw_limits(6, 9.5);
			npe_cut = "_6-9pe";
			Npe_tail_lims = std::pair<double, double> (0, 14.5);
			tail_bins = 70;
			break;			
		}
		case 2: {
			draw_limits(6, 17.5);
			npe_cut = "_6-17pe";
			Npe_tail_lims = std::pair<double, double> (0, 22.5);
			tail_bins = 70;
			break;			
		}
		case 3: {
			draw_limits(25, 70);
			npe_cut = "_25-70pe";
			Npe_tail_lims = std::pair<double, double> (0, 85);
			tail_bins = slowPMT_large_Npes;
			break;
		}
		}
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe_w"+S2_start+"-"+S2_finish+"us");
		set_as_run_cut(npe_cut);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 12);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 13);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 14);
		cut_S_t_rect_select(d_S2_start, 160.0, 0, 1e5, false, 15);
		set_zoom(0, tail_bins);
		set_bins(tail_bins);	
		draw_limits(Npe_tail_lims.first, Npe_tail_lims.second);
		update();
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/slowPMTs_Npe");
		set_as_run_cut(tail_cut);
		
		ty(AStates::MPPC_Npe_sum);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_Npe");
		ty(AStates::PMT_sum_N);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/fastPMTs_Npeaks");
		
		ty(AStates::MPPC_tbNpe_sum);
		set_bins(600);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/SiPMs_form_by_Npe");

		ty(AStates::PMT_tbN);
		ch(8);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/8_form_by_Npeaks");
		ch(9);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/9_form_by_Npeaks");
		ch(10);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/10_form_by_Npeaks");
		ch(11);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/11_form_by_Npeaks");
	
		ch(12);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/12_form_by_Npe");
		ch(13);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/13_form_by_Npe");
		ch(14);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/14_form_by_Npe");
		ch(15);
		saveas(data_output_path + folder + "/forms"+meth+npe_cut+"/15_form_by_Npe");
	}
}
//END OF FORMS
} 
