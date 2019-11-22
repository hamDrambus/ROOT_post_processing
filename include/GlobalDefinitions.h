#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <string.h>
#include <sstream>
#include <climits>
#include <algorithm>
#include <stdexcept>
//#include <functional>

#if defined (__WIN32__)
#include <sehmap.h>
#include <direct.h>
#include <Windows4Root.h>
#else
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#endif

#undef max
#undef min

#ifndef __ROOTCLING__
#include <boost/random/gamma_distribution.hpp>
#include <boost/random/inversive_congruential.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/lexical_cast.hpp>
#endif //__CINT__

#include <TROOT.h>
#include <TThread.h>
#include <TApplication.h>
#include <TRint.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TGraph.h>
#include <Rtypes.h>
#include <TVector.h>
#include "TMatrixD.h"
#include "TVectorD.h"
#include <TF1.h>
#include <TMath.h>
#include <Math/Point2D.h>
#include <TRandom1.h>
#include "TH1D.h"
#include "TH2D.h"
#include "TH1I.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TPolyLine.h"
#include "TStyle.h"
//#include "Math/Unuran.h"

#define _TEMP_CODE

//#define _COMPLIE_MAIN_

#define PEAK_AVR_TIME
//#define WEIGHTED_COORD

//#define STD_CONT std::deque

//#define _USE_DEQUE
#ifdef _USE_DEQUE
#define DVECTOR std::deque<double>
#else
#define DVECTOR std::vector<double>
#endif

#define DITERATOR std::vector<double>::iterator
#define D_REV_ITERATOR std::vector<double>::reverse_iterator

typedef bool(*CUTTER)(std::vector<double>& pars, int run_n, void* stat_data);
typedef bool(*CUTTER_DRAW)(TCanvas* can, void* stat_data);

void open_output_file(std::string name, std::ofstream &str, std::ios_base::openmode _mode = std::ios_base::trunc);
void ensure_folder(std::string folder);
void ensure_file(std::string fname);
bool confirm_action (std::string action);
int getIndex(const std::vector<int>& channels, int ch); //TODO: template
int getIndex(const std::deque<int>& channels, int ch);
std::string int_to_str(int num);
std::string int_to_str(std::size_t num);
std::string int_to_str(int num, std::size_t decimals);
std::string int_to_str(std::size_t num, std::size_t decimals);
std::string dbl_to_str (double val, int precision=0);

template <class T>
class channel_info {
protected:
	std::deque<std::pair<int, T> > _data;
public:
	channel_info()
	{}
	~channel_info()
	{}
	template <class U>
	bool isSameChannels(const channel_info<U>& b) const
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
	bool isSameChannels(const std::deque<int>& channels) const
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
	void push(const int& channel, const T& data) //preserves channel sorting
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
	void push_back(const int& channel, const T& data) //ignores channel sorting
	{
		_data.push_back(std::pair<int, T>(channel, data));
	}
	T* info(const int& channel)
	{
		std::size_t ch_ind = get_index(channel);
		if (std::numeric_limits<std::size_t>::max() == ch_ind) {
			return NULL;
		}
		return &(_data[ch_ind].second);
	}
	T& operator [] (const std::size_t& ch_ind)
	{
		std::size_t sz = _data.size();
		if (!(ch_ind < sz))
			throw std::out_of_range("channel_info<T>::operator[] index is out of range");
		return _data[ch_ind].second;
	}
	void erase(const int& channel)
	{
		std::size_t ch_ind = get_index(channel);
		if (std::numeric_limits<std::size_t>::max()==ch_ind){
			std::cout << "channel_info<T>::erase: Warning! Does not contain channel " << channel << std::endl;
			return;
		}
		erase_at(ch_ind);
	}
	void erase_at(const std::size_t& ch_ind)
	{
		std::size_t sz = _data.size();
		if (!(ch_ind < sz))
			throw std::out_of_range("channel_info<T>::erase_at index is out of range");
		_data.erase(_data.begin() + ch_ind);
	}
	void clear(void)
	{
		_data.clear();
	}
	std::size_t size(void) const
	{
		return _data.size();
	}
	bool empty(void) const
	{
		return _data.empty();
	}
	bool contains(const int& channel) const
	{
		if (std::numeric_limits<std::size_t>::max() != get_index(channel))
			return true;
		return false;
	}
	std::size_t get_index(const int& channel) const
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
	int channel(const std::size_t& ch_ind) const
	{
		std::size_t sz = _data.size();
		if (!(ch_ind < sz))
			throw std::out_of_range("channel_info<T>::channel index is out of range");
		return _data[ch_ind].first;
	}
	std::pair<std::size_t, std::size_t> get_bounds(const int& channel) const
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
	void sort(void)
	{
		std::sort(_data.begin(), _data.end(), [](const std::pair<int, T>& a, const std::pair<int, T>& b)->bool {
			return a.first < b.first;
		});
	}
	bool is_sorted(void) const
	{
		for (std::size_t ind = 0, ind_end_ = _data.size(); (ind + 1) != ind_end_; ++ind)
			if (_data[ind].first >= _data[ind+1].first)
				return false;
		return true;
	}
};

class dB_info {
	double _dB;
	double _atten; //attenuation. > 1 in case of positive dB
	double dB_to_atten(double dB) const {
		return std::pow(10, dB / 20.0);
	}
	double atten_to_dB(double atten) const {
		return 20.0*std::log(atten) / std::log(10.0);
	}
public:
	dB_info(double dB) : _dB(dB), _atten(dB_to_atten(dB))
	{}
	~dB_info()
	{}
	double get_dB(void) const {
		return _dB;
	}
	double get_atten(void) const {
		return _atten;
	}
	void set_dB(double dB) {
		_dB = dB;
		_atten = dB_to_atten(dB);
	}
	void set_atten(double atten) {
		_atten = atten;
		_dB = atten_to_dB(atten);
	}
};

class TestSignalGenerator {
public:
	TestSignalGenerator(std::string prefix);
};

#if defined(__WIN32__)
#define INVOKE_GNUPLOT(a) system(("start \"\" \"%GNUPLOT%\\gnuplot.exe\" -c \"" + a + "\"").c_str())
#else
#define INVOKE_GNUPLOT(a) system(("konsole -e gnuplot \"" + a +"\"").c_str());
#endif //__WIN32__


class peak
{
public:
	double left;
	double right;
	double S; //Area
	double A; //Amplitude (from baseline)
#ifdef PEAK_AVR_TIME
	double t;
#endif
	peak();
};

class peak_processed
{
public:
	double S; //Area
	double A; //Amplitude (from baseline)
	double left;
	double right;
	double t;
	int Npe;
	peak_processed(const double& iS, const double& iA, const double& ileft, const double& iright, const double& it, const int& iNpe);
	peak_processed(const peak& pk, const int& iNpe);
};

#endif
