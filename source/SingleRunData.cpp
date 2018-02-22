#include "SingleRunData.h"
#include <string>

SingleRunData::SingleRunData(/*ParameterPile::*/experiment_area area)
{
	curr_area.channels = area.channels;
	curr_area.experiments = area.experiments;
	curr_area.runs.push_back(area.runs.back());//only one run is in this class, so no pairs
	curr_area.sub_runs.push_back(area.sub_runs.back()); //only one subrun is in this class
}

SingleRunResults SingleRunData::processSingleRun_Iter_0(const AllRunsResults *all_runs_results)
{
	SingleRunResults _result(this);
	return _result;
}

SingleRunResults SingleRunData::processSingleRun_Iter_1(const AllRunsResults *all_runs_results)
{
	SingleRunResults _result(this);
	_result.setValid(kTRUE);
	_result._current_status = SingleRunResults::Status::Ok;
	return _result;
}

SingleRunResults SingleRunData::processSingleRun(const AllRunsResults *all_runs_results)
{
	if (0 == all_runs_results->Iteration())
		return processSingleRun_Iter_0(all_runs_results);
	if (1 == all_runs_results->Iteration())
		return processSingleRun_Iter_1(all_runs_results);
	return SingleRunResults(this);
}

void SingleRunData::clear_memory(void) //clears only 'input' data, preserves processing results
{}

/*ParameterPile::*/experiment_area SingleRunData::getArea(void) const {	return curr_area;}
