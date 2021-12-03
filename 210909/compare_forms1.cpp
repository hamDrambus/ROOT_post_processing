//Created on 2021.09.21
//For 210909/results_v5, same as 210902/compare_forms1.cpp
//11.0 mm with 238Pu set on the cathode. (Mesh instead of THGEM0, 28% CERN #8 as THEGEM1, PMT#1 malfunctions due to mixed signal wires)
#define PAIR std::pair<double, double>

std::string strtoken(std::string &in, std::string break_symbs)
{
 std::string out;
 while (!in.empty())
 {
   char a = in.front();
   in.erase(in.begin());
   bool break_ = false;
   for (auto h = break_symbs.begin(); h != break_symbs.end(); ++h)
     if (a == *h) {
       break_ = true;
       break;
     }
   if ((break_) && (out.empty()))
     continue;
   if (break_)
     return out;
   out.push_back(a);
 }
 return out;
}

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
	fit_f->SetNpx(800);
	fit_f->Draw("same");
}


int compare_forms1 (void) {
	std::cout<<"COMPARISON_MODE"<<std::endl;

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::string def_fit_option = "NLWRE";
	bool combined = true;
	bool Cd_peak = false;
	int Nbins = 150;
	bool linear = 0;
	bool PMTs = true;

	bool fast_PMTs = true;
	unsigned int PMT_used = 0x2 | 0x4 | 0x8;
	bool do_fit = true;
	bool fit_bad_forms = true;
	bool subtact_baseline = true;
	bool center_pulses = false;
	bool center_at_S1 = false; //Not used
	bool normalize_by_S1 = false; //Not used
	bool print_errors = true;
	bool print_results = true;
	double time_pretrigger_left = 4.0, time_pretrigger_right = 17.0;
	double time_left = 0, time_right = 160;//us
	double max_val = 0;
	double trigger_at = center_at_S1 ? 0 : 32; //Not used
	double y_min = 1e-5;

	pulse_shape* define = NULL, *copy = NULL;

	pulse_shape SiPM_17_3kV_no_trigger;
	define = &SiPM_17_3kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_17.3kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.5";
define->device = "SiPM-matrix";
define->fast_t_center = 28.9;
define->fast_t = PAIR(22.0, 30.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(30.8, 154);
define->long_fit_t = PAIR(30.8, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-2, 1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_17_3kV_no_trigger_v2;
	define = &SiPM_17_3kV_no_trigger_v2;
	copy = &SiPM_17_3kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(32.0, 40.0);
define->long_fit_t = PAIR(73, 154);
define->baseline_bound = PAIR(1e-6, 3e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-1);
define->long_tau_bound = PAIR(17, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_15_6kV_no_trigger;
	define = &SiPM_15_6kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_15.6kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "7.6";
define->device = "SiPM-matrix";
define->fast_t_center = 28.9;
define->fast_t = PAIR(22.0, 30.8);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(31.9, 154);
define->long_fit_t = PAIR(31.9, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 9e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(6e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_15_6kV_no_trigger_v2;
	define = &SiPM_15_6kV_no_trigger_v2;
	copy = &SiPM_15_6kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.5, 41.0);
define->long_fit_t = PAIR(70, 154);
define->baseline_bound = PAIR(1e-6, 2.4e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 2e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(6e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_13_8kV_no_trigger;
	define = &SiPM_13_8kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_13.8kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix";
define->fast_t_center = 28.8;
define->fast_t = PAIR(22, 31.3);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.0, 154);
define->long_fit_t = PAIR(32.0, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(6e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_13_8kV_no_trigger_v2;
	define = &SiPM_13_8kV_no_trigger_v2;
	copy = &SiPM_13_8kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.6, 41.9);
define->long_fit_t = PAIR(71, 154);
define->baseline_bound = PAIR(1e-6, 1.9e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(6e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_12_1kV_no_trigger;
	define = &SiPM_12_1kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_12.1kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.9";
define->device = "SiPM-matrix";
define->fast_t_center = 28.9;
define->fast_t = PAIR(22, 31.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(31.8, 154);
define->long_fit_t = PAIR(31.8, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(6e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_12_1kV_no_trigger_v2;
	define = &SiPM_12_1kV_no_trigger_v2;
	copy = &SiPM_12_1kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.1, 40.7);
define->long_fit_t = PAIR(67, 154);
define->baseline_bound = PAIR(1e-6, 1.0e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 2.8e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(6e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_11_2kV_no_trigger;
	define = &SiPM_11_2kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_11.2kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.5";
define->device = "SiPM-matrix";
define->fast_t_center = 29.0;
define->fast_t = PAIR(22, 31.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.0, 154);
define->long_fit_t = PAIR(32.0, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(3e-4, 7e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_11_2kV_no_trigger_v2;
	define = &SiPM_11_2kV_no_trigger_v2;
	copy = &SiPM_11_2kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.0, 40.2);
define->long_fit_t = PAIR(65, 154);
define->baseline_bound = PAIR(1e-6, 9e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 2.1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(3e-4, 7e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_10_4kV_no_trigger;
	define = &SiPM_10_4kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_10.4kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.1";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 32.3);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.7, 154);
define->long_fit_t = PAIR(32.7, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(5e-3, 7e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 4e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_10_4kV_no_trigger_v2;
	define = &SiPM_10_4kV_no_trigger_v2;
	copy = &SiPM_10_4kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.5, 39.9);
define->long_fit_t = PAIR(65, 154);
define->baseline_bound = PAIR(1e-6, 7e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(5e-3, 1.25e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 4e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_09_5kV_no_trigger;
	define = &SiPM_09_5kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_09.5kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "4.6";
define->device = "SiPM-matrix";
define->fast_t_center = 29.0;
define->fast_t = PAIR(22, 32.7);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.2, 154);
define->long_fit_t = PAIR(33.2, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 2e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_09_5kV_no_trigger_v2;
	define = &SiPM_09_5kV_no_trigger_v2;
	copy = &SiPM_09_5kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.7, 41.6);
define->long_fit_t = PAIR(62, 154);
define->baseline_bound = PAIR(1e-6, 4e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 6.9e-3);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_08_6kV_no_trigger;
	define = &SiPM_08_6kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_08.6kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "4.2";
define->device = "SiPM-matrix";
define->fast_t_center = 29.0;
define->fast_t = PAIR(22, 32.8);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.0, 154);
define->long_fit_t = PAIR(33.0, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_08_6kV_no_trigger_v2;
	define = &SiPM_08_6kV_no_trigger_v2;
	copy = &SiPM_08_6kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(34.0, 44.5);
define->long_fit_t = PAIR(56, 154);
define->baseline_bound = PAIR(1e-6, 2.8e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape SiPM_07_8kV_no_trigger;
	define = &SiPM_07_8kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_07.8kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.8";
define->device = "SiPM-matrix";
define->fast_t_center = 29.0;
define->fast_t = PAIR(22, 34.0);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.0, 154);
define->long_fit_t = PAIR(33.0, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape SiPM_07_8kV_no_trigger_v2;
	define = &SiPM_07_8kV_no_trigger_v2;
	copy = &SiPM_07_8kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(35.3, 47.0);
define->long_fit_t = PAIR(63, 154);
define->baseline_bound = PAIR(1e-6, 2.9e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

pulse_shape SiPM_06_9kV_no_trigger;
define = &SiPM_06_9kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_06.9kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.4";
define->device = "SiPM-matrix";
define->fast_t_center = 28.7;
define->fast_t = PAIR(22, 33.1);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.5, 154);
define->long_fit_t = PAIR(33.5, 154);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(8e-5, 1e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_06_9kV_no_trigger_v2;
define = &SiPM_06_9kV_no_trigger_v2;
copy = &SiPM_06_9kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(36.1, 48.0);
define->long_fit_t = PAIR(66, 154);
define->baseline_bound = PAIR(1e-6, 2.0e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(8e-5, 1e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;


	pulse_shape PMT4_17_3kV_no_trigger;
	define = &PMT4_17_3kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_17.3kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "8.5";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 28.9;
define->fast_t = PAIR(23.0, 30.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.7, 155);
define->long_fit_t = PAIR(32.7, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_17_3kV_no_trigger_v2;
	define = &PMT4_17_3kV_no_trigger_v2;
	copy = &PMT4_17_3kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(34.0, 39.0);
define->long_fit_t = PAIR(70, 155);
define->baseline_bound = PAIR(1e-6, 3.8e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_15_6kV_no_trigger;
	define = &PMT4_15_6kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_15.6kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "7.6";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 28.8;
define->fast_t = PAIR(23.0, 30.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.9, 155);
define->long_fit_t = PAIR(32.9, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_15_6kV_no_trigger_v2;
	define = &PMT4_15_6kV_no_trigger_v2;
	copy = &PMT4_15_6kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(34.4, 41.3);
define->long_fit_t = PAIR(71, 155);
define->baseline_bound = PAIR(1e-6, 3.3e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_13_8kV_no_trigger;
	define = &PMT4_13_8kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_13.8kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "6.8";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23.0, 31.2);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.2, 155);
define->long_fit_t = PAIR(33.2, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_13_8kV_no_trigger_v2;
	define = &PMT4_13_8kV_no_trigger_v2;
	copy = &PMT4_13_8kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(34.4, 40.8);
define->long_fit_t = PAIR(73, 155);
define->baseline_bound = PAIR(1e-6, 2.1e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 2.6e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-3, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_12_1kV_no_trigger;
	define = &PMT4_12_1kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_12.1kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "5.9";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 28.9;
define->fast_t = PAIR(23.0, 31.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.3, 155);
define->long_fit_t = PAIR(33.3, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 6e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(3e-4, 5e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_12_1kV_no_trigger_v2;
	define = &PMT4_12_1kV_no_trigger_v2;
	copy = &PMT4_12_1kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(34.5, 40.8);
define->long_fit_t = PAIR(65, 155);
define->baseline_bound = PAIR(1e-6, 1e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1.4e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(3e-4, 5e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_11_2kV_no_trigger;
	define = &PMT4_11_2kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_11.2kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "5.5";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 28.9;
define->fast_t = PAIR(23.0, 32.2);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.9, 155);
define->long_fit_t = PAIR(33.9, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(3e-3, 2e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(3e-4, 4e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_11_2kV_no_trigger_v2;
	define = &PMT4_11_2kV_no_trigger_v2;
	copy = &PMT4_11_2kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(35.4, 40.0);
define->long_fit_t = PAIR(62, 155);
define->baseline_bound = PAIR(1e-6, 9e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(3e-3, 2e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(3e-4, 4e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_10_4kV_no_trigger;
	define = &PMT4_10_4kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_10.4kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "5.1";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 29.1;
define->fast_t = PAIR(23.0, 32.4);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(34.3, 155);
define->long_fit_t = PAIR(34.3, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(3e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 3e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_10_4kV_no_trigger_v2;
	define = &PMT4_10_4kV_no_trigger_v2;
	copy = &PMT4_10_4kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(35.5, 40.5);
define->long_fit_t = PAIR(60, 155);
define->baseline_bound = PAIR(1e-6, 8e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(3e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 3e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_09_5kV_no_trigger;
	define = &PMT4_09_5kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_09.5kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "4.6";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 29.0;
define->fast_t = PAIR(23.0, 32.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.3, 155);
define->long_fit_t = PAIR(33.3, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 8e-4);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_09_5kV_no_trigger_v2;
	define = &PMT4_09_5kV_no_trigger_v2;
	copy = &PMT4_09_5kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(34.2, 39.3);
define->long_fit_t = PAIR(49, 155);
define->baseline_bound = PAIR(1e-6, 4.3e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 8e-4);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_08_6kV_no_trigger;
	define = &PMT4_08_6kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_08.6kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "4.2";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 29.1;
define->fast_t = PAIR(23.0, 33.2);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.5, 155);
define->long_fit_t = PAIR(33.5, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 6e-3);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 8e-4);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_08_6kV_no_trigger_v2;
	define = &PMT4_08_6kV_no_trigger_v2;
	copy = &PMT4_08_6kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.3, 41.0);
define->long_fit_t = PAIR(46, 155);
define->baseline_bound = PAIR(1e-6, 6.7e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 6e-3);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 8e-4);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_07_8kV_no_trigger;
	define = &PMT4_07_8kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_07.8kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "3.8";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 29.0;
define->fast_t = PAIR(23.0, 32.8);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.3, 155);
define->long_fit_t = PAIR(33.3, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 8e-3);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 1.3e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_07_8kV_no_trigger_v2;
	define = &PMT4_07_8kV_no_trigger_v2;
	copy = &PMT4_07_8kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(33.7, 40.7);
define->long_fit_t = PAIR(47, 155);
define->baseline_bound = PAIR(1e-6, 8.9e-4);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2e-3, 8e-3);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 1.3e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	pulse_shape PMT4_06_9kV_no_trigger;
	define = &PMT4_06_9kV_no_trigger;
define->folder = std::string("210909/results_v5/Pu_46V_06.9kV_850V/") + (Cd_peak ? "forms_Pu_peak/" : "forms_Pu_left/");
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "3.4";
if(PMT_used == (0x1|0x2|0x4|0x8))
define->device = "4PMT ";
else
define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->fast_t_center = 28.8;
define->fast_t = PAIR(23.0, 32.8);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(33.3, 155);
define->long_fit_t = PAIR(33.3, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(3e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	pulse_shape PMT4_06_9kV_no_trigger_v2;
	define = &PMT4_06_9kV_no_trigger_v2;
	copy = &PMT4_06_9kV_no_trigger;
*define = *copy;
define->slow_fit_t = PAIR(34.8, 46.0);
define->long_fit_t = PAIR(47, 155);
define->baseline_bound = PAIR(1e-6, 1e-3);
define->long_baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(3e-3, 1e-2);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;

	std::string folder = PMTs ? std::string("210909/results_v5/PMTs_v1/")
					: std::string("210909/results_v5/SiPMs_v1/");
	//std::vector<pulse_shape> pulses = {SiPM_17_3kV_no_trigger, SiPM_15_6kV_no_trigger, SiPM_13_8kV_no_trigger, SiPM_12_1kV_no_trigger, SiPM_11_2kV_no_trigger};
	//std::vector<pulse_shape> pulses = {SiPM_10_4kV_no_trigger, SiPM_09_5kV_no_trigger, SiPM_08_6kV_no_trigger, SiPM_07_8kV_no_trigger, SiPM_06_9kV_no_trigger};
	//std::vector<pulse_shape> pulses = {SiPM_17_3kV_no_trigger_v2, SiPM_15_6kV_no_trigger_v2, SiPM_13_8kV_no_trigger_v2, SiPM_12_1kV_no_trigger_v2, SiPM_11_2kV_no_trigger_v2};
	//std::vector<pulse_shape> pulses = {SiPM_10_4kV_no_trigger_v2, SiPM_09_5kV_no_trigger_v2, SiPM_08_6kV_no_trigger_v2, SiPM_07_8kV_no_trigger_v2, SiPM_06_9kV_no_trigger_v2};

	std::vector<pulse_shape> pulses = {PMT4_17_3kV_no_trigger, PMT4_15_6kV_no_trigger, PMT4_13_8kV_no_trigger, PMT4_12_1kV_no_trigger, PMT4_11_2kV_no_trigger};
	//std::vector<pulse_shape> pulses = {PMT4_10_4kV_no_trigger, PMT4_09_5kV_no_trigger, PMT4_08_6kV_no_trigger, PMT4_07_8kV_no_trigger, PMT4_06_9kV_no_trigger};
	//std::vector<pulse_shape> pulses = {PMT4_17_3kV_no_trigger_v2, PMT4_15_6kV_no_trigger_v2, PMT4_13_8kV_no_trigger_v2, PMT4_12_1kV_no_trigger_v2, PMT4_11_2kV_no_trigger_v2};
	//std::vector<pulse_shape> pulses = {PMT4_10_4kV_no_trigger_v2, PMT4_09_5kV_no_trigger_v2, PMT4_08_6kV_no_trigger_v2, PMT4_07_8kV_no_trigger_v2, PMT4_06_9kV_no_trigger_v2};

	//std::vector<pulse_shape> pulses = {PMT4_06_9kV_no_trigger};
	//For paper and reports:
	//std::vector<pulse_shape> pulses = {PMT4_20kV_no_trigger, PMT4_20kV_800V_no_trigger};
	//std::vector<pulse_shape> pulses = {PMT4_20kV_no_trigger, PMT4_16kV_no_trigger, PMT4_14kV_no_trigger, PMT4_13kV_no_trigger, PMT4_11kV_no_trigger};
	//std::vector<pulse_shape> pulses = {SiPM_20kV_no_trigger, SiPM_16kV_no_trigger, SiPM_14kV_no_trigger, SiPM_13kV_no_trigger, SiPM_11kV_no_trigger};

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	int contribution_est_method = 2; //0 - use fit of slow/long components at fast component range;
	//1 - use constant with amplitude from fit; 2 - use linear function rising up to amplitude from fit;
	//adsf - for fast Crtl + F
	std::string framename;
	if (PMTs)
		framename = std::string("Results for 3PMT (no WLS), ")+(Cd_peak ? "" : "< ")+"5.6 MeV #alpha ^{238}Pu";
	else
		framename = std::string("Results for SiPM-matrix (no WLS), ")+(Cd_peak ? "" : "< ")+"5.6 MeV #alpha ^{238}Pu";
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		hist_name = "hist" + std::to_string(hh) + "no_long";
		pulses[hh].no_long_hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		hist_name = "hist" + std::to_string(hh) + "residue";
		pulses[hh].t_offset = center_pulses ?
			(trigger_at - (center_at_S1 ? pulses[hh].S1_t_center : pulses[hh].fast_t_center)) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff)
			read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], pulses[hh].t_offset);

    pulses[hh].hist->Sumw2();
		pulses[hh].baseline = average(pulses[hh].hist, time_pretrigger_left + pulses[hh].t_offset, time_pretrigger_right + pulses[hh].t_offset);
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
			pulses[hh].long_baseline_bound = PAIR(0.8*pulses[hh].baseline, 1.2*pulses[hh].baseline);
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
	else
		frame->GetXaxis()->SetRangeUser(10, 70);
	//=====================================
	frame->GetYaxis()->SetTitle("PE peak count");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist->SetLineColor(palette_major[hh]);
		pulses[hh].no_long_hist->SetLineWidth(2);
		pulses[hh].no_long_hist->SetLineStyle(7);//9
		pulses[hh].no_long_hist->SetLineColor(palette_major[hh+1]);
		pulses[hh].hist->Draw("hist Lsame");
    }

	int precision1 = 2, precision2 = 0, precision3 = 2, precision4 = 3;
	int line_width = 3;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].total_integral = 0;
		pulses[hh].fast_integral = 0;
		pulses[hh].restored_integral = 0;
		pulses[hh].slow_integral = 0;
		pulses[hh].long_integral = 0;
		pulses[hh].slow_on_fast = 0;
		pulses[hh].long_on_fast = 0;
		double Toff = pulses[hh].t_offset;
		if (!pulses[hh].do_fit) {
			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);
			pulses[hh].slow_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
			pulses[hh].slow_on_fast = integrate(pulses[hh].hist, pulses[hh].fast_t.second + Toff, 2*pulses[hh].fast_t.second - pulses[hh].fast_t.first + Toff, pulses[hh].baseline);
			pulses[hh].tau1 = "--";
			pulses[hh].tau1_err = "--";
			pulses[hh].tau2 = "--";
			pulses[hh].tau2_err = "--";

			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
			pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
			pulses[hh].Fr2 = "--";
			pulses[hh].err2 = "--";
		} else {
		if (!pulses[hh].simultaneous_fit) {//first, fit long comp. only by tail, then slow component
			TF1* fit_f = NULL;
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {//no long component fit
				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";
				pulses[hh].Fr2 = "--";
				pulses[hh].err2 = "--";
			} else {
				fit_f = new TF1("fit2", FittingF_exp, pulses[hh].long_fit_t.first + Toff, pulses[hh].long_fit_t.second + Toff, 4);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
				SetParLimits(fit_f, 0, pulses[hh].slow_fit_t.first + Toff);
				SetParLimits(fit_f, 1, pulses[hh].long_baseline_bound.first, pulses[hh].long_baseline_bound.second);
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
				pulses[hh].long_integral = slow_int_f->Integral(pulses[hh].fast_t.second + Toff, time_right + Toff);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].long_on_fast = integrate_function_bound(pulses[hh].hist, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].long_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].long_on_fast = integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
			}
			bool long_exist = (fit_f != NULL);
			fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + Toff, pulses[hh].slow_fit_t.second + Toff, 4);
			pulses[hh].fit_fs.push_back(fit_f);
			fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
			SetParLimits(fit_f, 0, pulses[hh].slow_fit_t.first + Toff);
			SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
			SetParLimits(fit_f, 2, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
			SetParLimits(fit_f, 3, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
			fit_f->SetLineColor(palette_minor[hh]);
			fit_f->SetLineWidth(line_width);
    	pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
			draw_slow_component(fit_f, pulses[hh]);
			pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
			pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
			if (!long_exist) {
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
			}
			TF1* slow_int_f = new TF1(*fit_f);
			slow_int_f->FixParameter(1, 0);

			pulses[hh].slow_integral = slow_int_f->Integral(pulses[hh].fast_t.second + Toff, time_right + Toff);
			if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
				pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
			if (1==contribution_est_method)
				pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
			if (2==contribution_est_method)
				pulses[hh].slow_on_fast = integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second, pulses[hh].baseline);
			if (!long_exist) {
				pulses[hh].long_integral = pulses[hh].baseline * (pulses[hh].slow_fit_t.second - pulses[hh].fast_t.second);
				pulses[hh].long_on_fast = pulses[hh].baseline * (pulses[hh].fast_t.second - pulses[hh].fast_t.first);
			}
			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
			pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
			pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
			pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
		} else {
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {//no long component fit
				TF1* fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + Toff, pulses[hh].slow_fit_t.second + Toff, 4);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
				SetParLimits(fit_f, 0, pulses[hh].slow_fit_t.first + Toff);
				SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				fit_f->SetLineWidth(line_width);
	    	pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, pulses[hh]);
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";

				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
				TF1* slow_int_f = new TF1(*fit_f);
				slow_int_f->FixParameter(1, 0);

				pulses[hh].slow_integral = slow_int_f->Integral(pulses[hh].fast_t.second + Toff, time_right + Toff);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].slow_on_fast = integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second, pulses[hh].baseline);
				pulses[hh].long_integral = pulses[hh].baseline * (pulses[hh].slow_fit_t.second - pulses[hh].fast_t.second);
				pulses[hh].long_on_fast = pulses[hh].baseline * (pulses[hh].fast_t.second - pulses[hh].fast_t.first);
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			} else {
				TF1* fit_f = new TF1("fit12", FittingF_2exp, pulses[hh].slow_fit_t.first + Toff, pulses[hh].long_fit_t.second + Toff, 6);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "amplitude1", "#tau1", "amplitude2", "#tau2");
				SetParLimits(fit_f, 0, pulses[hh].slow_fit_t.first + Toff);
				SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
				SetParLimits(fit_f, 4, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
				SetParLimits(fit_f, 5, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				fit_f->SetLineWidth(line_width);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, pulses[hh]);
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
				pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(5), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(5), precision2);
				TF1* slow_f = new TF1(*fit_f);
				slow_f->FixParameter(1, 0);
				slow_f->FixParameter(4, 0);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, slow_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].slow_on_fast = integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				TF1* long_f = new TF1(*fit_f);
				long_f->FixParameter(1, 0);
				long_f->FixParameter(2, 0);
				pulses[hh].long_integral = long_f->Integral(pulses[hh].fast_t.second + Toff, time_right + Toff);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].long_on_fast = integrate_function_bound(pulses[hh].hist, long_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].long_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(4), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].long_on_fast = integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(4), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				pulses[hh].slow_integral = pulses[hh].total_integral - pulses[hh].long_integral - pulses[hh].fast_integral;
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			}
		}
		}
		std::cout<<"Results for "<<pulses[hh].folder<<"\""<<pulses[hh].device<<"\":"<<std::endl;
		std::cout<<"Fit option: \""<<pulses[hh].fit_option<<"\""<<std::endl;
		std::cout<<"Check Sum: Fast="<<pulses[hh].fast_integral<<" + Slow=" <<pulses[hh].slow_integral<<" + Long=" <<pulses[hh].long_integral << " == "<<std::endl;
		std::cout<<"\t\t"<<pulses[hh].fast_integral+pulses[hh].slow_integral+pulses[hh].long_integral << " vs Total= "<< pulses[hh].total_integral <<std::endl;
		std::cout<<"#tau1\t#tau1_err\tFr1\tFr1_err\t#tau2\t#tau2_err\tFr2\tFr2_err"<<std::endl;
		std::cout<<pulses[hh].tau1<<"\t"<<pulses[hh].tau1_err<<"\t"<<pulses[hh].Fr1<<"\t"<<pulses[hh].err1<<"\t"
			<<pulses[hh].tau2<<"\t"<<pulses[hh].tau2_err<<"\t"<<pulses[hh].Fr2<<"\t"<<pulses[hh].err2<<std::endl;
  }
	std::vector<std::string> tau1, tau2, frsS, frsL;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string emp = "";
		std::string unit = "#mus";
		if (pulses[hh].tau1.empty() || pulses[hh].tau1 == "--")
			tau1.push_back(emp);
		else
			tau1.push_back("#tau_{S}=" + pulses[hh].tau1 + (print_errors ? "#pm" + pulses[hh].tau1_err : unit));
		if (pulses[hh].tau2.empty() || pulses[hh].tau2 == "--")
			tau2.push_back("");
		else
			tau2.push_back("#tau_{L}=" + pulses[hh].tau2  + (print_errors ? "#pm" + pulses[hh].tau2_err : unit));
		if (pulses[hh].Fr1.empty() || pulses[hh].Fr1 == "--")
			frsS.push_back("");
		else
			frsS.push_back(pulses[hh].Fr1 + (print_errors ? "#pm" + pulses[hh].err1 : emp));
		if (pulses[hh].Fr2.empty() || pulses[hh].Fr2 == "--")
			frsL.push_back("");
		else
			frsL.push_back(pulses[hh].Fr2 + (print_errors ? "#pm" + pulses[hh].err2 : emp));
	}
	if (print_results) {
		if (!linear) {
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Contribution:", "Slow"};
			std::vector<std::string> Long_title = {"","Long"};
			if (print_errors) {
				//zcxv
				add_text(46, 0.02, no_title, tau1, palette_major);
				add_text(76, 0.005, Slow_title, frsS, palette_major);
				add_text(100, 0.005, Long_title, frsL, palette_major);
				add_text(129, 0.005, no_title, tau2, palette_major);
			} else {
				add_text(55, 0.01, no_title, tau1, palette_major);
				add_text(80, 0.006, Slow_title, frsS, palette_major);
				add_text(92, 0.006, Long_title, frsL, palette_major);
				add_text(110, 0.006, no_title, tau2, palette_major);
			}
			std::cout<<"tau1:"<<std::endl;
			for (std::size_t i = tau1.size() - 1; i!=-1; --i) {
				std::string res = tau1[i];
				std::string number = strtoken(res, "=");
				number = strtoken(res, "#pm");
				std::cout<<number<<"\t";
				number = strtoken(res, "m");
				number = strtoken(res, "#mus");
				std::cout<<number<<std::endl;
			}
			std::cout<<"Slow fraction:"<<std::endl;
			for (std::size_t i = frsS.size() - 1; i!=-1; --i) {
				std::string res = frsS[i];
				std::string number = strtoken(res, "#pm");
				std::cout<<number<<std::endl;
			}
			std::cout<<"tau2:"<<std::endl;
			for (std::size_t i = tau2.size() - 1; i!=-1; --i) {
				std::string res = tau2[i];
				std::string number = strtoken(res, "=");
				number = strtoken(res, "#pm");
				std::cout<<number<<"\t";
				number = strtoken(res, "m");
				number = strtoken(res, "#mus");
				std::cout<<number<<std::endl;
			}
			std::cout<<"Long fraction:"<<std::endl;
			for (std::size_t i = frsL.size() - 1; i!=-1; --i) {
				std::string res = frsL[i];
				std::string number = strtoken(res, "#pm");
				std::cout<<number<<std::endl;
			}
		} else {
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
			std::vector<std::string> Long_title;// = {"Long"};
			add_text(42, 0.15, no_title, tau1, palette_major);
			add_text(52, 0.15, Slow_title, frsS, palette_major);
			//add_text(52, 0.08, Long_title, frsL, palette_text);
			//add_text(58, 0.08, no_title, tau2, palette_text);
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
