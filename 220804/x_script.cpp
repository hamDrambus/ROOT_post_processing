{
if (false) {
	// nch(); set_zoom(0, 0.05, 0, 0.01); set_bins(500); set_log_z();
	int channel = 1;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_lower(0.020, 0.0019, 0.06, 0.0054, display, channel, "rem_smallA_smallS");
	cut_A_S_lower(0.06, 0.0054, 1.4, 0.25, display, channel, "rem_largeA_smallS");
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
	save_SiPM_Npe_table(data_output_path + "SiPM_Npes_Q1.00.txt");
}
}
