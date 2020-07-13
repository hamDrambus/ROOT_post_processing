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
if (true) {
	int channel = 8;
	bool display = true;
	int aggressiveness = 1;
	cut_A_S_upper(0.003, 0.0, 0.004, 0.0, display, channel, "rem_smallA_largeS_1");
	cut_A_S_upper(0.004, 0.0, 0.01, 0.015, display, channel, "rem_smallA_largeS_2");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.35, 0, 2, 0, display, channel, "rem_A>0.35");
		cut_A_S_upper(0.0, 0.02, 1.6, 0.02, display, channel, "rem_S>0.02");
	}
}
if (false) {
	bool calibrate = false;
	unsigned int iteration = 0;
	analysis_history(calibrate, iteration); //20
	nex();
	analysis_history(calibrate, iteration); //18
	nex();
	analysis_history(calibrate, iteration); //16
	nex();
	analysis_history(calibrate, iteration); //14
	nex();
	analysis_history(calibrate, iteration); //12
	nex();
	analysis_history(calibrate, iteration); //10
	nex();
	analysis_history(calibrate, iteration);	//8
	nex();
	analysis_history(calibrate, iteration);	//6
	nex();
}

}
