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

TGraph* CreateGraph(std::vector<double> &ixs, std::vector<double> &iys) {
	TGraph* out = NULL;
	Int_t size = std::min(ixs.size(), iys.size());
	Double_t *xs = NULL, *ys = NULL;
	if (size > 0) {
		xs = new Double_t[size];
		ys = new Double_t[size];
		for (Int_t i = 0; i != size; ++i) {
			xs[i] = ixs[i];
			ys[i] = iys[i];
		}
		out = new TGraph(size, xs, ys);
		delete [] xs;
		delete [] ys;
	}
	return out;
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
	std::string framename = "190404 adjusted trigger fast component FWHMs";
    
	std::vector<double> kVs, Ts;
	load_t_drift_data("190404/results_v3/190404_half_widths.txt", kVs, Ts, 3); //4 - SiPMs, 3 - fPMTs
	TGraph* fPMTs_data = CreateGraph(kVs, Ts);
	if (NULL!=fPMTs_data) {
		max_val = std::max(max_val, *std::max_element(Ts.begin(), Ts.end()));
		fPMTs_data->SetMarkerStyle(kFullCircle);
		fPMTs_data->SetMarkerSize(2);
		fPMTs_data->SetMarkerColor(palette_major[5]);
	}
	kVs.clear();
	Ts.clear();

	load_t_drift_data("190404/results_v3/190404_half_widths.txt", kVs, Ts, 4); //4 - SiPMs, 3 - fPMTs
	TGraph* SiPMs_data = CreateGraph(kVs, Ts);
	if (NULL!=SiPMs_data) {
		max_val = std::max(max_val, *std::max_element(Ts.begin(), Ts.end()));
		SiPMs_data->SetMarkerStyle(kFullSquare);
		SiPMs_data->SetMarkerSize(2);
		SiPMs_data->SetMarkerColor(palette_major[0]);
	}
	kVs.clear();
	Ts.clear();

	TF1* time_18mm = new TF1("func1", drift_time_as_f_kV, 2, 22, 2);
	time_18mm->SetParNames("gas Ar L", "time factor");
	time_18mm->FixParameter(0, 1.8);
	time_18mm->FixParameter(1, 1.0);
	time_18mm->SetLineColor(palette_major[2]);

	TF1* time_Xmm = new TF1("func2", drift_time_as_f_kV, 2, 22, 2);
	time_Xmm->SetParNames("gas Ar L", "time factor");
	time_Xmm->SetParLimits(0, 0.5, 2.2);
	time_Xmm->FixParameter(1, 1);
	time_Xmm->SetLineColor(palette_major[1]);
	if (fPMTs_data)
		fPMTs_data->Fit(time_Xmm);

	TF1* time_Xmm2 = new TF1("func3", drift_time_as_f_kV, 2, 22, 2);
	time_Xmm2->SetParNames("gas Ar L", "time factor");
	time_Xmm2->SetParLimits(0, 0.5, 2.2);
	time_Xmm2->FixParameter(1, 1);
	time_Xmm2->SetLineColor(palette_major[3]);
	if (SiPMs_data)
		SiPMs_data->Fit(time_Xmm2);

	std::string gap_width_fPMTs = dbl_to_str(10*time_Xmm->GetParameter(0), 2);//in mm
	std::string gap_width_SiPMs = dbl_to_str(10*time_Xmm2->GetParameter(0), 2);//in mm

	max_val = std::max(time_Xmm->Eval(min_V0), max_val);
	max_val = std::max(time_Xmm2->Eval(min_V0), max_val);
	max_val = std::max(time_18mm->Eval(min_V0), max_val);

	max_val *= 1.2;

	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_ = new TCanvas ((std::string(" ") + framename).c_str(), (std::string(" ") + framename).c_str(), DEF_W, DEF_H);
	c_->SetGrid();
	c_->SetTicks();
	c_->ToggleEventStatus();
    c_->ToggleToolBar();
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, min_V0, 21, 500, 0, max_val);
	frame->GetXaxis()->SetTitle("V_{0} [kV]");
	frame->GetYaxis()->SetTitle("#tau_{D} [#mus]");
	frame->Draw();
	
	if (SiPMs_data)
		SiPMs_data->Draw("p");
	if (fPMTs_data)	
		fPMTs_data->Draw("p");
	time_18mm->Draw("same");
	if (SiPMs_data)
		time_Xmm->Draw("same");
	if (fPMTs_data)
		time_Xmm2->Draw("same");
	
	if (SiPMs_data)
		legend->AddEntry(SiPMs_data, (std::string("190404 v3 SiPM matrix fast component FWHM")).c_str(), "p");
	if (fPMTs_data)	
		legend->AddEntry(fPMTs_data, (std::string("190404 v3 fPMTs fast component FWHM")).c_str(), "p");
	legend->AddEntry(time_18mm, (std::string("t_{drift} for 18 mm gap")).c_str(), "l");
	if (SiPMs_data)
		legend->AddEntry(time_Xmm2, (std::string("t_{drift} for SiPMs best fit ("+ gap_width_SiPMs +" mm)")).c_str(), "l");
	if (fPMTs_data)
		legend->AddEntry(time_Xmm, (std::string("t_{drift} for fPMTs best fit ("+ gap_width_fPMTs +" mm)")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();


    return 0;
}
