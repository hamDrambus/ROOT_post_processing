#ifndef ANALYSIS_STATES_H
#define ANALYSIS_STATES_H

#include "GlobalParameters.h"
#include "AllExperimentsResults.h"

class AllExperimentsResults;

class AnalysisStates {
public:
	enum Type {
		MPPC_Ss, MPPC_tfinal, MPPC_tstart, MPPC_tboth, MPPC_double_I, MPPC_S2_S, MPPC_t_S,
		PMT_S2_S, PMT_Ss, PMT_t_S};
protected:
	int MPPC_last_ch;
	int PMT_last_ch;
	virtual bool StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type,bool save);
	bool is_PMT_type(Type type);
	int channel_to_index(int ch);
public:
	int mppc_channel_to_index(int ch);
	int pmt_channel_to_index(int ch);
	STD_CONT<int> MPPC_channels;
	STD_CONT<int> PMT_channels;
	STD_CONT<std::string> experiments;
	Type current_type;
	int current_channel;
	int current_exp_index;
	AnalysisStates(AllExperimentsResults* data);
	bool NextType(bool save = true);
	bool PrevType(bool save = true);
	bool NextCh(bool save = true);
	bool PrevCh(bool save = true);
	bool NextExp(bool save = true);
	bool PrevExp(bool save = true);
	bool isValid();
};

#endif