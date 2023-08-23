#define PAIR std::pair<double, double>

std::string dbl_to_str (double val, int precision)
{
	std::stringstream ss;
	ss<<std::fixed<<std::setprecision(precision)<<val;
	return ss.str();
}

std::string int_to_str(int num, std::size_t decimals=0)
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

Double_t FittingF_exp (Double_t *x, Double_t *par) {
  //par[0] - start time (fixed), related to par[2]
  //par[1] - offset
  //par[2] - amplitude
  //par[3] - tau
  return par[1] + par[2]*std::exp((par[0]-x[0])/par[3]);
}

struct pulse_shape {
//INPUT:
	std::string Td;
	std::string name;
	double kV;
	double T_drift;
	double W_pass;
	double decay_tau;
	int event_n;
	double t_offset;
	double jitter;
	double lambda;

	PAIR fast_t; //fast component start&finish time, used for signal normalization
	double scale;
	bool renormalize;
	//Fit:
	PAIR slow_fit_t;
	PAIR baseline_bound;
	PAIR slow_ampl_bound;
	PAIR slow_tau_bound;
	bool do_fit;
	std::string fit_option; //"NLWRE" or "NRE"
//OUTPUT:
	TRandom *generator;
	TH1D* hist;
	TH1D* hist_n_delays;
	double max_val;

	double baseline;
	double total_integral;
	double fast_integral;
	double slow_integral;
	double slow_on_fast; //for systematic error estimation
	std::string tau1, tau1_err;
	std::string Fr1, err1;
};


double prob_to_length (double W, double on_length) { //length is drift time here, not distance
	//W is probability to NOT interact while passing interval on_length
	return -1*on_length/std::log(W);
}

double generate_interaction_point (double int_length, TRandom * generator) {
	//int_length - interaction length (time in this context, which is proportional to the actual length)
	return generator->Exp(int_length);
}

double generate_delay (double delay_tau, TRandom * generator) {
	return generator->Exp(delay_tau);
}

std::vector<PAIR> generate_event(double drift_time, double pass_probability, double delay_tau, TRandom * generator) {
	std::vector<PAIR> out;
	double resudial = drift_time;
	double int_length = prob_to_length(pass_probability, drift_time);
	double position = 0;
	while (resudial > 0) {
		double dL = generate_interaction_point(int_length, generator);
		//std::cout<<"dL="<<dL<<", Tdrift="<<drift_time<<std::endl;
		if (dL > resudial) {
			dL = resudial;
			resudial = 0;
		} else {
			resudial -= dL;
		}
		out.push_back(PAIR(position, position + dL));
		double delay = generate_delay(delay_tau, generator);
		position += delay + dL;
		//std::cout<<"delay="<<delay<<", position="<<position<<std::endl;
		//std::cout<<"residual="<<resudial<<std::endl;
	}
	return out;
}

void FillHist(TH1D *hist, std::vector<PAIR>& event) {
	//std::cout<<"event.size()="<<event.size()<<std::endl;
	for (int i = 0, i_end_ = event.size(); i!=i_end_; ++i) {
		for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
			double x = hist->GetBinCenter(bin);
			if ((event[i].first <= x) && (x < event[i].second)) {
				hist->SetBinContent(bin, hist->GetBinContent(bin) + 1);
			}
		}
	}
}

void FillHist_Ndelays(TH1D *hist, std::vector<PAIR>& event) {
	hist->Fill(std::max((int)event.size() - 1, 0));
}

void Simulate(pulse_shape &info) {
	for (int e = 0; e!=info.event_n; ++e) {
		std::vector<PAIR> event = generate_event(info.T_drift, info.W_pass, info.decay_tau, info.generator);
		double jitter = info.generator->Gaus(info.t_offset, info.jitter);
		for (int i = 0, i_end_ = event.size(); i!=i_end_; ++i) {
			event[i].first+= jitter;
			event[i].second+= jitter;
		}
		FillHist(info.hist, event);
		FillHist_Ndelays(info.hist_n_delays, event);
	}
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

int e_trapping_simulation (void) {

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::string def_fit_option = "NRE";
	bool combined = true;
	int Nbins = 5000;
	bool linear = 1;
	double def_jitter = 0.0;
	int event_n = 100000;
	int seed = 42;

	bool draw_n_delays = false;
	bool do_renormalize = true;
	bool do_fit = false;
	bool fit_bad_forms = true;
	bool print_errors = false;
	bool print_results = false;
	double time_pretrigger = 10;
	double time_left = 0, time_right = 60;//us
	double max_val = 0;
	double y_min = 1e-4;

	double L_0 = 18; //mm - project gap thickness, corresponds to the data_Fr
	double L_simulation = 18; //mm
	double lambda = 20.0; //mm Wpass = exp(-lambda/L_simulation);

	int W_precision = 1;
	int tau_precision = 1;
	int L_precision = 0;

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	int contribution_est_method = 0; //0 - use fit of slow/long components at fast component range;
	//1 - use constant with amplitude from fit; 2 - use linear function rising up to amplitude from fit;

	pulse_shape* define = NULL, *copy = NULL;
pulse_shape _20kV_00;
define = &_20kV_00;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.4;
	define->decay_tau = 4.0;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 40);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _20kV_01;
define = &_20kV_01;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.5;
	define->decay_tau = 4.0;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 40);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _20kV_02;
define = &_20kV_02;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.3;
	define->decay_tau = 4.0;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 40);
	define->baseline_bound = PAIR(1e-7, 1e-5);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _20kV_03;
define = &_20kV_03;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.3;
	define->decay_tau = 3.5;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 40);
	define->baseline_bound = PAIR(1e-7, 1e-5);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _20kV_04;
define = &_20kV_04;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.3;
	define->decay_tau = 4.5;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 40);
	define->baseline_bound = PAIR(1e-7, 1e-5);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _20kV_05;
define = &_20kV_05;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.273;
	define->decay_tau = 3.47;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 32);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 9);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _20kV_06;
define = &_20kV_06;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.25;
	define->decay_tau = 3.3;
	define->t_offset = time_pretrigger;
	define->jitter = 0.3;
	define->event_n = event_n;
	define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
			", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus, #sigma=" + dbl_to_str(define->jitter, 1)+"#mus";
	define->fast_t = PAIR(8, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.61 + define->T_drift, 35);
	define->baseline_bound = PAIR(1e-7, 1e-5);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _20kV_07;
define = &_20kV_07;
	define->Td = "8.5";
	define->kV = 20;
	define->T_drift = 2.2 * L_simulation / L_0;
	define->W_pass = 0.335;
	define->decay_tau = 3.52;
	define->t_offset = time_pretrigger;
	define->jitter = 0.6;
	define->event_n = event_n;
	define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
			", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus, #sigma=" + dbl_to_str(define->jitter, 1)+"#mus";
	define->fast_t = PAIR(7.8, 11.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(11.11 + define->T_drift, 35);
	define->baseline_bound = PAIR(1e-7, 1e-5);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _18kV_00;
define = &_18kV_00;
	define->Td = "7.6";
	define->kV = 18;
	define->T_drift = 2.5 * L_simulation / L_0;
	define->W_pass = 0.354;
	define->decay_tau = 3.47;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.22 + define->T_drift, 27.5);
	define->baseline_bound = PAIR(1e-7, 1e-5);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(3, 10);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _16kV_00;
define = &_16kV_00;
	define->Td = "6.8";
	define->kV = 16;
	define->T_drift = 2.8 * L_simulation / L_0;
	define->W_pass = 0.450;
	define->decay_tau = 3.47;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.22 + define->T_drift, 29.1);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(3, 8);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _14kV_00;
define = &_14kV_00;
	define->Td = "5.9";
	define->kV = 14;
	define->T_drift = 3.2 * L_simulation / L_0;
	define->W_pass = 0.588;
	define->decay_tau = 3.47;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + "#mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + "mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.22 + define->T_drift, 29.5);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2.5, 8);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _12kV_00;
define = &_12kV_00;
	define->Td = "5.1";
	define->kV = 12;
	define->T_drift = 3.7 * L_simulation / L_0;
	define->W_pass = 0.741;
	define->decay_tau = 3.47;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = define->Td + " Td, W="+dbl_to_str(define->W_pass, W_precision) +
				", #tau="+dbl_to_str(define->decay_tau, tau_precision) + " #mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + " mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.22 + define->T_drift, 29.9);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2.5, 8);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

// 210128 data
pulse_shape _19_0kV_new_00;
define = &_19_0kV_new_00;
	define->Td = "8.0";
	define->kV = 19;
	define->T_drift = 2.3 * L_simulation / L_0;
	define->lambda = 17.0;
	define->W_pass = exp(-L_simulation/define->lambda);
	define->decay_tau = 3.1;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = "E/N = "+ define->Td + " Td, l = "+dbl_to_str(define->lambda, W_precision) +
				", #tau = "+dbl_to_str(define->decay_tau, tau_precision) + " #mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + " mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 32);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 9);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _19_0kV_new_01;
define = &_19_0kV_new_01;
	define->Td = "8.0";
	define->kV = 19;
	define->T_drift = 2.3 * L_simulation / L_0;
	define->lambda = 17.0;
	define->W_pass = exp(-L_simulation/define->lambda);
	define->decay_tau = 3.1;
	define->t_offset = time_pretrigger;
	define->jitter = 0.5;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = "E/N = "+ define->Td + " Td, l = "+dbl_to_str(define->lambda, W_precision) +
				", #tau = "+dbl_to_str(define->decay_tau, tau_precision) + " #mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(1, L_precision) + " mm";
	define->fast_t = PAIR(9.9, 10.91 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.91 + define->T_drift, 32);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 9);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _16_0kV_new_00;
define = &_16_0kV_new_00;
	define->Td = "6.7";
	define->kV = 16;
	define->T_drift = 2.8 * L_simulation / L_0;
	define->lambda = 27.0;
	define->W_pass = exp(-L_simulation/define->lambda);
	define->decay_tau = 3.1;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = "E/N = "+ define->Td + " Td, l = "+dbl_to_str(define->lambda, W_precision) +
				", #tau = "+dbl_to_str(define->decay_tau, tau_precision) + " #mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + " mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 32);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 9);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _16_0kV_new_01;
define = &_16_0kV_new_01;
	define->Td = "6.7";
	define->kV = 16;
	define->T_drift = 2.8 * L_simulation / L_0;
	define->lambda = 27.0;
	define->W_pass = exp(-L_simulation/define->lambda);
	define->decay_tau = 3.1;
	define->t_offset = time_pretrigger;
	define->jitter = 0.5;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = "E/N = "+ define->Td + " Td, l = "+dbl_to_str(define->lambda, W_precision) +
				", #tau = "+dbl_to_str(define->decay_tau, tau_precision) + " #mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + " mm";
	define->fast_t = PAIR(9.9, 10.91 + define->T_drift);
	define->scale = 1.1956; //1.1956;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.91 + define->T_drift, 32);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 9);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _13_0kV_new_00;
define = &_13_0kV_new_00;
	define->Td = "5.5";
	define->kV = 16;
	define->T_drift = 3.4 * L_simulation / L_0;
	define->lambda = 57.0;
	define->W_pass = exp(-L_simulation/define->lambda);
	define->decay_tau = 3.1;
	define->t_offset = time_pretrigger;
	define->jitter = 0;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = "E/N = "+ define->Td + " Td, l = "+dbl_to_str(define->lambda, W_precision) +
				", #tau = "+dbl_to_str(define->decay_tau, tau_precision) + " #mus";
	else
		define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + " mm";
	define->fast_t = PAIR(9.9, 10.01 + define->T_drift);
	define->scale = 1.0;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.11 + define->T_drift, 32);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 9);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

pulse_shape _13_0kV_new_01;
define = &_13_0kV_new_01;
	define->Td = "5.5";
	define->kV = 16;
	define->T_drift = 3.4 * L_simulation / L_0;
	define->lambda = 57.0;
	define->W_pass = exp(-L_simulation/define->lambda);
	define->decay_tau = 3.1;
	define->t_offset = time_pretrigger;
	define->jitter = 0.5;
	define->event_n = event_n;
	if (L_simulation==L_0)
		define->name = "E/N = "+ define->Td + " Td, l = "+dbl_to_str(define->lambda, W_precision) +
					", #tau = "+dbl_to_str(define->decay_tau, tau_precision) + " #mus";
		else
			define->name = define->Td + " Td, EL gap="+dbl_to_str(L_simulation, L_precision) + " mm";
	define->fast_t = PAIR(9.9, 10.91 + define->T_drift);
	define->scale = 1.4376; //1.4376;
	define->renormalize = do_renormalize;
	define->slow_fit_t = PAIR(10.91 + define->T_drift, 32);
	define->baseline_bound = PAIR(1e-7, 1e-7);
	define->slow_ampl_bound = PAIR(1e-3, 1);
	define->slow_tau_bound = PAIR(2, 9);
	define->do_fit = do_fit;
	define->fit_option = def_fit_option;
	define->W_pass = std::exp(std::log(define->W_pass)*L_simulation/L_0);

	//adsf - for fast Ctrl+F
	//std::vector<pulse_shape> pulses = {_20kV_01, _20kV_00, _20kV_02};
	//std::vector<pulse_shape> pulses = {_20kV_03, _20kV_02, _20kV_04};
	//std::vector<pulse_shape> pulses = {_20kV_05};
	//std::vector<pulse_shape> pulses = {_20kV_05, _20kV_06, _20kV_07};
	//std::vector<pulse_shape> pulses = {_18kV_00, _18kV_01, _18kV_02};
	//std::vector<pulse_shape> pulses = {_18kV_00};
	//std::vector<pulse_shape> pulses = {_16kV_00};
	//std::vector<pulse_shape> pulses = {_14kV_00};
	//std::vector<pulse_shape> pulses = {_12kV_00};
	//std::vector<pulse_shape> pulses = {_20kV_05, _18kV_00, _16kV_00, _14kV_00, _12kV_00};

	// 210128
	std::vector<pulse_shape> pulses = {_19_0kV_new_01, _16_0kV_new_01, _13_0kV_new_01};
	//std::vector<pulse_shape> pulses = {_19_0kV_new_00, _16_0kV_new_00, _13_0kV_new_00};

	std::string framename = std::string("S2 pulse-shapes in simple simulation, Nevents=") + int_to_str(event_n);
	std::string framename2 = std::string("Number of delays during drift, Nevents=") + int_to_str(event_n);
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		hist_name = "hist_n_delays" + std::to_string(hh);
		pulses[hh].hist_n_delays = new TH1D(hist_name.c_str(), hist_name.c_str(), 10, 0, 10);
		pulses[hh].generator = new TRandom1(seed);
		Simulate(pulses[hh]);
		pulses[hh].t_offset = 0.0;
  }

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
    pulses[hh].hist->Sumw2();
		if (pulses[hh].renormalize) {
			double integral = 0;
			integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
			pulses[hh].hist->Scale(pulses[hh].scale/integral);
		}
		pulses[hh].max_val = pulses[hh].hist->GetBinContent(pulses[hh].hist->GetMaximumBin());
		max_val = std::max(max_val, pulses[hh].max_val);
		pulses[hh].baseline = 0;
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
	if (linear)
		frame->GetXaxis()->SetRangeUser(5, 25);
	frame->GetYaxis()->SetTitle("Amplitude (arb. u.)");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist->SetLineColor(palette_major[hh]);
		pulses[hh].hist->Draw("hist Lsame");
  }

	int precision1 = 2, precision2 = 3;
	int line_width = 2;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].total_integral = 0;
		pulses[hh].fast_integral = 0;
		pulses[hh].slow_integral = 0;
		pulses[hh].slow_on_fast = 0;
		double Toff = pulses[hh].t_offset;
		if (!pulses[hh].do_fit) {
			pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
			pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);
			pulses[hh].slow_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
			pulses[hh].slow_on_fast = integrate(pulses[hh].hist, pulses[hh].fast_t.second + Toff, 2*pulses[hh].fast_t.second - pulses[hh].fast_t.first + Toff, pulses[hh].baseline);
			pulses[hh].tau1 = "--";
			pulses[hh].tau1_err = "--";

			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision2);
			pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision2);
		} else {
			TF1* fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + Toff, pulses[hh].slow_fit_t.second + Toff, 4);
			fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
			SetParLimits(fit_f, 0, pulses[hh].slow_fit_t.first + Toff);
			SetParLimits(fit_f, 1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
			SetParLimits(fit_f, 2, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
			SetParLimits(fit_f, 3, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
			fit_f->SetLineColor(palette_minor[hh]);
			fit_f->SetLineWidth(line_width);
    	pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
			fit_f->SetNpx(800);
			fit_f->Draw("same");
			pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
			pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);

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
			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision2);
			pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision2);
		}
		std::cout<<"Results for "<<pulses[hh].name<<std::endl;
		std::cout<<"Fit option: \""<<pulses[hh].fit_option<<"\""<<std::endl;
		std::cout<<"Check Sum: Fast="<<pulses[hh].fast_integral<<" + Slow=" <<pulses[hh].slow_integral<<" == "<<std::endl;
		std::cout<<"\t\t"<<pulses[hh].fast_integral+pulses[hh].slow_integral<< " vs Total= "<< pulses[hh].total_integral <<std::endl;
		std::cout<<"#tau1\t#tau1_err\tFr1\tFr1_err"<<std::endl;
		std::cout<<pulses[hh].tau1<<"\t"<<pulses[hh].tau1_err<<"\t"<<pulses[hh].Fr1<<"\t"<<pulses[hh].err1<<std::endl;
  }

	std::vector<std::string> tau1, frsS;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string emp = "";
		std::string unit = "#mus";
		if (pulses[hh].tau1.empty() || pulses[hh].tau1 == "--")
			tau1.push_back(emp);
		else
			tau1.push_back("#tau_{S}=" + pulses[hh].tau1 + (print_errors ? "#pm" + pulses[hh].tau1_err : unit));
		if (pulses[hh].Fr1.empty() || pulses[hh].Fr1 == "--")
			frsS.push_back("");
		else
			frsS.push_back(pulses[hh].Fr1 + (print_errors ? "#pm" + pulses[hh].err1 : emp));
	}
	if (print_results) {
		if (!linear) {
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
			if (print_errors) {
				add_text(25, 0.025, no_title, tau1, palette_major);
				add_text(35, 0.025, Slow_title, frsS, palette_major);
			} else {
				add_text(35, 0.010, no_title, tau1, palette_major);
				add_text(45, 0.010, Slow_title, frsS, palette_major);
			}
		} else {
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
			add_text(15, 0.3, no_title, tau1, palette_major);
			add_text(20, 0.3, Slow_title, frsS, palette_major);
		}
	}

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh)
		legend->AddEntry(pulses[hh].hist, pulses[hh].name.c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();

	max_val = 0;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh)
		max_val = std::max(max_val, pulses[hh].hist_n_delays->GetBinContent(pulses[hh].hist_n_delays->GetMaximumBin()));
  max_val*= 1.2;

	if (draw_n_delays) {
		TCanvas *c_2 = new TCanvas ((std::string("2 ") + framename).c_str(), (std::string("2 ") + framename).c_str(), DEF_W, DEF_H);
		c_2->SetGrid(); c_2->SetTicks(); c_2->ToggleEventStatus(); c_2->ToggleToolBar();
		TLegend *legend2 = new TLegend(0.52, 0.8, 0.9, 0.9);
		//legend2->SetHeader("");
		legend2->SetMargin(0.25);
		TH2F* frame2 = new TH2F("frame", framename2.c_str(), 500, 0, 10, 500,  0, max_val);
		frame2->GetXaxis()->SetTitle("Number of delays");
		frame2->GetYaxis()->SetTitle("Counts");
		frame2->Draw();

		for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
			pulses[hh].hist_n_delays->SetLineWidth(2);
			pulses[hh].hist_n_delays->SetLineColor(palette_major[hh]);
			pulses[hh].hist_n_delays->Draw("hist same");
		}

		for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh)
			legend2->AddEntry(pulses[hh].hist_n_delays, pulses[hh].name.c_str(), "l");

		frame2->Draw("sameaxis");
		legend2->Draw("same");
		c_2->Update();
	}

  return 0;
}
