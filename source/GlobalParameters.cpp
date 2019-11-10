#include <stdio.h>
#if defined(__WIN32__)
#include <direct.h>
#endif

#include "GlobalParameters.h"
#include "AllRunsResults.h"

//TODO: some functions must be moved to PostProcessor class. (then I won't need the std::vector<double>* get_data methods)

peak::peak() : right(-1), left(-1), S(-1), A(-1)
#ifdef PEAK_AVR_TIME
,t(-1)
#endif
{}

GraphicOutputManager *gr_manager = NULL;
AnalysisManager *manager = NULL;
AllExperimentsResults* g_data = NULL;
PostProcessor* post_processor = NULL;

std::vector<double>::iterator iter_add(std::vector<double>::iterator& to, int what, std::vector<double>::iterator& end)
{
	if (what < 0)
		return end;
	return ((int)(end - to) < what) ? end : to + what;
}

int getIndex(const std::deque<int>& channels, int ch)
{
	for (std::size_t i = 0, i_end_ = channels.size(); i!=i_end_; ++i)
		if (channels[i]==ch)
			return i;
	return -1;
}

int getIndex(const std::vector<int>& channels, int ch)
{
	for (std::size_t i = 0, i_end_ = channels.size(); i!=i_end_; ++i)
		if (channels[i]==ch)
			return i;
	return -1;
}

std::string strtoken(std::string &in, std::string break_symbs)
{
	std::string out;
	while (!in.empty())
	{
		char a = in.front();
		in.erase(in.begin());
		bool break_ = false;
		for (auto h = break_symbs.begin(); h != break_symbs.end(); ++h)
			if (a == *h) {
				break_ = true;
				break;
			}
		if ((break_) && (out.empty()))
			continue;
		if (break_)
			return out;
		out.push_back(a);
	}
	return out;
}

void open_output_file(std::string name, std::ofstream &str, std::ios_base::openmode _mode)
{
	ensure_file(name);
	str.open(name.c_str(), _mode);
	if (!str.is_open()){
		std::cout << "Failed to open \"" << name << "\"" << std::endl;
	}
}

void ensure_file(std::string fname)
{
	std::string folder = fname;
	while ((folder.back() != '\\') &&(folder.back()!='/') &&!folder.empty())
		folder.pop_back();
	if (!folder.empty())
		folder.pop_back();
	ensure_folder(folder);
}

void ensure_folder(std::string folder)
{
#if defined(__WIN32__)
	if (!folder.empty()) {
		DWORD ftyp = GetFileAttributesA(folder.c_str());
		if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY) || ftyp == INVALID_FILE_ATTRIBUTES) {
			int code = system(("mkdir \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir error: " << GetLastError() << std::endl;
		}
	}
#else
	struct stat st;
	if (-1==stat(folder.c_str(), &st)) {
		int err = errno;
		switch (err) {
		case (EACCES): {
			std::cout<<"Access error"<<std::endl;
			break;
		}
		case (ENAMETOOLONG): {
			std::cout<<"Path is too long"<<std::endl;
			break;
		}
		case (ENOENT) :
		case (ENOTDIR): {
			int code = system(("mkdir -p \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir -p error: " << code << std::endl;
			break;
		}
		default:{
			std::cout<<"stat(\""<<folder<<"\") returned -1; errno == "<<err<<std::endl;
			break;
		}
		}
	} else {
		if (!S_ISDIR(st.st_mode)) {
			int code = system(("mkdir -p \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir -p error: " << code << std::endl;
		}
	}
#endif //_WIN32__
}

bool confirm_action (std::string action)
{
	std::cout<<"Confirm "<<action<<": y/n"<<std::endl;
	std::string a;
	std::cin>>a;
	if (a=="y"||a=="Y"||a=="yes"||a=="Yes") {
		return true;
	}
	std::cout<<action<<" denied"<<std::endl;
	return false;
}

template <class T>
channel_info<T>::channel_info()
{}
template <class T>
channel_info<T>::~channel_info()
{}
template <class T>
template <class U>
bool channel_info<T>::isSameChannels(const channel_info<U>& b) const
{
	if (_data.size() != b._data.size())
		return false;
	else {
		for (std::size_t ind = 0, ind_end_ = _data.size(); ind < ind_end_; ++ind)
			if (_data[ind].first != b._data[ind].second)
				return false;
	}
	return true;
}
template <class T>
bool channel_info<T>::isSameChannels(const std::deque<int>& channels) const
{
	if (_data.size() != channels.size())
		return false;
	else {
		for (std::size_t ind = 0, ind_end_ = _data.size(); ind < ind_end_; ++ind)
			if (_data[ind].first != channels[ind])
				return false;
	}
	return true;
}
template <class T>
void channel_info<T>::push(const int& channel, const T& data) //preserves channel sorting
{
	std::size_t sz = _data.size();
	if (0 == sz) {
		_data.push_back(std::pair<int, T>(channel, data));
		return;
	}
	if (channel < _data.front().first) {
		_data.insert(_data.begin(), std::pair<int, T>(channel, data));
		return;
	}
	if (channel > _data.back().first) {
		_data.push_back(std::pair<int, T>(channel, data));
		return;
	}
	std::pair<std::size_t, std::size_t> inds = get_bounds(channel);
	if (inds.first == inds.second) //do not insert points with equal channel, replace only
		_data[inds.first].second = data;
	else
		_data.insert(_data.begin() + inds.second, std::pair<int, T>(channel, data));
}
template <class T>
void channel_info<T>::push_back(const int& channel, const T& data) //ignores channel sorting
{
	_data.push_back(std::pair<int, T>(channel, data));
}
template <class T>
T* channel_info<T>::info(const int& channel)
{
	std::size_t ch_ind = get_index(channel);
	if (std::numeric_limits<std::size_t>::max() == ch_ind) {
		return NULL;
	}
	return &(_data[ch_ind].second);
}
template <class T>
T& channel_info<T>::operator [] (const std::size_t& ch_ind)
{
	std::size_t sz = _data.size();
	if (!(ch_ind < sz))
		throw std::out_of_range("channel_info<T>::operator[] index is out of range");
	return _data[ch_ind].second;
}
template <class T>
void channel_info<T>::erase(const int& channel)
{
	std::size_t ch_ind = get_index(channel);
	if (std::numeric_limits<std::size_t>::max()==ch_ind){
		std::cout << "channel_info<T>::erase: Warning! Does not contain channel " << channel << std::endl;
		return;
	}
	erase_at(ch_ind);
}
template <class T>
void channel_info<T>::erase_at(const std::size_t& ch_ind)
{
	std::size_t sz = _data.size();
	if (!(ch_ind < sz))
		throw std::out_of_range("channel_info<T>::erase_at index is out of range");
	_data.erase(data.begin() + ch_ind);
}
template <class T>
void channel_info<T>::clear(void)
{
	_data.clear();
}
template <class T>
std::size_t channel_info<T>::size(void) const
{
	return _data.size();
}
template <class T>
bool channel_info<T>::empty(void) const 
{
	return _data.empty();
}
template <class T>
bool channel_info<T>::contains(const int& channel) const
{
	if (std::numeric_limits<std::size_t>::max() != get_index(channel))
		return true;
	return false;
}
template <class T>
std::size_t channel_info<T>::get_index(const int& channel) const
{
	std::size_t sz = _data.size();
	if (0 == sz) {
		return std::numeric_limits<std::size_t>::max();
	}
	if (channel < _data.front().first) {
		return std::numeric_limits<std::size_t>::max();
	}
	if (channel > _data.back().first) {
		return std::numeric_limits<std::size_t>::max();
	}
	std::pair<std::size_t, std::size_t> inds = get_bounds(channel);
	if (inds.first == inds.second)
		return inds.first;
	else
		return std::numeric_limits<std::size_t>::max();
}
template <class T>
int channel_info<T>::channel(const std::size_t& ch_ind) const
{
	std::size_t sz = _data.size();
	if (!(ch_ind < sz))
		throw std::out_of_range("channel_info<T>::channel index is out of range");
	return _data[ch_ind].first;
}
template <class T>
std::pair<std::size_t, std::size_t> channel_info<T>::get_bounds(const int& channel) const
{
	std::pair<std::size_t, std::size_t> out(std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max());
	std::size_t sz = _data.size();
	if (0 == sz)
		return out;
	if (channel <= _data.front().first) {
		out = std::pair<std::size_t, std::size_t>(0, 0);
		return out;
	}
	if (channel >= _data.back().first) {
		out = std::pair<std::size_t, std::size_t>(sz - 1, sz - 1);
		return out;
	}
	//find first ch which is not less that channel. That is index bounding X_point: chs[first] <= channel < chs[first + 1]
	//See std::lower_bound and std::upper_bound:
	//std::lower_bound(xys.begin(), xys.end(), [](const std::pair<double, double> &a, const std::pair<double, double> &b)->bool{
	//	return a.first<b.first;
	//});
	std::size_t count = sz;
	std::size_t first = 0;
	while (count > 0) {
		std::size_t step = count / 2;
		std::size_t ind = first + step;
		if (!(channel < _data[ind].first)) {
			first = ++ind;
			count -= step + 1;
		} else
			count = step;
	}
	//first is such, that channel>=chs[first-1] and channel<chs[first]
	//first always != 0 here
	--first;
	if (channel == _data[first].first) {
		out = std::pair<std::size_t, std::size_t>(first, first);
		return out;
	}
	out = std::pair<std::size_t, std::size_t>(first, first + 1);
	return out;
}
template <class T>
void channel_info<T>::sort(void)
{
	std::sort(_data.begin(), _data.end(), [](const std::pair<int, T>& a, const std::pair<int, T>& b)->bool {
		return a.first < b.first;
	});
}
template <class T>
bool channel_info<T>::is_sorted(void) const
{
	for (std::size_t ind = 0, ind_end_ = _data.size(); (ind + 1) != ind_end_; ++ind)
		if (_data[ind].first >= _data[ind+1].first)
			return false;
	return true;
}

TestSignalGenerator::TestSignalGenerator(std::string prefix)
{
	double t_from = 0, t_to = 110;
	std::vector<std::string> exp0 = { "10V_exp", "15V_exp" };
	std::vector<int> chs = { 2, 3, 4, 5, 6 }; //PMTs
	for (int i = 32; i <= 48; chs.push_back(i++)); //MPPCs
	TRandom1 *rand = new TRandom1(42);
	boost::random::hellekalek1995 rand2;
	//ROOT::Math::TRandomEngine rand2; - root v6 on windows doest not have GSL libraries. v5 does though.
	std::vector<std::vector<double> > Nphe_signal_avr;
	std::vector<std::vector<double> > Nphe_noise_avr;
	std::vector<std::vector<double> > signal_time;
	for (std::size_t e = 0, e_end_ = exp0.size(); e != e_end_; ++e) { //set expected signal and noise values for each exp and channel
		Nphe_signal_avr.push_back(std::vector<double>());
		Nphe_noise_avr.push_back(std::vector<double>());
		signal_time.push_back(std::vector<double>());
		for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
			if (chs[c] < 6) {
				Nphe_signal_avr[e].push_back(rand->Poisson(12.5 *(1 + e / 4)));
				Nphe_noise_avr[e].push_back(rand->Poisson(2.5));
				signal_time[e].push_back(40 * (1 + e / 4));
				continue;
			}
			if (chs[c] == 6) {
				Nphe_signal_avr[e].push_back(10 * (1 + e / 4));
				Nphe_noise_avr[e].push_back(4);
				signal_time[e].push_back(43 * (1 + e / 4));
				continue;
			}
			Nphe_signal_avr[e].push_back(rand->Poisson((15 + chs[c] / 10))*(1 + e / 3));
			Nphe_noise_avr[e].push_back(rand->Poisson(3 + chs[c] / 30));
			signal_time[e].push_back(30 * (1 + e / 4));
		}
	}
	//peak parameters (A, S, 1phe-2phe distr, signal form, dt peak)
	//per channel. See their meaning in the generation
	std::vector<double> dts(chs.size());
	std::vector<double> dts_sigma(chs.size());
	std::vector<std::pair<double, double> > dts_mm(chs.size());
	std::vector<double> SoA(chs.size()); //not precise, sigma is linear from the smallest S to the largest ones
	std::vector<double> SoA_sigma(chs.size()); //maximum value
	std::vector<double> S1phe(chs.size()); //S1phe is distributed according to gamma. This value is mean a= k= 2.3
	std::vector<boost::random::gamma_distribution<double> > gamma_generators; //each channel has different, but fixed parameters.
	std::vector<double> signal_time_sigma(chs.size()); //noise sigma is fixed at 30.
	std::vector<double> Nphe(chs.size()); //poisson distribution over number of 1phe peak, 2phe peak ...
	for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
		if (chs[c] < 6) {
			dts[c] = std::fabs(rand->Gaus(1.0, 0.1));
			dts_sigma[c] = 0.3;
			dts_mm[c] = std::pair<double, double>(0.3, 2.0);
			SoA[c] = std::fabs(rand->Gaus(1.0, 0.1));
			SoA_sigma[c] = 0.08;
			S1phe[c] = 6;
			gamma_generators.push_back(boost::random::gamma_distribution<double>(2.3, S1phe[c]/2.3)); //(k, theta) E[x] = k*th == S1phe[c]
			signal_time_sigma[c] = 12;
			Nphe[c] = 0.25;
			continue;
		}
		if (chs[c] == 6) {
			dts[c] = std::fabs(rand->Gaus(1.0, 0.1));
			dts_sigma[c] = 0.3;
			dts_mm[c] = std::pair<double, double>(0.3, 2.0);
			SoA[c] = std::fabs(rand->Gaus(1.0, 0.1));
			SoA_sigma[c] = 0.1;
			S1phe[c] = 5;
			gamma_generators.push_back(boost::random::gamma_distribution<double>(2.3, S1phe[c]/2.3)); //(k, theta) E[x] = k*th == S1phe[c]
			signal_time_sigma[c] = 14;
			Nphe[c] = 0.10;
			continue;
		}
		dts[c] = std::fabs(rand->Gaus(0.4, 0.06));
		dts_sigma[c] = 0.2;
		dts_mm[c] = std::pair<double, double>(0.01, 1.0);
		SoA[c] = (1 + rand->Poisson(5.0)) / 6;
		SoA_sigma[c] = (1 + rand->Poisson(5.0)) / 200;
		S1phe[c] = 6;
		gamma_generators.push_back(boost::random::gamma_distribution<double>(2.3, S1phe[c]/2.3)); //(k, theta) E[x] = k*th == S1phe[c]
		signal_time_sigma[c] = 10;
		Nphe[c] = 0.35;
	}
	//generate events into memory
	experiment_area area;
	std::deque<AllRunsResults> data;
	for (std::size_t e = 0, e_end_ = exp0.size(); e != e_end_; ++e) {
		data.push_back(AllRunsResults(area));
		std::size_t event_end_ = (e == 0 ? 21000 : 30000);
		data.back()._valid.resize(event_end_, true);
		for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
			if (chs[c] < 32) {
				data.back().pmt_channels.push_back(chs[c]);
				data.back().pmt_peaks.push_back(std::deque<std::deque<peak> >(event_end_));
				data.back().pmt_S2_integral.push_back(std::vector<double>(event_end_));
			} else {
				data.back().mppc_channels.push_back(chs[c]);
				data.back().mppc_peaks.push_back(std::deque<std::deque<peak> >(event_end_));
				data.back().mppc_Double_Is.push_back(std::vector<double>(event_end_));
			}
		}
		for (std::size_t event = 0; event != event_end_; ++event) {
			bool is_noise = (rand->Uniform() < 0.4);
			bool is_gaus_noise = (rand->Uniform() < 0.3); //time distribution of noise
			std::size_t mppc_index = 0;
			std::size_t pmt_index = 0;
			for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
				int Nphe_ = (is_noise ? rand->Poisson(Nphe_noise_avr[e][c]) : rand->Poisson(Nphe_signal_avr[e][c]));
				bool is_pmt = (chs[c] < 32);
				for (int phe = 0; phe <= Nphe_; ++phe) {
					peak pk;
					pk.t = (is_noise ? (is_gaus_noise ? rand->Gaus(signal_time[e][c], 30) : t_from + rand->Uniform()*(t_to - t_from))
						: rand->Gaus(signal_time[e][c], signal_time_sigma[c]));
					if (pk.t > t_to || pk.t < t_from)
						continue;
					double dt = rand->Gaus(dts[c], dts_sigma[c]);
					pk.left = std::max(pk.t - 0.5*dt, t_from);
					pk.right = std::min(pk.t + 0.5*dt, t_to);
					dt = pk.right - pk.left;
					if (dt > dts_mm[c].second || dt < dts_mm[c].first)
						continue;
					double SoA_ = rand->Gaus(SoA[c], (dt - dts_mm[c].first) *SoA_sigma[c] / (dts_mm[c].second - dts_mm[c].first)); //signal shape factor.
					pk.A = gamma_generators[c](rand2);
					//pk.A = rand2.Gamma(2.3, 2.3 / S1phe[c]);
					pk.S = pk.A*SoA_*dt;
					int Nphe_in_peak = std::max(1 + rand->Poisson(Nphe[c]), 4);
					pk.A *= Nphe_in_peak;
					pk.S *= Nphe_in_peak;
					if (is_pmt)
						data.back().pmt_peaks[pmt_index][event].push_back(pk);
					else
						data.back().mppc_peaks[mppc_index][event].push_back(pk);
				}
				double t_zoom_from = 25, t_zoom_to = 45;
				double Ssum = 0, dt_avr = 0; //average distance between peaks inside zoom;
				unsigned int Npks = 0;
				std::deque<peak> peaks = (is_pmt ? data.back().pmt_peaks[pmt_index][event] : data.back().mppc_peaks[mppc_index][event]);
				std::vector<double> ts;
				for (auto p = peaks.begin(), p_end_ = peaks.end(); p != p_end_; ++p) {
					if (p->t<t_zoom_to && p->t>t_zoom_from) {
						++Npks;
						Ssum += p->S;
						ts.push_back(p->t);
					}
				}
				std::sort(ts.begin(), ts.end());
				for (std::size_t t = 0, t_end_ = ts.size(); t != t_end_; ++t) {
					if (t != 0)
						dt_avr += ts[t] - ts[t - 1];
				}
				dt_avr = (Npks <= 1 ? 0 : dt_avr / (Npks - 1));
				double F = 20 / 15.0 / dt_avr;
				F = F * 1 / (1 + std::exp((F - 0.8) / 0.02));
				F = F * 1 / (1 + std::exp(-1 * (F - 0.1) / 0.02));
				//F is between ~0.1 and ~0.8. dt large - dt small
				if (is_pmt) {
					data.back().pmt_S2_integral[pmt_index][event] = Ssum* (1.1 - std::pow(F, 0.8));
				} else {
					data.back().mppc_Double_Is[mppc_index][event] = Ssum* (1.1 - std::pow(F, 0.4)) *
						(ts.size() > 1 ? (ts.back() - ts.front()) : 0);
				}
				if (is_pmt)
					++pmt_index;
				else
					++mppc_index;
			}
		}
	}
	//write events as in Data_processing 
	for (std::size_t e = 0, e_end_ = exp0.size(); e != e_end_; ++e) {
		data[e]._exp.experiments.push_back(exp0[e]);
		data[e].SaveTo(prefix);
	}
}

void DrawFileData(std::string name, std::vector<double> xs, std::vector<double> ys,/* ParameterPile::*/DrawEngine de)
{
	if (xs.size() != ys.size()){
		std::cout << "DrawFileData::input data size mismatch" << std::endl;
		return;
	}
	if (de == /*ParameterPile::*/DrawEngine::ROOT_){
		double *xxxs = new double[xs.size()];
		double *yyys = new double[ys.size()];
		for (int h = 0; h < xs.size(); h++){
			xxxs[h] = xs.at(h);
			yyys[h] = ys.at(h);
		}
		TGraph* gr = new TGraph(xs.size(), xxxs, yyys);
		TCanvas* can = new TCanvas(name.c_str(), name.c_str());
		can->SetWindowPosition(100, 150);
		can->Update();
		gr->Draw();
		delete[] xxxs;
		delete[] yyys;
	} else {
		std::string mod_name = name;
		for (int s = 0; s < mod_name.size(); s++)
			if (mod_name[s] == '\\' || mod_name[s] == '/')
				mod_name[s] = '.';
		std::ofstream file;
		open_output_file("temp_gnuplot_files/" + mod_name, file);
		std::cout << "file " << "temp_gnuplot_files/" + mod_name << ".is_open() " << file.is_open() << std::endl;
#if defined(__WIN32__)
		if (!file.is_open())
			std::cout << GetLastError() << std::endl;
#endif //__WIN32__
		for (int h = 0; h < xs.size(); h++)
			file << xs[h] << '\t' << ys[h] << std::endl;
		file.close();
		open_output_file("temp_gnuplot_files/script.sc", file);
		file << "plot '" << /*ParameterPile::*/this_path + "/temp_gnuplot_files/" + mod_name << "' u 1:2" << std::endl;
		file << "pause -1";
		file.close();
		INVOKE_GNUPLOT(this_path + "/temp_gnuplot_files/script.sc");
	}
}

viewRegion::viewRegion(double x_min, double y_min, double x_max, double y_max) //constructs rectangular view region
{
	view_xs.push_back(x_min);
	view_ys.push_back(y_min);
	view_xs.push_back(x_min);
	view_ys.push_back(y_max);
	view_xs.push_back(x_max);
	view_ys.push_back(y_max);
	view_xs.push_back(x_max);
	view_ys.push_back(y_min);
}

viewRegion::viewRegion(std::vector<double> &xs, std::vector<double> &ys) //constructs rectangular from polyline defined by xs,ys
{
	if (xs.size() != ys.size()) {
		std::cout << "viewRegion::viewRegion: Warning x-y size mismatch, points are ignored";
		return;
	}
	view_xs = xs;
	view_ys = ys;
}

viewRegion::viewRegion() //no view region
{}

void viewRegion::view_push(double x, double y)
{
	view_xs.push_back(x);
	view_ys.push_back(y);
}

bool viewRegion::get_view(std::size_t index, double &x, double &y) const
{
	if (index >= view_xs.size())
		return false;
	x = view_xs[index];
	y = view_ys[index];
	return true;
}

bool viewRegion::set_view(std::size_t index, double x, double y)
{
	if (index >= view_xs.size()) {
		view_xs.resize(index+1, x);
		view_ys.resize(index+1, y);
		return true;
	}
	view_xs[index] = x;
	view_ys[index] = y;
	return true;
}

std::size_t viewRegion::get_view_size(void) const
{
	return view_xs.size();
}

void viewRegion::polyline_push(double x, double y)
{
	line_xs.push_back(x);
	line_ys.push_back(y);
}

bool viewRegion::get_polyline(std::size_t ind, double &x, double &y) const
{
	if (ind >= line_xs.size())
		return false;
	x = line_xs[ind];
	y = line_ys[ind];
	return true;
}

bool viewRegion::set_polyline(std::size_t ind, double x, double y)
{
	if (ind >= line_xs.size()) {
		line_xs.resize(ind + 1, x);
		line_ys.resize(ind + 1, y);
		return true;
	}
	line_xs[ind] = x;
	line_ys[ind] = y;
	return true;
}

void viewRegion::set_polyline(const std::vector<double> &x, const std::vector<double> &y)
{
	if (x.size() != y.size()) {
		std::cout << "viewRegion::set_polyline: Warning x-y size mismatch, points are ignored";
		return;
	}
	line_xs = x;
	line_ys = y;
}

std::size_t viewRegion::get_polyline_size(void) const
{
	return line_xs.size();
}

TPolyLine *viewRegion::get_clipped_polyline(void) const
{
	std::vector<double> clipped_xs, clipped_ys;
	ClipToView(line_xs, line_ys, clipped_xs, clipped_ys);
	TPolyLine* ret = new TPolyLine(clipped_xs.size(), &clipped_xs[0], &clipped_ys[0]);
	return ret;
}

void viewRegion::clear_polyline(void)
{
	line_xs.clear();
	line_ys.clear();
}

//0 - not crossed, 1 - crossed, 2 - crossed at ends
int viewRegion::LinesCrossed(double x1, double y1, double x2, double y2,
	double u1, double v1, double u2, double v2, double& xc, double& yc)
{
	//check infinity points: //TODO: write all cases properly
	double finite_x_max = 0, finite_y_max = 0;
	finite_x_max = (x1!=DBL_MAX && x1!=-DBL_MAX ? std::max(std::fabs(x1), finite_x_max) : finite_x_max);
	finite_x_max = (x2!=DBL_MAX && x2!=-DBL_MAX ? std::max(std::fabs(x2), finite_x_max) : finite_x_max);
	finite_x_max = (u1!=DBL_MAX && u1!=-DBL_MAX ? std::max(std::fabs(u1), finite_x_max) : finite_x_max);
	finite_x_max = (u2!=DBL_MAX && u2!=-DBL_MAX ? std::max(std::fabs(u2), finite_x_max) : finite_x_max);
	finite_y_max = (y1!=DBL_MAX && y1!=-DBL_MAX ? std::max(std::fabs(y1), finite_y_max) : finite_y_max);
	finite_y_max = (y2!=DBL_MAX && y2!=-DBL_MAX ? std::max(std::fabs(y2), finite_y_max) : finite_y_max);
	finite_y_max = (v1!=DBL_MAX && v1!=-DBL_MAX ? std::max(std::fabs(v1), finite_y_max) : finite_y_max);
	finite_y_max = (v2!=DBL_MAX && v2!=-DBL_MAX ? std::max(std::fabs(v2), finite_y_max) : finite_y_max);

	if ((x1==DBL_MAX)&&(y1==DBL_MAX))
		return 0;
	if (x1==DBL_MAX) {
		x1 = finite_x_max;
		y1 = y2;
	}
	if (x2==DBL_MAX) {
		x2 = finite_x_max;
		y2 = y1;
	}
	if (x1==-DBL_MAX) {
		x1 = -finite_x_max;
		y1 = y2;
	}
	if (x2==-DBL_MAX) {
		x2 = -finite_x_max;
		y2 = y1;
	}
	if (y1==DBL_MAX) {
		y1 = finite_y_max;
		x1 = x2;
	}
	if (y2==DBL_MAX) {
		y2 = finite_y_max;
		x2 = x1;
	}
	if (y1==-DBL_MAX) {
		y1 = -finite_y_max;
		x1 = x2;
	}
	if (y2==-DBL_MAX) {
		y2 = -finite_y_max;
		x2 = x1;
	}

	if (u1==DBL_MAX) {
		u1 = finite_x_max;
		v1 = v2;
	}
	if (u2==DBL_MAX) {
		u2 = finite_x_max;
		v2 = v1;
	}
	if (u1==-DBL_MAX) {
		u1 = -finite_x_max;
		v1 = v2;
	}
	if (u2==-DBL_MAX) {
		u2 = -finite_x_max;
		v2 = v1;
	}
	if (v1==DBL_MAX) {
		v1 = finite_y_max;
		u1 = u2;
	}
	if (v2==DBL_MAX) {
		v2 = finite_y_max;
		u2 = u1;
	}
	if (v1==-DBL_MAX) {
		v1 = -finite_y_max;
		u1 = u2;
	}
	if (v2==-DBL_MAX) {
		v2 = -finite_y_max;
		u2 = u1;
	}
	// Set the tolerances.
	double xtol =
		1.0e-10 *
		std::max(std::abs(x1),
			std::max(std::abs(x2), std::max(std::abs(u1), std::abs(u2))));
	double ytol =
		1.0e-10 *
		std::max(std::abs(y1),
			std::max(std::abs(y2), std::max(std::abs(v1), std::abs(v2))));
	if (xtol <= 0) xtol = 1.0e-10;
	if (ytol <= 0) ytol = 1.0e-10;

	// Compute the distances and determinant (dx,dy) x (du,dv).
	double dy = y2 - y1;
	double dv = v2 - v1;
	double dx = x1 - x2;
	double du = u1 - u2;
	double det = dy * du - dx * dv;

	// Check for crossing because one of the endpoints is on both lines.
	if (OnLine(x1, y1, x2, y2, u1, v1)) {
		xc = u1;
		yc = v1;
		return 2;
	}
	else if (OnLine(x1, y1, x2, y2, u2, v2)) {
		xc = u2;
		yc = v2;
		return 2;
	}
	else if (OnLine(u1, v1, u2, v2, x1, y1)) {
		xc = x1;
		yc = y1;
		return 2;
	}
	else if (OnLine(u1, v1, u2, v2, x2, y2)) {
		xc = x2;
		yc = y2;
		return 2;
	}
	// Check if the lines are parallel (zero determinant).
	else if (std::abs(det) < xtol * ytol)
		return 0;
	// No special case: compute point of intersection.
	else {

		// Solve crossing equations.
		xc = (du * (x1 * y2 - x2 * y1) - dx * (u1 * v2 - u2 * v1)) / det;
		yc = ((-1 * dv) * (x1 * y2 - x2 * y1) + dy * (u1 * v2 - u2 * v1)) / det;

		// Determine if this point is on both lines.
		if (OnLine(x1, y1, x2, y2, xc, yc) && OnLine(u1, v1, u2, v2, xc, yc))
			return 1;
	}
	// The lines do not cross if we have reached this point.
	return 0;
}

// Ported from Garfield (method GRCONV):
// Clip the specified polygon to the view region; return the clipped polygon.
// px: the x-vertices of the polygon
// py: the y-vertices of the polygon
// cx: to contain the x-vertices of the clipped polygon
// cy: to contain the y-vertices of the clipped polygon
void viewRegion::ClipToView(const std::vector<double>& px, const std::vector<double>& py,
                            std::vector<double>& cx, std::vector<double>& cy) const {

  // Get the number and coordinates of the polygon vertices.
  int pN = (int)px.size();

  // Clear the vectors to contain the final polygon.
  cx.clear();
  cy.clear();

  // Set up the view vertices.
  std::vector<double> vx = view_xs, vy = view_ys;

  int vN = (int)vx.size();

  // Do nothing if we have less than 2 points.
  if (pN < 2) return;

  // Loop over the polygon vertices. (Omit last index to avoid unnecessary loop)
  for (int i = 0; i < pN; ++i) {
    std::vector <double> tx, ty;
    // Loop over the view vertices.
    for (int j = 0; j < vN; ++j) {

      // Determine whether this vertex lies on a view edge:
      //  if so add the vertex to the final polygon.
      if (OnLine(vx[j % vN], vy[j % vN], vx[(j + 1) % vN], vy[(j + 1) % vN],
                 px[i], py[i])) {
        // Add the vertex.
        tx.push_back(px[i]);
        ty.push_back(py[i]);
      }

      // Determine whether a corner of the view area lies on this edge:
      //  if so add the corner to the final polygon.
      if (OnLine(px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN],
                 vx[j], vy[j])) {

        // Add the vertex.
        tx.push_back(vx[j]);
        ty.push_back(vy[j]);
      }
    }

    // Loop over the view vertices.
    for (int j = 0; (j < vN)&&(i!=pN-1); ++j) {
		// Check for a crossing with this edge;
		//  if one exists, add the crossing point.
		double xc = 0., yc = 0.;
		if (LinesCrossed(vx[j % vN], vy[j % vN], vx[(j + 1) % vN],
						 vy[(j + 1) % vN], px[i % pN], py[i % pN],
						 px[(i + 1) % pN], py[(i + 1) % pN], xc, yc)) {

		  // Add a vertex.
		  tx.push_back(xc);
		  ty.push_back(yc);
		}
	}
    // Test whether this vertex is inside the view polygon.
   //  If so, add it to the final line.
    bool edge = false;
    if (IsInPolygon(px[i], py[i], vx, vy, edge)) {
	  // Add the view vertex.
	  tx.push_back(px[i]);
	  ty.push_back(py[i]);
	}

    //in case several points are added to tx ty there is need to sort them according to p[j] pv[j+1]
    if (std::fabs(px[(i + 1) % vN] - px[i % vN]) > std::fabs(py[(i + 1) % vN] - py[i % vN])) {
		bool direction = px[(i + 1) % vN] > px[i % vN];
		while (tx.size()!=0) {
			std::size_t first_point = 0;
			for (std::size_t b=0, b_end_ = tx.size(); b!=b_end_; ++b) { //can modify it to splitting polyline into several
				if ((direction && tx[first_point] > tx[b])||(!direction && tx[first_point] < tx[b]))
					first_point = b;
			}
			cx.push_back(tx[first_point]);
			cy.push_back(ty[first_point]);
			tx.erase(tx.begin()+first_point);
			ty.erase(ty.begin()+first_point);
		}
    } else {
    	bool direction = py[(i + 1) % vN] > py[i % vN];
		while (tx.size()!=0) {
			std::size_t first_point = 0;
			for (std::size_t b=0, b_end_ = tx.size(); b!=b_end_; ++b) { //can modify it to splitting polyline into several
				if ((direction && ty[first_point] > ty[b])||(!direction && ty[first_point] < ty[b]))
					first_point = b;
			}
			cx.push_back(tx[first_point]);
			cy.push_back(ty[first_point]);
			tx.erase(tx.begin()+first_point);
			ty.erase(ty.begin()+first_point);
		}
    }
  }
}

// Ported from Garfield (function INTERD):
// Returns true if the point (x,y) is inside of the specified polygon.
// x: the x-coordinate
// y: the y-coordinate
// px: the x-vertices of the polygon
// py: the y-vertices of the polygon
// edge: a variable set to true if the point is located on the polygon edge
bool viewRegion::IsInPolygon(double x, double y, const std::vector<double>& px,
                             const std::vector<double>& py, bool& edge) {

  // Get the number and coordinates of the polygon vertices.
  int pN = (int)px.size();

  // Handle the special case of less than 2 vertices.
  if (pN < 2) return false;
  // Handle the special case of exactly 2 vertices (a line).
  if (pN == 2) return OnLine(px[0], py[0], px[1], py[1], x, y);

  // Set the minimum and maximum coordinates of all polygon vertices.
  double px_min = px[0], py_min = py[0];
  double px_max = px[0], py_max = py[0];
  for (int i = 0; i < pN; ++i) {
    px_min = std::min(px_min, px[i]);
    py_min = std::min(py_min, py[i]);
    px_max = std::max(px_max, px[i]);
    py_max = std::max(py_max, py[i]);
  }

  // Set the tolerances
  double xtol = 1.0e-10 * std::max(std::abs(px_min), std::abs(px_max));
  double ytol = 1.0e-10 * std::max(std::abs(py_min), std::abs(py_max));
  if (xtol <= 0) xtol = 1.0e-10;
  if (ytol <= 0) ytol = 1.0e-10;

  // If we have essentially one x value, check to see if y is in range.
  if (std::abs(px_max - px_min) < xtol) {
    edge = (y > (py_min - ytol) && y < (py_max + ytol) &&
            std::abs(px_max + px_min - 2 * x) < xtol);
    return false;
  }
  // If we have essentially one y value, check to see if x is in range.
  if (std::abs(py_max - py_min) < ytol) {
    edge = (x > (px_min - xtol) && x < (px_max + xtol) &&
            std::abs(py_max + py_min - 2 * y) < ytol);
    return false;
  }

  // Set "infinity" points.
  double xinf = px_min - std::abs(px_max - px_min);
  double yinf = py_min - std::abs(py_max - py_min);

  // Loop until successful or maximum iterations (100) reached.
  int niter = 0;
  bool done = false;
  int ncross = 0;

  TRandom *rand = new TRandom1();
  while (!done && niter < 100) {

    // Assume we will finish on this loop.
    done = true;

    // Loop over all edges, counting the number of edges crossed by a line
    //  extending from (x,y) to (xinf,yinf).
    ncross = 0;
    for (int i = 0; (done && i < pN); ++i) {

      // Determine whether the point lies on the edge.
      if (OnLine(px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN], x,
                 y)) {

        edge = true;
        return false;
      }

      // Determine whether this edge is crossed; if so increment the counter.
      double xc = 0., yc = 0.;
      if (LinesCrossed(x, y, xinf, yinf, px[i % pN], py[i % pN],
                       px[(i + 1) % pN], py[(i + 1) % pN], xc, yc))
        ++ncross;

      // Ensure this vertex is not crossed by the line from (x,y)
      //  to (xinf,yinf); if so recompute (xinf,yinf) and start over.
      if (OnLine(x, y, xinf, yinf, px[i], py[i])) {

        // Recompute (xinf,yinf).
		xinf = px_min - rand->Uniform() * std::abs(px_max - xinf);
        yinf = py_min - rand->Uniform() * std::abs(py_max - yinf);

        // Start over.
        done = false;
        ++niter;
      }
    }
  }

  // If we failed to finish iterating, return false.
  if (niter >= 100) {
    std::cerr <<"viewRegion::IsInPolygon: unable to determine whether ("
              << x << ", " << y << ") is inside a polygon.  Returning false.\n";
    return false;
  }

  // Point is inside for an odd, nonzero number of crossings.
  return (ncross != 2 * (ncross / 2));
}

//
// Determines whether the point (u,v) lies on the line connecting
//  points (x1,y1) and (x2,y2).
//
// Ported from Garfield function ONLIND
//
bool viewRegion::OnLine(double x1, double y1, double x2, double y2, double u,
                        double v) {
  // Set the tolerances
  double xtol =
      1.0e-10 * std::max(std::abs(x1), std::max(std::abs(x2), std::abs(u)));
  double ytol =
      1.0e-10 * std::max(std::abs(y1), std::max(std::abs(y2), std::abs(v)));
  if (xtol <= 0) xtol = 1.0e-10;
  if (ytol <= 0) ytol = 1.0e-10;

  // To store the coordinates of the comparison point
  double xc = 0, yc = 0;

  // Check if (u,v) is the point (x1,y1) or (x2,y2)
  if ((std::abs(x1 - u) <= xtol && std::abs(y1 - v) <= ytol) ||
      (std::abs(x2 - u) <= xtol && std::abs(y2 - v) <= ytol)) {
    return true;
  }
  // Check if the line is actually a point
  else if (std::abs(x1 - x2) <= xtol && std::abs(y1 - y2) <= ytol) {
    return false;
  }
  // Choose (x1,y1) as starting point if closer to (u,v)
  else if (std::abs(u - x1) + std::abs(v - y1) <
           std::abs(u - x2) + std::abs(v - y2)) {

    // Compute the component of the line from (x1,y1) to (u,v)
    //  along the line from (x1,y1) to (x2,y2)
    double dpar = ((u - x1) * (x2 - x1) + (v - y1) * (y2 - y1)) /
                  ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    // Determine the point on the line to which to compare (u,v)
    if (dpar < 0.0) {
      xc = x1;
      yc = y1;
    } else if (dpar > 1.0) {
      xc = x2;
      yc = y2;
    } else {
      xc = x1 + dpar * (x2 - x1);
      yc = y1 + dpar * (y2 - y1);
    }
  }
  // Choose (x2,y2) as starting point if closer to (u,v)
  else {

    // Compute the component of the line from (x2,y2) to (u,v)
    //  along the line from (x2,y2) to (x1,y1)
    double dpar = ((u - x2) * (x1 - x2) + (v - y2) * (y1 - y2)) /
                  ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    // Determine the point on the line to which to compare (u,v)
    if (dpar < 0.0) {
      xc = x2;
      yc = y2;
    } else if (dpar > 1.0) {
      xc = x1;
      yc = y1;
    } else {
      xc = x2 + dpar * (x1 - x2);
      yc = y2 + dpar * (y1 - y2);
    }
  }

  // Compare the calculated point to (u,v)
  if (std::abs(u - xc) < xtol && std::abs(v - yc) < ytol) return true;

  return false;
}

//namespace ParameterPile
//{
	std::deque <experiment_area> areas_to_draw;
	std::string this_path;

	std::string data_prefix_path;
	std::string calibration_file;
	std::string data_output_path;

	std::string DATA_MPPC_VERSION;
	std::string DATA_PMT_VERSION;

	std::string OUTPUT_MPPCS_PICS;
	std::string OUTPUT_PMT_PICS;
	std::string OUTPUT_MPPCS;

	experiment_area exp_area;
	int threads_number = 6; //obv. must be >=1

	int gnuplot_pad_size = 400;
	int gnuplot_max_size = 1600;
	int gnuplot_width = 900; //default for gnuplot is 640

	std::map < std::string, double > experiment_fields;
	std::map < std::string, double > PMT_V;
	std::map < std::string, double > MPPC_V;
	std::map < std::string, double > PMT_dB;
	std::pair<int, int> calibaration_points;
	std::map < int, std::pair<double,double> > MPPC_coords;

	Bool_t draw_required(/*ParameterPile::*/experiment_area what)
	{
		for (auto i = areas_to_draw.begin(); i != areas_to_draw.end(); ++i)
			if (i->contains(what))
				return kTRUE;
		return kFALSE;
	}

	void Init_globals(bool full)
	{
		char path[FILENAME_MAX];
#if defined(__WIN32__)
		this_path = _getcwd(path, FILENAME_MAX);
#else
		this_path = getcwd(path, FILENAME_MAX);
#endif //__WIN32__
		TThread::Initialize();
		gStyle->SetOptFit();
		gStyle->SetStatY(0.9);
		gStyle->SetStatX(0.9);
		calibaration_points = std::pair<int, int>(3, 6);

		double SiPM_size = 10; //mm
		//<x,y>
		MPPC_coords.clear();
		MPPC_coords[32]= std::pair<double,double>(-2,-2);
		MPPC_coords[33]= std::pair<double,double>(-1,-2);
		MPPC_coords[34]= std::pair<double,double>(2,-2);
		MPPC_coords[35]= std::pair<double,double>(-2,-1);
		MPPC_coords[36]= std::pair<double,double>(1,-1);
		MPPC_coords[37]= std::pair<double,double>(2,-1);
		MPPC_coords[38]= std::pair<double,double>(0,0);
		MPPC_coords[39]= std::pair<double,double>(1,0);
		MPPC_coords[40]= std::pair<double,double>(-1,1);
		MPPC_coords[41]= std::pair<double,double>(0,1);
		MPPC_coords[42]= std::pair<double,double>(-2,2);
		MPPC_coords[43]= std::pair<double,double>(-1,2);
		MPPC_coords[44]= std::pair<double,double>(2,2);
		MPPC_coords[48]= std::pair<double,double>(0,-2);
		MPPC_coords[49]= std::pair<double,double>(1,-2);
		MPPC_coords[50]= std::pair<double,double>(-1,-1);
		MPPC_coords[51]= std::pair<double,double>(0,-1);
		MPPC_coords[52]= std::pair<double,double>(-2,0);
		MPPC_coords[53]= std::pair<double,double>(-1,0);
		MPPC_coords[54]= std::pair<double,double>(2,0);
		MPPC_coords[55]= std::pair<double,double>(-2,1);
		MPPC_coords[56]= std::pair<double,double>(1,1);
		MPPC_coords[57]= std::pair<double,double>(2,1);
		MPPC_coords[58]= std::pair<double,double>(0,2);
		MPPC_coords[59]= std::pair<double,double>(1,2);
		for (auto i = MPPC_coords.begin();i!=MPPC_coords.end();++i){
			i->second.first *=SiPM_size;
			i->second.second*=SiPM_size;
		}

		if (!full)
			return;
		if (areas_to_draw.empty())
			areas_to_draw.push_back(experiment_area());
		areas_to_draw.back().channels.erase();
		areas_to_draw.back().runs.erase();
		areas_to_draw.back().sub_runs.erase();
		areas_to_draw.back().runs.push_pair(0, 0);
		areas_to_draw.back().channels.push_pair(0, 1);
		areas_to_draw.back().channels.push_pair(32, 62);
		areas_to_draw.back().sub_runs.push_pair(0, 0);

		exp_area.channels.erase();
		exp_area.runs.erase();
		exp_area.sub_runs.erase();
		exp_area.channels.push_pair(0, 1);
		exp_area.channels.push_pair(8, 8);
		exp_area.channels.push_pair(12,12);
		//exp_area.channels.push_pair(2, 2);
		exp_area.channels.push_pair(32, 62);//will load only present channels
		exp_area.runs.push_pair(0, 0);
		exp_area.sub_runs.push_pair(0, 0);


		data_prefix_path = "../Data/180215/results/";
		calibration_file = "PMT_SiPM_48V_180215.dat";
		data_output_path = "../Data/180215/results/";

		DATA_MPPC_VERSION = "MPPCs_v1";
		DATA_PMT_VERSION = "PMT_v1";

		OUTPUT_MPPCS_PICS = "MPPCs_v1/MPPCs_";
		OUTPUT_PMT_PICS = "PMT_v1/PMT_";
		OUTPUT_MPPCS = "MPPC_";

		PMT_V.clear();
		PMT_V["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;

		PMT_dB.clear();
		PMT_dB["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 3.98; //ratio, not actual dB
		PMT_dB["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 3.98;

		double coeff = (600 / 804.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
		experiment_fields.clear();
		experiment_fields["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 6;
		experiment_fields["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 8;
		experiment_fields["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 10;
		experiment_fields["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 11;
		experiment_fields["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 12;
		experiment_fields["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 14;

		for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
			j->second *= coeff;

		areas_to_draw.back().experiments.clear();
		areas_to_draw.back().experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");


		exp_area.experiments.clear();
		exp_area.experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	}
//};
