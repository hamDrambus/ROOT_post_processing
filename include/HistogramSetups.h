#ifndef HISTOGRAM_SETUPS_H
#define HISTOGRAM_SETUPS_H

#include "GlobalParameters.h"
#include "EventCut.h"

class HistogramSetups //just helper class, has no functionality in itself
{
public:
	Double_t left_limit;
	Double_t right_limit;
	Double_t bottom_limit;
	Double_t top_limit;
	Double_t left_drawn_limit;
	Double_t right_drawn_limit;
	//TODO: figure out displaying of arbitrary cuts. And whether arbitrary cuts are required.
	std::deque<EventCut> display_hist_cuts; //determines what is Filled Int_to histogram =~=left/right_limit
	std::deque<EventCut> phys_hist_cuts;	  //determines which values are used for calculations of physical parameters
										  //(after applying display_hist_cuts). =~= left/right_drawn_limit
	Int_t N_bins;
	//Fit info:
	Int_t N_gauss;
	std::deque<Double_t> par_val;
	std::deque<Double_t> par_left_limits;
	std::deque<Double_t> par_right_limits;
	Bool_t fitted;
};

#endif