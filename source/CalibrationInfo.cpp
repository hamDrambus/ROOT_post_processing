#include "CalibrationInfo.h"


CalibrationInfo::CalibrationInfo(AnalysisStates* data):
state_info(data)
{
	s1pe_PMT3 = 1;//TODO: ParameterPile (need to ask the real values from Vlad)
	s1pe_PMT1 = 1;
	for (auto i = state_info->experiments.begin(); i != state_info->experiments.end(); ++i) {
		avr_S1pe.push_back(std::deque<Double_t>());
		avr_S2pe.push_back(std::deque<Double_t>());
		method.push_back(std::deque<S1pe_method>());
		for (auto ff = state_info->MPPC_channels.begin(); ff != state_info->MPPC_channels.end(); ++ff){
			avr_S1pe.back().push_back(-1);
			avr_S2pe.back().push_back(-1);
			method.back().push_back(Ignore);
		}
	}
	for (auto ff = state_info->MPPC_channels.begin(); ff != state_info->MPPC_channels.end(); ++ff){
		s1pe.push_back(-1);
		forced_s1pe.push_back(kFALSE);
		N_used_in_calibration.push_back(/*ParameterPile::*/calibaration_poInt_ts);
	}
}

Double_t CalibrationInfo::calculateS1pe(Int_t channel)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(channel);
	if (ch_ind < 0)
		return -1;
	if (forced_s1pe[ch_ind])
		return s1pe[ch_ind];
	Double_t val = 0;
	Int_t n_used = 0;
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
	val = n_used ? val / n_used : s1pe[ch_ind];//does not invalidate s1pe (in case it was loaded)
	s1pe[ch_ind] = val;
	return val;
}

Double_t CalibrationInfo::getS1pe(Int_t channel)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(channel);
	return (ch_ind < 0) ? -1 : s1pe[ch_ind];
}
void CalibrationInfo::setS1pe(Int_t channel, Double_t val)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(channel);
	if (ch_ind < 0)
		return;
	s1pe[ch_ind] = val;
	forced_s1pe[ch_ind] = kTRUE;
}

void CalibrationInfo::unsetS1pe(Int_t ch)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	forced_s1pe[ch_ind] = kFALSE;
	calculateS1pe(ch);
}

Double_t CalibrationInfo::getPMT_S1pe(Int_t ch)
{
	return (0 == ch) ? s1pe_PMT3 : (1 == ch ? s1pe_PMT1 : -1);
}
void CalibrationInfo::setPMT_S1pe(Int_t ch, Double_t val)
{
	if (0 == ch)
		s1pe_PMT3 = val;
	if (1 == ch)
		s1pe_PMT1 = val;
}

void CalibrationInfo::set_N_calib(Int_t ch, Int_t from, Int_t to)
{
	set_N_calib(ch, std::pair<Int_t, Int_t>(from, to));
}

void CalibrationInfo::set_N_calib(Int_t channel, std::pair<Int_t, Int_t> val)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(channel);
	if (ch_ind < 0)
		return;
	N_used_in_calibration[ch_ind] = val;
}

std::pair<Int_t, Int_t> CalibrationInfo::get_N_calib(Int_t ch)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return std::pair<Int_t,Int_t>(-1,-1);
	return N_used_in_calibration[ch_ind];
}

CalibrationInfo::S1pe_method CalibrationInfo::get_method(Int_t exp_ch, Int_t ch)//called from PostProcessor
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return Ignore;
	return method[exp_ch][ch_ind];
}

void CalibrationInfo::set_method(Int_t exp_ch, Int_t ch, S1pe_method meth)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	method[exp_ch][ch_ind] = meth;
}

//setters/getters for avr_S1pe and avr_S2pe which are used for s1pe calcualtions
void CalibrationInfo::set_S1pe(Int_t ch, Int_t exp_index, Double_t val)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	avr_S1pe[exp_index][ch] = val;
	calculateS1pe(ch);
}
Double_t CalibrationInfo::get_S1pe(Int_t ch, Int_t exp_index)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return -1;
	return avr_S1pe[exp_index][ch];
}
void CalibrationInfo::set_S2pe(Int_t ch, Int_t exp_index, Double_t val)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	avr_S2pe[exp_index][ch] = val;
	calculateS1pe(ch);
}
Double_t CalibrationInfo::get_S2pe(Int_t ch, Int_t exp_index)
{
	Int_t ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return -1;
	return avr_S2pe[exp_index][ch];
}

//TODO: set Double_t I calibration from pre set parameters (e.g. from file or ParameterPile)
//S2_S and Double_I must be the full and same size [experiment]x[mppc_channel]
//sets S2_S and Double_I to N_pe
std::deque<std::deque<std::pair<Bool_t, Bool_t>>> &CalibrationInfo::recalibrate(std::deque<std::deque<Double_t>> &S2_S,
	std::deque<std::deque<Double_t>> &Double_I, std::vector<Double_t>& Fields)
{
	//experiment->channel->{direct, Double_I}
	std::deque<std::deque<std::pair<Bool_t, Bool_t>>> recalibration_sucess;
	for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		recalibration_sucess.push_back(std::deque<std::pair<Bool_t, Bool_t> >());
		for (Int_t ch_ind = 0; ch_ind < S2_S.back().size(); ++ch_ind)
			recalibration_sucess.back().push_back(std::pair<Bool_t, Bool_t>(kFALSE, kFALSE));
	}

	for (Int_t ch_ind = 0; ch_ind < S2_S.back().size(); ++ch_ind){
		if (s1pe[ch_ind] <= 0)
			continue;
		//calculate N_pe_direct
		for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			S2_S[exp_ind][ch_ind] /= s1pe[ch_ind];
			recalibration_sucess[exp_ind][ch_ind].first = kTRUE;
		}
		TVectorD direct_line;
		TVectorD Double_I_line;
		std::vector<Double_t> xs, direct_ys;
		std::vector<Double_t> Double_I_ys;
		for (Int_t pt = N_used_in_calibration[ch_ind].first; (pt <= N_used_in_calibration[ch_ind].second) && (pt < state_info->experiments.size()); ++pt){
			if (pt < 0)
				continue;
			direct_ys.push_back(S2_S[pt][ch_ind]);
			xs.push_back(Fields[pt]);
			Double_I_ys.push_back(Double_I[pt][ch_ind]);
		}
		if (direct_ys.size() == 0)
			continue;
		if (direct_ys.size() == 1){
			Double_t coef = Double_I_ys.back() / direct_ys.back();
			for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
				Double_I[exp_ind][ch_ind] = S2_S[exp_ind][ch_ind] * coef;
				recalibration_sucess[exp_ind][ch_ind].second = kTRUE;
			}
			continue;
		}
		PolynomialFit line_direct(1);
		PolynomialFit line_Double_I(1);
		line_direct(xs, direct_ys, direct_line);
		line_Double_I(xs, Double_I_ys, Double_I_line);
		Double_t x_Int_tersect = -(direct_line[0] - Double_I_line[0]) / (direct_line[1] - Double_I_line[1]);
		Double_t y_Int_tersect = x_Int_tersect*direct_line[1] + direct_line[0];
		Double_t slope_ratio = direct_line[1] / Double_I_line[1];
		for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			Double_I[exp_ind][ch_ind] = slope_ratio*(Double_I[exp_ind][ch_ind] - y_Int_tersect) + y_Int_tersect;
			recalibration_sucess[exp_ind][ch_ind].second = kTRUE;
		}
	}
	return recalibration_sucess;
}

std::deque<std::pair<Bool_t, Bool_t>> &CalibrationInfo::recalibrate(std::deque<Double_t> &S2_S,
	std::deque<Double_t> &Double_I, std::vector<Double_t>& Fields, Int_t channel)//sets S2_S and Double_I to N_pe. returns sucess vector
{
	//experiment->channel->{direct, Double_I}
	std::deque<std::pair<Bool_t, Bool_t>> recalibration_sucess;
	for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind)
		recalibration_sucess.push_back(std::pair<Bool_t, Bool_t>(kFALSE,kFALSE));

	Int_t ch_ind = state_info->mppc_channel_to_index(channel);
	if ((ch_ind < 0) || (s1pe[ch_ind] <= 0))
		return recalibration_sucess;
	//calculate N_pe_direct
	for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		S2_S[exp_ind] /= s1pe[ch_ind];
		recalibration_sucess[exp_ind].first = kTRUE;
	}
	TVectorD direct_line;
	TVectorD Double_I_line;
	std::vector<Double_t> xs, direct_ys;
	std::vector<Double_t> Double_I_ys;
	for (Int_t pt = N_used_in_calibration[ch_ind].first; (pt <= N_used_in_calibration[ch_ind].second) && (pt < state_info->experiments.size()); ++pt){
		if (pt < 0)
			continue;
		direct_ys.push_back(S2_S[pt]);
		xs.push_back(Fields[pt]);
		Double_I_ys.push_back(Double_I[pt]);
	}
	if (direct_ys.size() == 0)
		return recalibration_sucess;
	if (direct_ys.size() == 1){
		Double_t coef = Double_I_ys.back() / direct_ys.back();
		for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			Double_I[exp_ind] = S2_S[exp_ind] * coef;
			recalibration_sucess[exp_ind].second = kTRUE;
		}
		return recalibration_sucess;
	}
	PolynomialFit line_direct(1);
	PolynomialFit line_Double_I(1);
	line_direct(xs, direct_ys, direct_line);
	line_Double_I(xs, Double_I_ys, Double_I_line);
	Double_t x_Int_tersect = -(direct_line[0] - Double_I_line[0]) / (direct_line[1] - Double_I_line[1]);
	Double_t y_Int_tersect = x_Int_tersect*direct_line[1] + direct_line[0];
	Double_t slope_ratio = direct_line[1] / Double_I_line[1];
	for (Int_t exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		Double_I[exp_ind] = slope_ratio*(Double_I[exp_ind] - y_Int_tersect) + y_Int_tersect;
		recalibration_sucess[exp_ind].second = kTRUE;
	}

	return recalibration_sucess;
}

void CalibrationInfo::read_file(std::vector<std::pair<Int_t, Double_t>> &current_list)
{
	std::ifstream str;
	str.open(OUTPUT_DIR + CALIBRATION_FILE);
	Int_t ch = 0;
	Double_t val = 0;
	while (str.is_open() && str.good() && !str.eof()) {
		str >> ch;
		str >> val;
		if (str.good() && !str.eof()){
			current_list.push_back(std::pair<Int_t, Double_t>(ch, val));
		}
	}
	str.close();
}

void CalibrationInfo::Save(void)
{
	std::vector<std::pair<Int_t, Double_t>> list_;
	read_file(list_);

	std::ofstream str;
	open_output_file(OUTPUT_DIR + CALIBRATION_FILE, str);
	if (str.is_open() && str.good()) {
		std::vector<Bool_t> already_exists;
		already_exists.resize(2 + state_info->MPPC_channels.size(), kFALSE);
		//update existing info
		for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(),
			ch_begin_ = state_info->MPPC_channels.begin(); ch != ch_end_; ++ch) {
			for (auto ch_e = list_.begin(), ch_e_end_ = list_.end(); ch_e != ch_e_end_; ++ch_e) { 
				if (*ch == ch_e->first) {
					already_exists[ch - ch_begin_] = kTRUE;
					Int_t index = state_info->mppc_channel_to_index(*ch);
					if (index >= 0)
						ch_e->second = s1pe[index];
				}
			}
		}
		//insert additional info (only mppc channels)
		for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(),
			ch_begin_ = state_info->MPPC_channels.begin(); ch != ch_end_; ++ch) {
			Int_t index = state_info->mppc_channel_to_index(*ch);
			if ((index>=0)&&!(already_exists[ch - ch_begin_])) {
				for (auto ch_e = list_.begin(), ch_e_end_ = list_.end(); ch_e != ch_e_end_; ++ch_e) {
					if (ch_e == (ch_e_end_ - 1)){
						list_.insert(ch_e_end_, std::pair<Int_t, Double_t>(*ch, s1pe[index]));
					} else {
						if ((ch_e->first<*ch) && ((ch_e + 1)->first>*ch)){
							list_.insert(ch_e+1, std::pair<Int_t, Double_t>(*ch, s1pe[index]));
						}
					}
				}
			}
		}
		for (auto ch_e = list_.begin(), ch_e_end_ = list_.end(); ch_e != ch_e_end_; ++ch_e) {
			str << ch_e->first << "\t" << ch_e->second << ((ch_e == (ch_e_end_ - 1))?"":"\n");
		}
	}
	str.close();
}

void CalibrationInfo::Load(void)
{
	std::vector<std::pair<Int_t, Double_t>> list_;
	read_file(list_);
	for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(),
		ch_begin_ = state_info->MPPC_channels.begin(); ch != ch_end_; ++ch) {
		for (auto ch_e = list_.begin(), ch_e_end_ = list_.end(); ch_e != ch_e_end_; ++ch_e) {
			if (*ch == ch_e->first) {
				Int_t index = state_info->mppc_channel_to_index(*ch);
				if (index >= 0)
					s1pe[index] = ch_e->second;
				else {
					if (0 == *ch)
						s1pe_PMT3 = ch_e->second;
					if (1 == *ch)
						s1pe_PMT1 = ch_e->second;
				}
			}
		}
	}
}
