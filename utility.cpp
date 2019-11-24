//LOAD AFTER init.cpp loading "libpost_processing.so"

//This file requires PolynomialFit.h to be compiled in .so because
//the last version of DataVector uses boost linear algebra instead of ROOT
//and ROOT's CINT has troubles interpereting (creating dictionary for) boost
//So use this file after loading .so using init.cpp. The thing is 
//init.cpp loads libpost_processing.so which contains a lot of code
//unnecessary for this utility file. It would probably be much better
//to create separate library with all useful functions I created for
//my projects so far in one place and load that library.
//Useful functions: whole SignalOperations::, ensure_folder, str_token,
//c_str_copy(std::string), PolynomialFit (this one is tricky: may be based 
//on ROOT only or boost only, the latter is more preferable and currently 
//used), DataVector, channel_info, compressed run-channel index vectors
//(exp_area), gnuplot interface and several more.

const double bolzmann_SI = 1.38064852e-23; //SI
const double Td_is_Vcm2 = 1e-17; //1 Townsend = 1e-17 V*cm^2
const double LAr_epsilon = 1.54; //doi: 10.1016/j.nima.2019.162431
const double full_gap_length = 2.2; //cm, the distance between THGEM0 and THGEM1
const double R3 = 10; //MOhm, THGEM0 resistance. 4 MOhm in experiments before ~ Feb 2019.
const double Rgap = 600; //MOhm, resistance defining E field in EL gap
const double Rrest = 200; //MOhm
const double T = 87; //temperature in K
const double P = 1.015e5; //pressure in Pa

const std::string Vdrift_data_fname = "ArDriftVel.txt";
DataVector Vdrift(4, 5); //e drift speed in gaseous Ar as a function of Td (in m/s)

//LAr_layer must be in [0, full_gap_length]
double gasE_from_kV (double kV, double gasAr_layer) //gasAr_layer is in [cm], returns E in gaseous Ar in [V/cm]
{ 
	if (gasAr_layer < 0)
		gasAr_layer = 0;
	if (gasAr_layer > full_gap_length)
		gasAr_layer = full_gap_length;
	return 1e3*kV*(Rgap / (Rgap + Rrest + R3))*LAr_epsilon / (LAr_epsilon*gasAr_layer + (full_gap_length - gasAr_layer));
}

double Td_from_E (double E) //E is in [V/cm]
{
	double N = 1e-6*P / (T*bolzmann_SI); //in cm^-3
	return E / N / Td_is_Vcm2;
}

double Vdr_from_kV (double kV, double gasAr_layer) //LAr_layer is in [cm], returns drift velocity in gaseous Ar in [cm/s]
{
	if (!Vdrift.isValid()) {
		std::ifstream str;
		str.open(Vdrift_data_fname);
		if (!str.is_open()) {
			std::cerr << "Error: Failed to open file with V drfit data \"" << Vdrift_data_fname << "\"!" << std::endl;
			return DBL_MAX;
		}
		Vdrift.read(str);
	}
	if (!Vdrift.isValid()) {
		std::cerr << "Error: Failed to load file with V drfit data \"" << Vdrift_data_fname << "\"!" << std::endl;
		return DBL_MAX;
	}
	double Td = Td_from_E(gasE_from_kV(kV, gasAr_layer));
	Td = std::fabs(Td);
	double Vdr = 1e2 * Vdrift(Td);
	return Vdr;
}

double drift_time_from_kV(double kV, double gasAr_layer) //gasAr_layer is in [cm], returns drift time in gaseous Ar in [microseconds]
{
	double Vdr = Vdr_from_kV(kV, gasAr_layer);
	if (DBL_MAX == Vdr)
		return 0;
	return 1e6*LAr_layer / Vd;
}

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
		double tau = std::stod<double>(word);
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
	bool linear = true;
	std::string framename = "190404 fast component FWHMs SiPMs";
    
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
	TGraph* data_pts = new TGraph(size, xs, ys);
	TF1* time_18mm = new TF1("func1", drift_time_as_f_kV, 2, 22, 2);
	time_18mm->SetParNames("gas Ar L", "time factor");
	time_18mm->FixParameter(0, 1.8);
	time_18mm->FixParameter(1, 1.0);

	TF1* time_Xmm = new TF1("func2", drift_time_as_f_kV, 2, 22, 2);
	time_Xmm->SetParNames("gas Ar L", "time factor");
	time_Xmm->SetParLimits(0, 0.5, 2.2);
	time_Xmm->SetParLimits(1, 0.5, 2.0);
	data_pts->Fit(time_Xmm);

	double gap_width = 10*time_Xmm->GetParameter(0); //in mm
	std::string str_gap_width = dbl_to_str(gap_width, 2);

	max_val = std::max(time_Xmm->Eval(2), max_val);
	max_val = std::max(time_18mm->Eval(2), max_val);

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
	TH2F* frame = new TH2F( "frame", framename.c_str(), 500, 2, 22, 500, 0, max_val);
	frame->GetXaxis()->SetTitle("V_{0} [kV]");
	frame->GetYaxis()->SetTitle("#tau_{D} [#mus]");
	frame->Draw();
	
	data_pts->Draw("*");
	time_18mm->Draw("same");
	time_Xmm->Draw("same");
	
	legend->AddEntry(data_pts, (std::string("190404 SiPM fast component FWHM")).c_str(), "p");
	legend->AddEntry(time_18mm, (std::string("t_{drift} for 18 mm gap")).c_str(), "l");
	legend->AddEntry(time_Xmm, (std::string("t_{drift} for best fit ("+ str_gap_width +" mm)")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();

	if (NULL!=xs)
		delete[] xs;
	if (NULL != ys)
		delete[] ys;

    return 0;
}
