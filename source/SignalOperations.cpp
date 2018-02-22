#include "SignalOperations.h"
#include "Math/Functor.h"
#include "Math/BrentMinimizer1D.h"
#include "TSpectrum.h"
#include "Polynom2Order.h"
#include "GraphicOutputManager.h"

namespace SignalOperations {

	void invert_y(std::vector<Double_t> &x_in_out, std::vector<Double_t> &y_in_out)
	{
		auto _end_ = y_in_out.end();
		for (auto i = y_in_out.begin(); i != _end_; ++i)
			*i = -*i;
	}

	Double_t find_baseline_by_median(Double_t approx, std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks)
	{
		if (xs.size() != ys.size())
			return approx;
		std::vector<Double_t> selected_y;
#ifndef _USE_DEQUE
		selected_y.reserve(ys.size());
#endif
		auto _end_ = xs.end();
		Bool_t do_account = kTRUE;
		for (auto i = xs.begin(), j = ys.begin(); (i != _end_); ++i, ++j){
			do_account = kTRUE;
			for (auto pp = peaks.begin(),pp_end_ = peaks.end(); pp != pp_end_; ++pp)
				if (!(*i<pp->left) && !(*i>pp->right)) {
					do_account = kFALSE;
					break;
				}
			if (do_account){
				selected_y.push_back(*j);
			}
		}
		std::sort(selected_y.begin(), selected_y.end());
		Int_t ind = selected_y.size();
		if (0 == ind)
			return approx;
		if (0 == ind % 2)
			return selected_y[ind / 2];
		else
			return 0.5*(selected_y[ind / 2] + selected_y[1 + (ind / 2)]);
	}

	Double_t find_baseline_by_median(Double_t approx, std::vector<Double_t> &xs, std::vector<Double_t> &ys)
	{
		if (xs.size() != ys.size())
			return approx;
		std::vector<Double_t> selected_y=ys;
		std::sort(selected_y.begin(), selected_y.end());
		Int_t ind = selected_y.size();
		if (0 == ind)
			return approx;
		if (0 == ind % 2)
			return selected_y[ind / 2];
		else
			return 0.5*(selected_y[ind / 2] + selected_y[1 + (ind / 2)]);
	}

	Double_t find_baseline_by_Int_tegral(Double_t approx, std::vector<Double_t> &xs, std::vector<Double_t> &ys)
	{
		if ((xs.size() <= 1) || (xs.size() != ys.size()))
			return approx;
		Double_t val = 0;
		Double_t dx = xs.back()-*(xs.begin());
		if (0 == dx)
			return approx;
		Integrate(xs, ys, val, xs.begin(), --xs.end(), *(++xs.begin()) - *(xs.begin()), 0);
		return val/dx;
	}

	Double_t find_baseline_by_Int_tegral(Double_t approx, std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks)
	{
		if ((xs.size() != ys.size())||(xs.size()<2))
			return approx;
		Bool_t one_vector = kTRUE;
		auto _end_ = xs.end();
		auto _begin_ = xs.begin();
		Double_t Sum_dx = 0, Sum_Int_t = 0;
		std::vector<Double_t>::iterator x_cut_left = xs.begin(), x_cut_right = _end_;
		Bool_t do_account = kTRUE;
		Double_t delta_x = *(++xs.begin()) - *(xs.begin());
		auto pks_end_ = peaks.end();
		for (auto i = xs.begin(), j = ys.begin(); (i != _end_); ++i, ++j){
			do_account = kTRUE;
			for (auto pp = peaks.begin(); pp != pks_end_; ++pp)
				if (!(*i<(*pp).left) && !(*i>(*pp).right)) {
					do_account = kFALSE;
					break;
				}
			if (do_account&&!one_vector) {
				if ((x_cut_right!=_end_)&&(x_cut_right-x_cut_left>= 1)){
					Sum_dx += *x_cut_right - *x_cut_left;
					Double_t val;
					Integrate(xs, ys, val, x_cut_left, x_cut_right, delta_x, 0);
					Sum_Int_t += val;
				}
				x_cut_right = _end_;
				x_cut_left = i;
				one_vector = kTRUE;
			}
			if (!do_account&&one_vector){
				x_cut_right = (i==_begin_) ? _end_: i- 1;
				one_vector = kFALSE;
			}
		}
		//the code above doesn't add the last area. (one_vector === has do_account changed) 
		//if ((one_vector && (x_cut_right == _end_))/*last Int_terval before end*/
			//|| ((x_cut_right != _end_) && !one_vector)/*last peak overlaps with end, and the last area has not been added*/) {
			x_cut_right = (x_cut_right == _end_) ? x_cut_right-1 : x_cut_right;
			if (x_cut_right - x_cut_left >= 1){
				Sum_dx += *x_cut_right - *x_cut_left;
				Double_t val;
				Integrate(xs, ys, val, x_cut_left, x_cut_right,delta_x, 0);
				Sum_Int_t += val;
			}
		//}
		if (0 == Sum_dx)
			return approx;
		return (Sum_Int_t / Sum_dx);
	}

	//Not used, too unstable and very expensive
	void find_baseline_by_ROOT_advanced(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		if (xs.size() != ys.size()){
			ys_out.clear();
			return;
		}
			
		ys_out.resize(ys.size());

		TSpectrum *spec = new TSpectrum();
		float *f_ys = new float[ys.size()];
		for (Int_t h = 0; h != ys.size(); ++h)
			f_ys[h] = ys[h];
		spec->Background(f_ys, ys.size(), 60, TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE);
		for (Int_t h = 0; h != ys.size(); ++h)
			ys_out[h]=f_ys[h];
		delete[] f_ys;
		spec->Delete();

		std::vector<Double_t>::iterator x_min;
		Double_t y_min;
		SignalOperations::get_min(xs, ys_out, xs.begin(), xs.end(), x_min, y_min, 1);
		if (x_min == xs.end())
			return;
		std::vector<Double_t>::reverse_iterator x_prev_min(x_min);
		SignalOperations::find_previous_extremum_faster(xs, ys_out, x_prev_min, 5); //TODO: N_trust and ParameterPile
		if (x_prev_min == xs.rend())
			return;
		SignalOperations::find_previous_extremum_faster(xs, ys_out, x_prev_min, 5);
		if (x_prev_min == xs.rend())
			return;
		std::vector<Double_t>::iterator x_cut_from = x_prev_min.base();
		Double_t y_cut_from = ys_out[x_cut_from - xs.begin()];
		for (auto xx = x_cut_from; xx != x_min; ++xx)
			ys_out[xx - xs.begin()] = y_cut_from + (*xx-*x_cut_from)*(y_min - y_cut_from)/(*x_min-*x_cut_from);//line
	}

	void find_baseline_by_ROOT(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		Int_t _size_ = ys.size();
#ifndef _USE_DEQUE
		ys_out = ys;
		Double_t *f_ys = &ys_out[0];
#else
		ys_out.resize(ys.size());
		Double_t *f_ys = new Double_t[_size_];
		for (Int_t h = 0; h != _size_; ++h)
			f_ys[h] = ys[h];
#endif
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V0
#ifdef _USE_DEQUE
			for (Int_t h = 0; h != _size_; ++h)
				ys_out[h] = f_ys[h];
		delete[] f_ys;
#endif
	}

	void find_baseline_by_ROOT_v2(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		Int_t _size_ = ys.size();
#ifndef _USE_DEQUE
		ys_out = ys;
		Double_t *f_ys = &ys_out[0];
#else
		ys_out.resize(ys.size());
		Double_t *f_ys = new Double_t[_size_];
		for (Int_t h = 0; h != _size_; ++h)
			f_ys[h] = ys[h];
#endif
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V2
#ifdef _USE_DEQUE
		for (Int_t h = 0; h != _size_; ++h)
			ys_out[h] = f_ys[h];
		delete[] f_ys;
#endif
	}

	void find_baseline_by_ROOT_v3(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		Int_t _size_ = ys.size();
#ifndef _USE_DEQUE
		ys_out = ys;
		Double_t *f_ys = &ys_out[0];
#else
		ys_out.resize(ys.size());
		Double_t *f_ys = new Double_t[_size_];
		for (Int_t h = 0; h != _size_; ++h)
			f_ys[h] = ys[h];
#endif
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V3
#ifdef _USE_DEQUE
			for (Int_t h = 0; h != _size_; ++h)
				ys_out[h] = f_ys[h];
		delete[] f_ys;
#endif
	}

	void find_baseline_by_ROOT_v4(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		Int_t _size_ = ys.size();
#ifndef _USE_DEQUE
		ys_out = ys;
		Double_t *f_ys = &ys_out[0];
#else
		ys_out.resize(ys.size());
		Double_t *f_ys = new Double_t[_size_];
		for (Int_t h = 0; h != _size_; ++h)
			f_ys[h] = ys[h];
#endif
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V4
#ifdef _USE_DEQUE
			for (Int_t h = 0; h != _size_; ++h)
				ys_out[h] = f_ys[h];
		delete[] f_ys;
#endif
	}

	void find_baseline_by_ROOT_v5(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		ys_out.resize(ys.size());

		TSpectrum *spec = new TSpectrum();
		float *f_ys = new float[ys.size()];
		for (Int_t h = 0; h != ys.size(); ++h)
			f_ys[h] = ys[h];
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V5
		for (Int_t h = 0; h != ys.size(); ++h)
			ys_out[h]=f_ys[h];
		delete[] f_ys;
		spec->Delete();
	}

	void find_baseline_by_ROOT_v6(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		ys_out.resize(ys.size());

		TSpectrum *spec = new TSpectrum();
		float *f_ys = new float[ys.size()];
		for (Int_t h = 0; h != ys.size(); ++h)
			f_ys[h] = ys[h];
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V6
		for (Int_t h = 0; h != ys.size(); ++h)
			ys_out[h] = f_ys[h];
		delete[] f_ys;
		spec->Delete();

		std::vector<Double_t>::iterator x_min;
		Double_t y_min;
		SignalOperations::get_min(xs, ys_out, xs.begin(), xs.end(), x_min, y_min, 1);
		if (x_min == xs.end())
			return;
		std::vector<Double_t>::reverse_iterator x_prev_min(x_min);
		SignalOperations::find_previous_extremum_faster(xs, ys_out, x_prev_min, 5);
		if (x_prev_min == xs.rend())
			return;
		SignalOperations::find_previous_extremum_faster(xs, ys_out, x_prev_min, 5);
		if (x_prev_min == xs.rend())
			return;
		std::vector<Double_t>::iterator x_cut_from = x_prev_min.base();
		Double_t y_cut_from = ys_out[x_cut_from - xs.begin()];
		for (auto xx = x_cut_from; xx != x_min; ++xx)
			ys_out[xx - xs.begin()] = y_cut_from + (*xx - *x_cut_from)*(y_min - y_cut_from) / (*x_min - *x_cut_from);//line
	}

	void find_baseline_by_ROOT_v7(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		ys_out.resize(ys.size());

		TSpectrum *spec = new TSpectrum();
		float *f_ys = new float[ys.size()];
		for (Int_t h = 0; h != ys.size(); ++h)
			f_ys[h] = ys[h];
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V7
		for (Int_t h = 0; h != ys.size(); ++h)
			ys_out[h]=f_ys[h];
		delete[] f_ys;
		spec->Delete();
	}

	void find_baseline_by_ROOT_v8(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out)
	{
		ys_out.resize(ys.size());

		TSpectrum *spec = new TSpectrum();
		float *f_ys = new float[ys.size()];
		for (Int_t h = 0; h != ys.size(); ++h)
			f_ys[h] = ys[h];
		//TODO: ? ParameterPile and as input parameters?
		ROOT_BL_CALL_V8
		for (Int_t h = 0; h != ys.size(); ++h)
			ys_out[h]=f_ys[h];
		delete[] f_ys;
		spec->Delete();
	}

	const char *find_background_v_raw(Double_t *spectrum, Int_t ssize,
		Int_t numberIterations,
		Int_t direction, Int_t filterOrder,
		Bool_t smoothing, Int_t smoothWindow,
		Bool_t compton)
	{
	
		Int_t i, j, w, bw, b1, b2, priz;
		Double_t a, b, c, d, e, yb1, yb2, ai, av, men, b4, c4, d4, e4, b6, c6, d6, e6, f6, g6, b8, c8, d8, e8, f8, g8, h8, i8;
		if (ssize <= 0)
			return "Wrong Parameters";
		if (numberIterations < 1)
			return "Width of Clipping Window Must Be Positive";
		if (ssize < 2 * numberIterations + 1)
			return "Too Large Clipping Window";
		if (smoothing == kTRUE && smoothWindow != TSpectrum::kBackSmoothing3 && smoothWindow != TSpectrum::kBackSmoothing5 && smoothWindow != TSpectrum::kBackSmoothing7 && smoothWindow != TSpectrum::kBackSmoothing9 && smoothWindow != TSpectrum::kBackSmoothing11 && smoothWindow != TSpectrum::kBackSmoothing13 && smoothWindow != TSpectrum::kBackSmoothing15)
			return "Incorrect width of smoothing window";
		Double_t *working_space = new Double_t[2 * ssize];
		for (i = 0; i < ssize; i++){
			working_space[i] = spectrum[i];
			working_space[i + ssize] = spectrum[i];
		}
		bw = (smoothWindow - 1) / 2;
		if (direction == TSpectrum::kBackIncreasingWindow)
			i = 1;
		else if (direction == TSpectrum::kBackDecreasingWindow)
			i = numberIterations;
		if (filterOrder == TSpectrum::kBackOrder2) {
			do {
				for (j = i; j < ssize - i; j++) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += 1;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= 1;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		else if (filterOrder == TSpectrum::kBackOrder4) {
			do{
				for (j = i; j < ssize - i; j++) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						c = 0;
						ai = i / 2;
						c -= working_space[ssize + j - (Int_t)(2 * ai)] / 6;
						c += 4 * working_space[ssize + j - (Int_t)ai] / 6;
						c += 4 * working_space[ssize + j + (Int_t)ai] / 6;
						c -= working_space[ssize + j + (Int_t)(2 * ai)] / 6;
						if (b < c)
							b = c;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						ai = i / 2;
						b4 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b4 += working_space[ssize + w];
								men += 1;
							}
						}
						b4 = b4 / men;
						c4 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								c4 += working_space[ssize + w];
								men += 1;
							}
						}
						c4 = c4 / men;
						d4 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d4 += working_space[ssize + w];
								men += 1;
							}
						}
						d4 = d4 / men;
						e4 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								e4 += working_space[ssize + w];
								men += 1;
							}
						}
						e4 = e4 / men;
						b4 = (-b4 + 4 * c4 + 4 * d4 - e4) / 6;
						if (b < b4)
							b = b4;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += 1;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= 1;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		else if (filterOrder == TSpectrum::kBackOrder6) {
			do {
				for (j = i; j < ssize - i; j++) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						c = 0;
						ai = i / 2;
						c -= working_space[ssize + j - (Int_t)(2 * ai)] / 6;
						c += 4 * working_space[ssize + j - (Int_t)ai] / 6;
						c += 4 * working_space[ssize + j + (Int_t)ai] / 6;
						c -= working_space[ssize + j + (Int_t)(2 * ai)] / 6;
						d = 0;
						ai = i / 3;
						d += working_space[ssize + j - (Int_t)(3 * ai)] / 20;
						d -= 6 * working_space[ssize + j - (Int_t)(2 * ai)] / 20;
						d += 15 * working_space[ssize + j - (Int_t)ai] / 20;
						d += 15 * working_space[ssize + j + (Int_t)ai] / 20;
						d -= 6 * working_space[ssize + j + (Int_t)(2 * ai)] / 20;
						d += working_space[ssize + j + (Int_t)(3 * ai)] / 20;
						if (b < d)
							b = d;
						if (b < c)
							b = c;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						ai = i / 2;
						b4 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b4 += working_space[ssize + w];
								men += 1;
							}
						}
						b4 = b4 / men;
						c4 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								c4 += working_space[ssize + w];
								men += 1;
							}
						}
						c4 = c4 / men;
						d4 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d4 += working_space[ssize + w];
								men += 1;
							}
						}
						d4 = d4 / men;
						e4 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								e4 += working_space[ssize + w];
								men += 1;
							}
						}
						e4 = e4 / men;
						b4 = (-b4 + 4 * c4 + 4 * d4 - e4) / 6;
						ai = i / 3;
						b6 = 0, men = 0;
						for (w = j - (Int_t)(3 * ai) - bw; w <= j - (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b6 += working_space[ssize + w];
								men += 1;
							}
						}
						b6 = b6 / men;
						c6 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								c6 += working_space[ssize + w];
								men += 1;
							}
						}
						c6 = c6 / men;
						d6 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d6 += working_space[ssize + w];
								men += 1;
							}
						}
						d6 = d6 / men;
						e6 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								e6 += working_space[ssize + w];
								men += 1;
							}
						}
						e6 = e6 / men;
						f6 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								f6 += working_space[ssize + w];
								men += 1;
							}
						}
						f6 = f6 / men;
						g6 = 0, men = 0;
						for (w = j + (Int_t)(3 * ai) - bw; w <= j + (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								g6 += working_space[ssize + w];
								men += 1;
							}
						}
						g6 = g6 / men;
						b6 = (b6 - 6 * c6 + 15 * d6 + 15 * e6 - 6 * f6 + g6) / 20;
						if (b < b6)
							b = b6;
						if (b < b4)
							b = b4;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += 1;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= 1;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		else if (filterOrder == TSpectrum::kBackOrder8) {
			do{
				for (j = i; j < ssize - i; j++) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						c = 0;
						ai = i / 2;
						c -= working_space[ssize + j - (Int_t)(2 * ai)] / 6;
						c += 4 * working_space[ssize + j - (Int_t)ai] / 6;
						c += 4 * working_space[ssize + j + (Int_t)ai] / 6;
						c -= working_space[ssize + j + (Int_t)(2 * ai)] / 6;
						d = 0;
						ai = i / 3;
						d += working_space[ssize + j - (Int_t)(3 * ai)] / 20;
						d -= 6 * working_space[ssize + j - (Int_t)(2 * ai)] / 20;
						d += 15 * working_space[ssize + j - (Int_t)ai] / 20;
						d += 15 * working_space[ssize + j + (Int_t)ai] / 20;
						d -= 6 * working_space[ssize + j + (Int_t)(2 * ai)] / 20;
						d += working_space[ssize + j + (Int_t)(3 * ai)] / 20;
						e = 0;
						ai = i / 4;
						e -= working_space[ssize + j - (Int_t)(4 * ai)] / 70;
						e += 8 * working_space[ssize + j - (Int_t)(3 * ai)] / 70;
						e -= 28 * working_space[ssize + j - (Int_t)(2 * ai)] / 70;
						e += 56 * working_space[ssize + j - (Int_t)ai] / 70;
						e += 56 * working_space[ssize + j + (Int_t)ai] / 70;
						e -= 28 * working_space[ssize + j + (Int_t)(2 * ai)] / 70;
						e += 8 * working_space[ssize + j + (Int_t)(3 * ai)] / 70;
						e -= working_space[ssize + j + (Int_t)(4 * ai)] / 70;
						if (b < e)
							b = e;
						if (b < d)
							b = d;
						if (b < c)
							b = c;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						ai = i / 2;
						b4 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b4 += working_space[ssize + w];
								men += 1;
							}
						}
						b4 = b4 / men;
						c4 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								c4 += working_space[ssize + w];
								men += 1;
							}
						}
						c4 = c4 / men;
						d4 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d4 += working_space[ssize + w];
								men += 1;
							}
						}
						d4 = d4 / men;
						e4 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								e4 += working_space[ssize + w];
								men += 1;
							}
						}
						e4 = e4 / men;
						b4 = (-b4 + 4 * c4 + 4 * d4 - e4) / 6;
						ai = i / 3;
						b6 = 0, men = 0;
						for (w = j - (Int_t)(3 * ai) - bw; w <= j - (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b6 += working_space[ssize + w];
								men += 1;
							}
						}
						b6 = b6 / men;
						c6 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								c6 += working_space[ssize + w];
								men += 1;
							}
						}
						c6 = c6 / men;
						d6 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d6 += working_space[ssize + w];
								men += 1;
							}
						}
						d6 = d6 / men;
						e6 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								e6 += working_space[ssize + w];
								men += 1;
							}
						}
						e6 = e6 / men;
						f6 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								f6 += working_space[ssize + w];
								men += 1;
							}
						}
						f6 = f6 / men;
						g6 = 0, men = 0;
						for (w = j + (Int_t)(3 * ai) - bw; w <= j + (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								g6 += working_space[ssize + w];
								men += 1;
							}
						}
						g6 = g6 / men;
						b6 = (b6 - 6 * c6 + 15 * d6 + 15 * e6 - 6 * f6 + g6) / 20;
						ai = i / 4;
						b8 = 0, men = 0;
						for (w = j - (Int_t)(4 * ai) - bw; w <= j - (Int_t)(4 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b8 += working_space[ssize + w];
								men += 1;
							}
						}
						b8 = b8 / men;
						c8 = 0, men = 0;
						for (w = j - (Int_t)(3 * ai) - bw; w <= j - (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								c8 += working_space[ssize + w];
								men += 1;
							}
						}
						c8 = c8 / men;
						d8 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								d8 += working_space[ssize + w];
								men += 1;
							}
						}
						d8 = d8 / men;
						e8 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								e8 += working_space[ssize + w];
								men += 1;
							}
						}
						e8 = e8 / men;
						f8 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								f8 += working_space[ssize + w];
								men += 1;
							}
						}
						f8 = f8 / men;
						g8 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								g8 += working_space[ssize + w];
								men += 1;
							}
						}
						g8 = g8 / men;
						h8 = 0, men = 0;
						for (w = j + (Int_t)(3 * ai) - bw; w <= j + (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								h8 += working_space[ssize + w];
								men += 1;
							}
						}
						h8 = h8 / men;
						i8 = 0, men = 0;
						for (w = j + (Int_t)(4 * ai) - bw; w <= j + (Int_t)(4 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								i8 += working_space[ssize + w];
								men += 1;
							}
						}
						i8 = i8 / men;
						b8 = (-b8 + 8 * c8 - 28 * d8 + 56 * e8 - 56 * f8 - 28 * g8 + 8 * h8 - i8) / 70;
						if (b < b8)
							b = b8;
						if (b < b6)
							b = b6;
						if (b < b4)
							b = b4;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += 1;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= 1;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		if (compton == kTRUE) {
			for (i = 0, b2 = 0; i < ssize; i++){
				b1 = b2;
				a = working_space[i], b = spectrum[i];
				j = i;
				if (TMath::Abs(a - b) >= 1) {
					b1 = i - 1;
					if (b1 < 0)
						b1 = 0;
					yb1 = working_space[b1];
					for (b2 = b1 + 1, c = 0, priz = 0; priz == 0 && b2 < ssize; b2++){
						a = working_space[b2], b = spectrum[b2];
						c = c + b - yb1;
						if (TMath::Abs(a - b) < 1) {
							priz = 1;
							yb2 = b;
						}
					}
					if (b2 == ssize)
						b2 -= 1;
					yb2 = working_space[b2];
					if (yb1 <= yb2){
						for (j = b1, c = 0; j <= b2; j++){
							b = spectrum[j];
							c = c + b - yb1;
						}
						if (c > 1){
							c = (yb2 - yb1) / c;
							for (j = b1, d = 0; j <= b2 && j < ssize; j++){
								b = spectrum[j];
								d = d + b - yb1;
								a = c * d + yb1;
								working_space[ssize + j] = a;
							}
						}
					}

					else{
						for (j = b2, c = 0; j >= b1; j--){
							b = spectrum[j];
							c = c + b - yb2;
						}
						if (c > 1){
							c = (yb1 - yb2) / c;
							for (j = b2, d = 0; j >= b1 && j >= 0; j--){
								b = spectrum[j];
								d = d + b - yb2;
								a = c * d + yb2;
								working_space[ssize + j] = a;
							}
						}
					}
					i = b2;
				}
			}
		}

		for (j = 0; j < ssize; j++)
			spectrum[j] = working_space[ssize + j];
		delete[]working_space;
		return 0;
	}

	const char *find_background_v_0(Double_t *spectrum, Int_t ssize,
		Int_t numberIterations,
		Int_t direction, Int_t filterOrder,
		Bool_t smoothing, Int_t smoothWindow,
		Bool_t compton, Int_t sparse)
	{

		Int_t i, j, w, bw, b1, b2, priz;
		Double_t a, b, c, d, e, yb1, yb2, ai, av, men, b4, c4, d4, e4, b6, c6, d6, e6, f6, g6, b8, c8, d8, e8, f8, g8, h8, i8;
		if (ssize <= 0)
			return "Wrong Parameters";
		if (numberIterations < 1)
			return "Width of Clipping Window Must Be Positive";
		if (ssize < 2 * numberIterations + 1)
			return "Too Large Clipping Window";
		if (smoothing == kTRUE && smoothWindow != TSpectrum::kBackSmoothing3 && smoothWindow != TSpectrum::kBackSmoothing5 && smoothWindow != TSpectrum::kBackSmoothing7 && smoothWindow != TSpectrum::kBackSmoothing9 && smoothWindow != TSpectrum::kBackSmoothing11 && smoothWindow != TSpectrum::kBackSmoothing13 && smoothWindow != TSpectrum::kBackSmoothing15)
			return "Incorrect width of smoothing window";
		if (sparse < 1)
			return "Sparse Must Be Positive";
		if (sparse >= numberIterations)
			return "Sparse must be Less than the width of clipping window";
		Double_t *working_space = new Double_t[2 * ssize];
		for (i = 0; i < ssize; i++){
			working_space[i] = spectrum[i];
			working_space[i + ssize] = spectrum[i];
		}
		bw = (smoothWindow - 1) / 2;
		if (direction == TSpectrum::kBackIncreasingWindow)
			i = 1;
		else if (direction == TSpectrum::kBackDecreasingWindow)
			i = numberIterations;
		if (filterOrder == TSpectrum::kBackOrder2) {
			do {
				for (j = i; j < ssize - i; ++j) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += sparse;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= sparse;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		else if (filterOrder == TSpectrum::kBackOrder4) {
			do{
				for (j = i; j < ssize - i; j++) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						c = 0;
						ai = i / 2;
						c -= working_space[ssize + j - (Int_t)(2 * ai)] / 6;
						c += 4 * working_space[ssize + j - (Int_t)ai] / 6;
						c += 4 * working_space[ssize + j + (Int_t)ai] / 6;
						c -= working_space[ssize + j + (Int_t)(2 * ai)] / 6;
						if (b < c)
							b = c;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						ai = i / 2;
						b4 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b4 += working_space[ssize + w];
								men += 1;
							}
						}
						b4 = b4 / men;
						c4 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								c4 += working_space[ssize + w];
								men += 1;
							}
						}
						c4 = c4 / men;
						d4 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d4 += working_space[ssize + w];
								men += 1;
							}
						}
						d4 = d4 / men;
						e4 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								e4 += working_space[ssize + w];
								men += 1;
							}
						}
						e4 = e4 / men;
						b4 = (-b4 + 4 * c4 + 4 * d4 - e4) / 6;
						if (b < b4)
							b = b4;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += sparse;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= sparse;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		else if (filterOrder == TSpectrum::kBackOrder6) {
			do {
				for (j = i; j < ssize - i; j++) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						c = 0;
						ai = i / 2;
						c -= working_space[ssize + j - (Int_t)(2 * ai)] / 6;
						c += 4 * working_space[ssize + j - (Int_t)ai] / 6;
						c += 4 * working_space[ssize + j + (Int_t)ai] / 6;
						c -= working_space[ssize + j + (Int_t)(2 * ai)] / 6;
						d = 0;
						ai = i / 3;
						d += working_space[ssize + j - (Int_t)(3 * ai)] / 20;
						d -= 6 * working_space[ssize + j - (Int_t)(2 * ai)] / 20;
						d += 15 * working_space[ssize + j - (Int_t)ai] / 20;
						d += 15 * working_space[ssize + j + (Int_t)ai] / 20;
						d -= 6 * working_space[ssize + j + (Int_t)(2 * ai)] / 20;
						d += working_space[ssize + j + (Int_t)(3 * ai)] / 20;
						if (b < d)
							b = d;
						if (b < c)
							b = c;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						ai = i / 2;
						b4 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b4 += working_space[ssize + w];
								men += 1;
							}
						}
						b4 = b4 / men;
						c4 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								c4 += working_space[ssize + w];
								men += 1;
							}
						}
						c4 = c4 / men;
						d4 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d4 += working_space[ssize + w];
								men += 1;
							}
						}
						d4 = d4 / men;
						e4 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								e4 += working_space[ssize + w];
								men += 1;
							}
						}
						e4 = e4 / men;
						b4 = (-b4 + 4 * c4 + 4 * d4 - e4) / 6;
						ai = i / 3;
						b6 = 0, men = 0;
						for (w = j - (Int_t)(3 * ai) - bw; w <= j - (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b6 += working_space[ssize + w];
								men += 1;
							}
						}
						b6 = b6 / men;
						c6 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								c6 += working_space[ssize + w];
								men += 1;
							}
						}
						c6 = c6 / men;
						d6 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d6 += working_space[ssize + w];
								men += 1;
							}
						}
						d6 = d6 / men;
						e6 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								e6 += working_space[ssize + w];
								men += 1;
							}
						}
						e6 = e6 / men;
						f6 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								f6 += working_space[ssize + w];
								men += 1;
							}
						}
						f6 = f6 / men;
						g6 = 0, men = 0;
						for (w = j + (Int_t)(3 * ai) - bw; w <= j + (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								g6 += working_space[ssize + w];
								men += 1;
							}
						}
						g6 = g6 / men;
						b6 = (b6 - 6 * c6 + 15 * d6 + 15 * e6 - 6 * f6 + g6) / 20;
						if (b < b6)
							b = b6;
						if (b < b4)
							b = b4;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += sparse;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= sparse;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		else if (filterOrder == TSpectrum::kBackOrder8) {
			do{
				for (j = i; j < ssize - i; j++) {
					if (smoothing == kFALSE){
						a = working_space[ssize + j];
						b = (working_space[ssize + j - i] + working_space[ssize + j + i]) / 2.0;
						c = 0;
						ai = i / 2;
						c -= working_space[ssize + j - (Int_t)(2 * ai)] / 6;
						c += 4 * working_space[ssize + j - (Int_t)ai] / 6;
						c += 4 * working_space[ssize + j + (Int_t)ai] / 6;
						c -= working_space[ssize + j + (Int_t)(2 * ai)] / 6;
						d = 0;
						ai = i / 3;
						d += working_space[ssize + j - (Int_t)(3 * ai)] / 20;
						d -= 6 * working_space[ssize + j - (Int_t)(2 * ai)] / 20;
						d += 15 * working_space[ssize + j - (Int_t)ai] / 20;
						d += 15 * working_space[ssize + j + (Int_t)ai] / 20;
						d -= 6 * working_space[ssize + j + (Int_t)(2 * ai)] / 20;
						d += working_space[ssize + j + (Int_t)(3 * ai)] / 20;
						e = 0;
						ai = i / 4;
						e -= working_space[ssize + j - (Int_t)(4 * ai)] / 70;
						e += 8 * working_space[ssize + j - (Int_t)(3 * ai)] / 70;
						e -= 28 * working_space[ssize + j - (Int_t)(2 * ai)] / 70;
						e += 56 * working_space[ssize + j - (Int_t)ai] / 70;
						e += 56 * working_space[ssize + j + (Int_t)ai] / 70;
						e -= 28 * working_space[ssize + j + (Int_t)(2 * ai)] / 70;
						e += 8 * working_space[ssize + j + (Int_t)(3 * ai)] / 70;
						e -= working_space[ssize + j + (Int_t)(4 * ai)] / 70;
						if (b < e)
							b = e;
						if (b < d)
							b = d;
						if (b < c)
							b = c;
						if (b < a)
							a = b;
						working_space[j] = a;
					}

					else if (smoothing == kTRUE){
						a = working_space[ssize + j];
						av = 0;
						men = 0;
						for (w = j - bw; w <= j + bw; w++){
							if (w >= 0 && w < ssize){
								av += working_space[ssize + w];
								men += 1;
							}
						}
						av = av / men;
						b = 0;
						men = 0;
						for (w = j - i - bw; w <= j - i + bw; w++){
							if (w >= 0 && w < ssize){
								b += working_space[ssize + w];
								men += 1;
							}
						}
						b = b / men;
						c = 0;
						men = 0;
						for (w = j + i - bw; w <= j + i + bw; w++){
							if (w >= 0 && w < ssize){
								c += working_space[ssize + w];
								men += 1;
							}
						}
						c = c / men;
						b = (b + c) / 2;
						ai = i / 2;
						b4 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b4 += working_space[ssize + w];
								men += 1;
							}
						}
						b4 = b4 / men;
						c4 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								c4 += working_space[ssize + w];
								men += 1;
							}
						}
						c4 = c4 / men;
						d4 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d4 += working_space[ssize + w];
								men += 1;
							}
						}
						d4 = d4 / men;
						e4 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								e4 += working_space[ssize + w];
								men += 1;
							}
						}
						e4 = e4 / men;
						b4 = (-b4 + 4 * c4 + 4 * d4 - e4) / 6;
						ai = i / 3;
						b6 = 0, men = 0;
						for (w = j - (Int_t)(3 * ai) - bw; w <= j - (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b6 += working_space[ssize + w];
								men += 1;
							}
						}
						b6 = b6 / men;
						c6 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								c6 += working_space[ssize + w];
								men += 1;
							}
						}
						c6 = c6 / men;
						d6 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								d6 += working_space[ssize + w];
								men += 1;
							}
						}
						d6 = d6 / men;
						e6 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								e6 += working_space[ssize + w];
								men += 1;
							}
						}
						e6 = e6 / men;
						f6 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								f6 += working_space[ssize + w];
								men += 1;
							}
						}
						f6 = f6 / men;
						g6 = 0, men = 0;
						for (w = j + (Int_t)(3 * ai) - bw; w <= j + (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								g6 += working_space[ssize + w];
								men += 1;
							}
						}
						g6 = g6 / men;
						b6 = (b6 - 6 * c6 + 15 * d6 + 15 * e6 - 6 * f6 + g6) / 20;
						ai = i / 4;
						b8 = 0, men = 0;
						for (w = j - (Int_t)(4 * ai) - bw; w <= j - (Int_t)(4 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								b8 += working_space[ssize + w];
								men += 1;
							}
						}
						b8 = b8 / men;
						c8 = 0, men = 0;
						for (w = j - (Int_t)(3 * ai) - bw; w <= j - (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								c8 += working_space[ssize + w];
								men += 1;
							}
						}
						c8 = c8 / men;
						d8 = 0, men = 0;
						for (w = j - (Int_t)(2 * ai) - bw; w <= j - (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								d8 += working_space[ssize + w];
								men += 1;
							}
						}
						d8 = d8 / men;
						e8 = 0, men = 0;
						for (w = j - (Int_t)ai - bw; w <= j - (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								e8 += working_space[ssize + w];
								men += 1;
							}
						}
						e8 = e8 / men;
						f8 = 0, men = 0;
						for (w = j + (Int_t)ai - bw; w <= j + (Int_t)ai + bw; w++){
							if (w >= 0 && w < ssize){
								f8 += working_space[ssize + w];
								men += 1;
							}
						}
						f8 = f8 / men;
						g8 = 0, men = 0;
						for (w = j + (Int_t)(2 * ai) - bw; w <= j + (Int_t)(2 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								g8 += working_space[ssize + w];
								men += 1;
							}
						}
						g8 = g8 / men;
						h8 = 0, men = 0;
						for (w = j + (Int_t)(3 * ai) - bw; w <= j + (Int_t)(3 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								h8 += working_space[ssize + w];
								men += 1;
							}
						}
						h8 = h8 / men;
						i8 = 0, men = 0;
						for (w = j + (Int_t)(4 * ai) - bw; w <= j + (Int_t)(4 * ai) + bw; w++){
							if (w >= 0 && w < ssize){
								i8 += working_space[ssize + w];
								men += 1;
							}
						}
						i8 = i8 / men;
						b8 = (-b8 + 8 * c8 - 28 * d8 + 56 * e8 - 56 * f8 - 28 * g8 + 8 * h8 - i8) / 70;
						if (b < b8)
							b = b8;
						if (b < b6)
							b = b6;
						if (b < b4)
							b = b4;
						if (b < a)
							av = b;
						working_space[j] = av;
					}
				}
				for (j = i; j < ssize - i; j++)
					working_space[ssize + j] = working_space[j];
				if (direction == TSpectrum::kBackIncreasingWindow)
					i += sparse;
				else if (direction == TSpectrum::kBackDecreasingWindow)
					i -= sparse;
			} while ((direction == TSpectrum::kBackIncreasingWindow && i <= numberIterations) || (direction == TSpectrum::kBackDecreasingWindow && i >= 1));
		}

		if (compton == kTRUE) {
			for (i = 0, b2 = 0; i < ssize; i++){
				b1 = b2;
				a = working_space[i], b = spectrum[i];
				j = i;
				if (TMath::Abs(a - b) >= 1) {
					b1 = i - 1;
					if (b1 < 0)
						b1 = 0;
					yb1 = working_space[b1];
					for (b2 = b1 + 1, c = 0, priz = 0; priz == 0 && b2 < ssize; b2++){
						a = working_space[b2], b = spectrum[b2];
						c = c + b - yb1;
						if (TMath::Abs(a - b) < 1) {
							priz = 1;
							yb2 = b;
						}
					}
					if (b2 == ssize)
						b2 -= 1;
					yb2 = working_space[b2];
					if (yb1 <= yb2){
						for (j = b1, c = 0; j <= b2; j++){
							b = spectrum[j];
							c = c + b - yb1;
						}
						if (c > 1){
							c = (yb2 - yb1) / c;
							for (j = b1, d = 0; j <= b2 && j < ssize; j++){
								b = spectrum[j];
								d = d + b - yb1;
								a = c * d + yb1;
								working_space[ssize + j] = a;
							}
						}
					}

					else{
						for (j = b2, c = 0; j >= b1; j--){
							b = spectrum[j];
							c = c + b - yb2;
						}
						if (c > 1){
							c = (yb1 - yb2) / c;
							for (j = b2, d = 0; j >= b1 && j >= 0; j--){
								b = spectrum[j];
								d = d + b - yb2;
								a = c * d + yb2;
								working_space[ssize + j] = a;
							}
						}
					}
					i = b2;
				}
			}
		}

		for (j = 0; j < ssize; j++)
			spectrum[j] = working_space[ssize + j];
		delete[]working_space;
		return 0;
	}


	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &y_out, Double_t baseline)
	{
		if ((xs.size() != ys.size()) || (xs.size() <= 1)){
			y_out.clear();
			return;
		}
		y_out.resize(ys.size());
		Double_t prev = 0, dx;
		auto _begin_ = xs.begin();
		auto _end_ = xs.end();
		for (auto ix = xs.begin(), iy = ys.begin(); (ix != _end_); ++ix, ++iy) {
			dx = (ix == (_end_ - 1)) ? (*ix - *(ix - 1)) :
				(ix == _begin_) ? (*(ix + 1) - *ix) : (*(ix + 1) - *(ix - 1)) / 2;
			prev = dx*(*iy - baseline) + prev;
			y_out[ix-_begin_]=prev;
		}
	}

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &y_out, Double_t dx_hInt_t, Double_t baseline)
	{
		if ((xs.size() != ys.size()) || (xs.size() <= 1)) {
			y_out.clear();
			return;
		}
		y_out.resize(ys.size());
		Double_t prev = 0;
		auto _begin_ = xs.begin();
		auto _end_ = xs.end();
		for (auto ix = xs.begin(), iy = ys.begin(); (ix != _end_); ++ix, ++iy) {
			prev = dx_hInt_t*(*iy - baseline) + prev;
			y_out[ix - _begin_] = prev;
		}
	}

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, Double_t left, Double_t right, Double_t dx_hInt_t, Double_t baseline)
	{
		y_out.clear();
		x_out.clear();
		if ((xs.size() != ys.size()) || (xs.size() <= 1) || left >= right)
			return;
		Double_t prev = 0;
		auto _end_ = xs.end();
		auto _begin_ = xs.begin();
		for (auto ix = xs.begin(), iy = ys.begin(); (ix != _end_); ++ix, ++iy){
			if (!((*ix) < left) && !((*ix)>right)){
				prev = dx_hInt_t*(*iy - baseline) + prev;
				x_out.push_back(*ix);
				y_out.push_back(prev);
			}
		}
	}

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, Double_t left, Double_t right, Double_t baseline)
	{
		y_out.clear();
		x_out.clear();
		if ((xs.size() != ys.size()) || (xs.size() <= 1)||left>=right)
			return;
		Double_t prev = 0, dx;
		auto _end_ = xs.end();
		auto _begin_ = xs.begin();
		for (auto ix = xs.begin(), iy = ys.begin(); (ix != _end_); ++ix, ++iy){
			if (!((*ix) < left) && !((*ix)>right)){
				dx = (ix == (_end_ - 1)) ? (*ix - std::max(*(ix - 1), left)) : 
					(ix == _begin_) ? (std::min(*(ix + 1), right) - *ix) : std::min(0.5*(*(ix + 1) - *(ix-1)),left-right);
				prev = dx*(*iy - baseline) + prev;
				x_out.push_back(*ix);
				y_out.push_back(prev);
			}
		}
	}

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t dx_hInt_t, Double_t baseline)
	{
		if ((right == xs.end()) || (left == xs.end()) || (left >= right) || (xs.size() != ys.size())){
			x_out.clear();
			y_out.clear();
			return;
		}
		x_out.resize(right - left + 1);
		y_out.resize(right - left + 1);
		std::vector<Double_t>::iterator _end_ = ++right;
		std::vector<Double_t>::iterator _begin_ = left;
		Double_t prev = 0;
		for (auto ix = left, iy = ys.begin() + (left - xs.begin()); (ix != _end_); ++ix, ++iy){
			prev = dx_hInt_t*(*iy - baseline) + prev;
			y_out[ix - _begin_] = prev;
			x_out[ix - _begin_] = *ix;
		}
	}

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t baseline)
	{
		if ((right == xs.end()) ||(left == xs.end()) || (left >= right)||(xs.size()!=ys.size())){
			x_out.clear();
			y_out.clear();
			return;
		}
		x_out.resize(right - left + 1);
		y_out.resize(right - left + 1);
		std::vector<Double_t>::iterator _end_ = ++right;
		std::vector<Double_t>::iterator _begin_ = left;
		Double_t prev = 0, dx;
		for (auto ix = left, iy = ys.begin() + (left-xs.begin()); (ix != _end_); ++ix, ++iy){
			dx = (ix == (_end_ - 1)) ? (*ix - *(ix - 1)) :
				(ix == _begin_) ? (*(ix + 1) - *ix) : 0.5*(*(ix + 1) - *(ix - 1));
			prev = dx*(*iy - baseline) + prev;
			y_out[ix-_begin_]= prev;
			x_out[ix-_begin_]= *ix;
		}
	}

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t dx_hInt_t, Double_t baseline)
	{
		y_out = 0;
		if ((right == xs.end()) || (left == xs.end()) || (left >= right) || (xs.size() != ys.size()))
			return;
		std::vector<Double_t>::iterator _end_ = ++right;
		std::vector<Double_t>::iterator _begin_ = left;
		for (auto ix = left, iy = ys.begin() + (left - xs.begin()); (ix != _end_); ++ix, ++iy){
			y_out = dx_hInt_t*(*iy - baseline) + y_out;
		}
	}

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t baseline)
	{
		y_out = 0;
		if ((right == xs.end()) || (left == xs.end()) || (left >= right) || (xs.size() != ys.size()))
			return;
		std::vector<Double_t>::iterator _end_ = ++right;
		std::vector<Double_t>::iterator _begin_ = left;
		Double_t dx;
		for (auto ix = left, iy = ys.begin() + (left - xs.begin()); (ix != _end_); ++ix, ++iy){
			dx = (ix == (_end_ - 1)) ? (*ix - *(ix - 1)) :
				(ix == _begin_) ? (*(ix + 1) - *ix) : 0.5*(*(ix + 1) - *(ix - 1));
			y_out = dx*(*iy - baseline) + y_out;
		}
	}

	void apply_time_limits(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t x_left, Double_t x_right, Double_t dx_hInt_t)
	{
		if ((xs.size() != ys.size()) || x_left >= x_right||!(dx_hInt_t>0)||(xs.empty()))
			return;
		auto _end_ = xs.end();
		std::vector<Double_t>::iterator iterator_left = iter_add(xs.begin(),(Int_t)((x_left - *xs.begin()) / dx_hInt_t),_end_);
		Bool_t left_fallback = kFALSE;
		if ((iterator_left != _end_) && (iterator_left != (_end_-1))) { // ==end considered that the hInt_t worked
			if (!((*iterator_left <= x_left) && (*(iterator_left + 1) > x_left))) //hInt_t didn't work
				left_fallback = kTRUE;
		} else {
			if (x_left <= xs.back())
				left_fallback = kTRUE;
			if (iterator_left == _end_)
				if (x_left < *xs.begin()){
					left_fallback = kFALSE;
					iterator_left = xs.begin();
				}
		}
		std::vector<Double_t>::iterator iterator_right = iter_add(xs.begin(), (Int_t)((x_right - *xs.begin()) / dx_hInt_t), _end_);
		Bool_t right_fallback = kFALSE;
		if ((iterator_right != _end_) && (iterator_right != (_end_ - 1))) { // ==end considered that the hInt_t worked
			if (!((*iterator_right <= x_right) && (*(iterator_right + 1) > x_right))) //hInt_t didn't work
				right_fallback = kTRUE;
		} else {
			if (x_right <= xs.back())
				right_fallback = kTRUE;
			if (iterator_right == _end_)
				if (x_right < *xs.begin()) {
					right_fallback = kFALSE;
					iterator_right = xs.end();
				}
		}
		if (right_fallback && left_fallback) { //hInt_t failed for some reason
			for (auto ix = xs.begin(); (ix != _end_); ++ix) {
				if (!((*ix) > x_left))
					iterator_left = ix;
				if (!((*ix) > x_right))
					iterator_right = ix;
			}
		}
		if (right_fallback && !left_fallback) { //hInt_t failed for some reason
			for (auto ix = xs.begin(); (ix != _end_); ++ix) {
				if (!((*ix) > x_right))
					iterator_right = ix;
			}
		}
		if (left_fallback && !right_fallback) { //hInt_t failed for some reason
			for (auto ix = xs.begin(); (ix != _end_); ++ix) {
				if (!((*ix) > x_left))
					iterator_left = ix;
			}
		}

		iterator_right = iter_add(iterator_right,1,_end_);

		std::vector<Double_t>::iterator iterator_left_y = ys.begin() + (iterator_left - xs.begin());
		std::vector<Double_t>::iterator iterator_right_y = ys.begin() + (iterator_right - xs.begin());
		xs.erase(iterator_right, xs.end());
		xs.erase(xs.begin(), iterator_left);
		ys.erase(iterator_right_y, ys.end());
		ys.erase(ys.begin(), iterator_left_y);
	}

	void apply_time_limits(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t x_left, Double_t x_right)
	{
		if ((xs.size() != ys.size()) || x_left>=x_right)
			return;
		std::vector<Double_t>::iterator iterator_left = xs.begin(), iterator_right = xs.end() - 1;
		auto _end_ = xs.end();
		for (auto ix = xs.begin(); (ix != _end_); ++ix) {
			if (!((*ix) > x_left))
				iterator_left = ix;
			if (!((*ix) > x_right))
				iterator_right = ix;
		}
		++iterator_right;

		std::vector<Double_t>::iterator iterator_left_y = ys.begin() + (iterator_left - xs.begin());
		std::vector<Double_t>::iterator iterator_right_y = ys.begin() + (iterator_right - xs.begin());
		xs.erase(iterator_right, xs.end());
		xs.erase(xs.begin(), iterator_left);
		ys.erase(iterator_right_y, ys.end());
		ys.erase(ys.begin(), iterator_left_y);
	}

	std::vector<Double_t>::iterator find_x_iterator_by_value(std::vector<Double_t>::iterator &x_left, std::vector<Double_t>::iterator &x_right, Double_t x)
	{
		for (auto h = x_left; h != x_right; h++) //find in which poInt_t of the vector the maximum realizes
			if (!(*h > x) && !(*(h + 1) < x)){
				if ((x - *h) > (*(h + 1) - x))
					return h + 1;
				else
					return h;
				break;
			}
		return x_left; // mustn't occur
	}

	//hInt_t is the distance between 2 poInt_t in case they are all at equal distancess
	std::vector<Double_t>::iterator find_x_iterator_by_value(std::vector<Double_t>::iterator &x_left, std::vector<Double_t>::iterator &x_right, Double_t x, Double_t hInt_t)
	{
		std::vector<Double_t>::iterator approx_left = iter_add(x_left, (Int_t)((x - *x_left) / hInt_t),(x_right+1));
		if (approx_left==(x_right+1))
			return find_x_iterator_by_value(x_left, x_right, x); //fallback in case the hInt_t does not work
		if (!(*approx_left > x) && !(*(approx_left + 1) < x)) {
			if ((x - *approx_left) > (*(approx_left + 1) - x))
				return approx_left + 1;
			else
				return approx_left;
		}
		return find_x_iterator_by_value(x_left, x_right, x); //fallback in case the hInt_t does not work
	}

	void get_max(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator x_start, std::vector<Double_t>::iterator x_finish, std::vector<Double_t>::iterator &x_max, Double_t &y_max, Int_t N_trust)
	{
		N_trust = std::min((Int_t)(x_finish - x_start), N_trust); //other funtions return invalid results in this case
		if (xs.size() != ys.size()){
			x_max = xs.end();
			return;
		}
		Bool_t use_fit = kTRUE;
		if (N_trust < 3) {//2nd order polynom
			N_trust = 1;
			use_fit = kFALSE;
		}
		Int_t delta = N_trust / 3;
		y_max = *(ys.begin()+(x_start - xs.begin()));
		x_max = x_start;

		if (use_fit) {
			Polynom2Order fitter;
			auto _end_ = xs.end();
			auto _begin_ = xs.begin();
			std::size_t _size_ = xs.size();
			for (auto i = x_start; (i != _end_)&&(i<x_finish); ((delta<(_end_ - i)) ? i = i + delta : i = _end_)){
				Int_t shift = (Int_t)(_size_ - (i - _begin_) - N_trust) < 0 ? (_size_ - (i - _begin_) - N_trust) : 0; //accounts for the end

				TVectorD coefs;
				std::vector<Double_t>::iterator x_left = i + shift;
				fitter(xs, ys, x_left - _begin_, N_trust, coefs, *x_left);

				Double_t y_max_exact, x_max_exact;
				std::vector<Double_t>::iterator x_max_here;
				fitter.FindMaximum(x_max_here, x_max_exact, y_max_exact);
				if (y_max < y_max_exact){
					y_max = y_max_exact;
					x_max = x_max_here;
				}
			}
		} else {//do not use the fit
			auto _end_ = xs.end();
			for (auto i = x_start, j = ys.begin() + (x_start - xs.begin()); (i != _end_) && (i < x_finish); ++i, ++j){
				if (*j > y_max){
					x_max = i;
					y_max = *j;
				}
			}
		}
	}
	void get_min(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator x_start, std::vector<Double_t>::iterator x_finish, std::vector<Double_t>::iterator &x_min, Double_t &y_min, Int_t N_trust)
	{
		N_trust = std::min((Int_t)(x_finish - x_start), N_trust); //other funtions return invalid results in this case
		if (xs.size() != ys.size()){
			x_min = xs.end();
			return;
		}
		Bool_t use_fit = kTRUE;
		if (N_trust < 3) {//2nd order polynom
			N_trust = 1;
			use_fit = kFALSE;
		}
		Int_t delta = N_trust / 3;
		y_min = *(ys.begin() + (x_start - xs.begin()));
		x_min = x_start;

		if (use_fit){
			Polynom2Order fitter;
			auto _end_ = xs.end();
			auto _begin_ = xs.begin();
			std::size_t _size_ = xs.size();
			for (auto i = x_start; (i != _end_) && (i<x_finish); ((delta<(_end_ - i)) ? i = i + delta : i = _end_)){
				Int_t shift = (Int_t)(_size_- (i - _begin_) - N_trust) < 0 ? (_size_ - (i - _begin_) - N_trust) : 0; //accounts for the end

				TVectorD coefs;
				std::vector<Double_t>::iterator x_left = i + shift;
				fitter(xs, ys, x_left - _begin_, N_trust, coefs, *x_left);

				Double_t y_max_exact, x_max_exact;
				std::vector<Double_t>::iterator x_max_here;
				fitter.FindMinimum(x_max_here, x_max_exact, y_max_exact);
				if (y_min > y_max_exact) {
					y_min = y_max_exact;
					x_min = x_max_here;
				}
			}
		} else {//do not use the fit
			auto _end_ = xs.end();
			for (auto i = x_start, j = ys.begin() + (x_start - xs.begin()); (i != _end_)&& (i < x_finish); ++i, ++j){
				if (*j < y_min){
					x_min = i;
					y_min = *j;
				}
			}
		}
	}
	void get_max(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_max, Double_t &y_max, Int_t N_trust)
	{	get_max(xs, ys, xs.begin(), xs.end(), x_max, y_max, N_trust);}
	//after getting the peak, the next search must be started from (x_finish+1) or +(N_trust/2)!!!
	void find_next_peak(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_start,
		std::vector<Double_t>::iterator &x_finish, Double_t threshold, Int_t N_trust)//done - now every step by x uses fitting 
		//TODO: does not handle Double_t peak which middle is slightly above
		//the threshold (slightly means that 2nd order fit would Int_tersect the threshold)
	{
		x_finish = xs.end();
		if ((xs.size() != ys.size()) || ((xs.end() - x_start) < N_trust)){
			x_start = xs.end();
			x_finish = xs.begin();
			return;
		}
		std::vector<Double_t>::iterator minimal_iterator = x_start;
		x_start = xs.end();
		Bool_t use_fit = kTRUE;
		if (N_trust < 3) {//2nd order polynom
			N_trust = 1;
			use_fit = kFALSE;
		}
		Int_t delta = N_trust / 3;
		std::vector<Double_t>::iterator approx_x_left = minimal_iterator;
		std::vector<Double_t>::iterator approx_x_right = xs.end();
		Bool_t found_peak = kFALSE;

		auto _end_ = xs.end();
		if (use_fit) {
			Polynom2Order fitter;
			auto _begin_ = xs.begin();
			std::size_t _size_ = xs.size();
			for (auto i = minimal_iterator; (i != _end_); ((delta < (_end_ - i)) ? i = i + delta : i = _end_)) {
				Int_t shift = (Int_t)(_size_ - (i - _begin_) - N_trust) < 0 ? (_size_ - (i - _begin_) - N_trust) : 0; //accounts for the end
				TVectorD coefs;
				std::vector<Double_t>::iterator x_left = i + shift;
				fitter(xs, ys, (x_left - _begin_), N_trust, coefs, *x_left);

				//#ifdef _TEMP_CODE
				//				if ((*i <= 32.49) && (32.49 <= *(i + N_trust - 1))) {
				//					GraphicOutputManager man;
				//					Drawing *dr = man.GetDrawing("Peak find test "+std::to_string(*j), 0, ParameterPile::DrawEngine::Gnuplot);
				//					std::vector<Double_t> tmp_x, tmp_y;
				//					for (auto ti = i, tj = j; (ti < (i + N_trust)) && (tj < (j + N_trust)); ++ti, ++tj) {
				//						tmp_x.push_back(*ti);
				//						tmp_y.push_back(*tj);
				//					}
				//					dr->AddToDraw(tmp_x, tmp_y, "peak " + std::to_string(*j));
				//					TVectorD coefs;
				//					fitter.getCoefs(coefs);
				//					Double_t a = coefs[2];
				//					Double_t b = coefs[1]-2**x_left*coefs[2];
				//					Double_t c = coefs[0] - coefs[1] * *x_left + coefs[2] * *x_left * *x_left;
				//					std::stringstream aa, bb, cc;
				//					aa << std::setprecision(12) << a;
				//					bb << std::setprecision(12) << b;
				//					cc << std::setprecision(12) << c;
				//					dr->AddToDraw("a = " + aa.str() + "\nb = " + bb.str() + "\nc = " + cc.str() + "\nf(x) = a*x*x + b*x + c", "f(x)", "fit", "w l", 0);
				//					man.Draw();
				//				}
				//#endif
				std::vector<Double_t>::iterator x_Int_ter1, x_Int_ter2;
				Double_t x_Int_ter_exact1, x_Int_ter_exact2;
				fitter.FindInt_tersection(x_Int_ter1, x_Int_ter2, x_Int_ter_exact1, x_Int_ter_exact2, threshold);
				if (x_Int_ter2 != _end_){
					if (found_peak){
						if (fitter.Derivative(x_Int_ter_exact2) < 0.0){
							x_finish = x_Int_ter2;
							return;
						}
					} else {
						if (fitter.Derivative(x_Int_ter_exact2) > 0.0){
							x_start = x_Int_ter2;
							found_peak = kTRUE;
						}
					}
				}
				if (x_Int_ter1 != _end_){
					if (found_peak){
						if (fitter.Derivative(x_Int_ter_exact1) < 0.0){
							x_finish = x_Int_ter1;
							return;
						}
					} else {
						if (fitter.Derivative(x_Int_ter_exact1) > 0.0){
							x_start = x_Int_ter1;
							found_peak = kTRUE;
						}
					}
				}
			}
		} else { //do not use the fit
			for (auto i = minimal_iterator, j = ys.begin() + (minimal_iterator - xs.begin()); (i != _end_); ++i, ++j){
				if (found_peak){
					if ((*j) < threshold){
						x_finish = i - 1; //must be valid
						return;
					}
				} else {
					if ((*j) >= threshold){
						x_start = i;
						found_peak = kTRUE;
					}
				}
			}
		}
		if (found_peak&&x_start != _end_){
			x_finish = _end_ - 1;
			return;
		}
		x_start = _end_;
	}

	void find_peaks(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks, Double_t base_line, Double_t threshold, Int_t N_trust)
	{
		peaks.clear();

		if (xs.size() <= 1)
			return;
		std::vector<Double_t>::iterator x_peak_l = xs.begin(), x_peak_r = xs.begin();
		auto _end_ = xs.end();
		Double_t delta_x = *(++xs.begin()) - *(xs.begin());
		while (x_peak_l != _end_) {
			SignalOperations::find_next_peak(xs, ys, x_peak_l, x_peak_r, threshold, N_trust);
			if (x_peak_l != _end_){
				peak pk;
				pk.left = *x_peak_l;
				pk.right = *x_peak_r;
				SignalOperations::Integrate(xs, ys, pk.S, x_peak_l, x_peak_r, delta_x, base_line);
				std::vector<Double_t>::iterator pk_max;
				SignalOperations::get_max(xs, ys, x_peak_l, x_peak_r + 1, pk_max, pk.A, N_trust);
				if ((pk_max != _end_) && (pk.S > 0) && (pk.right>=pk.left))
					peaks.push_back(pk);
				x_peak_l = x_peak_r;
				Int_t delta_i = std::max(N_trust / 3, 1);
				x_peak_l = ((_end_ - x_peak_l) < delta_i) ? _end_ : (x_peak_l + delta_i);
			}
		}
	}

	//seaches peak from x_start, in difference to find_next peak this one first finds peak by threshold_finder and then finds its edges (wider 
	//than Int_tersection of threshold and signal) using thresh_edges.
	void find_next_peak_fine(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_start, std::vector<Double_t>::iterator &x_finish, Double_t &Amp,
		Double_t thresh_finder, Double_t thresh_edges, Int_t N_trust)
	{
		if (thresh_edges >= thresh_finder)
			thresh_edges = 0;
		x_finish = xs.end();
		std::vector<Double_t>::iterator minimal_iterator = x_start;
		std::vector<Double_t>::iterator pk_max;
		if ((xs.size() != ys.size()) || ((xs.end() - x_start)<N_trust))
			goto bad_return;
		find_next_peak(xs, ys, x_start, x_finish, thresh_finder, N_trust);
		if (x_start == xs.end())
			goto bad_return;
		SignalOperations::get_max(xs, ys, x_start, x_finish + 1, pk_max, Amp, N_trust);
		if (pk_max == xs.end())
			goto bad_return;

		Bool_t use_fit = kTRUE;
		if (N_trust < 3) {//2nd order polynom
			N_trust = 1;
			use_fit = kFALSE;
		}
		Int_t delta = N_trust / 3;
		if (use_fit){ //now extend edges
			Polynom2Order fitter;
			auto _end_ = xs.end();
			auto _begin_ = xs.begin();
			std::size_t _size_ = xs.size();
			for (auto i = x_finish; (i != _end_);((delta<(_end_ - i)) ? i = i + delta : i = _end_)){
				Int_t shift = (Int_t)(_size_ - (i - _begin_) - N_trust) < 0 ? (_size_ - (i - _begin_) - N_trust) : 0; //accounts for the end
				TVectorD coefs;
				std::vector<Double_t>::iterator x_left = i + shift;
				fitter(xs, ys, (i - _begin_) + shift, N_trust, coefs, *x_left);
				std::vector<Double_t>::iterator x_extr;
				std::vector<Double_t>::iterator x_Int_tersect1, x_Int_tersect2;
				std::vector<Double_t>::iterator x_Int_ter = _end_; //of Int_terest
				Double_t x_extr_exact, y_extr_exact;
				Double_t x_Int_ter_exact1, x_Int_ter_exact2;
				fitter.FindExtremum(x_extr, x_extr_exact, y_extr_exact);
				fitter.FindInt_tersection(x_Int_tersect1, x_Int_tersect2, x_Int_ter_exact1, x_Int_ter_exact2, thresh_edges);
				if (x_extr <= x_finish)
					x_extr = _end_; //accidently found extremum as peak maximum (in case x_start==x_finish)
				if (x_Int_tersect2 != _end_)
					if (fitter.Derivative(x_Int_ter_exact2) < 0)
						x_Int_ter = x_Int_tersect2;
				if (x_Int_tersect1 != _end_&&(x_Int_ter==_end_))
					if (fitter.Derivative(x_Int_ter_exact1) < 0)
						x_Int_ter = x_Int_tersect1;
				if (x_extr != _end_&&x_Int_ter!=_end_){
					if (y_extr_exact < thresh_edges)
						x_finish = x_Int_ter;
					else
						x_finish = x_extr;
					break;
				}
				if (x_extr != _end_){
					x_finish = x_extr;
					break;
				}
				if (x_Int_ter != _end_){
					x_finish = x_Int_ter;
					break;
				}
			}
			std::vector<Double_t>::reverse_iterator x_left_peak = std::vector<Double_t>::reverse_iterator(x_start);
			std::vector<Double_t>::reverse_iterator x_rend = std::vector<Double_t>::reverse_iterator(minimal_iterator);
			auto _rend_ = xs.rend();
			auto _rbegin_ = xs.rbegin();
			for (auto i = x_left_peak; (i != x_rend); ((delta<(x_rend - i)) ? i = i + delta : i = x_rend)){
				Int_t shift = (Int_t)(_size_ - (i - _rbegin_) - N_trust) < 0 ? (_size_ - (i - _rbegin_) - N_trust) : 0; //accounts for the rend
				TVectorD coefs;
				std::vector<Double_t>::iterator x_left = (i + shift).base();
				fitter(xs, ys, x_left - _begin_, N_trust, coefs, *x_left);
				std::vector<Double_t>::iterator x_extr;
				std::vector<Double_t>::iterator x_Int_tersect1, x_Int_tersect2;
				std::vector<Double_t>::iterator x_Int_ter = _end_; //of Int_terest
				Double_t x_extr_exact, y_extr_exact;
				Double_t x_Int_ter_exact1, x_Int_ter_exact2;
				fitter.FindExtremum(x_extr, x_extr_exact, y_extr_exact);
				fitter.FindInt_tersection(x_Int_tersect1, x_Int_tersect2, x_Int_ter_exact1, x_Int_ter_exact2, thresh_edges);
				if (x_extr >= x_start)
					x_extr = _end_; //accidently found extremum as peak maximum (in case x_start==x_finish)
				if (x_Int_tersect2 != _end_)
					if (fitter.Derivative(x_Int_ter_exact2) > 0)
						x_Int_ter = x_Int_tersect2;
				if (x_Int_tersect1 != _end_ && (x_Int_ter == _end_))
					if (fitter.Derivative(x_Int_ter_exact1) > 0)
						x_Int_ter = x_Int_tersect1;
				if (x_extr != _end_ && x_Int_ter != _end_){
					if (y_extr_exact < thresh_edges)
						x_start = x_Int_ter;
					else
						x_start = x_extr;
					break;
				}
				if (x_extr != _end_){
					x_start = x_extr;
					break;
				}
				if (x_Int_ter != _end_){
					x_start = x_Int_ter;
					break;
				}
			}
			if (x_start < minimal_iterator)
				x_start = minimal_iterator;
		} else { //do not use the fit
			auto _end_ = xs.end();
			auto _begin_ = xs.begin();
			for (auto i = x_finish, j = ys.begin() + (x_finish - xs.begin()); (i != _end_); ++i, ++j){
				if ((i == _begin_) || ((i + 1) == _end_))
					continue;
				if ((*j - thresh_edges)*(*(j - 1) - thresh_edges) <= 0){ //Int_tersection
					x_finish = i; //TODO: more exact?
					break;
				}
				//if (!((*j - *(j - 1))*(*(j + 1) - *j) <= 0)){ //extremum - too unstable for N_trust==1 and pretty much useless
				//	if (*j > thresh_edges)
				//		x_finish = i;
				//	break;
				//}
			}
			std::vector<Double_t>::reverse_iterator x_left_peak = std::vector<Double_t>::reverse_iterator(x_start);
			std::vector<Double_t>::reverse_iterator x_rend = std::vector<Double_t>::reverse_iterator(minimal_iterator);
			auto _rend_ = xs.rend();
			auto _rbegin_ = xs.rbegin();
			for (auto i = x_left_peak, j = ys.rbegin() + (x_left_peak - xs.rbegin()); (i != x_rend); ++i, ++j){
				if ((i == _rbegin_) || ((i + 1) == _rend_))
					continue;
				if ((*j - thresh_edges)*(*(j - 1) - thresh_edges) <= 0){
					x_finish = i.base(); //TODO: more exact?
					break;
				}
				if (!((*j - *(j - 1))*(*(j + 1) - *j) <= 0)){
					if (*j > thresh_edges) {
						x_start = i.base();
						break;
					}
				}
			}
			if (x_start < minimal_iterator)
				x_start = minimal_iterator;
		}
		return;
	bad_return:
		Amp = -1;
		x_start = xs.end();
		x_finish = xs.begin();
		return;
	}
	void find_peaks_fine(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks, Double_t base_line, Double_t threshold, Double_t threshold_edges, Int_t N_trust)
	{
		if (threshold_edges >= threshold)
			threshold_edges = 0;
		peaks.clear();
		if (xs.size() <= 1)
			return;
		Double_t delta_x = *(++xs.begin()) - *(xs.begin());
		std::vector<Double_t>::iterator x_peak_l = xs.begin(), x_peak_r = xs.begin();
		auto _end_ = xs.end();
		while (x_peak_l != _end_) {
			Double_t Amp;
			SignalOperations::find_next_peak_fine(xs, ys, x_peak_l, x_peak_r,Amp, threshold, threshold_edges, N_trust);
			if (x_peak_l != _end_) {
				peak pk;
				pk.left = *x_peak_l;
				pk.right = *x_peak_r;
				pk.A = Amp-base_line;
				SignalOperations::Integrate(xs, ys, pk.S, x_peak_l, x_peak_r, delta_x, base_line);
				if ((pk.S>0) && (pk.A>0)&&(pk.right>=pk.left))
					peaks.push_back(pk);
				x_peak_l = x_peak_r;
				Int_t delta_i = std::max(N_trust / 3, 1);
				x_peak_l = ((_end_ - x_peak_l) < delta_i) ? _end_ : (x_peak_l + delta_i);
			}
		}
	}

	void find_next_extremum_faster(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_start, Int_t N_trust)
	{	
		Int_t delta = std::max(N_trust / 2, 1);
		if ((xs.end() - x_start < (2 * delta + 1)) ||(xs.size()!=ys.size())){
			x_start = xs.end(); //not found
			return;
		}
		Bool_t min = kTRUE, max = kTRUE;
		auto _end_ = xs.end();
		for (auto i = x_start, j = ys.begin() + (x_start - xs.begin()); (i != _end_); ++i, ++j){
			if ((i <= x_start + delta) || (i >= (_end_ - delta - 1)))
				continue;
			min = kTRUE;
			max = kTRUE;
			for (auto jj = j - delta; jj <= (j + delta); ++jj) {
				if (*jj < *j) //if this extremum corresponds to minimum then all elements in [-delta,+delta] must be >=*j
					min = kFALSE;
				if (*jj > *j)
					max = kFALSE;
				if (!min&&!max)
					break;
			}
			if (min || max){
				x_start = i;
				return;
			}
		}
		x_start = _end_; //not found
	}

	void find_previous_extremum_faster(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::reverse_iterator &x_start, Int_t N_trust)
	{
		Int_t delta = std::max(N_trust / 2, 1);
		if ((xs.rend() - x_start < (2 * delta + 1)) || (xs.size() != ys.size())){
			x_start = xs.rend(); //not found
			return;
		}
		auto _rend_ = xs.rend();
		for (auto i = x_start, j = ys.rbegin() + (x_start - xs.rbegin()); (i != _rend_); ++i, ++j){
			if ((i <= (x_start + delta)) || (i >= (_rend_ - delta - 1)))
				continue;
			Bool_t min = kTRUE, max = kTRUE;
			for (auto jj = j - delta; jj <= (j + delta); ++jj) {
				if (*jj < *j) //if this extremum corresponds to minimum then all elements in [-delta,+delta] must be >=*j
					min = kFALSE;
				if (*jj > *j)
					max = kFALSE;
				if (!min&&!max)
					break;
			}
			if (min || max){
				x_start = i;
				return;
			}
		}
		x_start = _rend_; //not found
	}

	void find_next_extremum(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_start, Int_t N_trust)
	{
		if (xs.size() != ys.size()){
			x_start = xs.end();
			return;
		}
		Bool_t use_fit = kTRUE;
		if (N_trust < 3) {//2nd order polynom
			N_trust = 1;
			use_fit = kFALSE;
		}
		Int_t delta = N_trust / 3;
		if (use_fit) {
			Polynom2Order fitter;
			auto _end_ = xs.end();
			auto _begin_ = xs.begin();
			std::size_t _size_ = xs.size();
			for (auto i = x_start; (i != _end_); ((delta<(_end_ - i)) ? i = i + delta : i = _end_)) {
				Int_t shift = (Int_t)(_size_ - (i - _begin_) - N_trust) < 0 ? (_size_ - (i - _begin_) - N_trust) : 0; //accounts for the end
				TVectorD coefs;
				std::vector<Double_t>::iterator x_left = i + shift;
				fitter(xs, ys, (i - _begin_) + shift, N_trust, coefs, *x_left);
				std::vector<Double_t>::iterator x_extr;
				Double_t x_extr_exact, y_extr_exact;
				fitter.FindExtremum(x_extr, x_extr_exact, y_extr_exact);
				if (x_extr != _end_){
					if (x_extr <= x_start)
						continue; //accidently found previous extremum
					x_start = x_extr;
					return;
				}
			}
		} else { //do not use the fit
			auto _end_ = xs.end();
			auto _begin_ = xs.begin();
			for (auto i = x_start, j = ys.begin() +(x_start-xs.begin()) ; (i != _end_); ++i, ++j){
				if ((i == _begin_) || ((i + 1) == _end_) || x_start == i)
					continue;
				if (!((*j - *(j - 1))*(*(j + 1) - *j) <= 0)){
					x_start = i;
					return;
				}
			}
		}
		x_start = xs.end(); //not found
	}

	void find_previous_extremum(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::reverse_iterator &x_start, Int_t N_trust)
	{
		if (xs.size() != ys.size()){
			x_start = xs.rend();
			return;
		}
		Bool_t use_fit = kTRUE;
		if (N_trust < 3) {//2nd order polynom
			N_trust = 1;
			use_fit = kFALSE;
		}
		Int_t delta = N_trust / 3;
		if (use_fit) {
			Polynom2Order fitter;
			auto _end_ = xs.end();
			auto _begin_ = xs.begin();
			auto _rend_ = xs.rend();
			auto _rbegin_ = xs.rbegin();
			std::size_t _size_ = xs.size();
			for (auto i = x_start; (i != _rend_); ((delta<(_rend_ - i)) ? i = i + delta : i = _rend_)){
				Int_t shift = (Int_t)(_size_ - (i - _rbegin_) - N_trust) < 0 ? (_size_ - (i - _rbegin_) - N_trust) : 0; //accounts for the end
				TVectorD coefs;
				std::vector<Double_t>::reverse_iterator x_left = i + shift;
				fitter(xs, ys, (i.base() - _begin_) + shift, N_trust, coefs, *x_left);
				std::vector<Double_t>::iterator x_extr;
				Double_t x_extr_exact, y_extr_exact;
				fitter.FindExtremum(x_extr, x_extr_exact, y_extr_exact);
				if (x_extr != _end_) {
					if (x_extr >= x_start.base())
						continue; //accidently found previous extremum
					x_start = std::vector<Double_t>::reverse_iterator(x_extr);
					return;
				}
			}
		} else { //do not use the fit
			auto _rend_ = xs.rend();
			auto _rbegin_ = xs.rbegin();
			for (auto i = x_start, j = ys.rbegin() + (x_start - xs.rbegin()); (i != _rend_); ++i, ++j){
				if ((i == _rbegin_) || ((i + 1) == _rend_) || i == x_start)
					continue;
				if (!((*j - *(j - 1))*(*(j + 1) - *j) <= 0)){
					x_start = i;
					return;
				}
			}
		}
		x_start = xs.rend(); //not found
	}

	void spread_peaks(Double_t x_left, Double_t x_right, std::deque<peak> &peaks, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out)
	{
		//doesn't check whether peaks are valid (e.g. peak.A<0)
#ifdef _HOTFIX_CLEAR_MEMORY
		std::vector<Double_t>().swap(xs_out);
		std::vector<Double_t>().swap(ys_out);
#else
		xs_out.clear();
		ys_out.clear();
#endif
		for (auto pp = peaks.begin(); pp != peaks.end(); ++pp){
			Bool_t is_first = (pp == peaks.begin());
			Double_t x_l = is_first ? x_left - 1e-7 : ((pp - 1)->left + (pp - 1)->right) / 2;
			Double_t x_r = (pp->left + pp->right)/2;
			Double_t y_v = is_first ? 0.5*pp->S : 0.5*((pp - 1)->S + pp->S);
			y_v /= x_r - x_l;
			xs_out.push_back(x_l + 1e-7);
			xs_out.push_back(x_r - 1e-7);
			ys_out.push_back(y_v);
			ys_out.push_back(y_v);
		}
		Double_t x_l = peaks.empty() ? x_left : 0.5*(peaks.back().left + peaks.back().right);
		Double_t x_r = x_right;
		Double_t y_v = peaks.empty() ? 0 : 0.5*(peaks.back().S);
		y_v /= x_r - x_l;
		xs_out.push_back(x_l + 1e-7);
		xs_out.push_back(x_r);
		ys_out.push_back(y_v);
		ys_out.push_back(y_v);
	}
	//in comparisson to ^ has more smooth result
	void spread_peaks_v2(Double_t x_left, Double_t x_right, std::deque<peak> &peaks, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out, Double_t min_dx)
	{
		//doesn't check whether peaks are valid (e.g. peak.A<0)
#ifdef _HOTFIX_CLEAR_MEMORY
		std::vector<Double_t>().swap(xs_out);
		std::vector<Double_t>().swap(ys_out);
#else
		xs_out.clear();
		ys_out.clear();
#endif
		Double_t current_left_x;
		Bool_t uniting_several_peaks = kFALSE;
		Double_t y_v=0;
		for (auto pp = peaks.begin(); pp != peaks.end(); ++pp){
			Bool_t is_first = (pp == peaks.begin());
			Double_t x_l = uniting_several_peaks ? current_left_x :
				(is_first ? x_left - 1e-7 : ((pp - 1)->left + (pp - 1)->right) / 2);
			Double_t x_r = (pp->left + pp->right) / 2;
			y_v += is_first ? 0.5*pp->S : 0.5*((pp - 1)->S + pp->S);
			if ((x_r - x_l) < min_dx){
				uniting_several_peaks = kTRUE;
				current_left_x = x_l;
				continue;
			} else {
				uniting_several_peaks = kFALSE;
			}
			y_v /= x_r - x_l;
			xs_out.push_back(x_l + 1e-7);
			xs_out.push_back(x_r - 1e-7);
			ys_out.push_back(y_v);
			ys_out.push_back(y_v);
			y_v = 0;
		}
		Double_t x_l = uniting_several_peaks ? current_left_x : 
			peaks.empty() ? x_left : 0.5*(peaks.back().left + peaks.back().right);
		Double_t x_r = x_right;
		Double_t dx = x_r - x_l;
		if (xs_out.size() >= 2)
			dx = std::max(x_r - *(xs_out.end() - 2), std::max(min_dx,dx));
		else
			dx = std::max(min_dx,dx);
		y_v += peaks.empty() ? 0 : 0.5*(peaks.back().S);
		y_v /= dx;
		xs_out.push_back(x_l + 1e-7);
		xs_out.push_back(x_r);
		ys_out.push_back(y_v);
		ys_out.push_back(y_v);
	}

	void peaks_to_yx(Double_t x_left, Double_t x_right, std::deque<peak> &peaks, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out)
	{
#ifdef _HOTFIX_CLEAR_MEMORY
		std::vector<Double_t>().swap(xs_out);
		std::vector<Double_t>().swap(ys_out);
#else
		xs_out.clear();
		ys_out.clear();
#endif
		xs_out.push_back(x_left);
		ys_out.push_back(0);
		for (auto pp = peaks.begin(); pp != peaks.end(); ++pp) {
			Double_t y_v = pp->S / (pp->right - pp->left);
			xs_out.push_back(pp->left - 1e-7);
			ys_out.push_back(0);
			xs_out.push_back(pp->left + 1e-7);
			ys_out.push_back(y_v);
			xs_out.push_back(pp->right - 1e-7);
			ys_out.push_back(y_v);
			xs_out.push_back(pp->right + 1e-7);
			ys_out.push_back(0);
		}
		xs_out.push_back(x_right);
		ys_out.push_back(0);
	}

	void spread_peaks(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out)
	{
#ifdef _HOTFIX_CLEAR_MEMORY
		std::vector<Double_t>().swap(xs_out);
		std::vector<Double_t>().swap(ys_out);
#else
		xs_out.clear();
		ys_out.clear();
#endif
#ifndef _USE_DEQUE
		xs_out.reserve(xs_in.size());
		ys_out.reserve(ys_in.size());
#endif
		Bool_t first = kTRUE; //eliminating the same xs
		Double_t x_prev;
		Double_t y_val;
		for (auto i = xs_in.begin(), j = ys_in.begin(); (i != xs_in.end()) && (j != ys_in.end()); ++i, ++j) {
			if (first){
				x_prev = *i;
				y_val = *j;
				first= kFALSE;
			} else {
				if (x_prev == *i)
					y_val += *j;
				else {
					xs_out.push_back(x_prev);
					ys_out.push_back(y_val);
					x_prev = *i;
					y_val = *j;
				}
			}
		}
		xs_out.push_back(x_prev);
		ys_out.push_back(y_val);
		xs_in = xs_out;
		ys_in = ys_out; //eliminated the same xs

		if ((xs_in.size() < 2)||ys_in.size()!=xs_in.size())
			return;
#ifndef _USE_DEQUE
		xs_out.reserve(2*xs_in.size());
		ys_out.reserve(2*ys_in.size());
#endif
		for (auto i = xs_in.begin(), j = ys_in.begin(); (i != (xs_in.end() - 1)) && (j != (ys_in.end() - 1)); ++i, ++j){
			xs_out.push_back(*i);
			xs_out.push_back(*(i+1)-1e-7); //so I can plot it with gnuplot
			ys_out.push_back((*(j + 1) + (*j)) / (*(i + 1) - *i)); //xs are now different.
			ys_out.push_back((*(j + 1) + (*j)) / (*(i + 1) - *i));
		}
	}

	void exclude_peaks(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in, std::deque<peak> &peaks)
	{
		if (xs_in.size() != ys_in.size())
			return;
		std::vector<Double_t> x_out, y_out;
#ifndef _USE_DEQUE
		x_out.reserve(xs_in.size());
		y_out.reserve(ys_in.size());
#endif
		auto _end_ = xs_in.end();
		Bool_t do_account;
		auto pks_end_ = peaks.end();
		for (auto i = xs_in.begin(), j = ys_in.begin(); (i != _end_); ++i, ++j){
			do_account = kTRUE;
			for (auto pp = peaks.begin(); pp != pks_end_; ++pp)
				if (!(*i<(*pp).left) && !(*i>(*pp).right)) {
					do_account = kFALSE;
					break;
				}
			if (do_account) {
				x_out.push_back(*i);
				y_out.push_back(*j);
			}
		}
		xs_in = x_out;
		ys_in = y_out;
	}

	void substract_baseline(std::vector<Double_t> &ys_in, Double_t base_line)
	{
		auto _end_ = ys_in.end();//it's cheaper to call end() only once
		for (auto i = ys_in.begin(); i != _end_; ++i)
			*i -= base_line;
	}
	void substract_baseline(std::vector<Double_t> &ys_in, std::vector<Double_t> &base_ys)
	{
		if (ys_in.size() != base_ys.size())
			return;
		auto _end_ = ys_in.end();
		for (auto i = ys_in.begin(), j = base_ys.begin(); (i != _end_); ++i, ++j)
			*i -= *j;
	}

	//required for substracting ROOT's baseline which is calculated only for some range
	void substract_baseline(std::vector<Double_t>& xs_in, std::vector<Double_t> &ys_in, std::vector<Double_t> &base_xs, std::vector<Double_t> &base_ys, Double_t baseline_baseline)
	{
		if ((xs_in.size() != ys_in.size()) || (base_xs.size() != base_ys.size()))
			return;
		auto x = xs_in.begin();
		auto x_base = base_xs.begin();
		auto y = ys_in.begin();
		auto y_base = base_ys.begin();
		auto base_end_ = base_xs.end();
		auto in_end_ = xs_in.end();
		while (x != in_end_ && x_base != base_end_) {
			if (*x == *x_base){
				*y -= (*y_base - baseline_baseline);
				++x;
				++x_base;
				++y;
				++y_base;
				continue;
			}
			if (*x < *x_base){
				++x;
				++y;
				continue;
			}
			if (*x > *x_base){
				++x_base;
				++y_base;
				continue;
			}
		}
	}

	Double_t Mean(std::vector<Double_t>::iterator first, std::vector<Double_t>::iterator last)
	{
		// Return the weighted mean of an array defined by the iterators.
		Double_t sum = 0;
		Double_t sumw = 0;
		while (first != last) {
			sum += *first;
			sumw += 1;
			++first;
		}
		return sum / sumw;
	}

	Double_t RMS(std::vector<Double_t>::iterator first, std::vector<Double_t>::iterator last)
	{
		// Return the Standard Deviation of an array defined by the iterators.
		// Note that this function returns the sigma(standard deviation) and
		// not the root mean square of the array.

		// Use the two pass algorithm, which is slower (! a factor of 2) but much more 
		// precise.  Since we have a vector the 2 pass algorithm is still faster than the 
		// Welford algorithm. (See also ROOT-5545)

		Double_t n = 0;
		Double_t tot = 0;
		Double_t mean = Mean(first, last);
		while (first != last) {
			Double_t x = Double_t(*first);
			tot += (x - mean)*(x - mean);
			++first;
			n+=1;
		}
		Double_t rms = (n > 1) ? TMath::Sqrt(tot / (n - 1)) : 0.0;
		return rms;
	}
	//Double_t Mean(std::deque<std::deque<peak>>::iterator first, std::deque<std::deque<peak>>::iterator last, std::function<Double_t(peak& pk)> &value_picker)
	//{
	//	// Return the weighted mean of an array defined by the iterators.
	//	Double_t sum = 0;
	//	Double_t sumw = 0;
	//	while (first != last) {
	//		auto _end_ = first->end();
	//		for (auto i = first->begin(); i != _end_; ++i)
	//			sum += value_picker(*i);
	//		sumw += first->size();
	//		++first;
	//	}
	//	return sum / sumw;
	//}

	//Double_t RMS(std::deque<std::deque<peak>>::iterator first, std::deque<std::deque<peak>>::iterator last, std::function<Double_t(peak& pk)> &value_picker)
	//{
	//	// Return the Standard Deviation of an array defined by the iterators.
	//	// Note that this function returns the sigma(standard deviation) and
	//	// not the root mean square of the array.

	//	// Use the two pass algorithm, which is slower (! a factor of 2) but much more 
	//	// precise.  Since we have a vector the 2 pass algorithm is still faster than the 
	//	// Welford algorithm. (See also ROOT-5545)

	//	Double_t n = 0;
	//	Double_t tot = 0;
	//	Double_t mean = Mean(first, last, value_picker);
	//	while (first != last) {
	//		auto _end_ = first->end();
	//		for (auto i = first->begin(); i != _end_; ++i){
	//			Double_t x = value_picker(*i);
	//			tot += (x - mean)*(x - mean);
	//		}
	//		n += first->size();
	//		++first;
	//	}
	//	Double_t rms = (n > 1) ? TMath::Sqrt(tot / (n - 1)) : 0.0;
	//	return rms;
	//}

};
