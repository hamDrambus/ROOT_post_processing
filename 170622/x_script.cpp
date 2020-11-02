{
	/*if (true) { //requires .L ncompare_forms.cpp
		std::vector<std::string> kVs = {"20","18","16","14","12","10","08","06"};
			for (std::size_t i =0, i_end_ = kVs.size(); i!=i_end_; ++i) {
			ncompare_forms(kVs[i], "lin", "peak", 1800, "NRE", "comb");
			ncompare_forms(kVs[i], "log", "peak", 1800, "NRE", "comb");
			ncompare_forms(kVs[i], "log", "peak", 1800, "NRE", "sep");
			ncompare_forms(kVs[i], "log", "peak", 1800, "NLWRE", "comb");
			ncompare_forms(kVs[i], "log", "peak", 1800, "NLWRE", "sep");
			ncompare_forms(kVs[i], "log", "peak", 600, "NRE", "comb");
			ncompare_forms(kVs[i], "log", "peak", 600, "NRE", "sep");
			ncompare_forms(kVs[i], "log", "peak", 600, "NLWRE", "comb");
			ncompare_forms(kVs[i], "log", "peak", 600, "NLWRE", "sep");
			ncompare_forms(kVs[i], "log", "slope", 600, "NRE", "comb");
			ncompare_forms(kVs[i], "log", "slope", 600, "NRE", "sep");
			ncompare_forms(kVs[i], "log", "slope", 600, "NLWRE", "comb");
			ncompare_forms(kVs[i], "log", "slope", 600, "NLWRE", "sep");
		}
	}*/

if (false) {
	int offset = -2;
	cut_x_y_lower(25, 0 + offset, 48, 18+ offset, true, "1");
	cut_x_y_lower(48, 18+ offset, 68, 28+ offset, true, "2");
	cut_x_y_lower(68, 28+ offset, 103, 54+ offset, true, "3");
	cut_x_y_lower(103, 54+ offset, 120, 70+ offset, true, "4");
	cut_x_y_lower(120, 70+ offset, 300, 190+ offset, true, "5");
	update();
}
if (false) {
nch();
set_zoom(0, 0.06, 0, 0.01);
set_bins(1000);
noise_cut(post_processor->current_channel, 2, 0, true);
}
if (false) {
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_lower(0.012, 0.00079, 0.03, 0.00154, display, channel, "small_A-S_noise");
	x_y_regions = {0.0145, 0.012, 0.00020, 0.06, 0.0045, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	cut_A_S_upper(0, 0.04, 1, 0.04, display, channel, "rem_S>0.04");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0286, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.012, 0.00105, 0.0286, 0.00352, display, channel, "2pe_merged");
}

if (true) {
	bool calibrate = false;
	unsigned int iteration = 0;
	analysis_history(calibrate, iteration); //20
	nex();
	analysis_history(calibrate, iteration); //18
	nex();
	analysis_history(calibrate, iteration); //18
	nex();
	analysis_history(calibrate, iteration); //16
	nex();
	analysis_history(calibrate, iteration); //14
	nex();
}

}
