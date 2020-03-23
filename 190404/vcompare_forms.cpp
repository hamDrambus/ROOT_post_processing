#define PAIR std::pair<double, double>

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
		f->SetParLimits(n, lower, upper);
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
	double x0 = par[0] - par[2] * std::log((par[2] + par[4])/par[2]);
	double rise = (1-std::exp((x0-x[0])/par[2]));
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
	double x0 = par[0] - par[2] * std::log((par[2] + par[4])/par[2]);
	double rise = (1-std::exp((x0-x[0])/par[2]));
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
		std::cout<<"Size="<<entries.size()<<", offset = "<<offset<<std::endl;
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
	double scale;
	bool subtract_baseline;
	bool renormalize;
	//Fit:
	PAIR slow_fit_t;
	PAIR long_fit_t;
	PAIR baseline_bound;
	PAIR raise_tau_bound;
	PAIR slow_ampl_bound;
	PAIR slow_tau_bound;
	PAIR long_ampl_bound;
	PAIR long_tau_bound;
	double rising_time;
	double max_time;
	bool simultaneous_fit; //long+slow simultaneously or not
	bool do_fit;
	std::string fit_option; //"NWL" or "N"
//OUTPUT:
	TH1D* hist;
	TH1D* fast_hist;
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
	temp->Draw("same");
}

int vcompare_forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700; //adsf - for fast Ctrl+F
	std::string def_fit_option = "NWLRE";
	bool fit_bad_forms = true;
	bool subtact_baseline = true;
	pulse_shape* define = NULL;

	pulse_shape SiPM_20kV_no_trigger;
	define = &SiPM_20kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.5";
define->device = "SiPM-matrix";
define->fast_t_center = 28.68;
define->fast_t = PAIR(22, 30.83);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->max_time = 29.7;
define->slow_fit_t = PAIR(31.7, 160);
define->long_fit_t = PAIR(31.7, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->raise_tau_bound = PAIR(5, 5);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(20, 200);
define->rising_time = 50;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_18kV_no_trigger;
	define = &SiPM_18kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "7.6";
define->device = "SiPM-matrix";
define->fast_t_center = 28.91;
define->fast_t = PAIR(22, 31.0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(31.6, 160);
define->long_fit_t = PAIR(31.6, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_16kV_no_trigger;
	define = &SiPM_16kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 31.3);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(31.8, 160);
define->long_fit_t = PAIR(31.8, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(40, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_14kV_no_trigger;
	define = &SiPM_14kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.9";
define->device = "SiPM-matrix";
define->fast_t_center = 29.2;
define->fast_t = PAIR(22, 31.9);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.1, 160);
define->long_fit_t = PAIR(32.1, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_12kV_no_trigger;
	define = &SiPM_12kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.1";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.5);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.6, 46);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_10kV_no_trigger;
	define = &SiPM_10kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "4.2";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 47);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 5e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape SiPM_8kV_no_trigger;
	define = &SiPM_8kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.4";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 50);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

/*pulse_shape SiPM_20kV_no_trigger_v2;
	define = &SiPM_20kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.5";
define->device = "SiPM-matrix";
define->fast_t_center = 28.68;
define->fast_t = PAIR(22, 30.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.3, 45);
define->long_fit_t = PAIR(85, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_18kV_no_trigger_v2;
	define = &SiPM_18kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "7.6";
define->device = "SiPM-matrix";
define->fast_t_center = 28.91;
define->fast_t = PAIR(22, 31.0);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 43.6);
define->long_fit_t = PAIR(80, 160);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_16kV_no_trigger_v2;
	define = &SiPM_16kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 31.3);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.7, 41);
define->long_fit_t = PAIR(70, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(40, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_14kV_no_trigger_v2;
	define = &SiPM_14kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.9";
define->device = "SiPM-matrix";
define->fast_t_center = 29.2;
define->fast_t = PAIR(22, 31.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.0, 41);
define->long_fit_t = PAIR(60, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(2, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_12kV_no_trigger_v2;
	define = &SiPM_12kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.1";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.5);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.5, 37.4);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-3);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_10kV_no_trigger_v2;
	define = &SiPM_10kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "4.2";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(35.5, 45.3);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 5e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape SiPM_8kV_no_trigger_v2;
	define = &SiPM_8kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.4";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(38.4, 47);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape SiPM_20kV_no_trigger_left_slope;
	define = &SiPM_20kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.5";
define->device = "SiPM-matrix";
define->fast_t_center = 28.68;
define->fast_t = PAIR(22, 30.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(31.7, 160);
define->long_fit_t = PAIR(31.7, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_18kV_no_trigger_left_slope;
	define = &SiPM_18kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "7.6";
define->device = "SiPM-matrix";
define->fast_t_center = 28.91;
define->fast_t = PAIR(22, 31.0);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(31.6, 160);
define->long_fit_t = PAIR(31.6, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_16kV_no_trigger_left_slope;
	define = &SiPM_16kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 31.3);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(31.8, 160);
define->long_fit_t = PAIR(31.8, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(40, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_14kV_no_trigger_left_slope;
	define = &SiPM_14kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.9";
define->device = "SiPM-matrix";
define->fast_t_center = 29.2;
define->fast_t = PAIR(22, 31.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.1, 160);
define->long_fit_t = PAIR(32.1, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_12kV_no_trigger_left_slope;
	define = &SiPM_12kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.1";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.5);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.6, 46);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_10kV_no_trigger_left_slope;
	define = &SiPM_10kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "4.2";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 47);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 5e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape SiPM_8kV_no_trigger_left_slope;
	define = &SiPM_8kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.4";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 50);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape SiPM_20kV_no_trigger_v2_left_slope;
	define = &SiPM_20kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.5";
define->device = "SiPM-matrix";
define->fast_t_center = 28.68;
define->fast_t = PAIR(22, 30.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.3, 45);
define->long_fit_t = PAIR(85, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_18kV_no_trigger_v2_left_slope;
	define = &SiPM_18kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "7.6";
define->device = "SiPM-matrix";
define->fast_t_center = 28.91;
define->fast_t = PAIR(22, 31.0);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 43.6);
define->long_fit_t = PAIR(80, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_16kV_no_trigger_v2_left_slope;
	define = &SiPM_16kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 31.3);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.7, 41);
define->long_fit_t = PAIR(70, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(40, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_14kV_no_trigger_v2_left_slope;
	define = &SiPM_14kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.9";
define->device = "SiPM-matrix";
define->fast_t_center = 29.2;
define->fast_t = PAIR(22, 31.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.0, 41);
define->long_fit_t = PAIR(60, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(2, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_12kV_no_trigger_v2_left_slope;
	define = &SiPM_12kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "5.1";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.5);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.5, 37.4);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-3);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_10kV_no_trigger_v2_left_slope;
	define = &SiPM_10kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "4.2";
define->device = "SiPM-matrix";
define->fast_t_center = 29.4;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(35.5, 45.3);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 5e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape SiPM_8kV_no_trigger_v2_left_slope;
	define = &SiPM_8kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_right/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.4";
define->device = "SiPM-matrix";
define->fast_t_center = 29.1;
define->fast_t = PAIR(22, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(38.0, 47.2);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(2, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;*/

	pulse_shape PMT4_20kV_no_trigger;
	define = &PMT4_20kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "8.5";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.4);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.5, 160);
define->long_fit_t = PAIR(32.5, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 1);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_18kV_no_trigger;
	define = &PMT4_18kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "7.6";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.8);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.0, 160);
define->long_fit_t = PAIR(32.0, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_16kV_no_trigger;
	define = &PMT4_16kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "6.8";
define->device = "4PMT";
define->fast_t_center = 29.0;
define->fast_t = PAIR(23, 31.1);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.4, 160);
define->long_fit_t = PAIR(32.4, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_14kV_no_trigger;
	define = &PMT4_14kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.9";
define->device = "4PMT";
define->fast_t_center = 29.3;
define->fast_t = PAIR(23, 31.9);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(34.2, 160);
define->long_fit_t = PAIR(34.2, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_12kV_no_trigger;
	define = &PMT4_12kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.1";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.4);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(34.0, 46);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_10kV_no_trigger;
	define = &PMT4_10kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "4.2";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(35.5, 54);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape PMT4_8kV_no_trigger;
	define = &PMT4_8kV_no_trigger;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "3.4";
define->device = "4PMT";
define->fast_t_center = 29.2;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 51.0);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

/*pulse_shape PMT4_20kV_no_trigger_v2;
	define = &PMT4_20kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "8.5";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.4);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.5, 43);
define->long_fit_t = PAIR(85, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 1);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_18kV_no_trigger_v2;
	define = &PMT4_18kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "7.6";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.0, 40);
define->long_fit_t = PAIR(70, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_16kV_no_trigger_v2;
	define = &PMT4_16kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "6.8";
define->device = "4PMT";
define->fast_t_center = 29.0;
define->fast_t = PAIR(23, 31.1);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(34, 42);
define->long_fit_t = PAIR(70, 160);
define->baseline_bound = PAIR(1e-5, 1e-3);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->rising_time = 1;
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_14kV_no_trigger_v2;
	define = &PMT4_14kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.9";
define->device = "4PMT";
define->fast_t_center = 29.3;
define->fast_t = PAIR(23, 31.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(34.2, 42);
define->long_fit_t = PAIR(65, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_12kV_no_trigger_v2;
	define = &PMT4_12kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.1";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.4);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(35.3, 42);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_10kV_no_trigger_v2;
	define = &PMT4_10kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "4.2";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(36.2, 44);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape PMT4_8kV_no_trigger_v2;
	define = &PMT4_8kV_no_trigger_v2;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "3.4";
define->device = "4PMT";
define->fast_t_center = 29.2;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(39, 47);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape PMT4_20kV_no_trigger_left_slope;
	define = &PMT4_20kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "8.5";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.4);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.5, 160);
define->long_fit_t = PAIR(32.5, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 1);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_18kV_no_trigger_left_slope;
	define = &PMT4_18kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "7.6";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.0, 160);
define->long_fit_t = PAIR(32.0, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_16kV_no_trigger_left_slope;
	define = &PMT4_16kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "6.8";
define->device = "4PMT";
define->fast_t_center = 29.0;
define->fast_t = PAIR(23, 31.1);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.4, 160);
define->long_fit_t = PAIR(32.4, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_14kV_no_trigger_left_slope;
	define = &PMT4_14kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.9";
define->device = "4PMT";
define->fast_t_center = 29.3;
define->fast_t = PAIR(23, 31.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(34.2, 160);
define->long_fit_t = PAIR(34.2, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_12kV_no_trigger_left_slope;
	define = &PMT4_12kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.1";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.4);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(34.0, 46);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_10kV_no_trigger_left_slope;
	define = &PMT4_10kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "4.2";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 47);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape PMT4_8kV_no_trigger_left_slope;
	define = &PMT4_8kV_no_trigger_left_slope;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "3.4";
define->device = "4PMT";
define->fast_t_center = 29.2;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(32.8, 51.0);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 1e-2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

pulse_shape PMT4_20kV_no_trigger_v2_left_slope;
	define = &PMT4_20kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_20kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "8.5";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.4);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.5, 43);
define->long_fit_t = PAIR(85, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 1);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_18kV_no_trigger_v2_left_slope;
	define = &PMT4_18kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_18kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "7.6";
define->device = "4PMT";
define->fast_t_center = 28.7;
define->fast_t = PAIR(23, 30.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(33.0, 40);
define->long_fit_t = PAIR(70, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_16kV_no_trigger_v2_left_slope;
	define = &PMT4_16kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_16kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "6.8";
define->device = "4PMT";
define->fast_t_center = 29.0;
define->fast_t = PAIR(23, 31.1);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(34, 42);
define->long_fit_t = PAIR(70, 160);
define->baseline_bound = PAIR(1e-5, 1e-3);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_14kV_no_trigger_v2_left_slope;
	define = &PMT4_14kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_14kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.9";
define->device = "4PMT";
define->fast_t_center = 29.3;
define->fast_t = PAIR(23, 31.9);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(34.2, 42);
define->long_fit_t = PAIR(65, 160);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_12kV_no_trigger_v2_left_slope;
	define = &PMT4_12kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_12kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "5.1";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.4);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(35.3, 42);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 1e-1);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_10kV_no_trigger_v2_left_slope;
	define = &PMT4_10kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_10kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "4.2";
define->device = "4PMT";
define->fast_t_center = 29.4;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(36.2, 44);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;

	pulse_shape PMT4_8kV_no_trigger_v2_left_slope;
	define = &PMT4_8kV_no_trigger_v2_left_slope;
define->folder = "190404/results_v1/Cd_46V_8kV_850V/forms_Cd_right/";
define->fnames = {"8_form_by_S.hdata", "9_form_by_S.hdata", "10_form_by_S.hdata", "11_form_by_S.hdata"};
define->Td = "3.4";
define->device = "4PMT";
define->fast_t_center = 29.2;
define->fast_t = PAIR(23, 32.8);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(39, 47);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-5, 2e-4);
define->slow_ampl_bound = PAIR(1e-3, 2);
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = false;
define->do_fit = fit_bad_forms;
define->fit_option = def_fit_option;*/

	std::vector<pulse_shape> pulses = {SiPM_20kV_no_trigger};

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	int Nbins = 1800;
	bool center_pulses = false;
	bool print_errors = true;
  double time_pretrigger_left = 7, time_pretrigger_right = 20;
  double time_left = 0, time_right = 160;//us
  double max_val = 0;
	double trigger_at = 32;
	bool linear = true;
	double y_min = 1e-4;
	//qewr - for fast Crtl + F
	std::string framename = std::string("Results for SiPM-matrix (no WLS in setup), 88 keV #gamma ^{109}Cd");// + " " + Tds[0] + " Td";

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
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
	frame->GetXaxis()->SetTitle("Time [#mus]");
	frame->GetYaxis()->SetTitle("PE peak counts");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist->SetLineColor(palette_major[hh]);
		pulses[hh].fast_hist->SetLineWidth(2);
		pulses[hh].fast_hist->SetLineStyle(7);//9
		pulses[hh].fast_hist->SetLineColor(palette_major[hh]);
		pulses[hh].hist->Draw("hist Lsame");
  }

	int precision1 = 2, precision2 = 0, precision3 = 2, precision4 = 2;
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
			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
			pulses[hh].total_integral -= pulses[hh].baseline * (time_right - pulses[hh].fast_t.first);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
			pulses[hh].fast_integral -= pulses[hh].baseline * (pulses[hh].fast_t.second - pulses[hh].fast_t.first);
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
		if (!pulses[hh].simultaneous_fit) {
			TF1* fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].slow_fit_t.second + pulses[hh].t_offset, 5);
			pulses[hh].fit_fs.push_back(fit_f);
			fit_f->SetParNames("start_time", "y0", "#tau_{R}" "amplitude", "#tau");
			SetParLimits(fit_f, 0, pulses[hh].max_time + pulses[hh].t_offset);
			SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
			SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
			SetParLimits(fit_f, 3, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
			SetParLimits(fit_f, 4, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
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
			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
			pulses[hh].total_integral -= pulses[hh].baseline * (time_right - pulses[hh].fast_t.first);
			pulses[hh].slow_integral = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
			pulses[hh].slow_on_fast = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
			pulses[hh].fast_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {
				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = "--";
				pulses[hh].err2 = "--";
			} else {
				fit_f = new TF1("fit2", FittingF_exp, pulses[hh].long_fit_t.first + pulses[hh].t_offset, pulses[hh].long_fit_t.second + pulses[hh].t_offset, 5);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "#tau_{R}" "amplitude", "#tau");
				SetParLimits(fit_f, 0, pulses[hh].max_time + pulses[hh].t_offset);
				SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
				SetParLimits(fit_f, 4, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				fit_f->SetLineWidth(line_width);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, pulses[hh]);
				subtract_component(pulses[hh].fast_hist, pulses[hh].hist, fit_f);
				pulses[hh].fast_hist->Draw("hist Lsame");
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(4), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(4), precision2);
				TF1* slow_int_f = new TF1(*fit_f);
				slow_int_f->FixParameter(1, 0);
				pulses[hh].long_integral = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
				pulses[hh].long_on_fast = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);

				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			}
		} else {
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {
				TF1* fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].slow_fit_t.second + pulses[hh].t_offset, 5);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "#tau_{R}", "amplitude", "#tau");
				SetParLimits(fit_f, 0, pulses[hh].max_time + pulses[hh].t_offset);
				SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				SetParLimits(fit_f, 4, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
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
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
				pulses[hh].total_integral -= pulses[hh].baseline * (time_right - pulses[hh].fast_t.first);
				pulses[hh].slow_integral = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, time_right + pulses[hh].t_offset);
				pulses[hh].slow_on_fast = slow_int_f->Integral(pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
				pulses[hh].fast_integral = pulses[hh].total_integral - pulses[hh].fast_integral;

				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = "--";
				pulses[hh].err2 = "--";
			} else {
				TF1* fit_f = new TF1("fit1", FittingF_2exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].long_fit_t.second + pulses[hh].t_offset, 7);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "#tau_{R}", "amplitude1", "#tau1", "amplitude2", "#tau2");
				SetParLimits(fit_f, 0, pulses[hh].max_time + pulses[hh].t_offset);
				SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				SetParLimits(fit_f, 2, pulses[hh].raise_tau_bound.first, pulses[hh].raise_tau_bound.second);
				SetParLimits(fit_f, 3, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				SetParLimits(fit_f, 4, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
				SetParLimits(fit_f, 5, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
				SetParLimits(fit_f, 6, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				fit_f->SetLineWidth(line_width);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, pulses[hh]);
				subtract_component(pulses[hh].fast_hist, pulses[hh].hist, fit_f);
				pulses[hh].fast_hist->Draw("hist Lsame");
				pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
				pulses[hh].total_integral -= pulses[hh].baseline * (time_right - pulses[hh].fast_t.first);
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
		std::cout<<"Fit option: \""<<pulses[hh].fit_option<<"\""<<std::endl;
		std::cout<<"#tau1\t#tau1_err\tFr1\tFr1_err\t#tau2\t#tau2_err\tFr2\tFr2_err"<<std::endl;
		std::cout<<pulses[hh].tau1<<"\t"<<pulses[hh].tau1_err<<"\t"<<pulses[hh].Fr1<<"\t"<<pulses[hh].err1<<"\t"
			<<pulses[hh].tau2<<"\t"<<pulses[hh].tau2_err<<"\t"<<pulses[hh].Fr2<<"\t"<<pulses[hh].err2<<std::endl;
    }
	std::vector<std::string> tau1, tau2, frsS, frsL;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string emp = "";
		if (pulses[hh].tau1.empty() || pulses[hh].tau1 == "--")
			tau1.push_back(emp);
		else
			tau1.push_back("#tau_{S}=" + pulses[hh].tau1 + (print_errors ? "#pm" + pulses[hh].tau1_err : emp));
		if (pulses[hh].tau2.empty() || pulses[hh].tau2 == "--")
			tau2.push_back("");
		else
			tau2.push_back("#tau_{L}=" + pulses[hh].tau2  + (print_errors ? "#pm" + pulses[hh].tau2_err : emp));
		if (pulses[hh].Fr1.empty() || pulses[hh].Fr1 == "--")
			frsS.push_back("");
		else
			frsS.push_back(pulses[hh].Fr1 + (print_errors ? "#pm" + pulses[hh].err1 : emp));
		if (pulses[hh].Fr2.empty() || pulses[hh].Fr2 == "--")
			frsL.push_back("");
		else
			frsL.push_back(pulses[hh].Fr2 + (print_errors ? "#pm" + pulses[hh].err2 : emp));
	}
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
			add_text(45.8, 0.04, no_title, tau1, palette_major);
			add_text(68, 0.008, Slow_title, frsS, palette_major);
			add_text(85, 0.008, Long_title, frsL, palette_major);
			add_text(103, 0.008, no_title, tau2, palette_major);
		}
	} else {
		std::vector<std::string> no_title;
		std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
		std::vector<std::string> Long_title;// = {"Long"};
		add_text(36, 0.15, no_title, tau1, palette_major);
		add_text(46, 0.15, Slow_title, frsS, palette_major);
		//add_text(52, 0.08, Long_title, frsL, palette_text);
		//add_text(58, 0.08, no_title, tau2, palette_text);
	}

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh)
		legend->AddEntry(pulses[hh].hist, (std::string("E/N = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
