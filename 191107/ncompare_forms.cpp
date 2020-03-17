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
  //par[1] - amplitude1
  //par[2] - tau1
  //par[3] - amplitude2
  //par[4] - tau2
  return par[1]*std::exp((par[0]-x[0])/par[2]) + par[3]*std::exp((par[0]-x[0])/par[4]);
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
		offset = std::log(frame->GetYaxis()->GetXmax()/frame->GetYaxis()->GetXmin())/5.75;
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
	double scale;
	bool subtract_baseline;
	bool renormalize;
	//Fit:
	PAIR slow_fit_t;
	PAIR long_fit_t;	
	PAIR baseline_bound;	
	PAIR slow_ampl_bound;	
	PAIR slow_tau_bound;
	PAIR long_ampl_bound;
	PAIR long_tau_bound;
	bool simultaneous_fit; //long+slow simultaneously or not
	bool do_fit;
	std::string fit_option; //"NWL" or "N"
//OUTPUT:
	TH1D* hist;
	std::vector<TF1*> fit_fs;
	double max_val;
	double t_offset;
	bool do_restore; //signal outside of 160 mcs
	//Results:
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

int ncompare_forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
	std::string def_fit_option = "NRE";
	pulse_shape* define = NULL;

	pulse_shape SiPM_20kV_no_trigger;
	define = &SiPM_20kV_no_trigger;
define->folder = "191107/results_v1/Alpha_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.5";
define->device = "SiPM matrix";
define->fast_t_center = 92.7;
define->fast_t = PAIR(89, 94.08);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(95.4, 155);
define->long_fit_t = PAIR(95.4, 155);	
define->baseline_bound = PAIR(1e-5, 1e-1);	
define->slow_ampl_bound = PAIR(1e-3, 2);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape SiPM_8kV_no_trigger;
	define = &SiPM_8kV_no_trigger;
define->folder = "191107/results_v1/Alpha_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.4";
define->device = "SiPM matrix";
define->fast_t_center = 92.5;
define->fast_t = PAIR(88, 96.06);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(97.1, 117);
define->long_fit_t = PAIR(0, 0);	
define->baseline_bound = PAIR(1e-5, 2e-4);	
define->slow_ampl_bound = PAIR(1e-3, 2);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;


	pulse_shape PMT4_20kV_no_trigger;
	define = &PMT4_20kV_no_trigger;
define->folder = "191107/results_v1/Alpha_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"5_form_by_S.hdata", "6_form_by_S.hdata", "7_form_by_S.hdata", "8_form_by_S.hdata"};
define->Td = "8.5";
define->device = "4PMT";
define->fast_t_center = 92.5;
define->fast_t = PAIR(89, 94.3);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(96.3, 160);
define->long_fit_t = PAIR(96.3, 160);	
define->baseline_bound = PAIR(1e-5, 1e-1);	
define->slow_ampl_bound = PAIR(1e-3, 1);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;

	pulse_shape PMT4_8kV_no_trigger;
	define = &PMT4_8kV_no_trigger;
define->folder = "191107/results_v1/Alpha_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"5_form_by_S.hdata", "6_form_by_S.hdata", "7_form_by_S.hdata", "8_form_by_S.hdata"};
define->Td = "3.4";
define->device = "4PMT";
define->fast_t_center = 92.2;
define->fast_t = PAIR(88, 96.0);
define->scale = 1;
define->subtract_baseline = true;
define->renormalize = true;
define->slow_fit_t = PAIR(96.0, 125.0);
define->long_fit_t = PAIR(0, 0);	
define->baseline_bound = PAIR(1e-5, 2e-4);	
define->slow_ampl_bound = PAIR(1e-3, 2);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = true;
define->fit_option = def_fit_option;


	pulse_shape SiPM_20kV_trigger;
	define = &SiPM_20kV_trigger;
define->folder = "191107/results_v4/Alpha_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "8.5";
define->device = "SiPM matrix";
define->fast_t_center = 92.7;
define->fast_t = PAIR(87, 92.2);
define->scale = 1;
define->subtract_baseline = false;
define->renormalize = true;
define->slow_fit_t = PAIR(95.4, 155);
define->long_fit_t = PAIR(95.4, 155);	
define->baseline_bound = PAIR(1e-5, 1e-1);	
define->slow_ampl_bound = PAIR(1e-3, 2);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = false;
define->fit_option = def_fit_option;

	pulse_shape SiPM_8kV_trigger;
	define = &SiPM_8kV_trigger;
define->folder = "191107/results_v4/Alpha_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "3.4";
define->device = "SiPM matrix";
define->fast_t_center = 92.5;
define->fast_t = PAIR(85, 94.5);
define->scale = 1;
define->subtract_baseline = false;
define->renormalize = true;
define->slow_fit_t = PAIR(97.1, 117);
define->long_fit_t = PAIR(0, 0);	
define->baseline_bound = PAIR(1e-5, 2e-4);	
define->slow_ampl_bound = PAIR(1e-3, 2);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = false;
define->fit_option = def_fit_option;


	pulse_shape PMT4_20kV_trigger;
	define = &PMT4_20kV_trigger;
define->folder = "191107/results_v4/Alpha_46V_20kV_850V/forms_Cd_peak/";
define->fnames = {"5_form_by_S.hdata", "6_form_by_S.hdata", "7_form_by_S.hdata", "8_form_by_S.hdata"};
define->Td = "8.5";
define->device = "4PMT";
define->fast_t_center = 92.5;
define->fast_t = PAIR(89, 94.3);
define->scale = 1;
define->subtract_baseline = false;
define->renormalize = true;
define->slow_fit_t = PAIR(96.3, 160);
define->long_fit_t = PAIR(96.3, 160);	
define->baseline_bound = PAIR(1e-5, 1e-1);	
define->slow_ampl_bound = PAIR(1e-3, 1);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = false;
define->fit_option = def_fit_option;

	pulse_shape PMT4_8kV_trigger;
	define = &PMT4_8kV_trigger;
define->folder = "191107/results_v4/Alpha_46V_8kV_850V/forms_Cd_peak/";
define->fnames = {"5_form_by_S.hdata", "6_form_by_S.hdata", "7_form_by_S.hdata", "8_form_by_S.hdata"};
define->Td = "3.4";
define->device = "4PMT";
define->fast_t_center = 92.2;
define->fast_t = PAIR(88, 96.0);
define->scale = 1;
define->subtract_baseline = false;
define->renormalize = true;
define->slow_fit_t = PAIR(96.0, 125.0);
define->long_fit_t = PAIR(0, 0);	
define->baseline_bound = PAIR(1e-5, 2e-4);	
define->slow_ampl_bound = PAIR(1e-3, 2);	
define->slow_tau_bound = PAIR(1, 10);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(30, 200);
define->simultaneous_fit = true;
define->do_fit = false;
define->fit_option = def_fit_option;

	//std::vector<pulse_shape> pulses = {SiPM_20kV_no_trigger, SiPM_8kV_no_trigger};
	std::vector<pulse_shape> pulses = {SiPM_20kV_trigger, SiPM_8kV_trigger};

	//std::vector<pulse_shape> pulses = {PMT4_20kV_no_trigger, PMT4_8kV_no_trigger};
	//std::vector<pulse_shape> pulses = {PMT4_20kV_trigger, PMT4_8kV_trigger};	
	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kMagenta, kAzure + 10, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	int Nbins = 600;
	bool center_pulses = false;
	bool print_errors = true;
    double time_pretrigger_left = 5, time_pretrigger_right = 40;
    double time_left = 0, time_right = 160;//us
    double max_val = 0;
	double trigger_at = 90;
	bool linear = false;
	std::string framename = std::string("Results for SiPM matrix with trigger adjustment, 5 MeV #alpha ^{238}U");// + " " + Tds[0] + " Td";
	
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = "hist" + std::to_string(hh);
		pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right);
		pulses[hh].t_offset = center_pulses ? (trigger_at - pulses[hh].fast_t_center) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff)
			read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], pulses[hh].t_offset);	
	
        pulses[hh].hist->Sumw2();
		if (pulses[hh].subtract_baseline) {
        	double baseline = average(pulses[hh].hist, time_pretrigger_left + pulses[hh].t_offset, time_pretrigger_right + pulses[hh].t_offset);
			subtract_baseline(pulses[hh].hist, baseline);
		}
		if (pulses[hh].renormalize) {
		    double integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
			pulses[hh].hist->Scale(pulses[hh].scale/integral);
		}
		pulses[hh].total_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, DBL_MAX);
		pulses[hh].fast_integral = integrate(pulses[hh].hist, pulses[hh].fast_t.first + pulses[hh].t_offset, pulses[hh].fast_t.second + pulses[hh].t_offset);
		pulses[hh].restored_integral = 0;
		pulses[hh].slow_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
		pulses[hh].long_integral = 0; //ATM only
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
	TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : 1e-3, max_val);
	frame->GetXaxis()->SetTitle("t [#mus]");
	frame->GetYaxis()->SetTitle("Pulse-height [arb.]");
	frame->Draw();
	
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		pulses[hh].hist->SetLineWidth(2);
		pulses[hh].hist->SetLineColor(palette_major[hh]);
		pulses[hh].hist->Draw("hist Lsame");
    }
	
	int precision1 = 2, precision2 = 0, precision3 = 2, precision4 = 2;
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		if (!pulses[hh].do_fit) {
			pulses[hh].slow_on_fast = integrate(pulses[hh].hist, pulses[hh].fast_t.second + pulses[hh].t_offset, 2*pulses[hh].fast_t.second - pulses[hh].fast_t.first + pulses[hh].t_offset);
			pulses[hh].tau1 = "--";
			pulses[hh].tau1_err = "--";
			pulses[hh].tau2 = "--";
			pulses[hh].tau2_err = "--";
		
			pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
			pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
			pulses[hh].Fr2 = "--";
			pulses[hh].err2 = "--";
		} else {
		if (!pulses[hh].simultaneous_fit) {
			TF1* fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].slow_fit_t.second + pulses[hh].t_offset, 4);
			pulses[hh].fit_fs.push_back(fit_f);
			fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
			fit_f->FixParameter(0, pulses[hh].slow_fit_t.first + pulses[hh].t_offset);
			fit_f->SetParLimits(1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
			fit_f->SetParLimits(2, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
			fit_f->SetParLimits(3, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
			fit_f->SetLineColor(palette_minor[hh]);
    		pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
			fit_f->Draw("same");
			pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + pulses[hh].t_offset,  pulses[hh].fast_t.second + pulses[hh].t_offset); //estimate how much of a slow component is in the fast signal
			pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
			pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = "--";
				pulses[hh].err2 = "--";
			} else {
				fit_f = new TF1("fit2", FittingF_exp, pulses[hh].long_fit_t.first + pulses[hh].t_offset, pulses[hh].long_fit_t.second + pulses[hh].t_offset, 4);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
				fit_f->FixParameter(0, pulses[hh].slow_fit_t.first + pulses[hh].t_offset);
				fit_f->SetParLimits(1, pulses[hh].baseline_bound.first, 1.0e-4);//pulses[hh].baseline_bound.second);
				fit_f->SetParLimits(2, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
				fit_f->SetParLimits(3, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				fit_f->Draw("same");
				pulses[hh].long_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + pulses[hh].t_offset,  pulses[hh].fast_t.second + pulses[hh].t_offset); //estimate how much of a long component is in the fast signal
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(3), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(3), precision2);
				pulses[hh].long_integral = integrate_function(pulses[hh].hist, fit_f, pulses[hh].fast_t.second + pulses[hh].t_offset, DBL_MAX);
				pulses[hh].slow_integral -= pulses[hh].long_integral;

				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = dbl_to_str(pulses[hh].long_integral/pulses[hh].total_integral, precision4);
				pulses[hh].err2 = dbl_to_str(pulses[hh].long_on_fast/pulses[hh].total_integral, precision4);
			}
		} else {
			if (pulses[hh].long_fit_t.first<=0 && pulses[hh].long_fit_t.second <=0) {
				TF1* fit_f = new TF1("fit1", FittingF_exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].slow_fit_t.second + pulses[hh].t_offset, 4);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "y0", "amplitude", "#tau");
				fit_f->FixParameter(0, pulses[hh].slow_fit_t.first + pulses[hh].t_offset);
				fit_f->SetParLimits(1, pulses[hh].baseline_bound.first, pulses[hh].baseline_bound.second);
				fit_f->SetParLimits(2, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				fit_f->SetParLimits(3, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				fit_f->Draw("same");
				pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + pulses[hh].t_offset,  pulses[hh].fast_t.second + pulses[hh].t_offset); //estimate how much of a slow component is in the fast signal
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";
				pulses[hh].Fr1 = dbl_to_str(pulses[hh].slow_integral/pulses[hh].total_integral, precision3);
				pulses[hh].err1 = dbl_to_str(pulses[hh].slow_on_fast/pulses[hh].total_integral, precision3);
				pulses[hh].Fr2 = "--";
				pulses[hh].err2 = "--";
			} else {
				TF1* fit_f = new TF1("fit1", FittingF_2exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].long_fit_t.second + pulses[hh].t_offset, 5);
				pulses[hh].fit_fs.push_back(fit_f);
				fit_f->SetParNames("start_time", "amplitude1", "#tau1", "amplitude2", "#tau2");
				fit_f->FixParameter(0, pulses[hh].slow_fit_t.first + pulses[hh].t_offset);
				fit_f->SetParLimits(1, pulses[hh].slow_ampl_bound.first, pulses[hh].slow_ampl_bound.second);
				fit_f->SetParLimits(2, pulses[hh].slow_tau_bound.first, pulses[hh].slow_tau_bound.second);
				fit_f->SetParLimits(3, pulses[hh].long_ampl_bound.first, pulses[hh].long_ampl_bound.second);
				fit_f->SetParLimits(4, pulses[hh].long_tau_bound.first, pulses[hh].long_tau_bound.second);
				fit_f->SetLineColor(palette_minor[hh]);
				pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
				fit_f->Draw("same");
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(2), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(2), precision1);
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(4), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(4), precision2);
				TF1* slow_f = new TF1("slow_f", FittingF_exp, pulses[hh].slow_fit_t.first + pulses[hh].t_offset, pulses[hh].slow_fit_t.second + pulses[hh].t_offset, 4);
				slow_f->FixParameter(0, fit_f->GetParameter(0));
				slow_f->FixParameter(1, 0);
				slow_f->FixParameter(2, fit_f->GetParameter(1));
				slow_f->FixParameter(3, fit_f->GetParameter(2));
				pulses[hh].slow_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(1), pulses[hh].fast_t.first + pulses[hh].t_offset,  pulses[hh].fast_t.second + pulses[hh].t_offset); //estimate how much of a slow component is in the fast signal
				TF1* long_f = new TF1("long_f", FittingF_exp, pulses[hh].long_fit_t.first + pulses[hh].t_offset, pulses[hh].long_fit_t.second + pulses[hh].t_offset, 4);
				long_f->FixParameter(0, fit_f->GetParameter(0));
				long_f->FixParameter(1, 0);
				long_f->FixParameter(2, fit_f->GetParameter(3));
				long_f->FixParameter(3, fit_f->GetParameter(4));
				pulses[hh].long_integral = integrate_function(pulses[hh].hist, long_f, pulses[hh].fast_t.second + pulses[hh].t_offset, DBL_MAX);
				pulses[hh].slow_integral -= pulses[hh].long_integral;
				pulses[hh].long_on_fast = integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(3), pulses[hh].fast_t.first + pulses[hh].t_offset,  pulses[hh].fast_t.second + pulses[hh].t_offset); //estimate how much of a long component is in the fast signal
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
		add_text(3.5, 0.07e10, no_title, tau1, palette_major);
		add_text(35, 0.07e10, Slow_title, frsS, palette_major);
		add_text(60, 0.07e10, Long_title, frsL, palette_major);
		add_text(117, 0.013e10, no_title, tau2, palette_major);
	} else {
		std::vector<std::string> no_title;
		std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
		std::vector<std::string> Long_title;// = {"Long"};
		add_text(38, 0.15, no_title, tau1, palette_major);
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
