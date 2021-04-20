//Created on 2021.04.08
//For long component analysis (fit) only! Meant to be compared to 210302/compare_forms4.cpp and 210316/compare_forms2-3.cpp
#define PAIR std::pair<double, double>

//When on, compare_forms is called with parameters to analyze pulse-shape and save it
//to specified location automatically.
//#define FAST_FIGURES_MODE

std::string dbl_to_str (double val, int precision)
{
	std::stringstream ss;
	ss<<std::fixed<<std::setprecision(precision)<<val;
	return ss.str();
}

std::string int_to_str(int num, std::size_t decimals)
{
	std::string out = std::to_string(num);
	if (num < 0) {
		while ((out.size()-1)<decimals) {
			out = "0" + out;
		}
	} else {
		while (out.size()<decimals) {
			out = "0" + out;
		}
	}
	return out;
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

double integrate_function(TH1D *hist, TF1* func, double from, double to) {
	double Int = 0;
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
           Int += func->Eval(hist->GetBinCenter(bin)) * hist->GetBinWidth(bin);
	    }
    }
	return Int;
}

double integrate_function_bound(TH1D *hist, TF1* func, double from, double to) {
	double Int = 0;
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
           Int += std::min(hist->GetBinContent(bin), func->Eval(hist->GetBinCenter(bin))) * hist->GetBinWidth(bin);
	    }
    }
	return Int;
}

double integrate_function_bound(TH1D *hist, double constant, double from, double to) {
	double Int = 0;
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
           Int += std::min(hist->GetBinContent(bin), constant) * hist->GetBinWidth(bin);
	    }
    }
	return Int;
}

double integrate_linear_function_bound(TH1D *hist, double constant, double from, double to) {
	double Int = 0;
    for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	    if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
		   double x = hist->GetBinCenter(bin);
		   double f = 0 + constant * (x - from)/(to-from);
           Int += std::min(hist->GetBinContent(bin), f) * hist->GetBinWidth(bin);
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

double stdev(TH1D *hist, double from, double to) {
	double avr = average(hist, from, to);
	double sum = 0;
	int counts = 0;
	for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
	  if (hist->GetBinCenter(bin)>from && hist->GetBinCenter(bin)<to) {
	       sum += (hist->GetBinContent(bin) - avr)*(hist->GetBinContent(bin) - avr);
	       ++counts;
	  }
	}
	return std::sqrt(sum/(counts-1));
}

void SetParLimits(TF1* f, int n, double lower, double upper) {
	if (lower!=upper)
		f->SetParLimits(n, std::min(lower, upper), std::max(lower, upper));
	else
		f->FixParameter(n, lower);
}

void SetParLimits(TF1* f, int n, double value) {
	f->FixParameter(n, value);
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
	//par[1] - offset
  //par[2] - amplitude1
  //par[3] - tau1
  //par[4] - amplitude2
  //par[5] - tau2
  return par[1] + par[2]*std::exp((par[0]-x[0])/par[3]) + par[4]*std::exp((par[0]-x[0])/par[5]);
}

Double_t FittingF_exp (Double_t *x, Double_t *par) {
  //par[0] - start time (fixed), related to par[2]
  //par[1] - offset
  //par[2] - amplitude
  //par[3] - tau
  return par[1] + par[2]*std::exp((par[0]-x[0])/par[3]);
}

void add_text(double x, double y, std::vector<std::string> title, std::vector<std::string> &entries, std::vector<Color_t>& colors) {
	TPad *pad = (TPad*)gPad;
	double offset;
	if (pad->GetLogy()) {
		TH2F* frame = (TH2F*)gDirectory->FindObject("frame");
		if (NULL==frame)
			return;
		//offset at 1e-5 - 1 is std::log(1e5)/5.75;
		double off_at_1e5 = std::log(1e5)/5.75;
		off_at_1e5 = std::log(off_at_1e5)/std::log(1e5);
		offset = std::exp(std::log(frame->GetYaxis()->GetXmax()/frame->GetYaxis()->GetXmin()) * off_at_1e5);
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

struct pulse_shape {
//INPUT:
	std::string folder;
	std::vector<std::string> fnames;
	std::string Td;
	std::string device;
	PAIR baseline_t;
	double fast_t_center; //used for signal alignment
	PAIR fast_t; //fast component start&finish time, used for signal normalization
	double S1_t_center; //may be used for signal alignment
	PAIR S1_t; //S1 start&finish time, may be used for signal normalization
	double scale;
	bool subtract_baseline;
	bool renormalize;
	//Fit:
	PAIR long_fit_t;
	PAIR baseline_bound;
	PAIR long_ampl_bound;
	PAIR long_tau_bound;
	bool do_fit;
	std::string fit_option; //"NLWRE" or "NRE"
//OUTPUT:
	TH1D* hist;
	std::vector<TF1*> fit_fs;
	double max_val;
	double t_offset;
	bool do_restore; //signal outside of 160 mcs
	//Results:
	double baseline;
	double baseline_sigma;
	double total_integral;
	double fast_integral;
	double restored_integral; //outside of 160 mcs
	double long_integral;
	double long_on_fast; //for systematic error estimation
	std::string tau2, tau2_err;
	std::string Fr2, err2;
	std::string FrLost;
};

void draw_slow_component(TF1* fit_f, pulse_shape& shape)
{
	fit_f->SetNpx(800);
	fit_f->Draw("same");
}


int compare_forms3 (void) {
	std::cout<<"COMPARISON_MODE"<<std::endl;
	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::string def_fit_option = "NRE";
	bool combined = true;
	bool contribution_by_fit = true;
	bool min_baseline = 1;
	bool avg_baseline = true;
	double add_sigma = 0; //0, +-1
	int Nbins = 300;
	bool linear = 0;
	bool PMTs = true;
#ifdef FAST_FIGURES_MODE
	linear = (in_is_linear == "lin");
	Nbins = in_Nbins;
	def_fit_option = in_def_fit_option;
	combined = (in_is_combined != "sep");
#endif //FAST_FIGURES_MODE

	bool fast_PMTs = true;
	unsigned int PMT_used = 0;
	if (!fast_PMTs) PMT_used = 0x2 | 0x4 | 0x8; //PMT#1 is noisy
	else PMT_used = 0x8; //PMT#4 as the highest gain
//PMT_used = 0x2 | 0x4 | 0x8;
	bool do_fit = true;
	bool fit_bad_forms = true;
	bool subtact_baseline = true;
	bool center_pulses = false;
	bool center_at_S1 = false; //Not used
	bool normalize_by_S1 = false; //Not used
	bool print_errors = false;
	double time_pretrigger_left = 7.0, time_pretrigger_right = 20.0;
	double time_left = -18, time_right = 178;//us
	double right_cutoff = 142.1;//us
	double max_val = 0;
	double trigger_at = center_at_S1 ? 47.18 : 47.18;
	double y_min = 1e-5;
	Nbins = (time_right-time_left)*Nbins/(160.0);

	pulse_shape* define = NULL, *copy = NULL;

pulse_shape SiPM_16kV_87K_peak;
define = &SiPM_16kV_87K_peak;
	define->folder = std::string("210128/results_v6/Pu_46V_16kV_850V/forms_Alpha_peak/");
	define->fnames = {"SiPMs_form_by_Npe.hdata"};
	define->Td = "6.8";
	define->device = "SiPM-matrix, 87K, Pu peak";
	define->baseline_t = avg_baseline ? PAIR(4.77, 22.73) : (min_baseline ? PAIR(0, 0) : PAIR(0, 0));
	define->fast_t_center = 29.3;
	define->fast_t = PAIR(21, 67.1);
	define->S1_t_center = 0;
	define->S1_t = PAIR(0, 0);
	define->scale = 1;
	define->subtract_baseline = subtact_baseline;
	define->renormalize = true;
	define->long_fit_t = PAIR(62.1, 153);
	define->baseline_bound = PAIR(1e-6, 1e-6);
	define->long_ampl_bound = PAIR(3e-4, 1e-2);
	define->long_tau_bound = PAIR(15, 200);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;

pulse_shape SiPM_16kV_87K_slope;
define = &SiPM_16kV_87K_slope;
copy = &SiPM_16kV_87K_peak;
	define->folder = std::string("210128/results_v6/Pu_46V_16kV_850V/forms_Alpha_left/");
	define->fnames = {"SiPMs_form_by_Npe.hdata"};
	define->Td = "6.8";
	define->device = "SiPM-matrix, 87K, Pu slope";
	define->baseline_t = avg_baseline ? PAIR(4.77, 22.73) : (min_baseline ? PAIR(0, 0) : PAIR(0, 0));
	define->fast_t_center = copy->fast_t_center;
	define->fast_t = copy->fast_t;
	define->S1_t_center = copy->S1_t_center;
	define->S1_t = copy->S1_t;
	define->scale = 1;
	define->subtract_baseline = subtact_baseline;
	define->renormalize = true;
	define->long_fit_t = PAIR(62.1, 153);
	define->baseline_bound = PAIR(1e-6, 1e-6);
	define->long_ampl_bound = PAIR(3e-4, 1.9e-3);
	define->long_tau_bound = PAIR(15, 200);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;

pulse_shape PMT_16kV_87K_peak;
define = &PMT_16kV_87K_peak;
	define->folder = std::string("210128/results_v6/Pu_46V_16kV_850V/forms_Alpha_peak/");
	if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
	if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
	if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
	if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
	if(PMT_used == (0x1|0x2|0x4|0x8))
		define->device = "4PMT ";
	else
		define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
	define->device.pop_back();
	if (!fast_PMTs) define->device += " (slow)";
	define->device += ", 87K, Pu peak";
	define->Td = "6.8";
	define->baseline_t = avg_baseline ? PAIR(4.77, 23.2) : (min_baseline ? PAIR(0, 0) : PAIR(0, 0));
	define->fast_t_center = 29.3;
	define->fast_t = PAIR(21, 67.1);
	define->S1_t_center = 0;
	define->S1_t = PAIR(0, 0);
	define->scale = 1;
	define->subtract_baseline = subtact_baseline;
	define->renormalize = true;
	define->long_fit_t = PAIR(62.1, 153);
	define->baseline_bound = PAIR(1e-6, 1e-6);
	define->long_ampl_bound = PAIR(3e-4, 1e-2);
	define->long_tau_bound = PAIR(15, 200);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;

pulse_shape PMT_16kV_87K_slope;
define = &PMT_16kV_87K_slope;
copy = &PMT_16kV_87K_peak;
	define->folder = std::string("210128/results_v6/Pu_46V_16kV_850V/forms_Alpha_left/");
	define->fnames = copy->fnames;
	if(PMT_used == (0x1|0x2|0x4|0x8))
		define->device = "4PMT ";
	else
		define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
	define->device.pop_back();
	if (!fast_PMTs) define->device += " (slow)";
	define->device += ", 87K, Pu slope";
	define->Td = "6.8";
	define->baseline_t = avg_baseline ? PAIR(4.77, 23.2) : (min_baseline ? PAIR(0, 0) : PAIR(0, 0));
	define->fast_t_center = copy->fast_t_center;
	define->fast_t = copy->fast_t;
	define->S1_t_center = copy->S1_t_center;
	define->S1_t = copy->S1_t;
	define->scale = 1;
	define->subtract_baseline = subtact_baseline;
	define->renormalize = true;
	define->long_fit_t = PAIR(62.1, 153);
	define->baseline_bound = PAIR(1e-6, 1e-6);
	define->long_ampl_bound = PAIR(3e-4, 1e-2);
	define->long_tau_bound = PAIR(15, 200);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;

	//std::vector<pulse_shape> pulses = {SiPM_16kV_87K_peak, SiPM_16kV_87K_slope};
	std::vector<pulse_shape> pulses = {PMT_16kV_87K_peak, PMT_16kV_87K_slope};
	//std::vector<pulse_shape> pulses = {PMT_16kV_87K_peak};

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	//adsf - for fast Crtl + F
	std::string framename;
	if (PMTs)
		framename = std::string("Results for 4PMT (no WLS), ")+"5.6 MeV #alpha ^{238}Pu";
	else
		framename = std::string("Results for SiPM-matrix (no WLS), ")+"5.6 MeV #alpha ^{238}Pu";
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		pulses[hh].t_offset = center_pulses ?
			(trigger_at - (center_at_S1 ? pulses[hh].S1_t_center : pulses[hh].fast_t_center)) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff)
			read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], pulses[hh].t_offset);

    pulses[hh].hist->Sumw2();
		pulses[hh].baseline = average(pulses[hh].hist, pulses[hh].baseline_t.first + pulses[hh].t_offset, pulses[hh].baseline_t.second + pulses[hh].t_offset);
		pulses[hh].baseline_sigma = stdev(pulses[hh].hist, pulses[hh].baseline_t.first + pulses[hh].t_offset, pulses[hh].baseline_t.second + pulses[hh].t_offset);
		if (pulses[hh].subtract_baseline) {
			subtract_baseline(pulses[hh].hist, pulses[hh].baseline + add_sigma * pulses[hh].baseline_sigma);
			pulses[hh].baseline = 0;
		}
		if (pulses[hh].renormalize) {
				double integral = 0;
				if (normalize_by_S1)
		    	integral = integrate(pulses[hh].hist, pulses[hh].S1_t.first + pulses[hh].t_offset, pulses[hh].S1_t.second + pulses[hh].t_offset);
				else
					integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
			pulses[hh].hist->Scale(pulses[hh].scale/integral);
			pulses[hh].baseline *= pulses[hh].scale/integral;
		}
		if (!pulses[hh].subtract_baseline) {
			pulses[hh].baseline_bound = PAIR(0.8*pulses[hh].baseline, 1.2*pulses[hh].baseline);
		}
		pulses[hh].max_val = pulses[hh].hist->GetBinContent(pulses[hh].hist->GetMaximumBin());
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
	frame->GetXaxis()->SetTitle("Time [#mus]");
	//=====================================
	if (!linear)
		frame->GetXaxis()->SetRangeUser(0, 160);
	else {
		frame->GetXaxis()->SetRangeUser(0, 160);
		//frame->GetYaxis()->SetRangeUser(0, 0.02);
	}
	//=====================================
	frame->GetYaxis()->SetTitle("PE peak count");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist->SetLineColor(palette_major[hh]);
		pulses[hh].hist->Draw("hist Lsame");
  }

	int precision1 = 2, precision2 = 0, precision3 = 2, precision4 = 3;
	int line_width = 3;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].total_integral = 0;
		pulses[hh].fast_integral = 0;
		pulses[hh].restored_integral = 0;
		pulses[hh].long_integral = 0;
		pulses[hh].long_on_fast = 0;
		double Toff = pulses[hh].t_offset;
		if (!pulses[hh].do_fit || (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0)) {
			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);
			pulses[hh].long_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
			pulses[hh].long_on_fast = integrate(pulses[hh].hist, pulses[hh].fast_t.second + Toff, 2*pulses[hh].fast_t.second - pulses[hh].fast_t.first + Toff, pulses[hh].baseline);
			pulses[hh].tau2 = "--";
			pulses[hh].tau2_err = "--";

			pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision3);
			pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision3);
		} else {
			TF1* fit_f = new TF1("fit2", FittingF_exp, pulses[hh].long_fit_t.first + Toff, pulses[hh].long_fit_t.second + Toff, 4);
			pulses[hh].fit_fs.push_back(fit_f);
			fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
			SetParLimits(fit_f, 0, pulses[hh].long_fit_t.first + Toff);
			SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
			SetParLimits(fit_f, 2, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
			SetParLimits(fit_f, 3, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
			fit_f->SetLineColor(palette_minor[hh]);
			fit_f->SetLineWidth(line_width);
			pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
			draw_slow_component(fit_f, pulses[hh]);
			pulses[hh].baseline = fit_f->GetParameter(1);
			pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(3), precision2);
			pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(3), precision2);
			TF1* slow_int_f = new TF1(*fit_f);
			slow_int_f->FixParameter(1, 0);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);
			if (contribution_by_fit)
				pulses[hh].long_integral = slow_int_f->Integral(pulses[hh].fast_t.second + Toff, right_cutoff); //! not right_cutoff+Toff!
			else
				pulses[hh].long_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.second + Toff, right_cutoff, pulses[hh].baseline);
			pulses[hh].total_integral = pulses[hh].fast_integral + pulses[hh].long_integral;

			pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
			pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
		}
		std::cout<<"Results for "<<pulses[hh].folder<<"\""<<pulses[hh].device<<"\":"<<std::endl;
		std::cout<<"Fit option: \""<<pulses[hh].fit_option<<"\""<<std::endl;
		std::cout<<"Check Sum: Fast="<<pulses[hh].fast_integral<<" + Long=" <<pulses[hh].long_integral << " == "<<std::endl;
		std::cout<<"\t\t"<<pulses[hh].fast_integral+pulses[hh].long_integral << " vs Total= "<< pulses[hh].total_integral <<std::endl;
		std::cout<<"#tau2\t#tau2_err\tFr2\tFr2_err"<<std::endl;
		std::cout<<pulses[hh].tau2<<"\t"<<pulses[hh].tau2_err<<"\t"<<pulses[hh].Fr2<<"\t"<<pulses[hh].err2<<std::endl;
  }
	std::vector<std::string> tau2, frsL;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string emp = "";
		std::string unit = "#mus";
		if (pulses[hh].tau2.empty() || pulses[hh].tau2 == "--")
			tau2.push_back("");
		else
			tau2.push_back("#tau_{L}=" + pulses[hh].tau2  + (print_errors ? "#pm" + pulses[hh].tau2_err : unit));
		if (pulses[hh].Fr2.empty() || pulses[hh].Fr2 == "--")
			frsL.push_back("");
		else
			frsL.push_back(pulses[hh].Fr2 + (print_errors ? "#pm" + pulses[hh].err2 : emp));
	}
	if (do_fit) {
		if (!linear) {
			std::vector<std::string> no_title;
			std::vector<std::string> Long_title = {"Contribution:"};
			if (print_errors) {
				//zcxv
				add_text(100, 0.005, no_title, tau2, palette_major);
				add_text(129, 0.005, Long_title, frsL, palette_major);
			} else {
				add_text(122, 0.0017, Long_title, frsL, palette_major);
				add_text(140, 0.0017, no_title, tau2, palette_major);
			}
		} else {
			std::vector<std::string> no_title;
			std::vector<std::string> Long_title = {"Long component", "contribution:"};
			add_text(51, 0.08, no_title, tau2, palette_major);
			add_text(58, 0.08, Long_title, frsL, palette_major);
		}
	}

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh)
		legend->AddEntry(pulses[hh].hist, (std::string("E/N = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
#ifdef FAST_FIGURES_MODE
	std::string filename = in_kV + (PMTs ?"kV_4PMT_" :"kV_SiPMs_")+ (linear ? "lin":"log") + (Cd_peak ? "" : "_slope")
	 		+ "_Nb" + int_to_str(Nbins, 4) + "_" + def_fit_option + (combined ? "" : "sep") + ".png";
	c_->SaveAs((folder + filename).c_str(), "png");
#endif //FAST_FIGURES_MODE
  return 0;
}
