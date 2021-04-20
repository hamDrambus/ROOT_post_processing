{
if (false) {
	// nch(); set_zoom(0, 0.04, 0, 0.005); set_bins(1000); set_log_z();
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	cut_A_S_lower(0.0061, 0.00057, 0.01, 0.00049, display, channel, "small_A-S_noise");
	x_y_regions = {0.0061, 0.0061, 0.00046, 0.04, 0.00253, 1e3};
	cut_A_S_fast_PMT(x_y_regions, display, channel, "small_A-S_noise2");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0145, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0061, 0.00134, 0.0145, 0.00162, display, channel, "2pe_merged");
}
if (false) { //Inspect SiPM en spectrum
	double d_S2_start = 22;
	int SiPM_state = 0;
	nex();
	ty(AStates::MPPC_Npe_sum);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
		cut_t(d_S2_start, 160, false, chan);
	}
	set_bins(0, 800);
}
if (false) { //Two gausses + bkg fit
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	ShapeFitData::SetPeakType(ShapeFitData::ptNpe);
	ShapeFitData::SetNPars(7);
	ShapeFitData::SetBound(0, 0.1, 5);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 42, 55);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.55);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "BkgFr");
	ShapeFitData::SetBound(3, 0, 0);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 160, 160);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetBound(5, 1.5, 5.0);
	ShapeFitData::SetPrecision(5, 0.02);
	ShapeFitData::SetParameterName(5, "W2/W1");
	ShapeFitData::SetBound(6, 0, 0.45);
	ShapeFitData::SetPrecision(6, 0.01);
	ShapeFitData::SetParameterName(6, "Gauss2Fr");
	ShapeFitData::SetFitFunction(gauss_gauss_bkg_pdf);
	ShapeFitData::SetPlotParameter(0);
	view_event(0, 200);
}
if (true) { //Two gausses + bkg fit
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::PMT_shape_fit);
	fast_PMTs_only(); //slow PMTs' peaks are merging
	noise_cut(5, 0, PMT_state, 0);
	noise_cut(6, 0, PMT_state, 0);
	noise_cut(7, 0, PMT_state, 0);
	noise_cut(8, 0, PMT_state, 0);
	ShapeFitData::SetPeakType(ShapeFitData::ptNpe);
	ShapeFitData::SetNPars(7);
	ShapeFitData::SetBound(0, 0.1, 5);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 58, 70);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.55);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "BkgFr");
	ShapeFitData::SetBound(3, 0, 0);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 160, 160);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetBound(5, 1.5, 5.0);
	ShapeFitData::SetPrecision(5, 0.02);
	ShapeFitData::SetParameterName(5, "W2/W1");
	ShapeFitData::SetBound(6, 0, 0.45);
	ShapeFitData::SetPrecision(6, 0.01);
	ShapeFitData::SetParameterName(6, "Gauss2Fr");
	ShapeFitData::SetFitFunction(gauss_gauss_bkg_pdf);
	ShapeFitData::SetPlotParameter(0);
	view_event(0, 200);
}
}
