{
	//x_y_regions = {0, 13, 0, 1, 22, 36, 0, 1, 43, 60, 0, 1, 127, 160, 0, 1};
	//x_y_regions = {60, 61, 0, 1, 121, 122, 0, 1};	
	//x_y_regions = {14.5, 15.5, 0, 1, 138, 138.5, 0, 1};
	//For fast PMT A-S cuts:		
	x_y_regions = {0.0013, 0.0005, 0.0001, 0.016, 0.001, 0.05}; //ch8
	/*if (6 <=x_y_regions.size()) {
		std::vector<double> xs = {0.002, 0.016};
		std::vector<double> ys = {0.0025, 0.0005};
		TPolyLine *line = new TPolyLine(2, &(xs[0]), &(ys[0]));
		line->SetLineColor(kRed);
		gPad->GetCanvas()->cd();
		line->Draw("same");
		gPad->GetCanvas()->Update();
	}*/
} 
