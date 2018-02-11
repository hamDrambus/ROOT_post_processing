#ifndef SINGLE_RUN_RESULTS_H
#define SINGLE_RUN_RESULTS_H

#include "GlobalParameters.h"
#include "GraphicOutputManager.h"
#include "SignalOperations.h"
#include "SingleRunData.h"

class AnalysisManager;
class SingleRunData;
class AllRunsResults;

class SingleRunResults
{
public:
	enum Status { Ok};
protected:
	Status _current_status;
	ParameterPile::experiment_area curr_area;
	
	bool is_valid;
public:
	SingleRunResults(SingleRunData *of_what);
	SingleRunResults::Status getStatus(void) const;
	bool isValid(void) const;
	void setValid(bool val);

friend SingleRunData;
friend AnalysisManager;
friend AllRunsResults;
};

#endif
