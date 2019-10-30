#ifndef ALL_EXPERIMENTS_RESULTS_H
#define ALL_EXPERIMENTS_RESULTS_H

#include "GlobalParameters.h"
#include "AllRunsResults.h"
#include "PolynomialFit.h"
#include "PostProcessor.h"

class AnalysisManager;
class PostProcessor;

//contains only physical data. The helping data and functions are is the PostProcessor
class AllExperimentsResults
{
public:
	//experiment->channel->run->peak_itself:
	std::deque<std::deque<std::deque<std::deque<peak> > > > mppc_peaks;
	//experiment->channel->run->peak_itself:
	std::deque<std::deque<std::deque<std::deque<peak> > > > pmt_peaks;
	//experiment->channel->array_itself:
	//std::deque<std::deque<std::vector<double>>> Ss;
	std::deque<std::deque<std::vector<double> > > Double_I;
	//experiment->channel->array_itself:
	std::deque<std::deque<std::vector<double> > > PMT_S2_int;
	
	std::vector<double> Fields; //for each experiment
	std::deque<int> mppc_channels;
	std::deque<int> pmt_channels;
	/*ParameterPile::*/experiment_area exp_area;
protected:
	//experiment->channel:
	std::deque<std::deque<double> > N_pe_direct;
	std::deque<std::deque<double> > N_pe_Double_I;
	std::deque<std::deque<double> > N_pe_result;
	//experiment
	std::deque<double> N_pe_PMT3;
	std::deque<double> N_pe_PMT1;
public:
	AllExperimentsResults(/*ParameterPile::*/experiment_area area);
	void processAllExperiments(std::deque<AllRunsResults> &all_runs_results);

	int get_exp_index(std::string exp);
	int get_ch_index(int channel);

	friend AnalysisManager;
	friend PostProcessor;
};

#endif
