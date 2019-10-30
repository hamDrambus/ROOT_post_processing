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
	enum Type /*: std::size_t*/ {
		MPPC_Ss, MPPC_As, MPPC_t_S, MPPC_A_S, MPPC_Double_I, /*(old) MPPC_times*/ MPPC_tbS /*time by N*/, /*(old) MPPC_times_N*/ MPPC_tbN /*time by N*/,
		/*(old) MPPC_sum_ts*/ MPPC_tbS_sum /*time distribution with weights as peak area S*/,
		MPPC_tbN_sum /*time distribution with weights as peak Npe*/, MPPC_coord, MPPC_coord_x, MPPC_coord_y, MPPC_Npe_sum, MPPC_S2,
		Correlation /*uses x,y Type's cuts*/,CorrelationAll, PMT_S2_S, PMT_Npe_sum, PMT_S2_int, PMT_Ss, PMT_t_S, PMT_A_S, PMT_tbS, PMT_tbN, PMT_sum_N};
protected:
	const Type _first_state;
	const Type _last_state;
	Type _x_corr;
	Type _y_corr;
	int _x_corr_ch;
	int _y_corr_ch;
	int MPPC_last_ch;
	int PMT_last_ch;
	Type MPPC_last_type;
	Type PMT_last_type;
	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type);
	int channel_to_index(int ch);
	int channel_to_index(int ch, Type type);

	void loop_channels_reset(void);
	bool loop_channels (Type type, int &ch, int &ch_ind);
	int ch_ind_loop;
	Type type_loop;
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
	AnalysisStates(std::deque<int> &mppc_channels_, std::deque<int> &pmt_channels_, std::deque<std::string>& experiments_);
	virtual ~AnalysisStates();
	Bool_t NextType(void);
	Bool_t PrevType(void);
	bool GotoT(Type to_type);
	Bool_t NextCh(void);
	Bool_t PrevCh(void);
	Bool_t GotoCh(int channels);
	Bool_t NextExp(void);
	Bool_t PrevExp(void);
	Bool_t isValid();
	Bool_t isPerRun(Type type);
	Bool_t isMultichannel(Type type);
	Bool_t is_TH1D_hist(Type type);
	bool isComposite (Type type);
	Bool_t is_PMT_type(Type type);
};

#endif
