#include "AnalysisStates.h"

AnalysisStates::AnalysisStates(std::deque<int> &mppc_channels_, std::deque<int> &pmt_channels_, std::deque<std::string>& experiments_):
_first_state(MPPC_Ss), _last_state(PMT_sum_N),_x_corr(MPPC_Npe_sum),_y_corr(PMT_S2_S), ch_ind_loop(0), type_loop(_first_state)
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
	PMT_last_type = PMT_t_S;
	MPPC_last_type = _first_state;
	if (MPPC_last_ch<0)
		_x_corr = PMT_S2_S;
	if (PMT_last_ch<0)
		_y_corr = MPPC_Npe_sum;
	_x_corr_ch = is_PMT_type(_x_corr) ? PMT_last_ch : MPPC_last_ch;
	_y_corr_ch = is_PMT_type(_y_corr) ? PMT_last_ch : MPPC_last_ch;
}

AnalysisStates:: ~AnalysisStates()
{}

Bool_t AnalysisStates::StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type)
{
	if (!isMultichannel(to_type)) {
		if (is_PMT_type(to_type))
			PMT_last_ch = to_ch;
		else
			MPPC_last_ch = to_ch;
	}
	if (is_PMT_type(to_type))
		PMT_last_type = to_type;
	else
		MPPC_last_type = to_type;
	return !((to_ch == from_ch)&&(to_exp==from_exp)&&(to_type==from_type));
}

Bool_t AnalysisStates::NextType(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == _last_state) ? _first_state : (Type)(current_type + 1);
		if (isMultichannel(current_type)) {
			current_channel = -1;
		} else {
			//changed from PMT to MPPC and channels became invalid
			if (is_PMT_type(current_type) != is_PMT_type(prev_type))
				current_channel = is_PMT_type(current_type) ? PMT_last_ch : MPPC_last_ch;
			else
				current_channel = prev_ch;
		}
	} while (!isValid());//invalid here means that there is no PMT or MPPC to which the switch have happened
	return StateChange(current_channel,current_exp_index,current_type,prev_ch,prev_exp,prev_type);
}

bool AnalysisStates::GotoT(Type to_type)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_type = to_type;
	if (isMultichannel(current_type)) {
		current_channel = -1;
	} else {
		//changed from PMT to MPPC and channels became invalid
		if (is_PMT_type(current_type) != is_PMT_type(prev_type))
			current_channel = is_PMT_type(current_type) ? PMT_last_ch : MPPC_last_ch;
		else
			current_channel = prev_ch;
	}
	if (!isValid()){
		std::cout<<"can't go to "<<type_name(current_type)<<" type (no channels)"<<std::endl;
		current_type = prev_type;
	}
	return StateChange(current_channel,current_exp_index,current_type,prev_ch,prev_exp,prev_type);
}

Bool_t AnalysisStates::PrevType(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == _first_state) ? _last_state : (Type)(current_type - 1);
		if (isMultichannel(current_type)) {
			current_channel = -1;
		} else {
			//changed from PMT to MPPC and channels became invalid
			if (is_PMT_type(current_type) != is_PMT_type(prev_type))
				current_channel = is_PMT_type(current_type) ? PMT_last_ch : MPPC_last_ch;
			else
				current_channel = prev_ch;
		}
	} while (!isValid());//invalid here means that there is no PMT or MPPC to which the switch have happened
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AnalysisStates::NextCh(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	
	if (isMultichannel(current_type)){
		std::cout<<"Warning: current type has no channels"<<std::endl;
		return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
	}

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
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AnalysisStates::PrevCh(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;

	if (isMultichannel(current_type)) {
		std::cout<<"Warning: current type has no channels"<<std::endl;
		return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
	}

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
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AnalysisStates::GotoCh(int channel) //ignores types
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;

	if (-1==channel) {
		if (isMultichannel(current_type)) {
			return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
		}
		Type mch_type = (current_type == _last_state ? _first_state : (Type)(current_type+1));
		while (!isMultichannel(mch_type)&&mch_type!=current_type) {
			mch_type = (mch_type == _last_state ? _first_state : (Type)(mch_type+1));
		}
		if (mch_type==current_type){
			std::cout<<"Warning: Can't proceed to -1 channel - no multichannel type"<<std::endl;
		}
		current_type = mch_type;
		return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
	}

	int ind_pmt = pmt_channel_to_index(channel);
	int ind_mppc = mppc_channel_to_index(channel);
	if (ind_pmt<0 && ind_mppc<0) {
		std::cout<<"Error: no such channel for either PMT or MPPC"<<std::endl;
		return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
	}
	current_channel = channel;
	if (!(ind_pmt<0)) {
		if (!is_PMT_type(current_type))
			current_type = PMT_last_type;
	} else {
		if (is_PMT_type(current_type))
			current_type = MPPC_last_type;
	}
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AnalysisStates::NextExp(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == (experiments.size()-1)) ? 0 : current_exp_index+1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AnalysisStates::PrevExp(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == 0) ? (experiments.size() - 1) : current_exp_index - 1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AnalysisStates::is_PMT_type(Type type)
{
	return (type == PMT_S2_S || type == PMT_Ss || type == PMT_t_S || type == PMT_times|| PMT_S2_int==type || PMT_A_S == type || PMT_times_N == type || PMT_sum_N == type);
}

Bool_t AnalysisStates::isMultichannel(Type type)
{
	return (type == MPPC_sum_ts)||(type==MPPC_coord)||(type==MPPC_coord_x)||(type==MPPC_coord_y)||(type==MPPC_Npe_sum)||(type==Correlation)||(type==CorrelationAll)||(type==PMT_sum_N);
}

Bool_t AnalysisStates::is_TH1D_hist(Type type)
{
	return !((type == Type::PMT_t_S)||(type == Type::PMT_A_S) || (type == Type::MPPC_t_S)|| (type == Type::MPPC_A_S)||(type== Type::MPPC_coord)||(type==Correlation)||(type==CorrelationAll));
}

bool AnalysisStates::isComposite (Type type)
{
	return ((type == MPPC_coord) || (type == MPPC_coord_x)||(type== MPPC_coord_y)||(type==Correlation)||(type==CorrelationAll)||(type==MPPC_Npe_sum)||(type==MPPC_S2)||(type==PMT_S2_S)||(type==PMT_sum_N));
}

Bool_t AnalysisStates::isValid()
{
	if (MPPC_channels.empty() && PMT_channels.empty())
		return kFALSE;
	
	if (-1==current_channel&&isMultichannel(current_type))
		return (is_PMT_type(current_type)&&!PMT_channels.empty())||(!is_PMT_type(current_type)&&!MPPC_channels.empty());

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
	if (mppc_ch && (is_PMT_type(current_type)))
		return kFALSE;
	return kTRUE;
}

bool AnalysisStates::SetCorrelation(Type x, Type y, int chx, int chy)
{
	if (isPerRun(x)&&is_TH1D_hist(x)&&is_TH1D_hist(y)&&isPerRun(y)){
		bool ok = true;
		if (channel_to_index(chx, x)<0) {
			std::cout<<"Channel x is invalid."<< std::endl;
			ok = false;
		}
		if (channel_to_index(chy, y)<0) {
			std::cout<<"Channel y is invalid."<< std::endl;
			ok = false;
		}
		if (ok) {
			_x_corr_ch = chx;
			_y_corr_ch = chy;
			_x_corr = x;
			_y_corr = y;
		}
		//StateChange(current_channel, current_exp_index, current_type, current_channel, current_exp_index, current_type, ok);
		return true;
	} else
		std::cout<<"Can't uses these types for correlation"<<std::endl;
	return false;
}

int AnalysisStates::channel_to_index(int ch)
{
	return channel_to_index(ch, current_type);
}

int AnalysisStates::channel_to_index(int ch, Type type)
{
	if (isMultichannel(type)) {
		return (-1==ch ? 0 : -1);
	}
	if (is_PMT_type(type))
		return pmt_channel_to_index(ch);
	else
		return mppc_channel_to_index(ch);
	return -1;
}

void AnalysisStates::loop_channels_reset(void)
{
	ch_ind_loop=0;
}

bool AnalysisStates::loop_channels (Type type, int &ch, int &ch_ind)
{
	if(type!=type_loop)
		loop_channels_reset();
	type_loop = type;
	if (isMultichannel(type_loop)) {
		if (ch_ind_loop>0) {
			ch = -1;
			ch_ind = -1;
			loop_channels_reset();
			return false;
		}
		ch = -1;
		ch_ind = 0;
		++ch_ind_loop;
		return true;
	}
	if (is_PMT_type(type_loop)) {
		if (ch_ind_loop>=PMT_channels.size()) {
			ch = -1;
			ch_ind = -1;
			loop_channels_reset();
			return false;
		}
		ch = PMT_channels[ch_ind_loop];
		ch_ind = ch_ind_loop;
		++ch_ind_loop;
		return true;
	} else {
		if (ch_ind_loop>=MPPC_channels.size()) {
			ch = -1;
			ch_ind = -1;
			loop_channels_reset();
			return false;
		}
		ch = MPPC_channels[ch_ind_loop];
		ch_ind = ch_ind_loop;
		++ch_ind_loop;
		return true;
	}
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
	return -1;
}

std::string AnalysisStates::type_name(Type type)
{
	std::string name;
	switch (type){
	case Type::MPPC_Ss:{
		name += "_Ss";
		break;
	}
	case Type::MPPC_S2_S:{
		name += "_S2_auto";
		break;
	}
	case Type::MPPC_Double_I:{
		name += "_Double_I";
		break;
	}
	case Type::MPPC_tboth:{
		name += "_t_both";
		break;
	}
	case Type::MPPC_tfinal:{
		name += "_t_final";
		break;
	}
	case Type::MPPC_tstart:{
		name += "_t_start";
		break;
	}
	case Type::MPPC_t_S:{
		name += "_t_S";
		break;
	}
	case Type::MPPC_A_S:{
		name += "_A_S";
		break;
	}
	case Type::MPPC_sum_ts:{
		name += "_sum_ts";
		break;
	}
	case Type::MPPC_coord: {
		name += "_coordinate";
		break;
	}
	case Type::MPPC_coord_x: {
		name += "_x_coordinate";
		break;
	}
	case Type::MPPC_coord_y: {
		name += "_y_coordinate";
		break;
	}
	case Type::MPPC_Npe_sum :{
		name += "_N_pe";
		break;
	}
	case Type::MPPC_S2:{
		name += "_S2_manual";
		break;
	}
	case Type::Correlation:{
		name += "_Corr:";
		name += type_name(_x_corr) + "&" + type_name(_y_corr);
		break;
	}
	case Type::CorrelationAll:{
		name += "_Corr_all:";
		name += type_name(_x_corr) + "&" + type_name(_y_corr);
		break;
	}
	case Type::PMT_S2_S:{
		name += "_S2_S";
		break;
	}
	case Type::PMT_S2_int: {
		name += "_S2_by_integral";
		break;
	}
	case Type::PMT_Ss:{
		name += "_Ss";
		break;
	}
	case Type::PMT_t_S:{
		name += "_t_S";
		break;
	}
	case Type::PMT_A_S:{
		name += "_A_S";
		break;
	}
	case Type::MPPC_times: {
		name+="_signal_form";
		break;
	}
	case Type::MPPC_times_N: {
		name+="_signal_form_by_N_peaks";
		break;
	}
	case Type::PMT_times: {
		name+="_signal_form";
		break;
	}
	case Type::PMT_times_N: {
		name+="_signal_form_by_N_peaks";
		break;
	}
	case Type::PMT_sum_N: {
		name+="_sum_of_N_peaks";
		break;
	}
	}
	return name;
}
