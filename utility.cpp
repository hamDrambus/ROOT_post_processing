//LOAD AFTER init.cpp loading "libpost_processing.so"

//The core functionality of this utility was movwd to libpost_processing.so
//because calculating t_drift through gaseous argon is required
//for setting trigger adjustment time window (shaping) 

//ROOT wrappers:
Double_t drift_time_as_f_kV (Double_t *x, Double_t *par) {
  //x[0] - kV
  //par[0] - gas Ar layer in [cm] (fixed)
  //par[1] - a linear adjustment for drift time;
  return par[1]* drift_time_from_kV(x[0], par[0]);
}

bool load_t_drift_data(std::string file, std::vector<double> &Vs, std::vector<double> &Ts, int y_column)
{
	Vs.clear();
	Ts.clear();
	std::ifstream str;
	str.open(file);
	if (!str.is_open()) {
		std::cerr << "Error: Failed to open file \"" << file << "\"!" << std::endl;
		return false;
	}
	std::string line, word;
	int line_n = 0;
	while (!str.eof()) {
		std::getline(str, line);
		++line_n;
		if (line.size() >= 2) //Ignore simple c style comment
			if ((line[0] == '/') && (line[1] == '/'))
				continue;
		int column = 1;
		word = strtoken(line, "\t ");
		if (word.empty())
			continue;
		double kV = std::stod(word);
		while (column < y_column && !word.empty()) {
			word = strtoken(line, "\t ");
			++column;
		}
		if (word.empty())
			continue;
		double tau = std::stod(word);
		Vs.push_back(kV);
		Ts.push_back(tau);
	}
	return true;
}

int utility (void) {
	std::ifstream str;
	str.open(Vdrift_data_fname);
	if (!str.is_open()) {
		std::cerr << "Error: Failed to open file with e drift velocity data \"" << Vdrift_data_fname << "\"!" << std::endl;
		return -1;
	}
	Vdrift.read(str);
	Vdrift.setOrder(1);
	Vdrift.setNused(2);
	Vdrift.use_leftmost(true); //how to behave if Td is outside of data Tds range
	Vdrift.use_rightmost(true);
	str.close();

    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1300, DEF_H = 700;
    
	std::vector<Color_t> palette_major = {kBlack, kGreen, kRed, kBlue, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kGreen -2, kMagenta, kAzure + 10, kMagenta+3, kOrange - 7, kOrange + 6};
    double max_val = 0;
	double min_V0 = 7;
	bool linear = true;
	std::string framename = "190404 fast component FWHMs";
    
	std::vector<double> kVs, Ts;
	load_t_drift_data("190404/190404_half_widths.txt", kVs, Ts, 4); //4 - SiPMs, 3 - fPMTs
	Int_t size = std::min(kVs.size(), Ts.size());
	Double_t *xs = NULL, *ys = NULL;
	if (size > 0) {
		xs = new Double_t[size];
		ys = new Double_t[size];
		for (Int_t i = 0; i != size; ++i) {
			xs[i] = kVs[i];
			ys[i] = Ts[i];
			max_val = std::max(max_val, ys[i]);
		}
	}
	TGraph* SiPM_data = new TGraph(size, xs, ys);
	SiPM_data->SetMarkerStyle(kFullSquare);
	SiPM_data->SetMarkerSize(2);

	load_t_drift_data("190404/190404_half_widths.txt", kVs, Ts, 3); //4 - SiPMs, 3 - fPMTs
	size = std::min(kVs.size(), Ts.size());
	if (NULL!=xs)
		delete[] xs;
	if (NULL != ys)
		delete[] ys;
	xs = NULL; ys = NULL;
	if (size > 0) {
		xs = new Double_t[size];
		ys = new Double_t[size];
		for (Int_t i = 0; i != size; ++i) {
			xs[i] = kVs[i];
			ys[i] = Ts[i];
			max_val = std::max(max_val, ys[i]);
		}
	}
	TGraph* fPMTs_data = new TGraph(size, xs, ys);
	fPMTs_data->SetMarkerStyle(kFullCircle);
	fPMTs_data->SetMarkerSize(2);
	fPMTs_data->SetMarkerColor(palette_major[5]);

	TF1* time_18mm = new TF1("func1", drift_time_as_f_kV, 2, 22, 2);
	time_18mm->SetParNames("gas Ar L", "time factor");
	time_18mm->FixParameter(0, 1.8);
	time_18mm->FixParameter(1, 1.0);
	time_18mm->SetLineColor(palette_major[2]);

	TF1* time_Xmm = new TF1("func2", drift_time_as_f_kV, 2, 22, 2);
	time_Xmm->SetParNames("gas Ar L", "time factor");
	time_Xmm->SetParLimits(0, 0.5, 2.2);
	time_Xmm->FixParameter(1, 1.0);
	time_Xmm->SetLineColor(palette_major[3]);
	SiPM_data->Fit(time_Xmm);

	double gap_width = 10*time_Xmm->GetParameter(0); //in mm
	std::string str_gap_width = dbl_to_str(gap_width, 2);

	max_val = std::max(time_Xmm->Eval(min_V0), max_val);
	max_val = std::max(time_18mm->Eval(min_V0), max_val);

	max_val *= 1.2;

	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
	c_->SetGrid();
	c_->SetTicks();
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, min_V0, 21, 500, 0, max_val);
	frame->GetXaxis()->SetTitle("V_{0} [kV]");
	frame->GetYaxis()->SetTitle("#tau_{D} [#mus]");
	frame->Draw();
	
	SiPM_data->Draw("p");
	fPMTs_data->Draw("p");
	time_18mm->Draw("same");
	time_Xmm->Draw("same");
	
	legend->AddEntry(SiPM_data, (std::string("190404 SiPM matrix fast component FWHM")).c_str(), "p");
	legend->AddEntry(fPMTs_data, (std::string("190404 fPMTs fast component FWHM")).c_str(), "p");
	legend->AddEntry(time_18mm, (std::string("t_{drift} for 18 mm gap")).c_str(), "l");
	legend->AddEntry(time_Xmm, (std::string("t_{drift} for SiPM best fit ("+ str_gap_width +" mm)")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();

	if (NULL!=xs)
		delete[] xs;
	if (NULL != ys)
		delete[] ys;

    return 0;
}
