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

int PMT_compare_forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    int Nbins = 500;
    double time_left = 0, time_right = 160;//us
    double time_pretrigger_left = 0, time_pretrigger_right = 20;
	double norm_t_left = 25.5, norm_t_right = 32.5;
	double fit_from = 32.5;
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
    hists.push_back(hist_3);
    hists.push_back(hist_4);
    //hists.push_back(hist_5);
    //hists.push_back(hist_6);
    double max_val = 0;
    std::string Td = "4.1";
    std::string prefix = "190404/results/Cd_46V_10kV_850V/forms_aggr=1_24-70pe/";
    read_hist_w (hist_1, prefix + "8_form_by_Npeaks.hdata");
    read_hist_w (hist_2, prefix + "9_form_by_Npeaks.hdata");
    read_hist_w (hist_3, prefix + "10_form_by_Npeaks.hdata");
    read_hist_w (hist_4, prefix + "11_form_by_Npeaks.hdata");

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
    //hist_SiPM_S->Scale(max_val / hist_SiPM_S->GetBinContent(hist_SiPM_S->GetMaximumBin()));

    max_val = std::max(max_val, hist_1->GetBinContent(hist_1->GetMaximumBin()));
    max_val = std::max(max_val, hist_2->GetBinContent(hist_2->GetMaximumBin()));
    max_val = std::max(max_val, hist_3->GetBinContent(hist_3->GetMaximumBin()));
    max_val = std::max(max_val, hist_4->GetBinContent(hist_4->GetMaximumBin()));
    //max_val = std::max(max_val, hist_5->GetBinContent(hist_5->GetMaximumBin()));
    //max_val = std::max(max_val, hist_6->GetBinContent(hist_6->GetMaximumBin()));
    max_val*=2;
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_ = new TCanvas ((std::string("signal forms with PMT cuts 24-70pe ") + Td + " Td").c_str(), (std::string("signal forms with PMT cuts 24-70pe ") + Td + " Td").c_str(), DEF_W, DEF_H);
	c_->SetGrid();
	c_->SetTicks();
	c_->SetLogy();//Log Y
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F( "frame", (std::string("Signal forms with PMT cuts 24-70pe ") + Td + " Td").c_str(), 500, time_left, time_right, 500, 0, max_val);
	frame->GetXaxis()->SetTitle("t [#muV]");
	frame->GetYaxis()->SetTitle("");
	frame->Draw();
	
	hist_1->SetLineWidth(2);
	hist_1->SetLineColor(kBlack);
	hist_1->Draw("hist csame");
	hist_2->SetLineWidth(2);
	hist_2->SetLineColor(kRed);//(kRed);
	hist_2->Draw("hist csame");
	hist_3->SetLineWidth(2);
	hist_3->SetLineColor(kBlue);
	hist_3->Draw("hist csame");
	hist_4->SetLineWidth(2);
	hist_4->SetLineColor(kGreen);//(kBlack);
	hist_4->Draw("hist csame");
	//hist_5->SetLineWidth(2);
	//hist_5->SetLineColor(kYellow + 2);
	//hist_5->Draw("hist csame");
    //hist_6->SetLineWidth(2);
	//hist_6->SetLineColor(kMagenta);
	//hist_6->Draw("hist csame");
	
	std::vector<std::string> tau1, tau2;
	std::stringstream ss;
	TF1 *ff1 = new TF1("fit1", FittingF_2exp, fit_from, time_right, 5);
	ff1->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
	ff1->FixParameter(0, fit_from);
	ff1->SetParLimits(1, 2e-3, 1);
	ff1->SetParLimits(2, 1, 30);
	ff1->SetParLimits(3, 0, 0);
	ff1->SetParLimits(4, 1000, 1000);
	ff1->SetLineColor(kGreen - 2);//(kBlack);
	hist_1->Fit(ff1);
	int precision1 = 2, precision2 = 0;
	ss << std::fixed << std::setprecision(precision1) << ff1->GetParameter(2);
	tau1.push_back(ss.str());
	ss.str("");
	ss << std::fixed << std::setprecision(precision2) << ff1->GetParameter(4);
	tau2.push_back(ss.str());
	ss.str("");
	hist_2->Fit(ff1);
	ss << std::fixed << std::setprecision(precision1) << ff1->GetParameter(2);
	tau1.push_back(ss.str());
	ss.str("");
	ss << std::fixed << std::setprecision(precision2) << ff1->GetParameter(4);
	tau2.push_back(ss.str());
	ss.str("");
	hist_3->Fit(ff1);
	ss << std::fixed << std::setprecision(precision1) << ff1->GetParameter(2);
	tau1.push_back(ss.str());
	ss.str("");
	ss << std::fixed << std::setprecision(precision2) << ff1->GetParameter(4);
	tau2.push_back(ss.str());
	ss.str("");
	hist_4->Fit(ff1);
	ss << std::fixed << std::setprecision(precision1) << ff1->GetParameter(2);
	tau1.push_back(ss.str());
	ss.str("");
	ss << std::fixed << std::setprecision(precision2) << ff1->GetParameter(4);
	tau2.push_back(ss.str());
	ss.str("");
	ff1->Draw("same");
	auto *txt1 = new TLatex (45, 0.05*std::pow(0.07/0.05, 3), (std::string("#tau=")+tau1[0]).c_str());
	txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
    txt1->SetTextColor(kBlack); txt1->Draw();
	auto *txt2 = new TLatex (45, 0.05*std::pow(0.07/0.05, 2), (std::string("#tau=")+tau1[1]).c_str());
	txt2->SetTextAlign(12); txt2->SetTextSize(0.05);
    txt2->SetTextColor(kRed); txt2->Draw();
	auto *txt3 = new TLatex (45, 0.05*std::pow(0.07/0.05, 1), (std::string("#tau=")+tau1[2]).c_str());
	txt3->SetTextAlign(12); txt3->SetTextSize(0.05);
    txt3->SetTextColor(kBlue); txt3->Draw();
	auto *txt4 = new TLatex (45, 0.05*std::pow(0.07/0.05, 0), (std::string("#tau=")+tau1[3]).c_str());
	txt4->SetTextAlign(12); txt4->SetTextSize(0.05);
    txt4->SetTextColor(kGreen); txt4->Draw();
	auto *txt11 = new TLatex (120, 0.025*std::pow(0.07/0.05, 3), (std::string("#tau=")+tau2[0]).c_str());
	txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
    txt11->SetTextColor(kBlack); txt11->Draw();
	auto *txt21 = new TLatex (120, 0.025*std::pow(0.07/0.05, 2), (std::string("#tau=")+tau2[1]).c_str());
	txt21->SetTextAlign(12); txt21->SetTextSize(0.05);
    txt21->SetTextColor(kRed); txt21->Draw();
	auto *txt31 = new TLatex (120, 0.025*std::pow(0.07/0.05, 1), (std::string("#tau=")+tau2[2]).c_str());
	txt31->SetTextAlign(12); txt31->SetTextSize(0.05);
    txt31->SetTextColor(kBlue); txt31->Draw();
	auto *txt41 = new TLatex (120, 0.025*std::pow(0.07/0.05, 0), (std::string("#tau=")+tau2[3]).c_str());
	txt41->SetTextAlign(12); txt41->SetTextSize(0.05);
    txt41->SetTextColor(kGreen); txt41->Draw();
	
	legend->AddEntry(hist_1, (std::string(Td + " Td fast PMT#1 by counting Npeaks")).c_str(), "l");
	legend->AddEntry(hist_2, (std::string(Td + " Td fast PMT#2 by counting Npeaks")).c_str(), "l");
	legend->AddEntry(hist_3, (std::string(Td + " Td fast PMT#3 by counting Npeaks")).c_str(), "l");
	legend->AddEntry(hist_4, (std::string(Td + " Td fast PMT#4 by counting Npeaks")).c_str(), "l");
	//legend->AddEntry(hist_5, (std::string(Td + " Td slow PMT by counting peaks")).c_str(), "l");
	//legend->AddEntry(hist_6, (std::string(Td + " Td slow PMT weighting peaks")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
