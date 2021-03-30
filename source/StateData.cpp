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
		return true;
	default: {
		std::cout<<"Error: StateData::IsForState_virt is not implemented for type "<<state->type_name(type)<<std::endl;
	}
	}
	return false;
}

StateData::StateData() : static_instance(true)
{}
StateData::StateData(const CanvasSetups *for_state) : static_instance(false)
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
	post_processor->Invalidate(CanvasSetups::InvalidateLabel::invData);
	post_processor->update();
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

