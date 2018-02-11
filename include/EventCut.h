#ifndef EVENT_CUT_H
#define EVENT_CUT_H

#include "GlobalParameters.h"
#include "AnalysisStates.h"

class AnalysisStates;

//this class can be used in the two modes:
//1) run selection (based on PMT): based on a single channel and type, applied for all runs and a single experiment
//2) Histogram selection: for example cuts peaks' S to be > 0 (by default). Applied for the same type and channel as cutted by.
//	in this case do_accept is not used.
//The case when selection for channel1 and type1 is based on the channel2 and type2 is not implemented.
class EventCut
{
public:
	enum CutType {RunCut, HistCut};
protected:
	CutType cut_type;
	std::string cut_name;
	//run (only for a single experiment)
	STD_CONT<bool> do_accept;
	std::function<bool(DVECTOR &vals)> value_picker; //for vals sizes see "PostProcessor.h" (depends on AnalysisState::Type)
	int exp_ind;//for which experiment it is applied
	int channel; //by which channel (e.g. 0 is the Sum of pmt)
	AnalysisStates::Type type; //by which type. Defines the number of parameters
public:
	EventCut(int N_of_runs, CutType type, std::string name = "");
	void SetName(std::string nm);
	std::string GetName(void) const;
	void SetPicker(std::function<bool(DVECTOR &vals)> &picker);
	std::function<bool(DVECTOR &vals)> GetPicker(void) const;

	void SetExperiment(int exp_ind);
	int GetExperiment(void) const;
	void SetChannel(int ch);
	int GetChannel(void) const;
	void SetType(AnalysisStates::Type t);
	AnalysisStates::Type GetType(void) const;

	void SetAccept(int run, bool accept);
	bool GetAccept(int run) const;
};

#endif
