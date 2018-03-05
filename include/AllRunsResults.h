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
	Int_t N_of_runs;
	Int_t Iteration_N;
	/*ParameterPile::*/experiment_area _exp;
	
	std::deque<std::vector<Double_t> > mppc_S2_S; //size == mppc channels (depends on experiment area)
	std::deque<std::vector<Double_t> > mppc_S2_start_time; //size == mppc channels (depends on experiment area)
	std::deque<std::vector<Double_t> > mppc_S2_finish_time; //size == mppc channels (depends on experiment area)
	//std::deque<std::vector<Double_t>> mppc_all_peaks_Ss; //size == mppc channels (depends on experiment area)
	std::deque<std::vector<Double_t> > mppc_Double_Is; //size == mppc channels (depends on experiment area)
	std::deque<Int_t> mppc_channels;
	std::deque<Int_t> pmt_channels;
	std::deque<std::deque<std::deque<peak> > > mppc_peaks;	//[channel][run#][peaks]
	std::deque<std::deque<peak> > PMT3_peaks;			//[run#][peaks]
	std::deque<std::deque<peak> > PMT1_peaks;			//[run#][peaks]

	/*TH1D* createMPPCHist(std::vector<Double_t> &what, std::string name, Double_t left_cutoff, Double_t right_cutoff_from_RMS, Int_t N_bins = 0);
	void vector_to_file(std::vector<Double_t> &what, std::string fname);
	TF1* createMPPCFitFunc(TH1D* hist, std::string name);*/
	void vector_from_file(std::vector<Double_t> &what, std::string fname);
	void vector_from_file(std::deque<std::deque<peak> > &pks, std::string fname);
public:
	AllRunsResults(/*ParameterPile::*/experiment_area experiment);//only experiment and channels are important here
	void processAllRuns(std::deque<SingleRunResults> &single_results);
	//For multithreading:
	void Merge(AllRunsResults* with);
	void Merged(void);
	void Clear(void);
	Int_t Iteration(void) const;

	friend AnalysisManager;
	friend SingleRunData;
	friend AllExperimentsResults;
};


#endif
