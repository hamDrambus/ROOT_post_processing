#ifndef HISTOGRAM_SETUPS_H
#define HISTOGRAM_SETUPS_H

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
	bool set_hist1 (TH1D * hist);		//Creates copy!
	TH2D *get_current_hist2 (void);
	bool set_hist2 (TH2D * hist); 		//Creates copy!
	TCanvas *get_current_canvas (void);

	std::pair<double, double> get_current_x_zoom(void);
	std::pair<double, double> get_current_y_zoom(void);
	std::pair<bool, bool> is_zoomed(void);
	bool set_zoom(std::pair<double, double> x_z, std::pair<double, double> y_z);
	bool unset_zoom(void);
	void set_log_x(bool is_log);
	void set_log_y(bool is_log);
	void set_log_z(bool is_log);
	void set_draw_option(std::string option);
	void set_hist_stats(const std::string& location);
	void set_hist_stats(double x, double y);
	void set_hist_stats(bool on);

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
public:
	friend class StateData;
	friend class TriggerData;
};

#endif
