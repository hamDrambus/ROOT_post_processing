{
	//x_y_regions = {0, 13, 0, 1, 22, 36, 0, 1, 43, 60, 0, 1, 127, 160, 0, 1};
	//x_y_regions = {60, 61, 0, 1, 121, 122, 0, 1};	
	//x_y_regions = {14.5, 15.5, 0, 1, 138, 138.5, 0, 1};
	//For fast PMT A-S cuts:		
	//x_y_regions = {0.0013, 0.0005, 0.0001, 0.05, 0.0032, 100.05}; //ch8 11kV
	//x_y_regions = {0.004, 0.005, 0.0008, 0.1, 0.007, 100.05}; //ch9 11kV
	//x_y_regions = {0.004, 0.005, 0.0008, 0.1, 0.0072, 100.05}; //ch10 11kV
	x_y_regions = {0.005, 0.005, 0.0008, 0.1, 0.0072, 100.05}; //ch11 11kV
	/*if (6 <=x_y_regions.size()) {
		std::vector<double> xs = {0.006, 0.006};
		std::vector<double> ys = {10 , 0.0025};
		viewRegion region(gPad->GetUxmin(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax());
		region.polyline_push(xs[0], ys[0]);
		region.polyline_push(xs[1], ys[1]);		
		TPolyLine *line = region.get_clipped_polyline();
		line->SetLineColor(kGreen);
		line->Draw("same");
		gPad->GetCanvas()->Update();
	}*/
} 
