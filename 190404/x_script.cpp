
{
	if (true) {
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;	
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
