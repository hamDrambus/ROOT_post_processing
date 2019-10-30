{
	//This file is for substituting typing into console.
	//x_y_regions = {0.0035, 0.0035, 0.00008, 0.012, 0.00013, 0.05};//large A ch15 Cd_20kV_850V_46V Not really important for S2 Npe
	//cut_A_S_fast_PMT(x_y_regions, false, 5, "small_A-S_noise");
	ty(AnalysisStates::PMT_Npe_sum);
	std::string meth = "_v2";
	set_zoom(0, 140);
	set_bins(140);
	ch(1);
	cut_S_t_rect_select(25, 40, 0, 1e5, false, 0);
	cut_S_t_rect_select(25, 40, 0, 1e5, false, 1);
	noise_cut(0, 1, false);
	noise_cut(1, 1, false);
	draw_limits(20, 36);
	std::string npe_cut = "_20-36pe";
	std::string tail_cut = "_rem_tail";
	set_as_run_cut(npe_cut);
	cut_S_t_rect_select(25, 160, 0, 1e5, false, 0);
	cut_S_t_rect_select(25, 160, 0, 1e5, false, 1);
}
