#include "AnalysisStates.h"

AnalysisStates::AnalysisStates(AllExperimentsResults* data)
{
	MPPC_channels = data->mppc_channels;
	PMT_channels = data->pmt_channels;
	experiments = data->exp_area.experiments;
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

bool AnalysisStates::StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type, bool save)
{
	return !((to_ch == from_ch)&&(to_exp==from_exp)&&(to_type==from_type));
}

bool AnalysisStates::NextType(bool save = true)
{
	if (!isValid())
		return false;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == PMT_t_S) ? MPPC_Ss : (Type)(current_type + 1);
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

bool AnalysisStates::PrevType(bool save = true)
{
	if (!isValid())
		return false;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == MPPC_Ss) ? PMT_t_S : (Type)(current_type - 1);
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

bool AnalysisStates::NextCh(bool save = true)
{
	if (!isValid())
		return false;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
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

bool AnalysisStates::PrevCh(bool save = true)
{
	if (!isValid())
		return false;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
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

bool AnalysisStates::NextExp(bool save = true)
{
	if (!isValid())
		return false;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == (experiments.size()-1)) ? 0 : current_exp_index+1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type, save);
}

bool AnalysisStates::PrevExp(bool save = true)
{
	if (!isValid())
		return false;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == 0) ? (experiments.size() - 1) : current_exp_index - 1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type, save);
}

bool AnalysisStates::is_PMT_type(Type type)
{
	return (type == PMT_S2_S || type == PMT_Ss || type == PMT_t_S);
}

bool AnalysisStates::isValid()
{
	if (MPPC_channels.empty() && PMT_channels.empty())
		return false;
	
	bool mppc_ch = false;
	for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
		if (current_channel == *i){
			mppc_ch = true;
			break;
		}
	bool pmt_ch = false;
	for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
		if (current_channel == *i){
			pmt_ch = true;
			break;
		}
	if (mppc_ch&&pmt_ch)
		return true;
	//MPPC_Ss, MPPC_tfinal, MPPC_tstart, MPPC_tboth, MPPC_double_I, MPPC_S2_S, MPPC_t_S
	if (pmt_ch && !(is_PMT_type(current_type)))
		return false;
	//PMT_S2_S, PMT_Ss, PMT_t_S
	if (mppc_ch && is_PMT_type(current_type))
		return false;
	return true;
}

int AnalysisStates::channel_to_index(int ch)
{
	if (is_PMT_type(current_type))
		return pmt_channel_to_index(ch);
	else
		return mppc_channel_to_index(ch);
	return -1;
}

int AnalysisStates::mppc_channel_to_index(int ch)
{
	for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
		if (ch == *i)
			return (i - MPPC_channels.begin());
	return -1;
}
int AnalysisStates::pmt_channel_to_index(int ch)
{
	for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
		if (ch == *i)
			return (i - PMT_channels.begin());
}
