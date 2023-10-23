// Run .x init.cpp before executing this script
// This script plots 2D profile of average S2 signal (in LAr) recorded by SiPM matrix.
// Created on 2023.10.23

std::pair<double, double> SiPM_channel_XY(int channel) {
  auto elem = MPPC_coords.find(channel);
  if (elem == MPPC_coords.end())
    return std::pair<double, double>(DBL_MAX, DBL_MAX);
  return elem->second;
}

std::map<int, double> ReadExperimentData(std::string filename, double voltage) {
  std::map<int, double> result;
  std::ifstream str;
  str.open(filename);
  if (!str.is_open()) {
    std::cerr<<"Error:ReadExperimentData: could not open file \""<<filename<<"\"!"<<std::endl;
    return result;
  }
  std::vector<int> chs;
  std::vector<double> Npes;
  std::size_t line_n = 0;
  std::string line, word;
  bool found = false;
  while (!str.eof() && str.is_open()) {
		std::getline(str, line);
		++line_n;
    try {
  		if (line_n == 1) { // Header line is "V 32 33 .... 59" where numbers are channel numbers
        word = strtoken(line, "\t "); // ignore first word
        while (!line.empty()) {
          word = strtoken(line, "\t ");
          if (!word.empty())
            chs.push_back(std::stoi(word));
        }
        continue;
      }
      word = strtoken(line, "\t ");
      double V = stod(word);
      if ((V - voltage) < 1e-10*max(V, voltage)) { // extract only necessary voltage from table in the file
        found = true;
        while (!line.empty()) {
          word = strtoken(line, "\t ");
          if (!word.empty())
            Npes.push_back(std::max(std::stod(word), 0.0));
        }
        break;
      }
		} catch (std::invalid_argument &e) {
    } catch (std::out_of_range &e) {
			std::cerr << "Warning:ReadExperimentData: Invalid data on line " << line_n << std::endl;
			std::cerr << e.what() << std::endl;
			return result;
		} catch (std::exception &e) {
			std::cerr << "Warning:ReadExperimentData: Unforeseen exception on line " << line_n << std::endl;
			std::cerr << e.what() << std::endl;
			return result;
		}
	}
  if (!found) {
    std::cerr<<"Warning:ReadExperimentData: voltage "<<voltage<<" V was not found\n";
    std::cerr<<"\tin the table \""<<filename<<"\"."<<std::endl;
    return result;
  }
  if (chs.size()!=Npes.size()) {
    std::cerr<<"Warning:ReadExperimentData: number of channels and number of table entries are inconsistent!\n";
    std::cerr<<"\tCheck input file \""<<filename<<"\""<<std::endl;
  }
  for (std::size_t i = 0, i_end_ = std::min(chs.size(), Npes.size()); i!=i_end_; ++i)
    result[chs[i]] = Npes[i];
  return result;
}

std::map<int, double> ReadExperimentData(std::string filename, std::string voltage) {
  std::map<int, double> result;
  double Voltage;
  try {
    Voltage = stod(voltage);
  } catch (std::exception &e) {
    std::cerr << "Error:ReadExperimentData: could not get voltage from string \"" << voltage << "\"." << std::endl;
    std::cerr << e.what() << std::endl;
    return result;
  }
  return ReadExperimentData(filename, Voltage);
}

// Plots 2D intensity if signal Npe_exp for each SiPM channel.
// Returns S2 (source) center based on weighted average.
std::pair<double, double> plot_experiment_XY(std::map<int, double> &Npe_exp, double charge, bool do_draw = true) {
  const double SiPM_pitch = 10.0;
  const double SiPM_size = 6.0;
  const int SiPM_n_rows = 5;
  double SiPM_Npe_stat_error = 1e-3;
  std::pair<double, double> xy_avg(0, 0);
  double weight = 0;
  TH2D* hist_01 = new TH2D("SiPM experimental S2 profile", "SiPM experimental S2 profile", 2*SiPM_n_rows+1,
						-SiPM_pitch * (SiPM_n_rows + 1) / 2 + SiPM_size / 2.0, SiPM_pitch * (SiPM_n_rows + 1) / 2 - SiPM_size / 2.0,
						2*SiPM_n_rows+1,
						-SiPM_pitch * (SiPM_n_rows + 1) / 2 + SiPM_size / 2.0, SiPM_pitch * (SiPM_n_rows + 1) / 2 - SiPM_size / 2.0);
  for (auto const& it : Npe_exp) {
    std::pair<double, double> XY = SiPM_channel_XY(it.first);
    if (XY.first == DBL_MAX || XY.second == DBL_MAX)
      continue;
    hist_01->Fill(XY.first, XY.second, it.second == 0 ? 0.1*SiPM_Npe_stat_error/charge : it.second/charge);
    xy_avg.first += XY.first * it.second;
    xy_avg.second += XY.second * it.second;
    weight += it.second;
  }
  // Fix missing channels to avoid systematic x,y shift. Only cases of missing ch 43 and 44 are considered.
  bool is43 = (Npe_exp.find(43) != Npe_exp.end());
  bool is44 = (Npe_exp.find(44) != Npe_exp.end());
  if (!is43 && (Npe_exp.size() == 24 || (Npe_exp.size() == 23 && !is44))) { // mising only ch 43 or only channels 43 and 44
    double Npe43 = (Npe_exp[40] * Npe_exp[42] / Npe_exp[55] +
        Npe_exp[40] * Npe_exp[58] / Npe_exp[41]) / 2.0;
    std::pair<double, double> XY = SiPM_channel_XY(43);
    xy_avg.first += XY.first * Npe43;
    xy_avg.second += XY.second * Npe43;
    weight += Npe43;
  }
  if (!is44 && (Npe_exp.size() == 24 || (Npe_exp.size() == 23 && !is43))) { // mising only ch 44 or only channels 44 and 43
    double Npe44 = (Npe_exp[57] * Npe_exp[59] / Npe_exp[56] +
        0.5*(Npe_exp[57] + Npe_exp[59])) / 2.0;
    std::pair<double, double> XY = SiPM_channel_XY(44);
    xy_avg.first += XY.first * Npe44;
    xy_avg.second += XY.second * Npe44;
    weight += Npe44;
  }
  xy_avg.first /= weight;
  xy_avg.second /= weight;
  if (do_draw) {
    TCanvas *c_00 = new TCanvas ((std::string("01_") + "SiPM experimental S2 profile").c_str(), "SiPM experimental S2 profile");
    c_00->SetGrid(); c_00->SetTicks(); c_00->ToggleEventStatus(); c_00->ToggleToolBar();
    hist_01->SetStats(false);
    hist_01->GetXaxis()->SetTitle("X (mm)");
    hist_01->GetYaxis()->SetTitle("Y (mm)");
    double title_offset_01 = hist_01->GetYaxis()->GetTitleOffset() + 1.3;
    hist_01->GetYaxis()->SetTitleOffset(title_offset_01);
    //hist_01->Draw("lego");
    hist_01->Draw("colz");
    c_00->Update();
  }
  if (!do_draw)
    hist_01->Delete();
  return xy_avg;
} 

void plot_Npe_profile(double gem_voltage, std::string exp_input_file = "231012/results_v1/SiPM_Npes.txt",
  double experiment_charge = 4.05e6, bool do_draw = true)
{
  Init_globals(false);
  gStyle->SetCanvasDefH(800);
  gStyle->SetCanvasDefW(1000);
  gErrorIgnoreLevel = 1001;
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  gStyle->SetStatH(0.3);
  gStyle->SetGridStyle(3);
  gStyle->SetGridColor(14);
  gStyle->SetGridWidth(1);
  gStyle->SetOptStat("mr");

  std::map<int, double> Npe_per_e_per_ch_exp = ReadExperimentData(exp_input_file, gem_voltage);
  plot_experiment_XY(Npe_per_e_per_ch_exp, experiment_charge, do_draw);
}