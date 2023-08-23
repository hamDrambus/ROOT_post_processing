//Created on 2023.01.24
//requires .x init.cpp
//For 211007/results_v5 data
//Some parts of the code are copied from 211007/compare_forms1.cpp
//Obtaining EL gap only slow component by deconvolution using THGEM(2650V)+EL_gap(8.3 Td) and THGEM(2650V)+EL_gap(3.8 Td)

#include "TVirtualFFT.h"
#include "TF1.h"
#include "TMath.h"

#define PAIR std::pair<double, double>

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

double integrate(TH1D *hist, double from, double to) {
	double Int = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
      Int += hist->GetBinContent(bin) * hist->GetBinWidth(bin);
    }
  }
	return Int;
}

double integrate(TGraphErrors* graph, double from, double to) {
	double Int = 0;
  for (int bin = 0, bin_end = graph->GetN(); bin!=bin_end; ++bin) {
    if (graph->GetX()[bin]>from && graph->GetX()[bin]<to) {
      Int += graph->GetY()[bin] * (0 == bin ? graph->GetX()[1] - graph->GetX()[0] : graph->GetX()[bin] - graph->GetX()[bin-1]);
    }
  }
	return Int;
}

double integrate(TH1D *hist, double from, double to, double baseline) {
	double Int = 0;
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
				double val = std::max(hist->GetBinContent(bin) - baseline, 0.0);
				Int += val * hist->GetBinWidth(bin);
    }
  }
	return Int;
}

double integrate(TGraphErrors* graph, double from, double to, double baseline) {
	double Int = 0;
  for (int bin = 0, bin_end = graph->GetN(); bin!=bin_end; ++bin) {
    if (graph->GetX()[bin]>from && graph->GetX()[bin]<to) {
      double val = std::max(graph->GetY()[bin] - baseline, 0.0);
      Int += val * (0 == bin ? graph->GetX()[1] - graph->GetX()[0] : graph->GetX()[bin] - graph->GetX()[bin-1]);
    }
  }
	return Int;
}

void SubtractBaseline(TH1D *hist, double baseline) {
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
		hist->SetBinContent(bin, std::max(hist->GetBinContent(bin) - baseline, 0.0));
    }
}

void SubtractBaseline(TGraphErrors* graph, double baseline) {
  for (int i = 0, i_end_ = graph->GetN(); i!=i_end_; ++i) {
    graph->GetY()[i] = graph->GetY()[i] - baseline;
  }
}

void scale(TH1D *hist, double factor) {
  hist->Scale(factor);
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

double get_max_y(TH1D *hist) {
  return hist->GetBinContent(hist->GetMaximumBin());
}

enum ShapeFileType {Histogram = 0, Raw = 1, RawIntegral = 2, RawNoError = 3};

struct pulse_shape {
//INPUT:
	std::string folder;
	std::vector<std::string> fnames;
  int file_type; //0 - histogram, 1 - charge signal, 2 - integrated (large shaping) charge signal
	std::string Td;
	std::string device;
	double fast_t_center; //used for signal alignment
	PAIR fast_t; //fast component start&finish time, used for signal normalization
	double S1_t_center; //may be used for signal alignment
	PAIR S1_t; //S1 start&finish time, may be used for signal normalization
	double scale;
	bool subtract_baseline;
	bool renormalize;
//Fit:
	PAIR slow_fit_t;
	PAIR long_fit_t;
	PAIR baseline_bound;
	PAIR long_baseline_bound; //used only in separate fit
	PAIR slow_ampl_bound;
	PAIR slow_tau_bound;
	PAIR long_ampl_bound;
	PAIR long_tau_bound;
	bool simultaneous_fit; //long+slow simultaneously or not
	bool do_fit;
	std::string fit_option; //"NLWRE" or "NRE"
//OUTPUT:
	TH1D* hist;
  TGraphErrors* graph;
	TH1D* fast_hist;
	TH1D* no_long_hist;
	std::vector<TF1*> fit_fs;
	double max_val;
	double t_offset;
	bool do_restore; //signal outside of 160 mcs
	//Results:
	double baseline;
	double total_integral;
	double fast_integral;
	double restored_integral; //outside of 160 mcs
	double slow_integral;
	double long_integral;
	double slow_on_fast; //for systematic error estimation
	double long_on_fast; //for systematic error estimation
	std::string tau1, tau1_err;
	std::string tau2, tau2_err;
	std::string Fr1, err1;
	std::string Fr2, err2;
	std::string FrLost;
};

int s211007_1 (void) {
// .x init.cpp
// .x THGEM_subtraction/s211007_1.cpp

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	int DEF_W = 1300, DEF_H = 700;
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};

	//qewr
	int Nbins = 1200;
	bool linear = 1;
	bool subtract_baseline = true;
	bool center_pulses = true;
	//double time_pretrigger_left = 51.0, time_pretrigger_right = 62.0;
  double time_pretrigger_left = 6.0, time_pretrigger_right = 22.0;
	double time_left = 3, time_right = 159;//us
	double max_val = 0;
	double trigger_at = 28.4;
	double y_min = 1e-5;

	pulse_shape* define = NULL, *copy = NULL;

	pulse_shape SiPM_20_0kV_no_trigger;
	define = &SiPM_20_0kV_no_trigger;
define->folder = std::string("211007/results_v5/Pu_46V_20.0kV_800V_2650V/forms_Pu_peak/");
define->fnames = {"SiPMs_edge_form_by_Npe.hdata"};
define->Td = "8.3";
define->device = "V_{THGEM}=2110V";
define->fast_t_center = 28.4;
define->fast_t = PAIR(24.5, 29.6);
//define->fast_t = PAIR(24.5, 60);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;

	pulse_shape SiPM_11_8kV_no_trigger;
	define = &SiPM_11_8kV_no_trigger;
define->folder = std::string("211007/results_v5/Pu_46V_11.8kV_800V_2650V/forms_Pu_peak/");
define->fnames = {"SiPMs_edge_form_by_Npe.hdata"};
define->Td = "4.2";
define->device = "V_{THGEM}=2110V";
define->fast_t_center = 28.7;
define->fast_t = PAIR(24.5, 30.7);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;

	pulse_shape SiPM_11_0kV_no_trigger;
	define = &SiPM_11_0kV_no_trigger;
define->folder = std::string("211007/results_v5/Pu_46V_11.0kV_800V_2650V/forms_Pu_peak/");
define->fnames = {"SiPMs_edge_form_by_Npe.hdata"};
define->Td = "3.8";
define->device = "V_{THGEM}=2110V";
define->fast_t_center = 28.8;
define->fast_t = PAIR(24.5, 30.8);
//define->fast_t = PAIR(24.5, 60);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1.0;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;

	//Only 2 must be used!
	std::vector<pulse_shape> pulses = {SiPM_20_0kV_no_trigger, SiPM_11_0kV_no_trigger};

	std::string framename = std::string("Results for SiPM-matrix (no WLS), ")+"5.5 MeV #alpha ^{238}Pu";
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = ("hist") + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		double toff = pulses[hh].t_offset = center_pulses ?
			(trigger_at - pulses[hh].fast_t_center) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff)
      read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], toff);
    pulses[hh].hist->Sumw2();
		pulses[hh].baseline = average(pulses[hh].hist, time_pretrigger_left + toff, time_pretrigger_right + toff);
		if (pulses[hh].subtract_baseline) {
      SubtractBaseline(pulses[hh].hist, pulses[hh].baseline);
      pulses[hh].baseline = 0;
		}
		if (pulses[hh].renormalize) {
			double integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + toff, pulses[hh].fast_t.second + toff);
      scale(pulses[hh].hist, pulses[hh].scale/integral);
      pulses[hh].baseline *= pulses[hh].scale/integral;
		}
    pulses[hh].max_val = get_max_y(pulses[hh].hist);
    max_val = std::max(max_val, pulses[hh].max_val);
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
	TLegend *legend = new TLegend(0.52, 0.8, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : y_min, max_val);
	frame->GetXaxis()->SetTitle("Time (#mus)");
	//=====================================
	if (!linear)
		frame->GetXaxis()->SetRangeUser(0, 160);
	else
		frame->GetXaxis()->SetRangeUser(20, 80);
	//=====================================
	frame->GetYaxis()->SetTitle("PE peak count");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist->SetLineColor(palette_major[hh]);
		pulses[hh].hist->Draw("hist Lsame");
  }

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
    legend->AddEntry(pulses[hh].hist, (std::string("E/N_{EL} = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");
  }
	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();


	bool do_save = false;

	//!!! Many of the parameters are hard-coded !!!
  TF1 *unity_f = new TF1("f1", "1", time_left, time_right);
  TH1D* hist_input1 = pulses[0].hist;
	TH1D* hist_input2 = pulses[1].hist;
	TH1D* hist_result = NULL; //result is drawn in separate canvas

	std::string out_filename = "THGEM_subtraction/results_v5/8.3Td-3.8Td.dat";

	framename = std::string("EL gap only pulse-shape (deconvolution)");

	double *re_fft_res = new double [Nbins];
	double *im_fft_res = new double [Nbins];

	double *re_input1_fft = new double [Nbins];
	double *im_input1_fft = new double [Nbins];
	hist_input1->FFT(0, "MAG");
	TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
	fft->GetPointsComplex(re_input1_fft, im_input1_fft);

	double *re_input2_fft = new double [Nbins];
	double *im_input2_fft = new double [Nbins];
	hist_input2->FFT(0, "MAG");
	fft = TVirtualFFT::GetCurrentTransform();
	fft->GetPointsComplex(re_input2_fft, im_input2_fft);

	for (std::size_t k = 0; k!=Nbins; ++k) { // division of two complex numbers
		double re1 = re_input1_fft[k];
		double im1 = im_input1_fft[k];
		double re2 = re_input2_fft[k];
		double im2 = im_input2_fft[k];
		double ampl2_sqr = re2*re2 + im2*im2;
		re_fft_res[k] = (re1 * re2 + im1 * im2)/ampl2_sqr;
		im_fft_res[k] = (im1 * re2 - im2 * re1)/ampl2_sqr;
	}
	TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &Nbins, "C2R M");
	fft_back->SetPointsComplex(re_fft_res, im_fft_res);
	fft_back->Transform();
	hist_result = (TH1D*) TH1D::TransformHisto(fft_back, hist_result, "Re");
	ScaleAxis(hist_result->GetXaxis(), time_left, time_right);
	hist_result->Scale(1.0/Nbins);

	TCanvas *c_1 = new TCanvas (std::string("211007 deconvoluted pulse-shape").c_str(), std::string("211007 deconvoluted pulse-shape").c_str(), DEF_W, DEF_H);
	c_1->SetGrid();
	c_1->SetTicks();
	c_1->ToggleEventStatus();
	c_1->ToggleToolBar();
	if (!linear)
		c_1->SetLogy();
	max_val = 0;
	double min_val = 0;
	max_val = std::max(max_val, hist_result->GetBinContent(hist_result->GetMaximumBin()));
	min_val = std::min(min_val, hist_result->GetBinContent(hist_result->GetMinimumBin()));
	max_val*= linear ? 1.2 : 2;
	min_val*= linear ? 1.4 : 2;
	TH2F* frame_1 = new TH2F("frame1", framename.c_str(), 500, time_left, time_right, 500, linear ? min_val : 1e-4, max_val);
	frame_1->GetXaxis()->SetTitle("Time (#mus)");
	frame_1->GetYaxis()->SetTitle("Amplitude (arb.)");
	frame_1->Draw();

	hist_result->SetLineColor(palette_major[0]);
	hist_result->SetLineWidth(2);
	hist_result->SetNameTitle("211007 deconvoluted pulse-shape", "211007 deconvoluted pulse-shape");
	hist_result->Draw("hist Lsame");
	if (do_save)
		save_graph(hist_result, out_filename);

	TLegend *legend_1 = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend_1->SetHeader("");
	legend_1->SetMargin(0.25);
	legend_1->AddEntry(hist_result, (std::string("8.3 - 3.8Td deconvolution").c_str()), "l");
	legend_1->Draw("same");
	c_1->Update();


	delete [] re_fft_res;
	delete [] im_fft_res;
	delete [] re_input1_fft;
	delete [] im_input1_fft;
	delete [] re_input2_fft;
	delete [] im_input2_fft;

  return 0;
}
