#ifndef ALL_EXPERIMENTS_RESULTS_H
#define ALL_EXPERIMENTS_RESULTS_H

#include "GlobalParameters.h"
#include "AllRunsResults.h"
#include "PolynomialFit.h"
#include "PostProcessor.h"
#include "TF1.h"

class AnalysisManager;
class PostProcessor;

//contains only physical data. The helping data and functions are is the PostProcessor
class AllExperimentsResults
{
public:
	//expriment->channel->run->peak_itself:
	std::deque<std::deque<std::deque<std::deque<peak>>>> mppc_peaks;
	//expriment->run->peak_itself:
	std::deque<std::deque<std::deque<peak>>> PMT3_peaks;
	std::deque<std::deque<std::deque<peak>>> PMT1_peaks;
	//expriment->channel->array_itself:
	//std::deque<std::deque<std::vector<Double_t>>> Ss;
	std::deque<std::deque<std::vector<Double_t>>> S2_S; //size of std::vector<Double_t> must be the No. of runs
	std::deque<std::deque<std::vector<Double_t>>> S2_start_t;
	std::deque<std::deque<std::vector<Double_t>>> S2_finish_t;
	std::deque<std::deque<std::vector<Double_t>>> Double_I;
	
	std::vector<Double_t> Fields; //for each experiment
	std::deque<Int_t> mppc_channels;
	std::deque<Int_t> pmt_channels;
	/*ParameterPile::*/experiment_area exp_area;
protected:
	//experiment->channel:
	std::deque<std::deque<Double_t>> N_pe_direct;
	std::deque<std::deque<Double_t>> N_pe_Double_I;
	std::deque<std::deque<Double_t>> N_pe_result;
	//experiment
	std::deque<Double_t> N_pe_PMT3;
	std::deque<Double_t> N_pe_PMT1;
public:
	AllExperimentsResults(/*ParameterPile::*/experiment_area area);
	void processAllExperiments(std::deque<AllRunsResults> &all_runs_results);

	Int_t get_exp_index(std::string exp);
	Int_t get_ch_index(Int_t channel);

	friend AnalysisManager;
	friend PostProcessor;
};

#endif