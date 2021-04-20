{
if (true) {
	//nch(); set_zoom(0, 0.06, 0, 0.01); set_bins(500);
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	x_y_regions = {0.0133, 0.0133, 0.00191, 0.06, 0.00950, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
}
}
