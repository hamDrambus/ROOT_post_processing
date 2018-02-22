#include "AllExperimentsResults.h"

AllExperimentsResults::AllExperimentsResults(/*ParameterPile::*/experiment_area area)
{
	exp_area = area;
}

Int_t AllExperimentsResults::get_exp_index(std::string experiment)
{
	for (auto i = exp_area.experiments.begin(); i != exp_area.experiments.end(); ++i)
		if (*i == experiment)
			return i - exp_area.experiments.begin();
	return -1;
}
Int_t AllExperimentsResults::get_ch_index(Int_t channel)
{
	for (auto i = mppc_channels.begin(); i != mppc_channels.end(); ++i)
		if (*i == channel){
			return i - mppc_channels.begin();
		}
	return -1;
}

void AllExperimentsResults::processAllExperiments(std::deque<AllRunsResults> &all_runs_results)
{
	exp_area.experiments.clear();
	for (auto i = all_runs_results.begin(); i != all_runs_results.end(); ++i) {
		Bool_t valid_merge = kTRUE;
		if (mppc_channels.empty()){
			mppc_channels = i->mppc_channels;
			pmt_channels = i->pmt_channels;
		} else {
			if (mppc_channels.size() != i->mppc_channels.size() || pmt_channels.size() != i->pmt_channels.size())
				valid_merge = kFALSE;
			else {
				for (auto a1 = mppc_channels.begin(), a2 = i->mppc_channels.begin();
					a1 != mppc_channels.end() && a2 != i->mppc_channels.end(); ++a1, ++a2)
					if (*a1 != *a2){
						valid_merge = kFALSE;
						break;
					}
				for (auto a1 = pmt_channels.begin(), a2 = i->pmt_channels.begin();
					a1 != pmt_channels.end() && a2 != i->pmt_channels.end(); ++a1, ++a2)
					if (*a1 != *a2){
						valid_merge = kFALSE;
						break;
					}
			}
		}
		if (!valid_merge){
			std::cout << "AllExperimentResults::processAllExperiments Warning: invalid merge for" << std::endl;
			std::cout << "\t" << i->_exp.experiments.back()<<std::endl;
			continue;
		}
		exp_area.experiments.push_back(i->_exp.experiments.back());
		mppc_peaks.push_back(i->mppc_peaks);
		if (!i->PMT1_peaks.empty()) //TODO: the case when there is no mppc channels
			PMT1_peaks.push_back(i->PMT1_peaks);
		if (!i->PMT3_peaks.empty())
			PMT3_peaks.push_back(i->PMT3_peaks);
		S2_S.push_back(i->mppc_S2_S);
		S2_start_t.push_back(i->mppc_S2_start_time);
		S2_finish_t.push_back(i->mppc_S2_finish_time);
		Double_I.push_back(i->mppc_Double_Is);
		auto j = /*ParameterPile::*/experiment_fields.find(i->_exp.experiments.back());
		if (/*ParameterPile::*/experiment_fields.end() != j)
			Fields.push_back(j->second);
		else
			Fields.push_back(-1);
		N_pe_direct.push_back(std::deque<Double_t>());
		N_pe_Double_I.push_back(std::deque<Double_t>());
		N_pe_result.push_back(std::deque<Double_t>());
		N_pe_PMT3.push_back(0);
		N_pe_PMT1.push_back(0);
		for (auto ff = mppc_channels.begin(); ff != mppc_channels.end(); ++ff){
			N_pe_direct.back().push_back(0);
			N_pe_Double_I.back().push_back(0);
			N_pe_result.back().push_back(0);
		}
	}
	for (Int_t exp = 0, e_end_ = exp_area.experiments.size(); exp != e_end_; ++exp){
		for (Int_t ch = 0, _end_ = mppc_channels.size(); ch != _end_; ++ch){
			Int_t run_size = Double_I[exp][ch].size();
			if ((run_size != S2_S[exp][ch].size())
				|| (run_size != S2_start_t[exp][ch].size())
				|| (run_size != S2_finish_t[exp][ch].size())
				|| (run_size != (PMT3_peaks.empty() ? run_size : PMT3_peaks[exp].size()))
				|| (run_size != (PMT1_peaks.empty() ? run_size : PMT1_peaks[exp].size()))
				|| (run_size != mppc_peaks[exp][ch].size())){
				std::cout << "AllExperimentResults::processAllExperiments Error: exp " << exp_area.experiments[exp] << " ch "
					<< ch << " Run size mismathch!"<<std::endl;
			}
		}
	}
}
