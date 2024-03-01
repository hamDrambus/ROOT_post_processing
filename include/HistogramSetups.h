#ifndef HISTOGRAM_SETUPS_H
#define HISTOGRAM_SETUPS_H

#include <tuple>
#include <variant>
#include <memory>
#include <utility>
#include "GlobalParameters.h"
#include "EventCut.h"

class StateData;
class TriggerData;

class HistogramSetups //just helper class, has no functionality in itself
//part of defaults are set in PostProcessor. Default invalidate is in the constructor
{
public:
	HistogramSetups(const std::deque<int>& channels);
	HistogramSetups(const HistogramSetups& setups);
	~HistogramSetups();
	std::deque<EventCut> hist_cuts;
	channel_info<bool> active_channels;
	int N_bins;
	int N_bins_y; //For 2D histograms only
	bool bins_logscale_x;
	bool bins_logscale_y; //For 2D histograms only
	std::pair<double, double> x_zoom, y_zoom;
	std::pair<bool, bool> is_zoomed; //bool for each x and y
	//Fit info:
	int N_gauss;
	std::deque<double> par_val;
	std::deque<double> par_left_limits;
	std::deque<double> par_right_limits;
	Bool_t use_fit;
	//Following values represent status, not input parameters
	Bool_t fitted;
	std::string x_axis_title, y_axis_title;
	Bool_t is_valid_fit_function;
	Bool_t use_default_setups;
	StateData* extra_data; //for example parameters for trigger adjustment algorithm

	bool logscale_x, logscale_y, logscale_z;
	std::string draw_option;
	std::pair<double, double> stats_xs, stats_ys; // Stat box position in pad.
	bool draw_stats;
#ifndef __ROOTCLING__
	//1st tier parameters of distribution: (stored in order to minimize calls of LoopThroughData to recalculate them)
	boost::optional<std::size_t> num_of_runs;
	boost::optional<std::size_t> num_of_fills;
	boost::optional<std::size_t> num_of_drawn_fills;
	boost::optional<long double> stat_weight;
	boost::optional<long double> stat_drawn_weight;
	boost::optional<std::pair<double, double>> x_lims, y_lims;
	boost::optional<std::pair<double, double>> x_drawn_lims, y_drawn_lims;
	boost::optional<double> x_mean, y_mean;
	boost::optional<double> x_drawn_mean, y_drawn_mean;
	//2nd tier parameters (require 2 calls to LoopThroughData)
	boost::optional<double> x_max, y_max; //Bin with maximum value y. Require filled histogram
	boost::optional<double> x_drawn_max, y_drawn_max; //TODO: Implement. These require separate hidden histogram
	boost::optional<double> x_variance, x_drawn_variance;
	boost::optional<double> y_variance, y_drawn_variance;
#endif
	bool filled_hist; //1st call is to determine default N bins, x-y range, etc.
};

class CanvasSetups : public AStates //just helper class, has no fancy functionality in itself
{
protected:
	// P.S. I do not understand what life choices have led me to this day where
	// I write the few tens of lines below.
	typedef std::tuple<int, double, double> auto_bins_t;
	typedef std::tuple<int, double*> custom_bins_t;
	typedef std::variant<auto_bins_t, custom_bins_t> bins_t_1d;
	typedef std::variant<bins_t_1d, std::pair<bins_t_1d, bins_t_1d> > bins_t;

	// Axis visitor
	struct SetAxisBins {
		TAxis* axis;
		SetAxisBins(TAxis* ax, bins_t_1d& bins) : axis(ax) {
			std::visit(*this, bins);
		}
		void operator()(auto_bins_t& bins) {
			if (axis)
				std::apply(std::mem_fn<void(Int_t, Double_t, Double_t)>(&TAxis::Set), std::tuple_cat(std::make_tuple(axis), bins));
		}
		void operator()(custom_bins_t& bins) {
			if (axis)
				std::apply(std::mem_fn<void(Int_t, const Double_t*)>(&TAxis::Set), std::tuple_cat(std::make_tuple(axis), bins));
			delete [] std::get<1>(bins);
		}
	};

	// Histogram visitor
	struct SetHistogramBins {
		TH1* histogram;
		SetHistogramBins(TH1* hist, bins_t& bins) : histogram(hist) {
			std::visit(*this, bins);
		}
		// For 1D
		void operator()(bins_t_1d& bins) {
			if (histogram) {
				SetAxisBins(histogram->GetXaxis(), bins);
				histogram->SetBinsLength(); // Must inform histogram that binning has changed
			}
		}
		// For 2D
		void operator()(std::pair<bins_t_1d, bins_t_1d>& bins_xy) {
			if (histogram) {
				SetAxisBins(histogram->GetXaxis(), bins_xy.first);
				SetAxisBins(histogram->GetYaxis(), bins_xy.second);
				histogram->SetBinsLength(); // Must inform histogram that binning has changed
			}
		}
	};

public:
	struct stateS {
		int exp_index;
		int channel;
		Type type;
	};
	enum InvalidateLabel : unsigned int {
		invAll = 0xFFFFFFFF-0x40, invHistogram = 0x1, invCuts = 0x2, invDisplayedCuts = 0x4, invData = 0x8, invFit = 0x10, invFitFunction = 0x20, invDefault = 0x40
	};

protected:
	std::deque<stateS> canvases_states; //for each canvas
	std::size_t canvas_ind;
	std::deque<TCanvas*> canvases;
	std::deque<TF1*> fit_functions;
	std::deque<TH1D*> hists_1D;
	std::deque<TH2D*> hists_2D;
	//canvas->type{Ss,S2_S,Double_I}->experiment->channel (size of channel depends on Type)
	std::deque<std::deque<std::deque<std::deque<HistogramSetups*> > > > manual_setups;
	//canvas->experiment->cut array
	std::deque<std::deque < std::deque<EventCut> > > RunCuts;

	std::deque<EventCut>* get_run_cuts (int exp_ind);

	HistogramSetups* get_hist_setups(int exp_ind, int channel, Type type) const;
	bool set_hist_setups(HistogramSetups* setups, int exp_ind, int channel, Type type); //Creates copy!

	TF1 *get_current_fit_function(void);
	bool set_fit_function(TF1* func);	//Creates copy!
	TF1* create_fit_function(HistogramSetups* setups, std::pair<double, double> domain);

	TH1D *get_current_hist1 (void);
	bool set_hist1 (TH1D * hist);		// Takes ownership!
	TH2D *get_current_hist2 (void);
	bool set_hist2 (TH2D * hist); 		// Takes ownership!
	TCanvas *get_current_canvas (void);

	std::pair<double, double> get_current_x_zoom(void);
	std::pair<double, double> get_current_y_zoom(void);
	std::pair<bool, bool> is_zoomed(void);
	bool set_zoom(std::pair<double, double> x_z, std::pair<double, double> y_z);

	bins_t_1d create_bins(int N, double min_v, double max_v, bool logscale) const;

	std::string hist_name(void) const;
	std::pair<double, double> hist_x_limits(bool consider_displayed_cuts = false); //considering cuts
	std::pair<double, double> hist_y_limits(bool consider_displayed_cuts = false); //valid only for 2d plots

	// Called after data was pre-analyzed so that limits are known.
	// Creates/modifies histogram for proper binning
	bool prepare_histogram(void);
	// Called after histogam was filled in PostProcessor
	bool fit_and_draw_histogram(void);

	Bool_t StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type);
	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, std::size_t to_canvas, int from_ch, int from_exp, Type from_type, std::size_t from_canvas);
	virtual Bool_t CorrelationXChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type);
	virtual Bool_t CorrelationYChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type);

	virtual bool Invalidate(unsigned int label);
public:
	virtual ~CanvasSetups();
	CanvasSetups(std::deque<int> &mppc_channsels_, std::deque<int> &pmt_channsels_, std::deque<std::string>& experiments_);
	HistogramSetups* get_hist_setups(void) const;
	void next_canvas(void);
	void previous_canvas(void);
	virtual bool update(void) = 0;

	std::size_t numOfFills(bool consider_displayed_cuts = false);

	void off_ch(int channel);
	void on_ch(int channel);

	int list_hist_cuts (void);
	int list_run_cuts (void);

	void set_N_bins(int N);
	void set_N_bins(int from, int to); // Or NbinsX, NbinsY for 2D histograms

	void set_zoom (double xl, double xr);
	void set_zoom_y (double yl, double yr);
	void set_zoom (double xl, double xr, double yl, double yr);
	void unset_zoom(bool do_update = true);

	void set_log_bins_x(bool is_log = true, bool do_update = true);
	void unset_log_bins_x(bool do_update = true);

	void set_log_bins_y(bool is_log = true, bool do_update = true);
	void unset_log_bins_y(bool do_update = true);

	void set_log_bins_xy(bool is_log = true, bool do_update = true);
	void unset_log_bins_xy(bool do_update = true);

	void set_log_x(bool is_log);
	void set_log_y(bool is_log);
	void set_log_z(bool is_log);

	void set_draw_option(std::string option);
	bool set_X_title(std::string text);
	bool set_Y_title(std::string text);

	void set_hist_stats(const std::string& location);
	void set_hist_stats(double x, double y);
	void set_hist_stats(bool on);

	//adding/removing cuts does not update histogram. This is done because adding of several cuts in succession is often necessary
	void add_hist_cut(FunctionWrapper *picker, std::string name, bool affect_hist);
	void add_hist_cut(FunctionWrapper *picker, std::string name, int channel, bool affect_hist);
	void remove_hist_cut(int index);
	void remove_hist_cut(std::string name);
	void remove_hist_cut(std::string name, int ch);

	void set_fit_gauss(int N);
	void set_parameter_val(int index, double val);
	void set_parameter_limits(int index, double left, double right);
public:
	friend class StateData;
	friend class TriggerData;
	friend class ShapeFitData;
};

#endif
