#ifndef _GLOBAL_PARAMETERS_H
#define _GLOBAL_PARAMETERS_H

#include <algorithm>
//#include <functional>

#include "GlobalDefinitions.h"
#include "ExperimentArea.h"

#define ROOT_BL_CALL_V0 find_background_v_0(f_ys, ys.size(), 80,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE,2);
#define ROOT_BL_CALL_V2 find_background_v_0(f_ys, ys.size(), 60,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE,2);
#define ROOT_BL_CALL_V3 find_background_v_0(f_ys, ys.size(), 70,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE,2);
#define ROOT_BL_CALL_V4 find_background_v_raw(f_ys, ys.size(), 60,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V5 spec->Background(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V6 spec->Background(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V7 spec->Background(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V8 spec->Background(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);

std::vector<double>::iterator iter_add(std::vector<double>::iterator& to, int what, std::vector<double>::iterator& end);

class AnalysisManager;
class AllExperimentsResults;
class PostProcessor;
class GraphicOutputManager;

extern GraphicOutputManager *gr_manager;
extern AnalysisManager *manager;
extern AllExperimentsResults* g_data;
extern PostProcessor* post_processor;

//namespace ParameterPile
//{
	enum DrawEngine { Gnuplot, ROOT_ };

	Bool_t draw_required(/*ParameterPile::*/experiment_area what);

	extern std::deque <experiment_area> areas_to_draw;
	extern std::string this_path;
	
	extern experiment_area exp_area;
	extern int threads_number;

	extern int gnuplot_pad_size;
	extern int gnuplot_max_size;
	extern int gnuplot_width;

	extern std::map < std::string, double > experiment_fields;
	extern std::map < std::string, double > PMT_V;
	extern std::pair<int, int> calibaration_points;
	extern std::map < int, std::pair<double,double> > MPPC_coords;

	void Init_globals(void);
//};
void DrawFileData(std::string name, std::vector<double> xs, std::vector<double> ys, /*ParameterPile::*/DrawEngine de = /*ParameterPile::*/ ROOT_);

#endif
