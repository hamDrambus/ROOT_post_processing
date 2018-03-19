#include "CalibrationInfo.h"


CalibrationInfo::CalibrationInfo(AnalysisStates* data):
state_info(data)
{
	s1pe_PMT3 = 1;//TODO: ParameterPile (need to ask the real values from Vlad)
	s1pe_PMT1 = 1;
	for (auto i = state_info->experiments.begin(); i != state_info->experiments.end(); ++i) {
		avr_S1pe.push_back(std::deque<double>());
		avr_S2pe.push_back(std::deque<double>());
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
		N_used_in_calibration.push_back(/*ParameterPile::*/calibaration_points);
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
	val = n_used ? val / n_used : s1pe[ch_ind];//does not invalidate s1pe (in case it was loaded)
	s1pe[ch_ind] = val;
	return val;
}

double CalibrationInfo::getS1pe(int channel)
{
	return calculateS1pe(channel);
}
void CalibrationInfo::setS1pe(int channel, double val)
{
	int ch_ind = state_info->mppc_channel_to_index(channel);
	if (ch_ind < 0)
		return;
	s1pe[ch_ind] = val;
	forced_s1pe[ch_ind] = kTRUE;
}

void CalibrationInfo::unsetS1pe(int ch)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	forced_s1pe[ch_ind] = kFALSE;
	calculateS1pe(ch);
}

double CalibrationInfo::getPMT_S1pe(int ch, int exp_ind)
{
	if (PMT_V.find(state_info->experiments[exp_ind])!=PMT_V.end()){
		double V = PMT_V.find(state_info->experiments[exp_ind])->second;
		if (0==ch){
			if (s1pe_PMT3.find(V)!=s1pe_PMT3.end()) {
				return s1pe_PMT3.find(V)->second;
			}
		} else { //1==ch
			if (s1pe_PMT1.find(V)!=s1pe_PMT1.end()) {
				return s1pe_PMT1.find(V)->second;
			}
		}
	}
	std::cout<<"Error: no PMT calibration for ch "<<ch<<" and experiment '"<<state_info->experiments[exp_ind]<<"'"<<std::endl;
	return -1;
}
void CalibrationInfo::setPMT_S1pe(int ch, double pmt_v, double val)
{
	if (0 == ch)
		s1pe_PMT3[pmt_v] = val;
	if (1 == ch)
		s1pe_PMT1[pmt_v] = val;
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
	avr_S1pe[exp_index][ch_ind] = val;
	calculateS1pe(ch);
}
double CalibrationInfo::get_S1pe(int ch, int exp_index)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return -1;
	return avr_S1pe[exp_index][ch_ind];
}
void CalibrationInfo::set_S2pe(int ch, int exp_index, double val)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return;
	avr_S2pe[exp_index][ch_ind] = val;
	calculateS1pe(ch);
}
double CalibrationInfo::get_S2pe(int ch, int exp_index)
{
	int ch_ind = state_info->mppc_channel_to_index(ch);
	if (ch_ind < 0)
		return -1;
	return avr_S2pe[exp_index][ch_ind];
}

//TODO: set double I calibration from pre set parameters (e.g. from file or ParameterPile)
//S2_S and Double_I must be the full and same size [experiment]x[mppc_channel]
//sets S2_S and Double_I to N_pe
std::deque<std::deque<std::pair<Bool_t, Bool_t>>> &CalibrationInfo::recalibrate(std::deque<std::deque<double>> &S2_S,
	std::deque<std::deque<double>> &Double_I, std::vector<double>& Fields)
{
	//experiment->channel->{direct, Double_I}
	std::deque<std::deque<std::pair<Bool_t, Bool_t>>> recalibration_sucess;
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		recalibration_sucess.push_back(std::deque<std::pair<Bool_t, Bool_t> >());
		for (int ch_ind = 0; ch_ind < S2_S.back().size(); ++ch_ind)
			recalibration_sucess.back().push_back(std::pair<Bool_t, Bool_t>(kFALSE, kFALSE));
	}

	for (int ch_ind = 0; ch_ind < S2_S.back().size(); ++ch_ind){
		if (s1pe[ch_ind] <= 0)
			continue;
		//calculate N_pe_direct
		for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			S2_S[exp_ind][ch_ind] /= getS1pe(state_info->MPPC_channels[ch_ind]); //instead of s1pe[ch_ind] getS1pe is called which updates the value if necessary.
			recalibration_sucess[exp_ind][ch_ind].first = kTRUE;
		}
		TVectorD direct_line;
		TVectorD Double_I_line;
		std::vector<double> xs, direct_ys;
		std::vector<double> Double_I_ys;
		for (int pt = N_used_in_calibration[ch_ind].first; (pt <= N_used_in_calibration[ch_ind].second) && (pt < state_info->experiments.size()); ++pt){
			if (pt < 0)
				continue;
			direct_ys.push_back(S2_S[pt][ch_ind]);
			xs.push_back(Fields[pt]);
			Double_I_ys.push_back(Double_I[pt][ch_ind]);
		}
		if (direct_ys.size() == 0)
			continue;
		if (direct_ys.size() == 1){
			double coef = Double_I_ys.back() / direct_ys.back();
			for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
				Double_I[exp_ind][ch_ind] = S2_S[exp_ind][ch_ind] * coef;
				recalibration_sucess[exp_ind][ch_ind].second = kTRUE;
			}
			continue;
		}
		PolynomialFit line_direct(1);
		PolynomialFit line_Double_I(1);
		line_direct(xs, direct_ys, direct_line);
		line_Double_I(xs, Double_I_ys, Double_I_line);
		double x_intersect = -(direct_line[0] - Double_I_line[0]) / (direct_line[1] - Double_I_line[1]);
		double y_intersect = x_intersect*direct_line[1] + direct_line[0];
		double slope_ratio = direct_line[1] / Double_I_line[1];
		for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			Double_I[exp_ind][ch_ind] = slope_ratio*(Double_I[exp_ind][ch_ind] - y_intersect) + y_intersect;
			recalibration_sucess[exp_ind][ch_ind].second = kTRUE;
		}
	}
	return recalibration_sucess;
}

std::deque<std::pair<Bool_t, Bool_t>> &CalibrationInfo::recalibrate(std::deque<double> &S2_S,
	std::deque<double> &Double_I, std::vector<double>& Fields, int channel)//sets S2_S and Double_I to N_pe. returns sucess vector
{
	//experiment->channel->{direct, Double_I}
	std::deque<std::pair<Bool_t, Bool_t>> recalibration_sucess;
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind)
		recalibration_sucess.push_back(std::pair<Bool_t, Bool_t>(kFALSE,kFALSE));

	int ch_ind = state_info->mppc_channel_to_index(channel);
	if ((ch_ind < 0) || (s1pe[ch_ind] <= 0))
		return recalibration_sucess;
	//calculate N_pe_direct
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		S2_S[exp_ind] /= s1pe[ch_ind];
		recalibration_sucess[exp_ind].first = kTRUE;
	}
	TVectorD direct_line;
	TVectorD Double_I_line;
	std::vector<double> xs, direct_ys;
	std::vector<double> Double_I_ys;
	for (int pt = N_used_in_calibration[ch_ind].first; (pt <= N_used_in_calibration[ch_ind].second) && (pt < state_info->experiments.size()); ++pt){
		if (pt < 0)
			continue;
		direct_ys.push_back(S2_S[pt]);
		xs.push_back(Fields[pt]);
		Double_I_ys.push_back(Double_I[pt]);
	}
	if (direct_ys.size() == 0)
		return recalibration_sucess;
	if (direct_ys.size() == 1){
		double coef = Double_I_ys.back() / direct_ys.back();
		for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
			Double_I[exp_ind] = S2_S[exp_ind] * coef;
			recalibration_sucess[exp_ind].second = kTRUE;
		}
		return recalibration_sucess;
	}
	PolynomialFit line_direct(1);
	PolynomialFit line_Double_I(1);
	line_direct(xs, direct_ys, direct_line);
	line_Double_I(xs, Double_I_ys, Double_I_line);
	double x_intersect = -(direct_line[0] - Double_I_line[0]) / (direct_line[1] - Double_I_line[1]);
	double y_intersect = x_intersect*direct_line[1] + direct_line[0];
	double slope_ratio = direct_line[1] / Double_I_line[1];
	for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind){
		Double_I[exp_ind] = slope_ratio*(Double_I[exp_ind] - y_intersect) + y_intersect;
		recalibration_sucess[exp_ind].second = kTRUE;
	}

	return recalibration_sucess;
}

void CalibrationInfo::read_file(std::vector<std::pair<int/*ch*/, std::string> > &current_list)
{
	std::ifstream str;
	str.open(OUTPUT_DIR + CALIBRATION_FILE);
	int ch = 0;
	double val = 0;
	while (str.is_open() && str.good() && !str.eof()) {
		str >> ch;
		str >> val;
		if (str.good() && !str.eof()){
			current_list.push_back(std::pair<int, double>(ch, val));
		}
	}
	str.close();
}
void CalibrationInfo::extract_PMT_info (std::vector<std::pair<int/*ch*/, std::string> > &current_list, PMT_info_ &PMT_table)
{

}
void CalibrationInfo::extract_MPPC_info (std::vector<std::pair<int/*ch*/, std::string> > &current_list, MPPC_info_ &MPPC_table)
{

}
void CalibrationInfo::add_to_PMT_info (PMT_info_& table, int ch, double V, double S1pe) //preserves sorting, updates if necessary
{

}
void CalibrationInfo::add_to_MPPC_info (MPPC_info_& table, int ch, double S1pe)         //preserves sorting, updates if necessary
{

}
void CalibrationInfo::write_to_file (PMT_info_& PMT_table, MPPC_info_& MPPC_table)
{

}

void CalibrationInfo::Save(void) //TODO: this function does not work on empty file (empty list_)
{
	std::vector<std::pair<int, std::string>> list_;
	read_file(list_);

	PMT_info_ PMT_table;
	MPPC_info_ MPPC_table;

	extract_PMT_info (list_, PMT_table);
	extract_MPPC_info (list_, MPPC_table);

	for (auto ch = state_info->PMT_channels.begin(), ch_end_ = state_info->PMT_channels.end(); ch != ch_end_; ++ch) {
		if (0==*ch){
			for (auto v_s = s1pe_PMT3.begin(),_end_v_s = s1pe_PMT3.end();v_s!=_end_v_s;++v_s) {
				add_to_PMT_info(PMT_table, *ch, v_s->first, v_s->second);
			}
		}
		if (1==*ch){
			for (auto v_s = s1pe_PMT1.begin(),_end_v_s = s1pe_PMT1.end();v_s!=_end_v_s;++v_s) {
				add_to_PMT_info(PMT_table, *ch, v_s->first, v_s->second);
			}
		}
	}

	for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(); ch != ch_end_; ++ch) {
		add_to_MPPC_info(MPPC_table, *ch, getS1pe(*ch));
	}

	write_to_file(PMT_table, MPPC_table);
}

void CalibrationInfo::Load(void)
{
	std::vector<std::pair<int, std::string>> list_;
	read_file(list_);

	PMT_info_ PMT_table;
	MPPC_info_ MPPC_table;

	extract_PMT_info (list_, PMT_table);
	extract_MPPC_info (list_, MPPC_table);

	for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(); ch != ch_end_; ++ch) {
		for (auto ch_e = MPPC_table.begin(), ch_e_end_ = MPPC_table.end(); ch_e != ch_e_end_; ++ch_e) {
			if (*ch == ch_e->first) {
				int index = state_info->mppc_channel_to_index(*ch);
				if (index >= 0)
					s1pe[index] = ch_e->second;
			}
		}
	}

	for (auto ch = state_info->PMT_channels.begin(), ch_end_ = state_info->PMT_channels.end(); ch != ch_end_; ++ch) {
		for (auto ch_e = PMT_table.begin(), ch_e_end_ = PMT_table.end(); ch_e != ch_e_end_; ++ch_e) {
			if (*ch == ch_e->first) {
				if (0==*ch) {
					s1pe_PMT3.clear();
					for (auto v_s = ch_e->second.begin(), _end_v_s = ch_e->second.end(); v_s!=_end_v_s; ++v_s)
						s1pe_PMT3.insert(*v_s);
				}
				if (1==*ch) {
					s1pe_PMT1.clear();
					for (auto v_s = ch_e->second.begin(), _end_v_s = ch_e->second.end(); v_s!=_end_v_s; ++v_s)
						s1pe_PMT1.insert(*v_s);
				}
			}
		}
	}

}
