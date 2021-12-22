//Created on 2021.12.21
//Subracts one SiPM x-y Npe profile (light response funciton) from another
//Used to compare fast only and slow component only profiles without
//constant pe noise (seen in pre-trigger)

#define PAIR std::pair<double, double>

void read_hist_w (TH2D* hist, std::string fname) {
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (NULL == hist) {
		std::cerr<<"NULL histogram!"<<std::endl;
		return;
	}
	if (!str.is_open()) {
		std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;
		return;
	}
	std::size_t real_size = 0;
	str.read((char*)&real_size, sizeof(std::size_t));
	std::cout<<"File '"<<fname<<"': "<<real_size<<" events"<<std::endl;
	double val1;
	double val2;
	double weight;
	while (!str.eof()) {
		str.read((char*)&val1, sizeof(double));
		if (str.eof())
		break;
		str.read((char*)&val2, sizeof(double));
		if (str.eof())
		break;
		str.read((char*)&weight, sizeof(double));
		hist->Fill(val1, val2, weight);
	}
	str.close();
}

void renormalize_hist(TH2D* hist, int SiPMs_active) {
	int event_n = hist->GetEntries();
	if (event_n % SiPMs_active != 0) {
		std::cout<<"renormalize_hist: Warning! Event number is not integer"<<std::endl;
		return;
	}
	hist->Scale(1.0/(event_n/SiPMs_active));
}

TH1D* profile_x(TH2D* hist) {
	int Nbins = hist->GetNbinsX();
	double x_min = hist->GetXaxis()->GetXmin();
	double x_max = hist->GetXaxis()->GetXmax();
	TH1D* out = new TH1D ("profile_x", "profile_x", Nbins, x_min, x_max);
	for (int bx = 1, bx_end_ = hist->GetNbinsX()+1; bx!=bx_end_; ++bx)
		for (int by = 1, by_end_ = hist->GetNbinsY()+1; by!=by_end_; ++by) {
			out->SetBinContent(bx, out->GetBinContent(bx) + hist->GetBinContent(bx, by));
		}
	out->SetLineColor(kBlack);
	out->SetFillStyle(3335);
	out->SetFillColor(kBlack);
	return out;
}

TH1D* profile_y(TH2D* hist) {
	int Nbins = hist->GetNbinsY();
	double x_min = hist->GetYaxis()->GetXmin();
	double x_max = hist->GetYaxis()->GetXmax();
	TH1D* out = new TH1D ("profile_y", "profile_y", Nbins, x_min, x_max);
	for (int bx = 1, bx_end_ = hist->GetNbinsX()+1; bx!=bx_end_; ++bx)
		for (int by = 1, by_end_ = hist->GetNbinsY()+1; by!=by_end_; ++by) {
			out->SetBinContent(by, out->GetBinContent(by) + hist->GetBinContent(bx, by));
		}
	out->SetLineColor(kBlack);
	out->SetFillStyle(3335);
	out->SetFillColor(kBlack);
	return out;
}

int SiPM_profile_subtract (void) {

	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	gStyle->SetOptStat("nemr");
	gStyle->SetEndErrorSize(3);
	gStyle->SetCanvasDefH(800);
	gStyle->SetCanvasDefW(1000);

	std::vector<Color_t> palette_major = {kBlack, kGreen, kRed, kBlue, kYellow + 2, kMagenta, kOrange + 7};
	std::vector<Color_t> palette_minor = {kGray + 2, kGreen -2, kMagenta, kAzure + 10, kMagenta+3, kOrange - 7, kOrange + 6};

	std::string pre_trigger_fname = "210909/results_v9/Pu_46V_17.3kV_850V/54_SiPMs_profile_xy_cuts_05+06+07+47_0-23.0us.hdata";
	std::string target_fname = "210909/results_v9/Pu_46V_17.3kV_850V/57_SiPMs_profile_xy_cuts_05+06+07+47_32.0-160us.hdata";
	double pre_trigger_dt = 23-0; //us
	double target_dt = 160-32; //us

	TCanvas *c_pre_trigger = new TCanvas ("_pre_trigger", "_pre_trigger");
	c_pre_trigger->SetGrid();	c_pre_trigger->SetTicks();
	c_pre_trigger->ToggleEventStatus();	c_pre_trigger->ToggleToolBar();
	TH2D *hist_pre_trigger = new TH2D ("pre_trigger", "pre_trigger", 9, -22.5, 22.5, 9, -22.5, 22.5);
	read_hist_w(hist_pre_trigger, pre_trigger_fname);
	renormalize_hist(hist_pre_trigger, 24); //24 SiPMs are used
	hist_pre_trigger->Draw("lego");
	c_pre_trigger->Update();

	TCanvas *c_target = new TCanvas ("_target", "_target");
	c_target->SetGrid();	c_target->SetTicks();
	c_target->ToggleEventStatus();	c_target->ToggleToolBar();
	TH2D *hist_target = new TH2D ("target", "target", 9, -22.5, 22.5, 9, -22.5, 22.5);
	read_hist_w(hist_target, target_fname);
	renormalize_hist(hist_target, 24); //24 SiPMs are used
	hist_target->Draw("lego");
	c_target->Update();

	TCanvas *c_result = new TCanvas ("_result", "_result");
	c_result->SetGrid();	c_result->SetTicks();
	c_result->ToggleEventStatus();	c_result->ToggleToolBar();
	TH2D *hist_result = new TH2D ("result", "result", 9, -22.5, 22.5, 9, -22.5, 22.5);
	hist_result->Add(hist_target, hist_pre_trigger, 1.0, -1*target_dt/pre_trigger_dt);
	for (int bx = 1, bx_end_ = hist_result->GetNbinsX()+1; bx!=bx_end_; ++bx)
		for (int by = 1, by_end_ = hist_result->GetNbinsY()+1; by!=by_end_; ++by) {
			hist_result->SetBinContent(bx, by, std::max(hist_result->GetBinContent(bx, by), 0.0));
		}
	hist_result->Draw("lego");
	c_result->Update();

	TCanvas *c_result_x = new TCanvas ("_result_x", "_result_x");
	c_result_x->SetGrid();	c_result_x->SetTicks();
	c_result_x->ToggleEventStatus();	c_result_x->ToggleToolBar();
	TH1D *hist_result_x = profile_x(hist_result);
	hist_result_x->Draw("same");
	c_result_x->Update();

	TCanvas *c_result_y = new TCanvas ("_result_y", "_result_y");
	c_result_y->SetGrid();	c_result_y->SetTicks();
	c_result_y->ToggleEventStatus();	c_result_y->ToggleToolBar();
	TH1D *hist_result_y = profile_y(hist_result);
	hist_result_y->Draw("same");
	c_result_y->Update();

  return 0;
}
