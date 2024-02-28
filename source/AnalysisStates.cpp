#include "AnalysisStates.h"

AStates::AStates(std::deque<int> &mppc_channels_, std::deque<int> &pmt_channels_, std::deque<std::string>& experiments_):
_first_state(MPPC_Ss), _last_state(EventNumber),_x_corr(MPPC_Npe_sum),_y_corr(PMT_S2_S), ch_ind_loop(0), type_loop(_first_state)
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
	if (MPPC_last_ch >= 0) {
		_x_corr = MPPC_coord_x;
		_y_corr = MPPC_coord_x;
	} else {
		_x_corr = PMT_Npe_sum;
		_y_corr = PMT_Npe_sum;
	}
	_x_corr_ch = -1;
	_y_corr_ch = -1;
}

AStates::~AStates()
{}

Bool_t AStates::StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type)
{
	if (!isMultichannel(to_type) && !isVirtual(to_type)) {
		if (isPMTtype(to_type))
			PMT_last_ch = to_ch;
		else
			MPPC_last_ch = to_ch;
	}
	if (!isVirtual(to_type)) {
		if (isPMTtype(to_type))
			PMT_last_type = to_type;
		else
			MPPC_last_type = to_type;
	}
	if (to_exp == from_exp && to_type == from_type && to_type == Correlation_x) {
		_x_corr_ch = to_ch;
		CorrelationXChange(to_exp, to_ch, to_type, from_ch, from_type);
	}
	if (to_exp == from_exp && to_type == from_type && to_type == Correlation_y) {
		_y_corr_ch = to_ch;
		CorrelationYChange(to_exp, to_ch, to_type, from_ch, from_type);
	}
	return !((to_ch == from_ch)&&(to_exp==from_exp)&&(to_type==from_type));
}

Bool_t AStates::CorrelationXChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type)
{
	if (current_exp_index == exp_index && current_type == Correlation_x)
		current_channel = to_ch;
	return !((to_ch == from_ch) && (to_type == from_type));
}

Bool_t AStates::CorrelationYChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type)
{
	if (current_exp_index == exp_index && current_type == Correlation_y)
		current_channel = to_ch;
	return !((to_ch == from_ch) && (to_type == from_type));
}

Bool_t AStates::NextType(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == _last_state) ? _first_state : (Type)(current_type + 1);
		if (current_type == Correlation_x) {
			current_channel = _x_corr_ch;
			continue;
		}
		if (current_type == Correlation_y) {
			current_channel = _y_corr_ch;
			continue;
		}
		if (isMultichannel(current_type)) {
			current_channel = -1;
		} else {
			//changed from PMT to MPPC and channels became invalid
			if (isPMTtype(current_type) != isPMTtype(prev_type))
				current_channel = isPMTtype(current_type) ? PMT_last_ch : MPPC_last_ch;
			else
				current_channel = prev_ch;
		}
	} while (!isValid());//invalid here means that there is no PMT or MPPC to which the switch have happened
	return StateChange(current_channel,current_exp_index,current_type,prev_ch,prev_exp,prev_type);
}

bool AStates::GotoT(Type to_type)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_type = to_type;
	if (current_type == Correlation_x) {
		current_channel = _x_corr_ch;
		goto jump;
	}
	if (current_type == Correlation_y) {
		current_channel = _y_corr_ch;
		goto jump;
	}
	if (isMultichannel(current_type)) {
		current_channel = -1;
	} else {
		//changed from PMT to MPPC and channels became invalid
		if (isPMTtype(current_type) != isPMTtype(prev_type) || isMultichannel(prev_type))
			current_channel = isPMTtype(current_type) ? PMT_last_ch : MPPC_last_ch;
		else
			current_channel = prev_ch;
	}
jump:
	if (!isValid()){
		std::cout<<"can't go to "<<type_name(current_type)<<" type (no channels)"<<std::endl;
		current_type = prev_type;
		current_channel = prev_ch;
	}
	return StateChange(current_channel,current_exp_index,current_type,prev_ch,prev_exp,prev_type);
}

Bool_t AStates::PrevType(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	do {
		current_type = (current_type == _first_state) ? _last_state : (Type)(current_type - 1);
		if (current_type == Correlation_x) {
			current_channel = _x_corr_ch;
			continue;
		}
		if (current_type == Correlation_y) {
			current_channel = _y_corr_ch;
			continue;
		}
		if (isMultichannel(current_type)) {
			current_channel = -1;
		} else {
			//changed from PMT to MPPC and channels became invalid
			if (isPMTtype(current_type) != isPMTtype(prev_type))
				current_channel = isPMTtype(current_type) ? PMT_last_ch : MPPC_last_ch;
			else
				current_channel = prev_ch;
		}
	} while (!isValid());//invalid here means that there is no PMT or MPPC to which the switch have happened
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AStates::NextCh(void)
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

	if (isPMTtype(current_type)){
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

Bool_t AStates::PrevCh(void)
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

	if (isPMTtype(current_type)){
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

Bool_t AStates::GotoCh(int channel) //ignores types and virtual channels (Correlation_x/y)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;

	if (-1==channel) {
		current_channel = -1;
		if (isMultichannel(current_type)) {
			return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
		}
		Type mch_type = (current_type == _last_state ? _first_state : (Type)(current_type+1));
		while (!(isValid() && !isVirtual(mch_type)) && mch_type!=current_type) {
			mch_type = (mch_type == _last_state ? _first_state : (Type)(mch_type+1));
		}
		if (mch_type==current_type) {
			std::cout<<"Error: Can't proceed to -1 channel - no multichannel type"<<std::endl;
			current_channel = prev_ch;
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
		if (!isPMTtype(current_type))
			current_type = PMT_last_type;
	} else {
		if (isPMTtype(current_type))
			current_type = MPPC_last_type;
	}
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AStates::NextExp(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == (experiments.size()-1)) ? 0 : current_exp_index+1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AStates::PrevExp(void)
{
	if (!isValid())
		return kFALSE;
	int prev_ch = current_channel;
	int prev_exp = current_exp_index;
	Type prev_type = current_type;
	current_exp_index = (current_exp_index == 0) ? (experiments.size() - 1) : current_exp_index - 1;
	return StateChange(current_channel, current_exp_index, current_type, prev_ch, prev_exp, prev_type);
}

Bool_t AStates::isPMTtype(Type type) const
{
	if (type == Correlation_x)
		return isPMTtype(_x_corr);
	if (type == Correlation_y)
		return isPMTtype(_y_corr);
	if (type == Correlation)
		return isPMTtype(_x_corr)&&isPMTtype(_y_corr);
	return (type == PMT_S2_S || PMT_Npe_sum==type || PMT_S_sum==type || type == PMT_Ss || type == PMT_As || type == PMT_t_S
			|| PMT_A_S == type || type == PMT_tbS || PMT_tbN == type || PMT_tbNpe== type || PMT_sum_N == type
			|| type == PMT_trigger_bNpe || type==PMT_trigger_fit || type==PMT_trigger_bNpeaks|| type == PMT_trigger_bS || type == PMT_trigger_fit_chi2
			|| type == PMT_T_sum || type == PMT_shape_fit);
}

Bool_t AStates::isPerRun(Type type) const
{
	return  type==MPPC_coord|| type==MPPC_coord_x|| type==MPPC_coord_y|| type==MPPC_Npe_sum||type==MPPC_N_sum|| type==MPPC_S_sum ||
			type==MPPC_S2 || type == Correlation_x|| type == Correlation_y|| type==Correlation|| type==CorrelationAll||
			type== PMT_S2_S ||type==PMT_Npe_sum ||type==PMT_S_sum ||type==PMT_sum_N || type==PMT_trigger_bNpe ||
			type==PMT_trigger_fit || type==PMT_trigger_bNpeaks || type==PMT_trigger_bS || type == PMT_trigger_fit_chi2 ||
			type == MPPC_trigger_fit || type == MPPC_trigger_fit_chi2 || type == MPPC_trigger_avg || type == PMT_T_sum ||
			type == MPPC_shape_fit || type == PMT_shape_fit || type==MPPC_coord_disp || type == EventNumber;
}

Bool_t AStates::isMultichannel(Type type) const
{
	if (type == Correlation_x)
		return isMultichannel(_x_corr);
	if (type == Correlation_y)
		return isMultichannel(_y_corr);
	return (type == MPPC_tbS_sum) || type == MPPC_tbNpe_sum || type == MPPC_tbN_sum || type==MPPC_coord || type==MPPC_coord_x || type==MPPC_coord_y || type==MPPC_Npe_sum
			|| type==MPPC_N_sum || type==MPPC_S_sum || type == MPPC_trigger_avg || type == MPPC_shape_fit || type == PMT_shape_fit
			|| type==Correlation || type==CorrelationAll || type==PMT_sum_N || type==PMT_Npe_sum || type==PMT_S_sum || type==PMT_trigger_bNpe || type==PMT_trigger_bNpeaks
			|| type==PMT_trigger_fit || type==PMT_trigger_bS || type == PMT_trigger_fit_chi2 || type == MPPC_trigger_fit || type == MPPC_trigger_fit_chi2 || type == PMT_T_sum
			|| type==MPPC_Npe_profile || type==MPPC_Npe_profile_x ||type==MPPC_Npe_profile_y || type==MPPC_coord_disp || type == EventNumber;
}

Bool_t AStates::isTH1Dhist(Type type) const
{
	return !((type == Type::PMT_t_S)||(type == Type::PMT_A_S) || (type == Type::MPPC_t_S)||
			(type == Type::MPPC_A_S)||(type== Type::MPPC_coord)||(type==Correlation)||(type==CorrelationAll)||type==MPPC_Npe_profile);
}

bool AStates::isComposite (Type type) const
{
	if (type == Correlation_x)
		return isComposite(_x_corr);
	if (type == Correlation_y)
		return isComposite(_y_corr);
	return (type == MPPC_coord ||type == MPPC_coord_x ||type== MPPC_coord_y ||type==Correlation ||type==CorrelationAll
			|| type==MPPC_Npe_sum ||type==MPPC_N_sum || type==MPPC_S_sum ||type==MPPC_S2 ||type==PMT_S2_S ||type==PMT_sum_N || type==PMT_Npe_sum
			|| type==PMT_S_sum ||type==PMT_trigger_bNpe || type == PMT_trigger_bNpeaks || type==PMT_trigger_bS || type == PMT_trigger_fit
			|| type==PMT_trigger_fit_chi2 || type == MPPC_trigger_fit || type == MPPC_trigger_fit_chi2 || type == PMT_T_sum
			|| type == MPPC_trigger_avg || type == MPPC_shape_fit || type == PMT_shape_fit || type == MPPC_Npe_profile
			|| type == MPPC_Npe_profile_x ||type == MPPC_Npe_profile_y || type == MPPC_coord_disp);
}

Bool_t AStates::isVirtual(Type type) const
{
	return type == Correlation_x || type == Correlation_y || type == Correlation || type == CorrelationAll;
}

/*Bool_t AStates::isTrigger(Type type) const
{
	if (type == Correlation_x)
		return isTrigger(_x_corr);
	if (type == Correlation_y)
		return isTrigger(_y_corr);
	return type == PMT_trigger_bNpe || type == PMT_trigger_bNpeaks || type == PMT_trigger_bS;
}*/

Bool_t AStates::isValid() const
{
	if (MPPC_channels.empty() && PMT_channels.empty())
		return kFALSE;
	
	if (-1==current_channel&&isMultichannel(current_type))
		return (isPMTtype(current_type)&&!PMT_channels.empty())||(!isPMTtype(current_type)&&!MPPC_channels.empty());

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
	if (pmt_ch && !(isPMTtype(current_type)))
		return kFALSE;
	//PMT_S2_S, PMT_Ss, PMT_t_S
	if (mppc_ch && (isPMTtype(current_type)))
		return kFALSE;
	return kTRUE;
}

bool AStates::SetCorrelation(Type x, Type y, int chx, int chy)
{
	bool ok1 = SetCorrelation_x(x, chx);
	bool ok2 = SetCorrelation_y(y, chy);
	return ok1 || ok2;
}

bool AStates::SetCorrelation_x(Type x_type, int x_ch)
{
	if (isPerRun(x_type) && isTH1Dhist(x_type) && !isVirtual(x_type)) {
		if (channel_to_index(x_ch, x_type) < 0) {
			std::cout << "AStates::SetCorrelation_x::Error: channel "<< x_ch <<" is invalid for type " << type_name(x_type) << std::endl;
			return CorrelationXChange(current_exp_index, _x_corr_ch, _x_corr, _x_corr_ch, _x_corr);
		}
		Type old_ty = _x_corr;
		int old_ch = _x_corr_ch;
		_x_corr = x_type;
		_x_corr_ch = x_ch;
		return CorrelationXChange(current_exp_index, _x_corr_ch, _x_corr, old_ch, old_ty);
	} else
		std::cout << "AStates::SetCorrelation_x::Error: Can't use this type ("<< type_name(x_type) <<") for correlation x value" << std::endl;
	return CorrelationXChange(current_exp_index, _x_corr_ch, _x_corr, _x_corr_ch, _x_corr);
}

bool AStates::SetCorrelation_y(Type y_type, int y_ch)
{
	if (isPerRun(y_type) && isTH1Dhist(y_type) && !isVirtual(y_type)) {
		if (channel_to_index(y_ch, y_type) < 0) {
			std::cout << "AStates::SetCorrelation_y::Error: channel " << y_ch << " is invalid for type " << type_name(y_type) << std::endl;
			return CorrelationYChange(current_exp_index, _y_corr_ch, _y_corr, _y_corr_ch, _y_corr);
		}
		Type old_ty = _y_corr;
		int old_ch = _y_corr_ch;
		_y_corr = y_type;
		_y_corr_ch = y_ch;
		return CorrelationYChange(current_exp_index, _y_corr_ch, _y_corr, old_ch, old_ty);
	} else
		std::cout << "AStates::SetCorrelation_y::Error: Can't use this type (" << type_name(y_type) << ") for correlation y value" << std::endl;
	return CorrelationYChange(current_exp_index, _y_corr_ch, _y_corr, _y_corr_ch, _y_corr);
}

int AStates::channel_to_index(int ch) const
{
	return channel_to_index(ch, current_type);
}

int AStates::channel_to_index(int ch, Type type) const
{
	if (isMultichannel(type)) {
		return (-1==ch ? 0 : -1);
	}
	if (isPMTtype(type))
		return pmt_channel_to_index(ch);
	else
		return mppc_channel_to_index(ch);
	return -1;
}

void AStates::loop_channels_reset(void)
{
	ch_ind_loop=0;
}

bool AStates::loop_channels (Type type, int &ch, int &ch_ind)
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
	if (isPMTtype(type_loop)) {
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

std::deque<int> AStates::channel_list(Type type) const
{
	if (isMultichannel(type)) {
		if (isPMTtype(type))
			return PMT_channels;
		else
			return MPPC_channels;
	}
	if (type == Correlation_x)
		return std::deque<int>(1, _x_corr_ch);
	if (type == Correlation_y)
		return std::deque<int>(1, _y_corr_ch);
	return std::deque<int>(1, current_channel);
 }

int AStates::mppc_channel_to_index(int ch) const
{
	for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
		if (ch == *i)
			return (i - MPPC_channels.begin());
	return -1;
}
int AStates::pmt_channel_to_index(int ch) const
{
	for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
		if (ch == *i)
			return (i - PMT_channels.begin());
	return -1;
}

std::string AStates::type_name(Type type) const
{
	std::string name;
	switch (type) {
	case Type::Correlation_x: {
		name += "CorrX("+type_name(_x_corr)+")";
		break;
	}
	case Type::Correlation_y: {
		name += "CorrY(" + type_name(_y_corr) + ")";
		break;
	}
	case Type::MPPC_Ss:{
		name += "Ss";
		break;
	}
	case Type::MPPC_As:{
		name += "As";
		break;
	}
	case Type::MPPC_t_S:{
		name += "t_S";
		break;
	}
	case Type::MPPC_A_S:{
		name += "A_S";
		break;
	}
	case Type::MPPC_tbS_sum:{
		name += "form_by_S";
		break;
	}
	case Type::MPPC_tbNpe_sum:{
		name += "form_by_Npe";
		break;
	}
	case Type::MPPC_tbN_sum:{
		name += "form_by_Npeaks";
		break;
	}
	case Type::MPPC_coord: {
		name += "coordinate";
		break;
	}
	case Type::MPPC_coord_x: {
		name += "x_coordinate";
		break;
	}
	case Type::MPPC_coord_y: {
		name += "y_coordinate";
		break;
	}
	case Type::MPPC_Npe_sum :{
		name += "N_pe_sum";
		break;
	}
	case Type::MPPC_N_sum :{
		name += "N_sum";
		break;
	}
	case Type::MPPC_S_sum :{
		name += "S_sum";
		break;
	}
	case Type::MPPC_S2:{
		name += "S2";
		break;
	}
	case Type::Correlation:{
		name += "Corr:";
		name += type_name(_x_corr) + "&" + type_name(_y_corr);
		break;
	}
	case Type::CorrelationAll:{
		name += "Corr_all:";
		name += type_name(_x_corr) + "&" + type_name(_y_corr);
		break;
	}
	case Type::PMT_S2_S:{
		name += "S2_S";
		break;
	}
	case Type::PMT_Npe_sum:{
		name += "Npe_sum";
		break;
	}
	case Type::PMT_S_sum:{
		name += "S_sum";
		break;
	}
	case Type::PMT_Ss:{
		name += "Ss";
		break;
	}
	case Type::PMT_As:{
		name += "As";
		break;
	}
	case Type::PMT_t_S:{
		name += "t_S";
		break;
	}
	case Type::PMT_A_S:{
		name += "A_S";
		break;
	}
	case Type::MPPC_tbS: {
		name += "signal_form_by_S";
		break;
	}
	case Type::MPPC_tbN: {
		name += "signal_form_by_Npe";
		break;
	}
	case Type::PMT_tbS: {
		name += "signal_form_by_S";
		break;
	}
	case Type::PMT_tbN: {
		name += "signal_form_by_Npeaks";
		break;
	}
	case Type::PMT_tbNpe: {
		name += "signal_form_by_Npe";
		break;
	}
	case Type::PMT_sum_N: {
		name += "sum_of_N_peaks";
		break;
	}
	case Type::PMT_trigger_bNpe: {
		name += "trigger_by_Npe";
		break;
	}
	case Type::PMT_trigger_bNpeaks: {
		name += "trigger_by_Npeaks";
		break;
	}
	case Type::PMT_trigger_bS: {
		name += "trigger_by_S";
		break;
	}
	case Type::PMT_trigger_fit:
	case Type::MPPC_trigger_fit: {
		name += "trigger_by_fit";
		break;
	}
	case Type::PMT_trigger_fit_chi2:
	case Type::MPPC_trigger_fit_chi2: {
		name += "trigger_by_fit_chi2";
		break;
	}
	case Type::MPPC_trigger_avg: {
		name += "trigger_by_avg_t";
		break;
	}
	case Type::PMT_T_sum: {
		name += "total_time";
		break;
	}
	case Type::PMT_shape_fit:
	case Type::MPPC_shape_fit: {
		name += "shape_fit";
		break;
	}
	case Type::MPPC_Npe_profile: {
		name += "Npe_profile";
		break;
	}
	case Type::MPPC_Npe_profile_x: {
		name += "Npe_profile_X";
		break;
	}
	case Type::MPPC_Npe_profile_y: {
		name += "Npe_profile_Y";
		break;
	}
	case Type::MPPC_coord_disp: {
		name += "coordinate_dispersion";
		break;
	}
	case Type::EventNumber: {
		name += "event_number";
		break;
	}
	}
	return name;
}
