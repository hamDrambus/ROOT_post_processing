//Created on 2022.02.10
//requires .x init.cpp
//For 211007/results_v5 data
//Restoring undershoot (baseline shift) using fourier transform.
//undershoot:
//https://www-physics.lbl.gov/~spieler/NSS_short-course/nss99.pdf
//https://ns.ph.liv.ac.uk/~ajb/ukgs_nis/pre-course-material/lec2-03.pdf
//https://www.ortec-online.com/-/media/ametekortec/other/amplifier-introduction.pdf?la=en

#include "TVirtualFFT.h"
#include "TF1.h"
#include "TMath.h"

TH1D* CreateHist(std::vector<double> &ixs, std::vector<double> &iys, std::vector<double> &iyes) {
	double max_val = *std::max_element(ixs.begin(), ixs.end());
	double min_val = *std::min_element(ixs.begin(), ixs.end());
	Int_t Nbins = ixs.size();
	TH1D* out = new TH1D ("hist_input", "hist_input", Nbins, min_val, max_val);
	for (int bin = 1, bin_end = out->GetNbinsX()+1; bin!=bin_end; ++bin) {
		out->SetBinContent(bin, iys[bin-1]);
	}
	return out;
}

void read_graph (std::string fname, std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &errs, double offset = 0) {
  xs.clear();
  ys.clear();
  errs.clear();
  std::ifstream str;
	str.open(fname);
	if (!str.is_open()) {
      std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;
      return;
  }
  double x, y, err;
  std::string line, word;
  int line_n = 0;
	while (!str.eof() && str.is_open()) {
    std::getline(str, line);
		++line_n;
    try {
			word = strtoken(line, "\t ");
			x = std::stod(word) + offset;
			word = strtoken(line, "\t ");
			y = std::stod(word);
      word = strtoken(line, "\t ");
			err = std::stod(word);
      xs.push_back(x);
      ys.push_back(y);
      errs.push_back(err);
		}
		catch (std::invalid_argument &e) {
      std::cerr << "read_graph: invalid argument on line " << line_n << std::endl;
      std::cerr << e.what() << std::endl;
			continue;
		}
    catch (std::out_of_range &e) {
      std::cerr << "read_graph: string to value overflow on line " << line_n << std::endl;
      std::cerr << e.what() << std::endl;
			continue;
		}
		catch (std::exception &e) {
			std::cerr << "read_graph: Unforeseen exception on line " << line_n << std::endl;
			std::cerr << e.what() << std::endl;
			return;
		}
	}
	str.close();
}

void save_graph(TH1D* hist, std::string fname) {
	std::ofstream str;
	open_output_file(fname, str, std::ios_base::trunc);
	if (!str.is_open()) {
		std::cerr<<"Failed to open: "<<fname<<std::endl;
		return;
	}
	for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
		str<<hist->GetBinCenter(bin)<<"\t"<<hist->GetBinContent(bin)<<"\t0.0"<<std::endl;
	}
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

int FFT_baseline_restoration (void) {
/*
.x init.cpp
.x charge_shape/FFT_baseline_restoration.cpp
*/

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	int DEF_W = 1300, DEF_H = 700;
	bool do_save = true;
	//double CSA_tau = 90; //us, the decay of charge sensitive amplifier we are removing
	//double CSA_tau = 90; //20.0 kV 211007
	//double CSA_tau = 70; //18.8 kV 211007
	//double CSA_tau = 67; //17.0 kV 211007
	//double CSA_tau = 63; //15.3 kV 211007
	//double CSA_tau = 65; //14.4 kV 211007
	//double CSA_tau = 50; //13.6 kV 211007
	//double CSA_tau = 50; //12.7 kV 211007
	//double CSA_tau = 50; //11.8 kV 211007
	double CSA_tau = 50; //11.0 kV 211007
	double time_left = 0, time_right = 160;//us
	//!!! Many of the parameters are hard-coded !!!
  TF1 *unity_f = new TF1("f1", "1", time_left, time_right);
  TH1D* hist_input = NULL;
	TH1* hist_fft_ampl = NULL;
	TH1* hist_fft_phase = NULL; //not used
	TH1D* hist_result = NULL; //result is drawn together with input

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
  double max_val = 0, min_val = 0;
	bool linear = true;
	std::string in_filename = "../Data/211007/results_v2/211007_Pu_20.0kV_800V_46V_12dB_2650V/Charge_10/Charge211007_Pu_20.0kV_800V_46V_12dB_2650V_ch_10_AVR_0";
	std::string out_filename = "211007/results_v5/restored_Q_fixed_tau_50us/211007_Pu_20.0kV_800V_46V_12dB_2650V";

	std::vector<double> xs, ys, errs;
	read_graph(in_filename, xs, ys, errs, 0);
	int Nbins = xs.size();
	double dx = xs[1] - xs[0];
	hist_input = CreateHist(xs, ys, errs);
	hist_input->SetNameTitle("Input charge shape", "Input charge shape");

	std::string framename = std::string("211007 Charge shape 8.5 Td, 2650 V");

	double *re_fix_fft = new double [Nbins];
	double *im_fix_fft = new double [Nbins];

	for (std::size_t k = 0; k!=Nbins; ++k) {
		double value = CSA_tau / (time_right - time_left);
		re_fix_fft[k] = 1;
		if (k != 0)
			im_fix_fft[k] = -1.0 /(M_PI * 2 * k * value);
		else
			im_fix_fft[k] = 0;
		//re_fix_fft[k] = 1;
		//im_fix_fft[k] = 0;
	}

	double *re_signal_fft = new double [Nbins];
	double *im_signal_fft = new double [Nbins];
	hist_fft_ampl = hist_input->FFT(0, "MAG");
	TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();

	fft->GetPointsComplex(re_signal_fft, im_signal_fft);
	for (std::size_t k = 0; k!=Nbins; ++k) {
		double re = re_signal_fft[k];
		double im = im_signal_fft[k];
		re_signal_fft[k] = re * re_fix_fft[k] - im * im_fix_fft[k];
		im_signal_fft[k] = re * im_fix_fft[k] + im * re_fix_fft[k];
	}
	TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &Nbins, "C2R M");
	fft_back->SetPointsComplex(re_signal_fft, im_signal_fft);
	fft_back->Transform();
	hist_result = (TH1D*) TH1D::TransformHisto(fft_back, hist_result, "Re");
	ScaleAxis(hist_result->GetXaxis(), time_left, time_right);
	hist_result->Scale(1.0/Nbins);

	TCanvas *c_1 = new TCanvas (std::string("211007 Charge shape").c_str(), std::string("211007 Charge shape").c_str(), DEF_W, DEF_H);
	c_1->SetGrid();
	c_1->SetTicks();
	c_1->ToggleEventStatus();
	c_1->ToggleToolBar();
	if (!linear)
		c_1->SetLogy();
	max_val = *std::max_element(ys.begin(), ys.end());
	min_val = *std::min_element(ys.begin(), ys.end());
	max_val = std::max(max_val, hist_result->GetBinContent(hist_result->GetMaximumBin()));
	min_val = std::min(min_val, hist_result->GetBinContent(hist_result->GetMinimumBin()));
	max_val*= linear ? 1.2 : 2;
	min_val*= linear ? 1.4 : 2;
	TH2F* frame_1 = new TH2F("frame1", framename.c_str(), 500, time_left, time_right, 500, linear ? min_val : 1e-4, max_val);
	frame_1->GetXaxis()->SetTitle("t [#mus]");
	frame_1->GetYaxis()->SetTitle("Amplitude [V]");
	frame_1->Draw();
	max_val = 0;
	hist_input->SetLineColor(palette_major[0]);
	hist_input->SetLineWidth(2);
	hist_input->Draw("hist Lsame");

	TF1* baseline_f = new TF1("baseline", "[0]", 0, 160);
	baseline_f->SetParLimits(0, min_val, -min_val);
	hist_result->Fit(baseline_f, "NWW", "", 0, 25);
	baseline_f->SetLineColor(palette_minor[1]);
	baseline_f->SetLineWidth(2);
	baseline_f->Draw("same");

	TF1* tail_f = new TF1("baseline", "[0]+[1]*x", 0, 160);
	tail_f->SetParLimits(0, -max_val, max_val);
	tail_f->SetParLimits(1, -max_val/(time_right-time_left), max_val/(time_right-time_left));
	hist_result->Fit(tail_f, "NWW", "", 80, 160);
	tail_f->SetLineColor(palette_major[2]);
	tail_f->SetLineWidth(2);
	tail_f->Draw("same");

	hist_result->SetLineColor(palette_major[1]);
	hist_result->SetLineWidth(2);
	hist_result->SetNameTitle("Output charge shape", "Output charge shape");
	hist_result->Draw("hist Lsame");
	if (do_save)
		save_graph(hist_result, out_filename);

	TLegend *legend_1 = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend_1->SetHeader("");
	legend_1->SetMargin(0.25);
	legend_1->AddEntry(hist_input, (std::string("Input charge").c_str()), "l");
	legend_1->AddEntry(hist_result, (std::string("Output charge").c_str()), "l");
	legend_1->Draw("same");
	c_1->Update();

	TCanvas *c_fft = new TCanvas ((std::string("fft_") + framename).c_str(), (std::string("fft_") + framename).c_str(), DEF_W, DEF_H);
	c_fft->SetGrid();
	c_fft->SetTicks();
	c_fft->ToggleEventStatus();
  c_fft->ToggleToolBar();
	if (!linear)
		c_fft->SetLogy();
	hist_fft_ampl->Draw("hist Lsame");
	c_fft->Update();

	TLegend *legend_2 = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend_2->SetHeader("");
	legend_2->SetMargin(0.25);
	legend_2->AddEntry(hist_fft_ampl, (std::string("FFT amplitude").c_str()), "l");
	legend_2->Draw("same");
	c_fft->Update();

	delete [] re_signal_fft;
	delete [] im_signal_fft;
	delete [] re_fix_fft;
	delete [] im_fix_fft;
  return 0;
}
