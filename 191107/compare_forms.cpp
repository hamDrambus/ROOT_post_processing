std::string dbl_to_str (double val, int precision)
{
	std::stringstream ss;
	ss<<std::fixed<<std::setprecision(precision)<<val;
	return ss.str();
}

double integrate(TH1D *hist, double from, double to) {
	double Int = 0;
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
           Int += hist->GetBinContent(bin) * hist->GetBinWidth(bin);
	    }
    }
	return Int;
}

double counts(TH1D *hist, double from, double to) {
	double count = 0;
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
           count += hist->GetBinContent(bin);
	    }
    }
	return count;
}

double average(TH1D *hist, double from, double to) {
	double baseline = 0;
    int counts = 0;
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
           baseline += hist->GetBinContent(bin);
           ++counts;
	    }
    }
	return baseline/counts;
}

void subtract_baseline(TH1D *hist, double baseline) {
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
		hist->SetBinContent(bin, std::max(hist->GetBinContent(bin) - baseline, 0.0));		
    }
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
  return (x[0] < par[0]) ? 0 : (par[1]*std::exp((par[0]-x[0])/par[2]) + par[3]*std::exp((par[0]-x[0])/par[4]));
}

void add_text(double x, double y, std::string title, std::vector<std::string> &entries, std::vector<Color_t>& colors) {
	TPad *pad = (TPad*)gPad;
	double offset;
	if (pad->GetLogy()) {
		TH2F* frame = (TH2F*)gDirectory->FindObject("frame");
		if (NULL==frame)
			return;		
		offset = std::log(frame->GetYaxis()->GetXmax()/frame->GetYaxis()->GetXmin())/5.75;
		std::cout<<"Size="<<entries.size()<<", offset = "<<offset<<std::endl;
		if (!title.empty()) {
			auto *txtfr0 = new TLatex (x, y*std::pow(offset, entries.size()), title.c_str());
			txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
			txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		}
		for (int hh = 0, hh_end_ = entries.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (x, y*std::pow(offset, hh_end_ - hh - 1), entries[hh].c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(colors[hh]); txt1->Draw();
		}
	} else {
		TH2F* frame = (TH2F*)gDirectory->FindObject("frame");
		if (NULL==frame)
			return;
		offset = (frame->GetYaxis()->GetXmax() - frame->GetYaxis()->GetXmin())/18;
		if (!title.empty()) {
			auto *txtfr0 = new TLatex (x, y+offset*entries.size(), title.c_str());
			txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
			txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		}		
		for (int hh = 0, hh_end_ = entries.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (x, y+offset*(hh_end_ - hh - 1), entries[hh].c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(colors[hh]); txt1->Draw();
		}
	}
}

int compare_forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    int Nbins = 1800;
    double time_left = 0, time_right = 160;//us
	double time_S1_l = 60, time_S1_r = 62.5;//us
    double time_pretrigger_left = 5, time_pretrigger_right = 55;
	double norm_t_left = 88;
    TF1 *unity_f = new TF1("f1", "1", time_left, time_right);
    TH1D* hist_1 = new TH1D ("hist1", "hist1", Nbins, time_left, time_right);
    TH1D* hist_2 = new TH1D ("hist2", "hist2", Nbins, time_left, time_right);
    TH1D* hist_3 = new TH1D ("hist3", "hist3", Nbins, time_left, time_right);
    TH1D* hist_4 = new TH1D ("hist4", "hist4", Nbins, time_left, time_right);
    TH1D* hist_5 = new TH1D ("hist5", "hist5", Nbins, time_left, time_right);
    TH1D* hist_6 = new TH1D ("hist6", "hist6", Nbins, time_left, time_right);
	TH1D* hist_7 = new TH1D ("hist6", "hist6", Nbins, time_left, time_right);
    std::vector<TH1D*> hists;
    hists.push_back(hist_1);
    //hists.push_back(hist_2);
    //hists.push_back(hist_3);
    //hists.push_back(hist_4);
    //hists.push_back(hist_5);
    //hists.push_back(hist_6);
	//hists.push_back(hist_7);
    std::vector<std::string> Tds =      {"3.4","7.6","6.8","5.9","5.1","4.2","3.4"};//20, 18, ..., 8 kV
	std::vector<double> norm_t_right =  {91.7, 93.8, 94.1, 94.4, 94.9, 95.4, 95.4}; //20, 18, ..., 8 kV
	std::vector<double> fit_from = 	    {96.5, 95.0, 95.5, 95.3, 95.3, 95.4, 95.4}; //20, 18, ..., 8 kV
	std::vector<double> fit_to =        {156.5,  155,  155,  155,  53.0, 52.0, 52.0}; //PMTs 20, 18, ..., 8 kV
	//std::vector<double> fit_from = 	    {32.4, 32.1, 32.1, 32.8, 32.9, 33.0, 33.0}; //SiPMs
	//std::vector<double> fit_to =        {160,  160,  160,  48.7,  55.0, 52.0, 52.0}; //SiPMs 20, 18, ..., 8 kV
	//std::vector<double> fit_to =        {160,  160,  160,  50,  50.0, 52.0, 52.0}; //edge SiPMs 20, 18, ..., 8 kV
	//std::vector<std::string> Tds(6, "3.4");
	//std::vector<double> norm_t_right (6, 33.5);
	//std::vector<double> fit_from (6, 33.5);
	//std::vector<double> fit_to = {45, 45, 45, 45};
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
    double max_val = 0;
	bool linear = false;
	std::size_t Nevetnts = 21050;
    std::string prefix = "191107/results_v1/Alpha_46V_8kV_850V/forms_Cd_peak/";
	read_hist_w (hist_1, prefix + "1_form_by_Npe.hdata");
	read_hist_w (hist_1, prefix + "2_form_by_Npe.hdata");
	read_hist_w (hist_1, prefix + "3_form_by_Npe.hdata");
	read_hist_w (hist_1, prefix + "4_form_by_Npe.hdata");
	std::string framename = std::string("191107 v1 (no WLS) Signal forms sPMTs #alpha peak");// + " " + Tds[0] + " Td";
	std::vector<std::string> S1s(hists.size(), ""), S2s(hists.size(), "");
    for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        hists[hh]->Sumw2();
        double baseline = average(hists[hh], time_pretrigger_left, time_pretrigger_right);
		subtract_baseline(hists[hh], baseline);
        double integral = counts(hists[hh], norm_t_left, norm_t_right[hh]);
		S2s[hh] = dbl_to_str(integral/Nevetnts, 1);
		//hists[hh]->Scale(1.0/integral);
		double S1 = counts(hists[hh], time_S1_l, time_S1_r);
		S1s[hh] = dbl_to_str(S1/Nevetnts, 2);
    }
    std::vector<double> fractionsS(hists.size(), 0), fractionsL(hists.size(), 0), fractionsLost(hists.size(), 0);
	std::vector<std::string> frsS(hists.size(), ""), frsL(hists.size(), ""), frsLost(hists.size(), "");
	int fr_precision = 2;
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double fast_integral = integrate(hists[hh], norm_t_left, norm_t_right[hh]);
		double full_integral = integrate(hists[hh], norm_t_left, DBL_MAX);
		fractionsS[hh] = full_integral-fast_integral;
		fractionsL[hh] = full_integral;
    }
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh)
        max_val = std::max(max_val, hists[hh]->GetBinContent(hists[hh]->GetMaximumBin()));
    max_val*= linear ? 1.2 : 2;
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
	c_->SetGrid(); c_->SetTicks(); c_->ToggleEventStatus(); c_->ToggleToolBar();
	if (!linear)
		c_->SetLogy();
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : 1, max_val);
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
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
		ffs[hh] = new TF1("fit1", FittingF_2exp, fit_from[hh], fit_to[hh], 5);
		ffs[hh]->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
		ffs[hh]->FixParameter(0, fit_from[hh]);
		ffs[hh]->SetParLimits(1, 1e1, 5e5);
		ffs[hh]->SetParLimits(2, 1, 10);
		if (fit_to[hh]<(time_right-10)) {
			ffs[hh]->SetParLimits(3, 10, 1e4);
			ffs[hh]->FixParameter(4, 1e6);	
		} else {
			ffs[hh]->SetParLimits(3, 200, 1e4);
			ffs[hh]->SetParLimits(4, 50, 1000);
		}
		ffs[hh]->SetLineColor(palette_minor[hh]);
    	hists[hh]->Fit(ffs[hh], "W");
		tau1.push_back("#tau_{1}=" + dbl_to_str(ffs[hh]->GetParameter(2), precision1));
		tau2.push_back("#tau_{2}=" + dbl_to_str((ffs[hh]->GetParameter(4)>1000 ? 0 : ffs[hh]->GetParameter(4)), precision2));
		double long_component_extra;
		double long_component_full;
		if (fit_to[hh]<(time_right-10)) {
			long_component_full = ffs[hh]->GetParameter(3)*(time_right - fit_from[hh]);		
			long_component_extra = -long_component_full;
		} else {
			long_component_extra = ffs[hh]->GetParameter(4)*ffs[hh]->GetParameter(3)*
				std::exp((ffs[hh]->GetParameter(0) - time_right)/ffs[hh]->GetParameter(4));
			long_component_full = ffs[hh]->GetParameter(4)*ffs[hh]->GetParameter(3)*
				std::exp((ffs[hh]->GetParameter(0) - fit_from[hh])/ffs[hh]->GetParameter(4));
		}
		std::cout<<"#"<<hh<<":"<<Tds[hh]<<" Td."<<std::endl;
		std::cout<<"Full integral: "<<fractionsL[hh]<<std::endl;
		std::cout<<"Long extra integral: "<<long_component_extra<<std::endl;
		std::cout<<"Slow+Long integral: "<<fractionsS[hh]<<std::endl;
		std::cout<<"Long total integral: "<<long_component_full<<std::endl;

		double integral = fractionsL[hh] + long_component_extra;
		fractionsS[hh] = (fractionsS[hh] + long_component_extra - long_component_full)/integral;
		fractionsL[hh] = long_component_full/integral;
		fractionsLost[hh] = long_component_extra/integral;
		frsS[hh] = dbl_to_str(fractionsS[hh], fr_precision);
		frsL[hh] = dbl_to_str(fractionsL[hh], fr_precision);
		frsLost[hh] = dbl_to_str(fractionsLost[hh], fr_precision);
		ffs[hh]->Draw("same");
		std::cout<<Tds[hh]<<" Td. Lost signal fraction:"<<frsLost[hh]<<std::endl;
    }
	if (!linear) {
		add_text(60, 4000, "<S1>[Npe]:", S1s, palette_major);
		add_text(92, 15000, "<S2>[Npe]:", S2s, palette_major);
		add_text(92, 6000, "", tau1, palette_major);
		add_text(106, 3500, "Slow fractions:", frsS, palette_major);
		add_text(117, 3500, "", frsL, palette_major);
		add_text(127, 3500, "", tau2, palette_major);
	} else {
		add_text(60, 5000, "<S1>[Npe]:", S1s, palette_major);
		add_text(97, 25000, "<S2>[Npe]:", S2s, palette_major);
		add_text(97, 8000, "", tau1, palette_major);
		add_text(108, 8000, "Slow fractions:", frsS, palette_major);
		add_text(115, 8000, "", frsL, palette_major);
		add_text(130, 8000, "", tau2, palette_major);
	}
	
	legend->AddEntry(hist_1, (std::string(Tds[0] + " Td sPMTs #alpha peak")).c_str(), "l");
	//legend->AddEntry(hist_2, (std::string(Tds[1] + " Td fPMTs #alpha peak")).c_str(), "l");
	//legend->AddEntry(hist_3, (std::string(Tds[2] + " Td fPMTs #alpha peak")).c_str(), "l");
	//legend->AddEntry(hist_4, (std::string(Tds[3] + " Td fPMTs #alpha peak")).c_str(), "l");
	//legend->AddEntry(hist_5, (std::string(Tds[4] + " Td fPMTs #alpha peak")).c_str(), "l");
	//legend->AddEntry(hist_6, (std::string(Tds[5] + " Td fPMTs #alpha peak")).c_str(), "l");
	//legend->AddEntry(hist_7, (std::string(Tds[6] + " Td fPMTs #alpha peak")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
