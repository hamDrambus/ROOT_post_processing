#include "AnalysisStates.h"

AnalysisStates::AnalysisStates(std::deque<Int_t> &mppc_channels_, std::deque<Int_t> &pmt_channels_, std::deque<std::string>& experiments_):
_first_state(MPPC_Ss), _last_state(PMT_t_S)
{
	MPPC_channels = mppc_channels_;
	PMT_channels = pmt_channels_;
	experiments = experiments_;
	current_type = MPPC_Ss;
	current_channel = MPPC_channels.empty() ? -1 : MPPC_channels[0];
	MPPC_last_ch = current_channel;
	current_exp_index = experiments.empty() ? -1 : 0;
	PMT_last_ch = PMT_channels.empty() ? -1 : PMT_channels[0];
	if (current_channel < 0 && isValid()) {
		current_channel = PMT_last_ch;
		current_type = PMT_Ss;
	}
}

Bool_t AnalysisStates::StateChange(Int_t to_ch, Int_t to_exp, Type to_type, Int_t from_ch, Int_t from_exp, Type from_type, Bool_t save)
{
	return !((to_ch == from_ch)&&(to_exp==from_exp)&&(to_type==from_type));
}

Bool_t AnalysisStates::NextType(Bool_t save)
{
	if (!isValid())
		return kFALSE;
	Int_t prev_ch = current_channel;
	Int_t prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == _last_state) ? _first_state : (Type)(current_type + 1);
		//changed from PMT to MPPC and channels became invalid
		if (!is_PMT_type(current_type) && is_PMT_type(prev_type)){
			PMT_last_ch = current_channel;
			current_channel = MPPC_last_ch;
		}
		if (is_PMT_type(current_type) && !is_PMT_type(prev_type)){
			MPPC_last_ch = current_channel;
			current_channel = PMT_last_ch;
		}
	} while (!isValid());//invalid here means that there is no PMT or MPPC to which the switch have happened
	return StateChange(current_channel,current_exp_index,current_type,prev_ch,prev_exp,prev_type,save);
}

Bool_t AnalysisStates::PrevType(Bool_t save)
{
	if (!isValid())
		return kFALSE;
	Int_t prev_ch = current_channel;
	Int_t prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == _first_state) ? _last_state : (Type)(current_type - 1);
		//changed from PMT to MPPC and channels became invalid
		if (!is_PMT_type(current_type) && is_PMT_type(prev_type)){
			PMT_last_ch = current_channel;
			current_channel = MPPC_last_ch;
		}
		if (is_PMT_type(current_type) && !is_PMT_type(prev_type)){
			MPPC_last_ch = current_channel;
			current_channel = PMT_last_ch;
		}
	} while (!isValid());//invalid here means that there is no PMT or MPPC to which the switch have happened
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type, save);
}

Bool_t AnalysisStates::NextCh(Bool_t save)
{
	if (!isValid())
		return kFALSE;
	Int_t prev_ch = current_channel;
	Int_t prev_exp = current_exp_index;
	Type prev_type = current_type;
	
	if (is_PMT_type(current_type)){
		for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
			if (current_channel == *i){
				current_channel = (i == (_end_ - 1)) ? *PMT_channels.begin() : *(i + 1);
				break;
			}
	} else {
		for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
			if (current_channel == *i) {
				current_channel = (i == (_end_ - 1)) ? *MPPC_channels.begin() : *(i + 1);
				break;
			}
	}
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type, save);
}

Bool_t AnalysisStates::PrevCh(Bool_t save)
{
	if (!isValid())
		return kFALSE;
	Int_t prev_ch = current_channel;
	Int_t prev_exp = current_exp_index;
	Type prev_type = current_type;

	if (is_PMT_type(current_type)){
		for (auto i = PMT_channels.rbegin(), _end_ = PMT_channels.rend(); i != _end_; ++i)
			if (current_channel == *i){
				current_channel = (i == (_end_ - 1)) ? *PMT_channels.rbegin() : *(i + 1);
				break;
			}
	} else {
		for (auto i = MPPC_channels.rbegin(), _end_ = MPPC_channels.rend(); i != _end_; ++i)
			if (current_channel == *i) {
				current_channel = (i == (_end_ - 1)) ? *MPPC_channels.rbegin() : *(i + 1);
				break;
			}
	}
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type, save);
}

Bool_t AnalysisStates::NextExp(Bool_t save)
{
	if (!isValid())
		return kFALSE;
	Int_t prev_ch = current_channel;
	Int_t prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == (experiments.size()-1)) ? 0 : current_exp_index+1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type, save);
}

Bool_t AnalysisStates::PrevExp(Bool_t save)
{
	if (!isValid())
		return kFALSE;
	Int_t prev_ch = current_channel;
	Int_t prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == 0) ? (experiments.size() - 1) : current_exp_index - 1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type, save);
}

Bool_t AnalysisStates::is_PMT_type(Type type)
{
	return (type == PMT_S2_S || type == PMT_Ss || type == PMT_t_S);
}

Bool_t AnalysisStates::isValid()
{
	if (MPPC_channels.empty() && PMT_channels.empty())
		return kFALSE;
	
	Bool_t mppc_ch = kFALSE;
	for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
		if (current_channel == *i){
			mppc_ch = kTRUE;
			break;
		}
	Bool_t pmt_ch = kFALSE;
	for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
		if (current_channel == *i){
			pmt_ch = kTRUE;
			break;
		}
	if (mppc_ch&&pmt_ch)
		return kTRUE;
	//MPPC_Ss, MPPC_tfinal, MPPC_tstart, MPPC_tboth, MPPC_Double_I, MPPC_S2_S, MPPC_t_S
	if (pmt_ch && !(is_PMT_type(current_type)))
		return kFALSE;
	//PMT_S2_S, PMT_Ss, PMT_t_S
	if (mppc_ch && is_PMT_type(current_type))
		return kFALSE;
	return kTRUE;
}

Int_t AnalysisStates::channel_to_index(Int_t ch)
{
	if (is_PMT_type(current_type))
		return pmt_channel_to_index(ch);
	else
		return mppc_channel_to_index(ch);
	return -1;
}

Int_t AnalysisStates::mppc_channel_to_index(Int_t ch)
{
	for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
		if (ch == *i)
			return (i - MPPC_channels.begin());
	return -1;
}
Int_t AnalysisStates::pmt_channel_to_index(Int_t ch)
{
	for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
		if (ch == *i)
			return (i - PMT_channels.begin());
	return -1;
}
