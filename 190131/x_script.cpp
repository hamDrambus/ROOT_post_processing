{
	//This file is for substituting typing into console.
if (true) {	
	bool display = true;
	int aggressiveness = 1;
	int channel = 5;

	x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.05};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
    cut_A_S_upper(0.6, 0, 2, 0, display, channel, "rem_A>0.6");
    cut_A_S_upper(0.0, 0.01, 0.6, 0.046, display, channel, "rem_S>~0.01");
}
}
