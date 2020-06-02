{
if (false) {
	x_y_regions = {12.7, -0.3, 28.3, 9.9, 50.7, 19.2, 79.3, 45.7, 249.9, 164};
	cut_x_y_right(x_y_regions, true, "1");
}
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
	int channel = 11;
	bool display = false;
	int aggressiveness = 1;
	cut_A_S_upper(0.00362, 0.000112, 0.009, 0.000238, display, channel, "rem_smallA_largeS");
	if (aggressiveness == 1) {
		cut_A_S_upper(0.53, 0, 2, 0, display, channel, "rem_A>0.53");
		cut_A_S_upper(0.0, 0.09, 1.6, 0.09, display, channel, "rem_S>0.09");
	}
}
if (false) {
	bool calibrate = false;
	unsigned int method = 1;
	analysis_history(calibrate, method); //20
	nex();
	analysis_history(calibrate, method); //20
	nex();
	analysis_history(calibrate, method); //18
	nex();
	analysis_history(calibrate, method); //16
	nex();
	analysis_history(calibrate, method); //14
	nex();
	analysis_history(calibrate, method); //12
	nex();
	analysis_history(calibrate, method);	//10
	nex();
	analysis_history(calibrate, method);	//8
	nex();
}
if (true) { //to study cuts on 4PMT Npe t=[25, 40] vs t=[25, 160]
	//setups are for 20kV, 5analysis_states.cpp (launch analysis_states(0, 0) before this code)
	std::string exp = post_processor->experiments[post_processor->current_exp_index];
	int first_run;
	auto first_run_entry = experiment_runs.find(exp);
	if (first_run_entry != experiment_runs.end())
		first_run = first_run_entry->second;
	else {
		std::cout<<"Error: Could not find starting run index for '"<<exp<<"'! Will print invalid event indexes."<<std::endl;
		first_run = -10000;
	}
	std::string FOLDER = "190404/results_v5/Cd_46V_20kV_850V/cut_events/";

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

	x_y_regions = {112, 56, 182, 109, 182, 56};
	cut_x_y_poly_select(x_y_regions, true, "4");
	update();
	saveaspng(FOLDER + "cut4/2D_plot");
	set_as_run_cut("4");
	print_accepted_events(FOLDER + "cut4/events.txt", first_run);
	unset_as_run_cut("4"); remcut(-1, "4");

	x_y_regions = {39.6, 19.8, 66.6, 39.7, 70.5, 37.4, 45.3, 17.0};
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
	x_y_regions = {112, 56, 182, 109, 182, 56};
	cut_x_y_poly_select(x_y_regions, true, "4");
	x_y_regions = {39.6, 19.8, 66.6, 39.7, 70.5, 37.4, 45.3, 17.0};
	cut_x_y_poly_select(x_y_regions, true, "5");
	x_y_regions = {85, 72, 152, 125, 173, 111, 104, 59};
	cut_x_y_poly_select(x_y_regions, true, "6");
	update();
	saveaspng(FOLDER + "all_cuts");
	remcut(-1, "1"); remcut(-1, "2"); remcut(-1, "3"); remcut(-1, "4"); remcut(-1, "5"); remcut(-1, "6");
}
} 
