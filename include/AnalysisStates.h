#ifndef ANALYSIS_STATES_H
#define ANALYSIS_STATES_H

#include "GlobalParameters.h"

//TODO: add class StateGroup(?), which is stored in AnalysisStates. So this class will have
//several state groups like: MPPCs with their types, PMT slow (ch0-1), PMT fast (ch8,12) and possibly more.
//TODO: PMT_times now is valid for both ch{0,1} and ch{8,12} whereas it should be only for the latter.
//similar goes for PMT_S2_S, PMT_Ss. PMT_t_S is good for both.
//TODO: CorrelationAll is not only multichannel but also multiexperiment.
class AnalysisStates {
public:
	enum Type {
		MPPC_Ss, MPPC_t_S, MPPC_Double_I, MPPC_S2_S, MPPC_times, MPPC_sum_ts, MPPC_coord, MPPC_coord_x, MPPC_coord_y, MPPC_Npe_sum, MPPC_S2, MPPC_tfinal, MPPC_tstart, MPPC_tboth,
		Correlation /*uses x,y Type's cuts*/,CorrelationAll, PMT_S2_S, PMT_S2_int, PMT_Ss, PMT_t_S, PMT_times};
protected:
	const Type _first_state;
	const Type _last_state;
	Type _x_corr;
	Type _y_corr;
	int _x_corr_ch;
	int _y_corr_ch;
	int MPPC_last_ch;
	int PMT_last_ch;
	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type,Bool_t save);
	int channel_to_index(int ch);
	int channel_to_index(int ch, Type type);
public:
	bool SetCorrelation(Type x, Type y, int chx, int chy);
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
	bool GotoT(Type to_type, bool save = true);
	Bool_t NextCh(Bool_t save = kTRUE);
	Bool_t PrevCh(Bool_t save = kTRUE);
	Bool_t NextExp(Bool_t save = kTRUE);
	Bool_t PrevExp(Bool_t save = kTRUE);
	Bool_t isValid();
	Bool_t isMultichannel(Type type);
	Bool_t is_TH1D_hist(Type type);
	bool isPerRun (Type type);
	bool isPerPeak (Type type);
	bool isComposite (Type type);
	Bool_t is_PMT_type(Type type);
};

#endif
