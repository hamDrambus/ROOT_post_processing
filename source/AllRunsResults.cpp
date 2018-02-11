#include "AllRunsResults.h"

AllRunsResults::AllRunsResults(ParameterPile::experiment_area experiment)
{
	_exp = experiment;
	N_of_runs = 0;
	Iteration_N = 0;
	for (auto ex = _exp.experiments.begin(); ex != _exp.experiments.end(); ++ex) {
		int mppc_ch = 0;
		for (int ch = _exp.channels.get_next_index(); ch != -1; ch = _exp.channels.get_next_index()){
			if (ch == 0){
				std::string prefix = DATA_PREFIX + "/"+ DATA_PMT_VERSION + "/PMT_" + *ex + "/PMT_0/PMT_0_";
				vector_from_file(PMT3_peaks, prefix + "peaks.dat");
				if (!PMT3_peaks.empty())
					pmt_channels.push_back(ch);
				continue;
			}
			if (ch == 1){
				std::string prefix = DATA_PREFIX + "/" + DATA_PMT_VERSION + "/PMT_" + *ex + "/PMT_1/PMT_1_";
				vector_from_file(PMT1_peaks, prefix + "peaks.dat");
				if (!PMT1_peaks.empty())
					pmt_channels.push_back(ch);
				continue;
			}
			std::string prefix = DATA_PREFIX+"/"+DATA_MPPC_VERSION + "/MPPCs_" + *ex + "/MPPC_" + std::to_string(ch) + "/MPPC_" + std::to_string(ch) + "_";
			
			mppc_peaks.push_back(STD_CONT<STD_CONT<peak>>());
			mppc_S2_S.push_back(DVECTOR());
			mppc_S2_start_time.push_back(DVECTOR());
			mppc_S2_finish_time.push_back(DVECTOR());
			mppc_double_Is.push_back(DVECTOR());
			mppc_channels.push_back(ch);

			vector_from_file(mppc_peaks.back(), prefix + "peaks.dat");
			vector_from_file(mppc_S2_S.back(), prefix + "S2_S.dat");
			vector_from_file(mppc_double_Is.back(), prefix + "double_I.dat");
			vector_from_file(mppc_S2_start_time.back(), prefix + "S2_start_t.dat");
			vector_from_file(mppc_S2_finish_time.back(), prefix + "S2_finish_t.dat");

			if (mppc_peaks.back().empty()|| mppc_S2_S.back().empty() || mppc_double_Is.back().empty()){ //empty files
				mppc_peaks.pop_back();
				mppc_double_Is.pop_back();
				mppc_S2_S.pop_back();
				mppc_channels.pop_back();
			} else {
				++mppc_ch;
				std::cout << "Loaded channel " << ch << std::endl;
				N_of_runs = std::max(N_of_runs, (int)std::max(mppc_double_Is.back().size(), mppc_S2_S.back().size()));
			}
		}
	}
}

void AllRunsResults::processAllRuns(STD_CONT<SingleRunResults> &single_results)
{}


void AllRunsResults::Merge(AllRunsResults* with)
{
	//N_of_runs += with->N_of_runs;
	////Iteration_N = with->Iteration_N;
	//
	//bool empty = false, valid = true;
	//if (mppc_S2_S.empty())
	//	empty = true;
	//if ((mppc_S2_S.size() != mppc_all_peaks_Ss.size()) || (mppc_S2_S.size() != mppc_channels.size())
	//	|| (mppc_S2_S.size() != mppc_double_Is.size()))
	//	valid = false;
	//if ((with->mppc_S2_S.size() != with->mppc_all_peaks_Ss.size()) || (with->mppc_S2_S.size() != with->mppc_channels.size())
	//	|| (mppc_S2_S.size() != mppc_double_Is.size()))
	//	valid = false;
	//if (!valid) {
	//	std::cout << "AllRunsResults contains invalid MPPC data: channels' size mismatches" << std::endl;
	//	return;
	//}
	//if (!empty && (mppc_S2_S.size() != with->mppc_S2_S.size())){
	//	std::cout << "WARNING Two AllRunsResults have MPPC channels' size mismatches: not Merging" << std::endl;
	//	return;
	//}
	//if (empty) {
	//	mppc_S2_S = with->mppc_S2_S;
	//	mppc_all_peaks_Ss = with->mppc_all_peaks_Ss;
	//	mppc_channels = with->mppc_channels;
	//	mppc_double_Is = with->mppc_double_Is;
	//} else {
	//	for (int ch = 0; ch < mppc_S2_S.size(); ++ch) {
	//		mppc_S2_S[ch].insert(mppc_S2_S[ch].end(), with->mppc_S2_S[ch].begin(), with->mppc_S2_S[ch].end());
	//		mppc_all_peaks_Ss[ch].insert(mppc_all_peaks_Ss[ch].end(), with->mppc_all_peaks_Ss[ch].begin(), with->mppc_all_peaks_Ss[ch].end());
	//		mppc_double_Is[ch].insert(mppc_double_Is[ch].end(), with->mppc_double_Is[ch].begin(), with->mppc_double_Is[ch].end());
	//	}
	//}
}

void AllRunsResults::Merged(void)
{	
//	bool valid = true;
//	if ((mppc_S2_S.size() != mppc_all_peaks_Ss.size()) || (mppc_S2_S.size() != mppc_channels.size())
//		|| (mppc_S2_S.size() != mppc_double_Is.size()))
//		valid = false;
//	if (valid && !(mppc_S2_S.empty())) {
//		ParameterPile::experiment_area area_(ParameterPile::experiment_area::Type::Point);
//		area_.experiments.push_back(_exp.experiments.back()); //done: TODO: account for MPPC channel
//		if (ParameterPile::draw_required(area_)) {
//			std::string fname = std::string(OUTPUT_DIR) + OUTPUT_MPPCS + area_.experiments.back()+".dat";
//			std::ofstream output;
//			open_output_file(fname, output);
//			output << "MPPC#\tS_1pe_avr\tS_2pe_avr\tS2_S_avr\tdouble_I_avr\tS_1pe_sigma\tS_2pe_sigma\tS2_S_sigma\tdouble_I_sigma\ttime_left_avr\ttime_right_avr" << std::endl;
//			for (int ch = 0; ch < mppc_peaks_in_S2_area.size(); ++ch) {
//				std::string Ss_name = area_.experiments.back()+"_MPPC#" + std::to_string(mppc_channels[ch]) + "_peaks_S";
//				std::string S2_S_name = area_.experiments.back() + "_MPPC#" + std::to_string(mppc_channels[ch]) + "_S2_S";
//				std::string S2_start_t_name = area_.experiments.back() + "_MPPC#" + std::to_string(mppc_channels[ch]) + "_S2_start_t";
//				std::string S2_finish_t_name = area_.experiments.back() + "_MPPC#" + std::to_string(mppc_channels[ch]) + "_S2_finish_t";
//				std::string double_I_name = area_.experiments.back() + "_MPPC#" + std::to_string(mppc_channels[ch]) + "_double_I";
//				
//				//done: TODO: Figure out MPPC channel. And add historam creating function
//				//TODO: ParameterPile
//				TH1D *hist_S = createMPPCHist(mppc_all_peaks_Ss[ch], Ss_name, 0, 4.0, 60);
//				TH1D *hist_S2_S = createMPPCHist(mppc_peaks_in_S2_area[ch], S2_S_name, 0, 4.0, 60);
//				TH1D *hist_S2_start_t = createMPPCHist(mppc_S2_start_time[ch], S2_start_t_name, ParameterPile::S2_start_time, 4.0, 60);
//				TH1D *hist_S2_finish_t = createMPPCHist(mppc_S2_finish_time[ch], S2_finish_t_name, ParameterPile::S2_start_time, 6.0, 60);
//				TH1D *hist_double_I = createMPPCHist(mppc_double_Is[ch], double_I_name, -1, 6.0, 60);
//
//				/*TF1 *g1 = new TF1("m1", "gaus", hist_S->GetMinimum(), hist_S->GetMaximum());
//				TF1 *g2 = new TF1("m2", "gaus", hist_S->GetMinimum(), hist_S->GetMaximum());*/
//				TF1 *_S_fit = new TF1(Ss_name.c_str(), "gaus(0)+gaus(3)", hist_S ? hist_S->GetXaxis()->GetXmin() : -1,
//					hist_S ? hist_S->GetXaxis()->GetXmax() : 1);
//				if (NULL != hist_S){
//					_S_fit->SetParLimits(0, 0, hist_S->GetMaximum()*1.2);//TODO: ParameterPile
//					_S_fit->SetParLimits(1, hist_S->GetXaxis()->GetXmin(), hist_S->GetXaxis()->GetXmax());
//					_S_fit->SetParLimits(3, 0, hist_S->GetMaximum() * 1);//TODO: ? ParameterPile
//					_S_fit->SetParLimits(4, hist_S->GetBinCenter(hist_S->GetMaximumBin()), hist_S->GetXaxis()->GetXmax());
//
//					_S_fit->SetParameter(1, hist_S->GetBinCenter(hist_S->GetMaximumBin()));
//					_S_fit->SetParameter(2, hist_S->GetBinWidth(0));
//					_S_fit->SetParameter(4, 2 * hist_S->GetBinCenter(hist_S->GetMaximumBin())); //'2*' because 2 photoelectron peak
//					_S_fit->SetParameter(5, hist_S->GetBinWidth(0));
//				}
//				TF1 *_S2_S_fit = createMPPCFitFunc(hist_S2_S, S2_S_name);
//				TF1 *_S2_start_t_fit = createMPPCFitFunc(hist_S2_start_t, S2_start_t_name);
//				TF1 *_S2_finish_t_fit = createMPPCFitFunc(hist_S2_finish_t, S2_finish_t_name);
//				TF1 *_double_I_fit = createMPPCFitFunc(hist_double_I, double_I_name);
//
//#ifdef _TEMP_CODE
//				if (hist_S2_start_t)
//					hist_S2_start_t->Fit(_S2_start_t_fit, "Q"); //affects the name of the previous to this code fit in canvas, So not rendered fits
//				//should be placed before all of the canvaces
//				if (hist_S2_finish_t)
//					hist_S2_finish_t->Fit(_S2_finish_t_fit, "Q");
//#endif
//
//				gStyle->SetOptFit(102);
//				TCanvas *c1 = new TCanvas(Ss_name.c_str(), Ss_name.c_str());
//				c1->cd();
//				c1->SetTitle(Ss_name.c_str());
//				if (hist_S){
//					hist_S->Fit(_S_fit, "Q");
//					hist_S->Draw();
//				}
//				std::cout << "GaussSum pars: " << _S_fit->GetParameter(0) << "; "<<_S_fit->GetParameter(1) << "; "<<_S_fit->GetParameter(2) << std::endl;
//				//_S_fit->Draw();
//				c1->Update();
//
//				TCanvas *c2 = new TCanvas(S2_S_name.c_str(), S2_S_name.c_str());
//				c2->cd();
//				c2->SetTitle(S2_S_name.c_str());
//				if (hist_S2_S) {
//					hist_S2_S->Fit(_S2_S_fit, "Q");
//					hist_S2_S->Draw();
//				}
//				//_S2_S_fit->Draw();
//				c2->Update();
//
//#ifndef _TEMP_CODE
//				TCanvas *c3 = new TCanvas(S2_start_t_name.c_str(), S2_start_t_name.c_str());
//				c3->cd();
//				c3->SetTitle(S2_start_t_name.c_str());
//				if (hist_S2_start_t) {
//					hist_S2_start_t->Fit(_S2_start_t_fit,"Q");
//					hist_S2_start_t->Draw();
//				}
//				//_S2_start_t_fit->Draw();
//				c3->Update();
//
//				TCanvas *c4 = new TCanvas(S2_finish_t_name.c_str(), S2_finish_t_name.c_str());
//				c4->cd();
//				c4->SetTitle(S2_finish_t_name.c_str());
//				if (hist_S2_finish_t) {
//					hist_S2_finish_t->Fit(_S2_finish_t_fit,"Q");
//					hist_S2_finish_t->Draw();
//				}
//				//_S2_finish_t_fit->Draw();
//				c4->Update();		
//#endif
//				TCanvas *c5 = new TCanvas(double_I_name.c_str(), double_I_name.c_str());
//				c5->cd();
//				c5->SetTitle(double_I_name.c_str());
//				if (hist_double_I) {
//					hist_double_I->Fit(_double_I_fit,"Q");
//					hist_double_I->Draw();
//				}
//				//_S2_finish_t_fit->Draw();
//				c5->Update();
//#ifdef OUTPUT_MPPCS_PICS
//				std::string output_prefix =std::string(ParameterPile::this_path)+"\\"+ std::string(OUTPUT_DIR) + OUTPUT_MPPCS_PICS + area_.experiments.back() 
//					+ "\\" + OUTPUT_MPPCS + std::to_string(mppc_channels[ch]) + "\\" + OUTPUT_MPPCS + std::to_string(mppc_channels[ch]) + "_";
//				vector_to_file(mppc_all_peaks_Ss[ch], output_prefix + "Ss.dat");
//				vector_to_file(mppc_peaks_in_S2_area[ch], output_prefix + "S2_S.dat");
//				vector_to_file(mppc_S2_start_time[ch], output_prefix + "S2_start_t.dat");
//				vector_to_file(mppc_S2_finish_time[ch], output_prefix + "S2_finish_t.dat");
//				vector_to_file(mppc_double_Is[ch], output_prefix + "double_I.dat");
//				c1->SaveAs((output_prefix + "Ss.png").c_str(),"png");
//				c2->SaveAs((output_prefix + "S2_S.png").c_str(),"png");
//#ifndef _TEMP_CODE
//				c3->SaveAs((output_prefix + "S2_stat_t.png").c_str(),"png");
//				c4->SaveAs((output_prefix + "S2_finish_t.png").c_str(),"png");
//#endif
//				c5->SaveAs((output_prefix + "double_I.png").c_str(),"png");
//#endif
//				//output << "MPPC#\tS_1pe_avr\tS_2pe_avr\tS2_S_avr\tdouble_I_avr\tS_1pe_sigma\tS_2pe_sigma\tS2_S_sigma\tdouble_I_sigma\ttime_left_avr\ttime_right_avr" << std::endl;
//				output << mppc_channels[ch] << "\t" << _S_fit->GetParameter(1) << "\t" << _S_fit->GetParameter(4)
//					<< "\t" << _S2_S_fit->GetParameter(1) << "\t" << _double_I_fit->GetParameter(1)
//					<< "\t" << _S_fit->GetParameter(2) << "\t" << _S_fit->GetParameter(5) << "\t" << _S2_S_fit->GetParameter(2)
//					<< "\t" << _double_I_fit->GetParameter(2) << "\t" << _S2_start_t_fit->GetParameter(1)
//					<< "\t" << _S2_finish_t_fit->GetParameter(1) << std::endl;
//				std::cout << "MPPC " << area_.experiments.back() << " processed" << std::endl;
//				std::cout << "# of runs " << N_of_runs << std::endl;
//				std::cout << "# of valid runs " << n_valid_runs << std::endl;
//			}
//			output.close();
//		}
//	}
	++Iteration_N;
}

void AllRunsResults::vector_from_file(DVECTOR &what, std::string fname)
{
	what.clear();
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (!str.is_open())
		return;
	double val;
	size_t size;
	while (!str.eof()) {
		str.read((char*)&size,sizeof(std::size_t));
		std::size_t counter = 0;
		while (!str.eof() && counter != size){
			str.read((char*)&val,sizeof(double));
			what.push_back(val);
			++counter;
		}
		break;
	}
	str.close();
}

void AllRunsResults::vector_from_file(STD_CONT<STD_CONT<peak>> &pks, std::string fname)
{
	pks.clear();
	std::ifstream str;
	str.open(fname,std::ios_base::binary);
	if (!str.is_open())
		return;
	std::size_t run_size;
	while (!str.eof()){
		str.read((char*)&run_size,sizeof(std::size_t));
		std::size_t run_counter = 0;
		while (!str.eof() && run_counter != run_size){
			pks.push_back(STD_CONT<peak>());
			std::size_t size;
			str.read((char*)&size,sizeof(std::size_t));
			std::size_t counter = 0;
			while (!str.eof() && counter != size){
				peak pk;
				str.read((char*)&pk.left, sizeof(double));
				str.read((char*)&pk.right, sizeof(double));
				str.read((char*)&pk.S, sizeof(double));
				str.read((char*)&pk.A, sizeof(double));
				pks.back().push_back(pk);
				++counter;
			}
			++run_counter;
		}
		break;
	}
	str.close();
}

int AllRunsResults::Iteration(void) const
{	return Iteration_N;}

void AllRunsResults::Clear(void)
{
	STD_CONT<DVECTOR>().swap(mppc_S2_S);
	STD_CONT<STD_CONT<STD_CONT<peak>>>().swap(mppc_peaks);
	STD_CONT<STD_CONT<peak>>().swap(PMT3_peaks);
	STD_CONT<STD_CONT<peak>>().swap(PMT1_peaks);
	STD_CONT<DVECTOR>().swap(mppc_double_Is);
	STD_CONT<int>().swap(mppc_channels);
	N_of_runs = 0;
	//Iteration_N preserved;
}
