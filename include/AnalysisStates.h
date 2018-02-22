#ifndef ANALYSIS_STATES_H
#define ANALYSIS_STATES_H

#include "GlobalParameters.h"

class AnalysisStates {
public:
	enum Type {
		MPPC_Ss, MPPC_tfinal, MPPC_tstart, MPPC_tboth, MPPC_Double_I, MPPC_S2_S, MPPC_t_S,
		PMT_S2_S, PMT_Ss, PMT_t_S};
protected:
	const Type _first_state;
	const Type _last_state;
	Int_t MPPC_last_ch;
	Int_t PMT_last_ch;
	virtual Bool_t StateChange(Int_t to_ch, Int_t to_exp, Type to_type, Int_t from_ch, Int_t from_exp, Type from_type,Bool_t save);
	Bool_t is_PMT_type(Type type);
	Int_t channel_to_index(Int_t ch);
public:
	Int_t mppc_channel_to_index(Int_t ch);
	Int_t pmt_channel_to_index(Int_t ch);
	std::deque<Int_t> MPPC_channels;
	std::deque<Int_t> PMT_channels;
	std::deque<std::string> experiments;
	Type current_type;
	Int_t current_channel;
	Int_t current_exp_index;
	AnalysisStates(std::deque<Int_t> &mppc_channsels_, std::deque<Int_t> &pmt_channsels_, std::deque<std::string>& experiments_);
	Bool_t NextType(Bool_t save = kTRUE);
	Bool_t PrevType(Bool_t save = kTRUE);
	Bool_t NextCh(Bool_t save = kTRUE);
	Bool_t PrevCh(Bool_t save = kTRUE);
	Bool_t NextExp(Bool_t save = kTRUE);
	Bool_t PrevExp(Bool_t save = kTRUE);
	Bool_t isValid();
};

#endif