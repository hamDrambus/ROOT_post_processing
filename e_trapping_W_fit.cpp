//Requires post_processing.so (.x init.cpp)

#define PAIR std::pair<double, double>


void SetParLimits(TF1* f, int n, double lower, double upper) {
	if (lower!=upper)
		f->SetParLimits(n, std::min(lower, upper), std::max(lower, upper));
	else
		f->FixParameter(n, lower);
}

void SetParLimits(TF1* f, int n, double value) {
	f->FixParameter(n, value);
}

//Normalized before being used in FittingF
std::map<double, DataVector> electron_En_distributions;
//Number of collisions for 10 electrons through 1e-3 m drift gap, not normalized
std::map<double, double> electron_collision_rate;

double FractionAboveThreshold(DataVector& distr, double threshold) {
	std::size_t sz = distr.size();
	if (sz < 2)
		return 0;
	double integral = 0;
	bool above_thresold = false;
	double dx = 0;
	for (std::size_t i = 0, i_end_ = sz; i != i_end_; ++i) {
		if (!above_thresold && distr.getX(i) < threshold)
			continue;
		if (!above_thresold) {
			dx = (i == 0 ? 0 : distr.getX(i) - threshold) +
				(i == (i_end_ - 1) ? 0 : 0.5*(distr.getX(i + 1) - distr.getX(i)));
			above_thresold = true;
		} else {
			dx = (i == 0 ? 0 : distr.getX(i) - distr.getX(i - 1)) +
				(i == (i_end_ - 1) ? 0 : 0.5*(distr.getX(i + 1) - distr.getX(i)));
		}
		integral += dx * distr.getY(i);
	}
	return integral;
}

//XS growth from threshold to threshold + width according to ((E-E0)/width)^power
//power should be <=1 (physical case)
double FractionAboveThreshold(DataVector& distr, double threshold, double width, double power) {
	std::size_t sz = distr.size();
	if (sz < 2)
		return 0;
	double integral = 0;
	bool above_thresold = false;
	double dx = 0;
	for (std::size_t i = 0, i_end_ = sz; i != i_end_; ++i) {
		if (!above_thresold && distr.getX(i) < threshold)
			continue;
		if (!above_thresold) {
			dx = (i == 0 ? 0 : distr.getX(i) - threshold) +
				(i == (i_end_ - 1) ? 0 : 0.5*(distr.getX(i + 1) - distr.getX(i)));
			above_thresold = true;
		} else {
			dx = (i == 0 ? 0 : distr.getX(i) - distr.getX(i - 1)) +
				(i == (i_end_ - 1) ? 0 : 0.5*(distr.getX(i + 1) - distr.getX(i)));
		}
		double y_effective = distr.getY(i);
		double E = distr.getX(i);
		if (E < (threshold + width)) {
			y_effective *= std::pow((E - threshold)/width, power);
		}
		integral += dx * y_effective;
	}
	return integral;
}

Double_t FittingF (Double_t *x, Double_t *par) {
	//x[0] - electric field in Td
	//par[0] - amplitude
  //par[1] - threshold, in eV
	std::map<double, DataVector>::iterator entry = electron_En_distributions.find(x[0]);
	if (electron_En_distributions.end() == entry) {
		throw std::runtime_error(std::string("Error: FittingF called on undefined field ") + dbl_to_str(x[0], 1) + " Td");
	}
	std::map<double, double>::iterator entry_N = electron_collision_rate.find(x[0]);
	if (electron_collision_rate.end() == entry_N) {
		throw std::runtime_error(std::string("Error: FittingF: colision rate is undefined for field ") + dbl_to_str(x[0], 1) + " Td");
	}
	double Fr = FractionAboveThreshold(entry->second, par[1]);
	//double Vdrift = Vdr_from_E(E_from_Td(x[0])); //~1e5 cm/s
	//if (0 == Vdrift)
	//	return 0;
  //return std::exp(-par[0] * Fr / Vdrift);
	return std::exp(-par[0] * Fr * entry_N->second);
}

Double_t FittingF_smooth (Double_t *x, Double_t *par) {
	//x[0] - electric field in Td
	//par[0] - amplitude
  //par[1] - threshold, in eV
	//par[2] - threshold growth width
	//par[3] - threshold behavior power, probably better to be fixed
	std::map<double, DataVector>::iterator entry = electron_En_distributions.find(x[0]);
	if (electron_En_distributions.end() == entry) {
		throw std::runtime_error(std::string("Error: FittingF called on undefined field ") + dbl_to_str(x[0], 1) + " Td");
	}
	std::map<double, double>::iterator entry_N = electron_collision_rate.find(x[0]);
	if (electron_collision_rate.end() == entry_N) {
		throw std::runtime_error(std::string("Error: FittingF: colision rate is undefined for field ") + dbl_to_str(x[0], 1) + " Td");
	}
	double Fr = FractionAboveThreshold(entry->second, par[1], par[2], par[3]);
	//double Vdrift = Vdr_from_E(E_from_Td(x[0])); //~1e5 cm/s
	//if (0 == Vdrift)
	//	return 0;
  //return std::exp(-par[0] * Fr / Vdrift);
	return std::exp(-par[0] * Fr * entry_N->second);
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

int e_trapping_W_fit (void) {
	//This part is initializing DataVector Vdrift from post_processing.so
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
	//End of initializing post_processing.so

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	gStyle->SetOptStat("nemr");
	gStyle->SetEndErrorSize(3);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::vector<Color_t> palette_major = {kBlack, kGreen, kRed, kBlue, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kGreen -2, kMagenta, kAzure + 10, kMagenta+3, kOrange - 7, kOrange + 6};
	double max_val = 0;
	bool linear = true;

	std::string framename = "Fit of Ar2- threshold from experimental W";

	std::vector<double> Tds = {5.1, 5.9, 6.8, 7.6, 8.5};
	std::vector<double> Ws =     {0.740688, 0.587782, 0.450438, 0.354056, 0.273268};
	std::vector<double> errors = {0.0855755, 0.0829716, 0.0693931, 0.0537868, 0.0461788};
	std::vector<double> Ncollisions = {67250175, 63424467, 57687149, 52448809, 47097010}; //4-7e7
	std::string folder = "simulation/";
	std::vector<std::string> filenames = {"Table_Ec_5.1Td.dat", "Table_Ec_5.9Td.dat",
			"Table_Ec_6.8Td.dat", "Table_Ec_7.6Td.dat", "Table_Ec_8.5Td.dat"};

	for(std::size_t i = 0, i_end_ = Tds.size(); i != i_end_; ++i) {
		DataVector temp(1, 2);
		temp.set_out_value(0);
		std::ifstream str;
		str.open(folder + filenames[i]);
		temp.read(str, false);
		if (temp.size() == 0) {
			std::cout<<"Failed to load Ec distribution: \""<<folder + filenames[i] << "\""<<std::endl;
			return -1;
		} else {
			std::cout<<"Read \""<<folder + filenames[i] << "\""<<std::endl;
		}
		electron_En_distributions[Tds[i]] = temp;
		electron_collision_rate[Tds[i]] = Ncollisions[i];
	}

	std::cout<<"e distributions are defined for:"<<std::endl;
	for (std::map<double, DataVector>::iterator i = electron_En_distributions.begin();
	 		i!=electron_En_distributions.end(); ++i) {
		std::cout<<i->first<<"\t";
	}
	std::cout<<std::endl;


	TGraphErrors* graph = CreateGraph(Tds, Ws, errors);
  if (NULL!=graph) {
		max_val = std::max(max_val, *std::max_element(Ws.begin(), Ws.end()));
		graph->SetMarkerStyle(kFullSquare);
		graph->SetMarkerSize(2);
		graph->SetLineWidth(3);
		graph->SetMarkerColor(palette_major[0]);
	}

	//TF1* fit_f = new TF1("func", FittingF, 4.5, 9, 2);
	//fit_f->SetParNames("Factor", "threshold");
	//SetParLimits(fit_f, 0, 5e-8, 9e-6);
	//SetParLimits(fit_f, 1, 8, 13);

	TF1* fit_f = new TF1("func", FittingF_smooth, 4.5, 9, 4);
	fit_f->SetParNames("Factor", "threshold", "width", "power");
	SetParLimits(fit_f, 0, 5e-8, 9e-6);
	SetParLimits(fit_f, 1, 11.5, 11.5);
	SetParLimits(fit_f, 2, 0.3, 10);
	SetParLimits(fit_f, 3, 0.1, 3);
	if (graph)
		graph->Fit(fit_f, "NREF");

	std::vector<double> Ws_fitted;
	for (std::size_t i = 0, i_end_ = Tds.size(); i!=i_end_; ++i)
		Ws_fitted.push_back(fit_f->Eval(Tds[i]));

	TGraph* graph_fit = CreateGraph(Tds, Ws_fitted);
  if (NULL!=graph_fit) {
		max_val = std::max(max_val, *std::max_element(Ws_fitted.begin(), Ws_fitted.end()));
		graph_fit->SetMarkerStyle(kOpenCircle);
		graph_fit->SetMarkerSize(2);
		graph_fit->SetLineWidth(3);
		graph_fit->SetMarkerColor(palette_major[1]);
	}

	std::string threshold1 = dbl_to_str(fit_f->GetParameter(1), 2);//in eV
	std::string threshold1_error = dbl_to_str(fit_f->GetParError(1), 2);//in eV
	std::string width1 = dbl_to_str(fit_f->GetParameter(2), 2);//in eV
	std::string width1_error = dbl_to_str(fit_f->GetParError(2), 2);//in eV
	std::string power1 = dbl_to_str(fit_f->GetParameter(3), 2);
	std::string power1_error = dbl_to_str(fit_f->GetParError(3), 2);

	TF1* fit_f2 = new TF1("func2", FittingF_smooth, 4.5, 9, 4);
	fit_f2->SetParNames("Factor", "threshold", "width", "power");
	SetParLimits(fit_f2, 0, 5e-8, 9e-6);
	SetParLimits(fit_f2, 1, 11.5, 11.5);
	SetParLimits(fit_f2, 2, 0.3, 10);
	SetParLimits(fit_f2, 3, 0.2, 3); //1 - linear behavior near threshold
	if (graph)
		graph->Fit(fit_f2, "NREF");

	std::vector<double> Ws_fitted2;
	for (std::size_t i = 0, i_end_ = Tds.size(); i!=i_end_; ++i)
		Ws_fitted2.push_back(fit_f2->Eval(Tds[i]));

	TGraph* graph_fit2 = CreateGraph(Tds, Ws_fitted2);
	if (NULL!=graph_fit2) {
		max_val = std::max(max_val, *std::max_element(Ws_fitted2.begin(), Ws_fitted2.end()));
		graph_fit2->SetMarkerStyle(kOpenSquare);
		graph_fit2->SetMarkerSize(2);
		graph_fit2->SetLineWidth(3);
		graph_fit2->SetMarkerColor(palette_major[2]);
	}

	std::string threshold2 = dbl_to_str(fit_f2->GetParameter(1), 2);//in eV
	std::string threshold2_error = dbl_to_str(fit_f2->GetParError(1), 2);//in eV
	std::string width2 = dbl_to_str(fit_f2->GetParameter(2), 2);//in eV
	std::string width2_error = dbl_to_str(fit_f2->GetParError(2), 2);//in eV
	std::string power2 = dbl_to_str(fit_f2->GetParameter(3), 2);
	std::string power2_error = dbl_to_str(fit_f2->GetParError(3), 2);

	TF1* fit_f3 = new TF1("func3", FittingF_smooth, 4.5, 9, 4);
	fit_f3->SetParNames("Factor", "threshold", "width", "power");
	SetParLimits(fit_f3, 0, 5e-8, 9e-6);
	SetParLimits(fit_f3, 1, 11.5, 11.5);
	SetParLimits(fit_f3, 2, 0.05, 5);
	SetParLimits(fit_f3, 3, 0.2, 3); //1 - linear behavior near threshold
	if (graph)
		graph->Fit(fit_f3, "NREF");

	std::vector<double> Ws_fitted3;
	for (std::size_t i = 0, i_end_ = Tds.size(); i!=i_end_; ++i)
		Ws_fitted3.push_back(fit_f3->Eval(Tds[i]));

	TGraph* graph_fit3 = CreateGraph(Tds, Ws_fitted3);
	if (NULL!=graph_fit3) {
		max_val = std::max(max_val, *std::max_element(Ws_fitted3.begin(), Ws_fitted3.end()));
		graph_fit3->SetMarkerStyle(kOpenTriangleUp);
		graph_fit3->SetMarkerSize(2);
		graph_fit3->SetLineWidth(3);
		graph_fit3->SetMarkerColor(palette_major[3]);
	}

	std::string threshold3 = dbl_to_str(fit_f3->GetParameter(1), 2);//in eV
	std::string threshold3_error = dbl_to_str(fit_f3->GetParError(1), 2);//in eV
	std::string width3 = dbl_to_str(fit_f3->GetParameter(2), 2);//in eV
	std::string width3_error = dbl_to_str(fit_f3->GetParError(2), 2);//in eV
	std::string power3 = dbl_to_str(fit_f3->GetParameter(3), 2);
	std::string power3_error = dbl_to_str(fit_f3->GetParError(3), 2);

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
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, 4.5, 9, 500, 0, max_val);
	frame->GetXaxis()->SetTitle("E/N [Td]");
	frame->GetYaxis()->SetTitle("W_{pass}");
	frame->Draw();

	if (graph)
		graph->Draw("p");
  if (graph_fit)
    graph_fit->Draw("p same");
	if (graph_fit2)
    graph_fit2->Draw("p same");
	if (graph_fit3)
    graph_fit3->Draw("p same");

	if (graph)
		legend->AddEntry(graph, (std::string("W from experiment")).c_str(), "p");
	if (graph_fit)
		//legend->AddEntry(graph_fit, (std::string("Fit (Ar_{2}^{-} threshold="+ threshold +"#pm" + threshold_error + " eV)")).c_str(), "p");
		legend->AddEntry(graph_fit,
			(std::string("(#frac{E - "+ threshold1 +"#pm" + threshold1_error + " eV}{"
			+ width1 + "#pm" + width1_error + " eV})^{"+power1+"#pm"+power1_error+"}")).c_str(), "p");
	if (graph_fit2)
		legend->AddEntry(graph_fit2,
			(std::string("(#frac{E - "+ threshold2 +"#pm" + threshold2_error + " eV}{"
			+ width2 + "#pm" + width2_error + " eV})^{"+power2+"#pm"+power2_error+"}")).c_str(), "p");
	if (graph_fit3)
		legend->AddEntry(graph_fit3,
			(std::string("(#frac{E - "+ threshold3 +"#pm" + threshold3_error + " eV}{"
			+ width3 + "#pm" + width3_error + " eV})^{"+power3+"#pm"+power3_error+"}")).c_str(), "p");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();

  return 0;
}
