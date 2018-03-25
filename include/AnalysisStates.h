#ifndef ANALYSIS_STATES_H
#define ANALYSIS_STATES_H

#include "GlobalParameters.h"

//TODO: add class StateGroup(?), which is stored in AnalysisStates. So this class will have
//several state groups like: MPPCs with their types, PMT slow (ch0-1), PMT fast (ch8,12) and possibly more.
//TODO: PMT_times now is valid for both ch{0,1} and ch{8,12} whereas it should be only for the latter.
//similar goes for PMT_S2_S, PMT_Ss. PMT_t_S is good for both.
class AnalysisStates {
public:
	enum Type {
		MPPC_Ss, MPPC_t_S, MPPC_Double_I, MPPC_S2_S, MPPC_times, MPPC_sum_ts, MPPC_coord, MPPC_coord_x, MPPC_coord_y, MPPC_S2, MPPC_tfinal, MPPC_tstart, MPPC_tboth,
		PMT_S2_S, PMT_Ss, PMT_t_S, PMT_times};
protected:
	const Type _first_state;
	const Type _last_state;
	int MPPC_last_ch;
	int PMT_last_ch;
	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type,Bool_t save);
	Bool_t is_PMT_type(Type type);
	int channel_to_index(int ch);
public:
	int mppc_channel_to_index(int ch);
	int pmt_channel_to_index(int ch);
	std::deque<int> MPPC_channels;
	std::deque<int> PMT_channels;
	std::deque<std::string> experiments;
	Type current_type;
	int current_channel;
	int current_exp_index;
	std::string type_name(Type type);
	AnalysisStates(std::deque<int> &mppc_channsels_, std::deque<int> &pmt_channsels_, std::deque<std::string>& experiments_);
	Bool_t NextType(Bool_t save = kTRUE);
	Bool_t PrevType(Bool_t save = kTRUE);
	Bool_t NextCh(Bool_t save = kTRUE);
	Bool_t PrevCh(Bool_t save = kTRUE);
	Bool_t NextExp(Bool_t save = kTRUE);
	Bool_t PrevExp(Bool_t save = kTRUE);
	Bool_t isValid();
	Bool_t isMultichannel(Type type);
};

#endif
