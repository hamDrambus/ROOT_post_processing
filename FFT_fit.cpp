#include "TVirtualFFT.h"
#include "TF1.h"
#include "TMath.h"

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

void ScaleAxis(TAxis *a, double min, double max) {
	if (!a) return;
	if (a->GetXbins()->GetSize()) {
		// an axis with variable bins
		TArrayD X(*(a->GetXbins()));
		double prev_min = a->GetXmin();
		double prev_max = a->GetXmax();
		for(int i = 0, i_end_ = X.GetSize(); i !=i_end_ ; ++i)
			X[i] = min + (max-min)*(X[i]-prev_min)/(prev_max-prev_min);
		a->Set((X.GetSize() - 1), X.GetArray());
	} else {
	  // an axis with fixed bins
	  a->Set(a->GetNbins(), min, max);
	}
	return;
}

Double_t FittingF_2exp (Double_t *x, Double_t *par) {
  //par[0] - start time (fixed)
  //par[1] - amplitude1
  //par[2] - tau1
  //par[3] - amplitude2
  //par[4] - tau2
  return (x[0] <= par[0]) ? 0 : (par[1]*std::exp((par[0]-x[0])/par[2]) + par[3]*std::exp((par[0]-x[0])/par[4]));
}

int FFT_fit (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    int Nbins = 1800;
	double triplet_tau = 3.2; //us
	std::vector<double> VUV_fr = {1, 0.5, 0.2, 0.0};
    double time_left = 0, time_right = 160;//us
    double time_pretrigger_left = 7, time_pretrigger_right = 20;
	double norm_t_left = 28;
    TF1 *unity_f = new TF1("f1", "1", time_left, time_right);
    TH1D* hist_1 = new TH1D ("hist1", "hist1", Nbins, time_left, time_right);
	TH1* hist_fft = NULL;
	TH1D* hist_result = NULL;
	TH1D* hist_triplet_exp = NULL;
	std::vector<std::string> Tds(7, "6.8");
	//16kV, 6.8Td
	std::vector<double> norm_t_right =  {35.0, 35.0, 35.0, 35.0, 33.2, 34.9, 33.6}; //with trigger adjustment
	std::vector<double> fit_from = 	    {38.6, 37.0, 36.2, 36.1, 35.1, 36.0, 36.0}; //20, 18, ..., 8 kV
	std::vector<double> fit_to =        {160,  160,  160,  160,  50.0, 52.0, 52.0}; //edge SiPMs 20, 18, ..., 8 kV
	//12kV, 5.1Td
	//std::vector<double> norm_t_right =  {35.0, 35.0, 35.0, 35.0, 33.2, 34.9, 33.6};
	//std::vector<double> fit_from = 	    {38.6, 37.0, 36.2, 36.1, 35.1, 36.0, 36.0};
	//std::vector<double> fit_to =        {49,  49,  49,  55,  50.0, 52.0, 52.0};
	
    std::vector<TH1D*> hists;
    //hists.push_back(hist_1);
    //hists.push_back(hist_2);
    //hists.push_back(hist_3);
    //hists.push_back(hist_4);
    //hists.push_back(hist_5);
    //hists.push_back(hist_6);
	//hists.push_back(hist_7);
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
    double max_val = 0;
	bool linear = false;
	bool do_fit = true;
	std::string prefix = "190404/results_v3/Cd_46V_16kV_850V/forms_Cd_peak/";
	read_hist_w (hist_1, prefix + "8_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "9_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "10_form_by_Npeaks.hdata");
	read_hist_w (hist_1, prefix + "11_form_by_Npeaks.hdata");

	std::string framename = std::string("190404 No WLS convolution ") + dbl_to_str(triplet_tau, 1) + "#mus, " + Tds[0] + " Td";

	{
        double baseline = 0;
        int counts = 0;
        for (int bin = 1, bin_end = hist_1->GetNbinsX()+1; bin!=bin_end; ++bin) {
		    if (hist_1->GetBinCenter(bin)>time_pretrigger_left && hist_1->GetBinCenter(bin)<time_pretrigger_right) {
	           baseline += hist_1->GetBinContent(bin);
               ++counts;
		    }
        }
        hist_1->Sumw2();
        baseline = -1*baseline/counts;
        hist_1->Add(unity_f, baseline);
        double integral = 0;
        for (int bin = 1, bin_end = hist_1->GetNbinsX()+1; bin!=bin_end; ++bin) {
		    if (hist_1->GetBinCenter(bin)>norm_t_left && hist_1->GetBinCenter(bin)<norm_t_right[0]) {
	           integral += hist_1->GetBinContent(bin) * hist_1->GetBinWidth(bin);
		    }
        }
		hist_1->Scale(1.0/integral);
    }

	double *re_signal_fft = new double [Nbins];
	double *im_signal_fft = new double [Nbins];
	double *re_conv_fft = new double [Nbins];
	double *im_conv_fft = new double [Nbins];

	for (std::size_t k = 0; k!=Nbins; ++k) {
		double Nt = (time_right - time_left) / triplet_tau;
		double pipi4 = M_PI * M_PI * 4;
		re_conv_fft[k] = Nt / (Nt*Nt + pipi4*k*k);
		im_conv_fft[k] = -1.0 * M_PI * 2 * k / (Nt*Nt + pipi4*k*k);
	}


	hist_fft = hist_1->FFT(0, "MAG");
	TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
	
	fft->GetPointsComplex(re_signal_fft, im_signal_fft);
	for (std::size_t k = 0; k!=Nbins; ++k) {
		double re = re_signal_fft[k];
		double im = im_signal_fft[k];
		re_signal_fft[k] = re * re_conv_fft[k] - im * im_conv_fft[k];
		im_signal_fft[k] = re * im_conv_fft[k] + im * re_conv_fft[k];
	}	
	TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &Nbins, "C2R M");
	fft_back->SetPointsComplex(re_signal_fft, im_signal_fft);
	fft_back->Transform();
	hist_result = (TH1D*) TH1D::TransformHisto(fft_back, hist_result, "Re");
	ScaleAxis(hist_result->GetXaxis(), time_left, time_right);
	hist_result->Scale(1.5/Nbins);

	TVirtualFFT *fft_back2 = TVirtualFFT::FFT(1, &Nbins, "C2R M");
	fft_back2->SetPointsComplex(re_conv_fft, im_conv_fft);
	fft_back2->Transform();
	hist_triplet_exp = (TH1D*) TH1D::TransformHisto(fft_back2, hist_triplet_exp, "Re");
	ScaleAxis(hist_triplet_exp->GetXaxis(), time_left, time_right);

	{
        double integral = 0;
        for (int bin = 1, bin_end = hist_result->GetNbinsX()+1; bin!=bin_end; ++bin) {
		    if (hist_result->GetBinCenter(bin)>norm_t_left && hist_result->GetBinCenter(bin)<norm_t_right[0]) {
	           integral += hist_result->GetBinContent(bin) * hist_result->GetBinWidth(bin);
		    }
        }
		hist_result->Scale(1.0/integral);
    }
	for (std::size_t f = 0, f_end_ = VUV_fr.size(); f!=f_end_; ++f) {
		hists.push_back((TH1D*) hist_1->Clone());
		hists.back()->Scale(1-VUV_fr[f]);
		hists.back()->Add(hist_result, VUV_fr[f]);
	}

	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh)
        max_val = std::max(max_val, hists[hh]->GetBinContent(hists[hh]->GetMaximumBin()));
	max_val*= linear ? 1.2 : 2;
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_fft = new TCanvas ((std::string("fft_") + framename).c_str(), (std::string("fft_") + framename).c_str(), DEF_W, DEF_H);
	c_fft->SetGrid();
	c_fft->SetTicks();
	c_fft->ToggleEventStatus();
    c_fft->ToggleToolBar();
	if (!linear)
		c_fft->SetLogy();
	hist_fft->Draw("same");

	TCanvas *c_triplet = new TCanvas ((std::string("triplet_") + framename).c_str(), (std::string("triplet_") + framename).c_str(), DEF_W, DEF_H);
	c_triplet->SetGrid();
	c_triplet->SetTicks();
	c_triplet->ToggleEventStatus();
    c_triplet->ToggleToolBar();
	if (!linear)
		c_triplet->SetLogy();
	hist_triplet_exp->Draw("hist Lsame");
	TF1* triplet_fit = new TF1("fit_triplet", FittingF_2exp, time_left, time_right, 5);
	triplet_fit->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
	triplet_fit->FixParameter(0, 0);
	triplet_fit->SetParLimits(1, 1e-3, 1e3);
	triplet_fit->SetParLimits(2, 1, 10);
	triplet_fit->SetParLimits(3, 1e-6, 1e-1);
	triplet_fit->SetParLimits(4, 11, 1e6);	
	std::cout<<"Triplet fit:"<<std::endl;
	triplet_fit->SetLineColor(palette_minor[3]);
	hist_triplet_exp->Fit(triplet_fit);
	triplet_fit->Draw("same");

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
	for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_ && do_fit; ++hh) {
		ffs[hh] = new TF1("fit1", FittingF_2exp, fit_from[hh], fit_to[hh], 5);
		ffs[hh]->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
		ffs[hh]->FixParameter(0, fit_from[hh]);
		ffs[hh]->SetParLimits(1, 1e-3, 2);
		ffs[hh]->SetParLimits(2, 1, 10);
		if (fit_to[hh]<150) {
			ffs[hh]->SetParLimits(3, 1e-8, 1e-1);
			ffs[hh]->FixParameter(4, 1e6);	
		} else {
			ffs[hh]->SetParLimits(3, 3e-4, 1e-1);
			ffs[hh]->SetParLimits(4, 30, 1000);
		}
		ffs[hh]->SetLineColor(palette_minor[hh]);
    	hists[hh]->Fit(ffs[hh]);
		tau1.push_back(dbl_to_str(ffs[hh]->GetParameter(2), 2));
		tau2.push_back(dbl_to_str(ffs[hh]->GetParameter(4)>1000 ? 0 : ffs[hh]->GetParameter(4), 0));
		ffs[hh]->Draw("same");
    }
	if (!linear && do_fit) { 
		double ypos0 = 0.057;	
		double ypos1 = 0.008;
		double offset = 0.08/0.05;
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (50, ypos0*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau_{1}=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (105, ypos1*std::pow(offset, hh_end_ - hh - 1), (std::string("#tau_{2}=")+tau2[hh]).c_str());
			txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
			txt11->SetTextColor(palette_major[hh]); txt11->Draw();

		}
	}
	if (linear && do_fit) { 
		double ypos0 = 0.18;	
		double ypos1 = 0.18;
		double offset = 0.035;
		for (int hh = 0, hh_end_ = tau1.size(); hh!=hh_end_; ++hh) {
			auto *txt1 = new TLatex (40, ypos0+offset*(hh_end_ - hh - 1), (std::string("#tau_{1}=")+tau1[hh]).c_str());
			txt1->SetTextAlign(12); txt1->SetTextSize(0.05);
			txt1->SetTextColor(palette_major[hh]); txt1->Draw();

			auto *txt11 = new TLatex (115, ypos1+offset*(hh_end_ - hh - 1), (std::string("#tau_{2}=")+tau2[hh]).c_str());
			txt11->SetTextAlign(12); txt11->SetTextSize(0.05);
			txt11->SetTextColor(palette_major[hh]); txt11->Draw();

		}
	}
	
	legend->AddEntry(hists[0], (std::string(Tds[0] + " Td 190404 fPMTs " + dbl_to_str(VUV_fr[0], 1) +" VUV contribution" )).c_str(), "l");
	legend->AddEntry(hists[1], (std::string(Tds[0] + " Td 190404 fPMTs " + dbl_to_str(VUV_fr[1], 1) +" VUV contribution" )).c_str(), "l");
	legend->AddEntry(hists[2], (std::string(Tds[0] + " Td 190404 fPMTs " + dbl_to_str(VUV_fr[2], 1) +" VUV contribution" )).c_str(), "l");
	legend->AddEntry(hists[3], (std::string(Tds[0] + " Td 190404 fPMTs " + dbl_to_str(VUV_fr[3], 1) +" VUV contribution" )).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
