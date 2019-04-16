#ifndef MAIN_H
#define MAIN_H

#include "GlobalParameters.h"
#include "AnalysisManager.h"
#include "GraphicOutputManager.h"
#include "PostProcessor.h"


/*TODO: rework types (and names):
*	unite mppc and pmt peak analysis - there is a lot of repeating code
* DONE: unite hist and physical cuts into one. Add bool affect_histogram to EventCut class.
* DONE: implement GotoCh()
* DONE: move parts of code in cutters.cpp to the main program
* TODO: cleanly separate initialization in hard-coded and externally loaded scripts
* TODO: share cuts across types, e.g. A_S cut for ch8 PMT_A_S is the automatically used for signal forms or S2byN signal etc.
* TODO*: for Cd time analysis: signal <time> per run per ch
* TODO: remove all data except peaks?
* TODO: unite some multichannel types, e.g. MPPC_times and MPPC_times_sum
* 	the first type can be realized as second one with cuts turning off unneeded channels
* DONE: add drawing cuts on relevant histograms. (with PolyLine)
* 	Maybe and render function for EventCut
* TODO: work out which commands exactly will be needed to obtain signal forms
*	and optimize interface based on that.
*/
//DONE: new interface:
//+//
void Initialize(bool pars_in_script = true);
void ch(int ch); //Go to channel. Does not save previous hist in file but does save parameters such as cuts
void ty(AnalysisStates::Type to_type);
void nex(void);		//next experiment
void pex(void); 	//previous experiment
void nch(void);
void pch(void); 	//previous channel;
void update(void);
void saveas(std::string path = "");	//"" - use default path: "Data/results/{PMT_v1|MPPC_v1}/experiment/{PMT_|MPPC_}ch/{pic&data}
void status(Bool_t full = false);	//displays current state
void state(Bool_t full); 			//same as ^
void set_fit_gauss(int N);
void set_parameter_val(int index, double val);
void set_parameter_limits(int index, double left, double right);
void do_fit(bool is_on);	//always updates visuals

void set_bins(int n);
void set_zoom (double xl, double xr);
void set_zoom_y (double yl, double yr);
void set_zoom (double xl, double xr, double yl, double yr);
void unset_zoom(void);
void next_canvas(void); //creates new canvas or goes to the next existing. The current one will stay unchanged.
//Have independent cuts but the new one inherits the previous ones
void prev_canvas(void);
void set_corr (AnalysisStates::Type x_t, AnalysisStates::Type y_t, int chx, int chy);
void add_hist_cut(FunctionWrapper *picker, std::string name, int ch, bool draw = false);
//^If draw==false it shows cut via DrawCut method of FunctionWrapper
//^and does not change histogram itself.
//^In case there is no draw method do_replot is forced to true and warning is issued.
//^In case "name" is present in existing cut list the old cut is replaced.
void add_hist_cut(FunctionWrapper *picker, std::string name, bool draw = false);
//same as above but ch is set automatically if possible.
int list_hist_cuts (void); //returns number of cuts
void remove_hist_cut(int index); //if index is larger than number of cuts the last cut is removed (popped)
void remove_hist_cut(std::string name, int ch);
void remove_hist_cut(std::string name); //same as above but for each channel possible
void set_as_run_cut(std::string name = "");
//^use all cuts for current histogram as run cut. In ambiguous cases such as cuts on S_t
//^if not a single peak is accepted in the run then the run is excluded.
//^In general such cuts are useless so the implementation of this case in not important.
int list_run_cuts (void); //returns number of cuts
void unset_as_run_cut(std::string name = "");
void clear(void);	//clear cuts for current histogram. Run cuts derived from it are not touched
void clearAll(void); //clear everything, return to initial state (leaves all existing histograms empty)

//Calibration methods:
void calib_status(Bool_t uncalibrated_only = false);
void draw_Npe(void);
void add_1peS(void);
void add_2peS(void);
void rem_1peS(void);
void rem_2peS(void);
void set_1peS(double val);
void unset_1peS(void);
void set_use_mean(Bool_t do_use = true);
void calib_load(std::string fname = ""); //if fname=="" uses file location from global parameters
void calib_save(std::string fname = "");
//TODO: remove?
void set_calib_N(int from, int to);//in order to set default use invalid values
void Exit(Bool_t save = kTRUE);

//Often used cutting functions: (in order to decrease code in ROOT's macros)
void draw_limits(double left, double right);	//2 tier method
void set_limits(double left, double right);		//2 tier method
void unset_limits(void);						//2 tier method

void cut_S_t_rect_exclude(double t_min, double t_max, double S_min, double S_max, bool drawn, int channel = -1, std::string name = "");
void cut_S_t_rect_exclude(std::vector<double> region, bool drawn, int channel = -1, std::string name = ""); //region is {t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
//remcut is for remove_cut
void remcut_S_t_rect_exclude(int channel = -1, std::string name = "");

void cut_S_t_rect_select(double t_min, double t_max, double S_min, double S_max, bool drawn, int channel = -1, std::string name = "");
void cut_S_t_rect_select(std::vector<double> region, bool drawn, int channel = -1, std::string name = ""); //region is {t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
void remcut_S_t_rect_select(int channel = -1, std::string name = "");

void cut_A_S_rect_exclude(double A_min, double A_max, double S_min, double S_max, bool drawn, int channel = -1, std::string name = "");
void cut_A_S_rect_exclude(std::vector<double> region, bool drawn, int channel = -1, std::string name = ""); //region is {A_min0, A_max0, S_min0, S_max0, A_min1, A_max1 ...}
void remcut_A_S_rect_exclude(int channel = -1, std::string name = "");

void cut_S(double S_min, double S_max, bool drawn, int channel = -1, std::string name = "");
void remcut_S(int channel = -1, std::string name = "");
void cut_t(double t_min, double t_max, bool drawn, int channel = -1, std::string name = "");
void remcut_t(int channel = -1, std::string name = "");

//(turn_)off/on
void off_ch (int ch); //for multichannel types (e.g. signal form of all SiPMs (MPPCs)). TODO: single channel case may be implemented with multichannel one - decrease the number of types
void on_ch (int ch);

//region is {A_min, A0, S0, A1, S1, A_max}, draw it for clarification, e.g.:
//ch(7); add_S_t_fast_PMT(region, true); //- will display cuts with red lines
void cut_A_S_fast_PMT(std::vector<double> region, bool drawn, int channel = -1, std::string _name = "");
void remcut_A_S_fast_PMT(int channel = -1, std::string _name = "");

#endif
