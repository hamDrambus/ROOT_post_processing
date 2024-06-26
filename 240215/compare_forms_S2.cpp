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
    hist->SetBinContent(bin, hist->GetBinContent(bin) - baseline);
  }
}

void trim_histogram(TH1D *hist) {
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    hist->SetBinContent(bin, std::max(hist->GetBinContent(bin), 0.0));
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

int compare_forms_S2(void) {

  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

  std::string def_fit_option = "NRE";
  bool combined = true;
  bool Cd_peak = true;
  int Nbins = 300;
  bool linear = 0;
  bool PMTs = true;

  bool fast_PMTs = false;
	unsigned int PMT_used = 0x1 | 0x2 | 0x4 | 0x8;
  unsigned int PMT_used_N2 = 0x1;
  unsigned int PMT_used_NIR = 0x1;
  bool fast_PMTs_NIR = true;
  bool do_fit = true;
  bool fit_bad_forms = false;
  bool subtact_baseline = true;
  bool center_pulses = false;
  bool center_at_S1 = false;
  bool normalize_by_S1 = false;
  bool normalize = true;
  bool print_errors = false;
  double time_pretrigger_left = 4, time_pretrigger_right = 30;
  double time_left = 0, time_right = 160; // us
  double max_val = 0;
  double trigger_at = center_at_S1 ? 32 : 63;
  double y_min = 1e-5;
  double time_offset = 0; // 30.0-93;
  double y_offset = 0; // 5e-3;
  Nbins *= (time_right - time_left)/160.0;

  pulse_shape* define = NULL, *copy = NULL;
  pulse_shape SiPM_N2_20kV_no_trigger;
  define = &SiPM_N2_20kV_no_trigger;
define->folder = "231109/results_v2/X-ray_6mm_coll_20kV/forms_X-ray/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "9.2";
define->device = "SiPM matrix, Ar + 913 ppm N2";
define->fast_t_center = 62.7;
define->fast_t = PAIR(42, 74.6);
define->S1_t_center = define->fast_t_center;
define->S1_t = define->fast_t;
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = normalize;
define->slow_fit_t = PAIR(85, 159);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

  pulse_shape SiPM_N2_18kV_no_trigger;
  define = &SiPM_N2_18kV_no_trigger;
  copy = &SiPM_N2_20kV_no_trigger;
*define = *copy;
define->folder = "231109/results_v2/X-ray_6mm_coll_18kV/forms_X-ray/";
define->Td = "8.3";
define->scale = 1;
define->fast_t_center = 63.7;
define->fast_t = PAIR(42, 80);
define->slow_fit_t = PAIR(89, 159);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape SiPM_N2_12kV_no_trigger;
  define = &SiPM_N2_12kV_no_trigger;
  copy = &SiPM_N2_20kV_no_trigger;
*define = *copy;
define->folder = "231109/results_v2/X-ray_6mm_coll_12kV/forms_X-ray/";
define->Td = "5.5";
define->scale = 1;
define->fast_t_center = 70.2;
define->fast_t = PAIR(42, 82.7);
define->slow_fit_t = PAIR(89, 159);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape SiPM_Ar_20kV_no_trigger;
  define = &SiPM_Ar_20kV_no_trigger;
define->folder = "201015_Xray/results_v1/X-ray_6mm_coll_20kV/forms_X-ray/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "9.5";
define->device = "SiPM matrix, pure Ar";
define->fast_t_center = 63.1;
define->fast_t = PAIR(42, SiPM_N2_20kV_no_trigger.fast_t.second + define->fast_t_center - SiPM_N2_20kV_no_trigger.fast_t_center);
define->S1_t_center = define->fast_t_center;
define->S1_t = define->fast_t;
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = normalize;
define->slow_fit_t = PAIR(90, 156);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

  pulse_shape SiPM_Ar_18kV_no_trigger;
  define = &SiPM_Ar_18kV_no_trigger;
  copy = &SiPM_Ar_20kV_no_trigger;
*define = *copy;
define->folder = "201015_Xray/results_v1/X-ray_6mm_coll_18kV/forms_X-ray/";
define->Td = "8.5";
define->scale = 1;
define->fast_t_center = 64.4;
define->fast_t = PAIR(42, SiPM_N2_18kV_no_trigger.fast_t.second + define->fast_t_center - SiPM_N2_18kV_no_trigger.fast_t_center);
define->slow_fit_t = PAIR(88, 154);
define->slow_ampl_bound = PAIR(1e-3, 1);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape SiPM_Ar_12kV_no_trigger;
  define = &SiPM_Ar_12kV_no_trigger;
  copy = &SiPM_Ar_20kV_no_trigger;
*define = *copy;
define->folder = "201015_Xray/results_v1/X-ray_6mm_coll_12kV/forms_X-ray/";
define->Td = "5.7";
define->scale = 1;
define->fast_t_center = 70.2;
define->fast_t = PAIR(42, SiPM_N2_12kV_no_trigger.fast_t.second + define->fast_t_center - SiPM_N2_12kV_no_trigger.fast_t_center);
define->slow_fit_t = PAIR(91, 154);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape SiPM_Ar_20kV_no_trigger_NIR;
  define = &SiPM_Ar_20kV_no_trigger_NIR;
define->folder = "240215/results_v1/X-ray_6mm_coll_20kV/forms_X-ray/";
define->fnames = {"SiPMs_form_by_Npe.hdata"};
define->Td = "9.5";
define->device = "SiPM matrix, pure Ar, NIR";
define->fast_t_center = 63.1;
define->fast_t = PAIR(42, SiPM_N2_20kV_no_trigger.fast_t.second + define->fast_t_center - SiPM_N2_20kV_no_trigger.fast_t_center);
define->S1_t_center = define->fast_t_center;
define->S1_t = define->fast_t;
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = normalize;
define->slow_fit_t = PAIR(110, 156);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

  pulse_shape SiPM_Ar_18kV_no_trigger_NIR;
  define = &SiPM_Ar_18kV_no_trigger_NIR;
  copy = &SiPM_Ar_20kV_no_trigger_NIR;
*define = *copy;
define->folder = "240215/results_v1/X-ray_6mm_coll_18kV/forms_X-ray/";
define->Td = "8.5";
define->scale = 1;
define->fast_t_center = 64.4;
define->fast_t = PAIR(42, SiPM_N2_18kV_no_trigger.fast_t.second + define->fast_t_center - SiPM_N2_18kV_no_trigger.fast_t_center);
define->slow_fit_t = PAIR(105, 154);
define->slow_ampl_bound = PAIR(1e-3, 1);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape SiPM_Ar_12kV_no_trigger_NIR;
  define = &SiPM_Ar_12kV_no_trigger_NIR;
  copy = &SiPM_Ar_20kV_no_trigger_NIR;
*define = *copy;
define->folder = "240215/results_v1/X-ray_6mm_coll_12kV/forms_X-ray/";
define->Td = "5.7";
define->scale = 1;
define->fast_t_center = 70.2;
define->fast_t = PAIR(42, SiPM_N2_12kV_no_trigger.fast_t.second + define->fast_t_center - SiPM_N2_12kV_no_trigger.fast_t_center);
define->slow_fit_t = PAIR(106, 154);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape PMT4_N2_20kV_no_trigger;
  define = &PMT4_N2_20kV_no_trigger;
define->folder = "231109/results_v2/X-ray_6mm_coll_20kV/forms_X-ray/";
if(PMT_used_N2&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used_N2&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used_N2&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used_N2&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "9.2";
if(PMT_used_N2 == (0x1|0x2|0x4|0x8))
  define->device = "4PMT ";
else
  define->device = std::string("PMT#") + (PMT_used_N2&0x1 ? "1,":"") + (PMT_used_N2&0x2 ? "2,":"") + (PMT_used_N2&0x4 ? "3,":"") + (PMT_used_N2&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->device += ", Ar + 913 ppm N2";
define->fast_t_center = 62.4;
define->fast_t = SiPM_N2_20kV_no_trigger.fast_t;
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(72, 159);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

  pulse_shape PMT4_N2_18kV_no_trigger;
  define = &PMT4_N2_18kV_no_trigger;
  copy = &PMT4_N2_20kV_no_trigger;
*define = *copy;
define->folder = "231109/results_v2/X-ray_6mm_coll_18kV/forms_X-ray/";
define->Td = "8.3";
define->scale = 1;
define->fast_t_center = 63.9;
define->fast_t = SiPM_N2_18kV_no_trigger.fast_t;
define->slow_fit_t = PAIR(80, 159);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape PMT4_N2_12kV_no_trigger;
  define = &PMT4_N2_12kV_no_trigger;
  copy = &PMT4_N2_20kV_no_trigger;
*define = *copy;
define->folder = "231109/results_v2/X-ray_6mm_coll_12kV/forms_X-ray/";
define->Td = "5.5";
define->scale = 1;
define->fast_t_center = 63.9;
define->fast_t = SiPM_N2_12kV_no_trigger.fast_t;
define->slow_fit_t = PAIR(80, 159);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape PMT4_Ar_20kV_no_trigger;
  define = &PMT4_Ar_20kV_no_trigger;
define->folder = "201015_Xray/results_v1/X-ray_6mm_coll_20kV/forms_X-ray/";
if(PMT_used&0x1) define->fnames.push_back(fast_PMTs ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used&0x2) define->fnames.push_back(fast_PMTs ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used&0x4) define->fnames.push_back(fast_PMTs ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used&0x8) define->fnames.push_back(fast_PMTs ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "9.5";
if(PMT_used == (0x1|0x2|0x4|0x8))
  define->device = "4PMT ";
else
  define->device = std::string("PMT#") + (PMT_used&0x1 ? "1,":"") + (PMT_used&0x2 ? "2,":"") + (PMT_used&0x4 ? "3,":"") + (PMT_used&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs) define->device += " (slow)";
define->device += ", pure Ar";
define->fast_t_center = 63.1;
define->fast_t = SiPM_Ar_20kV_no_trigger.fast_t;
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(86, 156);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

  pulse_shape PMT4_Ar_18kV_no_trigger;
  define = &PMT4_Ar_18kV_no_trigger;
  copy = &PMT4_Ar_20kV_no_trigger;
*define = *copy;
define->folder = "201015_Xray/results_v1/X-ray_6mm_coll_18kV/forms_X-ray/";
define->Td = "8.5";
define->scale = 1;
define->fast_t_center = 63.1;
define->fast_t = SiPM_Ar_18kV_no_trigger.fast_t;
define->slow_fit_t = PAIR(88, 156);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape PMT4_Ar_12kV_no_trigger;
  define = &PMT4_Ar_12kV_no_trigger;
  copy = &PMT4_Ar_20kV_no_trigger;
*define = *copy;
define->folder = "201015_Xray/results_v1/X-ray_6mm_coll_12kV/forms_X-ray/";
define->Td = "5.7";
define->scale = 1;
define->fast_t_center = 70.2;
define->fast_t = SiPM_Ar_12kV_no_trigger.fast_t;
define->slow_fit_t = PAIR(91, 156);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape PMT4_Ar_20kV_no_trigger_NIR;
  define = &PMT4_Ar_20kV_no_trigger_NIR;
define->folder = "240215/results_v1/X-ray_6mm_coll_20kV/forms_X-ray/";
if(PMT_used_NIR&0x1) define->fnames.push_back(fast_PMTs_NIR ? "5_form_by_Npe.hdata" : "1_form_by_Npe.hdata");
if(PMT_used_NIR&0x2) define->fnames.push_back(fast_PMTs_NIR ? "6_form_by_Npe.hdata" : "2_form_by_Npe.hdata");
if(PMT_used_NIR&0x4) define->fnames.push_back(fast_PMTs_NIR ? "7_form_by_Npe.hdata" : "3_form_by_Npe.hdata");
if(PMT_used_NIR&0x8) define->fnames.push_back(fast_PMTs_NIR ? "8_form_by_Npe.hdata" : "4_form_by_Npe.hdata");
define->Td = "9.5";
if(PMT_used_NIR == (0x1|0x2|0x4|0x8))
  define->device = "4PMT ";
else
  define->device = std::string("PMT#") + (PMT_used_NIR&0x1 ? "1,":"") + (PMT_used_NIR&0x2 ? "2,":"") + (PMT_used_NIR&0x4 ? "3,":"") + (PMT_used_NIR&0x8 ? "4,":"");
define->device.pop_back();
if (!fast_PMTs_NIR) define->device += " (slow)";
define->device += ", pure Ar";
define->fast_t_center = 63.0;
define->fast_t = SiPM_Ar_20kV_no_trigger_NIR.fast_t;
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtact_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(92, 152);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);
define->long_ampl_bound = PAIR(3e-4, 1e-1);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = false;
define->do_fit = do_fit;
define->fit_option = def_fit_option;

  pulse_shape PMT4_Ar_18kV_no_trigger_NIR;
  define = &PMT4_Ar_18kV_no_trigger_NIR;
  copy = &PMT4_Ar_20kV_no_trigger_NIR;
*define = *copy;
define->folder = "240215/results_v1/X-ray_6mm_coll_18kV/forms_X-ray/";
define->Td = "8.5";
define->scale = 1;
define->fast_t_center = 64.5;
define->fast_t = SiPM_Ar_18kV_no_trigger_NIR.fast_t;
define->slow_fit_t = PAIR(92, 152);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  pulse_shape PMT4_Ar_12kV_no_trigger_NIR;
  define = &PMT4_Ar_12kV_no_trigger_NIR;
  copy = &PMT4_Ar_20kV_no_trigger_NIR;
*define = *copy;
define->folder = "240215/results_v1/X-ray_6mm_coll_12kV/forms_X-ray/";
define->Td = "5.7";
define->scale = 1;
define->fast_t_center = 71.1;
define->fast_t = SiPM_Ar_12kV_no_trigger_NIR.fast_t;
define->slow_fit_t = PAIR(96, 153);
define->slow_ampl_bound = PAIR(1e-4, 1e-2);
define->slow_tau_bound = PAIR(20, 200);

  // std::vector<pulse_shape> pulses = {PMT4_Ar_12kV_no_trigger};
  // std::vector<pulse_shape> pulses = {SiPM_N2_20kV_no_trigger, SiPM_Ar_20kV_no_trigger, SiPM_Ar_20kV_no_trigger_NIR};
  // std::vector<pulse_shape> pulses = {SiPM_N2_18kV_no_trigger, SiPM_Ar_18kV_no_trigger, SiPM_Ar_18kV_no_trigger_NIR};
  // std::vector<pulse_shape> pulses = {SiPM_N2_12kV_no_trigger, SiPM_Ar_12kV_no_trigger, SiPM_Ar_12kV_no_trigger_NIR};
  // std::vector<pulse_shape> pulses = {PMT4_N2_20kV_no_trigger, PMT4_Ar_20kV_no_trigger, PMT4_Ar_20kV_no_trigger_NIR};
  // std::vector<pulse_shape> pulses = {PMT4_N2_18kV_no_trigger, PMT4_Ar_18kV_no_trigger, PMT4_Ar_18kV_no_trigger_NIR};
  std::vector<pulse_shape> pulses = {PMT4_N2_12kV_no_trigger, PMT4_Ar_12kV_no_trigger, PMT4_Ar_12kV_no_trigger_NIR};
  // std::vector<pulse_shape> pulses = {SiPM_Ar_20kV_no_trigger_NIR, SiPM_Ar_18kV_no_trigger_NIR, SiPM_Ar_12kV_no_trigger_NIR};
  // std::vector<pulse_shape> pulses = {PMT4_Ar_20kV_no_trigger_NIR, PMT4_Ar_18kV_no_trigger_NIR, PMT4_Ar_12kV_no_trigger_NIR};
  //for paper:

  std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
  std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
  int contribution_est_method = 0; //0 - use fit of slow/long components at fast component range;
  //1 - use constant with amplitude from fit; 2 - use linear function rising up to amplitude from fit;
  //adsf - for fast Crtl + F
  std::string framename;
  if (PMTs)
    framename = std::string("Results for 4PMT (no WLS), ")+"X-ray";// + " " + Tds[0] + " Td";
  else
    framename = std::string("Results for SiPM-matrix (no WLS), ")+"X-ray";// + " " + Tds[0] + " Td";
  for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
    std::string hist_name = "hist" + std::to_string(hh);
    pulses[hh].hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left + time_offset, time_right + time_offset);
    hist_name = "hist" + std::to_string(hh) + "no_long";
    pulses[hh].no_long_hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left + time_offset, time_right + time_offset);
    hist_name = "hist" + std::to_string(hh) + "residue";
    pulses[hh].fast_hist = new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left + time_offset, time_right + time_offset);
    pulses[hh].t_offset = center_pulses ?
      (trigger_at - (center_at_S1 ? pulses[hh].S1_t_center : pulses[hh].fast_t_center)) : 0;
    pulses[hh].t_offset += time_offset;
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
    subtract_baseline(pulses[hh].hist, -y_offset);
    pulses[hh].baseline += y_offset;
    pulses[hh].baseline_bound.first += y_offset; pulses[hh].baseline_bound.second += y_offset;
    pulses[hh].long_baseline_bound.first += y_offset; pulses[hh].long_baseline_bound.second += y_offset;
    trim_histogram(pulses[hh].hist);
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
  TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left + time_offset, time_right + time_offset, 500, linear ? 0 : y_min, max_val);
  frame->GetXaxis()->SetTitle("Time (#mus)");
  frame->GetXaxis()->SetLabelSize(0.045);
  frame->GetXaxis()->SetTitleSize(0.045);
  //=====================================
  if (!linear)
    frame->GetXaxis()->SetRangeUser(2, 158);
  else
    frame->GetXaxis()->SetRangeUser(20, 90);
  //=====================================
  frame->GetYaxis()->SetTitle("PE peak count (arb. units)");
  frame->GetYaxis()->SetLabelSize(0.045);
  frame->GetYaxis()->SetTitleSize(0.045);
  frame->Draw();

  for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
    pulses[hh].hist->SetLineWidth(2);
    pulses[hh].hist->SetLineColor(palette_major[hh]);
    pulses[hh].fast_hist->SetLineWidth(2);
    pulses[hh].fast_hist->SetLineStyle(7);//9
    pulses[hh].fast_hist->SetLineColor(palette_major[hh]);
    pulses[hh].no_long_hist->SetLineWidth(2);
    pulses[hh].no_long_hist->SetLineStyle(7);//9
    pulses[hh].no_long_hist->SetLineColor(palette_major[hh+1]);
    pulses[hh].hist->Draw("hist Lsame");
    }

  int precision1 = 2, precision2 = 0, precision3 = 2, precision4 = 3;
  int line_width = 2;
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
      pulses[hh].fast_hist->Draw("hist Lsame");
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
        pulses[hh].fast_hist->Draw("hist Lsame");
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
      tau1.push_back("#tau_{L}=" + pulses[hh].tau1 + (print_errors ? "#pm" + pulses[hh].tau1_err : unit));
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
  
  if (!linear) {
    std::vector<std::string> no_title;
    std::vector<std::string> Slow_title = {"Contribution:"};
    std::vector<std::string> Long_title;
    //std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
    //std::vector<std::string> Long_title;
    if (print_errors) { //zcxv - for fast Crtl+F
      add_text(90, 0.007, no_title, tau1, palette_major);
      add_text(120, 0.007, Slow_title, frsS, palette_major);
      //add_text(128, 0.025, Long_title, frsL, palette_major);
      //add_text(144, 0.025, no_title, tau2, palette_major);
    } else {
      add_text(90, 0.007, no_title, tau1, palette_major);
      add_text(120, 0.007, Slow_title, frsS, palette_major);
      //add_text(129, 0.015, Long_title, frsL, palette_major);
      //add_text(142, 0.015, no_title, tau2, palette_major);
    }
  } else {
    std::vector<std::string> no_title;
    std::vector<std::string> Slow_title = {"Contribution:"};
    std::vector<std::string> Long_title;// = {"Long"};
    add_text(96, 0.15, no_title, tau1, palette_major);
    add_text(106, 0.15, Slow_title, frsS, palette_major);
    //add_text(52, 0.08, Long_title, frsL, palette_text);
    //add_text(58, 0.08, no_title, tau2, palette_text);
  }

  for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
    legend->AddEntry(pulses[hh].hist, ("E/N = "s + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");
  }

  frame->Draw("sameaxis");
  legend->Draw("same");
  c_->Update();
  return 0;
}
