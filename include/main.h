#ifndef MAIN_H
#define MAIN_H

#include "GlobalParameters.h"
#include "AnalysisManager.h"
#include "GraphCollection.h"
#include "PostProcessor.h"


/*TODO: rework types (and names):
*	unite mppc and pmt peak analysis - there is a lot of repeating code
* DONE: unite hist and physical cuts into one. Add bool affect_histogram to EventCut class.
* DONE: implement GotoCh()
* DONE: move parts of code in cutters.cpp to the main program
* TODO: cleanly separate initialization in hard-coded and externally loaded scripts
* DONE - dropped: share cuts across types, e.g. A_S cut for ch8 PMT_A_S is the automatically used for signal forms or S2byN signal etc.
* DONE*: for Cd time analysis: signal <time> per run per ch
* TODO: remove all data except peaks?
* TODO: unite some multichannel types, e.g. MPPC_times and MPPC_times_sum
* 	the first type can be implemented as second one with cuts turning off unneeded channels
* DONE: add drawing cuts on relevant histograms. (with PolyLine)
* 	Maybe and render function for EventCut
* DONE: work out which commands exactly will be needed to obtain signal forms
*	and optimize interface based on that.
* DONE: some of the cuts' code is redundant (can be implemented via more general viewRegion::IsInPolygon method)
*
*/
//DONE: new interface:
//+//
//2020.05.09 UPD: part of interface is now in StateData.h (setters/getters related to specific settings of AState)
enum XY_cut_type : unsigned int {Horizontal = 0x1, UpperLeft = 0x2, Inclusive = 0x4}; //e.g. 0b111 = 7 = cut_x_y_upper_select, 0b000 = cut_x_y_right

void Initialize(bool pars_in_script = true);
void GenTest(std::string prefix);
void ch(int ch); //Go to channel. Does not save previous hist in file but does save parameters such as cuts
void ty(AStates::Type to_type);
void nex(void);		//next experiment
void pex(void); 	//previous experiment
void nch(void);
void pch(void); 	//previous channel;
void update(void);
void saveas(std::string path = "");	//"" - use default path: "Data/results/{PMT_v1|MPPC_v1}/experiment/{PMT_|MPPC_}ch/{pic&data}
void saveaspng(std::string path = "");	//"" - use default path: "Data/results/{PMT_v1|MPPC_v1}/experiment/{PMT_|MPPC_}ch/{pic&data}
void status(void);	//displays current state
void set_fit_gauss(int N);
void set_parameter_val(int index, double val);
void set_parameter_limits(int index, double left, double right);
void do_fit(bool is_on);	//always updates visuals

//void set_trigger_shaping(double val); //in microseconds
//double get_trigger_shaping(void); //in microseconds
void unset_trigger_offsets(void);
void set_trigger_offsets(double extra_offset); //uses trigger type histogram only. extra_offset is in microseconds

void set_bins(int n);
void set_bins(int from, int to); //simultaneous set_bins and set_zoom for discrete distributions
void set_zoom (double xl, double xr);
void set_zoom_y (double yl, double yr);
void set_zoom (double xl, double xr, double yl, double yr);
void set_X_title(std::string text);
void set_Y_title(std::string text);
void set_titles(std::string x_title, std::string y_title);
void unset_zoom(void);
void next_canvas(void); //creates new canvas or goes to the next existing. The current one will stay unchanged.
//Have independent cuts but the new one inherits the previous ones
void prev_canvas(void);
void set_corr (AStates::Type x_t, AStates::Type y_t, int chx, int chy);
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
void print_accepted_events (std::string file, int run_offset, int sub_runs = 1000);
void print_rejected_events (std::string file, int run_offset, int sub_runs = 1000);
void clear(void);	//clear cuts for current histogram. Run cuts derived from it are not touched
void clearAll(void); //clear everything, return to initial state (leaves all existing histograms empty)

//Calibration methods:
//TODO: add erasing calibration. (When want to update calibration in existing file with, for example, changed cuts)
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
void Exit(Bool_t save = kTRUE); //It's better do not call it and save all relevant info manually

//Often used cutting functions: (in order to decrease code in ROOT's macros)
void draw_limits(double left, double right, std::string name = ""); //2 tier method
void set_limits(double left, double right);		//2 tier method
void unset_limits(void);						//2 tier method
void unset_draw_limits(void);					//2 tier method

void set_log_x(void);
void set_log_y(void);
void set_log_z(void);
void unset_log_x(void);
void unset_log_y(void);
void unset_log_z(void);

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
void cut_t_S1_S2(double t_min_S1, double t_max_S1, double t_min_S2, double t_max_S2, bool drawn, int channel = -1, std::string name = "");
void remcut_t(int channel = -1, std::string name = "");
void remcut(int channel, std::string name);
void remcut(std::string name);

//(turn_)off/on
void off_ch (int ch); //for multichannel types (e.g. signal form of all SiPMs (MPPCs)). TODO: single channel case may be implemented with multichannel one - decrease the number of types
void on_ch (int ch);

//region is {A_min, A0, S0, A1, S1, A_max}, draw it for clarification, e.g.:
//ch(7); add_S_t_fast_PMT(region, true); //- will display cuts with red lines
void cut_A_S_fast_PMT(std::vector<double> region, bool drawn, int channel = -1, std::string _name = "");
void remcut_A_S_fast_PMT(int channel = -1, std::string _name = "");

void cut_A_S_upper(double A_min, double S_min, double A_max, double S_max, bool drawn, int channel = -1, std::string _name = "");
void cut_A_S_upper(std::vector<double> region, bool drawn, int channel = -1, std::string _name = "");

void cut_A_S_lower(double A_min, double S_min, double A_max, double S_max, bool drawn, int channel = -1, std::string _name = "");
void cut_A_S_lower(std::vector<double> region, bool drawn, int channel = -1, std::string _name = "");

void cut_A_S_left(double A_min, double S_min, double A_max, double S_max, bool drawn, int channel = -1, std::string _name = "");
void cut_A_S_left(std::vector<double> region, bool drawn, int channel = -1, std::string _name = "");

void cut_A_S_right(double A_min, double S_min, double A_max, double S_max, bool drawn, int channel = -1, std::string _name = "");
void cut_A_S_right(std::vector<double> region, bool drawn, int channel = -1, std::string _name = "");

void cut_x_y_poly(std::vector<double> region, bool drawn, std::string _name);

void cut_x_y_upper(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_upper(std::vector<double> region, bool drawn, std::string _name = "");

void cut_x_y_lower(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_lower(std::vector<double> region, bool drawn, std::string _name = "");

void cut_x_y_left(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_left(std::vector<double> region, bool drawn, std::string _name = "");

void cut_x_y_right(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_right(std::vector<double> region, bool drawn, std::string _name = "");

void cut_x_y_poly_select(std::vector<double> region, bool drawn, std::string _name);

void cut_x_y_upper_select(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_upper_select(std::vector<double> region, bool drawn, std::string _name = "");

void cut_x_y_lower_select(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_lower_select(std::vector<double> region, bool drawn, std::string _name = "");

void cut_x_y_left_select(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_left_select(std::vector<double> region, bool drawn, std::string _name = "");

void cut_x_y_right_select(double X_min, double Y_min, double X_max, double Y_max, bool drawn, std::string _name = "");
void cut_x_y_right_select(std::vector<double> region, bool drawn, std::string _name = "");

void cut_runs(int max_index, std::string _name = "");

double get_mean(void); //x mean (ignores drawn cuts)

void view_event(int event_index, int Nbins = 0, double x_min = 0, double x_max = 160);

#ifdef _COMPLIE_MAIN_
int main(int argc, char* argv[]) {
	TestSignalGenerator("test/");
	return 0;
}
#endif

#endif
