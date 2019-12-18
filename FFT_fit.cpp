std::string dbl_to_str (double val, int precision)
{
	std::stringstream ss;
	ss<<std::fixed<<std::setprecision(precision)<<val;
	return ss.str();
}

void read_hist (TH1D *hist, std::string fname) {
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (!str.is_open()) {
        std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;	    
        return;
    }
	std::size_t real_size = 0;
	str.read((char*)&real_size, sizeof(std::size_t));
	std::cout<<"File '"<<fname<<"': "<<real_size<<" events"<<std::endl;
	double val;
	while (!str.eof()) {
		str.read((char*)&val, sizeof(double));
        hist->Fill(val);
	}
	str.close();
}

void read_hist_w (TH1D *hist, std::string fname) {
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (!str.is_open()) {
        std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;	    
        return;
    }
	std::size_t real_size = 0;
	str.read((char*)&real_size, sizeof(std::size_t));
	std::cout<<"File '"<<fname<<"': "<<real_size<<" events"<<std::endl;
	double val1;
    double val2;
	while (!str.eof()) {
		str.read((char*)&val1, sizeof(double));
		if (str.eof())
			break;
        str.read((char*)&val2, sizeof(double));
        hist->Fill(val1, val2);
	}
	str.close();
}

Double_t FittingF_2exp (Double_t *x, Double_t *par) {
  //par[0] - start time (fixed)
  //par[1] - amplitude1
  //par[2] - tau1
  //par[3] - amplitude2
  //par[4] - tau2
  return (x[0] <= par[0]) ? 0 : (par[1]*std::exp((par[0]-x[0])/par[2]) + par[3]*std::exp((par[0]-x[0])/par[4]));
}

int compare_forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    int Nbins = 1800;
    double time_left = 0, time_right = 160;//us
    double time_pretrigger_left = 7, time_pretrigger_right = 20;
	double norm_t_left = 28;
    TF1 *unity_f = new TF1("f1", "1", time_left, time_right);
    TH1D* hist_1 = new TH1D ("hist1", "hist1", Nbins, time_left, time_right);
    TH1D* hist_2 = new TH1D ("hist2", "hist2", Nbins, time_left, time_right);
    TH1D* hist_3 = new TH1D ("hist3", "hist3", Nbins, time_left, time_right);
    TH1D* hist_4 = new TH1D ("hist4", "hist4", Nbins, time_left, time_right);
    TH1D* hist_5 = new TH1D ("hist5", "hist5", Nbins, time_left, time_right);
    TH1D* hist_6 = new TH1D ("hist6", "hist6", Nbins, time_left, time_right);
	TH1D* hist_7 = new TH1D ("hist6", "hist6", Nbins, time_left, time_right);
    std::vector<std::string> Tds(7, "6.8");
	std::vector<double> norm_t_right =  {33.7, 35.3, 33.8, 34.52, 33.2, 34.9, 33.6}; //with trigger adjustment
	std::vector<double> fit_from = 	    {36.0, 36.0, 36.0, 36.0, 35.1, 36.0, 36.0}; //20, 18, ..., 8 kV
	std::vector<double> fit_to =        {160,  160,  160,  50,  50.0, 52.0, 52.0}; //edge SiPMs 20, 18, ..., 8 kV
	
    std::vector<TH1D*> hists;
    hists.push_back(hist_1);
    //hists.push_back(hist_2);
    //hists.push_back(hist_3);
    //hists.push_back(hist_4);
    //hists.push_back(hist_5);
    //hists.push_back(hist_6);
	//hists.push_back(hist_7);
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
    double max_val = 0;
	bool linear = true;
	bool do_fit = false;
	std::string prefix = "190404/results_v3/Cd_46V_16kV_850V/forms_Cd_peak/";
	read_hist_w (hist_1, prefix + "8_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "9_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "10_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "11_form_by_Npeaks.hdata");

	std::string framename = std::string("No WLS convolution ") + " " + Tds[0] + " Td";

    for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double baseline = 0;
        int counts = 0;
        for (int bin = 1, bin_end = hists[hh]->GetNbinsX()+1; bin!=bin_end; ++bin) {
		    if (hists[hh]->GetBinCenter(bin)>time_pretrigger_left && hists[hh]->GetBinCenter(bin)<time_pretrigger_right) {
	           baseline += hists[hh]->GetBinContent(bin);
               ++counts;
		    }
        }
        hists[hh]->Sumw2();
        baseline = -1*baseline/counts;
        hists[hh]->Add(unity_f, baseline);
    }
	
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double integral = 0;
        for (int bin = 1, bin_end = hists[hh]->GetNbinsX()+1; bin!=bin_end; ++bin) {
		    if (hists[hh]->GetBinCenter(bin)>norm_t_left && hists[hh]->GetBinCenter(bin)<norm_t_right[hh]) {
	           integral += hists[hh]->GetBinContent(bin) * hists[hh]->GetBinWidth(bin);
		    }
        }
		hists[hh]->Scale(1.0/integral);
    }

	double *re_signal_fft = new double [Nbins];
	double *im_signal_fft = new double [Nbins];
	double *re_conv_fft = new double [Nbins];
	double *im_conv_fft = new double [Nbins];

	hists[0]->FFT(0, "MAG");

	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh)
        max_val = std::max(max_val, hists[hh]->GetBinContent(hists[hh]->GetMaximumBin()));
    max_val*= linear ? 1.2 : 2;
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
	c_->SetGrid();
	c_->SetTicks();
	c_->ToggleEventStatus();
    c_->ToggleToolBar();
	if (!linear)
		c_->SetLogy();
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : 1e-4, max_val);
	frame->GetXaxis()->SetTitle("t [#mus]");
	frame->GetYaxis()->SetTitle("");
	frame->Draw();
	
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
		hists[hh]->SetLineWidth(2);
		hists[hh]->SetLineColor(palette_major[hh]);
		hists[hh]->Draw("hist Lsame");
    }
	std::vector<std::string> tau1, tau2;
	std::vector<TF1*> ffs(hists.size(), NULL);
	int precision1 = 2, precision2 = 0;
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_ && do_fit; ++hh) {
		ffs[hh] = new TF1("fit1", FittingF_2exp, fit_from[hh], fit_to[hh], 5);
		ffs[hh]->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
		ffs[hh]->FixParameter(0, fit_from[hh]);
		ffs[hh]->SetParLimits(1, 1e-3, 2);
		ffs[hh]->SetParLimits(2, 1, 10);
		if (fit_to[hh]<time_right) {
			ffs[hh]->SetParLimits(3, 1e-5, 1e-1);
			ffs[hh]->FixParameter(4, 1e6);	
		} else {
			ffs[hh]->SetParLimits(3, 3e-4, 1e-1);
			ffs[hh]->SetParLimits(4, 30, 1000);
		}
		ffs[hh]->SetLineColor(palette_minor[hh]);
    	hists[hh]->Fit(ffs[hh]);
		ss << std::fixed << std::setprecision(precision1) << ffs[hh]->GetParameter(2);
		tau1.push_back(ss.str());
		ss.str("");
		ss << std::fixed << std::setprecision(precision2) << (ffs[hh]->GetParameter(4)>1000 ? 0 : ffs[hh]->GetParameter(4));
		tau2.push_back(ss.str());
		ss.str("");
		//ffs[hh]->Draw("same");
    }
	//ffs[0]->Draw("same");
	//ffs[1]->Draw("same");
	//ffs[2]->Draw("same");
	//ffs[3]->Draw("same");
	//ffs[4]->Draw("same");
	//ffs[5]->Draw("same");
	//ffs[6]->Draw("same");
	if (!linear && do_fit) { 
		double ypos0 = 0.02;	
		double ypos1 = 0.008;
		double offset = 0.08/0.05;
		auto *txtfr0 = new TLatex (80, ypos1*std::pow(offset, frs.size()), "Slow fraction:");
		txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
		txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (50, ypos0*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (105, ypos1*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau=")+tau2[hh]).c_str());
			txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
			txt11->SetTextColor(palette_major[hh]); txt11->Draw();

			auto *txtfr1 = new TLatex (80, ypos1*std::pow(offset, hh_end_ - hh - 1), (frs[hh]).c_str());
			txtfr1->SetTextAlign(12); txtfr1->SetTextSize(0.05);
			txtfr1->SetTextColor(palette_major[hh]); txtfr1->Draw();
		}
	}
	if (linear && do_fit) { 
		double ypos0 = 0.17;	
		double ypos1 = 0.17;
		double offset = 0.035;
		auto *txtfr0 = new TLatex (36, ypos1+offset*frs.size(), "Slow fraction:");
		txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
		txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (34.3, ypos0+offset*(hh_end_ - hh - 1), (std::string("#tau=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (115, ypos1+offset*(hh_end_ - hh - 1), (std::string("#tau=")+tau2[hh]).c_str());
			txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
			txt11->SetTextColor(palette_major[hh]); txt11->Draw();

			auto *txtfr1 = new TLatex (36, ypos1+offset*(hh_end_ - hh - 1), (frs[hh]).c_str());
			txtfr1->SetTextAlign(12); txtfr1->SetTextSize(0.05);
			txtfr1->SetTextColor(palette_major[hh]); txtfr1->Draw();
		}
	}
	
	legend->AddEntry(hist_1, (std::string(Tds[0] + " Td fPMTs (no WLS) trigger sh. ampl.")).c_str(), "l");
	legend->AddEntry(hist_2, (std::string(Tds[1] + " Td fPMTs (no WLS) trigger sh. ampl.")).c_str(), "l");
	legend->AddEntry(hist_3, (std::string(Tds[2] + " Td fPMTs (with WLS) trigger 4 slowPMTs")).c_str(), "l");
	legend->AddEntry(hist_4, (std::string(Tds[3] + " Td fPMTs (with WLS) trigger 4 slowPMTs")).c_str(), "l");
	legend->AddEntry(hist_5, (std::string(Tds[4] + " Td SiPM matrix trigger 4 slowPMTs")).c_str(), "l");
	legend->AddEntry(hist_6, (std::string(Tds[5] + " Td SiPM matrix trigger 4 slowPMTs")).c_str(), "l");
	legend->AddEntry(hist_7, (std::string(Tds[6] + " Td SiPM matrix trigger sh. ampl.")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
