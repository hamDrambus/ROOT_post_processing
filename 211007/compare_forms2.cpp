//Created on 2022.02.10
//For 211007/results_v5
//9.2 mm with 238Pu set on the cathode. (Mesh instead of THGEM0, 28% CERN #8 as THEGEM1, PMT#1 malfunctions due to mixed signal wires)
//2650 on THGEM1 (gain~20), charge signal and SiPMs forms only
//Charge with restored baseline
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

void differentiate_function(TGraphErrors* graph) {
  if (graph->GetN() < 2) {
    std::cout<<"differentiate_function: wrong input"<<std::endl;
    return;
  }
  double y_prev = graph->GetY()[0], err_prev = graph->GetEY()[0];
  double y_, err_;
  for (int i = 1, i_end_ = graph->GetN(); i!=i_end_; ++i) {
    double x_prev = graph->GetX()[i-1], x_ = graph->GetX()[i];
    y_ = graph->GetY()[i];
    err_ = graph->GetEY()[i];
    graph->GetY()[i] = (y_ - y_prev)/(x_ - x_prev);
    graph->GetEY()[i] = sqrt(std::pow(err_prev, 2) + std::pow(err_, 2))/(x_ - x_prev);
    y_prev = y_;
    err_prev = err_;
  }
  graph->GetY()[0] = graph->GetY()[1];
  graph->GetEY()[0] = graph->GetEY()[1];
  return;
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

double integrate_function(TGraphErrors* graph, TF1* func, double from, double to) {
	double Int = 0;
  for (int bin = 0, bin_end = graph->GetN(); bin!=bin_end; ++bin) {
    if (graph->GetX()[bin]>from && graph->GetX()[bin]<to) {
      double dx = (0 == bin ? graph->GetX()[1] - graph->GetX()[0] : graph->GetX()[bin] - graph->GetX()[bin-1]);
      Int += func->Eval(graph->GetX()[bin]) * dx;
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

double integrate_function_bound(TGraphErrors* graph, TF1* func, double from, double to) {
	double Int = 0;
  for (int bin = 0, bin_end = graph->GetN(); bin!=bin_end; ++bin) {
    if (graph->GetX()[bin]>from && graph->GetX()[bin]<to) {
      double dx = (0 == bin ? graph->GetX()[1] - graph->GetX()[0] : graph->GetX()[bin] - graph->GetX()[bin-1]);
      Int += std::min(graph->GetY()[bin], func->Eval(graph->GetX()[bin])) * dx;
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

double integrate_function_bound(TGraphErrors* graph, double constant, double from, double to) {
	double Int = 0;
  for (int bin = 0, bin_end = graph->GetN(); bin!=bin_end; ++bin) {
    if (graph->GetX()[bin]>from && graph->GetX()[bin]<to) {
      double dx = (0 == bin ? graph->GetX()[1] - graph->GetX()[0] : graph->GetX()[bin] - graph->GetX()[bin-1]);
      Int += std::min(graph->GetY()[bin], constant) * dx;
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

double integrate_linear_function_bound(TGraphErrors* graph, double constant, double from, double to) {
	double Int = 0;
  for (int bin = 0, bin_end = graph->GetN(); bin!=bin_end; ++bin) {
    if (graph->GetX()[bin]>from && graph->GetX()[bin]<to) {
      double dx = (0 == bin ? graph->GetX()[1] - graph->GetX()[0] : graph->GetX()[bin] - graph->GetX()[bin-1]);
      double x = graph->GetX()[bin];
      double f = 0 + constant * (x - from)/(to-from);
      Int += std::min(graph->GetY()[bin], f) * dx;
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

double average(TGraphErrors* graph, double from, double to) {
	double baseline = 0;
  int counts = 0;
  for (int i = 0, i_end_ = graph->GetN(); i!=i_end_; ++i) {
    if (graph->GetX()[i]>from && graph->GetX()[i]<to) {
      baseline += graph->GetY()[i];
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

void scale(TGraphErrors* graph, double factor) {
  for (int i = 0, i_end_ = graph->GetN(); i!=i_end_; ++i) {
    graph->GetY()[i] = graph->GetY()[i] * factor;
    graph->GetEY()[i] = graph->GetEY()[i] * factor;
  }
}

double get_max_y(TGraphErrors* graph) {
  double max = -DBL_MAX;
  for (int i = 0, i_end_ = graph->GetN(); i!=i_end_; ++i) {
    max = std::max(max, graph->GetY()[i]);
  }
  return max;
}

double get_min_y(TGraphErrors* graph) {
  double min = DBL_MAX;
  for (int i = 0, i_end_ = graph->GetN(); i!=i_end_; ++i) {
    min = std::min(min, graph->GetY()[i]);
  }
  return min;
}

double get_max_y(TH1D *hist) {
  return hist->GetBinContent(hist->GetMaximumBin());
}

double get_min_y(TH1D *hist) {
  return 0;
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

TGraphErrors* CreateGraph(std::vector<double> &ixs, std::vector<double> &iys, std::vector<double> &iyes) {
	TGraphErrors* out = NULL;
	Int_t size = std::min(ixs.size(), iys.size());
	Int_t err_size = iyes.size();
	Double_t *xs = NULL, *ys = NULL, *xes = NULL, *yes = NULL;
	if (size > 0) {
		xs = new Double_t[size];
		ys = new Double_t[size];
		xes = new Double_t[size];
		yes = new Double_t[size];
		for (Int_t i = 0; i != size; ++i) {
			xs[i] = ixs[i];
			ys[i] = iys[i];
			xes[i] = 0;
			yes[i] = 1*(i < err_size ? iyes[i] : 0);
		}
		out = new TGraphErrors(size, xs, ys, xes, yes);
		delete [] xs;
		delete [] ys;
		delete [] xes;
		delete [] yes;
	}
	return out;
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

class FileExporter {
public:
  FileExporter(std::string filename) : _filename(filename)
  {}
  std::vector<PAIR> TabulateFunction (TF1* func) {
    std::vector<PAIR> out;
    if (func==NULL)
      return out;
    double from, to;
    func->GetRange(from, to);
    for (std::size_t i = 0, i_end_ = 2000; i!=i_end_; ++i) {
      double x = from + (to-from)*((double)i/(i_end_-1));
      out.push_back(PAIR(x, func->Eval(x)));
    }
    return out;
  }
  std::vector<PAIR> TabulateShape (pulse_shape* shape) {
    std::vector<PAIR> out;
    if (shape == NULL || (shape->hist == NULL && shape->graph == NULL))
      return out;
    if (shape->hist != NULL) {
      for (int bin = 1, bin_end = shape->hist->GetNbinsX()+1; bin!=bin_end; ++bin)
        out.push_back(PAIR(shape->hist->GetBinCenter(bin), shape->hist->GetBinContent(bin)));
    } else {
      double x, y;
      for (int p = 0, p_end = shape->graph->GetN(); p!=p_end; ++p) {
        if (shape->graph->GetPoint(p, x, y) != -1)
          out.push_back(PAIR(x, y));
      }
    }
    return out;
  }
  void AddToPrint(TF1* func, pulse_shape* shape, std::size_t index) {
    if (index < xy_table.size()/2) {
      std::vector<PAIR> new_f = TabulateFunction(func);
      std::size_t f_ind = index * 2 + 1;
      xy_table[f_ind].push_back(PAIR(DBL_MAX, DBL_MAX)); //separate new fit function from the previous ones
      xy_table[f_ind].insert(xy_table[f_ind].end(), new_f.begin(), new_f.end());
    } else {
      std::vector<PAIR> new_f = TabulateFunction(func);
      std::vector<PAIR> new_h = TabulateShape(shape);
      xy_table.push_back(new_h);
      xy_table.push_back(new_f);
    }
  }
  void Print(void) {
    std::ofstream str;
    str.open(_filename, std::ios_base::trunc);
    if (!str.is_open()) {
      std::cerr<<"FileExporter:Print: Failed to open file"<<std::endl;
      std::cerr<<"\t\""<<_filename<<"\""<<std::endl;
      return;
    }
    std::cout<<"FileExporter: printing to file"<<std::endl;
    std::cout<<"\t\""<<_filename<<"\""<<std::endl;
    std::size_t n_rows = 0;
    for (std::size_t c = 0, c_end_ = xy_table.size(); c!=c_end_; ++c) {
      n_rows = std::max(n_rows, xy_table[c].size());
    }
    for (std::size_t r = 0; r!=n_rows; ++r) {
      for (std::size_t c = 0, c_end_ = xy_table.size(); c!=c_end_; ++c) {
        str << (c == 0 ? "" : "\t");
        if (r < xy_table[c].size() && xy_table[c][r].first != DBL_MAX)
          str<< xy_table[c][r].first;
        else
          str<< "--";
        str<<"\t";
        if (r < xy_table[c].size() && xy_table[c][r].second != DBL_MAX)
          str<< xy_table[c][r].second;
        else
          str<< "--";
      }
      str<<std::endl;
    }
    str.close();
  }
  std::deque<std::vector<PAIR>> xy_table; //stores both histogram and fit function.
  std::string _filename;
};

FileExporter* file_exporter = NULL;

void draw_slow_component(TF1* fit_f, pulse_shape* shape, std::size_t index)
{
	fit_f->SetNpx(800);
	//fit_f->Draw("same");
  if (file_exporter) {
    file_exporter->AddToPrint(fit_f, shape, index);
  }
}


int compare_forms2 (void) {
	std::cout<<"COMPARISON_MODE"<<std::endl;

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::string def_fit_option = "NRE";
	bool combined = true;
	bool Cd_peak = true;
	int Nbins = 300;
	bool linear = 1;
	bool SiPMs = false;

	bool fast_PMTs = true;
	unsigned int PMT_used = 0x2 | 0x4 | 0x8;
	bool do_fit = true;
	bool fit_bad_forms = true;
	bool subtract_baseline = false;
	bool center_pulses = false;
	bool center_at_S1 = false; //Not used
	bool normalize_by_S1 = false; //Not used
	bool print_errors = false;
	bool print_results = true;
	double time_pretrigger_left = 51.0, time_pretrigger_right = 62.0;
	double time_left = 0, time_right = 160;//us
	double max_val = 0;
	double trigger_at = center_at_S1 ? 0 : 32; //Not used
	double y_min = 1e-4;

	pulse_shape* define = NULL, *copy = NULL;

  pulse_shape Q_20_0kV_raw;
  define = &Q_20_0kV_raw;
define->folder = std::string("../Data/211007/results_v2/211007_Pu_20.0kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_20.0kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->file_type = ShapeFileType::RawNoError;
define->Td = "8.3";
define->device = "#DeltaV_{THGEM}=2110V";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 100);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(-0.2, 0.2);
define->slow_ampl_bound = PAIR(1e-1, 2.9e-1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_18_8kV_raw;
  define = &Q_18_8kV_raw;
  copy = &Q_20_0kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_18.8kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_18.8kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "7.7";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 3e-1);

  pulse_shape Q_17_0kV_raw;
  define = &Q_17_0kV_raw;
  copy = &Q_18_8kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_17.0kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_17.0kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "6.8";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 1);

  pulse_shape Q_15_3kV_raw;
  define = &Q_15_3kV_raw;
  copy = &Q_17_0kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_15.3kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_15.3kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "6.0";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 1);

  pulse_shape Q_14_4kV_raw;
  define = &Q_14_4kV_raw;
  copy = &Q_15_3kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_14.4kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_14.4kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "5.5";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 1);

  pulse_shape Q_13_6kV_raw;
  define = &Q_13_6kV_raw;
  copy = &Q_14_4kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_13.6kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_13.6kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "5.1";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 1);

  pulse_shape Q_12_7kV_raw;
  define = &Q_12_7kV_raw;
  copy = &Q_13_6kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_12.7kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_12.7kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "4.7";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 1);

  pulse_shape Q_11_8kV_raw;
  define = &Q_11_8kV_raw;
  copy = &Q_12_7kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_11.8kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_11.8kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "4.2";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 1);

  pulse_shape Q_11_0kV_raw;
  define = &Q_11_0kV_raw;
  copy = &Q_11_8kV_raw;
*define = *copy;
define->fnames.clear();
define->folder = std::string("../Data/211007/results_v2/211007_Pu_11.0kV_800V_46V_12dB_2650V/Charge_10/");
define->fnames.push_back("Charge211007_Pu_11.0kV_800V_46V_12dB_2650V_ch_10_AVR_0");
define->Td = "3.8";
define->device = "V_{THGEM}=2110V";
define->slow_ampl_bound = PAIR(1e-1, 1);

  pulse_shape Q_20_0kV_restored;
  define = &Q_20_0kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_20.0kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "8.5";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 100);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(-0.3, 0.3);
define->slow_ampl_bound = PAIR(1e-1, 2.9e-1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_18_8kV_restored;
  define = &Q_18_8kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_18.8kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "7.9";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 3e-1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_17_0kV_restored;
  define = &Q_17_0kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_17.0kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "7.0";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_15_3kV_restored;
  define = &Q_15_3kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_15.3kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "6.1";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_14_4kV_restored;
  define = &Q_14_4kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_14.4kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "5.7";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_13_6kV_restored;
  define = &Q_13_6kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_13.6kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "5.3";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_12_7kV_restored;
  define = &Q_12_7kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_12.7kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "4.8";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 30.9;
define->fast_t = PAIR(29.7, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

  pulse_shape Q_11_8kV_restored;
  define = &Q_11_8kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_11.8kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "4.4";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 31.0;
define->fast_t = PAIR(29.8, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*30.94 - 29.70, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

	pulse_shape Q_11_0kV_restored;
	define = &Q_11_0kV_restored;
define->folder = std::string("211007/results_v5/restored_Q/");
define->fnames.push_back("211007_Pu_11.0kV_800V_46V_12dB_2650V");
define->file_type = ShapeFileType::RawNoError;
define->Td = "4.0";
define->device = "Charge, 2650V, restored";
define->fast_t_center = 31.1;
define->fast_t = PAIR(29.8, 2*30.94 - 29.70);
define->S1_t_center = 0;
define->S1_t = PAIR(0, 0);
define->scale = 1;
define->subtract_baseline = subtract_baseline;
define->renormalize = true;
define->slow_fit_t = PAIR(2*31.12 - 29.81, 60);
define->long_fit_t = PAIR(0, 0);
define->baseline_bound = PAIR(1e-6, 1e-6);
define->slow_ampl_bound = PAIR(1e-1, 1);
define->slow_tau_bound = PAIR(2.0, 10);
define->long_ampl_bound = PAIR(2e-4, 2e-3);
define->long_tau_bound = PAIR(15, 200);
define->simultaneous_fit = true;
define->do_fit = do_fit;
define->fit_option = "NRWE";

	std::string folder = SiPMs ? std::string("211007/results_v5/SiPMs_v1/")
					: std::string("211007/results_v5/Charge_v1/");
  file_exporter = new FileExporter("211007/results_v5/temp.txt");
	//std::vector<pulse_shape> pulses = {SiPM_20_0kV_no_trigger, SiPM_20_0kV_no_trigger_1, SiPM_20_0kV_no_trigger_2, SiPM_20_0kV_no_trigger_3};
  //std::vector<pulse_shape> pulses = {SiPM_20_0kV_no_trigger, SiPM_18_8kV_no_trigger, SiPM_17_0kV_no_trigger, SiPM_15_3kV_no_trigger, SiPM_14_4kV_no_trigger};
  //std::vector<pulse_shape> pulses = {SiPM_13_6kV_no_trigger, SiPM_12_7kV_no_trigger, SiPM_11_8kV_no_trigger, SiPM_11_0kV_no_trigger};

	//std::vector<pulse_shape> pulses = {Q_20_0kV_no_trigger, Q_20_0kV_no_trigger_1, Q_20_0kV_no_trigger_2, Q_20_0kV_no_trigger_3};
  //std::vector<pulse_shape> pulses = {Q_20_0kV_no_trigger, Q_18_8kV_no_trigger, Q_17_0kV_no_trigger, Q_15_3kV_no_trigger, Q_14_4kV_no_trigger};
  //std::vector<pulse_shape> pulses = {Q_13_6kV_no_trigger, Q_12_7kV_no_trigger, Q_11_8kV_no_trigger, Q_11_0kV_no_trigger};

  //std::vector<pulse_shape> pulses = {Q_20_0kV_raw, Q_18_8kV_raw, Q_17_0kV_raw, Q_15_3kV_raw, Q_14_4kV_raw};
  //std::vector<pulse_shape> pulses = {Q_13_6kV_raw, Q_12_7kV_raw, Q_11_8kV_raw, Q_11_0kV_raw};

	//std::vector<pulse_shape> pulses = {SiPM_11_0kV_no_trigger};
	//For paper and reports:
  //std::vector<pulse_shape> pulses = {Q_20_0kV_restored, Q_20_0kV_raw};
  std::vector<pulse_shape> pulses = {Q_20_0kV_raw};
  //std::vector<pulse_shape> pulses = {Q_20_0kV_restored, Q_18_8kV_restored, Q_17_0kV_restored, Q_15_3kV_restored, Q_14_4kV_restored};

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};
	int contribution_est_method = 2; //0 - use fit of slow/long components at fast component range;
	//1 - use constant with amplitude from fit; 2 - use linear function rising up to amplitude from fit;
	//adsf - for fast Crtl + F
	std::string framename;
	if (SiPMs)
    framename = std::string("Results for SiPM-matrix (no WLS), ")+(Cd_peak ? "" : "< ")+"5.5 MeV #alpha ^{238}Pu";
	else
    framename = std::string("Charge signal in THGEM1 from ")+(Cd_peak ? "" : "< ")+"5.5 MeV #alpha (^{238}Pu)";
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
		std::string hist_name = (pulses[hh].file_type == ShapeFileType::Histogram ? "hist" : "graph") + std::to_string(hh);
		pulses[hh].hist = (pulses[hh].file_type == ShapeFileType::Histogram ? new TH1D (hist_name.c_str(), hist_name.c_str(), Nbins, time_left, time_right) : NULL);
		std::string hist_name1 = "hist" + std::to_string(hh) + "no_long";
		pulses[hh].no_long_hist = (pulses[hh].file_type == ShapeFileType::Histogram ? new TH1D (hist_name1.c_str(), hist_name.c_str(), Nbins, time_left, time_right) : NULL);
		double toff = pulses[hh].t_offset = center_pulses ?
			(trigger_at - (center_at_S1 ? pulses[hh].S1_t_center : pulses[hh].fast_t_center)) : 0;
		for (int ff = 0, ff_end_ = pulses[hh].fnames.size(); ff!=ff_end_; ++ff) {
      if (ShapeFileType::Histogram == pulses[hh].file_type)
        read_hist_w (pulses[hh].hist, pulses[hh].folder + pulses[hh].fnames[ff], toff);
      else {
        std::vector<double> xs, ys, errs;
        read_graph(pulses[hh].folder + pulses[hh].fnames[ff], xs, ys, errs, toff);
        if (pulses[hh].file_type == ShapeFileType::RawNoError)
          errs.clear();
        pulses[hh].graph = CreateGraph(xs, ys, errs);
        pulses[hh].graph->SetNameTitle(hist_name.c_str());
        if (ShapeFileType::RawIntegral == pulses[hh].file_type)
          differentiate_function(pulses[hh].graph);
      }
    }

    if (ShapeFileType::Histogram == pulses[hh].file_type)
      pulses[hh].hist->Sumw2();
		pulses[hh].baseline = (ShapeFileType::Histogram == pulses[hh].file_type ?
      average(pulses[hh].hist, time_pretrigger_left + toff, time_pretrigger_right + toff) :
      average(pulses[hh].graph, time_pretrigger_left + toff, time_pretrigger_right + toff));
		if (pulses[hh].subtract_baseline) {
      if (ShapeFileType::Histogram == pulses[hh].file_type)
        SubtractBaseline(pulses[hh].hist, pulses[hh].baseline);
      else
        SubtractBaseline(pulses[hh].graph, pulses[hh].baseline);
			pulses[hh].baseline = 0;
		}
		if (pulses[hh].renormalize) {
				double integral = 0;
				if (normalize_by_S1)
		    	integral = (ShapeFileType::Histogram == pulses[hh].file_type ?
            integrate(pulses[hh].hist, pulses[hh].S1_t.first + toff, pulses[hh].S1_t.second + toff) :
            integrate(pulses[hh].graph, pulses[hh].S1_t.first + toff, pulses[hh].S1_t.second + toff));
				else
					integral = (ShapeFileType::Histogram == pulses[hh].file_type ?
            integrate(pulses[hh].hist, pulses[hh].fast_t.first + toff, pulses[hh].fast_t.second + toff) :
            integrate(pulses[hh].graph, pulses[hh].fast_t.first + toff, pulses[hh].fast_t.second + toff));
      if (ShapeFileType::Histogram == pulses[hh].file_type)
        scale(pulses[hh].hist, pulses[hh].scale/integral);
      else
        scale(pulses[hh].graph, pulses[hh].scale/integral);
			pulses[hh].baseline *= pulses[hh].scale/integral;
		}
		if (!pulses[hh].subtract_baseline) {
			pulses[hh].baseline_bound = PAIR(0.4*pulses[hh].baseline, 1.5*pulses[hh].baseline);
			pulses[hh].long_baseline_bound = PAIR(0.4*pulses[hh].baseline, 1.5*pulses[hh].baseline);
		}
    if (ShapeFileType::Histogram == pulses[hh].file_type)
		  pulses[hh].max_val = get_max_y(pulses[hh].hist);
    else
      pulses[hh].max_val = get_max_y(pulses[hh].graph);
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
	TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? -0.3 : y_min, max_val);
	frame->GetXaxis()->SetTitle("Time (#mus)");
	//=====================================
	if (!linear)
		frame->GetXaxis()->SetRangeUser(0, 160);
	else
		frame->GetXaxis()->SetRangeUser(20, 80);
	//=====================================
	frame->GetYaxis()->SetTitle("Amplitude (V)");
	frame->Draw();

	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
    if (ShapeFileType::Histogram == pulses[hh].file_type) {
  		pulses[hh].hist->SetLineWidth(2);
  		pulses[hh].hist->SetLineColor(palette_major[hh]);
  		pulses[hh].no_long_hist->SetLineWidth(2);
  		pulses[hh].no_long_hist->SetLineStyle(7);//9
  		pulses[hh].no_long_hist->SetLineColor(palette_major[hh+1]);
  		pulses[hh].hist->Draw("hist Lsame");
    } else {
      pulses[hh].graph->SetLineColor(palette_major[hh]);
      pulses[hh].graph->SetMarkerColor(palette_major[hh]);
      pulses[hh].graph->SetMarkerStyle(kFullDotSmall);
      pulses[hh].graph->SetMarkerSize(1);
      pulses[hh].graph->SetLineWidth(2);
      pulses[hh].graph->Draw("Lsame");
    }
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
			pulses[hh].total_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
        integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline) :
        integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
			pulses[hh].fast_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
        integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline) :
        integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);
			pulses[hh].slow_integral = pulses[hh].total_integral - pulses[hh].fast_integral;
			pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
        integrate(pulses[hh].hist, pulses[hh].fast_t.second + Toff, 2*pulses[hh].fast_t.second - pulses[hh].fast_t.first + Toff, pulses[hh].baseline) :
        integrate(pulses[hh].graph, pulses[hh].fast_t.second + Toff, 2*pulses[hh].fast_t.second - pulses[hh].fast_t.first + Toff, pulses[hh].baseline);
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
        if (ShapeFileType::Histogram == pulses[hh].file_type)
          pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
        else
          pulses[hh].graph->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, &pulses[hh], hh);
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(3), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(3), precision2);
				TF1* slow_int_f = new TF1(*fit_f);
				slow_int_f->FixParameter(1, 0);
				pulses[hh].total_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline) :
          integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
				pulses[hh].long_integral = slow_int_f->Integral(pulses[hh].fast_t.second + Toff, time_right + Toff);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].long_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].long_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].long_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_linear_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
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
      if (ShapeFileType::Histogram == pulses[hh].file_type)
        pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
      else
        pulses[hh].graph->Fit(fit_f, pulses[hh].fit_option.c_str());
			draw_slow_component(fit_f, &pulses[hh], hh);
			pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
			pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
			if (!long_exist) {
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline) :
          integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
			}
			TF1* slow_int_f = new TF1(*fit_f);
			slow_int_f->FixParameter(1, 0);

			pulses[hh].slow_integral = slow_int_f->Integral(pulses[hh].fast_t.second + Toff, time_right + Toff);
			if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
				pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate_function_bound(pulses[hh].hist, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
          integrate_function_bound(pulses[hh].graph, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
			if (1==contribution_est_method)
				pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
          integrate_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
			if (2==contribution_est_method)
				pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
          integrate_linear_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
			pulses[hh].fast_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
        integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second, pulses[hh].baseline) :
        integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second, pulses[hh].baseline);
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
        if (ShapeFileType::Histogram == pulses[hh].file_type)
          pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
        else
          pulses[hh].graph->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, &pulses[hh], hh);
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
				pulses[hh].tau2 = "--";
				pulses[hh].tau2_err = "--";

				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].slow_fit_t.second + Toff, pulses[hh].baseline) :
          integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, pulses[hh].slow_fit_t.second + Toff, pulses[hh].baseline);
				TF1* slow_int_f = new TF1(*fit_f);
				slow_int_f->FixParameter(1, 0);

				pulses[hh].slow_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate(pulses[hh].hist, pulses[hh].fast_t.second + Toff, pulses[hh].slow_fit_t.second + Toff, pulses[hh].baseline) :
          integrate(pulses[hh].graph, pulses[hh].fast_t.second + Toff, pulses[hh].slow_fit_t.second + Toff, pulses[hh].baseline);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, slow_int_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_linear_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				pulses[hh].fast_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second, pulses[hh].baseline) :
          integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second, pulses[hh].baseline);
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
        if (ShapeFileType::Histogram == pulses[hh].file_type)
          pulses[hh].hist->Fit(fit_f, pulses[hh].fit_option.c_str());
        else
          pulses[hh].graph->Fit(fit_f, pulses[hh].fit_option.c_str());
				draw_slow_component(fit_f, &pulses[hh], hh);
				pulses[hh].baseline = fit_f->GetParameter(1);
				pulses[hh].total_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline) :
          integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, DBL_MAX, pulses[hh].baseline);
				pulses[hh].fast_integral = (ShapeFileType::Histogram == pulses[hh].file_type) ?
          integrate(pulses[hh].hist, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline) :
          integrate(pulses[hh].graph, pulses[hh].fast_t.first + Toff, pulses[hh].fast_t.second + Toff, pulses[hh].baseline);
				pulses[hh].tau1 = dbl_to_str(fit_f->GetParameter(3), precision1);
				pulses[hh].tau1_err = dbl_to_str(fit_f->GetParError(3), precision1);
				pulses[hh].tau2 = dbl_to_str(fit_f->GetParameter(5), precision2);
				pulses[hh].tau2_err = dbl_to_str(fit_f->GetParError(5), precision2);
				TF1* slow_f = new TF1(*fit_f);
				slow_f->FixParameter(1, 0);
				slow_f->FixParameter(4, 0);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, slow_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, slow_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].slow_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_linear_function_bound(pulses[hh].graph, fit_f->GetParameter(2), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				TF1* long_f = new TF1(*fit_f);
				long_f->FixParameter(1, 0);
				long_f->FixParameter(2, 0);
				pulses[hh].long_integral = long_f->Integral(pulses[hh].fast_t.second + Toff, time_right + Toff);
				if (0==contribution_est_method)//estimate how much of a long component is in the fast signal
					pulses[hh].long_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, long_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, long_f, pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (1==contribution_est_method)
					pulses[hh].long_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_function_bound(pulses[hh].hist, fit_f->GetParameter(4), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_function_bound(pulses[hh].graph, fit_f->GetParameter(4), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
				if (2==contribution_est_method)
					pulses[hh].long_on_fast = (ShapeFileType::Histogram == pulses[hh].file_type) ?
            integrate_linear_function_bound(pulses[hh].hist, fit_f->GetParameter(4), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff) :
            integrate_linear_function_bound(pulses[hh].graph, fit_f->GetParameter(4), pulses[hh].fast_t.first + Toff,  pulses[hh].fast_t.second + Toff);
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
      //std::vector<std::string> Slow_title = {"Slow contribution:"};
			std::vector<std::string> Long_title = {"","Long"};
			if (print_errors) {
				//zcxv
				add_text(46, 0.05, no_title, tau1, palette_major);
				add_text(76, 0.009, Slow_title, frsS, palette_major);
				add_text(100, 0.009, Long_title, frsL, palette_major);
				add_text(129, 0.009, no_title, tau2, palette_major);
        //add_text(40, 0.4, no_title, tau1, palette_major);
				//add_text(70, 0.4, Slow_title, frsS, palette_major);
				//add_text(100, 0.02, Long_title, frsL, palette_major);
				//add_text(129, 0.02, no_title, tau2, palette_major);
			} else {
				add_text(48, 0.032, no_title, tau1, palette_major);
				add_text(76, 0.008, Slow_title, frsS, palette_major);
				add_text(93, 0.008, Long_title, frsL, palette_major);
				add_text(110, 0.008, no_title, tau2, palette_major);
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
		} else { //linear
			std::vector<std::string> no_title;
			std::vector<std::string> Slow_title = {"Slow component", "contribution:"};
			std::vector<std::string> Long_title;// = {"Long"};
			add_text(40, 0.15, no_title, tau1, palette_major);
			add_text(54, 0.15, Slow_title, frsS, palette_major);
			//add_text(52, 0.08, Long_title, frsL, palette_text);
			//add_text(58, 0.08, no_title, tau2, palette_text);
		}
	}
	for (int hh = 0, hh_end_ = pulses.size(); hh!=hh_end_; ++hh) {
    if (ShapeFileType::Histogram == pulses[hh].file_type)
      legend->AddEntry(pulses[hh].hist, (std::string("E/N_{EL} = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");
    else
      legend->AddEntry(pulses[hh].graph, (std::string("E/N_{EL} = ") + pulses[hh].Td + " Td, " + pulses[hh].device).c_str(), "l");
  }

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
#ifdef FAST_FIGURES_MODE
	std::string filename = in_kV + (PMTs ?"kV_4PMT_" :"kV_SiPMs_")+ (linear ? "lin":"log") + (Cd_peak ? "" : "_slope")
	 		+ "_Nb" + int_to_str(Nbins, 4) + "_" + def_fit_option + (combined ? "" : "sep") + ".png";
	c_->SaveAs((folder + filename).c_str(), "png");
#endif //FAST_FIGURES_MODE
  if (file_exporter) {
    file_exporter->Print();
    delete file_exporter;
    file_exporter = NULL;
  }
  return 0;
}
