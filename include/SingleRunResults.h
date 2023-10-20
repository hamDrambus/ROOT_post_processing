#ifndef SINGLE_RUN_RESULTS_H
#define SINGLE_RUN_RESULTS_H

#include "GlobalParameters.h"
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
	/*ParameterPile::*/experiment_area curr_area;
	
	Bool_t is_valid;
public:
	SingleRunResults(SingleRunData *of_what);
	SingleRunResults::Status getStatus(void) const;
	Bool_t isValid(void) const;
	void setValid(Bool_t val);

friend SingleRunData;
friend AnalysisManager;
friend AllRunsResults;
};

#endif
