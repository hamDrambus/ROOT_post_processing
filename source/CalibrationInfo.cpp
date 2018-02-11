#include "CalibrationInfo.h"


CalibrationInfo::CalibrationInfo(AnalysisStates* data):
state_info(data)
{
	s1pe_PMT3 = 1;//TODO: ParameterPile (need to ask the real values from Vlad)
	s1pe_PMT1 = 1;
	for (auto i = state_info->experiments.begin(); i != state_info->experiments.end(); ++i) {
		avr_S1pe.push_back(STD_CONT<double>());
		avr_S2pe.push_back(STD_CONT<double>());
		method.push_back(STD_CONT<S1pe_method>());
		for (auto ff = state_info->MPPC_channels.begin(); ff != state_info->MPPC_channels.end(); ++ff){
			avr_S1pe.back().push_back(-1);
			avr_S2pe.back().push_back(-1);
			method.back().push_back(Ignore);
		}
	}
	for (auto ff = state_info->MPPC_channels.begin(); ff != state_info->MPPC_channels.end(); ++ff){
		s1pe.push_back(-1);
		forced_s1pe.push_back(false);
		N_used_in_calibration.push_back(ParameterPile::calibaration_points);
	}
}

double CalibrationInfo::calculateS1pe(int channel)
{
	int ch_ind = state_info->mppc_channel_to_index(channel);
	if (ch_ind < 0)
		return -1;
	if (forced_s1pe[ch_ind])
		return s1pe[ch_ind];
	double val = 0;
	int n_used = 0;
	for (auto e = avr_S1pe.begin(), _end_ = avr_S1pe.end(); e != _end_; ++e) {
		std::size_t exp_ind = e - avr_S1pe.begin();
		if (Using1pe == method[exp_ind][ch_ind] || Using1pe2pe == method[exp_ind][ch_ind] 
			|| UsingMean == method[exp_ind][ch_ind]){
			if (avr_S1pe[exp_ind][ch_ind] > 0){
				val += avr_S1pe[exp_ind][ch_ind];
				n_used++;
			}
		}
		if (Using2pe == method[exp_ind][ch_ind] || Using1pe2pe == method[exp_ind][ch_ind]){
			if (avr_S2pe[exp_ind][ch_ind] > 0){
				val += 0.5*avr_S2pe[exp_ind][ch_ind];
				n_used++;
			}
		}
	}
	val = n_used ? val / n_used : -1;
	s1pe[ch_ind] = val;
	return val;
}

double CalibrationInfo::getS1pe(int channel)
{
	int ch_ind = state_info->mppc_channel_to_index(channel);
	return (ch_ind < 0) ? -1 : s1pe[ch_ind];
}
void CalibrationInfo::setS1pe(int channel, double val)
{
	int ch_ind = state_info->mppc_channel_to_index(channel);
	if (ch_ind < 0)
		return;
	s1pe[ch_ind] = val;
	forced_s1pe[ch_ind] = true;
}

void CalibrationInfo::unsetS1pe(int ch)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	forced_s1pe[ch_ind] = false;
	calculateS1pe(ch);
}

double CalibrationInfo::getPMT_S1pe(int ch)
{
	return (0 == ch) ? s1pe_PMT3 : (1 == ch ? s1pe_PMT1 : -1);
}
void CalibrationInfo::setPMT_S1pe(int ch, double val)
{
	if (0 == ch)
		s1pe_PMT3 = val;
	if (1 == ch)
		s1pe_PMT1 = val;
}

void CalibrationInfo::set_N_calib(int ch, int from, int to)
{
	set_N_calib(ch, std::pair<int, int>(from, to));
}

void CalibrationInfo::set_N_calib(int channel, std::pair<int, int> val)
{
	int ch_ind = state_info->mppc_channel_to_index(channel);
	if (ch_ind < 0)
		return;
	N_used_in_calibration[ch_ind] = val;
}

std::pair<int, int> CalibrationInfo::get_N_calib(int ch)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return std::pair<int,int>(-1,-1);
	return N_used_in_calibration[ch_ind];
}

CalibrationInfo::S1pe_method CalibrationInfo::get_method(int exp_ch, int ch)//called from PostProcessor
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return Ignore;
	return method[exp_ch][ch_ind];
}

void CalibrationInfo::set_method(int exp_ch, int ch, S1pe_method meth)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	method[exp_ch][ch_ind] = meth;
}

//setters/getters for avr_S1pe and avr_S2pe which are used for s1pe calcualtions
void CalibrationInfo::set_S1pe(int ch, int exp_index, double val)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	avr_S1pe[exp_index][ch] = val;
	calculateS1pe(ch);
}
double CalibrationInfo::get_S1pe(int ch, int exp_index)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return -1;
	return avr_S1pe[exp_index][ch];
}
void CalibrationInfo::set_S2pe(int ch, int exp_index, double val)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	avr_S2pe[exp_index][ch] = val;
	calculateS1pe(ch);
}
double CalibrationInfo::get_S2pe(int ch, int exp_index)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return -1;
	return avr_S2pe[exp_index][ch];
}

//TODO: set double I calibration from pre set parameters (e.g. from file or ParameterPile)
//S2_S and double_I must be the full and same size [experiment]x[mppc_channel]
//sets S2_S and double_I to N_pe
STD_CONT<STD_CONT<std::pair<bool, bool>>> &CalibrationInfo::recalibrate(STD_CONT<STD_CONT<double>> &S2_S,
	STD_CONT<STD_CONT<double>> &double_I, DVECTOR& Fields)
{
	//experiment->channel->{direct, double_I}
	STD_CONT<STD_CONT<std::pair<bool, bool>>> recalibration_sucess;
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		recalibration_sucess.push_back(STD_CONT<std::pair<bool, bool> >());
		for (int ch_ind = 0; ch_ind < S2_S.back().size(); ++ch_ind)
			recalibration_sucess.back().push_back(std::pair<bool, bool>(false, false));
	}

	for (int ch_ind = 0; ch_ind < S2_S.back().size(); ++ch_ind){
		if (s1pe[ch_ind] <= 0)
			continue;
		//calculate N_pe_direct
		for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			S2_S[exp_ind][ch_ind] /= s1pe[ch_ind];
			recalibration_sucess[exp_ind][ch_ind].first = true;
		}
		TVectorD direct_line;
		TVectorD double_I_line;
		DVECTOR xs, direct_ys;
		DVECTOR double_I_ys;
		for (int pt = N_used_in_calibration[ch_ind].first; (pt <= N_used_in_calibration[ch_ind].second) && (pt < state_info->experiments.size()); ++pt){
			if (pt < 0)
				continue;
			direct_ys.push_back(S2_S[pt][ch_ind]);
			xs.push_back(Fields[pt]);
			double_I_ys.push_back(double_I[pt][ch_ind]);
		}
		if (direct_ys.size() == 0)
			continue;
		if (direct_ys.size() == 1){
			double coef = double_I_ys.back() / direct_ys.back();
			for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
				double_I[exp_ind][ch_ind] = S2_S[exp_ind][ch_ind] * coef;
				recalibration_sucess[exp_ind][ch_ind].second = true;
			}
			continue;
		}
		PolynomialFit line_direct(1);
		PolynomialFit line_double_I(1);
		line_direct(xs, direct_ys, direct_line);
		line_double_I(xs, double_I_ys, double_I_line);
		double x_intersect = -(direct_line[0] - double_I_line[0]) / (direct_line[1] - double_I_line[1]);
		double y_intersect = x_intersect*direct_line[1] + direct_line[0];
		double slope_ratio = direct_line[1] / double_I_line[1];
		for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			double_I[exp_ind][ch_ind] = slope_ratio*(double_I[exp_ind][ch_ind] - y_intersect) + y_intersect;
			recalibration_sucess[exp_ind][ch_ind].second = true;
		}
	}
	return recalibration_sucess;
}

STD_CONT<std::pair<bool, bool>> &CalibrationInfo::recalibrate(STD_CONT<double> &S2_S,
	STD_CONT<double> &double_I, DVECTOR& Fields, int channel)//sets S2_S and double_I to N_pe. returns sucess vector
{
	//experiment->channel->{direct, double_I}
	STD_CONT<std::pair<bool, bool>> recalibration_sucess;
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind)
		recalibration_sucess.push_back(std::pair<bool, bool>(false,false));

	int ch_ind = state_info->mppc_channel_to_index(channel);
	if ((ch_ind < 0) || (s1pe[ch_ind] <= 0))
		return;
	//calculate N_pe_direct
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		S2_S[exp_ind] /= s1pe[ch_ind];
		recalibration_sucess[exp_ind].first = true;
	}
	TVectorD direct_line;
	TVectorD double_I_line;
	DVECTOR xs, direct_ys;
	DVECTOR double_I_ys;
	for (int pt = N_used_in_calibration[ch_ind].first; (pt <= N_used_in_calibration[ch_ind].second) && (pt < state_info->experiments.size()); ++pt){
		if (pt < 0)
			continue;
		direct_ys.push_back(S2_S[pt]);
		xs.push_back(Fields[pt]);
		double_I_ys.push_back(double_I[pt]);
	}
	if (direct_ys.size() == 0)
		return;
	if (direct_ys.size() == 1){
		double coef = double_I_ys.back() / direct_ys.back();
		for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			double_I[exp_ind] = S2_S[exp_ind] * coef;
			recalibration_sucess[exp_ind].second = true;
		}
		return;
	}
	PolynomialFit line_direct(1);
	PolynomialFit line_double_I(1);
	line_direct(xs, direct_ys, direct_line);
	line_double_I(xs, double_I_ys, double_I_line);
	double x_intersect = -(direct_line[0] - double_I_line[0]) / (direct_line[1] - double_I_line[1]);
	double y_intersect = x_intersect*direct_line[1] + direct_line[0];
	double slope_ratio = direct_line[1] / double_I_line[1];
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		double_I[exp_ind] = slope_ratio*(double_I[exp_ind] - y_intersect) + y_intersect;
		recalibration_sucess[exp_ind].second = true;
	}

	return recalibration_sucess;
}
