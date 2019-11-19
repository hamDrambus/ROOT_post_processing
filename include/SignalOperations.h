#ifndef SIGNAL_OPERATIONS_H
#define SIGNAL_OPERATIONS_H

#include "GlobalParameters.h"

namespace SignalOperations
{
	void signal_from_file(std::vector<double> &xs, std::vector<double> &ys, std::string fname);
	void signal_to_file(std::vector<double> &xs, std::vector<double> &ys, std::string fname);

	void invert_y(std::vector<double> &x_in_out, std::vector<double> &y_in_out);
	double find_baseline_by_median(double approx, std::vector<double> &xs, std::vector<double> &ys, std::deque<peak> &peaks);
	double find_baseline_by_integral(double approx, std::vector<double> &xs, std::vector<double> &ys);
	double find_baseline_by_integral(double approx, std::vector<double> &xs, std::vector<double> &ys, std::deque<peak> &peaks);
	void find_baseline_by_ROOT(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_advanced(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_v2(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_v3(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_v4(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_v5(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_v6(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_v7(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	void find_baseline_by_ROOT_v8(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ys_out);
	const char *find_background_v_raw(double *spectrum, int ssize,
		int numberIterations,
		int direction, int filterOrder,
		Bool_t smoothing, int smoothWindow,
		Bool_t compton);
	const char *find_background_v_0(double *spectrum, int ssize,
		int numberIterations,
		int direction, int filterOrder,
		Bool_t smoothing, int smoothWindow,
		Bool_t compton, int sparse = 1);

	void Integrate(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &y_out, double baseline);
	void Integrate(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &y_out, double dx_hint, double baseline);
	void Integrate(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &x_out, std::vector<double> &y_out, double left, double right, double baseline);
	void Integrate(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &x_out, std::vector<double> &y_out, double left, double right, double dx_hint, double baseline);
	void Integrate(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &x_out, std::vector<double> &y_out, std::vector<double>::iterator left, std::vector<double>::iterator right, double baseline);
	void Integrate(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &x_out, std::vector<double> &y_out, std::vector<double>::iterator left, std::vector<double>::iterator right, double dx_hint, double baseline);
	void Integrate(std::vector<double> &xs, std::vector<double> &ys, double &y_out, std::vector<double>::iterator left, std::vector<double>::iterator right, double baseline);
	void Integrate(std::vector<double> &xs, std::vector<double> &ys, double &y_out, std::vector<double>::iterator left, std::vector<double>::iterator right, double dx_hint, double baseline);
	//^area is [a,b], not (a,b)
	void apply_time_limits(std::vector<double> &xs, std::vector<double> &ys, double x_left, double x_right);
	void apply_time_limits(std::vector<double> &xs, std::vector<double> &ys, double x_left, double x_right, double dx_hint);

	std::vector<double>::iterator find_x_iterator_by_value(std::vector<double>::iterator &x_left, std::vector<double>::iterator &x_right, double x);
	std::vector<double>::iterator find_x_iterator_by_value(std::vector<double>::iterator &x_left, std::vector<double>::iterator &x_right, double x, double hint);
	void get_max(std::vector<double> &xs, std::vector<double> &ys, std::vector<double>::iterator &x_max, double &y_max, int N_trust = 1);
	void get_max(std::vector<double> &xs, std::vector<double> &ys, std::vector<double>::iterator x_start, std::vector<double>::iterator x_finish, std::vector<double>::iterator &x_max, double &y_max, int N_trust = 1);
	void get_min(std::vector<double> &xs, std::vector<double> &ys, std::vector<double>::iterator x_start, std::vector<double>::iterator x_finish, std::vector<double>::iterator &x_min, double &y_min, int N_trust = 1);
	//finds max only at [x_start, x_finish)
	void find_next_peak(std::vector<double>&xs, std::vector<double>&ys, std::vector<double>::iterator &x_start, std::vector<double>::iterator &x_finish, double threshold, int N_trust);
	//seaches peak from x_start
	void find_peaks(std::vector<double> &xs, std::vector<double> &ys, std::deque<peak> &peaks, double base_line, double threshold, int N_trust);
	//^these two aren't fully right functions, but ok for selection of runs by PMT signal i.e. coarse operations
	void find_next_peak_fine(std::vector<double>&xs, std::vector<double>&ys, std::vector<double>::iterator &x_start, std::vector<double>::iterator &x_finish, double &Amp,
		double thresh_finder, double thresh_edges, int N_trust);
	//seaches peak from x_start, in difference to find_next peak this one first finds peak by threshold_finder and then finds its edges (wider 
	//than intersection of threshold and signal) using thresh_edges.
	void find_peaks_fine(std::vector<double> &xs, std::vector<double> &ys, std::deque<peak> &peaks, double base_line, double threshold, double threshold_edges, int N_trust);

	void find_next_extremum_faster(std::vector<double> &xs, std::vector<double> &ys, std::vector<double>::iterator &x_start, int N_trust = 5);
	void find_previous_extremum_faster(std::vector<double> &xs, std::vector<double> &ys, std::vector<double>::reverse_iterator &x_start, int N_trust = 5);
	void find_next_extremum(std::vector<double> &xs, std::vector<double> &ys, std::vector<double>::iterator &x_start, int N_trust);
	//N trust is the number of points over which 2nd order interpolation takes effect
	//done: TODO: theese 5 above functions rely on the 2nd order polynom properties, so it will be neater to create 2nd order polynome on the range [a,b]
	//class with such methods as getting maximum, obtaining iterators and such.
	void find_previous_extremum(std::vector<double> &xs, std::vector<double> &ys, std::vector<double>::reverse_iterator &x_start, int N_trust);
	void spread_peaks(double x_left, double x_right, std::deque<peak> &peaks, std::vector<double> &xs_out, std::vector<double>& ys_out);
	//in comparisson to ^ has more smooth result
	void spread_peaks_v2(double x_left, double x_right, std::deque<peak> &peaks, std::vector<double> &xs_out, std::vector<double>& ys_out, double min_dx);
	void peaks_to_yx(double x_left, double x_right, std::deque<peak> &peaks, std::vector<double> &xs_out, std::vector<double>& ys_out);
	void spread_peaks(std::vector<double> &xs_in, std::vector<double> &ys_in, std::vector<double> &xs_out, std::vector<double>& ys_out);

	void exclude_peaks(std::vector<double> &xs_in, std::vector<double> &ys_in, std::deque<peak> &peaks);

	void substract_baseline(std::vector<double> &ys_in, double base_line);
	void substract_baseline(std::vector<double> &ys_in, std::vector<double> &base_ys);
	void substract_baseline(std::vector<double>& xs_in, std::vector<double> &ys_in, std::vector<double> &base_xs, std::vector<double> &base_ys, double baseline_baseline);

	double Mean(std::vector<double>::iterator first, std::vector<double>::iterator last);
	double RMS(std::vector<double>::iterator first, std::vector<double>::iterator last);
	/*double Mean(std::deque<std::deque<peak>>::iterator first, std::deque<std::deque<peak>>::iterator last, std::function<double(peak& pk)> &value_picker);
	double RMS(std::deque<std::deque<peak>>::iterator first, std::deque<std::deque<peak>>::iterator last, std::function<double(peak& pk)> &value_picker);*/

	double find_trigger(std::deque<peak_processed> &peaks, double time_window, bool use_Npes);
};

#endif
