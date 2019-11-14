#include "CalibrationInfo.h"

CalibrationInfo::CalibrationInfo(const AStates* data, std::string fname):
state_info(data), s1pe_table_()
{
	for (auto i = state_info->experiments.begin(); i != state_info->experiments.end(); ++i) {
		s1pe_exp_.push_back(S1pe_exp());
		for (auto ff = state_info->PMT_channels.begin(); ff != state_info->PMT_channels.end(); ++ff) {
			s1pe_exp_.back().avr_S1pe.push_back(-1);
			s1pe_exp_.back().avr_S2pe.push_back(-1);
			s1pe_exp_.back().avr_S1pe_w.push_back(0);
			s1pe_exp_.back().avr_S2pe_w.push_back(0);
			s1pe_exp_.back().method.push_back(Ignore);
		}
		for (auto ff = state_info->MPPC_channels.begin(); ff != state_info->MPPC_channels.end(); ++ff) {
			s1pe_exp_.back().avr_S1pe.push_back(-1);
			s1pe_exp_.back().avr_S2pe.push_back(-1);
			s1pe_exp_.back().avr_S1pe_w.push_back(0);
			s1pe_exp_.back().avr_S2pe_w.push_back(0);
			s1pe_exp_.back().method.push_back(Ignore);
		}
	}
	for (auto ff = state_info->PMT_channels.begin(); ff != state_info->PMT_channels.end(); ++ff) {
		s1pe_table_.push(*ff);
		N_used_in_calibration.push_back(/*ParameterPile::*/calibaration_points);
	}
	for (auto ff = state_info->MPPC_channels.begin(); ff != state_info->MPPC_channels.end(); ++ff) {
		s1pe_table_.push(*ff);
		N_used_in_calibration.push_back(/*ParameterPile::*/calibaration_points);
	}
	Load(fname);
}

void CalibrationInfo::S1pe_exp::set_S1pe_exp(std::size_t ch, double val, int weight)
{
	if (ch>=avr_S1pe.size()) {
		std::cerr<<"CalibrationInfo::S1pe_exp::set_S1pe_exp: Error: ch_index "<<ch<<"is out of range"<<std::endl;
		return;
	}
	avr_S1pe[ch] = val;
	avr_S1pe_w[ch] = weight;
}

double CalibrationInfo::S1pe_exp::get_S1pe_exp(std::size_t ch) const
{
	if (ch>=avr_S1pe.size()) {
		std::cerr<<"CalibrationInfo::S1pe_exp::get_S1pe_exp: Error: ch_index "<<ch<<"is out of range"<<std::endl;
		return -1;
	}
	return avr_S1pe[ch];
}

int CalibrationInfo::S1pe_exp::get_S1pe_w_exp(std::size_t ch) const
{
	if (ch >= avr_S1pe.size()) {
		std::cerr << "CalibrationInfo::S1pe_exp::get_S1pe_w_exp: Error: ch_index " << ch << "is out of range" << std::endl;
		return -1;
	}
	return avr_S1pe_w[ch];
}

void CalibrationInfo::S1pe_exp::set_S2pe_exp(std::size_t ch, double val, int weight)
{
	if (ch>=avr_S1pe.size()) {
		std::cerr<<"CalibrationInfo::S1pe_exp::set_S2pe_exp: Error: ch_index "<<ch<<"is out of range"<<std::endl;
		return;
	}
	avr_S2pe[ch] = val;
	avr_S2pe_w[ch] = weight;
}

double CalibrationInfo::S1pe_exp::get_S2pe_exp(std::size_t ch) const
{
	if (ch>=avr_S1pe.size()) {
		std::cerr<<"CalibrationInfo::S1pe_exp::get_S2pe_exp: Error: ch_index "<<ch<<"is out of range"<<std::endl;
		return -1;
	}
	return avr_S2pe[ch];
}

int CalibrationInfo::S1pe_exp::get_S2pe_w_exp(std::size_t ch) const
{
	if (ch >= avr_S1pe.size()) {
		std::cerr << "CalibrationInfo::S1pe_exp::get_S2pe_w_exp: Error: ch_index " << ch << "is out of range" << std::endl;
		return -1;
	}
	return avr_S2pe_w[ch];
}

CalibrationInfo::S1pe_method CalibrationInfo::S1pe_exp::get_method(std::size_t ch) const
{
	if (ch>=avr_S1pe.size()) {
		std::cerr<<"CalibrationInfo::S1pe_exp::get_method: Error: ch_index "<<ch<<"is out of range"<<std::endl;
		return CalibrationInfo::S1pe_method::Ignore;
	}
	return method[ch];
}

void CalibrationInfo::S1pe_exp::set_method(std::size_t ch, S1pe_method meth)
{
	if (ch>=avr_S1pe.size()) {
		std::cerr<<"CalibrationInfo::S1pe_exp::set_method: Error: ch_index "<<ch<<"is out of range"<<std::endl;
		return;
	}
	method[ch] = meth;
}

CalibrationInfo::S1pe_table::S1pe_table()
{}

int CalibrationInfo::S1pe_table::ch_to_index(int ch) const
{
	return getIndex(channels_, ch);
}

int CalibrationInfo::S1pe_table::ch_index_to_ch(int ch_index) const
{
	if (ch_index < 0 || ch_index >= channels_.size()) {
		std::cerr<<"CalibrationInfo::S1pe_table::ch_index_to_ch: Error: channel index"<<ch_index<<" is out of range of S1pe table"<<std::endl;
		return INT_MIN;
	}
	return channels_[ch_index];
}

std::pair<std::size_t, std::size_t> CalibrationInfo::S1pe_table::ch_bound_indices(int ch, bool &failed) const
{
	std::pair<std::size_t, std::size_t> out;
	failed = true;
	std::size_t sz = channels_.size();
	if (0 == sz)
		return out;
	failed = false;
	if (ch <= channels_.front()) {
		out = std::pair<std::size_t, std::size_t>(0, 0);
		return out;
	}
	if (ch >= channels_.back()) {
		out = std::pair<std::size_t, std::size_t>(sz - 1, sz - 1);
		return out;
	}
	//find first x which is not less that X_point. That is index bounding X_point: xs[first] <= X_point < xs[first + 1]
	//See std::lower_bound and std::upper_bound
	std::size_t count = sz;
	std::size_t first = 0;
	//std::lower_bound(xys.begin(), xys.end(), [](const std::pair<double, double> &a, const std::pair<double, double> &b)->bool{
	//	return a.first<b.first;
	//});
	while (count > 0) {
		std::size_t step = count / 2;
		std::size_t ind = first + step;
		if (!(ch < channels_[ind])) {
			first = ++ind;
			count -= step + 1;
		} else
			count = step;
	}
	//first is such, that x>=xs[first-1] and x<xs[first]
	//first always != 0 here
	--first;
	if (ch == channels_[first]) {
		out = std::pair<std::size_t, std::size_t>(first, first);
		return out;
	}
	out = std::pair<std::size_t, std::size_t>(first, first + 1);
	return out;
}

bool CalibrationInfo::S1pe_table::push(int ch) //does not erase already existing ch. returns true if channel is created. Preserves sorting by channel
{
	std::size_t sz = channels_.size();
	if (0 == sz) {
		channels_.push_back(ch);
		s1pe_.push_back(S1pe());
		return true;
	}
	if (ch < channels_.front()) {
		channels_.insert(channels_.begin(), ch);
		s1pe_.insert(s1pe_.begin(), S1pe());
		return true;
	}
	if (ch > channels_.back()) {
		channels_.push_back(ch);
		s1pe_.push_back(S1pe());
		return true;
	}
	bool failed = false;
	std::pair<std::size_t, std::size_t> inds = ch_bound_indices(ch, failed); //can't return failed == true here
	if (inds.first == inds.second) {
		std::cout<<"CalibrationInfo::S1pe_table::push(int ch): Warning: channel "<<ch<<" already exists"<<std::endl;
		return false;
	} else {
		channels_.insert(channels_.begin() + inds.second, ch);
		s1pe_.insert(s1pe_.begin() + inds.second, S1pe());
	}
	return true;
}

bool CalibrationInfo::S1pe_table::push(int ch, double V, double s1pe_val, bool forced, bool forced_by_default) //creates or updates entry
{
	bool ret = false;
	int ind = ch_to_index(ch);
	if (ind<0) {
		ret = true;
		push(ch);
		ind = ch_to_index(ch);
	}
	if (ind < 0) //TODO: throw?
		return ret;
	auto entry = s1pe_[ind].find(V);
	if (entry != s1pe_[ind].end()) {//modify if value not forced
		if (!forced && entry->second.first) {
			//std::cout << "CalibrationInfo::S1pe_table::push: Warning: forced value of s1pe for ch " << ch << "V=" << V << " (="<<s1pe_val<<") is not updated" << std::endl;
		} else {
			if (forced)
				entry->second.first = true;
			entry->second.second = s1pe_val;
		}
	} else {
		ret = true;
		s1pe_[ind][V] = std::pair<bool, double>(forced||forced_by_default, s1pe_val);
	}
	return ret;
}

void CalibrationInfo::S1pe_table::remove(int ch)
{
	int ind = ch_to_index(ch);
	if (ind < 0)
		return;
	channels_.erase(channels_.begin()+ind);
	s1pe_.erase(s1pe_.begin()+ind);
}

bool CalibrationInfo::S1pe_table::unforce_S1pe(int ch, double V)  //returns true if recalculation is required
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0) {
		auto entry = s1pe_[ch_index].find(V);
		if (s1pe_[ch_index].end() != entry && entry->second.first) {
			entry->second.first = false;
			return true;
		}
	}
	return false;
}

bool CalibrationInfo::S1pe_table::isforced_S1pe(int ch, double V)
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0) {
		auto entry = s1pe_[ch_index].find(V);
		if (s1pe_[ch_index].end() != entry && entry->second.first) {
			return true;
		}
	}
	return false;
}

double CalibrationInfo::S1pe_table::get_S1pe(int ch, double V) const
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0) {
		auto entry = s1pe_[ch_index].find(V);
		if (s1pe_[ch_index].end() != entry)
			return entry->second.second;
	}
	return -1;
}

int CalibrationInfo::ch_to_index(int ch) const
{
	int pmt_i = getIndex(state_info->PMT_channels, ch), mppc_i =  getIndex(state_info->MPPC_channels, ch);
	if (pmt_i>=0 && mppc_i >=0) {
		std::cerr<<"CalibrationInfo::ch_to_index: Error: channel "<<ch<<" belongs to both PMT and MPPC channels"<<std::endl;
		return -1;
	}
	if (pmt_i<0 && mppc_i <0) {
		return -1;
	}
	if (mppc_i<0)
		return pmt_i;
	else
		return state_info->PMT_channels.size() + mppc_i;
}

int CalibrationInfo::ch_index_to_ch(int ch_index) const
{
	std::size_t pmt_sz = state_info->PMT_channels.size();
	if (ch_index<0 || ch_index > pmt_sz + state_info->MPPC_channels.size())
		return INT_MIN;
	else
		return (ch_index >= pmt_sz ? state_info->MPPC_channels[ch_index - pmt_sz] : state_info->PMT_channels[ch_index]);
}

std::vector<int> CalibrationInfo::translate_V_to_exp (int ch, double V) const
{
	std::vector<int> ret;
	for (std::size_t i = 0, i_end_= state_info->experiments.size(); i!=i_end_; ++i) {
		std::string exp_str = state_info->experiments[i];
		if (getIndex(state_info->PMT_channels, ch)>=0) {
			auto e = PMT_V.find(exp_str);
			if (e!=PMT_V.end())
				if (e->second==V) {
					ret.push_back(i);
					continue;
				}
		}
		if (getIndex(state_info->MPPC_channels, ch)>=0) {
			auto e = MPPC_V.find(exp_str);
			if (e!=MPPC_V.end())
				if (e->second==V) {
					ret.push_back(i);
					continue;
				}
		}
	}
	return ret;
}

double CalibrationInfo::translate_exp_to_V (int ch, int exp_index) const
{
	if (exp_index >= s1pe_exp_.size() || exp_index < 0)
		return -1;
	std::string exp_str = state_info->experiments[exp_index];
	if (getIndex(state_info->PMT_channels, ch)>=0) {
		auto e = PMT_V.find(exp_str);
		if (e!=PMT_V.end())
			return e->second;
	}
	if (getIndex(state_info->MPPC_channels, ch)>=0) {
		auto e = MPPC_V.find(exp_str);
		if (e!=MPPC_V.end())
			return e->second;
	}
	return -1;
}

bool CalibrationInfo::read_file(std::ifstream &str, S1pe_table &to_table) const //does not overwrite forced values set during analysis
{
	bool loaded = false;
	int ch = 0;
	std::string line, word;
	while (str.is_open() && str.good() && !str.eof()) {
		std::getline(str,line);
		word = strtoken(line, " \t,");
		if (word.size()>=1) {
			if (word[0]=='/' && word[1]=='/') //ignore primitive comment
				continue;
		}
		try {
			ch = std::stoi(word);
		} catch (...) {
			continue;
		}
		std::map<double, double> V_s1pe_to_add;
		while (!line.empty()) {
			double V, s1pe;
			try {
				word = strtoken(line, " \t,");
				V = std::stod(word);
				word = strtoken(line, " \t,");
				s1pe = std::stod(word);
			} catch (...) {
				break;
			}
			loaded = true;
			if (V_s1pe_to_add.find(V) == V_s1pe_to_add.end()) {
				V_s1pe_to_add[V] = s1pe;
			} else {
				std::cerr << "CalibrationInfo::extract_calibration_info: Warninig: ch " << ch << " contains the same V (" << V << ")" << std::endl;
				std::cerr << "\tExtra values are ignored" << std::endl;
			}
		}
		for (auto vv = V_s1pe_to_add.begin(), vv_end_ = V_s1pe_to_add.end(); vv != vv_end_; ++vv)
			to_table.push(ch, vv->first, vv->second, true, true);
	}
	return loaded;
}

bool CalibrationInfo::write_to_file(std::ofstream &str, S1pe_table& table) const
{
	if (str.is_open()) {
		for (std::size_t chi = 0, chi_end_ = table.channels_.size(); chi != chi_end_; ++chi) {
			int ch = table.channels_[chi];
			str << ch << "\t";
			for (auto i = table.s1pe_[chi].begin(), i_end_ = table.s1pe_[chi].end(); i != i_end_; ++i) {
				str << i->first<< "\t" << i->second.second << "\t";
			}
			str << std::endl;
		}
		return true;
	}
	return false;
}

double CalibrationInfo::attenuate(int ch, int exp_index, double val) const
{
	std::string exp_str = state_info->experiments[exp_index];
	auto entry = dBs.find(exp_str);
	if (dBs.end() == entry)
		return val;
	dB_info* dB = entry->second.info(ch);
	if (NULL == dB)
		return val;
	return val / dB->get_atten();
}

double CalibrationInfo::deattenuate(int ch, int exp_index, double val) const
{
	std::string exp_str = state_info->experiments[exp_index];
	auto entry = dBs.find(exp_str);
	if (dBs.end() == entry)
		return val;
	dB_info* dB = entry->second.info(ch);
	if (NULL == dB)
		return val;
	return val * dB->get_atten();
}

double CalibrationInfo::get_S1pe(int ch, double V) const
{
	return s1pe_table_.get_S1pe(ch, V);
}

double CalibrationInfo::get_S1pe(int ch, int exp_index) const
{
	double V = translate_exp_to_V(ch, exp_index);
	return attenuate(ch, exp_index, s1pe_table_.get_S1pe(ch, V));
}

void CalibrationInfo::force_S1pe(int ch, double V, double val) //forces specific value which is not erased by calculateS1pe. Attenuation is considered to be 0 dB here!
{
	s1pe_table_.push(ch, V, val, true, true);
}

void CalibrationInfo::unforce_S1pe(int ch, double V) //does not erase/loads forced value from file, which cannot be calculated from experiment.
{
	if (s1pe_table_.unforce_S1pe(ch, V))
		calculateS1pe(ch, V);
}

CalibrationInfo::S1pe_method CalibrationInfo::get_method(int ch, int exp_ind) const//called from PostProcessor
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0 && exp_ind < s1pe_exp_.size() && exp_ind>=0)
		return s1pe_exp_[exp_ind].get_method(ch_index);
	return Ignore;
}

void CalibrationInfo::set_method(int ch, int exp_ind, S1pe_method method)
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0 && exp_ind < s1pe_exp_.size() && exp_ind >= 0)
		s1pe_exp_[exp_ind].set_method(ch_index, method);
}

void CalibrationInfo::set_S1pe_exp(int ch, int exp_index, double val, int weight) //!accepts value attenuated according to ::dBs
{
	int ch_index = ch_to_index(ch);
	val = deattenuate(ch, exp_index, val);
	if (ch_index >= 0 && exp_index < s1pe_exp_.size() && exp_index >= 0)
		s1pe_exp_[exp_index].set_S1pe_exp(ch_index, val, weight);
}

double CalibrationInfo::get_S1pe_exp(int ch, int exp_index) const //!returns value attenuated according to ::dBs value
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0 && exp_index < s1pe_exp_.size() && exp_index >= 0)
		return attenuate(ch, exp_index, s1pe_exp_[exp_index].get_S1pe_exp(ch_index));
	return -1;
}

int CalibrationInfo::get_S1pe_weight_exp(int ch, int exp_index) const
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0 && exp_index < s1pe_exp_.size() && exp_index >= 0)
		return s1pe_exp_[exp_index].get_S1pe_w_exp(ch_index);
	return 0;
}

void CalibrationInfo::set_S2pe_exp(int ch, int exp_index, double val, int weight) //!accepts value attenuated according to ::dBs
{
	int ch_index = ch_to_index(ch);
	val = deattenuate(ch, exp_index, val);
	if (ch_index >= 0 && exp_index < s1pe_exp_.size() && exp_index >= 0)
		s1pe_exp_[exp_index].set_S2pe_exp(ch_index, val, weight);
}

double CalibrationInfo::get_S2pe_exp(int ch, int exp_index) const //!returns value attenuated according to ::dBs
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0 && exp_index < s1pe_exp_.size() && exp_index >= 0)
		return attenuate(ch, exp_index, s1pe_exp_[exp_index].get_S2pe_exp(ch_index));
	return -1;
}

int CalibrationInfo::get_S2pe_weight_exp(int ch, int exp_index) const
{
	int ch_index = ch_to_index(ch);
	if (ch_index >= 0 && exp_index < s1pe_exp_.size() && exp_index >= 0)
		return s1pe_exp_[exp_index].get_S2pe_w_exp(ch_index);
	return 0;
}

double CalibrationInfo::calculateS1pe(int ch, double V) //from experimental avr_S1pe
{
	if (s1pe_table_.isforced_S1pe(ch, V))
		return s1pe_table_.get_S1pe(ch, V);
	std::vector<int> exps = translate_V_to_exp(ch, V);
	double val = 0;
	int n_used = 0;
	for (auto e = exps.begin(), e_end_ = exps.end(); e != e_end_; ++e) {
		S1pe_method meth = get_method(ch, *e);
		if (Using1pe == meth|| Using1pe2pe == meth || UsingMean == meth) {
			double exp_s1pe = get_S1pe_exp(ch, *e);
			int exp_s1pe_w = get_S1pe_weight_exp(ch, *e);
			if (exp_s1pe_w > 0 && exp_s1pe > 0) {
				val += exp_s1pe_w * exp_s1pe;
				n_used += exp_s1pe_w;
			}
		}
		if (Using2pe == meth || Using1pe2pe == meth) {
			double exp_s2pe = get_S2pe_exp(ch, *e);
			int exp_s2pe_w = get_S2pe_weight_exp(ch, *e);
			if (exp_s2pe_w > 0 && exp_s2pe > 0) {
				val += 0.5*exp_s2pe_w * exp_s2pe;
				n_used += exp_s2pe_w;
			}
		}
	}
	if (n_used) {
		val = val / n_used;
		s1pe_table_.push(ch, V, val, false, false);
	} //do not update otherwise (or calibrations from file will always be overwritten)
	return s1pe_table_.get_S1pe(ch, V);
}

void CalibrationInfo::calculateS1pe(int ch) //for all V present.
{
	std::map<double, bool> Vs;
	for (std::size_t i = 0, i_end_ = state_info->experiments.size(); i != i_end_; ++i) {
		double V = translate_exp_to_V(ch, i);
		Vs[V] = true;
	}
	for (auto i = Vs.begin(), i_end_ = Vs.end(); i != i_end_; ++i)
		calculateS1pe(ch, i->first);
}

void CalibrationInfo::calculateS1pe(void) //for all channels and V.
{
	for (auto ch = state_info->PMT_channels.begin(), ch_end_ = state_info->PMT_channels.end(); ch != ch_end_; ++ch)
		calculateS1pe(*ch);
	for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(); ch != ch_end_; ++ch)
		calculateS1pe(*ch);
}

void CalibrationInfo::Save(std::string fname) const //Adds info to the file, not overrides it entirely
{
	S1pe_table file_table;
	std::ifstream istr;
	istr.open(fname);
	read_file(istr, file_table);
	istr.close();
	for (std::size_t chi = 0, chi_end_ = s1pe_table_.channels_.size(); chi != chi_end_; ++chi)
		for (auto vv = s1pe_table_.s1pe_[chi].begin(), vv_end_ = s1pe_table_.s1pe_[chi].end(); vv != vv_end_; ++vv)
			file_table.push(s1pe_table_.channels_[chi], vv->first, vv->second.second, true, true);
	std::ofstream str;
	open_output_file(fname, str, std::ios_base::trunc);
	if (write_to_file(str, file_table))
		std::cout << "Saved calibration to \"" << fname << "\"" << std::endl;
	else
		std::cout << "Failed to save calibration to \"" << fname << "\"" << std::endl;
	str.close();
}

bool CalibrationInfo::Load(std::string fname)
{
	std::ifstream str;
	str.open(fname);
	bool ret = read_file(str, s1pe_table_);
	if (ret)
		std::cout << "Loaded calibration from \"" << fname << "\"" << std::endl;
	else
		std::cout << "Failed to load calibration from \"" << fname << "\"" << std::endl;
	str.close();
	if (!isFull(false))
		std::cout << "Warning: not all [ch, V] have calibration. Run 'calib_status(true)' to display missing channels and voltages" << std::endl;
	return ret;
}

bool CalibrationInfo::isFull(bool display_bad) const
{
	if (display_bad)
		std::cout << "[ch-V] without calibration: " << std::endl;

	auto process_ch = [&] (int ch) -> bool {
		std::map<double, bool> Vs;
		bool is_first = true;
		for (std::size_t i = 0, i_end_ = state_info->experiments.size(); i != i_end_; ++i) {
			double V = translate_exp_to_V(ch, i);
			Vs[V] = true;
		}
		for (auto vv = Vs.begin(), vv_end_ = Vs.end(); vv != vv_end_; ++vv) {
			if (-1 == get_S1pe(ch, vv->first)) {
				if (display_bad) {
					if (is_first) {
						is_first = false;
						std::cout << "ch " << ch << " V = " << vv->first << "\t";
					} else {
						std::cout << vv->first << "\t";
					}
				} else
					return false;
			}
		}
		if (!is_first)
			std::cout << std::endl;
		return is_first;
	};

	for (auto ch = state_info->PMT_channels.begin(), ch_end_ = state_info->PMT_channels.end(); ch != ch_end_; ++ch) {
		bool is_full = process_ch(*ch);
		if (!display_bad && !is_full)
			return false;
	}
	for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(); ch != ch_end_; ++ch) {
		bool is_full = process_ch(*ch);
		if (!display_bad && !is_full)
			return false;
	}
	return true;
}

void CalibrationInfo::status(bool display_all) const
{
	if (display_all)
		std::cout << "Calibration table: " << std::endl;
	else
		std::cout << "[ch-V] without calibration: " << std::endl;

	auto process_ch = [&](int ch) {
		std::map<double, bool> Vs;
		bool is_first = true;
		for (std::size_t i = 0, i_end_ = state_info->experiments.size(); i != i_end_; ++i) {
			double V = translate_exp_to_V(ch, i);
			Vs[V] = true;
		}
		if (!display_all) {
			for (auto vv = Vs.begin(), vv_end_ = Vs.end(); vv != vv_end_; ++vv) {
				if (-1 == get_S1pe(ch, vv->first)) {
					if (is_first) {
						is_first = false;
						std::cout << "ch " << ch << " V = " << vv->first << "\t";
					} else {
						std::cout << vv->first << "\t";
					}
				}
			}
			if (!is_first)
				std::cout << std::endl;
		} else {
			std::cout << "V =\t";
			for (auto vv = Vs.begin(), vv_end_ = Vs.end(); vv != vv_end_; ++vv)
				std::cout << vv->first << "\t";
			std::cout << std::endl<<"ch "<<ch<<"\t";
			for (auto vv = Vs.begin(), vv_end_ = Vs.end(); vv != vv_end_; ++vv)
				std::cout << get_S1pe(ch, vv->first) << "\t";
			std::cout << std::endl;
		}
	};

	for (auto ch = state_info->PMT_channels.begin(), ch_end_ = state_info->PMT_channels.end(); ch != ch_end_; ++ch) {
		process_ch(*ch);
	}
	for (auto ch = state_info->MPPC_channels.begin(), ch_end_ = state_info->MPPC_channels.end(); ch != ch_end_; ++ch) {
		process_ch(*ch);
	}
}

void CalibrationInfo::set_N_calib(int ch, int from, int to)
{
	set_N_calib(ch, std::pair<int, int>(from, to));
}

void CalibrationInfo::set_N_calib(int channel, std::pair<int, int> val)
{
	int ch_ind = ch_to_index(channel);
	if (ch_ind < 0)
		return;
	N_used_in_calibration[ch_ind] = val;
}

std::pair<int, int> CalibrationInfo::get_N_calib(int ch) const
{
	int ch_ind = ch_to_index(ch);
	if (ch_ind < 0)
		return std::pair<int,int>(-1,-1);
	return N_used_in_calibration[ch_ind];
}
/*
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

	for (int ch_ind = 0; ch_ind < S2_S.back().size(); ++ch_ind) {
		if (s1pe[ch_ind] <= 0)
			continue;
		//calculate N_pe_direct
		for (int exp_ind = 0; exp_ind < S2_S.size(); ++exp_ind) {
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
*/
