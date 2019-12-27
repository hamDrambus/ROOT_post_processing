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

Double_t Electron_real_delayed_density (Double_t *x, Double_t *par) {//electrons only
	//x[0] - t/t_drift, >= 0
	//x[1] - x/L_gap. x/L>t/t_drift -> 0. x/L must be <=1 and >=0
	//Normalization is not taken (saves 1 multiplication during integration)
	//par[1] - Attachment probability (for the whole dirft through L). <=1
	//par[2] - t_drift/tau_ion
	if (((x[0] - x[1]) < 0) || (x[0] < 0))
		return 0;
	double z = 2.0 * std::sqrt(-1.0 * x[1] * par[2] * std::log(par[1]) * (x[0] - x[1]));
	double I1z = 0;	
	if (z < 1e-3) {//avoid singularity
		I1z = 0.5 + z*z/16 + z*z*z*z/192; //I_1(z)/z approximation
	} else {
		I1z = TMath::BesselI(1, z) / z;
	}
	return -2 * par[2] * x[1] * std::log(par[1]) * I1z * std::exp(-1.0 * (x[0] - x[1])*par[2] + x[1] * std::log(par[1]));
}

Double_t Ion_real_delayed_density (Double_t *x, Double_t *par) {//ions only
	//x[0] - t/t_drift, >= 0
	//x[1] - x/L_gap. x/L>t/t_drift -> 0. x/L must be <=1 and >=0
	//Normalization is not taken (saves 1 multiplication during integration)
	//par[1] - Attachment probability (for the whole dirft through L). <=1
	//par[2] - t_drift/tau_ion
	if (((x[0] - x[1]) < 0) || (x[0] < 0))
		return 0;
	double z = 2.0 * std::sqrt(-1.0 * x[1] * par[2] * std::log(par[1]) * (x[0] - x[1]));
	double I0z = TMath::BesselI(0, z);
	return -1 * std::log(par[1]) * I0z * std::exp(-1.0 * (x[0] - x[1])*par[2] + x[1] * std::log(par[1]));
}

Double_t Electron_arrived_current (Double_t *x, Double_t *par) {
	//x[0] - time [microseconds]
	//par[0] - Normalization
	//par[1] - Attachment probability
	//par[2] - t_drift/tau_ion
	//par[3] - t_drift;
	//par[4] - time offset
	//par[5] - cathode change pulse full width (microseconds) 
	double tx[2];
	tx[0] = (x[0] - par[4])/par[3]; // t/t_drift
	tx[1] = 1; //x/L_gap.
	double Val = Electron_real_delayed_density(tx, par); //Delayed contribution
	Val = Ion_real_delayed_density(tx, par);
	//Not delayed contribution (integral of delta function (V_drift * t - x)) W_attach^(x/L) dx:
	double rel_delta = 0.5*par[5]/par[3];	
	Val += (tx[0] >= (1-rel_delta) && tx[0] <= (1+rel_delta) ? par[1] : 0);
	return par[0]*Val;
}

Double_t Electron_induced_current (Double_t *x, Double_t *par) {
	//x[0] - time [microseconds]
	//par[0] - Normalization
	//par[1] - Attachment probability
	//par[2] - t_drift/tau_ion
	//par[3] - t_drift;
	//par[4] - time offset
	const std::size_t Nbins = 10000;
	const double dx = ((double)1)/(Nbins-1);
	double tx[2];
	tx[0] = (x[0] - par[4])/par[3]; // t/t_drift
	double Int = 0;
	for (std::size_t i = 0; i!=Nbins; ++i) { //Delayed contribution
		tx[1] = dx*i; //x/L_gap.
		Int += Electron_real_delayed_density(tx, par) * dx * (i == 0 || i==(Nbins-1) ? 0.5 : 1.0);
	}
	//Not delayed contribution (integral of delta function (V_drift * t - x)) W_attach^(x/L) dx:
	Int += (tx[0] < 0 || tx[0] > 1) ? 0 : std::pow(par[1], tx[0]);
	//std::cout<<"F("<<x[0]<<"us, t/t_d = "<<tx[0]<<") =\t"<<par[0]*Int<<std::endl;
	return par[0]*Int;
}

Double_t FittingF_2exp (Double_t *x, Double_t *par) {
  //par[0] - start time (fixed)
  //par[1] - amplitude1
  //par[2] - tau1
  //par[3] - amplitude2
  //par[4] - tau2
  return (x[0] <= par[0]) ? 0 : (par[1]*std::exp((par[0]-x[0])/par[2]) + par[3]*std::exp((par[0]-x[0])/par[4]));
}

int fit_no_wls (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    int Nbins = 1800;
    double time_left = 0, time_right = 160;//us
	double fit_time_maximum = 140;
    double time_pretrigger_left = 6, time_pretrigger_right = 22;
	double norm_t_left = 24;
    TH1D* hist_1 = new TH1D ("hist1", "hist1", Nbins, time_left, time_right);
    std::vector<TH1D*> hists;
    hists.push_back(hist_1);
	//kVs:								  20,   18,   16,   14,   12,   10,   8 kV
    //std::vector<std::string> Tds =      {"8.5","7.6","6.8","5.9","5.1","4.2","3.4"};
	//std::vector<double> norm_t_right =  {32.8, 32.8, 33.2, 33.5, 33.9, 34.7, 36.2}; //20, 18, ..., 8 kV
	//std::vector<double> fit_from = 	  	{33.5, 33.6, 33.9, 34.2, 35.0, 35.0, 35.4}; //fPMT#1
	//std::vector<double> fit_to =        {53,  52.5,  52,  51.5,  50,   48,   42}; //fPMT#1
	
	std::vector<std::string> Tds(7, "6.8");
	std::vector<double> norm_t_right (7, 33.2);
	std::vector<double> fit_from (7, 33.9);
	std::vector<double> fit_to (7, 160);
	
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
    double max_val = 0;
	bool linear = true;
    std::string prefix = "190404/results_v4/Cd_46V_16kV_850V/forms_Cd_peak/";
	read_hist_w (hist_1, prefix + "8_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "9_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "10_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "11_form_by_Npeaks.hdata");
	std::string framename = std::string("190404 v4 (no WLS) Signal forms fast PMTs Cd peak") + " " + Tds[0] + " Td";

    for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double baseline = average(hists[hh], time_pretrigger_left, time_pretrigger_right);
        hists[hh]->Sumw2();
		subtract_baseline(hists[hh], baseline);
        double integral = integrate(hists[hh], norm_t_left, norm_t_right[hh]);
		hists[hh]->Scale(1.0/integral);
    }
    std::vector<double> fractionsS(hists.size(), 0), fractionsL(hists.size(), 0), fractionsLost(hists.size(), 0);
	std::vector<std::string> frsS(hists.size(), ""), frsL(hists.size(), ""), frsLost(hists.size(), "");
	std::stringstream ss;
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
	TF1* f_theory = new TF1("f_theory", Electron_induced_current, time_left, time_right, 5);
	f_theory->SetParNames("Norm", "Attach. Prob.", "t_drift/tau_ion", "t_drift", "time_offset");
	f_theory->FixParameter(0, 1);
	f_theory->FixParameter(1, 0.09);
	f_theory->FixParameter(2, 1);
	f_theory->FixParameter(3, 3.1);
	f_theory->FixParameter(4, 28);
	f_theory->SetLineColor(palette_minor[3]);
	f_theory->SetNpx(Nbins);
	TCanvas *c_1 = new TCanvas ((std::string("theory_") + framename).c_str(), (std::string("theory_") + framename).c_str(), DEF_W, DEF_H);
	c_1->SetGrid(); c_1->SetTicks(); c_1->ToggleEventStatus(); c_1->ToggleToolBar();
	if (!linear)
		c_1->SetLogy();
	f_theory->Draw("L");

	TF1* f_theory2 = new TF1("f_theory2", Electron_arrived_current, time_left, time_right, 6);
	f_theory2->SetParNames("Norm", "Attach. Prob.", "t_drift/tau_ion", "t_drift", "time_offset", "pulse_width");
	f_theory2->FixParameter(0, 1);
	f_theory2->FixParameter(1, 0.9);
	f_theory2->FixParameter(2, 1);
	f_theory2->FixParameter(3, 3.1);
	f_theory2->FixParameter(4, 28);
	f_theory2->FixParameter(5, 0.5);
	f_theory2->SetLineColor(palette_minor[3]);
	f_theory2->SetNpx(Nbins);
	TCanvas *c_2 = new TCanvas ((std::string("theory2_") + framename).c_str(), (std::string("theory2_") + framename).c_str(), DEF_W, DEF_H);
	c_2->SetGrid();	c_2->SetTicks(); c_2->ToggleEventStatus(); c_2->ToggleToolBar();
	if (!linear)
		c_2->SetLogy();
	f_theory2->Draw("L");

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
		double ypos0 = 0.02;	
		double ypos1 = 0.008;
		double offset = 0.08/0.05;
		auto *txtfr0 = new TLatex (80, ypos1*std::pow(offset, frsS.size()), "Slow fraction:");
		txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
		txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (50, ypos0*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau_{1}=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (111, ypos1*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau_{2}=")+tau2[hh]).c_str());
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
		double ypos0 = 0.17;	
		double ypos1 = 0.17;
		double offset = 0.035;
		auto *txtfr0 = new TLatex (36, ypos1+offset*frsS.size(), "Slow fraction:");
		txtfr0->SetTextAlign(12); txtfr0->SetTextSize(0.05);
		txtfr0->SetTextColor(kBlack); txtfr0->Draw();
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (34.3, ypos0+offset*(hh_end_ - hh - 1), (std::string("#tau_{1}=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (115, ypos1+offset*(hh_end_ - hh - 1), (std::string("#tau_{2}=")+tau2[hh]).c_str());
			txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
			txt11->SetTextColor(palette_major[hh]); txt11->Draw();
		}
		for (int hh = 0, hh_end_ = frsS.size(); hh!=hh_end_; ++hh) {
			auto *txtfr1 = new TLatex (36, ypos1+offset*(hh_end_ - hh - 1), (frsS[hh]).c_str());
			txtfr1->SetTextAlign(12); txtfr1->SetTextSize(0.05);
			txtfr1->SetTextColor(palette_major[hh]); txtfr1->Draw();
			auto *txtfr2 = new TLatex (37, ypos1+offset*(hh_end_ - hh - 1), (frsL[hh]).c_str());
			txtfr2->SetTextAlign(12); txtfr2->SetTextSize(0.05);
			txtfr2->SetTextColor(palette_major[hh]); txtfr2->Draw();
		}
	}
	
	legend->AddEntry(hist_1, (std::string(Tds[0] + " Td, fPMTs, Cd peak")).c_str(), "l");
	//legend->AddEntry(hist_2, (std::string(Tds[1] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	//legend->AddEntry(hist_3, (std::string(Tds[2] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	//legend->AddEntry(hist_4, (std::string(Tds[3] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	//legend->AddEntry(hist_5, (std::string(Tds[4] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	//legend->AddEntry(hist_6, (std::string(Tds[5] + " Td, fPMT#1 Cd peak")).c_str(), "l");
	//legend->AddEntry(hist_7, (std::string(Tds[6] + " Td, fPMT#1 Cd peak")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
