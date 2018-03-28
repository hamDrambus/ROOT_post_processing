#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "TROOT.h"

#include "SignalOperations.h"
#include "AnalysisStates.h"
#include "AllExperimentsResults.h"
#include "GraphicOutputManager.h"
#include "HistogramSetups.h"
#include "CalibrationInfo.h"

//for adding new types of analysis - dependence on AnalysisStates::Type
//1) AnalysisStates::AnalysisStates (first/last state)
//2) AnalysisStates::isMultichannel();
//3) AnalysisStates::is_PMT_type
//4) AnalysisStates::isPerRun
//5) AnalysisStates::isPerPeak
//6) AnalysisStates::isComposite
//7) std::string AnalysisStates::type_name(Type type);
//8) AnalysisStates::is_TH1D_hist

//9) void PostProcessor::LoopThroughData(FunctionWrapper*);
//10) void PostProcessor::FillHist(void* p_hist)
//11) int PostProcessor::numOfFills(void);
//12) std::pair<double, double> PostProcessor::hist_x_limits(void);
//13) std::pair<double, double> PostProcessor::hist_y_limits(void);

//14) void PostProcessor::set_default_hist_setups(void);

//15) void PostProcessor::update_physical(void);
//16) void PostProcessor::set_limits(double left, double right);
//17) void PostProcessor::set_drawn_limits(double left, double right);
//18) void PostProcessor::set_as_run_cut(std::string name)


//TODO: rename display_cuts from HistogramSetups (counter-intuitive)

class PostProcessor : public AnalysisStates {
public:
	enum UpdateState {Histogram=0x1,FitFunction=0x2,Fit=0x4,Results = 0x8, All = Histogram|FitFunction|Fit|Results, AllFit = FitFunction|Fit,
		Canvas = 0x10, NewCanvas = Canvas|Histogram|AllFit};
protected:
	HistogramSetups* current_setups;
	TCanvas *current_canvas;
	int canvas_n;
	TF1 *current_fit_func;
	TH1D *current_hist1;
	TH2D *current_hist2;
	TLine *current_vert_line0;
	TLine *current_vert_line1;
	
	AllExperimentsResults* data;

	//experiment->channel->{Ss,S2_S,Double_I}
	std::deque<std::deque<std::deque<HistogramSetups*> > > manual_setups;
	//experiment->cut array
	std::deque < std::deque<EventCut> > RunCuts;

	//experiment->channel
	std::deque <std::deque<double> > avr_S2_S; //initial (automatic) values are set in processAllExperiments
	std::deque <std::deque<double> > avr_Double_I;
	std::deque <double> PMT3_avr_S2_S;
	std::deque <double> PMT1_avr_S2_S;

	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type,Bool_t save);
	
	void set_hist_setups(HistogramSetups* setups, std::string exp, int channel, Type type);//does not call update
	HistogramSetups* get_hist_setups(std::string exp, int channel, Type type);//does not call update

	void FillHist(void* p_hist);//considers cuts and histogram tipe (void*)==either TH1D* or TH2D*
	//see function for std::vector<double> &vals usage in cuts' picker
	int numOfFills(void);
	std::pair<double, double> hist_x_limits(void); //considering cuts
	std::pair<double, double> hist_y_limits(void); //valid only for 2d plots
	void set_default_hist_setups(void);//

	TF1* create_fit_function(HistogramSetups* func);
	void update_fit_function(bool do_delete); //uses current_fit_func and current_setups
	//TODO: add setting average S2 and double I without manual setups. Maybe as exit() method, which will
	//set S2 and double integral with NULL HistogramSetups by default and won't touch the calibration
	void update_physical(void); //2nd and 3rd mandates of ::update(void)
	void update_Npe(void);		//4th part of ::update(void). TODO: actually it is better to move it to CalibrationInfo.

	std::string hist_name();
	void print_hist(void);
	void print_hist(int ch, int exp_ind, Type type);

public:
	void LoopThroughData(FunctionWrapper* operation, int channel, Type type, bool apply_phys_cuts, bool apply_run_cuts);

	void update(UpdateState to_update = All); //mandates:	1)update current picture. (only displayed histogram but not a png, as well as TLines and TF1)
	//								2)update physical parameters obtained from the current hist
	//								3)in case it is calibration hist (Ss), update calibration
	//								4)recalibrate Npe.
	//png is saved at exit or next/prev function.
	PostProcessor(AllExperimentsResults* results); //results must be already processed, e.g. loaded
	CalibrationInfo calibr_info;
	//TODO: save current setups and picture without moving to other {ch,exp,type}
	void save(int ch);	//TODO: make that it saves results such as calibration and Npe(E) (for both PMT and MPPC). That is updates only one line in calibr. file
	void save_all(void);
	
	void plot_N_pe(int ch, GraphicOutputManager* gr_man);

	//~done: will be done to the fullest from root macros. Then can add code from there. TO DO: add several default cuts (e.g. from left/right limit)
	void add_hist_cut(FunctionWrapper *picker, std::string name = "", bool update = true);
	void add_hist_cut(FunctionWrapper* picker, std::string name, int channel, bool update = true);
	void remove_hist_cut(std::string name = "", bool update = true);
	void remove_hist_cut(std::string name, int ch, bool update = true);
	void set_as_run_cut(std::string name = "");//adds current drawn_limits in HistogramSetups to runs cut (from current exp, channel and type)
	void unset_as_run_cut(std::string name = "");//deletes current exp,ch and type from current cuts (if present) deletes from back, that is
	//if a single exp,ch,type produces several EventCuts, unset must be called respective amount of times
	void do_fit(Bool_t upd_vis = kTRUE);

	void set_N_bins(int N);
	void set_limits(double left, double right);
	void set_drawn_limits(double left, double right);
	void unset_limits(void);
	void unset_drawn_limits(void);

	void set_fit_gauss(int N);
	void set_parameter_val(int index, double val);
	void set_parameter_limits(int index, double left, double right);

	void new_canvas(void);

	void status(Bool_t full);
};

#endif
