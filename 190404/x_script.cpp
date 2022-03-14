{
if (false) {
	// nch(); set_zoom(0, 0.08, 0, 0.012); set_bins(500);
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	x_y_regions = {0.0193, 0.0193, 0.00150, 0.08, 0.00795, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0412, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0193, 0.00241, 0.0412, 0.0057, display, channel, "2pe_merged");
}
if (false) {
	unsigned int max_iter = 6;
	for (int i = 1; i!=(max_iter+1); ++i)
		analysis_history(false, i);
}
if (false) {
	unsigned int max_iter = 6;
	unsigned int exps = post_processor->experiments.size();
	for (int e = 0; e!=exps; ++e) {
		for (int i = 1; i!=(max_iter+1); ++i)
			analysis_history(false, i);
		nex();
	}
}
if (true) {
	for (int e = 0; e != exp_area.experiments.size(); ++e) {
		analysis_history(0);
		nex();
	}
}
if (false) { //to study cuts on 4PMT Npe t=[23, 40] vs t=[23, 160]
	//setups are for 20kV, 6analysis_states.cpp (launch analysis_states(0, 0) with 'return;' after correlation setup there before running this code)
	std::string exp = post_processor->experiments[post_processor->current_exp_index];
	int first_run;
	auto first_run_entry = experiment_runs.find(exp);
	if (first_run_entry != experiment_runs.end())
		first_run = first_run_entry->second;
	else {
		std::cout<<"Error: Could not find starting run index for '"<<exp<<"'! Will print invalid event indexes."<<std::endl;
		first_run = -10000;
	}
	std::string FOLDER = "190404/results_v6/Cd_46V_20kV_850V/cut_events/";

	x_y_regions = {96, 18.5, 69.5, 18.5, 80.2, 39.7, 98.6, 39.7};
	cut_x_y_poly_select(x_y_regions, true, "1");
	update();
	saveaspng(FOLDER + "cut1/2D_plot");
	set_as_run_cut("1");
	print_accepted_events(FOLDER + "cut1/events.txt", first_run);
	unset_as_run_cut("1"); remcut(-1, "1");

	x_y_regions = {54, 16, 74, 36, 66, 16};
	cut_x_y_poly_select(x_y_regions, true, "2");
	update();
	saveaspng(FOLDER + "cut2/2D_plot");
	set_as_run_cut("2");
	print_accepted_events(FOLDER + "cut2/events.txt", first_run);
	unset_as_run_cut("2"); remcut(-1, "2");

	x_y_regions = {116, 5, 116, 46, 248, 46, 248, 5};
	cut_x_y_poly_select(x_y_regions, true, "3");
	update();
	saveaspng(FOLDER + "cut3/2D_plot");
	set_as_run_cut("3");
	print_accepted_events(FOLDER + "cut3/events.txt", first_run);
	unset_as_run_cut("3"); remcut(-1, "3");

	x_y_regions = {27.4, 3.2, 52, 15, 51, 3.2};
	cut_x_y_poly_select(x_y_regions, true, "4");
	update();
	saveaspng(FOLDER + "cut4/2D_plot");
	set_as_run_cut("4");
	print_accepted_events(FOLDER + "cut4/events.txt", first_run);
	unset_as_run_cut("4"); remcut(-1, "4");

	x_y_regions = {35.4, 14.5, 56.5, 27.8, 50.7, 39.7, 30.1, 25.8};
	cut_x_y_poly_select(x_y_regions, true, "5");
	update();
	saveaspng(FOLDER + "cut5/2D_plot");
	set_as_run_cut("5");
	print_accepted_events(FOLDER + "cut5/events.txt", first_run);
	unset_as_run_cut("5"); remcut(-1, "5");

	x_y_regions = {85, 72, 152, 125, 173, 111, 104, 59};
	cut_x_y_poly_select(x_y_regions, true, "6");
	update();
	saveaspng(FOLDER + "cut6/2D_plot");
	set_as_run_cut("6");
	print_accepted_events(FOLDER + "cut6/events.txt", first_run);
	unset_as_run_cut("6"); remcut(-1, "6");

	x_y_regions = {96, 18.5, 69.5, 18.5, 80.2, 39.7, 98.6, 39.7};
	cut_x_y_poly_select(x_y_regions, true, "1");
	x_y_regions = {54, 16, 74, 36, 66, 16};
	cut_x_y_poly_select(x_y_regions, true, "2");
	x_y_regions = {116, 5, 116, 46, 248, 46, 248, 5};
	cut_x_y_poly_select(x_y_regions, true, "3");
	x_y_regions = {27.4, 3.2, 52, 15, 51, 3.2};
	cut_x_y_poly_select(x_y_regions, true, "4");
	x_y_regions = {35.4, 14.5, 56.5, 27.8, 50.7, 39.7, 30.1, 25.8};
	cut_x_y_poly_select(x_y_regions, true, "5");
	x_y_regions = {85, 72, 152, 125, 173, 111, 104, 59};
	cut_x_y_poly_select(x_y_regions, true, "6");
	update();
	saveaspng(FOLDER + "all_cuts");
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
}
}
