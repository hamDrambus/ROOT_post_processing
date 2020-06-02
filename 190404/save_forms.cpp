// Created on 2020.05.09
// Designed to be called from analysis_history to create file which
// will be loaded in TriggerFitData::SetPulseShape in order to 
// iteratively adjust the trigger. This means this function will
// be called after each analysis_history pass for PMTs/SiPMs for
// all electric fields.
// Relies on post_processor.so (run after .x init.h)

#define PAIR std::pair<double, double>

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

void print_hist(TH1D *hist, std::ofstream &str) {
  if (!str.is_open() || hist == NULL) {
    std::cout<<"Error: print_hist: invalid parameters"<<std::endl;
    return;
  }
  for (int bin = 1, bin_end = hist->GetNbinsX()+1; bin!=bin_end; ++bin) {
    str<<hist->GetBinCenter(bin)<<"\t"<<hist->GetBinContent(bin)<<std::endl;
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

int save_forms (std::string prefix, std::string output_file, int Nbins, int type) {
  //type: 1 - 4PMT by Npeaks, 2 - 4PMT by Npe, 3 - 4PMT by S
  //4 - SiPM-matrix by Npeaks, 5 - SiPM-matrix by Npe, 6 - SiPM-matrix by S
  std::string output_data = output_file + ".dat";
  std::string output_fig = output_file + ".png";
  std::ofstream output;
  open_output_file(output_data, output, std::ios_base::trunc);
  if (!output.is_open()) {
    std::cout<<"Error: save_forms: could not open output file \""<<output_file<<"\""<<std::endl;
    return 1;
  }
  if (type<1 || type>6) {
    std::cout<<"Error: save_forms: input int type="<<type<<" is out of range [1, 6]"<<std::endl;
    return 1;
  }
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  int DEF_W = 1300, DEF_H = 700;
  std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
  std::vector<Color_t> palette_minor = {kGray + 1, kRed-3, kAzure + 6, kGreen -2, kMagenta+3, kOrange - 7, kOrange + 6};

  bool linear = 1;

  bool subtact_baseline = true;
  bool renormalize = true;
  double time_pretrigger_left = 5, time_pretrigger_right = 20;
  double time_left = 0, time_right = 160;//us
  double max_val = 0;
  double y_min = 1e-4;

  std::string device = type > 3 ? "SiPM-matrix" : "4PMT";
  std::string weight_type;
  switch ((type - 1) % 3) {
  case 0: {
    weight_type = "by Npeaks";
    break;  
  }
  case 1: {
    weight_type = "by Npe";
    break;  
  }
  case 2: {
    weight_type = "by S";
    break;  
  }
  }

  std::string framename = prefix + " " + device + " " + weight_type;
  std::string histname = std::string("hist_") + prefix + " " + device + " " + weight_type;
  TH1D *hist = new TH1D (histname.c_str(), histname.c_str(), Nbins, time_left, time_right);

  std::vector<std::string> files_to_read;
  if (type > 3) {
    std::string fname = "SiPMs_form_by_";
    switch ((type - 1) % 3) {
    case 0: {
      fname += "Npeaks";
      break;  
    }
    case 1: {
      fname += "Npe";
      break;  
    }
    case 2: {
      fname += "S";
      break;  
    }
    }
    fname += ".hdata";
    fname = prefix + fname;
    files_to_read.push_back(fname);
  } else {
    files_to_read = {"8_form_by_", "9_form_by_", "10_form_by_", "11_form_by_"};
    for (std::size_t f = 0, f_end_ = files_to_read.size(); f!=f_end_; ++f) {
      switch ((type - 1) % 3) {
      case 0: {
        files_to_read[f] += "Npeaks";
        break;  
      }
      case 1: {
        files_to_read[f] += "Npe";
        break;  
      }
      case 2: {
        files_to_read[f] += "S";
        break;  
      }
      }
      files_to_read[f] += ".hdata";
      files_to_read[f] = prefix + files_to_read[f];
    }
  }

  for (std::size_t f = 0, f_end_ = files_to_read.size(); f!=f_end_; ++f)
    read_hist_w (hist, files_to_read[f], 0.0);

  double baseline = average(hist, time_pretrigger_left, time_pretrigger_right);
  if (subtact_baseline) {
    subtract_baseline(hist, baseline);
    baseline = 0;
  }
  if (renormalize) {
    double integral = integrate(hist, time_pretrigger_left, time_pretrigger_right);
    hist->Scale(1.0/integral);
    baseline *= 1.0/integral;
  }
  max_val = hist->GetBinContent(hist->GetMaximumBin());
  max_val*= linear ? 1.2 : 2;

  gStyle->SetGridStyle(3);
  gStyle->SetGridColor(14);
  gStyle->SetGridWidth(1);
  gStyle->SetOptStat("");
  TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
  c_->SetGrid(); c_->SetTicks(); c_->ToggleEventStatus(); c_->ToggleToolBar();
  if (!linear)
    c_->SetLogy();
  TLegend *legend = new TLegend(0.35, 0.7, 0.9, 0.9);
  //legend->SetHeader("");
  legend->SetMargin(0.25);
  TH2F* frame = new TH2F("frame", framename.c_str(), 500, time_left, time_right, 500, linear ? 0 : y_min, max_val);
  frame->GetXaxis()->SetTitle("Time [#mus]");
  frame->GetXaxis()->SetRangeUser(20, 60);
  frame->GetYaxis()->SetTitle("PE peak counts");
  frame->Draw();

  hist->SetLineWidth(2);
  hist->SetLineColor(palette_major[0]);
  hist->Draw("hist Lsame");

  legend->AddEntry(hist, framename.c_str(), "l");

  frame->Draw("sameaxis");
  legend->Draw("same");
  c_->Update();
  
  print_hist(hist, output);

  ensure_file(output_fig);
	c_->SaveAs(output_fig.c_str(), "png");
  delete c_;
  return 0;
}
