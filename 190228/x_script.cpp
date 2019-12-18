{
	//This file is for substituting typing into console.
if (false) {
	x_y_regions = {0.0035, 0.0035, 0.000037, 0.012, 0.00013, 0.05};
	cut_A_S_fast_PMT(x_y_regions, 1, 5, "small_A-S_noise");
}
if (true) {
	analysis_history(false, 0); //20kV
	nex();
	analysis_history(false, 0); //20kV
	nex();
	analysis_history(false, 0); //18kV
	nex();
	analysis_history(false, 0); //16kV
	nex();
	analysis_history(false, 0); //20kV bkg
	nex();
}
}
