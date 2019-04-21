#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "SignalOperations.h"
#include "AnalysisStates.h"
#include "AllExperimentsResults.h"
#include "GraphicOutputManager.h"
#include "HistogramSetups.h"
#include "CalibrationInfo.h"

//for adding new types of analysis - dependence on AnalysisStates::Type
//1) AnalysisStates::AnalysisStates (first/last state)
//2) AnalysisStates::isMultichannel();
//3) AnalysisStates::isPerRun(Type type);
//4) AnalysisStates::is_PMT_type
//5) AnalysisStates::isComposite
//6) std::string AnalysisStates::type_name(Type type);
//7) AnalysisStates::is_TH1D_hist

//8) void PostProcessor::LoopThroughData(FunctionWrapper*);
//9) void PostProcessor::print_hist(int ch, int exp_ind, Type type);
//10) void PostProcessor::FillHist(void* p_hist)
//11) int PostProcessor::numOfFills(int channel, Type type);
//12) std::pair<double, double> PostProcessor::hist_x_limits(void);
//13) std::pair<double, double> PostProcessor::hist_y_limits(void);
//14) void PostProcessor::default_hist_setups(void);

//15) void PostProcessor::update_physical(void);
//16) 2nd tier methods in main:
//	FunctionWrapper* create_vertical_lines_cut(double left, double right)
//	FunctionWrapper* create_S_t_rect_exclude_cut(std::vector<double> region)
//	FunctionWrapper* create_S_t_rect_select_cut(std::vector<double> region)
//	FunctionWrapper* create_A_S_rect_exclude_cut(std::vector<double> region)
//	FunctionWrapper* create_off_ch_cut(int channel)
//	FunctionWrapper* create_A_S_fastPMT_cut(std::vector<double> region)

class PostProcessor : public CanvasSetups {
public:
	enum UpdateState {Histogram=0x1,FitFunction=0x2,Fit=0x4,Results = 0x8, All = Histogram|FitFunction|Fit|Results, AllFit = FitFunction|Fit,
		Canvas = 0x10, NewCanvas = Canvas|Histogram|AllFit};
protected:
	AllExperimentsResults* data;

	std::deque <std::deque<double> > avr_S2_S;	//TODO: reorganize? //initial (automatic) values are set in processAllExperiments
	std::deque <std::deque<double> > avr_Double_I;
	std::deque <double> PMT3_avr_S2_S;
	std::deque <double> PMT1_avr_S2_S;

	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, std::size_t to_canvas, int from_ch, int from_exp, Type from_type, std::size_t from_canvas);
	
	void FillHist(void* p_hist);//considers cuts and histogram type (void*)==either TH1D* or TH2D*
	//see function LoopThroughData for std::vector<double> &vals usage in cuts' picker
	int numOfFills(int channel, Type type);
	int numOfRuns (void);
	std::pair<double, double> hist_x_limits(bool consider_displayed_cuts = false); //considering cuts
	std::pair<double, double> hist_y_limits(void); //valid only for 2d plots
	void default_hist_setups(HistogramSetups*);

	void update_physical(void); //2nd and 3rd mandates of ::update(void)
	void update_Npe(void);		//4th part of ::update(void). TODO: actually it is better to move it to CalibrationInfo.

	std::string hist_name();
	void print_hist(std::string path); //use "" for default path
	void print_hist(int ch, int exp_ind, Type type, std::string path);

public:
	void LoopThroughData(FunctionWrapper* operation, int channel, Type type, bool apply_phys_cuts, bool apply_run_cuts, bool apply_hist_cuts = true);

	void update(UpdateState to_update = All); //mandates:	1)update current picture. (only displayed histogram but not a png, as well as TF1)
	//								2)update physical parameters obtained from the current hist
	//								3)in case it is calibration hist (Ss), update calibration
	//								4)recalibrate Npe.
	//png and raw data are saved manually via interface
	PostProcessor(AllExperimentsResults* results); //results must be already processed, i.e. loaded
	CalibrationInfo calibr_info;

	void save(int ch);	//TODO: make that it saves results such as calibration and Npe(E) (for both PMT and MPPC). That is updates only one line in calibr. file
	void save_all(void);
	//TODO: save_calibration(void)
	void saveAs(std::string path);
	
	void clear(void);	//clear cuts for current histogram. Run cuts derived from it are not touched
	void clearAll(void); //clear everything, return to initial state (leaves all existing histograms empty)

	void plot_N_pe(int ch, GraphicOutputManager* gr_man);

	//adding/removing cuts does not update histogram. This is done because adding of several cuts in succession is often necessary
	void add_hist_cut(FunctionWrapper *picker, std::string name, bool affect_hist);
	void add_hist_cut(FunctionWrapper *picker, std::string name, int channel, bool affect_hist);
	int list_hist_cuts (void);
	void remove_hist_cut(int index);
	void remove_hist_cut(std::string name);
	void remove_hist_cut(std::string name, int ch);
	void set_as_run_cut(std::string name);//adds current drawn_limits in HistogramSetups to runs cut (from current exp, channel and type)
	int list_run_cuts (void);
	void unset_as_run_cut(std::string name);//deletes current exp,ch and type from current cuts (if present) deletes from back, that is
	//if a single exp,ch,type produces several EventCuts, unset must be called respective amount of times
	void do_fit(bool do_fit);
	void set_N_bins(int N);


	void set_zoom (double xl, double xr);
	void set_zoom_y (double yl, double yr);
	void set_zoom (double xl, double xr, double yl, double yr);
	void unset_zoom(bool do_update = true);

	void set_fit_gauss(int N);
	void set_parameter_val(int index, double val);
	void set_parameter_limits(int index, double left, double right);

	void status(Bool_t full);

	//done via general methods in the main
	//void set_limits(double left, double right);
	//void set_drawn_limits(double left, double right);
	//void unset_limits(void);
	//void unset_drawn_limits(void);
};

#endif
