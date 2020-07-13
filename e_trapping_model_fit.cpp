//Requires post_processing.so (.x init.cpp)
//Created on 2020.07.03
//This code uses simple theoretical model of e_trapping_simulation.cpp
//to fit experimantal data on tau_S(E/N) and contribution_S(E/N) (Fr(E/N)) simulteneously
//using tau_Ar_state(const) and probability for electron to pass trough EL gap without delay W_pass(E/N)
//as free parameters. Two key points:
//1) Both tau_S(E/N) and contribution_S(E/N) are fitted simulteneously by a single model
//2) The simple model can't be used to conviniently calculate tau_S(tau_Ar, W_pass(E/N)) and Fr(tau_Ar, W_pass(E/N))
//because fit of MC-generated signals is required which is unstable (and slow, see e_trapping_simulation.cpp).
//Instead, the model was used to calculate tau_Ar and W_pass(E/N) describing the experimental data in
//several (9) points (combinations of tau_S(E/N)+-Error and Fr(E/N)+-Error). Using this numerical relationship,
//tau_S(tau_Ar, W_pass(E/N)) and Fr(tau_Ar, W_pass(E/N)) were extrapolated as paraboloid functions
//for each electric field. This extrapolated function is then used in global fit.

//On how to conduct simulteneous (global) fit in ROOT see:
//https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuideChapters/FittingHistograms.pdf (documentation)
//and https://root-forum.cern.ch/t/simultaneous-fitting-of-multiple-graphs/14383 (code example)

#define PAIR std::pair<double, double>

std::map<double, Transformation2D> model_extrapolation;
std::map<double, int> model_W_index;

struct GlobalChi2 {
	const ROOT::Math::IMultiGenFunction *fChi2_1;
	const ROOT::Math::IMultiGenFunction *fChi2_2;
	GlobalChi2(ROOT::Math::IMultiGenFunction &f1, ROOT::Math::IMultiGenFunction &f2) :
      fChi2_1(&f1), fChi2_2(&f2) {}

	double operator() (const double *par) const {
		//All parameters are shared. If not, correct parameter arrays must be passed to each chi2 funcion
		return (*fChi2_1)(par) + (*fChi2_2)(par);
  }
};

//Routine to check tau_S(tau_Ar, W_pass(E/N)) and Fr(tau_Ar, W_pass(E/N)) extrapolation
void Draw_contour (Transformation2D &data, std::string name) {
	//x,y = slow compoenent tau, contribution
	//u,v = model tau, W pass
	std::vector<TMarker*> points_uv, points_xy;
	std::vector<TArrow*> polyline_uv, polyline_xy, polyline_uv_fit, polyline_xy_fit;
	double x_min = DBL_MAX, x_max = -DBL_MAX, y_min = DBL_MAX, y_max = -DBL_MAX;
	double u_min = DBL_MAX, u_max = -DBL_MAX, v_min = DBL_MAX, v_max = -DBL_MAX;
	for (std::size_t i = 0, i_end_ = data.size(); i!=i_end_; ++i) {
		x_min = std::min(x_min, data.getX(i)); x_max = std::max(x_max, data.getX(i));
		y_min = std::min(y_min, data.getY(i)); y_max = std::max(y_max, data.getY(i));
		u_min = std::min(u_min, data.getU(i)); u_max = std::max(u_max, data.getU(i));
		v_min = std::min(v_min, data.getV(i)); v_max = std::max(v_max, data.getV(i));

		TMarker* point_xy = new TMarker(data.getX(i), data.getY(i), 20);
		TMarker* point_uv = new TMarker(data.getU(i), data.getV(i), 20);
		point_xy->SetMarkerSize(2);
		point_uv->SetMarkerSize(2);
		points_xy.push_back(point_xy);
		points_uv.push_back(point_uv);

		double x0 = data.getX(i), y0 = data.getY(i), x1 = data.getX((i+1)%i_end_), y1 = data.getY((i+1)%i_end_);
		TArrow* line_xy = new TArrow(x0, y0, x1, y1, 0.05, "-|>-");
		double u0 = data.getU(i), v0 = data.getV(i), u1 = data.getU((i+1)%i_end_), v1 = data.getV((i+1)%i_end_);
		TArrow* line_uv = new TArrow(u0, v0, u1, v1, 0.05, "-|>-");
		TArrow* line_uv_fit = new TArrow(data.evalU(x0, y0), data.evalV(x0, y0), data.evalU(x1, y1), data.evalV(x1, y1), 0.05, "->-");
		TArrow* line_xy_fit = new TArrow(data.evalX(u0, v0), data.evalY(u0, v0), data.evalX(u1, v1), data.evalY(u1, v1), 0.05, "->-");
		line_xy->SetLineWidth(2);
		line_xy_fit->SetLineWidth(2);
		line_xy_fit->SetLineColor(kRed);
		line_uv->SetLineWidth(2);
		line_uv_fit->SetLineWidth(2);
		line_uv_fit->SetLineColor(kRed);
		polyline_xy.push_back(line_xy);
		polyline_xy_fit.push_back(line_xy_fit);
		polyline_uv.push_back(line_uv);
		polyline_uv_fit.push_back(line_uv_fit);
	}
	int DEF_W = 800, DEF_H = 700;
	gStyle->SetOptStat("");
	TCanvas *c_xy = new TCanvas ((name + "_#tau-Fr").c_str(), (name + "_#tau-Fr").c_str(), DEF_W, DEF_H);
	c_xy->SetGrid();
	c_xy->SetTicks();
	c_xy->ToggleEventStatus();
  c_xy->ToggleToolBar();
	TH2F* frame_xy = new TH2F((name + " experimental data").c_str(), (name + " experimental data").c_str(),
			500, x_min - 0.1 *(x_max - x_min), x_max + 0.1 *(x_max - x_min), 500, y_min - 0.1 *(y_max - y_min), y_max + 0.1 *(y_max - y_min));
	frame_xy->GetXaxis()->SetTitle("#tau_{S} [#mus]");
	frame_xy->GetYaxis()->SetTitle("Contribution [%]");
	frame_xy->Draw();
	for (std::size_t i = 0, i_end_ = points_xy.size(); i!=i_end_; ++i) {
		polyline_xy[i]->Draw();
		points_xy[i]->Draw();
		polyline_xy_fit[i]->Draw();
	}
	c_xy->Update();

	TCanvas *c_uv = new TCanvas ((name + "_#tau-W").c_str(), (name + "_#tau-W").c_str(), DEF_W, DEF_H);
	c_uv->SetGrid();
	c_uv->SetTicks();
	c_uv->ToggleEventStatus();
  c_uv->ToggleToolBar();
	TH2F* frame_uv = new TH2F((name + " simple model").c_str(), (name + " simple model").c_str(),
			500, u_min - 0.1 *(u_max - u_min), u_max + 0.1 *(u_max - u_min), 500, v_min - 0.1 *(v_max - v_min), v_max + 0.1 *(v_max - v_min));
	frame_uv->GetXaxis()->SetTitle("#tau_{Ar-} [#mus]");
	frame_uv->GetYaxis()->SetTitle("W_{pass}");
	frame_uv->Draw();
	for (std::size_t i = 0, i_end_ = points_uv.size(); i!=i_end_; ++i) {
		polyline_uv[i]->Draw();
		points_uv[i]->Draw();
		polyline_uv_fit[i]->Draw();
	}
	c_uv->Update();
}

Double_t FittingF_tau (Double_t *x, Double_t *par) {
	//x[0] - electric field in Td
	//par[0] - tau_Ar
  //par[1] - W_pass for 5.1 Td
	//par[2] - W_pass for 5.9 Td
	//...
	//output - observed time constant of slow component tau_S
	std::map<double, Transformation2D>::iterator entry = model_extrapolation.find(x[0]);
	if (model_extrapolation.end() == entry) {
		throw std::runtime_error(std::string("Error: FittingF_tau called on undefined field ") + dbl_to_str(x[0], 1) + " Td (model_extrapolation)");
	}
	std::map<double, int>::iterator entry_i = model_W_index.find(x[0]);
	if (model_W_index.end() == entry_i) {
		throw std::runtime_error(std::string("Error: FittingF_tau called on undefined field ") + dbl_to_str(x[0], 1) + " Td (model_W_index)");
	}
	return entry->second.evalX(par[0], par[entry_i->second]);
}

Double_t FittingF_Fr (Double_t *x, Double_t *par) {
	//x[0] - electric field in Td
	//par[0] - tau_Ar
	//par[1] - W_pass for 5.1 Td
	//par[2] - W_pass for 5.9 Td
	//...
	//output - observed contribution of slow component Fr
	std::map<double, Transformation2D>::iterator entry = model_extrapolation.find(x[0]);
	if (model_extrapolation.end() == entry) {
		throw std::runtime_error(std::string("Error: FittingF_Fr called on undefined field ") + dbl_to_str(x[0], 1) + " Td (model_extrapolation)");
	}
	std::map<double, int>::iterator entry_i = model_W_index.find(x[0]);
	if (model_W_index.end() == entry_i) {
		throw std::runtime_error(std::string("Error: FittingF_Fr called on undefined field ") + dbl_to_str(x[0], 1) + " Td (model_W_index)");
	}
	return entry->second.evalY(par[0], par[entry_i->second]);
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


int e_trapping_model_fit (void) {

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	gStyle->SetOptStat("nemr");
	gStyle->SetEndErrorSize(3);
	int DEF_W = 1300, DEF_H = 700; //qewr - for fast Ctrl+F

	std::vector<Color_t> palette_major = {kBlack, kGreen, kRed, kBlue, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kGreen -2, kMagenta, kAzure + 10, kMagenta+3, kOrange - 7, kOrange + 6};
	bool linear = true;

	std::string framename = "Global fit of experimental data by simple model";

	{
		//determined from e_trapping_simulation.cpp (by hand)
		Transformation2D model_20kV;
		std::vector<double> xs = {5.29, 5.58, 5.29, 5.03, 5.03, 5.03, 5.29, 5.58, 5.58}; //tau_S
		std::vector<double> ys = {37.9, 42.8, 42.8, 42.8, 37.9, 33.5, 33.5, 33.5, 37.9}; //Fr_S
		std::vector<double> us = {3.49, 3.39, 3.16, 3.03, 3.27, 3.52, 3.70, 3.93, 3.66}; //tau_Ar
		std::vector<double> vs = {0.272,0.206,0.201,0.196,0.268,0.335,0.338,0.343,0.277}; //W_pass
		model_20kV.initialize(xs, ys, us, vs);
		//Draw_contour(model_20kV, "20kV");
		model_extrapolation[8.5] = model_20kV;
	}
	{
		Transformation2D model_18kV;
		std::vector<double> xs = {4.80, 5.25, 4.80, 4.54, 4.54, 4.54, 4.80, 5.25, 5.25};
		std::vector<double> ys = {32.9, 36.6, 36.6, 36.6, 32.9, 28.0, 28.0, 28.0, 32.9};
		std::vector<double> us = {3.30, 3.46, 3.11, 2.91, 3.10, 3.37, 3.57, 3.91, 3.64};
		std::vector<double> vs = {0.328,0.278,0.270,0.263,0.322,0.406,0.412,0.418,0.338};
		model_18kV.initialize(xs, ys, us, vs);
		//Draw_contour(model_18kV, "18kV");
		model_extrapolation[7.6] = model_18kV;
	}
	{
		Transformation2D model_16kV;
		std::vector<double> xs = {4.57, 4.94, 4.57, 4.30, 4.30, 4.30, 4.57, 4.94, 4.94};
		std::vector<double> ys = {24.8, 29.8, 29.8, 29.8, 24.8, 19.9, 19.9, 19.9, 24.8};
		std::vector<double> us = {3.50, 3.56, 3.25, 3.04, 3.28, 3.48, 3.71, 4.02, 3.79};
		std::vector<double> vs = {0.456,0.373,0.364,0.356,0.449,0.545,0.550,0.558,0.462};
		model_16kV.initialize(xs, ys, us, vs);
		//Draw_contour(model_16kV, "16kV");
		model_extrapolation[6.8] = model_16kV;
	}
	{
		Transformation2D model_14kV;
		std::vector<double> xs = {4.30, 4.65, 4.30, 3.98, 3.98, 3.98, 4.30, 4.65, 4.65};
		std::vector<double> ys = {16.2, 20.9, 20.9, 20.9, 16.2, 11.5, 11.5, 11.5, 16.2};
		std::vector<double> us = {3.61, 3.71, 3.39, 3.12, 3.33, 3.51, 3.80, 4.12, 3.91};
		std::vector<double> vs = {0.613,0.519,0.510,0.501,0.607,0.717,0.721,0.724,0.619};
		model_14kV.initialize(xs, ys, us, vs);
		//Draw_contour(model_14kV, "14kV");
		model_extrapolation[5.9] = model_14kV;
	}
	{
		Transformation2D model_12kV;
		std::vector<double> xs = {3.95, 4.58, 3.95, 3.35, 3.35, 3.35, 3.95, 4.58, 4.58};
		std::vector<double> ys = { 9.9, 14.1, 14.1, 14.1,  9.9,  6.2,  6.2,  6.2,  9.9};
		std::vector<double> us = {3.52, 3.92, 3.36, 2.82, 2.97, 3.14, 3.71, 4.33, 4.12};
		std::vector<double> vs = {0.744,0.656,0.641,0.623,0.732,0.828,0.836,0.841,0.754};
		model_12kV.initialize(xs, ys, us, vs);
		//Draw_contour(model_12kV, "12kV");
		model_extrapolation[5.1] = model_12kV;
	}


	std::vector<double> Tds =          { 5.1,  5.9,  6.8,  7.6,  8.5};
	std::vector<double> tau_S =				 {3.95, 4.30, 4.57, 4.80, 5.29};
	std::vector<double> tau_S_errors = {0.61, 0.33, 0.32, 0.35, 0.27};
	std::vector<double> Fr_S =				 { 9.9, 16.2, 24.8, 32.9, 37.9};
	std::vector<double> Fr_S_errors =  { 3.9,  4.7,  4.9,  4.3,  4.6};


	for(std::size_t i = 0, i_end_ = Tds.size(); i != i_end_; ++i) {
		model_W_index[Tds[i]] = 1 + i;
	}

	double max_val_tau = 0, max_val_Fr = 0;
	TGraphErrors* graph_tau = CreateGraph(Tds, tau_S, tau_S_errors);
	max_val_tau = std::max(max_val_tau, *std::max_element(tau_S.begin(), tau_S.end()));
	graph_tau->SetMarkerStyle(kFullSquare);
	graph_tau->SetMarkerSize(2);
	graph_tau->SetLineWidth(3);
	graph_tau->SetMarkerColor(palette_major[0]);

	TGraphErrors* graph_Fr = CreateGraph(Tds, Fr_S, Fr_S_errors);
	max_val_Fr = std::max(max_val_Fr, *std::max_element(Fr_S.begin(), Fr_S.end()));
	graph_Fr->SetMarkerStyle(kFullSquare);
	graph_Fr->SetMarkerSize(2);
	graph_Fr->SetLineWidth(3);
	graph_Fr->SetMarkerColor(palette_major[0]);

	TF1* fit_f_tau = new TF1("func_tau", FittingF_tau, 4.5, 9, Tds.size() + 1);
	TF1* fit_f_Fr = new TF1("func_Fr", FittingF_Fr, 4.5, 9, Tds.size() + 1);

	ROOT::Math::WrappedMultiTF1 wf_tau(*fit_f_tau, 1);
  ROOT::Math::WrappedMultiTF1 wf_Fr(*fit_f_Fr, 1);
	ROOT::Fit::DataOptions opt;
	//opt.fUseRange = true;

	ROOT::Fit::BinData data_tau(opt);
  ROOT::Fit::BinData data_Fr(opt);

	ROOT::Fit::FillData(data_tau, graph_tau);
  ROOT::Fit::FillData(data_Fr, graph_Fr);

	ROOT::Fit::Chi2Function chi2_tau(data_tau, wf_tau);
	ROOT::Fit::Chi2Function chi2_Fr(data_Fr, wf_Fr);

	GlobalChi2 globalChi2(chi2_tau, chi2_Fr); //unite chi2 functions for tau and Fr

  ROOT::Fit::Fitter fitter;
	const int Npar = 6;//tau, then W_pass(E/N)
  double pars[Npar] = {3.3, 0.744, 0.613, 0.456, 0.328, 0.272};//initial values

	fitter.Config().SetParamsSettings(Npar, pars);
	fitter.Config().ParSettings(0).SetLimits(2.5, 4.4);
	fitter.Config().ParSettings(1).SetLimits(0.60, 0.95); //12kV
	fitter.Config().ParSettings(2).SetLimits(0.45, 0.75); //14kV
	fitter.Config().ParSettings(3).SetLimits(0.30, 0.60); //16kV
	fitter.Config().ParSettings(4).SetLimits(0.20, 0.45); //18kV
	fitter.Config().ParSettings(5).SetLimits(0.15, 0.40); //20kV

	fitter.Config().MinimizerOptions().SetPrintLevel(0); //default - 0
  fitter.Config().SetMinimizer("Minuit", "Minimize");
	fitter.FitFCN(Npar, globalChi2, 0, data_tau.Size() + data_Fr.Size(), true);
  ROOT::Fit::FitResult fit_result = fitter.Result();
  fit_result.Print(std::cout);
	fit_result.PrintCovMatrix(std::cout);
	std::cout<<"p-value: "<<fit_result.Prob()<<std::endl;
	const double *out_pars = fit_result.GetParams();
	fit_f_tau->SetParameters(out_pars);
	fit_f_Fr->SetParameters(out_pars);


	std::vector<double> tau_fitted, Fr_fitted;
	for (std::size_t i = 0, i_end_ = Tds.size(); i!=i_end_; ++i) {
		tau_fitted.push_back(fit_f_tau->Eval(Tds[i]));
		Fr_fitted.push_back(fit_f_Fr->Eval(Tds[i]));
	}

	TGraph* graph_fit_tau = CreateGraph(Tds, tau_fitted);
	TGraph* graph_fit_Fr = CreateGraph(Tds, Fr_fitted);
	max_val_tau = std::max(max_val_tau, *std::max_element(tau_fitted.begin(), tau_fitted.end()));
	max_val_Fr = std::max(max_val_Fr, *std::max_element(Fr_fitted.begin(), Fr_fitted.end()));

	max_val_tau = 9;
	max_val_Fr = 50;

	graph_fit_tau->SetMarkerStyle(kFullCircle);
	graph_fit_tau->SetMarkerSize(1.5);
	graph_fit_tau->SetLineWidth(3);
	graph_fit_tau->SetMarkerColor(palette_major[2]);
	graph_fit_Fr->SetMarkerStyle(kFullCircle);
	graph_fit_Fr->SetMarkerSize(1.5);
	graph_fit_Fr->SetLineWidth(3);
	graph_fit_Fr->SetMarkerColor(palette_major[2]);


	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	{
		std::string tau_name = "Slow component time constant";
		TCanvas *c_tau = new TCanvas (tau_name.c_str(), tau_name.c_str(), DEF_W, DEF_H);
		c_tau->SetGrid();
		c_tau->SetTicks();
		c_tau->ToggleEventStatus();
	  c_tau->ToggleToolBar();
		TLegend *legend_tau = new TLegend(0.55, 0.65, 0.9, 0.9);
		//legend_tau->SetHeader("");
		legend_tau->SetMargin(0.25);
		TH2F* frame_tau = new TH2F("frame_tau", tau_name.c_str(), 500, 4.5, 9, 500, 0, max_val_tau);
		frame_tau->GetXaxis()->SetTitle("E/N [Td]");
		frame_tau->GetYaxis()->SetTitle("#tau_{S}");
		frame_tau->Draw();

		graph_tau->Draw("p");
	  graph_fit_tau->Draw("p same");
		legend_tau->AddEntry(graph_tau, (std::string("Experiment")).c_str(), "p");
		legend_tau->AddEntry(graph_fit_tau, (std::string("Fit")).c_str(), "p");

		frame_tau->Draw("sameaxis");
		legend_tau->Draw("same");
		c_tau->Update();
	}

	{
		std::string Fr_name = "Slow component contribution";
		TCanvas *c_Fr = new TCanvas (Fr_name.c_str(), Fr_name.c_str(), DEF_W, DEF_H);
		c_Fr->SetGrid();
		c_Fr->SetTicks();
		c_Fr->ToggleEventStatus();
	  c_Fr->ToggleToolBar();
		TLegend *legend_Fr = new TLegend(0.55, 0.65, 0.9, 0.9);
		//legend_tau->SetHeader("");
		legend_Fr->SetMargin(0.25);
		TH2F* frame_Fr = new TH2F("frame_Fr", Fr_name.c_str(), 500, 4.5, 9, 500, 0, max_val_Fr);
		frame_Fr->GetXaxis()->SetTitle("E/N [Td]");
		frame_Fr->GetYaxis()->SetTitle("Contribution [%]");
		frame_Fr->Draw();

		graph_Fr->Draw("p");
	  graph_fit_Fr->Draw("p same");
		legend_Fr->AddEntry(graph_Fr, (std::string("Experiment")).c_str(), "p");
		legend_Fr->AddEntry(graph_fit_Fr, (std::string("Fit")).c_str(), "p");

		frame_Fr->Draw("sameaxis");
		legend_Fr->Draw("same");
		c_Fr->Update();
	}

  return 0;
}
