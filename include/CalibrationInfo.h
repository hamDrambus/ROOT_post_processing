#ifndef CALIBRATION_INFO_H
#define CALIBRATION_INFO_H

#include "GlobalParameters.h"
#include "PolynomialFit.h"
#include "AnalysisStates.h"

class AnalysisStates;

//TODO: is AnalysisStates inheritance required?
class CalibrationInfo {//recalculation of MPPC and PMT areas to photoelcetrons as well as Npe from double integral
	//this class does not access fit parameters and such, but specifies how avr_s1pe must be calculated for each experiment(x)mppc_channel  
public:
	enum S1pe_method {Ignore, Using1pe, Using2pe, Using1pe2pe, UsingMean};
	typedef std::vector<std::pair<int/*ch*/, std::vector<std::pair<double/*V PMT*/, double /*S1pe*/> > > > PMT_info_;
	typedef std::vector<std::pair<int/*ch*/, double/*S1pe*/> > MPPC_info_;
protected:
	AnalysisStates* state_info; //for channels only
	//for each mppc channel
	std::deque<std::pair<int, int> > N_used_in_calibration; //that is for calculating s1pe (S of 1 photoelectron)
	//experiment->channel
	std::deque<std::deque<S1pe_method> > method;
	std::deque<std::deque<double> > avr_S1pe;//from manual fitting/analysing of histograms and with cuts applied
	std::deque<std::deque<double> > avr_S2pe;

	std::deque<Bool_t> forced_s1pe;
	std::deque<double> s1pe;	//for each mppc channel
	std::map<double, double> s1pe_PMT3;	//[{V,S1pe[V*us]}] ch 0
	std::map<double, double> s1pe_PMT1;	//[{V,S1pe[V*us]}] ch 1

	void read_file(std::vector<std::pair<int/*ch*/, std::string> > &current_list);
	void extract_PMT_info (std::vector<std::pair<int/*ch*/, std::string> > &current_list, PMT_info_ &PMT_table);
	void extract_MPPC_info (std::vector<std::pair<int/*ch*/, std::string> > &current_list, MPPC_info_ &MPPC_table);
	void add_to_PMT_info (PMT_info_& table, int ch, double V, double S1pe); //preserves sorting, updates if necessary
	void add_to_MPPC_info (MPPC_info_ &table, int ch, double S1pe);         //preserves sorting, updates if necessary
	void write_to_file (PMT_info_& PMT_table, MPPC_info_& MPPC_table);

public:
	CalibrationInfo(AnalysisStates* data);
	double getPMT_S1pe(int ch, int exp_ind);
	void setPMT_S1pe(int ch, double pmt_v, double val);
	//All methods below are for MPPC only
	void set_N_calib(int ch, int from, int to);
	void set_N_calib(int ch, std::pair<int, int>);
	std::pair<int, int> get_N_calib(int ch);
	double calculateS1pe(int ch); //from avr_S1pe
	double getS1pe(int ch);
	void setS1pe(int ch, double val); //forces specific value. Not erased by calculateS1pe
	void unsetS1pe(int ch);

	S1pe_method get_method(int exp_ch, int ch);//called from PostProcessor
	void set_method(int exp_ch, int ch, S1pe_method method);

	//setters/getters for avr_S1pe and avr_S2pe which are used for s1pe calcualtions
	void set_S1pe(int ch, int exp_index, double val);
	double get_S1pe(int ch, int exp_index);
	void set_S2pe(int ch, int exp_index, double val);
	double get_S2pe(int ch, int exp_index);

	std::deque<std::deque<std::pair<Bool_t, Bool_t> > > &recalibrate(std::deque<std::deque<double> > &S2_S,
		std::deque<std::deque<double> >  &Double_I, std::vector<double>& Fields);//sets S2_S and Double_I to N_pe. returns sucess vector
	std::deque<std::pair<Bool_t, Bool_t> > &recalibrate(std::deque<double> &S2_S,
		std::deque<double> &Double_I, std::vector<double>& Fields, int channel);//sets S2_S and Double_I to N_pe. returns sucess vector

	void Save(void);
	void Load(void);
};

#endif
