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
	double fit_time_maximum = 140;
    double time_pretrigger_left = 4, time_pretrigger_right = 22;
	double norm_t_left = 24;
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
    hists.push_back(hist_2);
    hists.push_back(hist_3);
    hists.push_back(hist_4);
    hists.push_back(hist_5);
    hists.push_back(hist_6);
	hists.push_back(hist_7);
	//kVs:								  20,   18,   16,   14,   12,   10,   8 kV
    std::vector<std::string> Tds =      {"8.5","7.6","6.8","5.9","5.1","4.2","3.4"};
	std::vector<double> norm_t_right =  {32.8, 32.8, 33.2, 33.5, 33.9, 34.7, 36.2}; //20, 18, ..., 8 kV
	std::vector<double> fit_from = 	  	{33.5, 33.6, 33.9, 34.2, 35.0, 35.0, 35.4}; //fPMT#1
	std::vector<double> fit_to =        {53,  52.5,  52,  51.5,  50,   48,   42}; //fPMT#1
	//std::vector<double> fit_from = 	  	{34.8, 34.8, 34.6, 34.6, 35.3, 35.6, 36.2}; //fPMTs#2-4
	//std::vector<double> fit_to =        {160,  160,  160,  160,  160,  55,   50}; //fPMTs#2-4
	//std::vector<double> fit_from = 	  	{33.4, 33.5, 33.8, 34.2, 35.3, 35.3, 36.2}; //SiPMs
	//std::vector<double> fit_to =        {155,  155,  155,  155,  50,   46,   45}; //SiPMs 
	//kVs:								   13,   11,   9kV
    //std::vector<std::string> Tds =      {"5.5","4.7","3.8"};
	//std::vector<double> norm_t_right =  {33.8, 34.5, 35.1}; //13,   11,   9 kV
	//std::vector<double> fit_from = 	  	{34.7, 35.0, 35.1}; //fPMT#1
	//std::vector<double> fit_to =        {51, 48, 44}; //fPMT#1
	//std::vector<double> fit_from = 	  	{35.2, 35.3, 35.7}; //fPMTs#2-4
	//std::vector<double> fit_to =        {60, 56, 54}; //fPMTs#2-4
	//std::vector<double> fit_from = 	  	{34.4, 35.0, 35.3}; //SiPMs
	//std::vector<double> fit_to =        {50, 48.5, 45}; //SiPMs
	
	//std::vector<std::string> Tds(7, "6.8");
	//std::vector<double> norm_t_right {31.5, 31.5, 31.5, 31.0, 31.0, 31.0, 34.7};
	//std::vector<double> fit_from = {31.5, 31.5, 31.5, 31.5, 31.5, 31.5, 35};
	//std::vector<double> fit_to = {160, 160, 160, 160, 160, 160, 160};
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
    double max_val = 0;
	bool linear = true;
    std::string prefix = "180705/results_v4/Cd_48V_20kV_800V/forms_Cd_peak/";
	read_hist_w (hist_1, prefix + "8_form_by_S.hdata");
prefix = "180705/results_v4/Cd_48V_18kV_800V/forms_Cd_peak/";
	read_hist_w (hist_2, prefix + "8_form_by_S.hdata");
prefix = "180705/results_v4/Cd_48V_18kV_800V/forms_Cd_peak/";
	read_hist_w (hist_2, prefix + "8_form_by_S.hdata");
prefix = "180705/results_v4/Cd_48V_16kV_800V/forms_Cd_peak/";
	read_hist_w (hist_3, prefix + "8_form_by_S.hdata");
prefix = "180705/results_v4/Cd_48V_14kV_800V/forms_Cd_peak/";
	read_hist_w (hist_4, prefix + "8_form_by_S.hdata");
prefix = "180705/results_v4/Cd_48V_12kV_800V/forms_Cd_peak/";
	read_hist_w (hist_5, prefix + "8_form_by_S.hdata");
prefix = "180705/results_v4/Cd_48V_10kV_800V/forms_Cd_peak/";
	read_hist_w (hist_6, prefix + "8_form_by_S.hdata");
prefix = "180705/results_v4/Cd_48V_8kV_800V/forms_Cd_peak/";
	read_hist_w (hist_7, prefix + "8_form_by_S.hdata");
	std::string framename = std::string("180705 Signal forms v4 fast PMT#1 (no WLS) Cd peak");// + " " + Tds[0] + " Td";

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
    std::vector<double> fractionsS(hists.size(), 0), fractionsL(hists.size(), 0), fractionsLost(hists.size(), 0);
	std::vector<std::string> frsS(hists.size(), ""), frsL(hists.size(), ""), frsLost(hists.size(), "");
	std::stringstream ss;
	int fr_precision = 2;
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double fast_integral = 0;
		double full_integral = 0;        
		for (int bin = 1, bin_end = hists[hh]->GetNbinsX()+1; bin!=bin_end; ++bin) {
			if (hists[hh]->GetBinContent(bin)<0.0)
				hists[hh]->SetBinContent(bin, 0.0);
		    if (hists[hh]->GetBinCenter(bin)>norm_t_left && hists[hh]->GetBinCenter(bin)<norm_t_right[hh]) {
	           fast_integral += hists[hh]->GetBinContent(bin) * hists[hh]->GetBinWidth(bin);
		    }
			if (hists[hh]->GetBinCenter(bin)>norm_t_left) {
	           full_integral += hists[hh]->GetBinContent(bin) * hists[hh]->GetBinWidth(bin);
		    }
        }
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
	c_->SetGrid();
	c_->SetTicks();
	c_->ToggleEventStatus();
    c_->ToggleToolBar();
	if (!linear)
		c_->SetLogy();
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : 1e-5, std::min(max_val, 1.0));
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
		ffs[hh]->SetParLimits(1, 1e-3, 2);
		ffs[hh]->SetParLimits(2, 1, 8);
		if (fit_to[hh]<fit_time_maximum) {
			ffs[hh]->SetParLimits(3, 1e-5, 2e-4);
			ffs[hh]->FixParameter(4, 1e6);	
		} else {
			ffs[hh]->SetParLimits(3, 5e-5, 1e-2);
			ffs[hh]->SetParLimits(4, 25, 1000);
		}
		ffs[hh]->SetLineColor(palette_minor[hh]);
    	hists[hh]->Fit(ffs[hh]);
		tau1.push_back(dbl_to_str(ffs[hh]->GetParameter(2), precision1));
		tau2.push_back(dbl_to_str((ffs[hh]->GetParameter(4)>1000 ? 0 : ffs[hh]->GetParameter(4)), precision2));
		double long_component_extra;
		double long_component_full;
		if (fit_to[hh]<time_right) {
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
		double ypos0 = 0.007;	
		double ypos1 = 0.001;
		double offset = 0.1/0.05;
		auto *txtfr0 = new TLatex (80, ypos1*std::pow(offset, frsS.size()), "Slow fraction:");
		txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
		txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (56, ypos0*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (111, ypos1*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau=")+tau2[hh]).c_str());
			txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
			txt11->SetTextColor(palette_major[hh]); txt11->Draw();

			auto *txtfr1 = new TLatex (80, ypos1*std::pow(offset, hh_end_ - hh - 1), (frsS[hh]).c_str());
			txtfr1->SetTextAlign(12); txtfr1->SetTextSize(0.05);
			txtfr1->SetTextColor(palette_major[hh]); txtfr1->Draw();
			auto *txtfr2 = new TLatex (96, ypos1*std::pow(offset, hh_end_ - hh - 1), (frsL[hh]).c_str());
			txtfr2->SetTextAlign(12); txtfr2->SetTextSize(0.05);
			txtfr2->SetTextColor(palette_major[hh]); txtfr2->Draw();
		}
	} else {
		double ypos0 = 0.266;	
		double ypos1 = 0.21;
		double offset = 0.040;
		auto *txtfr0 = new TLatex (36, ypos1+offset*frsS.size(), "Slow fraction:");
		txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
		txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (34.3, ypos0+offset*(hh_end_ - hh - 1), (std::string("#tau=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (115, ypos1+offset*(hh_end_ - hh - 1), (std::string("#tau=")+tau2[hh]).c_str());
			txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
			txt11->SetTextColor(palette_major[hh]); txt11->Draw();

			auto *txtfr1 = new TLatex (36, ypos1+offset*(hh_end_ - hh - 1), (frsS[hh]).c_str());
			txtfr1->SetTextAlign(12); txtfr1->SetTextSize(0.05);
			txtfr1->SetTextColor(palette_major[hh]); txtfr1->Draw();
			auto *txtfr2 = new TLatex (37, ypos1+offset*(hh_end_ - hh - 1), (frsL[hh]).c_str());
			txtfr2->SetTextAlign(12); txtfr2->SetTextSize(0.05);
			txtfr2->SetTextColor(palette_major[hh]); txtfr2->Draw();
		}
	}
	
	legend->AddEntry(hist_1, (std::string(Tds[0] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	legend->AddEntry(hist_2, (std::string(Tds[1] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	legend->AddEntry(hist_3, (std::string(Tds[2] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	legend->AddEntry(hist_4, (std::string(Tds[3] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	legend->AddEntry(hist_5, (std::string(Tds[4] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	legend->AddEntry(hist_6, (std::string(Tds[5] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	legend->AddEntry(hist_7, (std::string(Tds[6] + " Td, fPMT#1 Cd peak")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
