#ifndef ANALYSIS_STATES_H
#define ANALYSIS_STATES_H

#include "GlobalParameters.h"

//TODO: CorrelationAll is not only multichannel but also multiexperiment.
class AStates { //short for AnalysisStates - faster to type (autocomplete) in root console.
public:
	enum Type {
		MPPC_Ss, MPPC_As, MPPC_t_S, MPPC_A_S, MPPC_Double_I, MPPC_tbS /*time by peak area*/, MPPC_tbN /*time by N*/,
		MPPC_tbS_sum /*time distribution with weights as peak area S*/,
		MPPC_tbN_sum /*time distribution with weights as peak Npe*/, MPPC_coord, MPPC_coord_x, MPPC_coord_y, MPPC_Npe_sum, MPPC_N_sum, MPPC_S2,
		Correlation_x, Correlation_y /*these two are mask (virtual) types*/, Correlation /*uses Correlation_x/y*/,CorrelationAll,
		PMT_S2_S, PMT_Npe_sum, PMT_S2_int, PMT_Ss, PMT_As, PMT_t_S, PMT_A_S, PMT_tbS, PMT_tbN, PMT_tbNpe, PMT_sum_N, PMT_trigger_fit, PMT_trigger_bNpe,
		PMT_trigger_bNpeaks, PMT_trigger_bS};
protected:
	const Type _first_state;
	const Type _last_state;
	int MPPC_last_ch;
	int PMT_last_ch;
	Type MPPC_last_type;
	Type PMT_last_type;
	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type);
	virtual Bool_t CorrelationXChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type);
	virtual Bool_t CorrelationYChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type);
	int channel_to_index(int ch) const;
	int channel_to_index(int ch, Type type) const;

	void loop_channels_reset(void);
	int ch_ind_loop;
	Type type_loop;
	std::deque<int> channel_list(Type type) const;
public:
	bool loop_channels (Type type, int &ch, int &ch_ind);
	Type _x_corr;
	Type _y_corr;
	int _x_corr_ch;
	int _y_corr_ch;
	bool SetCorrelation(Type x, Type y, int chx, int chy);
	bool SetCorrelation_x(Type x_type, int x_ch);
	bool SetCorrelation_y(Type y_type, int y_ch);
	int mppc_channel_to_index(int ch) const;
	int pmt_channel_to_index(int ch) const;
	std::deque<int> MPPC_channels;
	std::deque<int> PMT_channels;
	std::deque<std::string> experiments;
	Type current_type;
	int current_channel;
	int current_exp_index;
	AStates(std::deque<int> &mppc_channels_, std::deque<int> &pmt_channels_, std::deque<std::string>& experiments_);
	virtual ~AStates();
	Bool_t NextType(void);
	Bool_t PrevType(void);
	bool GotoT(Type to_type);
	Bool_t NextCh(void);
	Bool_t PrevCh(void);
	Bool_t GotoCh(int channels);
	Bool_t NextExp(void);
	Bool_t PrevExp(void);
	Bool_t isValid() const;
	std::string type_name(Type type) const;
	Bool_t isPerRun(Type type) const;
	Bool_t isMultichannel(Type type) const;
	Bool_t isTH1Dhist(Type type) const;
	bool isComposite (Type type) const;
	Bool_t isPMTtype(Type type) const;
	Bool_t isVirtual(Type type) const;
	//Bool_t isTrigger(Type type) const;
};

#endif
