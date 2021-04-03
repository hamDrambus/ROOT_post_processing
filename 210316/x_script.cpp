{
if (false) {
	// nch(); set_zoom(0, 0.04, 0, 0.009); set_bins(1000); set_log_z();
	int channel = 59;
	bool display = true;
	int aggressiveness = 2;
	x_y_regions = {0.0030, 0.00041, 0.0086, 0.00073, 0.0104, 0.00073, 0.0141, 0.00098, 0.0141, 0.00157, 1, 0.18};
	cut_A_S_lower(x_y_regions, display, channel, "small_A-S_noise");
	if (aggressiveness>=1)//select only 1 photoelectron for calibration
		cut_A_S_upper(0.0132, 0.0001, 1, 0.0001, display, channel, "2pe");
	if (aggressiveness>=2)//remove afterimpulses
		cut_A_S_upper(0.0030, 0.00106, 0.0132, 0.00261, display, channel, "2pe_merged");
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
if (false) { //Simple rectange fit
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	ShapeFitData::SetNPars(2);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetBound(0, 1, 30);
	ShapeFitData::SetBound(1, 40, 60);
	ShapeFitData::SetPlotParameter(0);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetFitFunction(rectangle_pdf);
	view_event(0);
}
if (false) { //Rectange + background fit
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	ShapeFitData::SetNPars(5);
	//par[0] - width of rectangle, par[1] - center position, par[2] - fraction of bkg, (<1 !)
	//par[3] - start time for bkg (fix to 0us), par[4] - finish time for bkg (fix to 160us),
	ShapeFitData::SetBound(0, 1, 30);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 40, 60);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.9);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "Bkg_fr");
	ShapeFitData::SetBound(3, 0, 0);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 160, 160);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetFitFunction(rectangle_bkg_pdf);
	ShapeFitData::SetPlotParameter(0);
	view_event(0);
}
if (false) { //Triangle + background fit
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	ShapeFitData::SetNPars(5);
	//par[0] - width of rectangle, par[1] - center position, par[2] - fraction of bkg, (<1 !)
	//par[3] - start time for bkg (fix to 0us), par[4] - finish time for bkg (fix to 160us),
	ShapeFitData::SetBound(0, 1, 30);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 38, 50);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Front");
	ShapeFitData::SetBound(2, 0, 0.9);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "Bkg_fr");
	ShapeFitData::SetBound(3, 28, 28);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 58, 58);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetFitFunction(triangle_bkg_pdf);
	ShapeFitData::SetPlotParameter(0);
	view_event(0);
}
if (false) { //Rectange for fast component + triangle for slow
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	ShapeFitData::SetNPars(4);
	//par[0] - width of rectangle, par[1] - center of rectangle, par[2] - fraction(area) of triangle,
	//par[3] - width of triangle
	ShapeFitData::SetBound(0, 0.5, 17);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 43, 53);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.5);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "SlowFr");
	ShapeFitData::SetBound(3, 10, 20);
	ShapeFitData::SetPrecision(3, 0.05);
	ShapeFitData::SetParameterName(3, "SlowWidth");
	ShapeFitData::SetFitFunction(rect_tria_pdf);
	ShapeFitData::SetPlotParameter(0);
	view_event(0);
}
if (false) { //Simple gauss+bkg fit
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
	ShapeFitData::SetNPars(5);
	ShapeFitData::SetBound(0, 0.1, 8);
	ShapeFitData::SetPrecision(0, 0.05);
	ShapeFitData::SetParameterName(0, "Width");
	ShapeFitData::SetBound(1, 42, 55);
	ShapeFitData::SetPrecision(1, 0.05);
	ShapeFitData::SetParameterName(1, "Center");
	ShapeFitData::SetBound(2, 0, 0.8);
	ShapeFitData::SetPrecision(2, 0.005);
	ShapeFitData::SetParameterName(2, "BkgFr");
	ShapeFitData::SetBound(3, 0, 0);
	ShapeFitData::SetParameterName(3, "t1");
	ShapeFitData::SetBound(4, 160, 160);
	ShapeFitData::SetParameterName(4, "t2");
	ShapeFitData::SetFitFunction(gauss_bkg_pdf);
	ShapeFitData::SetPlotParameter(0);
	view_event(0);
}
if (true) { //Two gausses + bkg fit
	int PMT_state = 1, SiPM_state = 1; //256K
	ty(AStates::MPPC_shape_fit);
	for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
		int chan = post_processor->MPPC_channels[ich];
		noise_cut(chan, 0, SiPM_state, false);
	}
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
}
