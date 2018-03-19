#ifndef ALL_RUNS_RESULTS_H
#define ALL_RUNS_RESULTS_H

#include "TH1D.h"
#include "TH1I.h"
#include "TLine.h"
#include "TStyle.h"
#include "GlobalParameters.h"
#include "SingleRunResults.h"

class AnalysisManager;
class SingleRunData;
class SingleRunResults;
class AllExperimentsResults;

class AllRunsResults
{
protected:
	int N_of_runs;
	int Iteration_N;
	/*ParameterPile::*/experiment_area _exp;
	
	std::deque<std::vector<double> > mppc_S2_S; //size == mppc channels (depends on experiment area)
	std::deque<std::vector<double> > mppc_S2_start_time; //size == mppc channels (depends on experiment area)
	std::deque<std::vector<double> > mppc_S2_finish_time; //size == mppc channels (depends on experiment area)
	//std::deque<std::vector<double>> mppc_all_peaks_Ss; //size == mppc channels (depends on experiment area)
	std::deque<std::vector<double> > mppc_Double_Is; //size == mppc channels (depends on experiment area)
	std::deque<int> mppc_channels;
	std::deque<int> pmt_channels;
	std::deque<std::deque<std::deque<peak> > > mppc_peaks;	//[channel][run#][peaks]
	std::deque<std::deque<std::deque<peak> > > pmt_peaks;	//[channel][run#][peaks]

	/*TH1D* createMPPCHist(std::vector<double> &what, std::string name, double left_cutoff, double right_cutoff_from_RMS, int N_bins = 0);
	void vector_to_file(std::vector<double> &what, std::string fname);
	TF1* createMPPCFitFunc(TH1D* hist, std::string name);*/
	void vector_from_file(std::vector<double> &what, std::string fname);
	void vector_from_file(std::deque<std::deque<peak> > &pks, std::string fname);
public:
	AllRunsResults(/*ParameterPile::*/experiment_area experiment);//only experiment and channels are important here
	void processAllRuns(std::deque<SingleRunResults> &single_results);
	//For multithreading:
	void Merge(AllRunsResults* with);
	void Merged(void);
	void Clear(void);
	int Iteration(void) const;

	friend AnalysisManager;
	friend SingleRunData;
	friend AllExperimentsResults;
};


#endif
