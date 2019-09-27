
void read_hist (TH1D *hist, std::string fname) {
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (!str.is_open()) {
        std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;	    
        return;
    }
	double val;
	while (!str.eof()) {
		str.read((char*)&val, sizeof(double));
        hist->Fill(val);
	}
	str.close();
}

void read_hist_w (TH1D *hist, std::string fname) {
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (!str.is_open()) {
        std::cerr<<"Failed to open file '"<<fname<<"'"<<std::endl;	    
        return;
    }
	double val1;
    double val2;
	while (!str.eof()) {
		str.read((char*)&val1, sizeof(double));
        str.read((char*)&val2, sizeof(double));
        hist->Fill(val1, val2);
	}
	str.close();
} 

int forms (void) {
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    int DEF_W = 1100, DEF_H = 700;
    int Nbins = 300;
    double time_left = 19, time_right = 81;//us
    double time_pretrigger_left = 20, time_pretrigger_right = 26;
    TF1 *unity_f = new TF1("f1", "1", time_left, time_right);
    TH1D* hist_SiPM_N = new TH1D ("SiPM by counting peaks", "SiPM by counting peaks", Nbins, time_left, time_right);
    TH1D* hist_SiPM_S = new TH1D ("SiPM by weighting peaks", "SiPM by weighting peaks", Nbins, time_left, time_right);
    TH1D* hist_fastPMT_N = new TH1D ("fast PMT by counting peaks", "fast PMT by counting peaks", Nbins, time_left, time_right);
    TH1D* hist_fastPMT_S = new TH1D ("fast PMT by weighting peaks", "fast PMT by weighting peaks", Nbins, time_left, time_right);
    TH1D* hist_slowPMT_N = new TH1D ("slow PMT by counting peaks", "slow PMT by counting peaks", Nbins, time_left, time_right);
    TH1D* hist_slowPMT_S = new TH1D ("slow PMT by weighting peaks", "slow PMT by weighting peaks", Nbins, time_left, time_right);
    std::vector<TH1D*> hists;
    hists.push_back(hist_SiPM_N);
    hists.push_back(hist_SiPM_S);
    hists.push_back(hist_fastPMT_N);
    hists.push_back(hist_fastPMT_S);
    hists.push_back(hist_slowPMT_N);
    hists.push_back(hist_slowPMT_S);
    double max_val = 0;
    std::string Td = "8.2"
    std::string prefix = "190404/results/Cd_46V_20kV_850V/";
    read_hist (hist_fastPMT_N, prefix + "formN_8_Cd_peak.hdata");
    read_hist (hist_fastPMT_N, prefix + "formN_9_Cd_peak.hdata");
    read_hist (hist_fastPMT_N, prefix + "formN_10_Cd_peak.hdata");
    read_hist (hist_fastPMT_N, prefix + "formN_11_Cd_peak.hdata");
    
    read_hist_w (hist_fastPMT_S, prefix + "formS_8_Cd_peak.hdata");
    read_hist_w (hist_fastPMT_S, prefix + "formS_9_Cd_peak.hdata");
    read_hist_w (hist_fastPMT_S, prefix + "formS_10_Cd_peak.hdata");
    read_hist_w (hist_fastPMT_S, prefix + "formS_11_Cd_peak.hdata");
    
    read_hist (hist_slowPMT_N, prefix + "formN_12_Cd_peak.hdata");
    read_hist (hist_slowPMT_N, prefix + "formN_13_Cd_peak.hdata");
    read_hist (hist_slowPMT_N, prefix + "formN_14_Cd_peak.hdata");
    read_hist (hist_slowPMT_N, prefix + "formN_15_Cd_peak.hdata");

    read_hist_w (hist_slowPMT_S, prefix + "formS_12_Cd_peak.hdata");
    read_hist_w (hist_slowPMT_S, prefix + "formS_13_Cd_peak.hdata");
    read_hist_w (hist_slowPMT_S, prefix + "formS_14_Cd_peak.hdata");
    read_hist_w (hist_slowPMT_S, prefix + "formS_15_Cd_peak.hdata");

    read_hist_w (hist_SiPM_S, prefix + "formS_sumSiPM_Cd_peak.hdata");

    for (int hh = 0, hh_end_ = hists.size(); hh!=hh_end_; ++hh) {
        double baseline = 0;
        int counts = 0;
        for (int bin = 1, bin_end = hists[hh]->GetNbinsX()+1; bin!=bin_end; ++bin) {
		    if (hists[hh]->GetBinCenter(bin)>time_pretrigger_left && hists[hh]->GetBinCenter(bin)<time_pretrigger_right) {
	           baseline += hists[hh]->GetBinContent(bin);
               ++counts;
		    }
        }
        hists[hh]->Sumw2();
        baseline = -1*baseline/counts;
        hists[hh]->Add(unity_f, baseline);
    }

    max_val = std::max(max_val, hist_fastPMT_N->GetBinContent(hist_fastPMT_N->GetMaximumBin()));
    max_val = std::max(max_val, hist_fastPMT_S->GetBinContent(hist_fastPMT_S->GetMaximumBin()));
    max_val = std::max(max_val, hist_slowPMT_N->GetBinContent(hist_slowPMT_N->GetMaximumBin()));
    max_val = std::max(max_val, hist_slowPMT_S->GetBinContent(hist_slowPMT_S->GetMaximumBin()));
    max_val = std::max(max_val, hist_SiPM_N->GetBinContent(hist_SiPM_N->GetMaximumBin()));
    max_val = std::max(max_val, hist_SiPM_S->GetBinContent(hist_SiPM_S->GetMaximumBin()));
    
    hist_fastPMT_N->Scale(max_val / hist_fastPMT_N->GetBinContent(hist_fastPMT_N->GetMaximumBin()));
    hist_fastPMT_S->Scale(max_val / hist_fastPMT_S->GetBinContent(hist_fastPMT_S->GetMaximumBin()));
    hist_slowPMT_N->Scale(max_val / hist_slowPMT_N->GetBinContent(hist_slowPMT_N->GetMaximumBin()));
    hist_slowPMT_S->Scale(max_val / hist_slowPMT_S->GetBinContent(hist_slowPMT_S->GetMaximumBin()));
    hist_SiPM_N->Scale(max_val / hist_SiPM_N->GetBinContent(hist_SiPM_N->GetMaximumBin()));
    hist_SiPM_S->Scale(max_val / hist_SiPM_S->GetBinContent(hist_SiPM_S->GetMaximumBin()));

    max_val*=2;
	gStyle->SetGridStyle(3);
	gStyle->SetGridColor(14);
	gStyle->SetGridWidth(1);
	gStyle->SetOptStat("");
	TCanvas *c_ = new TCanvas (std::string("Signal forms ") + Td + " Td", std::string("Signal forms ") + Td + " Td", DEF_W, DEF_H);
	c_->SetGrid();
	c_->SetTicks();
	c_->SetLogy();//Log Y
	TLegend *legend = new TLegend(0.55, 0.65, 0.9, 0.9);
	//legend->SetHeader("");
	legend->SetMargin(0.25);
	TH2F* frame = new TH2F( "frame", "Collision E spectra 3D", 500, time_left, time_right, 500, 0, max_val);
	frame->GetXaxis()->SetTitle("t [#muV]");
	frame->GetYaxis()->SetTitle("");
	frame->Draw();
	
	hist_SiPM_S->SetLineWidth(2);
	hist_SiPM_S->SetLineColor(kBlack);
	hist_SiPM_S->Draw("hist csame");
	hist_SiPM_N->SetLineWidth(2);
	hist_SiPM_N->SetLineColor(kRed);//(kRed);
	hist_SiPM_N->Draw("hist csame");
	hist_fastPMT_N->SetLineWidth(2);
	hist_fastPMT_N->SetLineColor(kBlue);
	hist_fastPMT_N->Draw("hist csame");
	hist_fastPMT_S->SetLineWidth(2);
	hist_fastPMT_S->SetLineColor(kGreen);//(kBlack);
	hist_fastPMT_S->Draw("hist csame");
	hist_slowPMT_S->SetLineWidth(2);
	hist_slowPMT_S->SetLineColor(kYellow + 2);
	hist_slowPMT_S->Draw("hist csame");
    hist_slowPMT_N->SetLineWidth(2);
	hist_slowPMT_N->SetLineColor(kMagenta);
	hist_slowPMT_N->Draw("hist csame");
	
	legend->AddEntry(hist_SiPM_S, (std::string(Td + " Td SiPM by weighting peaks")).c_str(), "l");
	legend->AddEntry(hist_SiPM_N, (std::string(Td + " Td SiPM by counting peaks")).c_str(), "l");
	legend->AddEntry(hist_fastPMT_N, (std::string(Td + " Td fast PMT by counting peaks")).c_str(), "l");
	legend->AddEntry(hist_fastPMT_S, (std::string(Td + " Td fast PMT by weighting peaks")).c_str(), "l");
	legend->AddEntry(hist_slowPMT_N, (std::string(Td + " Td slow PMT by counting peaks")).c_str(), "l");
	legend->AddEntry(hist_slowPMT_S, (std::string(Td + " Td slow PMT weighting peaks")).c_str(), "l");

	frame->Draw("sameaxis");
	legend->Draw("same");
	c_->Update();
    return 0;
}
