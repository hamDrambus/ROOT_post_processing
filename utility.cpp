//LOAD AFTER init.cpp loading "libpost_processing.so"

//The core functionality of this utility was moved to libpost_processing.so
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

	std::vector<Color_t> palette_major = {kBlack, kRed, kBlue, kGreen, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kGreen -2, kMagenta, kAzure + 10, kMagenta+3, kOrange - 7, kOrange + 6};
  //qewr - for fast Ctrl+F
  double max_val = 0;
	double min_V0 = 3, max_V0 = 10;
	bool linear = true;
  bool fit_all = false;
  bool fit1 = fit_all, fit2 = fit_all, fit3 = fit_all; //Use fit for Nth graph
  bool free_offset = false; //Use offset as free fitting parameter
  bool draw_18mm = false;
  double error_factor = 2.0;
	std::string framename = "Data without WLS: EL gap thickness measurement";
  //std::string framename = "\\hbox{Данные без ТФБ: измерение толщины ЭЛ зазора 2020.10.15}";

	std::vector<double> kVs1, Ts1, errs1;
  std::vector<double> kVs2, Ts2, errs2;
  std::vector<double> kVs3, Ts3, errs3;

	load_column_data("190404/190404_FWHMs.txt", kVs1, 1);
	load_column_data("190404/190404_FWHMs.txt", Ts1, 5);
	load_column_data("190404/190404_FWHMs.txt", errs1, 6);
  load_column_data("210429/210429_FWHMs.txt", kVs2, 1);
	load_column_data("210429/210429_FWHMs.txt", Ts2, 5);
	load_column_data("210429/210429_FWHMs.txt", errs2, 6);
  load_column_data("210513/210513_FWHMs.txt", kVs3, 1);
	load_column_data("210513/210513_FWHMs.txt", Ts3, 5);
	load_column_data("210513/210513_FWHMs.txt", errs3, 6);
  /*
  load_column_data("210128/210128_FWHMs.txt", kVs1, 1);
	load_column_data("210128/210128_FWHMs.txt", Ts1, 5);
	load_column_data("210128/210128_FWHMs.txt", errs1, 6);
  load_column_data("210121/210121_FWHMs.txt", kVs2, 1);
	load_column_data("210121/210121_FWHMs.txt", Ts2, 5);
	load_column_data("210121/210121_FWHMs.txt", errs2, 6);
  load_column_data("201217/201217_FWHMs.txt", kVs3, 1);
	load_column_data("201217/201217_FWHMs.txt", Ts3, 5);
	load_column_data("201217/201217_FWHMs.txt", errs3, 6);
  */
  for (std::size_t i=0, i_end_=errs1.size(); i!=i_end_; ++i)
    errs1[i] *= error_factor;
  for (std::size_t i=0, i_end_=errs2.size(); i!=i_end_; ++i)
    errs2[i] *= error_factor;
  for (std::size_t i=0, i_end_=errs3.size(); i!=i_end_; ++i)
    errs3[i] *= error_factor;

	TGraphErrors* graph1 = CreateGraph(kVs1, Ts1, errs1);
  TGraphErrors* graph2 = CreateGraph(kVs2, Ts2, errs2);
  TGraphErrors* graph3 = CreateGraph(kVs3, Ts3, errs3);
	if (NULL!=graph1) {
		max_val = std::max(max_val, *std::max_element(Ts1.begin(), Ts1.end()));
		graph1->SetMarkerStyle(kFullSquare);
		graph1->SetMarkerSize(2);
		graph1->SetLineWidth(3);
    graph1->SetLineColor(palette_major[0]);
		graph1->SetMarkerColor(palette_major[0]);
	}
  if (NULL!=graph2) {
		max_val = std::max(max_val, *std::max_element(Ts2.begin(), Ts2.end()));
		graph2->SetMarkerStyle(kFullCircle);
		graph2->SetMarkerSize(2);
		graph2->SetLineWidth(3);
    graph2->SetLineColor(palette_major[1]);
		graph2->SetMarkerColor(palette_major[1]);
	}
  if (NULL!=graph3) {
		max_val = std::max(max_val, *std::max_element(Ts3.begin(), Ts3.end()));
		graph3->SetMarkerStyle(kFullTriangleDown);
		graph3->SetMarkerSize(2);
		graph3->SetLineWidth(3);
    graph3->SetLineColor(palette_major[2]);
		graph3->SetMarkerColor(palette_major[2]);
	}
	kVs1.clear();
	Ts1.clear();
  errs1.clear();
  kVs2.clear();
	Ts2.clear();
  errs2.clear();
  kVs2.clear();
	Ts2.clear();
  errs2.clear();

	TF1* fit_18mm = new TF1("func0", drift_time_as_f_kV, min_V0, max_V0, 3);
	fit_18mm->SetParNames("gas Ar L", "time factor", "offset");
	fit_18mm->FixParameter(0, 1.8);
	fit_18mm->FixParameter(1, 1.0);
  fit_18mm->FixParameter(2, 0);
	fit_18mm->SetLineColor(palette_major[0]);
  fit_18mm->SetLineStyle(2);

	TF1* fit_Xmm1 = new TF1("func1", drift_time_as_f_kV, min_V0, max_V0, 3);
	fit_Xmm1->SetParNames("gas Ar L", "time factor", "offset");
	fit_Xmm1->SetParLimits(0, 0.3, 2.2);
  fit_Xmm1->FixParameter(1, 1);
  if (free_offset)
    fit_Xmm1->SetParLimits(2, 0, 1.0);
  else
    fit_Xmm1->FixParameter(2, 0);
	fit_Xmm1->SetLineColor(palette_major[0]);
  TF1* fit_Xmm2 = new TF1("func2", drift_time_as_f_kV, min_V0, max_V0, 3);
	fit_Xmm2->SetParNames("gas Ar L", "time factor", "offset");
	fit_Xmm2->SetParLimits(0, 0.3, 2.2);
	fit_Xmm2->FixParameter(1, 1);
  if (free_offset)
    fit_Xmm2->SetParLimits(2, 0, 1.0);
  else
    fit_Xmm2->FixParameter(2, 0);
	fit_Xmm2->SetLineColor(palette_major[1]);
  TF1* fit_Xmm3 = new TF1("func3", drift_time_as_f_kV, min_V0, max_V0, 3);
	fit_Xmm3->SetParNames("gas Ar L", "time factor", "offset");
	fit_Xmm3->SetParLimits(0, 0.3, 2.2);
	fit_Xmm3->FixParameter(1, 1);
  if (free_offset)
    fit_Xmm3->SetParLimits(2, 0, 1.0);
  else
    fit_Xmm3->FixParameter(2, 0);
	fit_Xmm3->SetLineColor(palette_major[2]);
	if (graph1 && fit1)
		graph1->Fit(fit_Xmm1, "NREF");
  if (graph2 && fit2)
		graph2->Fit(fit_Xmm2, "NREF");
  if (graph1 && fit1)
		graph3->Fit(fit_Xmm3, "NREF");

	std::string gap_width_1 = dbl_to_str(10*fit_Xmm1->GetParameter(0), 1);//in mm
	std::string gap_width_error_1 = dbl_to_str(10*fit_Xmm1->GetParError(0), 1);//in mm
  std::string gap_width_2 = dbl_to_str(10*fit_Xmm2->GetParameter(0), 1);//in mm
	std::string gap_width_error_2 = dbl_to_str(10*fit_Xmm2->GetParError(0), 1);//in mm
  std::string gap_width_3 = dbl_to_str(10*fit_Xmm3->GetParameter(0), 1);//in mm
	std::string gap_width_error_3 = dbl_to_str(10*fit_Xmm3->GetParError(0), 1);//in mm

  if (draw_18mm)
    max_val = std::max(fit_18mm->Eval(min_V0), max_val);
  if (graph1&&fit1)
    max_val = std::max(fit_Xmm1->Eval(min_V0), max_val);
  if (graph2&&fit2)
    max_val = std::max(fit_Xmm2->Eval(min_V0), max_val);
  if (graph3&&fit3)
    max_val = std::max(fit_Xmm3->Eval(min_V0), max_val);

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
  //adsf - for fast Ctrl+F
  max_val = 6.8;
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, min_V0, max_V0, 500, 0, max_val);
	//frame->GetXaxis()->SetTitle("V_{0} [kV]");
  frame->GetXaxis()->SetTitle("Nominal E/N [Td]");
	frame->GetYaxis()->SetTitle("T_{drift} [#mus]");
	//frame->GetXaxis()->SetTitle("\\it{V_{0}},\\hbox{ кВ}");
	//frame->GetYaxis()->SetTitle("\\hbox{Время дрейфа, мкс}");
	frame->Draw();

	if (graph1)
		graph1->Draw("p");
  if (graph2)
    graph2->Draw("p same");
  if (graph3)
    graph3->Draw("p same");
  if (draw_18mm)
    fit_18mm->Draw("same");
	if (graph1 && fit1)
		fit_Xmm1->Draw("same");
  if (graph2 && fit2)
		fit_Xmm2->Draw("same");
  if (graph3 && fit3)
		fit_Xmm3->Draw("same");


	if (graph1)
		legend->AddEntry(graph1, (std::string("190404 (Cd, 18mm nominal gap) SiPMs fast comp. FWHM")).c_str(), "p");
		//legend->AddEntry(graph1, (std::string("\\hbox{Длительность быстрой компоненты для матрицы Si-ФЭУ}")).c_str(), "p");
  if (graph2)
		legend->AddEntry(graph2, (std::string("210429 (Cd, 12.1mm nominal gap) SiPMs fast comp. FWHM")).c_str(), "p");
  if (graph3)
		legend->AddEntry(graph3, (std::string("210513 (Cd, 18.4mm nominal gap) SiPMs fast comp. FWHM")).c_str(), "p");
  /*
  if (graph1)
		legend->AddEntry(graph1, (std::string("210128 (Pu, 17.7mm nominal gap) SiPMs fast comp. FWHM")).c_str(), "p");
  if (graph2)
		legend->AddEntry(graph2, (std::string("210121 (Pu, 12.7mm nominal gap) SiPMs fast comp. FWHM")).c_str(), "p");
  if (graph3)
		legend->AddEntry(graph3, (std::string("201217 (Pu, 4.4mm nominal gap) SiPMs fast comp. FWHM")).c_str(), "p");
  */
  if (draw_18mm)
    legend->AddEntry(fit_18mm, (std::string("T_{drift} for 18 mm gap")).c_str(), "l");
	//legend->AddEntry(fit_18mm, (std::string("\\hbox{Время дрейфа для 13 мм зазора}")).c_str(), "l");

  if (graph1 && fit1)
		legend->AddEntry(fit_Xmm1, (std::string("190404 fit (Gap thickness="+ gap_width_1 +"#pm" + gap_width_error_1 + " mm)")).c_str(), "l");
		//legend->AddEntry(time_Xmm2, (std::string("\\hbox{Аппроксимация (толщина ЭЛ зазора = "+ gap_width_SiPMs +" }\\pm\\hbox{ " + gap_width_SiPMs_error + " мм)}")).c_str(), "l");
  if (graph2 && fit2)
		legend->AddEntry(fit_Xmm2, (std::string("210429 fit (Gap thickness="+ gap_width_2 +"#pm" + gap_width_error_2 + " mm)")).c_str(), "l");
  if (graph3 && fit3)
		legend->AddEntry(fit_Xmm3, (std::string("210513 fit (Gap thickness="+ gap_width_3 +"#pm" + gap_width_error_3 + " mm)")).c_str(), "l");
  /*
  if (graph1 && fit1)
		legend->AddEntry(fit_Xmm1, (std::string("210128 fit (Gap thickness="+ gap_width_1 +"#pm" + gap_width_error_1 + " mm)")).c_str(), "l");
  if (graph2 && fit2)
		legend->AddEntry(fit_Xmm2, (std::string("210121 fit (Gap thickness="+ gap_width_2 +"#pm" + gap_width_error_2 + " mm)")).c_str(), "l");
  if (graph3 && fit3)
		legend->AddEntry(fit_Xmm3, (std::string("201217 fit (Gap thickness="+ gap_width_3 +"#pm" + gap_width_error_3 + " mm)")).c_str(), "l");
  */
	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();


  return 0;
}
