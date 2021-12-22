#include "StateData.h"

bool StateData::IsForState(CanvasSetups *state) //I want this method to be virtual AND static
{
	return StateData().IsForState_virt(state);
}
bool StateData::IsForState(CanvasSetups *state, int channel, AStates::Type type) //I want this method to be virtual AND static
{
	return StateData().IsForState_virt(state, channel, type);
}

bool StateData::IsForState_virt(CanvasSetups *state) {
	return this->IsForState_virt(state, state->current_channel, state->current_type);
}

bool StateData::IsForState_virt(CanvasSetups *state, int channel, AStates::Type type) {
	switch (type) {
	case AStates::MPPC_Ss:
	case AStates::MPPC_As:
	case AStates::MPPC_t_S:
	case AStates::MPPC_A_S:
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbNpe_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_coord_y:
	case AStates::MPPC_Npe_sum:
	case AStates::MPPC_N_sum:
	case AStates::MPPC_S_sum:
	case AStates::MPPC_S2:
	case AStates::Correlation_x:
	case AStates::Correlation_y:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	case AStates::PMT_S2_S:
	case AStates::PMT_Npe_sum:
	case AStates::PMT_S_sum:
	case AStates::PMT_Ss:
	case AStates::PMT_As:
	case AStates::PMT_t_S:
	case AStates::PMT_A_S:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::PMT_tbNpe:
	case AStates::PMT_sum_N:
	case AStates::PMT_trigger_bNpe:
	case AStates::PMT_trigger_bNpeaks:
	case AStates::PMT_trigger_bS:
	case AStates::PMT_trigger_fit:
	case AStates::PMT_trigger_fit_chi2:
	case AStates::MPPC_trigger_fit:
	case AStates::MPPC_trigger_fit_chi2:
	case AStates::MPPC_trigger_avg:
	case AStates::PMT_T_sum:
	case AStates::MPPC_shape_fit:
	case AStates::PMT_shape_fit:
	case AStates::MPPC_Npe_profile:
	case AStates::MPPC_Npe_profile_x:
	case AStates::MPPC_Npe_profile_y:
	case AStates::MPPC_coord_disp:
		return true;
	default: {
		std::cout<<"Error: StateData::IsForState_virt is not implemented for type "<<state->type_name(type)<<std::endl;
	}
	}
	return false;
}

StateData::StateData()
{}
StateData::StateData(const CanvasSetups *for_state)
{}
StateData::~StateData()
{}
void StateData::SetDefaultSettings(const CanvasSetups *for_state)
{}
StateData * StateData::Clone () {
	return new StateData(*this);
}

//I want this method to be virtual AND static
StateData* StateData::GetData(CanvasSetups *setups, int channel, AStates::Type type) //returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
{
	return (TriggerData*) StateData().GetData_virt(setups, channel, type);
}
//I want this method to be virtual AND static
StateData* StateData::GetData(void)//returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
{
	return (TriggerData*) StateData().GetData_virt();
}
//I want this method to be virtual AND static
void StateData::DataChanged(void)
{
	StateData().DataChanged_virt();
}

void* StateData::GetData_virt(CanvasSetups *setups, int channel, AStates::Type type) //returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
{
	HistogramSetups *hist_setups = setups->get_hist_setups(setups->current_exp_index, channel, type);
	if (NULL == hist_setups) {
		std::cout<<"Error: StateData:GetData: NULL histogram setups"<<std::endl;
		return NULL;
	}
	if (this->IsForState_virt(setups, channel, type))
		return (void *) hist_setups->extra_data;
	return NULL;
}

void* StateData::GetData_virt(void) { //returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
	if (NULL == post_processor) {
		std::cout << "run Initialize() first" << std::endl;
		return NULL;
	}
	return this->GetData_virt(post_processor, post_processor->current_channel, post_processor->current_type);
}

void StateData::DataChanged_virt(void) {
	if (NULL == post_processor) {
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	if (IsForState(post_processor)) {
		post_processor->Invalidate(CanvasSetups::InvalidateLabel::invData);
		post_processor->update();
	}
}


bool TriggerData::IsForState(CanvasSetups *state)
{
	StateData* d = new TriggerData();
	bool ret = d->IsForState_virt(state);
	delete d;
	return ret;
}
bool TriggerData::IsForState(CanvasSetups *state, int channel, AStates::Type type)
{
	StateData* d = new TriggerData();
	bool ret = d->IsForState_virt(state, channel, type);
	delete d;
	return ret;
}

bool TriggerData::IsForState_virt(CanvasSetups *state, int channel, AStates::Type type) {
	if (StateData::IsForState_virt(state, channel, type)) {
		switch (type) {
		case AStates::MPPC_Ss:
		case AStates::MPPC_As:
		case AStates::MPPC_t_S:
		case AStates::MPPC_A_S:
		case AStates::MPPC_tbS:
		case AStates::MPPC_tbN:
		case AStates::MPPC_tbS_sum:
		case AStates::MPPC_tbNpe_sum:
		case AStates::MPPC_tbN_sum:
		case AStates::MPPC_coord:
		case AStates::MPPC_coord_x:
		case AStates::MPPC_coord_y:
		case AStates::MPPC_Npe_sum:
		case AStates::MPPC_N_sum:
		case AStates::MPPC_S_sum:
		case AStates::MPPC_S2:
		case AStates::PMT_S2_S:
		case AStates::PMT_Npe_sum:
		case AStates::PMT_S_sum:
		case AStates::PMT_Ss:
		case AStates::PMT_As:
		case AStates::PMT_t_S:
		case AStates::PMT_A_S:
		case AStates::PMT_tbS:
		case AStates::PMT_tbN:
		case AStates::PMT_tbNpe:
		case AStates::PMT_sum_N:
		case AStates::Correlation:
		case AStates::CorrelationAll:
		case AStates::PMT_trigger_fit:
		case AStates::PMT_trigger_fit_chi2:
		case AStates::MPPC_trigger_fit:
		case AStates::MPPC_trigger_fit_chi2:
		case AStates::MPPC_trigger_avg:
		case AStates::PMT_T_sum:
		case AStates::MPPC_shape_fit:
		case AStates::PMT_shape_fit:
		case AStates::MPPC_Npe_profile:
		case AStates::MPPC_Npe_profile_x:
		case AStates::MPPC_Npe_profile_y:
		case AStates::MPPC_coord_disp:
			return false;
		case AStates::Correlation_x: {
			return (state->_x_corr == AStates::PMT_trigger_bNpe) || (state->_x_corr == AStates::PMT_trigger_bNpeaks)
					|| (state->_x_corr == AStates::PMT_trigger_bS);
		}
		case AStates::Correlation_y:{
			return (state->_y_corr == AStates::PMT_trigger_bNpe) || (state->_y_corr == AStates::PMT_trigger_bNpeaks)
					|| (state->_y_corr == AStates::PMT_trigger_bS);
		}
		case AStates::PMT_trigger_bNpe:
		case AStates::PMT_trigger_bNpeaks:
		case AStates::PMT_trigger_bS:
			return true;
		default: {
			std::cout<<"Error: TriggerData::IsForState_virt is not implemented for type "<<state->type_name(type)<<std::endl;
		}
		}
	}
	return false;
}
TriggerData::TriggerData() : StateData(), trigger_window(-1.0)
{}
TriggerData::TriggerData(const CanvasSetups *for_state) : StateData(for_state), trigger_window(3.0)
{}
void TriggerData::SetDefaultSettings(const CanvasSetups *for_state) {
	trigger_window = 3.0;
}
StateData * TriggerData::Clone () {
	return new TriggerData(*this);
}

//I want this method to be virtual AND static
TriggerData* TriggerData::GetData(CanvasSetups *setups, int channel, AStates::Type type) //returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
{
	StateData* d = new TriggerData();
	TriggerData* ret =  (TriggerData*) d->GetData_virt(setups, channel, type);
	delete d;
	return ret;
}
//I want this method to be virtual AND static
TriggerData* TriggerData::GetData(void)//returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
{
	StateData* d = new TriggerData();
	TriggerData* ret = (TriggerData*) d->GetData_virt();
	delete d;
	return ret;
}
//I want this method to be virtual AND static
void TriggerData::DataChanged(void)
{
	StateData* d = new TriggerData();
	d->DataChanged_virt();
	delete d;
}

//CINT interface:
void TriggerData::SetShaping(double val) { //in microseconds
	TriggerData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerData:SetShaping: current type is not trigger type"<<std::endl;
		return;
	}
	bool update = (data->trigger_window != std::fabs(val));
	data->trigger_window = std::fabs(val);
	if (update)
		DataChanged();
}
double TriggerData::GetShaping(void) { //in microseconds
	TriggerData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerData:SetShaping: current type is not trigger type"<<std::endl;
		return -DBL_MAX;
	}
	return data->trigger_window;
}

bool TriggerFitData::IsForState(CanvasSetups *state)
{
	StateData* d = new TriggerFitData();
	bool ret = d->IsForState_virt(state);
	delete d;
	return ret;
}
bool TriggerFitData::IsForState(CanvasSetups *state, int channel, AStates::Type type)
{
	StateData* d = new TriggerFitData();
	bool ret = d->IsForState_virt(state, channel, type);
	delete d;
	return ret;
}

bool TriggerFitData::IsForState_virt(CanvasSetups *state, int channel, AStates::Type type) //override
{
	if (StateData::IsForState_virt(state, channel, type)) {
		switch (type) {
		case AStates::MPPC_Ss:
		case AStates::MPPC_As:
		case AStates::MPPC_t_S:
		case AStates::MPPC_A_S:
		case AStates::MPPC_tbS:
		case AStates::MPPC_tbN:
		case AStates::MPPC_tbS_sum:
		case AStates::MPPC_tbNpe_sum:
		case AStates::MPPC_tbN_sum:
		case AStates::MPPC_coord:
		case AStates::MPPC_coord_x:
		case AStates::MPPC_coord_y:
		case AStates::MPPC_Npe_sum:
		case AStates::MPPC_N_sum:
		case AStates::MPPC_S_sum:
		case AStates::MPPC_S2:
		case AStates::PMT_S2_S:
		case AStates::PMT_Npe_sum:
		case AStates::PMT_S_sum:
		case AStates::PMT_Ss:
		case AStates::PMT_As:
		case AStates::PMT_t_S:
		case AStates::PMT_A_S:
		case AStates::PMT_tbS:
		case AStates::PMT_tbN:
		case AStates::PMT_tbNpe:
		case AStates::PMT_sum_N:
		case AStates::Correlation:
		case AStates::CorrelationAll:
		case AStates::PMT_trigger_bNpe:
		case AStates::PMT_trigger_bNpeaks:
		case AStates::PMT_trigger_bS:
		case AStates::MPPC_trigger_avg:
		case AStates::PMT_T_sum:
		case AStates::MPPC_shape_fit:
		case AStates::PMT_shape_fit:
		case AStates::MPPC_Npe_profile:
		case AStates::MPPC_Npe_profile_x:
		case AStates::MPPC_Npe_profile_y:
		case AStates::MPPC_coord_disp:
			return false;
		case AStates::Correlation_x: {
			return (state->_x_corr == AStates::PMT_trigger_fit || state->_x_corr == AStates::PMT_trigger_fit_chi2
					|| state->_x_corr == AStates::MPPC_trigger_fit || state->_x_corr == AStates::MPPC_trigger_fit_chi2);
		}
		case AStates::Correlation_y:{
			return (state->_y_corr == AStates::PMT_trigger_fit || state->_y_corr == AStates::PMT_trigger_fit_chi2
					|| state->_y_corr == AStates::MPPC_trigger_fit || state->_y_corr == AStates::MPPC_trigger_fit_chi2);
		}
		case AStates::PMT_trigger_fit:
		case AStates::PMT_trigger_fit_chi2:
		case AStates::MPPC_trigger_fit:
		case AStates::MPPC_trigger_fit_chi2:
			return true;
		default: {
			std::cout<<"Error: TriggerFitData::IsForState_virt is not implemented for type "<<state->type_name(type)<<std::endl;
		}
		}
	}
	return false;
}

TriggerFitData::TriggerFitData() :
		StateData(), exp_pulse_shape(), trigger_type(tbNpe), t0_precision(0.2), scan_dt(1.6)
{
	exp_pulse_shape.set_out_value(0.0);
};

TriggerFitData::TriggerFitData(const CanvasSetups *for_state) :
		StateData(for_state), exp_pulse_shape(), trigger_type(tbNpe), t0_precision(0.2), scan_dt(1.6)
{
	exp_pulse_shape.set_out_value(0.0);
};

void TriggerFitData::SetDefaultSettings(const CanvasSetups *for_state) //override
{
	exp_pulse_shape = DataVector();
	exp_pulse_shape.set_out_value(0.0);
	trigger_type = tbNpe;
	t0_precision = 0.2;
	scan_dt = 1.6;
}

StateData * TriggerFitData::Clone () //override
{
	return new TriggerFitData(*this);
}

bool TriggerFitData::IsValid() const
{
	return exp_pulse_shape.isValid() && t0_precision > 0 && scan_dt > 0;
}

//I want this method to be virtual AND static
TriggerFitData* TriggerFitData::GetData(CanvasSetups *setups, int channel, AStates::Type type) //returns NULL if current setups do not contain this data class.
{
	StateData* d = new TriggerFitData();
	TriggerFitData* ret =  (TriggerFitData*) d->GetData_virt(setups, channel, type);
	delete d;
	return ret;
}

TriggerFitData* TriggerFitData::GetData(void)//returns NULL if current setups do not contain this data class.
{
	StateData* d = new TriggerFitData();
	TriggerFitData* ret =  (TriggerFitData*) d->GetData_virt();
	delete d;
	return ret;
}
//I want this method to be virtual AND static
void TriggerFitData::DataChanged(void)
{
	StateData* d = new TriggerFitData();
	d->DataChanged_virt();
	delete d;
}

//CINT interface:
void TriggerFitData::SetPulseShape(DataVector &pulse_shape)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:SetPulseShape: current type is not trigger fit type"<<std::endl;
		return;
	}
	data->exp_pulse_shape = pulse_shape;
	DataChanged();
}

void TriggerFitData::SetPulseShape(std::string fname)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:SetPulseShape: current type is not trigger fit type"<<std::endl;
		return;
	}
	std::ifstream str;
	str.open(fname);
	if (!str.is_open()) {
		std::cout<<"Error: TriggerFitData:SetPulseShape: could not open file \""<<fname<<"\""<<std::endl;
		return;
	}
	data->exp_pulse_shape.read(str, false);
	data->exp_pulse_shape.set_out_value(0.0); //amplitude is 0 outside defined x domain
	data->exp_pulse_shape.setNused(2); //linear extrapolation
	data->exp_pulse_shape.setOrder(1);
	data->exp_pulse_shape.renormalize();
	DataChanged();
}

DataVector TriggerFitData::GetPulseShape(void)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:GetPulseShape: current type is not trigger fit type"<<std::endl;
		return DataVector();
	}
	return data->exp_pulse_shape;
}

void TriggerFitData::SetTriggerType(TriggerType type)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:SetTriggerType: current type is not trigger fit type"<<std::endl;
		return;
	}
	bool update = data->trigger_type!=type;
	data->trigger_type = type;
	if (update)
		DataChanged();
}
TriggerFitData::TriggerType TriggerFitData::GetTriggerType(void)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:GetTriggerType: current type is not trigger fit type"<<std::endl;
		return tbNpe;
	}
	return data->trigger_type;
}

void TriggerFitData::SetTriggerPrecision(double precision)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:SetTriggerPrecision: current type is not trigger fit type"<<std::endl;
		return;
	}
	bool update = data->t0_precision!=std::fabs(precision);
	data->t0_precision = std::fabs(precision);
	if (update)
		DataChanged();
}

double TriggerFitData::GetTriggerPrecision(void)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:GetTriggerPrecision: current type is not trigger fit type"<<std::endl;
		return -DBL_MAX;
	}
	return data->t0_precision;
}

void TriggerFitData::SetTriggerFirstScant(double dt)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:SetTriggerFirstScant: current type is not trigger fit type"<<std::endl;
		return ;
	}
	bool update = data->scan_dt!=std::fabs(dt);
	data->scan_dt = std::fabs(dt);
	if (update)
		DataChanged();
}

double TriggerFitData::GetTriggerFirstScant(void)
{
	TriggerFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerFitData:GetTriggerFirstScant: current type is not trigger fit type"<<std::endl;
		return -DBL_MAX;
	}
	return data->scan_dt;
}


bool TriggerAvgTData::IsForState(CanvasSetups *state)
{
	StateData* d = new TriggerAvgTData();
	bool ret = d->IsForState_virt(state);
	delete d;
	return ret;
}
bool TriggerAvgTData::IsForState(CanvasSetups *state, int channel, AStates::Type type)
{
	StateData* d = new TriggerAvgTData();
	bool ret = d->IsForState_virt(state, channel, type);
	delete d;
	return ret;
}

bool TriggerAvgTData::IsForState_virt(CanvasSetups *state, int channel, AStates::Type type) //override
{
	if (StateData::IsForState_virt(state, channel, type)) {
		switch (type) {
		case AStates::MPPC_Ss:
		case AStates::MPPC_As:
		case AStates::MPPC_t_S:
		case AStates::MPPC_A_S:
		case AStates::MPPC_tbS:
		case AStates::MPPC_tbN:
		case AStates::MPPC_tbS_sum:
		case AStates::MPPC_tbNpe_sum:
		case AStates::MPPC_tbN_sum:
		case AStates::MPPC_coord:
		case AStates::MPPC_coord_x:
		case AStates::MPPC_coord_y:
		case AStates::MPPC_Npe_sum:
		case AStates::MPPC_N_sum:
		case AStates::MPPC_S_sum:
		case AStates::MPPC_S2:
		case AStates::PMT_S2_S:
		case AStates::PMT_Npe_sum:
		case AStates::PMT_S_sum:
		case AStates::PMT_Ss:
		case AStates::PMT_As:
		case AStates::PMT_t_S:
		case AStates::PMT_A_S:
		case AStates::PMT_tbS:
		case AStates::PMT_tbN:
		case AStates::PMT_tbNpe:
		case AStates::PMT_sum_N:
		case AStates::Correlation:
		case AStates::CorrelationAll:
		case AStates::PMT_trigger_bNpe:
		case AStates::PMT_trigger_bNpeaks:
		case AStates::PMT_trigger_bS:
		case AStates::PMT_trigger_fit:
		case AStates::PMT_trigger_fit_chi2:
		case AStates::MPPC_trigger_fit:
		case AStates::MPPC_trigger_fit_chi2:
		case AStates::PMT_T_sum:
		case AStates::MPPC_shape_fit:
		case AStates::PMT_shape_fit:
		case AStates::MPPC_Npe_profile:
		case AStates::MPPC_Npe_profile_x:
		case AStates::MPPC_Npe_profile_y:
		case AStates::MPPC_coord_disp:
			return false;
		case AStates::Correlation_x: {
			return (state->_x_corr == AStates::MPPC_trigger_avg);
		}
		case AStates::Correlation_y: {
			return (state->_y_corr == AStates::MPPC_trigger_avg);
		}
		case AStates::MPPC_trigger_avg:
			return true;
		default: {
			std::cout<<"Error: TriggerAvgTData::IsForState_virt is not implemented for type "<<state->type_name(type)<<std::endl;
		}
		}
	}
	return false;
}

TriggerAvgTData::TriggerAvgTData() :
		StateData(), trigger_type(tbNpe)
{};

TriggerAvgTData::TriggerAvgTData(const CanvasSetups *for_state) :
		StateData(for_state), trigger_type(tbNpe)
{};

void TriggerAvgTData::SetDefaultSettings(const CanvasSetups *for_state) //override
{
	trigger_type = tbNpe;
}

StateData * TriggerAvgTData::Clone () //override
{
	return new TriggerAvgTData(*this);
}

bool TriggerAvgTData::IsValid() const
{
	return true;
}

//I want this method to be virtual AND static
TriggerAvgTData* TriggerAvgTData::GetData(CanvasSetups *setups, int channel, AStates::Type type) //returns NULL if current setups do not contain this data class.
{
	StateData* d = new TriggerAvgTData();
	TriggerAvgTData* ret =  (TriggerAvgTData*) d->GetData_virt(setups, channel, type);
	delete d;
	return ret;
}

TriggerAvgTData* TriggerAvgTData::GetData(void)//returns NULL if current setups do not contain this data class.
{
	StateData* d = new TriggerAvgTData();
	TriggerAvgTData* ret =  (TriggerAvgTData*) d->GetData_virt();
	delete d;
	return ret;
}
//I want this method to be virtual AND static
void TriggerAvgTData::DataChanged(void)
{
	StateData* d = new TriggerAvgTData();
	d->DataChanged_virt();
	delete d;
}

//CINT interface:
void TriggerAvgTData::SetTriggerType(TriggerType type)
{
	TriggerAvgTData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerAvgTData:SetTriggerType: current type is not trigger fit type"<<std::endl;
		return;
	}
	bool update = data->trigger_type!=type;
	data->trigger_type = type;
	if (update)
		DataChanged();
}
TriggerAvgTData::TriggerType TriggerAvgTData::GetTriggerType(void)
{
	TriggerAvgTData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: TriggerAvgTData:GetTriggerType: current type is not trigger fit type"<<std::endl;
		return tbNpe;
	}
	return data->trigger_type;
}

//*********************************************************************************************************

bool ShapeFitData::IsForState(CanvasSetups *state)
{
	StateData* d = new ShapeFitData();
	bool ret = d->IsForState_virt(state);
	delete d;
	return ret;
}
bool ShapeFitData::IsForState(CanvasSetups *state, int channel, AStates::Type type)
{
	StateData* d = new ShapeFitData();
	bool ret = d->IsForState_virt(state, channel, type);
	delete d;
	return ret;
}

bool ShapeFitData::IsForState_virt(CanvasSetups *state, int channel, AStates::Type type)
{
	if (StateData::IsForState_virt(state, channel, type)) {
		switch (type) {
		case AStates::MPPC_Ss:
		case AStates::MPPC_As:
		case AStates::MPPC_t_S:
		case AStates::MPPC_A_S:
		case AStates::MPPC_tbS:
		case AStates::MPPC_tbN:
		case AStates::MPPC_tbS_sum:
		case AStates::MPPC_tbNpe_sum:
		case AStates::MPPC_tbN_sum:
		case AStates::MPPC_coord:
		case AStates::MPPC_coord_x:
		case AStates::MPPC_coord_y:
		case AStates::MPPC_Npe_sum:
		case AStates::MPPC_N_sum:
		case AStates::MPPC_S_sum:
		case AStates::MPPC_S2:
		case AStates::PMT_S2_S:
		case AStates::PMT_Npe_sum:
		case AStates::PMT_S_sum:
		case AStates::PMT_Ss:
		case AStates::PMT_As:
		case AStates::PMT_t_S:
		case AStates::PMT_A_S:
		case AStates::PMT_tbS:
		case AStates::PMT_tbN:
		case AStates::PMT_tbNpe:
		case AStates::PMT_sum_N:
		case AStates::Correlation:
		case AStates::CorrelationAll:
		case AStates::PMT_trigger_bNpe:
		case AStates::PMT_trigger_bNpeaks:
		case AStates::PMT_trigger_bS:
		case AStates::PMT_trigger_fit:
		case AStates::PMT_trigger_fit_chi2:
		case AStates::MPPC_trigger_fit:
		case AStates::MPPC_trigger_fit_chi2:
		case AStates::PMT_T_sum:
		case AStates::MPPC_trigger_avg:
		case AStates::MPPC_Npe_profile:
		case AStates::MPPC_Npe_profile_x:
		case AStates::MPPC_Npe_profile_y:
		case AStates::MPPC_coord_disp:
			return false;
		case AStates::Correlation_x: {
			return (state->_x_corr == AStates::MPPC_shape_fit || state->_x_corr == AStates::PMT_shape_fit);
		}
		case AStates::Correlation_y: {
			return (state->_y_corr == AStates::MPPC_shape_fit || state->_y_corr == AStates::PMT_shape_fit);
		}
		case AStates::MPPC_shape_fit:
		case AStates::PMT_shape_fit: {
			return true;
		}
		default: {
			std::cout<<"Error: ShapeFitData::IsForState_virt is not implemented for type "<<state->type_name(type)<<std::endl;
		}
		}
	}
	return false;
}

ShapeFitData::ShapeFitData():
		StateData(), peak_type(ptNpe), n_parameters(0), fit_function(), parameter_to_plot(0)
{}

ShapeFitData::ShapeFitData(const CanvasSetups *for_state):
		StateData(for_state), peak_type(ptNpe), n_parameters(0), fit_function(), parameter_to_plot(0)
{}

void ShapeFitData::SetDefaultSettings(const CanvasSetups *for_state)
{
	peak_type = ptNpe;
	n_parameters = 0;
	fit_function.SetFunction(NULL);
	par_bounds.clear();
	par_precisions.clear();
}

StateData* ShapeFitData::Clone ()
{
	return new ShapeFitData(*this);
}

bool ShapeFitData::IsValid() const
{
	return (n_parameters > 0 && par_bounds.size()==n_parameters && par_precisions.size()==n_parameters && parameter_to_plot <= n_parameters && fit_function.isValid());
}

//I want this method to be virtual AND static
ShapeFitData* ShapeFitData::GetData(CanvasSetups *setups, int channel, AStates::Type type) //returns NULL if current setups do not contain this data class.
{
	StateData* d = new ShapeFitData();
	ShapeFitData* ret =  (ShapeFitData*) d->GetData_virt(setups, channel, type);
	delete d;
	return ret;
}
ShapeFitData* ShapeFitData::GetData(void) //returns NULL if current setups do not contain this data class.
{
	StateData* d = new ShapeFitData();
	ShapeFitData* ret =  (ShapeFitData*) d->GetData_virt();
	delete d;
	return ret;
}

//I want this method to be virtual AND static
void ShapeFitData::DataChanged(void)
{
	StateData* d = new ShapeFitData();
	d->DataChanged_virt();
	delete d;
}

//Same as StateData, but does not force update()
void ShapeFitData::DataChanged_virt(void)
{
	if (NULL == post_processor) {
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	if (ShapeFitData::IsForState(post_processor)) {
		post_processor->Invalidate(CanvasSetups::InvalidateLabel::invData);
	}
}

void ShapeFitData::PeaksToXY(std::deque<peak_processed> &peaks, std::vector<double> &xs, std::vector<double> &ys) const
{
	xs.clear();
	ys.clear();
	xs.reserve(peaks.size());
	ys.reserve(peaks.size());
	for (auto pk = peaks.begin(), pk_end_ = peaks.end(); pk!=pk_end_; ++pk) {
		if (ptNpe == peak_type) {
			if (pk->Npe <= 0)
				continue;
			xs.push_back(pk->t);
			ys.push_back(pk->Npe);
			continue;
		}
		if (ptS == peak_type) {
			if (pk->S <= 0)
				continue;
			xs.push_back(pk->t);
			ys.push_back(pk->S);
			continue;
		}
		xs.push_back(pk->t);
		ys.push_back(1.0);
	}
}
//returns n parameters + likelihood
std::vector<double> ShapeFitData::Fit(std::deque<peak_processed> &peaks) const
{
	if (!IsValid()) {
		return std::vector<double>(n_parameters +1, DBL_MAX);
	}
	std::vector<double> xs, ys;
	PeaksToXY(peaks, xs, ys);
	ROOT::Fit::UnBinData data_to_fit(xs.size(), &xs[0], &ys[0]);
	FitWrapper temp = fit_function;
	ROOT::Math::WrappedParamFunction<FitWrapper*> root_f(&temp, 1, n_parameters, 0);
	ROOT::Fit::LogLikelihoodFunction lkhood_fnc(data_to_fit, root_f);
	ROOT::Fit::Fitter fitter;
	fitter.Config().ParamsSettings() = std::vector<ROOT::Fit::ParameterSettings>(n_parameters);
	for (int i = 0, i_end_ = n_parameters; i!=i_end_; ++i) {
		double initial_value = 0;
		initial_value += 0.5*(IsInf(par_bounds[i].first) ? (par_bounds[i].first > 0 ? 2 : -2) : par_bounds[i].first);
		initial_value += 0.5*(IsInf(par_bounds[i].second) ? (par_bounds[i].second > 0 ? 2 : -2) : par_bounds[i].second);
		fitter.Config().ParSettings(i).SetValue(initial_value);
		fitter.Config().ParSettings(i).SetLimits(par_bounds[i].first, par_bounds[i].second);
		double step_size = 0.05*((IsInf(par_bounds[i].second) ? 0 : par_bounds[i].second) - (IsInf(par_bounds[i].first) ? 0 : par_bounds[i].first));
		if (0 == step_size)
			step_size = 1;
		if (par_precisions[i] > 0)
			step_size = std::min(step_size, 10 * par_precisions[i]);
		fitter.Config().ParSettings(i).SetStepSize(step_size);
		fitter.Config().ParSettings(i).SetName("[" + int_to_str(i) + "]"); //Must set a name for Minuit2 to work
	}
	fitter.Config().SetMinimizer("Minuit2", "Minimize");
	fitter.Config().MinimizerOptions().SetPrintLevel(0); //default - 0
	fitter.Config().MinimizerOptions().SetTolerance(1e-7);
	fitter.Config().SetMinosErrors(false);
	fitter.Config().SetNormErrors(false);
	fitter.Config().SetParabErrors(false);
	fitter.FitFCN(n_parameters, lkhood_fnc, 0, xs.size(), false);
	ROOT::Fit::FitResult fit_result = fitter.Result();
	const double *out_pars = fit_result.GetParams();
	std::vector<double> result(n_parameters + 1, 0);
	for (int i = 0, i_end_ = n_parameters; i!=i_end_; ++i)
		result[i] = out_pars[i];
	result[n_parameters] = lkhood_fnc(out_pars);
	return result;
}

//CINT interface:
void ShapeFitData::SetPeakType(PeakType type)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetPeakType: current type is not shape fit type"<<std::endl;
		return;
	}
	bool update = data->peak_type!=type;
	data->peak_type = type;
	if (update)
		DataChanged();
}

ShapeFitData::PeakType ShapeFitData::GetPeakType(void)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::GetPeakType: current type is not shape fit type"<<std::endl;
		return PeakType::ptNpe;
	}
	return data->peak_type;
}

void ShapeFitData::SetNPars(int n)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetNPars: current type is not shape fit type"<<std::endl;
		return;
	}
	bool update = false;
	n = std::max(0, n);
	if (data->n_parameters < n) {
		update = true;
		data->par_bounds.insert(data->par_bounds.end(), n - data->n_parameters, std::pair<double, double>(-DBL_MAX, DBL_MAX));
		data->par_precisions.insert(data->par_precisions.end(), n - data->n_parameters, -1);
		data->par_names.insert(data->par_names.end(), n - data->n_parameters, "");
		data->n_parameters = n;
	}
	if (data->n_parameters > n) {
		update = true;
		data->par_bounds.erase(data->par_bounds.begin() + n, data->par_bounds.end());
		data->par_precisions.erase(data->par_precisions.begin() + n, data->par_precisions.end());
		data->par_names.erase(data->par_names.begin() + n, data->par_names.end());
		data->n_parameters = n;
	}
	if (update)
		DataChanged();
}

int ShapeFitData::GetNPars(void)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::GetNPars: current type is not shape fit type"<<std::endl;
		return 0;
	}
	return data->n_parameters;
}

void ShapeFitData::SetPrecision(int n, double precision)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetPrecision: current type is not shape fit type"<<std::endl;
		return;
	}
	if (n >= data->n_parameters || n < 0) {
		std::cout<<"Error: ShapeFitData::SetPrecision: index is out of bounds"<<std::endl;
		return;
	}
	bool update = data->par_precisions[n] != precision;
	data->par_precisions[n] = precision;
	if (update)
		DataChanged();
}

double ShapeFitData::GetPrecision(int n)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::GetPrecision: current type is not shape fit type"<<std::endl;
		return -1;
	}
	if (n >= data->n_parameters || n < 0) {
		std::cout<<"Error: ShapeFitData::GetPrecision: index is out of bounds"<<std::endl;
		return -1;
	}
	return data->par_precisions[n];
}

void ShapeFitData::SetPrecisions(std::vector<double> precisions)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetPrecisions: current type is not shape fit type"<<std::endl;
		return;
	}
	if (precisions.size() != data->n_parameters) {
		std::cout<<"Warning: ShapeFitData::SetPrecisions: input array does not have correct size"<<std::endl;
	}
	bool update = false;
	for (int i = 0, i_end_= std::min(precisions.size(), (std::size_t)data->n_parameters); i!=i_end_; ++i) {
		if (!update && data->par_precisions[i] != precisions[i])
			update = true;
		data->par_precisions[i] = precisions[i];
	}
	if (update)
		DataChanged();
}

std::vector<double> ShapeFitData::GetPrecisions(void)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::GetPrecisions: current type is not shape fit type"<<std::endl;
		return std::vector<double>();
	}
	return data->par_precisions;
}

void ShapeFitData::SetBound(int n, double _min, double _max)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetBound: current type is not shape fit type"<<std::endl;
		return;
	}
	if (n >= data->n_parameters || n < 0) {
		std::cout<<"Error: ShapeFitData::SetBound: index is out of bounds"<<std::endl;
		return;
	}
	double min = std::min(_min, _max);
	double max = std::max(_min, _max);
	bool update = (data->par_bounds[n].first != min || data->par_bounds[n].second != max);
	data->par_bounds[n].first = min;
	data->par_bounds[n].second = max;
	if (update)
		DataChanged();
}

void ShapeFitData::SetBound(int n, std::pair<double, double> bound)
{
	SetBound(n, bound.first, bound.second);
}

void ShapeFitData::SetBound(int n, double fixed)
{
	SetBound(n, fixed, fixed);
}

std::pair<double, double> ShapeFitData::GetBound(int n)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetBound: current type is not shape fit type"<<std::endl;
		return std::pair<double, double>(DBL_MAX, DBL_MAX);
	}
	if (n >= data->n_parameters || n < 0) {
		std::cout<<"Error: ShapeFitData::SetBound: index is out of bounds"<<std::endl;
		return std::pair<double, double>(DBL_MAX, DBL_MAX);
	}
	return data->par_bounds[n];
}

void ShapeFitData::SetBounds(std::vector<double> mins, std::vector<double> maxs)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetBounds: current type is not shape fit type"<<std::endl;
		return;
	}
	if (mins.size() != data->n_parameters || maxs.size() != data->n_parameters) {
		std::cout<<"Warning: ShapeFitData::SetBounds: input array does not have correct size"<<std::endl;
	}
	bool update = false;
	for (int i = 0, i_end_= std::min(std::min(mins.size(), maxs.size()), (std::size_t)data->n_parameters); i!=i_end_; ++i) {
		double min = std::min(mins[i], maxs[i]);
		double max = std::max(mins[i], maxs[i]);
		if (!update && (data->par_bounds[i].first != min || data->par_bounds[i].second != max))
			update = true;
		data->par_bounds[i].first = min;
		data->par_bounds[i].second = max;
	}
	if (update)
		DataChanged();
}

void ShapeFitData::SetBounds(std::vector<std::pair<double, double>> bounds)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetBounds: current type is not shape fit type"<<std::endl;
		return;
	}
	if (bounds.size() != data->n_parameters) {
		std::cout<<"Warning: ShapeFitData::SetBounds: input array does not have correct size"<<std::endl;
	}
	bool update = false;
	for (int i = 0, i_end_= std::min(bounds.size(), (std::size_t)data->n_parameters); i!=i_end_; ++i) {
		double min = std::min(bounds[i].first, bounds[i].second);
		double max = std::max(bounds[i].first, bounds[i].second);
		if (!update && (data->par_bounds[i].first != min || data->par_bounds[i].second != max))
			update = true;
		data->par_bounds[i].first = min;
		data->par_bounds[i].second = max;
	}
	if (update)
		DataChanged();
}

std::vector<std::pair<double, double>> ShapeFitData::GetBounds(void)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::GetBounds: current type is not shape fit type"<<std::endl;
		return std::vector<std::pair<double, double>>();
	}
	return data->par_bounds;
}

void ShapeFitData::SetPlotParameter(int index)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetPlotParameter: current type is not shape fit type"<<std::endl;
		return;
	}
	if (index > data->n_parameters || index < 0) {
		std::cout<<"Error: ShapeFitData::SetPlotParameter: index is out of bounds"<<std::endl;
		return;
	}
	bool update = data->parameter_to_plot != index;
	data->parameter_to_plot = index;
	if (update)
		DataChanged();
}

int ShapeFitData::GetPlotParameter(void)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::GetPlotParameter: current type is not shape fit type"<<std::endl;
		return -1;
	}
	return data->parameter_to_plot;
}

void ShapeFitData::SetParameterName(int n, std::string name)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetParameterName: current type is not shape fit type"<<std::endl;
		return;
	}
	if (n >= data->n_parameters || n < 0) {
		std::cout<<"Error: ShapeFitData::SetParameterName: index is out of bounds"<<std::endl;
		return;
	}
	data->par_names[n] = name;
}

std::string ShapeFitData::GetParameterName(int n)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::GetParameterName: current type is not shape fit type"<<std::endl;
		return "";
	}
	if (n == data->n_parameters && n != 0) {
		return "LogLH";
	}
	if (n >= data->n_parameters || n < 0) {
		std::cout<<"Error: ShapeFitData::GetParameterName: index is out of bounds"<<std::endl;
		return "";
	}
	return data->par_names[n];
}

void ShapeFitData::SetParameterNames(std::vector<std::string> names)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetParameterNames: current type is not shape fit type"<<std::endl;
		return;
	}
	if (names.size() != data->n_parameters) {
		std::cout<<"Warning: ShapeFitData::SetParameterNames: input array does not have correct size"<<std::endl;
	}
	data->par_names = names;
}

std::vector<std::string> ShapeFitData::GetParameterNames(void)
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetParameterNames: current type is not shape fit type"<<std::endl;
		return std::vector<std::string>();
	}
	return data->par_names;
}

FIT_F ShapeFitData::SetFitFunction(FIT_F f) //ignore run_n and stat_data.
{
	ShapeFitData* data = GetData();
	if (NULL == data) {
		std::cout<<"Error: ShapeFitData::SetPrecision: current type is not shape fit type"<<std::endl;
		return f;
	}
	data->fit_function.SetFunction(f);
	DataChanged();
	return f;
}


