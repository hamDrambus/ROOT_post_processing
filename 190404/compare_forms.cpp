
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
  return x[0] <= par[0] ? 0 : par[1]*std::exp((par[0]-x[0])/par[2]) + par[3]*std::exp((par[0]-x[0])/par[4]);
}

int compare_forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    int Nbins = 500;
    double time_left = 0, time_right = 160;//us
    double time_pretrigger_left = 0, time_pretrigger_right = 15;
	double norm_t_left = 24, norm_t_right = 30.6;
	//norm_t_right = 31.5; //18kV
	//norm_t_right = 31.5; //16kV
	//norm_t_right = 32.0; //14kV
	//norm_t_right = 32.5; //12kV
	//norm_t_right = 33.0; //10kV
	//norm_t_right = 33.5; //8kV
	double fit_from = 30.6;
	norm_t_right = 30.0; //TEMP!!!
    TF1 *unity_f = new TF1("f1", "1", time_left, time_right);
    TH1D* hist_1 = new TH1D ("hist1", "hist1", Nbins, time_left, time_right);
    TH1D* hist_2 = new TH1D ("hist2", "hist2", Nbins, time_left, time_right);
    TH1D* hist_3 = new TH1D ("hist3", "hist3", Nbins, time_left, time_right);
    TH1D* hist_4 = new TH1D ("hist4", "hist4", Nbins, time_left, time_right);
    TH1D* hist_5 = new TH1D ("hist5", "hist5", Nbins, time_left, time_right);
    TH1D* hist_6 = new TH1D ("hist6", "hist6", Nbins, time_left, time_right);
    std::vector<TH1D*> hists;
    hists.push_back(hist_1);
    hists.push_back(hist_2);
    //hists.push_back(hist_3);
    //hists.push_back(hist_4);
    //hists.push_back(hist_5);
    //hists.push_back(hist_6);
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2};
    double max_val = 0;
    std::string Td = "8.2";
    std::string prefix = "190404/results/Cd_46V_20kV_850V/";
	read_hist_w (hist_1, prefix + "forms_x_5-20pe/8_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "forms_x_5-20pe/9_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "forms_x_5-20pe/10_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "forms_x_5-20pe/11_form_by_Npeaks.hdata");

	read_hist_w (hist_2, prefix + "forms_x_5-20pe/SiPMs_form_by_Npe.hdata");

	std::string framename = std::string("Signal forms ") + Td + " Td 5-20 pe eqv. 3.3 Td (8kV)";

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
		    if (hists[hh]->GetBinCenter(bin)>norm_t_left && hists[hh]->GetBinCenter(bin)<norm_t_right) {
	           integral += hists[hh]->GetBinContent(bin) * hists[hh]->GetBinWidth(bin);
		    }
        }
		hists[hh]->Scale(1.0/integral);
    }
    std::vector<double> fractions;
	std::vector<std::string> frs;
	std::stringstream ss;
	int fr_precision = 2;
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double fast_integral = 0;
		double full_integral = 0;        
		for (int bin = 1, bin_end = hists[hh]->GetNbinsX()+1; bin!=bin_end; ++bin) {
			if (hists[hh]->GetBinContent(bin)<0.0)
				hists[hh]->SetBinContent(bin, 0.0);
		    if (hists[hh]->GetBinCenter(bin)>norm_t_left && hists[hh]->GetBinCenter(bin)<norm_t_right) {
	           fast_integral += hists[hh]->GetBinContent(bin) * hists[hh]->GetBinWidth(bin);
		    }
			if (hists[hh]->GetBinCenter(bin)>norm_t_left) {
	           full_integral += hists[hh]->GetBinContent(bin) * hists[hh]->GetBinWidth(bin);
		    }
        }
		fractions.push_back((full_integral-fast_integral)/full_integral);
		ss << std::fixed<<std::setprecision(fr_precision) << fractions.back();
		frs.push_back(ss.str());
		ss.str("");
    }
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh)
        max_val = std::max(max_val, hists[hh]->GetBinContent(hists[hh]->GetMaximumBin()));
    max_val*=2;
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
	c_->SetGrid();
	c_->SetTicks();
	c_->SetLogy();//Log Y
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, time_left, time_right, 500, 0, max_val);
	frame->GetXaxis()->SetTitle("t [#muV]");
	frame->GetYaxis()->SetTitle("");
	frame->Draw();
	
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
		hists[hh]->SetLineWidth(2);
		hists[hh]->SetLineColor(palette_major[hh]);
		hists[hh]->Draw("hist csame");
    }
	
	std::vector<std::string> tau1, tau2;
	TF1 *ff1 = new TF1("fit1", FittingF_2exp, fit_from, time_right, 5);
	ff1->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
	ff1->FixParameter(0, fit_from);
	ff1->SetParLimits(1, 2e-3, 1);
	ff1->SetParLimits(2, 1, 10);
	ff1->SetParLimits(3, 0, 0);
	ff1->SetParLimits(4, 1000, 1000);	
	//ff1->SetParLimits(3, 3e-4, 1e-1);
	//ff1->SetParLimits(4, 20, 1000);
	TF1 *ff2 = new TF1("fit1", FittingF_2exp, fit_from, time_right, 5);
	ff2->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
	ff2->FixParameter(0, fit_from);
	ff2->SetParLimits(1, 2e-3, 1);
	ff2->SetParLimits(2, 3, 10);
	ff2->SetParLimits(3, 0, 0);
	ff2->SetParLimits(4, 1000, 1000);	
	//ff2->SetParLimits(3, 1e-5, 1e-1);
	//ff2->SetParLimits(4, 20, 1000);
	int precision1 = 2, precision2 = 0;
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
    	hists[hh]->Fit(ff1);
		ss << std::fixed << std::setprecision(precision1) << ff1->GetParameter(2);
		tau1.push_back(ss.str());
		ss.str("");
		ss << std::fixed << std::setprecision(precision2) << ff1->GetParameter(4);
		tau2.push_back(ss.str());
		ss.str("");
    }
	ff1->SetLineColor(palette_minor[0]);
	ff2->SetLineColor(palette_minor[1]);
	hist_1->Fit(ff1);
	hist_2->Fit(ff2);	
	ff1->Draw("same");
	ff2->Draw("same");
	double ypos = 0.02;
	auto *txtfr0 = new TLatex (80, ypos*std::pow(0.07/0.05, frs.size()), "Slow fraction:");
	txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
    txtfr0->SetTextColor(kBlack); txtfr0->Draw();
	for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
		auto *txt1 = new TLatex (45, 0.1*std::pow(0.07/0.05, hh_end_ - hh - 1), (std::string("#tau=")+tau1[hh]).c_str());
		txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
		txt1->SetTextColor(palette_major[hh]); txt1->Draw();

		auto *txt11 = new TLatex (120, ypos*std::pow(0.07/0.05, hh_end_ - hh - 1), (std::string("#tau=")+tau2[hh]).c_str());
		txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
		txt11->SetTextColor(palette_major[hh]); txt11->Draw();

		auto *txtfr1 = new TLatex (80, ypos*std::pow(0.07/0.05, hh_end_ - hh - 1), (frs[hh]).c_str());
		txtfr1->SetTextAlign(12); txtfr1->SetTextSize(0.05);
		txtfr1->SetTextColor(palette_major[hh]); txtfr1->Draw();
    }
	
	legend->AddEntry(hist_1, (std::string(Td + " Td fPMTs 5-20pe")).c_str(), "l");
	legend->AddEntry(hist_2, (std::string(Td + " Td SiPMs 5-20pe")).c_str(), "l");
	//legend->AddEntry(hist_3, (std::string(Td + " Td SiPMs Cd peak (5-20pe)")).c_str(), "l");
	//legend->AddEntry(hist_4, (std::string(Td + " Td SiPMs large Npes (25-70pe)")).c_str(), "l");
	//legend->AddEntry(hist_5, (std::string(Td + " Td slow PMT by counting peaks")).c_str(), "l");
	//legend->AddEntry(hist_6, (std::string(Td + " Td slow PMT weighting peaks")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
