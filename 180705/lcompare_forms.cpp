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

void read_hist_w (TH1D *hist, std::string fname, double offset = 0) {
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
        hist->Fill(val1 + offset, val2);
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

void add_text(double x, double y, std::vector<std::string> title, std::vector<std::string> &entries, std::vector<Color_t>& colors) {
	TPad *pad = (TPad*)gPad;
	double offset;
	if (pad->GetLogy()) {
		TH2F* frame = (TH2F*)gDirectory->FindObject("frame");
		if (NULL==frame)
			return;		
		offset = std::log(frame->GetYaxis()->GetXmax()/frame->GetYaxis()->GetXmin())/5.75;
		//std::cout<<"Size="<<entries.size()<<", offset = "<<offset<<std::endl;
		for (int hh = 0, hh_end_ = title.size(); hh!=hh_end_; ++hh) {
			auto *txtfr0 = new TLatex (x, y*std::pow(offset, hh_end_ - hh - 1 + entries.size()), title[hh].c_str());
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
		for (int hh = 0, hh_end_ = title.size(); hh!=hh_end_; ++hh) {
			auto *txtfr0 = new TLatex (x, y+offset*(hh_end_ - hh - 1 + entries.size()), title[hh].c_str());
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

int lcompare_forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    int Nbins = 600;
    double time_left = 0, time_right = 160;//us
    double time_pretrigger_left = 7, time_pretrigger_right = 20;
	double norm_t_left = 24;
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
	std::vector<bool> do_draw = 		{1,    1, 	 0,    1,    0,    1,    0};
	std::vector<bool> do_fit = 			{1,    1, 	 1,    1,    1,    0,    0};
    std::vector<std::string> Tds =      {"8.5","7.6","6.8","5.9","5.1","4.2","3.4"};//20, 18, ..., 8 kV
	std::vector<double> trigger_offsets = {0,   0,    0,    0,   1.5, 	1.5,  1.5};
	std::vector<double> norm_t_right =  {34.3, 34.3, 34.9, 34.9, 35.8, 35.8, 36.7}; //WLS PMTs 20, 18, ..., 8 kV
	std::vector<double> fit_from = 	    {35.5, 35.6, 35.7, 35.8, 36.6, 36.0, 36.7}; //WLS PMTs 20, 18, ..., 8 kV
	std::vector<double> fit_to =        {160,  160,  160,  160,  56.6, 58.0, 52.0}; //WLS PMTs 20, 18, ..., 8 kV
	//std::vector<double> norm_t_right =  {34.0, 34.6, 35.0, 35.0, 35.8, 36.4, 36.7}; //bare PMTs 20, 18, ..., 8 kV
	//std::vector<double> fit_from = 	    {35.0, 35.0, 35.0, 35.5, 36.6, 37.0, 36.7}; //bare PMTs 20, 18, ..., 8 kV
	//std::vector<double> fit_to =        {160,  160,  160,  53.8,  53.0, 49.5, 45.5}; //bare PMTs 20, 18, ..., 8 kV
	//std::vector<double> norm_t_right =  {34.3, 34.6, 34.9, 35.0, 35.6, 36.1, 37.5}; //SiPMs 20, 18, ..., 8 kV
	//std::vector<double> fit_from = 	    {35.5, 35.6, 35.0, 35.4, 35.6, 36.0, 37.5}; //SiPMs 20, 18, ..., 8 kV
	//std::vector<double> fit_to =        {160,  160,  160,  160,  53.0, 49.5, 45.5}; //SiPMs 20, 18, ..., 8 kV
	std::vector<Color_t> palette_major = {kBlack, kRed, kMagenta, kGreen, kYellow + 2, kBlue, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kMagenta+3, kGreen -2, kMagenta+3, kAzure + 10, kOrange + 6};
    double max_val = 0;
	bool linear = false;
    std::string prefix = "180705/results_v1/Cd_48V_20kV_800V/forms_Cd_peak/";
	read_hist_w (hist_1, prefix + "SiPMs_form_by_Npe.hdata", trigger_offsets[0]);
	//read_hist_w (hist_1, prefix + "10_form_by_S.hdata", trigger_offsets[0]);
	//read_hist_w (hist_1, prefix + "11_form_by_S.hdata", trigger_offsets[0]);
	prefix = "180705/results_v1/Cd_48V_18kV_800V/forms_Cd_peak/";
	read_hist_w (hist_2, prefix + "SiPMs_form_by_Npe.hdata", trigger_offsets[1]);
	//read_hist_w (hist_2, prefix + "10_form_by_S.hdata", trigger_offsets[1]);
	//read_hist_w (hist_2, prefix + "11_form_by_S.hdata", trigger_offsets[1]);
	prefix = "180705/results_v1/Cd_48V_16kV_800V/forms_Cd_peak/";
	read_hist_w (hist_3, prefix + "SiPMs_form_by_Npe.hdata", trigger_offsets[2]);
	//read_hist_w (hist_3, prefix + "10_form_by_S.hdata", trigger_offsets[2]);
	//read_hist_w (hist_3, prefix + "11_form_by_S.hdata", trigger_offsets[2]);
	prefix = "180705/results_v1/Cd_48V_14kV_800V/forms_Cd_peak/";
	read_hist_w (hist_4, prefix + "SiPMs_form_by_Npe.hdata", trigger_offsets[3]);
	//read_hist_w (hist_4, prefix + "10_form_by_S.hdata", trigger_offsets[3]);
	//read_hist_w (hist_4, prefix + "11_form_by_S.hdata", trigger_offsets[3]);
	prefix = "180705/results_v1/Cd_48V_12kV_800V/forms_Cd_peak/";
	read_hist_w (hist_5, prefix + "SiPMs_form_by_Npe.hdata", trigger_offsets[4]);
	//read_hist_w (hist_5, prefix + "10_form_by_S.hdata", trigger_offsets[4]);
	//read_hist_w (hist_5, prefix + "11_form_by_S.hdata", trigger_offsets[4]);
	prefix = "180705/results_v1/Cd_48V_10kV_800V/forms_Cd_peak/";
	read_hist_w (hist_6, prefix + "SiPMs_form_by_Npe.hdata", trigger_offsets[5]);
	//read_hist_w (hist_6, prefix + "10_form_by_S.hdata", trigger_offsets[5]);
	//read_hist_w (hist_6, prefix + "11_form_by_S.hdata", trigger_offsets[5]);
	prefix = "180705/results_v1/Cd_48V_8kV_800V/forms_Cd_peak/";
	read_hist_w (hist_7, prefix + "SiPMs_form_by_Npe.hdata", trigger_offsets[6]);
	//read_hist_w (hist_7, prefix + "10_form_by_S.hdata", trigger_offsets[6]);
	//read_hist_w (hist_7, prefix + "11_form_by_S.hdata", trigger_offsets[6]);
	std::string framename = std::string("Results for SiPM matrix, 88 keV ^{209}Cd");// + " " + Tds[0] + " Td";

    for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        hists[hh]->Sumw2();
        double baseline = average(hists[hh], time_pretrigger_left, time_pretrigger_right);
		subtract_baseline(hists[hh], baseline);
        double integral = integrate(hists[hh], norm_t_left, norm_t_right[hh]);
		hists[hh]->Scale(1.0/integral);
    }
    std::vector<double> fractionsS(hists.size(), 0), fractionsL(hists.size(), 0), fractionsLost(hists.size(), 0);
	int fr_precision = 2;
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double fast_integral = integrate(hists[hh], norm_t_left, norm_t_right[hh]);
		double full_integral = integrate(hists[hh], norm_t_left, DBL_MAX);
		fractionsS[hh] = full_integral-fast_integral;
		fractionsL[hh] = full_integral;
    }
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
		if (do_draw[hh])
       		max_val = std::max(max_val, hists[hh]->GetBinContent(hists[hh]->GetMaximumBin()));
	}
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
	TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : 1e-4, max_val);
	frame->GetXaxis()->SetTitle("t [#mus]");
	frame->GetYaxis()->SetTitle("");
	frame->Draw();
	
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
		hists[hh]->SetLineWidth(2);
		hists[hh]->SetLineColor(palette_major[hh]);
		if (do_draw[hh])
			hists[hh]->Draw("hist Lsame");
    }
	std::vector<std::string> tau1, tau2;
	std::vector<std::string> frsS, frsL, frsLost;
	std::vector<Color_t> palette_text;
	std::vector<TF1*> ffs(hists.size(), NULL);
	int precision1 = 2, precision2 = 0;
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
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
    	hists[hh]->Fit(ffs[hh], "N");
		if (!do_draw[hh] || !do_fit[hh])
			continue;
		palette_text.push_back(palette_major[hh]);
		tau1.push_back("#tau_{S}=" + dbl_to_str(ffs[hh]->GetParameter(2), precision1));
		tau2.push_back("#tau_{L}=" + dbl_to_str((ffs[hh]->GetParameter(4)>1000 ? 0 : ffs[hh]->GetParameter(4)), precision2));
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
		frsS.push_back(dbl_to_str(fractionsS[hh], fr_precision));
		frsL.push_back(dbl_to_str(fractionsL[hh], fr_precision));
		frsLost.push_back(dbl_to_str(fractionsLost[hh], fr_precision));
		ffs[hh]->Draw("same");
		std::cout<<Tds[hh]<<" Td. Lost signal fraction:"<<frsLost.back()<<std::endl;
    }	
	if (!linear) {
		std::vector<std::string> no_title;
		std::vector<std::string> Slow_title = {"Contribution:", "Slow"};
		std::vector<std::string> Long_title = {"Long"};
		add_text(50, 0.04, no_title, tau1, palette_text);
		add_text(85, 0.0047, Slow_title, frsS, palette_text);
		add_text(101, 0.0047, Long_title, frsL, palette_text);
		add_text(116, 0.0047, no_title, tau2, palette_text);
	} else {
		std::vector<std::string> no_title;
		std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
		std::vector<std::string> Long_title;// = {"Long"};
		add_text(38, 0.15, no_title, tau1, palette_text);
		add_text(46, 0.15, Slow_title, frsS, palette_text);
		//add_text(52, 0.08, Long_title, frsL, palette_text);
		//add_text(58, 0.08, no_title, tau2, palette_text);
	}
	
	if (do_draw[0])
		legend->AddEntry(hist_1, ("E/N = " + std::string(Tds[0] + " Td, SiPM matrix")).c_str(), "l");	
	if (do_draw[1])
		legend->AddEntry(hist_2, ("E/N = " +std::string(Tds[1] + " Td, SiPM matrix")).c_str(), "l");
	if (do_draw[2])
		legend->AddEntry(hist_3, ("E/N = " +std::string(Tds[2] + " Td, SiPM matrix")).c_str(), "l");
	if (do_draw[3])
		legend->AddEntry(hist_4, ("E/N = " +std::string(Tds[3] + " Td, SiPM matrix")).c_str(), "l");
	if (do_draw[4])
		legend->AddEntry(hist_5, ("E/N = " +std::string(Tds[4] + " Td, SiPM matrix")).c_str(), "l");
	if (do_draw[5])
		legend->AddEntry(hist_6, ("E/N = " +std::string(Tds[5] + " Td, SiPM matrix")).c_str(), "l");
	if (do_draw[6])
		legend->AddEntry(hist_7, ("E/N = " +std::string(Tds[6] + " Td, SiPM matrix")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
