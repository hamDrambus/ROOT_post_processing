#ifndef MAIN_H
#define MAIN_H

#include "GlobalParameters.h"
#include "AnalysisManager.h"
#include "GraphicOutputManager.h"
#include "PostProcessor.h"

void Initialize(void);
void Ne(Bool_t save_state = kTRUE); //Next (E == experiment ==field)
void Pe(Bool_t save_state = kTRUE); //Previous (E == experiment ==field)
void Nch(Bool_t save_state = kTRUE); //Next channel
void Pch(Bool_t save_state = kTRUE); //Previous channel
void GotoCh(Int_t ch, Bool_t save_state = kTRUE); //go to specific channel
void Nt(Bool_t save_state = kTRUE); //Next type (Ss, S2_S, Double_Int_tergral)
void Pt(Bool_t save_state = kTRUE); //Prev type (Ss, S2_S, Double_Int_tergral)
void update();
void status(Bool_t full = kFALSE);// displays current state
void state(Bool_t full); // displays current state
void set_fit_1_gauss(void);
void set_fit_2_gauss(void);
void set_fit_3_gauss(void);
void set_fit_gauss(Int_t N);
void set_parameter_val(Int_t index, Double_t val);
void set_parameter_limits(Int_t index, Double_t left, Double_t right);
void do_fit(Bool_t update_vis);
void draw_limits(Double_t left, Double_t right);
void impose_limits(Double_t left, Double_t right);
void unset_limits(void);
void next_canvas(void); //creates new canvas, so the current one will stay unchanged
void set_calib_N(Int_t from, Int_t to);//in order to set default use invalid values
void draw_Npe(void);
void add_1peS(void);
void add_2peS(void);
void rem_1peS(void);
void rem_2peS(void);
void set_1peS(Double_t val);
void unset_1peS(void);
void use_mean(Bool_t do_use = kTRUE); //uses mean value of data instead of gauss' mean. May be usefull for S2_S
void set_N_bin(Int_t n);
void Exit(Bool_t save = kTRUE);

void add_hist_cut(FunctionWrapper *picker, std::string name = "");
void remove_hist_cut(std::string name = "");
void set_as_run_cut(std::string name = "");
void unset_as_run_cut(std::string name = "");

#endif
