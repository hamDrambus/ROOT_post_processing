#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "TROOT.h"

#include "SignalOperations.h"
#include "AllExperimentsResults.h"
#include "GraphicOutputManager.h"
#include "AnalysisStates.h"
#include "CalibrationInfo.h"
#include "EventCut.h"
#include "HistogramSetups.h"

class PostProcessor : public AnalysisStates {
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

	//experiment->channel->{Ss,S2_S,double_I}
	STD_CONT<STD_CONT<STD_CONT<HistogramSetups*>>> manual_setups;
	//experinent->cut array
	STD_CONT < STD_CONT<EventCut> > RunCuts;

	//experiment->channel
	STD_CONT <STD_CONT<double>> avr_S2_S; //initial (automatic) values are set in processAllExperiments
	STD_CONT <STD_CONT<double>> avr_double_I;
	STD_CONT <double> PMT3_avr_S2_S;
	STD_CONT <double> PMT1_avr_S2_S;

	/*TH1D* createDefMPPCHist(DVECTOR &what, std::string name, double left_cutoff, double right_cutoff_from_RMS, int N_bins = 0);
	TH1D* createMPPCHist_peaks_S(STD_CONT<STD_CONT<peak>> &what, std::string name, double left_cutoff, double right_cutoff_from_RMS, int N_bins = 0);
	TF1* createDefMPPCFitFunc(TH1D* hist, std::string name);
	*/
	/*void update_by_manual_setups(std::string experiment, int ch, int type);
	void update_results(std::string experiment, int channel, Type type);*/
	

	
	virtual bool StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type,bool save) override;
	
	void set_hist_setups(HistogramSetups* setups, std::string exp, int channel, Type type);//does not call update
	HistogramSetups* get_hist_setups(std::string exp, int channel, Type type);//does not call update

	void FillHist(void* p_hist);//considers cuts and histogram tipe (void*)==either TH1D* or TH2D*
	//see function for DVECTOR &vals usage in cuts' picker
	int numOfFills(void);
	std::pair<double, double> hist_x_limits(void); //considering cuts
	std::pair<double, double> hist_y_limits(void); //valid only for 2d plots
	void set_default_hist_setups(void);//

	TF1* create_fit_function(HistogramSetups* func);
	void update_fit_function(void); //uses current_fit_func and current_setups
	//TODO: add setting average S2 and double I without manual setups. Maybe as exit() method, which will
	//set S2 and double integral with NULL HistogramSetups by default and won't touh the calibration
	void update_physical(void); //2nd and 3rd mandates of ::update(void)
	void update_Npe(void);		//4th part of ::update(void). TODO: actually it is better to move it to CalibrationInfo.

	bool is_TH1D_hist();
	std::string hist_name();

public:
	void update(void); //mandates:	1)update current picture. (only displayed histogram but not a png, as well as TLines and TF1)
	//								2)update physical parameters obtained from the current hist
	//								3)in case it is calibration hist (Ss), update calibration
	//								4)recalibrate Npe.
	//png is saved at exit or next/prev function.
	PostProcessor(AllExperimentsResults* results); //results must be already processed, e.g. loaded
	CalibrationInfo calibr_info;
	
	void save(int ch);	//saves results such as calibration and Npe(E) (for both PMT and MPPC)
	void save_all(void);
	
	//void update(ParameterPile::experiment_area to_update, int type/*{Ss,S2_S,double_I}*/); //updates images and avr_... parameters
	//void update(std::string experiment, int channel, int type/*{Ss,S2_S,double_I}*/); //updates images and avr_... parameters
	void plot_N_pe(int ch, GraphicOutputManager* gr_man);

	//TODO: add several default cuts (e.g. from left/right limit)
	void add_hist_cut(std::function<bool(DVECTOR &vals)> &picker, std::string name = "");
	void remove_hist_cut(std::string name = "");
	void set_as_run_cut(std::string name = "");//adds current drawn_limits in HistogramSetups to runs cut (from current exp, channel and type)
	void unset_as_run_cut(std::string name = "");//deletes current exp,ch and type from current cuts (if present) deletes from back, that is
	//if a single exp,ch,type produces several EventCuts, unset must be called respective amount of times
	void do_fit(bool upd_vis = true);
	//per_point_data process_default(std::string experiment, int ch, int type/*{Ss,S2_S,double_I}*/);
	//DVECTOR* get_data(int exp_index, int channel, int type);
};

#endif