#ifndef ANALYSIS_MANAGER_H
#define ANALYSIS_MANAGER_H

#include "TH1D.h"
#include "TH1I.h"
#include "GlobalParameters.h"
#include "SingleRunData.h"
#include "AllRunsResults.h"
#include "AllExperimentsResults.h"

class AnalysisManager
{
public:
	AnalysisManager(/*ParameterPile::*/experiment_area area);
protected:
	/*ParameterPile::*/experiment_area _exp_area;
	//ParameterPile::experiment_area first_level_processed;  //per run processed - e.g. filters
	//ParameterPile::experiment_area second_level_processed; //per experiment processed - averaged per runs.
	//ParameterPile::experiment_area third_level_processed;  //everything
	/*ParameterPile::*/experiment_area current_under_processing;

	std::deque<SingleRunData> one_run_data;
	std::deque<SingleRunResults> one_run_results;
	std::deque<AllRunsResults> all_runs_results;
	AllExperimentsResults all_exp_results;

	TCondition* _cond;
	TMutex* _thread_mutex;

	enum NextRunIs { FirstRun, NewSubRun, NewRun, NewExperiment, Null} curr_run;
	virtual void processOneRun_first_iteration(AllRunsResults *_all_results);
	virtual void nextRun(void);
	virtual void processAllRuns(void);
	virtual void loopAllRuns_first_iteration(AllRunsResults *_all_results);
	virtual void loopAllRuns(AllRunsResults *_all_results);
	
public:
	
	virtual void processAllExperiments(void);
	virtual void proceessAllRunsOneThread(void);//there is supposed to be only single experiment //TODO: there is the first and second iterations
	//after the first one, one_run_data must not be erased for higher perfomance. After the second iteration data should be erased.
	//In the case of several experements it may be expensive to store data for every one for the second iteration
	std::deque<AllRunsResults>* getAllRunsResults(void);
	void setAllRunsResults(AllRunsResults* to_what);
	void setCondition(TCondition* cond);
	TCondition* getCondition(void);
	void setThreadMutex(TMutex* mutex);
	TMutex* getThreadMutex(void);
	
	AllExperimentsResults* getAllExperimentsResults(void);
};

#endif