#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <thread>
#include "SignalOperations.h"
#include "AllExperimentsResults.h"
#include "GraphicOutputManager.h"
#include "AnalysisStates.h"
#include "HistogramSetups.h"
#include "CalibrationInfo.h"

//TODO: too many places to adjust to add a new type.
//Many functions can be rewritten by simply using value picker for each type.
//In addition, methods in main can be rewritten using AnalysisStates::is... methods.

//for adding new types of analysis - dependence on AnalysisStates::Type
//1) AnalysisStates::AnalysisStates (first/last state)
//2) AnalysisStates::isMultichannel(Type type) const;
//3) AnalysisStates::isPerRun(Type type) const;
//4) AnalysisStates::isPMTtype(Type::type) const;
//5) AnalysisStates::isComposite(Type::type) const;
//6) AnalysisStates::isTH1Dhist(Type::type) const;
//7) AnalysisStates::isVirtual(Type::type) const;
//8) std::string AnalysisStates::type_name(Type type) const;

//9) Corresponding StateData.h if necessary (initialized in PostProcessor::default_hist_setups(HistogramSetups*))
//10) StateData::IsForState_virt for each class derived from StateData including StateData itself
//11) bool CanvasSetups::set_hist_setups(HistogramSetups* setups, int exp_ind, int channel, Type type)

//12) void PostProcessor::LoopThroughData(std::vector<Operation> &operations, int channel, Type type);
//13) bool PostProcessor::set_correlation_filler(FunctionWrapper* operation, Type type);
//14) void PostProcessor::print_hist(std::string path);
//15) bool PostProcessor::update(void);
//16) void PostProcessor::update_physical(void)
//17) void PostProcessor::default_hist_setups(HistogramSetups*);
//18) bool PostProcessor::set_trigger_offsets(double extra_offset);
//19) void view_event(int event_index, double x_min = 0, double x_max = 160);


//20) 2nd tier methods in main:
//	FunctionWrapper* create_vertical_lines_cut(double left, double right)
//	FunctionWrapper* create_S_t_rect_exclude_cut(std::vector<double> region)
//	FunctionWrapper* create_S_t_rect_select_cut(std::vector<double> region)
//	FunctionWrapper* create_A_S_rect_exclude_cut(std::vector<double> region)
//	FunctionWrapper* create_A_S_fastPMT_cut(std::vector<double> region)
//	FunctionWrapper* create_A_S_polygon_cut(std::vector<double> region, unsigned int cut_type)
//	FunctionWrapper* create_x_y_polygon_cut(std::vector<double> region, unsigned int cut_type)

class PostProcessor : public CanvasSetups {
public:
	enum UpdateState {Histogram=0x1,FitFunction=0x2,Fit=0x4,Results = 0x8, All = Histogram|FitFunction|Fit|Results, AllFit = FitFunction|Fit,
		Canvas = 0x10, NewCanvas = Canvas|Histogram|AllFit};
	class Operation {
	public:
		FunctionWrapper * operation;
		bool apply_run_cuts;
		bool apply_hist_cuts;
		bool apply_phys_cuts;
		Operation() :
			operation(NULL), apply_run_cuts(false), apply_hist_cuts(false), apply_phys_cuts(false)
		{}
		Operation(FunctionWrapper *op, bool apply_run_cut, bool apply_hist_cut, bool apply_phys_cut) :
			operation(op), apply_run_cuts(apply_run_cut), apply_hist_cuts(apply_hist_cut), apply_phys_cuts(apply_phys_cut)
		{}
	};
	CalibrationInfo calibr_info;

protected:
#ifndef __ROOTCLING__
	struct correlation_data {
		std::vector<boost::optional<double>> *vals;
		int ch_size;
	};
#endif
	AllExperimentsResults* data;
	TCanvas* event_canvas; //TODO: separate class for event viewer?

	virtual Bool_t StateChange(int to_ch, int to_exp, Type to_type, std::size_t to_canvas, int from_ch, int from_exp, Type from_type, std::size_t from_canvas);
	virtual Bool_t CorrelationXChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type);
	virtual Bool_t CorrelationYChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type);

	bool set_correlation_filler(FunctionWrapper* operation, Type type);
	std::size_t numOfRuns (void);
	std::pair<double, double> hist_x_limits(bool consider_displayed_cuts = false); //considering cuts
	std::pair<double, double> hist_y_limits(bool consider_displayed_cuts = false); //valid only for 2d plots
	void default_hist_setups(HistogramSetups*);
	void update_physical(void); //2nd and 3rd mandates of ::update(void)
	std::string hist_name();
	void print_hist(std::string path, bool png_only); //use "" for default path
	void LoopThroughData(std::vector<Operation> &operations, int channel, Type type);

public:
	virtual bool Invalidate(unsigned int label);
	std::size_t numOfFills(bool consider_displayed_cuts = false);
	bool update(void); //mandates:	0)Calculate and store all relevant parameters for current state (x/y limits, number of entries to histogram, etc.)
	//								1)update current picture. (only displayed histogram but not a png, as well as TF1)
	//								2)update physical parameters obtained from the current hist
	//								3)in case it is calibration hist (Ss), update calibration
	//								4)recalibrate Npe.
	//png and raw data are saved manually via interface
	PostProcessor(AllExperimentsResults* results); //results must be already processed, i.e. loaded

	void save(int ch);	//TODO: make that it saves results such as calibration and Npe(E) (for both PMT and MPPC). That is updates only one line in calibr. file
	void save_all(void);
	//TODO: save_calibration(void)
	void saveAs(std::string path, bool png_only);
	
	void clear(void);	//clear cuts for current histogram. Run cuts derived from it are not touched
	void clearAll(void); //clear everything, return to initial state (leaves all existing histograms empty)

	void plot_N_pe(int ch, GraphicOutputManager* gr_man);

	//adding/removing cuts does not update histogram. This is done because adding of several cuts in succession is often necessary
	void add_hist_cut(FunctionWrapper *picker, std::string name, bool affect_hist);
	void add_hist_cut(FunctionWrapper *picker, std::string name, int channel, bool affect_hist);
	void off_ch(int channel);
	void on_ch(int channel);
	int list_hist_cuts (void);
	void remove_hist_cut(int index);
	void remove_hist_cut(std::string name);
	void remove_hist_cut(std::string name, int ch);
	void set_as_run_cut(std::string name);//adds current drawn_limits in HistogramSetups to runs cut (from current exp, channel and type)
	int list_run_cuts (void);
	void unset_as_run_cut(std::string name);//deletes current exp,ch and type from current cuts (if present) deletes from back, that is
	//if a single exp,ch,type produces several EventCuts, unset must be called respective amount of times
	void print_events (std::string file, int run_offset, int sub_runs, bool accepted); //accepted = true - print accepted events, false - rejected ones
	void do_fit(bool do_fit);
	void set_N_bins(int N);
	void set_N_bins(int from, int to); //Or NbinsX, NbinsY for 2D histograms

	void set_zoom (double xl, double xr);
	void set_zoom_y (double yl, double yr);
	void set_zoom (double xl, double xr, double yl, double yr);
	void unset_zoom(bool do_update = true);
	void set_log_x(bool is_log);
	void set_log_y(bool is_log);
	void set_log_z(bool is_log);
	bool set_X_title(std::string text);
	bool set_Y_title(std::string text);

	void view_event(int event_index, int N_bins, double x_min, double x_max); //If N_bins =0 draw graph

	//Trigger adjustment related:
	bool unset_trigger_offsets(void);
	bool set_trigger_offsets(double extra_offset); //uses trigger type histogram only

	void set_fit_gauss(int N);
	void set_parameter_val(int index, double val);
	void set_parameter_limits(int index, double left, double right);

	void status(Bool_t full);
public:
	friend class StateData;
	friend class TriggerData;
};

#endif
