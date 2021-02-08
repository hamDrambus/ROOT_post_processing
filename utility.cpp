//LOAD AFTER init.cpp loading "libpost_processing.so"

//The core functionality of this utility was movwd to libpost_processing.so
//because calculating t_drift through gaseous argon is required
//for setting trigger adjustment time window (shaping)

//ROOT wrappers:
Double_t drift_time_as_f_kV (Double_t *x, Double_t *par) {
  //x[0] - kV
  //par[0] - gas Ar layer in [cm] (fixed)
  //par[1] - a linear adjustment for drift time;
  //par[2] - offset;
  return par[2] + par[1]* drift_time_from_kV(x[0], par[0]);
}

//column number starts from 0
bool load_column_data(std::string file, std::vector<double> &Vs, std::size_t y_column)
{
	Vs.clear();
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
		std::size_t column = 0;
		word = strtoken(line, "\t ");
		while (column < y_column && !word.empty()) {
			word = strtoken(line, "\t ");
			++column;
		}
		if (word.empty())
			continue;
		double val = std::stod(word);
		Vs.push_back(val);
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
	gStyle->SetEndErrorSize(3);
    int DEF_W = 1300, DEF_H = 700;

	std::vector<Color_t> palette_major = {kBlack, kGreen, kRed, kBlue, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kGreen -2, kMagenta, kAzure + 10, kMagenta+3, kOrange - 7, kOrange + 6};
    double max_val = 0;
	double min_V0 = 7;
	bool linear = true;
  double error_factor = 2.0;
	//std::string framename = "Data without WLS: EL gap thickness measurement";
  std::string framename = "\\hbox{Данные без ТФБ: измерение толщины ЭЛ зазора 2020.10.15}";

	std::vector<double> kVs, Ts, errors, PMT_Ts, PMT_Errs;

	load_column_data("201015/201015_FWHMs.txt", kVs, 0);
	load_column_data("201015/201015_FWHMs.txt", PMT_Ts, 0);
	load_column_data("201015/201015_FWHMs.txt", PMT_Errs, 0);
  load_column_data("201015/201015_FWHMs.txt", Ts, 3);
	load_column_data("201015/201015_FWHMs.txt", errors, 4);
  for (std::size_t i=0, i_end_=errors.size(); i!=i_end_; ++i)
    errors[i] *= error_factor;
	TGraphErrors* SiPMs_data = CreateGraph(kVs, Ts, errors);
  TGraphErrors* PMTs_data = NULL; //CreateGraph(kVs, PMT_Ts, PMT_Errs);
	if (NULL!=SiPMs_data) {
		max_val = std::max(max_val, *std::max_element(Ts.begin(), Ts.end()));
		SiPMs_data->SetMarkerStyle(kFullSquare);
		SiPMs_data->SetMarkerSize(2);
		SiPMs_data->SetLineWidth(3);
		SiPMs_data->SetMarkerColor(palette_major[0]);
	}
  if (NULL!=PMTs_data) {
		max_val = std::max(max_val, *std::max_element(PMT_Ts.begin(), PMT_Ts.end()));
		PMTs_data->SetMarkerStyle(kFullCircle);
		PMTs_data->SetMarkerSize(2);
		PMTs_data->SetLineWidth(3);
		PMTs_data->SetMarkerColor(palette_major[1]);
	}
	kVs.clear();
	Ts.clear();
  errors.clear();
  PMT_Ts.clear();
	PMT_Errs.clear();

	TF1* time_18mm = new TF1("func1", drift_time_as_f_kV, 2, 22, 3);
	time_18mm->SetParNames("gas Ar L", "time factor", "offset");
	time_18mm->FixParameter(0, 1.3);
	time_18mm->FixParameter(1, 1.0);
  time_18mm->FixParameter(2, 0);
	time_18mm->SetLineColor(palette_major[0]);
  time_18mm->SetLineStyle(2);

	TF1* time_Xmm2 = new TF1("func3", drift_time_as_f_kV, 2, 22, 3);
	time_Xmm2->SetParNames("gas Ar L", "time factor", "offset");
	time_Xmm2->SetParLimits(0, 0.5, 2.2);
	time_Xmm2->FixParameter(1, 1);
	time_Xmm2->SetParLimits(2, 0, 0.5);
	time_Xmm2->SetLineColor(palette_major[0]);
	if (SiPMs_data)
		SiPMs_data->Fit(time_Xmm2, "NREF");

	std::string gap_width_SiPMs = dbl_to_str(10*time_Xmm2->GetParameter(0), 1);//in mm
	std::string gap_width_SiPMs_error = dbl_to_str(10*time_Xmm2->GetParError(0), 1);//in mm

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
	//frame->GetXaxis()->SetTitle("V_{0} [kV]");
	//frame->GetYaxis()->SetTitle("T_{drift} [#mus]");
	frame->GetXaxis()->SetTitle("\\it{V_{0}},\\hbox{ кВ}");
	frame->GetYaxis()->SetTitle("\\hbox{Время дрейфа, мкс}");
	frame->Draw();

	if (SiPMs_data)
		SiPMs_data->Draw("p");
  if (PMTs_data)
    PMTs_data->Draw("p same");
	time_18mm->Draw("same");
	if (SiPMs_data)
		time_Xmm2->Draw("same");

	if (SiPMs_data)
		//legend->AddEntry(SiPMs_data, (std::string("SiPM-matrix fast component FWHM")).c_str(), "p");
		legend->AddEntry(SiPMs_data, (std::string("\\hbox{Длительность быстрой компоненты для матрицы Si-ФЭУ}")).c_str(), "p");
	if (PMTs_data)
		legend->AddEntry(PMTs_data, (std::string("4PMT fast component FWHM")).c_str(), "p");
	//legend->AddEntry(time_18mm, (std::string("T_{drift} for 18 mm gap")).c_str(), "l");
	legend->AddEntry(time_18mm, (std::string("\\hbox{Время дрейфа для 13 мм зазора}")).c_str(), "l");
	if (SiPMs_data)
		//legend->AddEntry(time_Xmm2, (std::string("Best fit (EL gap thickness="+ gap_width_SiPMs +"#pm" + gap_width_SiPMs_error + " mm)")).c_str(), "l");
		legend->AddEntry(time_Xmm2, (std::string("\\hbox{Аппроксимация (толщина ЭЛ зазора = "+ gap_width_SiPMs +" }\\pm\\hbox{ " + gap_width_SiPMs_error + " мм)}")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();


  return 0;
}
