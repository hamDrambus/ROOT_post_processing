//Created on 2021.04.22
//Analyzing gaseous Ar only data for orthogonal tracks (selection by SiPMs or fPMTs fit)
//Slow component is obtained by subtraction of 300K pulse-shape from other temperatures
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
	TH1D* hist_to_subtract;
	TH1D* hist_result;
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

void renormalize_shape(pulse_shape& shape, double integral) {
	shape.hist->Scale(shape.scale/integral);
	shape.baseline *= shape.scale/integral;
	shape.baseline_bound = PAIR(shape.baseline + shape.baseline_bound.first, shape.baseline + shape.baseline_bound.second);
	shape.long_baseline_bound = PAIR(shape.baseline + shape.long_baseline_bound.first, shape.baseline + shape.long_baseline_bound.second);
}

void subtract_ref_hist(pulse_shape& shape) {
    for (int bin = 1, bin_end = shape.hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
			shape.hist_result->SetBinContent(bin, std::max(shape.hist->GetBinContent(bin) - shape.hist_to_subtract->GetBinContent(bin), 0.0));
    }
}

void draw_slow_component(TF1* fit_f, pulse_shape& shape)
{
	fit_f->SetNpx(800);
	fit_f->Draw("same");
}

int compare_forms5 (void) {
	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::string def_fit_option = "NRE";
	bool combined = true;
	double add_sigma = 0; //0, +-1
	int Nbins = 300;
	bool linear = 0;
	bool PMTs = false;
	bool fit_by_SiPMs = false;
	double slow_start_t_131K = 51.8; //51.8 +-1
	double t_normalize = 50; //us. Take amplitude at this time (shifted already)

	bool fast_PMTs = true;
	unsigned int PMT_used;
	if (!fast_PMTs) PMT_used = 0x2 | 0x4 | 0x8; //PMT#1 is noisy
	else PMT_used = 0x8; //PMT#4 as the highest gain
//PMT_used = 0x2 | 0x4 | 0x8;
	bool do_fit = true;
	bool fit_bad_forms = true;
	bool subtact_baseline = true;
	bool center_pulses = true;
	bool center_at_S1 = false; //Not used
	bool normalize_by_S1 = false; //Not used
	bool normalze_by_amplitude = false && center_pulses; //Use only when pulses are aligned
	bool draw_originals = false;
	bool draw_subtracted = true;
	bool print_errors = false;
	bool print_results = true && do_fit;
	double time_pretrigger_left = 7.0, time_pretrigger_right = 20.0;
	double time_left = -18, time_right = 178;//us
	double right_cutoff = 160;//us
	double max_val = 0;
	double trigger_at = 47.18; //for 131 and 256K
	double y_min = 1e-5;
	Nbins = (time_right-time_left)*Nbins/(160.0);

	pulse_shape* define = NULL, *copy = NULL;

	pulse_shape SiPM_16kV_87K_peak;
	define = &SiPM_16kV_87K_peak;
define->folder = std::string("210128/results_v6/Pu_46V_16kV_850V/") + "forms_Alpha_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 87K, Pu peak";
define->baseline_t = PAIR(4.77, 22.73);
define->fast_t_center = 29.3;
define->fast_t = PAIR(21, 31.5);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(31.5, 155);
define->long_fit_t = PAIR(31.5, 155);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_120K_orth;
define = &SiPM_7_6kV_120K_orth;
define->folder = std::string("210302/") + (fit_by_SiPMs ? "results_v4" : "results_v5") + "/Pu_46V_7.6kV_800V_120K/forms_Alpha_peak_v1/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 120K, orth.";
define->baseline_t = PAIR(5.50, 41.60);
define->fast_t_center = 63.05;
define->fast_t = PAIR(58.4, 64.2);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(68.0, 143);
define->long_fit_t = PAIR(68.0, 143);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(1e-4, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_131K_orth;
define = &SiPM_7_6kV_131K_orth;
define->folder = std::string("210302/") + (fit_by_SiPMs ? "results_v4" : "results_v5") + "/Pu_46V_7.6kV_800V_131K/forms_Alpha_peak_v1/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 131K, orth.";
define->baseline_t = PAIR(5.50, 25.36);
define->fast_t_center = 47.18 + 0.25;
define->fast_t = PAIR(41.8, 48.5);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(51.8, 140);
define->long_fit_t = PAIR(51.8, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(2.8e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(5e-5, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_200K_orth;
define = &SiPM_7_6kV_200K_orth;
define->folder = std::string("210311/") + (fit_by_SiPMs ? "results_v4" : "results_v5") + "/Pu_50V_7.6kV_800V_200K/forms_Alpha_peak_v1/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 200K, orth.";
define->baseline_t = PAIR(5.50, 25.0);
define->fast_t_center = 47.18;
define->fast_t = PAIR(41.66, 48.3);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(52.2, 140);
define->long_fit_t = PAIR(52.2, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-2, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(5e-5, 1e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_256K_orth;
define = &SiPM_7_6kV_256K_orth;
define->folder = std::string("210316/") + (fit_by_SiPMs ? "results_v3" : "results_v4") + "/Pu_54V_7.6kV_800V_256K/forms_Alpha_peak_v1/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 256K, orth.";
define->baseline_t = PAIR(6.63, 25.51);
define->fast_t_center = 47.18 - 0.15;
define->fast_t = PAIR(41.6, 47.6);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(52.5, 140);
define->long_fit_t = PAIR(52.5, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(5e-5, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

pulse_shape SiPM_7_6kV_300K_orth;
define = &SiPM_7_6kV_300K_orth;
define->folder = std::string("210316/") + (fit_by_SiPMs ? "results_v3" : "results_v4") + "/Pu_55V_7.6kV_850V_300K/forms_Alpha_peak_v1/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "6.8";
define->device = "SiPM-matrix, 300K, orth.";
define->baseline_t = PAIR(4.0, 25.51);
define->fast_t_center = 47.18;
define->fast_t = PAIR(25.5, slow_start_t_131K);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(52.5, 140);
define->long_fit_t = PAIR(52.5, 140);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-3, 1e-1);
define->slow_tau_bound = PAIR(2.5, 10);
define->long_ampl_bound = PAIR(5e-5, 1e-2);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

	std::vector<pulse_shape> pulses = {SiPM_7_6kV_120K_orth, SiPM_7_6kV_131K_orth, SiPM_7_6kV_200K_orth, SiPM_7_6kV_256K_orth};
	//std::vector<pulse_shape> pulses = {SiPM_7_6kV_120K_orth};
	pulse_shape reference_shape = SiPM_7_6kV_300K_orth;
	reference_shape.renormalize = false;

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	int contribution_est_method = 2; //0 - use fit of slow/long components at fast component range;
	//1 - use constant with amplitude from fit; 2 - use linear function rising up to amplitude from fit;
	//adsf - for fast Crtl + F
	std::string framename;
	if (PMTs)
		framename = std::string("Results for 4PMT (no WLS), ")+"5.6 MeV #alpha ^{238}Pu" + (fit_by_SiPMs ? " (fit by SiPMs)" : " (fit by fPMTs)");
	else
		framename = std::string("Results for SiPM-matrix (no WLS), ")+"5.6 MeV #alpha ^{238}Pu" + (fit_by_SiPMs ? " (fit by SiPMs)" : " (fit by fPMTs)");
	{
		std::string hist_name = "hist_ref.";
		reference_shape.hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		reference_shape.t_offset = center_pulses ? (trigger_at - (center_at_S1 ? reference_shape.S1_t_center : reference_shape.fast_t_center)) : 0;
		for (int ff = 0, ff_end_ = reference_shape.fnames.size(); ff!=ff_end_; ++ff)
			read_hist_w(reference_shape.hist, reference_shape.folder + reference_shape.fnames[ff], reference_shape.t_offset);

		reference_shape.hist->Sumw2();
		reference_shape.baseline = average(reference_shape.hist, time_pretrigger_left + reference_shape.t_offset, time_pretrigger_right + reference_shape.t_offset);
		reference_shape.baseline_sigma = stdev(reference_shape.hist, reference_shape.baseline_t.first + reference_shape.t_offset, reference_shape.baseline_t.second + reference_shape.t_offset);
		if (reference_shape.subtract_baseline) {
			subtract_baseline(reference_shape.hist, reference_shape.baseline + add_sigma * reference_shape.baseline_sigma);
			reference_shape.baseline = 0;
		}
		double integral = 1.0;
		if (reference_shape.renormalize && !normalze_by_amplitude) {
			if (normalize_by_S1)
				integral = integrate(reference_shape.hist, reference_shape.S1_t.first + reference_shape.t_offset, reference_shape.S1_t.second + reference_shape.t_offset);
			else
				integral = integrate(reference_shape.hist, reference_shape.fast_t.first + reference_shape.t_offset, reference_shape.fast_t.second + reference_shape.t_offset);
		}
		if(reference_shape.renormalize && normalze_by_amplitude) {
			integral = reference_shape.hist->Interpolate(t_normalize);
		}
		renormalize_shape(reference_shape, integral);
		reference_shape.max_val = reference_shape.hist->GetBinContent(reference_shape.hist->GetMaximumBin());
	}
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		hist_name = "hist" + std::to_string(hh) + "to_substract";
		pulses[hh].hist_to_subtract = (TH1D*) reference_shape.hist->Clone();
		hist_name = "hist" + std::to_string(hh) + "residue";
		pulses[hh].hist_result = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		pulses[hh].t_offset = center_pulses ?
			(trigger_at - (center_at_S1 ? pulses[hh].S1_t_center : pulses[hh].fast_t_center)) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff)
			read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], pulses[hh].t_offset);

    pulses[hh].hist->Sumw2();
		pulses[hh].baseline = average(pulses[hh].hist, time_pretrigger_left + pulses[hh].t_offset, time_pretrigger_right + pulses[hh].t_offset);
		pulses[hh].baseline_sigma = stdev(pulses[hh].hist, pulses[hh].baseline_t.first + pulses[hh].t_offset, pulses[hh].baseline_t.second + pulses[hh].t_offset);
		if (pulses[hh].subtract_baseline) {
			subtract_baseline(pulses[hh].hist, pulses[hh].baseline + add_sigma * pulses[hh].baseline_sigma);
			pulses[hh].baseline = 0;
		}
		double integral = 1.0, integral_ref = 1.0;
		if (pulses[hh].renormalize && !normalze_by_amplitude) {
			if (normalize_by_S1) {
				integral = integrate(pulses[hh].hist, pulses[hh].S1_t.first + pulses[hh].t_offset, pulses[hh].S1_t.second + pulses[hh].t_offset);
				integral_ref = integrate(pulses[hh].hist_to_subtract, pulses[hh].S1_t.first + pulses[hh].t_offset, pulses[hh].S1_t.second + pulses[hh].t_offset);
			}
			else {
				integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
				integral_ref = integrate(pulses[hh].hist_to_subtract, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
			}
		}
		if (pulses[hh].renormalize && normalze_by_amplitude) {
			integral = pulses[hh].hist->Interpolate(t_normalize);
			integral_ref = pulses[hh].hist_to_subtract->Interpolate(t_normalize);
		}
		renormalize_shape(pulses[hh], integral);
		pulses[hh].hist_to_subtract->Scale(pulses[hh].scale/integral_ref);
		subtract_ref_hist(pulses[hh]);
		pulses[hh].max_val = pulses[hh].hist->GetBinContent(pulses[hh].hist->GetMaximumBin());
		pulses[hh].max_val = std::max(pulses[hh].hist_to_subtract->GetBinContent(pulses[hh].hist_to_subtract->GetMaximumBin()), pulses[hh].max_val);
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
		frame->GetXaxis()->SetRangeUser(0, 160);
	//=====================================
	frame->GetYaxis()->SetTitle("PE peak count");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineColor(palette_major[hh%palette_major.size()]);
		pulses[hh].hist_to_subtract->SetLineColor(palette_major[(hh+1)%palette_major.size()]);
		if (!draw_originals)
			pulses[hh].hist_result->SetLineColor(palette_major[hh%palette_major.size()]);
		else
			pulses[hh].hist_result->SetLineColor(palette_major[(hh+2)%palette_major.size()]);
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist_to_subtract->SetLineWidth(2);
		pulses[hh].hist_result->SetLineWidth(2);
		//pulses[hh].no_long_hist->SetLineStyle(7);//9
		if (draw_originals) {
			pulses[hh].hist->Draw("hist Lsame");
			pulses[hh].hist_to_subtract->Draw("hist Lsame");
		}
		if (draw_subtracted)
			pulses[hh].hist_result->Draw("hist Lsame");
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

			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, right_cutoff, pulses[hh].baseline);
			pulses[hh].slow_integral = integrate(pulses[hh].hist_result, pulses[hh].fast_t.first + Toff, right_cutoff, pulses[hh].baseline);
			pulses[hh].slow_on_fast = 0;
			pulses[hh].tau1 = "--";
			pulses[hh].tau1_err = "--";
			pulses[hh].tau2 = "--";
			pulses[hh].tau2_err = "--";

			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
			pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
			pulses[hh].Fr2 = "--";
			pulses[hh].err2 = "--";

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
				add_text(80, 0.0035, no_title, tau1, palette_major);
				add_text(108, 0.002, Slow_title, frsS, palette_major);
				add_text(123, 0.002, Long_title, frsL, palette_major);
				add_text(139, 0.002, no_title, tau2, palette_major);
			}
		} else {
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
			std::vector<std::string> Long_title;// = {"Long"};
			add_text(16, 0.3, no_title, tau1, palette_major);
			add_text(30, 0.3, Slow_title, frsS, palette_major);
			//add_text(52, 0.08, Long_title, frsL, palette_text);
			//add_text(58, 0.08, no_title, tau2, palette_text);
		}
	}
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		if (draw_originals) {
			legend->AddEntry(pulses[hh].hist, (std::string("E/N = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");
			legend->AddEntry(pulses[hh].hist_to_subtract, (std::string("E/N = ") + reference_shape.Td + " Td, " + reference_shape.device).c_str(), "l");
		}
		if (draw_subtracted)
			legend->AddEntry(pulses[hh].hist_result, (std::string("E/N = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");
	}

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
  return 0;
}
