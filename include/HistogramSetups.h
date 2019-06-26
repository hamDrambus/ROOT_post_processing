#ifndef HISTOGRAM_SETUPS_H
#define HISTOGRAM_SETUPS_H

#include "GlobalParameters.h"
#include "EventCut.h"

class HistogramSetups //just helper class, has no functionality in itself
//defaults are set in PostProcessor!
{
public:
	//TODO: figure out displaying of arbitrary cuts. And whether arbitrary cuts are required.
	std::deque<EventCut> hist_cuts;
	int N_bins;
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
	Bool_t is_valid_fit_function;
	//1st tier parameters of distribution: (stored in order to minimize calls of LoopThroughData to recalculate them)
	boost::optional<std::size_t> num_of_runs;
	boost::optional<std::size_t> num_of_fills;
	boost::optional<std::size_t> num_of_drawn_fills;
	boost::optional<std::pair<double, double>> x_lims, y_lims;
	boost::optional<std::pair<double, double>> x_drawn_lims, y_drawn_lims;
	boost::optional<double> x_mean, y_mean;
	boost::optional<double> x_drawn_mean, y_drawn_mean;
	//2nd tier parameters (require 2 calls to LoopThroughData)
	bool filled_hist; //1st call is to determine default N bins, x-y range, etc.
	boost::optional<double> x_max, y_max; //Bin with maximum value y. Require filled histogram
	boost::optional<double> x_drawn_max, y_drawn_max; //TODO: Implement. These require separate hidden histogram
	boost::optional<double> x_variance, x_drawn_variance;
	boost::optional<double> y_variance, y_drawn_variance;
};

class CanvasSetups : public AnalysisStates //just helper class, has no fancy functionality in itself
{
public:
	struct stateS {
		int exp_index;
		int channel;
		Type type;
	};
	enum InvalidateLabel : unsigned int {
		invAll = 0xFFFFFFFF, invHistogram = 0x1, invCuts = 0x2, invDisplaedCuts = 0x4, invData = 0x8, invFit = 0x10, invFitFunction = 0x20
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

	HistogramSetups* get_hist_setups(void);
	HistogramSetups* get_hist_setups(int exp_ind, int channel, Type type);
	bool set_hist_setups(HistogramSetups* setups, int exp_ind, int channel, Type type); //Creates copy!
	TCanvas *get_current_canvas (void);

	TF1 *get_current_fit_function(void);
	bool set_fit_function(TF1* func);	//Creates copy!
	TF1* create_fit_function(HistogramSetups* setups, std::pair<double, double> domain);

	TH1D *get_current_hist1 (void);
	bool set_hist1 (TH1D * hist);		//Creates copy!
	TH2D *get_current_hist2 (void);
	bool set_hist2 (TH2D * hist); 		//Creates copy!

	std::pair<double, double> get_current_x_zoom(void);
	std::pair<double, double> get_current_y_zoom(void);
	std::pair<bool, bool> is_zoomed(void);
	bool set_zoom(std::pair<double, double> x_z, std::pair<double, double> y_z);
	bool unset_zoom(void);

	Bool_t StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type);
	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, std::size_t to_canvas, int from_ch, int from_exp, Type from_type, std::size_t from_canvas);

	virtual bool Invalidate(unsigned int label);
public:
	virtual ~CanvasSetups();
	CanvasSetups(std::deque<int> &mppc_channsels_, std::deque<int> &pmt_channsels_, std::deque<std::string>& experiments_);
	void next_canvas(void);
	void previous_canvas(void);
};

#endif
