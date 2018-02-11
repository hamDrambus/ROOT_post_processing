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
	STD_CONT<STD_CONT<STD_CONT<STD_CONT<peak>>>> mppc_peaks;
	//expriment->run->peak_itself:
	STD_CONT<STD_CONT<STD_CONT<peak>>> PMT3_peaks;
	STD_CONT<STD_CONT<STD_CONT<peak>>> PMT1_peaks;
	//expriment->channel->array_itself:
	//STD_CONT<STD_CONT<DVECTOR>> Ss;
	STD_CONT<STD_CONT<DVECTOR>> S2_S; //size of DVECTOR must be the No. of runs
	STD_CONT<STD_CONT<DVECTOR>> S2_start_t;
	STD_CONT<STD_CONT<DVECTOR>> S2_finish_t;
	STD_CONT<STD_CONT<DVECTOR>> double_I;
	
	DVECTOR Fields; //for each experiment
	STD_CONT<int> mppc_channels;
	STD_CONT<int> pmt_channels;
	ParameterPile::experiment_area exp_area;
protected:
	//experiment->channel:
	STD_CONT<STD_CONT<double>> N_pe_direct;
	STD_CONT<STD_CONT<double>> N_pe_double_I;
	STD_CONT<STD_CONT<double>> N_pe_result;
	//experiment
	STD_CONT<double> N_pe_PMT3;
	STD_CONT<double> N_pe_PMT1;
public:
	AllExperimentsResults(ParameterPile::experiment_area area);
	void processAllExperiments(STD_CONT<AllRunsResults> &all_runs_results);

	int get_exp_index(std::string exp);
	int get_ch_index(int channel);

	friend AnalysisManager;
	friend PostProcessor;
};

#endif