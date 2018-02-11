#ifndef HISTOGRAM_SETUPS_H
#define HISTOGRAM_SETUPS_H

#include "GlobalParameters.h"
#include "EventCut.h"

class HistogramSetups //just helper class, has no functionality in itself
{
public:
	HistogramSetups() = default;
	double left_limit;
	double right_limit;
	double bottom_limit;
	double top_limit;
	double left_drawn_limit;
	double right_drawn_limit;
	//TODO: figure out displaying of arbitrary cuts. And whether arbitrary cuts are required.
	STD_CONT<EventCut> display_hist_cuts; //determines what is Filled into histogram =~=left/right_limit
	STD_CONT<EventCut> phys_hist_cuts;	  //determines which values are used for calculations of physical parameters
										  //(after applying display_hist_cuts). =~= left/right_drawn_limit
	int N_bins;
	//Fit info:
	int N_gauss;
	STD_CONT<double> par_val;
	STD_CONT<double> par_left_limits;
	STD_CONT<double> par_right_limits;
	bool fitted;
};

#endif