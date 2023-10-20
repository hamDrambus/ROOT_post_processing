#ifndef SINGLE_RUN_DATA_H
#define SINGLE_RUN_DATA_H

#include "GlobalParameters.h"
#include "SignalOperations.h"
#include "SingleRunResults.h"
#include "AllRunsResults.h"

class AnalysisManager;
class AllRunsResults;
class SingleRunResults;

class SingleRunData
{
protected:
	/*ParameterPile::*/experiment_area curr_area; //sets channel area
public:
	SingleRunData(/*ParameterPile::*/experiment_area area);
	SingleRunResults processSingleRun_Iter_0(const AllRunsResults *all_runs_results);
	SingleRunResults processSingleRun_Iter_1(const AllRunsResults *all_runs_results);
	SingleRunResults processSingleRun(const AllRunsResults *all_runs_results);
	void clear_memory(void); //clears only 'input' data, preserves processing results
	/*ParameterPile::*/experiment_area getArea(void) const;
};

#endif