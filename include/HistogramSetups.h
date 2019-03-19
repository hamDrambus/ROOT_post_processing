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
	Bool_t fitted;
};

class CanvasSetups : public AnalysisStates //just helper class, has no fancy functionality in itself
{
protected:
	struct stateS {
		int exp_index;
		int channel;
		Type type;
	};
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

public:
	virtual ~CanvasSetups();
	CanvasSetups(std::deque<int> &mppc_channsels_, std::deque<int> &pmt_channsels_, std::deque<std::string>& experiments_);
	void next_canvas(void);
	void previous_canvas(void);
};

#endif
