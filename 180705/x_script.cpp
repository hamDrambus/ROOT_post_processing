{
if (false) {
	channel_info<dB_info> atten0;
	atten0.push(0, dB_info(12)); //decibells, not ratio
	atten0.push(1, dB_info(12));
	dBs["180705_Cd_20kV_800V_12bB_48V"] = atten0;
}
if (false) {
	int channel = 59;
	bool display = true;
	int device_condition = 0;
	int aggressiveness = 2;

	x_y_regions = {0.021, 0.021, 0.0015, 0.05, 0.0, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise");
	cut_A_S_upper(0, 0.1, 1, 0.1, display, channel, "rem_S>0.1");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.044, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.020, 0.0023, 0.044, 0.0061, display, channel, "2pe_merged");
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
}
}
