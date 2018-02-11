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
	ParameterPile::experiment_area _exp;
	
	STD_CONT<DVECTOR> mppc_S2_S; //size == mppc channels (depends on experiment area)
	STD_CONT<DVECTOR> mppc_S2_start_time; //size == mppc channels (depends on experiment area)
	STD_CONT<DVECTOR> mppc_S2_finish_time; //size == mppc channels (depends on experiment area)
	//STD_CONT<DVECTOR> mppc_all_peaks_Ss; //size == mppc channels (depends on experiment area)
	STD_CONT<DVECTOR> mppc_double_Is; //size == mppc channels (depends on experiment area)
	STD_CONT<int> mppc_channels;
	STD_CONT<int> pmt_channels;
	STD_CONT<STD_CONT<STD_CONT<peak>>> mppc_peaks;	//[channel][run#][peaks]
	STD_CONT<STD_CONT<peak>> PMT3_peaks;			//[run#][peaks]
	STD_CONT<STD_CONT<peak>> PMT1_peaks;			//[run#][peaks]

	/*TH1D* createMPPCHist(DVECTOR &what, std::string name, double left_cutoff, double right_cutoff_from_RMS, int N_bins = 0);
	void vector_to_file(DVECTOR &what, std::string fname);
	TF1* createMPPCFitFunc(TH1D* hist, std::string name);*/
	void vector_from_file(DVECTOR &what, std::string fname);
	void vector_from_file(STD_CONT<STD_CONT<peak>> &pks, std::string fname);
public:
	AllRunsResults(ParameterPile::experiment_area experiment);//only experiment and channels are important here
	void processAllRuns(STD_CONT<SingleRunResults> &single_results);
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
