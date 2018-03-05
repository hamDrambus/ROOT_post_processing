#ifndef SIGNAL_OPERATIONS_H
#define SIGNAL_OPERATIONS_H

#include "GlobalParameters.h"

namespace SignalOperations
{
	void signal_from_file(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::string fname);
	void signal_to_file(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::string fname);

	void invert_y(std::vector<Double_t> &x_in_out, std::vector<Double_t> &y_in_out);
	Double_t find_baseline_by_median(Double_t approx, std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks);
	Double_t find_baseline_by_Int_tegral(Double_t approx, std::vector<Double_t> &xs, std::vector<Double_t> &ys);
	Double_t find_baseline_by_Int_tegral(Double_t approx, std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks);
	void find_baseline_by_ROOT(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_advanced(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_v2(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_v3(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_v4(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_v5(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_v6(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_v7(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	void find_baseline_by_ROOT_v8(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &ys_out);
	const char *find_background_v_raw(Double_t *spectrum, Int_t ssize,
		Int_t numberIterations,
		Int_t direction, Int_t filterOrder,
		Bool_t smoothing, Int_t smoothWindow,
		Bool_t compton);
	const char *find_background_v_0(Double_t *spectrum, Int_t ssize,
		Int_t numberIterations,
		Int_t direction, Int_t filterOrder,
		Bool_t smoothing, Int_t smoothWindow,
		Bool_t compton, Int_t sparse = 1);

	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &y_out, Double_t baseline);
	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &y_out, Double_t dx_hInt_t, Double_t baseline);
	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, Double_t left, Double_t right, Double_t baseline);
	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, Double_t left, Double_t right, Double_t dx_hInt_t, Double_t baseline);
	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t baseline);
	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t> &x_out, std::vector<Double_t> &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t dx_hInt_t, Double_t baseline);
	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t baseline);
	void Integrate(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t &y_out, std::vector<Double_t>::iterator left, std::vector<Double_t>::iterator right, Double_t dx_hInt_t, Double_t baseline);
	//^area is [a,b], not (a,b)
	void apply_time_limits(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t x_left, Double_t x_right);
	void apply_time_limits(std::vector<Double_t> &xs, std::vector<Double_t> &ys, Double_t x_left, Double_t x_right, Double_t dx_hInt_t);

	std::vector<Double_t>::iterator find_x_iterator_by_value(std::vector<Double_t>::iterator &x_left, std::vector<Double_t>::iterator &x_right, Double_t x);
	std::vector<Double_t>::iterator find_x_iterator_by_value(std::vector<Double_t>::iterator &x_left, std::vector<Double_t>::iterator &x_right, Double_t x, Double_t hInt_t);
	void get_max(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_max, Double_t &y_max, Int_t N_trust = 1);
	void get_max(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator x_start, std::vector<Double_t>::iterator x_finish, std::vector<Double_t>::iterator &x_max, Double_t &y_max, Int_t N_trust = 1);
	void get_min(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator x_start, std::vector<Double_t>::iterator x_finish, std::vector<Double_t>::iterator &x_min, Double_t &y_min, Int_t N_trust = 1);
	//finds max only at [x_start, x_finish)
	void find_next_peak(std::vector<Double_t>&xs, std::vector<Double_t>&ys, std::vector<Double_t>::iterator &x_start, std::vector<Double_t>::iterator &x_finish, Double_t threshold, Int_t N_trust);
	//seaches peak from x_start
	void find_peaks(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks, Double_t base_line, Double_t threshold, Int_t N_trust);
	//^these two aren't fully right functions, but ok for selection of runs by PMT signal i.e. coarse operations
	void find_next_peak_fine(std::vector<Double_t>&xs, std::vector<Double_t>&ys, std::vector<Double_t>::iterator &x_start, std::vector<Double_t>::iterator &x_finish, Double_t &Amp,
		Double_t thresh_finder, Double_t thresh_edges, Int_t N_trust);
	//seaches peak from x_start, in difference to find_next peak this one first finds peak by threshold_finder and then finds its edges (wider 
	//than Int_tersection of threshold and signal) using thresh_edges.
	void find_peaks_fine(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::deque<peak> &peaks, Double_t base_line, Double_t threshold, Double_t threshold_edges, Int_t N_trust);

	void find_next_extremum_faster(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_start, Int_t N_trust = 5);
	void find_previous_extremum_faster(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::reverse_iterator &x_start, Int_t N_trust = 5);
	void find_next_extremum(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::iterator &x_start, Int_t N_trust);
	//N trust is the number of poInt_ts over which 2nd order Int_terpolation takes effect
	//done: TODO: theese 5 above functions rely on the 2nd order polynom properties, so it will be neater to create 2nd order polynome on the range [a,b]
	//class with such methods as getting maximum, obtaining iterators and such.
	void find_previous_extremum(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::vector<Double_t>::reverse_iterator &x_start, Int_t N_trust);
	void spread_peaks(Double_t x_left, Double_t x_right, std::deque<peak> &peaks, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out);
	//in comparisson to ^ has more smooth result
	void spread_peaks_v2(Double_t x_left, Double_t x_right, std::deque<peak> &peaks, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out, Double_t min_dx);
	void peaks_to_yx(Double_t x_left, Double_t x_right, std::deque<peak> &peaks, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out);
	void spread_peaks(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in, std::vector<Double_t> &xs_out, std::vector<Double_t>& ys_out);

	void exclude_peaks(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in, std::deque<peak> &peaks);

	void substract_baseline(std::vector<Double_t> &ys_in, Double_t base_line);
	void substract_baseline(std::vector<Double_t> &ys_in, std::vector<Double_t> &base_ys);
	void substract_baseline(std::vector<Double_t>& xs_in, std::vector<Double_t> &ys_in, std::vector<Double_t> &base_xs, std::vector<Double_t> &base_ys, Double_t baseline_baseline);

	Double_t Mean(std::vector<Double_t>::iterator first, std::vector<Double_t>::iterator last);
	Double_t RMS(std::vector<Double_t>::iterator first, std::vector<Double_t>::iterator last);
	/*Double_t Mean(std::deque<std::deque<peak>>::iterator first, std::deque<std::deque<peak>>::iterator last, std::function<Double_t(peak& pk)> &value_picker);
	Double_t RMS(std::deque<std::deque<peak>>::iterator first, std::deque<std::deque<peak>>::iterator last, std::function<Double_t(peak& pk)> &value_picker);*/
};

#endif
