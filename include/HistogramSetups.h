#ifndef HISTOGRAM_SETUPS_H
#define HISTOGRAM_SETUPS_H

#include "GlobalParameters.h"
#include "EventCut.h"

class HistogramSetups //just helper class, has no functionality in itself
{
public:
	double left_limit;
	double right_limit;
	double bottom_limit;
	double top_limit;
	double left_drawn_limit;
	double right_drawn_limit;
	//TODO: figure out displaying of arbitrary cuts. And whether arbitrary cuts are required.
	std::deque<EventCut> display_hist_cuts; //determines what is Filled into histogram =~=left/right_limit
	std::deque<EventCut> phys_hist_cuts;	  //determines which values are used for calculations of physical parameters
										  //(after applying display_hist_cuts). =~= left/right_drawn_limit
	int N_bins;
	//Fit info:
	int N_gauss;
	std::deque<double> par_val;
	std::deque<double> par_left_limits;
	std::deque<double> par_right_limits;
	Bool_t fitted;
};

#endif