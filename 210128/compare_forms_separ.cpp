//Created on 01.09.2023

// Analysis of "proper" separation of fast and slow components for 210128 data.
// This is done for answering reviewers' questions on Paper91 (Slow-long comp. ratio and theory)
// Based on 190404/vcompare_forms.cpp and 210128/compare_forms2.cpp
// TODO: Requires code clean-up like in 220112/subtract_forms2.cpp

#define PAIR std::pair<double, double>

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

void subtract_component(TH1D *hist_out, TH1D *hist_in, TF1* f) {
		if (hist_in!=NULL && f!=NULL && hist_out != NULL) {
	    for (int bin = 1, bin_end = hist_in->GetNbinsX()+1; bin!=bin_end; ++bin) {
				double f_val = std::max(f->Eval(hist_in->GetBinCenter(bin)), 0.0);
				hist_out->SetBinContent(bin, std::max(hist_in->GetBinContent(bin) - f_val, 0.0));
	    }
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
  //par[0] - signal maximum position (fixed)
	//par[1] - Y offset
	//par[2] - tau rising (fixed)
	//par[3] - amplitude1
  //par[4] - tau1 falling
  //par[5] - amplitude2
  //par[6] - tau2 falling
	//par[7] - sharpness (fixed)
	double x0 = par[0] - par[2] * std::log((par[2] + par[4])/par[2]/par[7]);
	double rise = (par[7]-std::exp((x0-x[0])/par[2]));
	if (rise<0)
		return 0;
  return par[1] + rise*(par[3]*std::exp((x0-x[0])/par[4]) + par[5]*std::exp((x0-x[0])/par[6]));
}

Double_t FittingF_exp (Double_t *x, Double_t *par) {
  //par[0] - signal maximum position (fixed)
  //par[1] - Y offset
	//par[2] - tau rising (fixed)
  //par[3] - amplitude
	//par[4] - tau falling
	//par[5] - sharpness (fixed)
	double x0 = par[0] - par[2] * std::log((par[2] + par[4])/par[2]/par[5]);
	double rise = (par[5]-std::exp((x0-x[0])/par[2]));
	if (rise<0)
		return 0;
  return par[1] + par[3]*rise*std::exp((x0-x[0])/par[4]);
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
	PAIR raise_tau_bound;
	PAIR sharpness_bound;
	PAIR slow_ampl_bound;
	PAIR slow_tau_bound;
	PAIR long_ampl_bound;
	PAIR long_tau_bound;
	double max_time;
	double long_max_time; //used only in separate fit
	bool simultaneous_fit; //long+slow simultaneously or not
	bool do_fit;
	std::string fit_option; //"NWL" or "N"
//OUTPUT:
	TH1D* hist;
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

void draw_slow_component(TF1* fit_f, pulse_shape& shape)
{
	TF1* temp = new TF1(*fit_f);
	double xmin, xmax;
	temp->GetRange(xmin, xmax);
	temp->SetRange(shape.fast_t.first + shape.t_offset, xmax);
	temp->SetNpx(800);
	//temp->Draw("same");
}

int compare_forms_separ (void) {
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F
	std::string def_fit_option = "NLWRE";
	bool combined = true;
	bool Cd_peak = true;
	int Nbins = 1000;
	bool linear = 1;

	bool PMTs = false;
	bool central = true;
	bool fit_bad_forms = true;
	bool do_fit = true;
	bool subtact_baseline = true;
	bool center_pulses = false;
	bool print_errors = false;
	bool print_results = false;
  double time_pretrigger_left = 5.5, time_pretrigger_right = 19;
  double time_left = 0, time_right = 160;//us
  double max_val = 0;
	double trigger_at = 32;
	double y_min = 1e-4;
	bool draw_fast_only = true;

	pulse_shape* define = NULL, *copy = NULL;

	pulse_shape SiPM_19kV_no_trigger;
	define = &SiPM_19kV_no_trigger;
define->folder = std::string("210128/results_v6/Pu_46V_19kV_850V/") + (Cd_peak ? (central ? "forms_Alpha_peak/" : "forms_Alpha_peak_x-y/") : "forms_Alpha_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.0";
define->device = "SiPM matrix";
define->fast_t_center = 29.0;
define->fast_t = PAIR(23, 30.8);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->max_time = 30.1;
define->slow_fit_t = PAIR(31.2, 154);
define->long_fit_t = PAIR(31.2, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->raise_tau_bound = PAIR(4.0, 4.0);
define->sharpness_bound = PAIR(0.3, 0.3);
define->slow_ampl_bound = PAIR(1e-3, 10);
define->slow_tau_bound = PAIR(2.5, 100);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_19kV_no_trigger_edge;
	define = &SiPM_19kV_no_trigger_edge;
	copy = &SiPM_19kV_no_trigger;
*define = *copy;
define->fnames = {"SiPMs_edge_form_by_Npe.hdata"};
define->device = "SiPM matrix (edge)";
define->max_time = 30.1;
define->slow_fit_t = PAIR(31.2, 154);
define->long_fit_t = PAIR(31.2, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->raise_tau_bound = PAIR(4.0, 4.0);
define->sharpness_bound = PAIR(0.4, 0.4);
define->slow_ampl_bound = PAIR(1e-3, 10);
define->slow_tau_bound = PAIR(2.5, 100);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);

	pulse_shape SiPM_16kV_no_trigger;
	define = &SiPM_16kV_no_trigger;
define->folder = std::string("210128/results_v6/Pu_46V_16kV_850V/") + (Cd_peak ? (central ? "forms_Alpha_peak_x-y/" : "forms_Alpha_peak/") : "forms_Alpha_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.7";
define->device = "SiPM matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(21, 31.5);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->max_time = 30.4;
define->slow_fit_t = PAIR(31.5, 155);
define->long_fit_t = PAIR(31.5, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->raise_tau_bound = PAIR(5, 5);
define->sharpness_bound = PAIR(0.3, 0.3);
define->slow_ampl_bound = PAIR(1e-3, 10);
define->slow_tau_bound = PAIR(2.5, 100);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_16kV_no_trigger_edge;
	define = &SiPM_16kV_no_trigger_edge;
	copy = &SiPM_16kV_no_trigger;
*define = *copy;
define->fnames = {"SiPMs_edge_form_by_Npe.hdata"};
define->device = "SiPM matrix (edge)";
define->max_time = 30.4;
define->slow_fit_t = PAIR(31.5, 155);
define->long_fit_t = PAIR(31.5, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->raise_tau_bound = PAIR(5, 5);
define->sharpness_bound = PAIR(0.3, 0.3);
define->slow_ampl_bound = PAIR(1e-3, 10);
define->slow_tau_bound = PAIR(2.5, 100);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);

	pulse_shape SiPM_13kV_no_trigger;
	define = &SiPM_13kV_no_trigger;
define->folder = std::string("210128/results_v6/Pu_46V_13kV_850V/") + (Cd_peak ? (central ? "forms_Alpha_peak_x-y/" : "forms_Alpha_peak/") : "forms_Alpha_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.5";
define->device = "SiPM matrix";
define->fast_t_center = 29.2;
define->fast_t = PAIR(20, 32.3);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->max_time = 30.8;
define->slow_fit_t = PAIR(31.5, 155);
define->long_fit_t = PAIR(31.5, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->raise_tau_bound = PAIR(6, 6);
define->sharpness_bound = PAIR(0.3, 0.3);
define->slow_ampl_bound = PAIR(1e-1, 10);
define->slow_tau_bound = PAIR(2.5, 100);
define->long_ampl_bound = PAIR(1e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_13kV_no_trigger_edge;
	define = &SiPM_13kV_no_trigger_edge;
	copy = &SiPM_13kV_no_trigger;
*define = *copy;
define->fnames = {"SiPMs_edge_form_by_Npe.hdata"};
define->device = "SiPM matrix (edge)";
define->max_time = 30.8;
define->slow_fit_t = PAIR(31.5, 155);
define->long_fit_t = PAIR(31.5, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->raise_tau_bound = PAIR(6, 6);
define->sharpness_bound = PAIR(0.3, 0.3);
define->slow_ampl_bound = PAIR(1e-1, 10);
define->slow_tau_bound = PAIR(2.5, 100);
define->long_ampl_bound = PAIR(1e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	//std::vector<pulse_shape> pulses = {SiPM_19kV_no_trigger};
	//std::vector<pulse_shape> pulses = {SiPM_16kV_no_trigger};
	//std::vector<pulse_shape> pulses = {SiPM_13kV_no_trigger};
	//std::vector<pulse_shape> pulses = {SiPM_13kV_no_trigger_edge};

	//For papers/reports:
	//std::vector<pulse_shape> pulses = {SiPM_19kV_no_trigger, SiPM_16kV_no_trigger, SiPM_13kV_no_trigger};
	std::vector<pulse_shape> pulses = {SiPM_19kV_no_trigger_edge, SiPM_16kV_no_trigger_edge, SiPM_13kV_no_trigger_edge};

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	//adsf - for fast Crtl + F
	std::string framename;
	if (PMTs)
		framename = std::string("Results for PMTs (no WLS), ")+(Cd_peak ? "" : "< ")+"5.6 MeV #alpha ^{238}Pu";
	else
		framename = std::string("Results for SiPM matrix (edge, no WLS), ")+(Cd_peak ? "" : "< ")+"5.6 MeV #alpha ^{238}Pu";

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		hist_name = "hist" + std::to_string(hh) + "no_long";
		pulses[hh].no_long_hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		hist_name = "hist" + std::to_string(hh) + "residue";
		pulses[hh].fast_hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		pulses[hh].t_offset = center_pulses ? (trigger_at - pulses[hh].fast_t_center) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff)
			read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], pulses[hh].t_offset);

    pulses[hh].hist->Sumw2();
		pulses[hh].baseline = average(pulses[hh].hist, time_pretrigger_left + pulses[hh].t_offset, time_pretrigger_right + pulses[hh].t_offset);
		if (pulses[hh].subtract_baseline) {
				subtract_baseline(pulses[hh].hist, pulses[hh].baseline);
				pulses[hh].baseline = 0;
		}
		if (pulses[hh].renormalize) {
		    double integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
				pulses[hh].hist->Scale(pulses[hh].scale/integral);
				pulses[hh].baseline *= pulses[hh].scale/integral;
		}
		if (!pulses[hh].subtract_baseline)
				pulses[hh].baseline_bound = PAIR(0.8*pulses[hh].baseline, 1.2*pulses[hh].baseline);
		//pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
		//pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
		//pulses[hh].restored_integral = 0;
		//pulses[hh].slow_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
		//pulses[hh].long_integral = 0; //ATM only
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
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : y_min, max_val);
	frame->GetXaxis()->SetTitle("Time (#mus)");
	//=====================================
	if (!linear)
		frame->GetXaxis()->SetRangeUser(0, 160);
	else
		frame->GetXaxis()->SetRangeUser(20, 40);
	//=====================================
	frame->GetYaxis()->SetTitle("PE peak count (arb. u.)");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist->SetLineColor(palette_major[hh]);
		pulses[hh].fast_hist->SetLineWidth(2);
		if (!draw_fast_only)
			pulses[hh].fast_hist->SetLineStyle(7);//9
		pulses[hh].fast_hist->SetLineColor(palette_major[hh]);
		pulses[hh].no_long_hist->SetLineWidth(2);
		pulses[hh].no_long_hist->SetLineStyle(7);//9
		pulses[hh].no_long_hist->SetLineColor(palette_major[hh+1]);
		if (!draw_fast_only)
			pulses[hh].hist->Draw("hist Lsame");
  }

	int precision1 = 1, precision2 = 0, precision3 = 2, precision4 = 2;
	int line_width = 3;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].total_integral = 0;
		pulses[hh].fast_integral = 0;
		pulses[hh].restored_integral = 0;
		pulses[hh].slow_integral = 0;
		pulses[hh].long_integral = 0;
		pulses[hh].slow_on_fast = 0;
		pulses[hh].long_on_fast = 0;
		if (!pulses[hh].do_fit) {
			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX, pulses[hh].baseline);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset, 		pulses[hh].baseline);
			pulses[hh].slow_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
			pulses[hh].tau1 = "--";
			pulses[hh].tau1_err = "--";
			pulses[hh].tau2 = "--";
			pulses[hh].tau2_err = "--";

			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
			pulses[hh].err1 = "--";
			pulses[hh].Fr2 = "--";
			pulses[hh].err2 = "--";
		} else {
		if (!pulses[hh].simultaneous_fit) { //first, fit long comp. only by tail, then slow component when long is subtracted
			TF1* fit_f = NULL;
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {//no long component fit
				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";
				pulses[hh].Fr2 = "--";
				pulses[hh].err2 = "--";
				pulses[hh].no_long_hist = pulses[hh].hist;
			} else {
				fit_f = new TF1("fit2", FittingF_exp, pulses[hh].long_fit_t.first + pulses[hh].t_offset, pulses[hh].long_fit_t.second + pulses[hh].t_offset, 6);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "#tau_{R}", "amplitude", "#tau", "sharpness");
				SetParLimits(fit_f, 0, pulses[hh].long_max_time + pulses[hh].t_offset);
				SetParLimits(fit_f, 1, pulses[hh].long_baseline_bound.first, pulses[hh].long_baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
				SetParLimits(fit_f, 4, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
				SetParLimits(fit_f, 5, pulses[hh].sharpness_bound.first, pulses[hh].sharpness_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				fit_f->SetLineWidth(line_width);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, pulses[hh]);
				subtract_component(pulses[hh].no_long_hist, pulses[hh].hist, fit_f);
				pulses[hh].no_long_hist->Draw("hist Lsame");
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(4), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(4), precision2);
				pulses[hh].baseline = fit_f->GetParameter(1);
				TF1* slow_int_f = new TF1(*fit_f);
				slow_int_f->FixParameter(1, 0);
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX, pulses[hh].baseline);
				pulses[hh].long_integral = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
				pulses[hh].long_on_fast = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);

				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			}
			bool long_exist = (fit_f != NULL);
			fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].slow_fit_t.second + pulses[hh].t_offset, 6);
			fit_f->SetParNames("start_time", "y0", "#tau_{R}", "amplitude", "#tau", "sharpness");
			SetParLimits(fit_f, 0, pulses[hh].max_time + pulses[hh].t_offset);
			SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
			SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
			SetParLimits(fit_f, 3, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
			SetParLimits(fit_f, 4, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
			SetParLimits(fit_f, 5, pulses[hh].sharpness_bound.first, pulses[hh].sharpness_bound.second);
			fit_f->SetLineColor(palette_minor[hh]);
			fit_f->SetLineWidth(line_width);
    	pulses[hh].no_long_hist->Fit(fit_f, pulses[hh].fit_option.c_str());
			draw_slow_component(fit_f, pulses[hh]);
			pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(4), precision1);
			pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(4), precision1);
			if (!long_exist) {
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = integrate(pulses[hh].no_long_hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX, pulses[hh].baseline);
			}
			TF1* slow_int_f = new TF1(*fit_f);
			slow_int_f->FixParameter(1, 0);
			subtract_component(pulses[hh].fast_hist, pulses[hh].no_long_hist, slow_int_f);
			pulses[hh].slow_integral = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
			pulses[hh].slow_on_fast = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
			pulses[hh].fast_integral = integrate(pulses[hh].fast_hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
			pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
			if (!long_exist) {
				pulses[hh].long_integral = pulses[hh].baseline * (pulses[hh].slow_fit_t.second - pulses[hh].fast_t.first);
				pulses[hh].long_on_fast = pulses[hh].baseline * (pulses[hh].fast_t.second - pulses[hh].fast_t.first);

				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			}
			pulses[hh].fit_fs.push_back(fit_f);
			pulses[hh].fast_hist->Draw("hist Lsame");
		} else { //sumulteneous fit
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {
				TF1* fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].slow_fit_t.second + pulses[hh].t_offset, 6);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "#tau_{R}", "amplitude", "#tau", "sharpness");
				SetParLimits(fit_f, 0, pulses[hh].max_time + pulses[hh].t_offset);
				SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				SetParLimits(fit_f, 4, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
				SetParLimits(fit_f, 5, pulses[hh].sharpness_bound.first, pulses[hh].sharpness_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				fit_f->SetLineWidth(line_width);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, pulses[hh]);
				subtract_component(pulses[hh].fast_hist, pulses[hh].hist, fit_f);
				pulses[hh].fast_hist->Draw("hist Lsame");
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(4), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(4), precision1);
				TF1* slow_int_f = new TF1(*fit_f);
				slow_int_f->FixParameter(1, 0);
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX, pulses[hh].baseline);
				pulses[hh].slow_integral = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
				pulses[hh].slow_on_fast = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
				pulses[hh].fast_integral = integrate(pulses[hh].fast_hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
				pulses[hh].long_integral = pulses[hh].baseline * (pulses[hh].slow_fit_t.second - pulses[hh].fast_t.first);
				pulses[hh].long_on_fast = pulses[hh].baseline * (pulses[hh].fast_t.second - pulses[hh].fast_t.first);

				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);

				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			} else {
				TF1* fit_f = new TF1("fit1", FittingF_2exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].long_fit_t.second + pulses[hh].t_offset, 8);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "#tau_{R}", "amplitude1", "#tau1", "amplitude2", "#tau2", "sharpness");
				SetParLimits(fit_f, 0, pulses[hh].max_time + pulses[hh].t_offset);
				SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				SetParLimits(fit_f, 4, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
				SetParLimits(fit_f, 5, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
				SetParLimits(fit_f, 6, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
				SetParLimits(fit_f, 7, pulses[hh].sharpness_bound.first, pulses[hh].sharpness_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				fit_f->SetLineWidth(line_width);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, pulses[hh]);
				subtract_component(pulses[hh].fast_hist, pulses[hh].hist, fit_f);
				pulses[hh].fast_hist->Draw("hist Lsame");
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX, pulses[hh].baseline);
				pulses[hh].fast_integral = integrate(pulses[hh].fast_hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(4), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(4), precision1);
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(6), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(6), precision2);
				TF1* slow_f = new TF1(*fit_f);
				slow_f->FixParameter(1, 0);
				slow_f->FixParameter(5, 0);
				pulses[hh].slow_integral = slow_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
				pulses[hh].slow_on_fast = slow_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
				TF1* long_f = new TF1(*fit_f);
				long_f->FixParameter(1, 0);
				long_f->FixParameter(3, 0);
				pulses[hh].long_integral = long_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
				pulses[hh].long_on_fast = long_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			}
		}
		}
		std::cout<<"Results for "<<pulses[hh].folder<<"\""<<pulses[hh].device<<"\":"<<std::endl;
		std::cout<<"Fit option: \""<<pulses[hh].fit_option<<"\" Nbins="<<Nbins<<std::endl;
		std::cout<<"Check Sum: Fast="<<pulses[hh].fast_integral<<" + Slow=" <<pulses[hh].slow_integral<<" + Long=" <<pulses[hh].long_integral << " == "<<std::endl;
		std::cout<<"\t\t"<<pulses[hh].fast_integral+pulses[hh].slow_integral+pulses[hh].long_integral << " vs Total= "<< pulses[hh].total_integral <<std::endl;
		std::cout<<"#tau1\t#tau1_err\tFr1\tFr1_err\t#tau2\t#tau2_err\tFr2\tFr2_err"<<std::endl;
		std::cout<<pulses[hh].tau1<<"\t"<<pulses[hh].tau1_err<<"\t"<<pulses[hh].Fr1<<"\t"<<pulses[hh].err1<<"\t"
			<<pulses[hh].tau2<<"\t"<<pulses[hh].tau2_err<<"\t"<<pulses[hh].Fr2<<"\t"<<pulses[hh].err2<<std::endl;
    }
	std::vector<std::string> tau1, tau2, frsS, frsL;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string emp = "";
		std::string unit = " #mus";
		//std::string unit = "\\hbox{ мкс}";
		if (pulses[hh].tau1.empty() || pulses[hh].tau1 == "--")
			tau1.push_back(emp);
		else
			tau1.push_back("#tau_{S}=" + pulses[hh].tau1 + (print_errors ? "#pm" + pulses[hh].tau1_err : unit));
			//tau1.push_back("{\\it{\\tau_{S}}} = " + pulses[hh].tau1 + (print_errors ? "#pm" + pulses[hh].tau1_err : unit));
		if (pulses[hh].tau2.empty() || pulses[hh].tau2 == "--")
			tau2.push_back("");
		else
			tau2.push_back("#tau_{L}=" + pulses[hh].tau2  + (print_errors ? "#pm" + pulses[hh].tau2_err : unit));
			//tau2.push_back("{\\it{\\tau_{L}}} = " + pulses[hh].tau2  + (print_errors ? "#pm" + pulses[hh].tau2_err : unit));
		if (pulses[hh].Fr1.empty() || pulses[hh].Fr1 == "--")
			frsS.push_back("");
		else
			frsS.push_back(pulses[hh].Fr1 + (print_errors ? "#pm" + pulses[hh].err1 : emp));
			//frsS.push_back(std::string("\\hbox{") + pulses[hh].Fr1 + (print_errors ? "#pm" + pulses[hh].err1 : emp) + "}");
		if (pulses[hh].Fr2.empty() || pulses[hh].Fr2 == "--")
			frsL.push_back("");
		else
			frsL.push_back(pulses[hh].Fr2 + (print_errors ? "#pm" + pulses[hh].err2 : emp));
			//frsL.push_back(std::string("\\hbox{") + pulses[hh].Fr2 + (print_errors ? "#pm" + pulses[hh].err2 : emp) + "}");
	}
	if (print_results) {
		if (!linear) {
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Contribution:", "Slow"};
			std::vector<std::string> Long_title = {"Long"};
			if (print_errors) {
				add_text(45.8, 0.04, no_title, tau1, palette_major);
				add_text(75, 0.007, Slow_title, frsS, palette_major);
				add_text(101, 0.007, Long_title, frsL, palette_major);
				add_text(130, 0.007, no_title, tau2, palette_major);
			} else {
				add_text(43, 0.05, no_title, tau1, palette_major);
				add_text(68, 0.008, Slow_title, frsS, palette_major);
				add_text(98, 0.008, Long_title, frsL, palette_major);
				add_text(125, 0.008, no_title, tau2, palette_major);
			}
		} else {
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Slow component", "Contribution:"};
			std::vector<std::string> Long_title;// = {"Long"};
			add_text(36, 0.15, no_title, tau1, palette_major);
			add_text(46, 0.15, Slow_title, frsS, palette_major);
			//add_text(52, 0.08, Long_title, frsL, palette_text);
			//add_text(58, 0.08, no_title, tau2, palette_text);
		}
	}

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh)
		legend->AddEntry(pulses[hh].hist, (std::string("E/N = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
