#ifndef EVENT_CUT_H
#define EVENT_CUT_H

#include "GlobalParameters.h"
#include "AnalysisStates.h"
#include "FunctionWrapper.h"

//this class can be used in the two modes:
//1) run selection (based on PMT): based on a single channel and type, applied for all runs and a single experiment
//2) Histogram selection: for example cuts peaks' S to be > 0 (by default). In this case do_accept is not used.
//The case when selection for channel1 and type1 is based on the channel2 and type2 is not implemented.
class FunctionWrapper;

class EventCut //has no logic, basically data class.
{
public:
	enum CutType {RunCut, HistCut};
protected:
	CutType cut_type;
	std::string cut_name;
	//run (only for a single experiment)
	std::deque<Bool_t> do_accept;
	FunctionWrapper *value_picker; //for input pars meanings see "PostProcessor.h" (depends on AnalysisState::Type)
	int exp_ind; //for which experiment it is applied
	int channel; //for which channel (e.g. 0 is the Sum of pmt)
	AStates::Type type; //for which type. Defines the number of parameters
	Bool_t affects_histogram; //if false, only display cut and use it for determination of analysis output parameters.
public:
	EventCut(int N_of_runs, CutType type, std::string name = "");
	~EventCut();
	EventCut(const EventCut &);//= delete; //TODO: implement (currently have to decide how to copy value_picker which stores (CUTTER) and (void*)). ATM there is memory leak!
	void SetName(std::string nm);
	std::string GetName(void) const;
	void SetPicker(FunctionWrapper *picker);
	FunctionWrapper* GetPicker(void) const;

	Bool_t operator () (std::vector<double> &pars, int run);
	Bool_t Draw (TCanvas *can);

	void SetExperiment(int exp_ind);
	int GetExperiment(void) const;
	void SetChannel(int ch);
	int GetChannel(void) const;
	void SetType(AStates::Type t);
	AStates::Type GetType(void) const;
	void SetAffectingHistogram(Bool_t affect_hist);
	Bool_t GetAffectingHistogram(void) const;

	void SetAccept(int run, Bool_t accept);
	Bool_t GetAccept(int run) const;
	std::size_t GetRejectedN(void) const;
};

#endif
