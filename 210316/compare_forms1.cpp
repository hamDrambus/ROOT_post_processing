//Created on 2021.03.28
//For several runs with gaseous Ar. Only long component analysis (fit)
//Data without trigger adjustment and without track selection
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

#ifdef FAST_FIGURES_MODE
//compare_forms1("20", "lin", "peak", 1800, "NLWRE", "comb");
int compare_forms1 (std::string in_kV, std::string in_is_linear, std::string in_is_orthogonal,
	 	int in_Nbins, std::string in_def_fit_option, std::string in_is_combined) {
		std::cout<<"FAST_FIGURES_MODE"<<std::endl;
#else //FAST_FIGURES_MODE
int compare_forms1 (void) {
		std::cout<<"COMPARISON_MODE"<<std::endl;
#endif //FAST_FIGURES_MODE
	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::string def_fit_option = "NRE";
	bool combined = true;
	bool contribution_by_fit = true;
	bool min_baseline = 1;
	bool avg_baseline = true;
	int Nbins = 300;
	bool linear = 0;
	bool PMTs = true;
#ifdef FAST_FIGURES_MODE
	linear = (in_is_linear == "lin");
	Nbins = in_Nbins;
	def_fit_option = in_def_fit_option;
	combined = (in_is_combined != "sep");
#endif //FAST_FIGURES_MODE

	bool fast_PMTs = false;
	//unsigned int PMT_used = 0x2 | 0x4 | 0x8; //PMT#1 is noisy
	unsigned int PMT_used = 0x8; //PMT#4 has the highest gain
	bool do_fit = true;
	bool fit_bad_forms = true;
	bool subtact_baseline = true;
	bool center_pulses = true;
	bool center_at_S1 = false; //Not used
	bool normalize_by_S1 = false; //Not used
	bool print_errors = false;
	double time_pretrigger_left = 7.0, time_pretrigger_right = 20.0;
	double time_left = -18, time_right = 178;//us
	double right_cutoff = 160;//us
	double max_val = 0;
	double trigger_at = center_at_S1 ? 47.18 : 47.18;
	double y_min = 1e-6;
	Nbins = (time_right-time_left)*Nbins/(160.0);

	pulse_shape* define = NULL, *copy = NULL;

	pulse_shape SiPM_16kV_87K_no_trigger;
	define = &SiPM_16kV_87K_no_trigger;
define->folder = std::string("210128/results_v5/Pu_46V_16kV_850V/forms_Alpha_peak/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 87K";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) : (min_baseline ? PAIR(15.28, 16.67) : PAIR(5.42, 6.66));
define->fast_t_center = 29.3;
define->fast_t = PAIR(21, 62.1);
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

pulse_shape SiPM_7_6kV_300K_no_trigger;
define = &SiPM_7_6kV_300K_no_trigger;
define->folder = std::string("210316/results_v2/Pu_55V_7.6kV_850V_300K/forms_Alpha_peak_v0/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 300K";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) : (min_baseline ? PAIR(20.36, 22.53) : PAIR(4.96, 6.50));
define->fast_t_center = 47.18;
define->fast_t = PAIR(26.2, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(80, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(1e-5, 3e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_256K_no_trigger;
define = &SiPM_7_6kV_256K_no_trigger;
define->folder = std::string("210316/results_v2/Pu_54V_7.6kV_800V_256K/forms_Alpha_peak_v0/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 256K";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) : (min_baseline ? PAIR(19.68, 21.05) : PAIR(3.72, 5.47));
define->fast_t_center = 47.18;
define->fast_t = PAIR(25.5, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(82, 138);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(5e-5, 1e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_200K_no_trigger;
define = &SiPM_7_6kV_200K_no_trigger;
define->folder = std::string("210311/results_v2/Pu_50V_7.6kV_800V_200K/forms_Alpha_peak_v0/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 200K";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) : (min_baseline ? PAIR(20.37, 22.68) : PAIR(4.50, 5.73));
define->fast_t_center = 47.18;
define->fast_t = PAIR(25.5, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(82, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(1e-4, 3e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_131K_no_trigger;
define = &SiPM_7_6kV_131K_no_trigger;
define->folder = std::string("210302/results_v3/Pu_46V_7.6kV_800V_131K/forms_Alpha_peak_v0/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 131K";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) : (min_baseline ? PAIR(16.16, 17.84) : PAIR(8.17, 8.79));
define->fast_t_center = 47.18;
define->fast_t = PAIR(25.5, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(85, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(1e-4, 3e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_120K_no_trigger;
define = &SiPM_7_6kV_120K_no_trigger;
define->folder = std::string("210302/results_v3/Pu_46V_7.6kV_800V_120K/forms_Alpha_peak_v0/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 120K";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) : (min_baseline ? PAIR(26.07, 27.00) : PAIR(14.67, 16.05));
define->fast_t_center = 63.05;
define->fast_t = PAIR(42.0, 95.9);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(105, 143);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(1e-4, 1e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape PMT4_16kV_87K_no_trigger;
define = &PMT4_16kV_87K_no_trigger;
define->folder = std::string("210128/results_v5/Pu_46V_16kV_850V/forms_Alpha_peak/");
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
define->device += ", 87K";
define->Td = "6.8";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) :
 			(fast_PMTs ? (min_baseline ? PAIR(9.26, 11.58) : PAIR(5.86, 8.18)) : (min_baseline ? PAIR(21.49, 23.81) : PAIR(15.30, 18.40)));
define->fast_t_center = 29.3;
define->fast_t = PAIR(21, 62.1);
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

pulse_shape PMT4_7_6kV_300K_no_trigger;
define = &PMT4_7_6kV_300K_no_trigger;
define->folder = std::string("210316/results_v2/Pu_55V_7.6kV_850V_300K/forms_Alpha_peak_v0/");
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
define->device += ", 300K";
define->Td = "6.8";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) :
 			(fast_PMTs ? (min_baseline ? PAIR(16.23, 18.55) : PAIR(5.55, 6.17)) : (min_baseline ? PAIR(10.19, 11.43) : PAIR(7.72, 9.88)));
define->fast_t_center = 47.18;
define->fast_t = PAIR(26.2, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(80, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(5e-6, 1e-4);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape PMT4_7_6kV_256K_no_trigger;
define = &PMT4_7_6kV_256K_no_trigger;
define->folder = std::string("210316/results_v2/Pu_54V_7.6kV_800V_256K/forms_Alpha_peak_v0/");
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
define->device += ", 256K";
define->Td = "6.8";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) :
 				(fast_PMTs ? (min_baseline ? PAIR(18.90, 19.79) : PAIR(4.62, 6.17)) : (min_baseline ? PAIR(8.80, 9.42) : PAIR(9.42, 10.96)));
define->fast_t_center = 47.18;
define->fast_t = PAIR(25.5, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(82, 138);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(5e-5, 1e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape PMT4_7_6kV_200K_no_trigger;
define = &PMT4_7_6kV_200K_no_trigger;
define->folder = std::string("210311/results_v2/Pu_50V_7.6kV_800V_200K/forms_Alpha_peak_v0/");
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
define->device += ", 200K";
define->Td = "6.8";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) :
 			(fast_PMTs ? (min_baseline ? PAIR(12.98, 14.22) : PAIR(21.64, 23.82)) : (min_baseline ? PAIR(18.40, 19.02) : PAIR(5.09, 7.10)));
define->fast_t_center = 47.18;
define->fast_t = PAIR(25.5, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(85, 143);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(3e-5, 1.17e-4);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape PMT4_7_6kV_131K_no_trigger;
define = &PMT4_7_6kV_131K_no_trigger;
define->folder = std::string("210302/results_v3/Pu_46V_7.6kV_800V_131K/forms_Alpha_peak_v0/");
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
define->device += ", 131K";
define->Td = "6.8";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) :
 			(fast_PMTs ? (min_baseline ? PAIR(12.20, 13.13) : PAIR(10.98, 11.90)) : (min_baseline ? PAIR(6.17, 6.79) : PAIR(20.10, 20.87)));
define->fast_t_center = 47.18;
define->fast_t = PAIR(25.5, 80.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(85, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(3e-5, 3e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape PMT4_7_6kV_120K_no_trigger;
define = &PMT4_7_6kV_120K_no_trigger;
define->folder = std::string("210302/results_v3/Pu_46V_7.6kV_800V_120K/forms_Alpha_peak_v0/");
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
define->device += ", 120K";
define->Td = "6.8";
define->baseline_t = avg_baseline ? PAIR(time_pretrigger_left, time_pretrigger_right) :
 			(fast_PMTs ? (min_baseline ? PAIR(23.82, 25.37) : PAIR(25.51, 26.91)) : (min_baseline ? PAIR(5.09, 7.25) : PAIR(38.21, 39.14)));
define->fast_t_center = 63.05;
define->fast_t = PAIR(42.0, 95.9);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->long_fit_t = PAIR(105, 143);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->long_ampl_bound = PAIR(1e-4, 1e-3);
define->long_tau_bound = PAIR(15, 200);
define->do_fit = do_fit;
define->fit_option = def_fit_option;

#ifdef FAST_FIGURES_MODE
	std::string folder = PMTs ? std::string("210316/results_v1/PMTs/") : std::string("210316/results_v1/SiPMs/");
	std::vector<pulse_shape> pulses;
	if (PMTs) {
		if (in_kV == "7.6")
			pulses.push_back((combined ? PMT4_7_6kV_200K_no_trigger :  PMT4_7_6kV_200K_no_trigger));
	} else {
		if (in_kV == "7.6")
			pulses.push_back((combined ? SiPM_7_6kV_200K_no_trigger :  SiPM_7_6kV_200K_no_trigger));
	}
#else //FAST_FIGURES_MODE

	//std::vector<pulse_shape> pulses = {SiPM_7_6kV_300K_no_trigger, SiPM_7_6kV_256K_no_trigger, SiPM_7_6kV_200K_no_trigger, SiPM_7_6kV_131K_no_trigger, SiPM_7_6kV_120K_no_trigger,  SiPM_16kV_87K_no_trigger};
	std::vector<pulse_shape> pulses = {PMT4_7_6kV_300K_no_trigger, PMT4_7_6kV_256K_no_trigger, PMT4_7_6kV_200K_no_trigger, PMT4_7_6kV_131K_no_trigger, PMT4_7_6kV_120K_no_trigger,  PMT4_16kV_87K_no_trigger};
	//std::vector<pulse_shape> pulses = {PMT4_7_6kV_120K_no_trigger};

#endif //FAST_FIGURES_MODE

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	//adsf - for fast Crtl + F
	std::string framename;
	if (PMTs)
		framename = std::string("Results for 4PMT (no WLS), ")+"5.6 MeV #alpha ^{238}Pu all tracks";
	else
		framename = std::string("Results for SiPM-matrix (no WLS), ")+"5.6 MeV #alpha ^{238}Pu all tracks";
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		pulses[hh].t_offset = center_pulses ?
			(trigger_at - (center_at_S1 ? pulses[hh].S1_t_center : pulses[hh].fast_t_center)) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff)
			read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], pulses[hh].t_offset);

    pulses[hh].hist->Sumw2();
		pulses[hh].baseline = average(pulses[hh].hist, pulses[hh].baseline_t.first + pulses[hh].t_offset, pulses[hh].baseline_t.second + pulses[hh].t_offset);
		if (pulses[hh].subtract_baseline) {
			subtract_baseline(pulses[hh].hist, pulses[hh].baseline);
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
		frame->GetYaxis()->SetRangeUser(0, 0.02);
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
			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
			if (contribution_by_fit)
				pulses[hh].long_integral = slow_int_f->Integral(pulses[hh].fast_t.second + Toff, right_cutoff); //! not right_cutoff+Toff!
			else
				pulses[hh].long_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.second + Toff, right_cutoff, pulses[hh].baseline);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);

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
