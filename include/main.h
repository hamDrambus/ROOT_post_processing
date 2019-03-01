#ifndef MAIN_H
#define MAIN_H

#include "GlobalParameters.h"
#include "AnalysisManager.h"
#include "GraphicOutputManager.h"
#include "PostProcessor.h"


/*TODO: rework types (and names):
*	unite mppc and pmt peak analysis - there is a lot of repeating code
* TODO: implement GotoCh()
* TODO: move parts of code in cutters.cpp to the main program
* TODO: cleanly separate initialization in hard-coded and externally loaded scripts
* TODO: share cuts across types, e.g. A_S cut for ch8 PMT_A_S is the automatically used for signal forms or S2byN signal etc.
* TODO*: for Cd time analysis: signal <time> per run per ch
* TODO: remove all data except peaks?
* TODO: unite some multichannel types, e.g. MPPC_times and MPPC_times_sum
* 	the first type can be realized as second one with cuts turning off unneeded channels
* TODO: add drawing cuts on relevant histograms. (with PolyLine)
* 	Maybe and render function for EventCut
* TODO: work out which commands exactly will be needed to obtain signal forms
*	and optimize interface based on that.
*/
void Initialize(bool pars_in_script = true);
void Ne(Bool_t save_state = kTRUE); //Next (E == experiment ==field)
void Pe(Bool_t save_state = kTRUE); //Previous (E == experiment ==field)
void Nch(Bool_t save_state = kTRUE); //Next channel
void Pch(Bool_t save_state = kTRUE); //Previous channel
void GotoCh(int ch, Bool_t save_state = kTRUE); //go to specific channel
void Nt(Bool_t save_state = kTRUE); //Next type (Ss, S2_S, Double_intergral)
void Pt(Bool_t save_state = kTRUE); //Prev type (Ss, S2_S, Double_intergral)
void GotoT(AnalysisStates::Type to_type, bool do_save = true);
void update();
void status(Bool_t full = kFALSE);// displays current state
void state(Bool_t full); // displays current state
void set_fit_1_gauss(void);
void set_fit_2_gauss(void);
void set_fit_3_gauss(void);
void set_fit_gauss(int N);
void set_parameter_val(int index, double val);
void set_parameter_limits(int index, double left, double right);
void do_fit(Bool_t update_vis);
void draw_limits(double left, double right);
void impose_limits(double left, double right);
void unset_limits(void);
void set_zoom (double xl, double xr);
void set_zoom_y (double yl, double yr);
void set_zoom (double xl, double xr, double yl, double yr);
void unset_zoom(bool do_update = true);
void next_canvas(void); //creates new canvas, so the current one will stay unchanged
void set_calib_N(int from, int to);//in order to set default use invalid values
void draw_Npe(void);
void add_1peS(void);
void add_2peS(void);
void rem_1peS(void);
void rem_2peS(void);
void set_1peS(double val);
void unset_1peS(void);
void use_mean(Bool_t do_use = kTRUE); //uses mean value of data instead of gauss' mean. May be usefull for S2_S
void set_N_bin(int n);
void SetCorr (AnalysisStates::Type x_t, AnalysisStates::Type y_t, int chx, int chy);
void Exit(Bool_t save = kTRUE);

void add_hist_cut(FunctionWrapper *picker, std::string name, bool update);
void add_hist_cut(FunctionWrapper *picker, std::string name, int ch, bool update);
void remove_hist_cut(std::string name, bool update);
void remove_hist_cut(std::string name, int ch, bool update);
void set_as_run_cut(std::string name = "");
void unset_as_run_cut(std::string name = "");

#endif
