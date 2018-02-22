#ifndef CALIBRATION_INFO_H
#define CALIBRATION_INFO_H

#include "GlobalParameters.h"
#include "PolynomialFit.h"
#include "AnalysisStates.h"

class AnalysisStates;

//TODO: is AnalysisStates inheritance required?
class CalibrationInfo {//recalculation of MPPC and PMT areas to photoelcetrons as well as Npe from Double_t Int_tegral
	//this class does not access fit parameters and such, but specifies how avr_s1pe must be calculated for each experiment(x)mppc_channel  
public:
	enum S1pe_method {Ignore, Using1pe, Using2pe, Using1pe2pe, UsingMean};
protected:
	AnalysisStates* state_info; //for channels only
	//for each mppc channel
	std::deque<std::pair<Int_t, Int_t>> N_used_in_calibration; //that is for calculating s1pe (S of 1 photoelectron)
	//experiment->channel
	std::deque<std::deque<S1pe_method> > method;
	std::deque<std::deque<Double_t> > avr_S1pe;//from manual fitting/analysing of histograms and with cuts applied
	std::deque<std::deque<Double_t> > avr_S2pe;

	std::deque<Bool_t> forced_s1pe;
	std::deque<Double_t> s1pe;	//for each mppc channel
	Double_t s1pe_PMT3;		//ch 0
	Double_t s1pe_PMT1;		//ch 1

	void read_file(std::vector<std::pair<Int_t, Double_t>> &current_list);
public:
	CalibrationInfo(AnalysisStates* data);
	Double_t getPMT_S1pe(Int_t ch);
	void setPMT_S1pe(Int_t ch, Double_t val);
	//All methods below are for MPPC only
	void set_N_calib(Int_t ch, Int_t from, Int_t to);
	void set_N_calib(Int_t ch, std::pair<Int_t, Int_t>);
	std::pair<Int_t, Int_t> get_N_calib(Int_t ch);
	Double_t calculateS1pe(Int_t ch); //from avr_S1pe
	Double_t getS1pe(Int_t ch);
	void setS1pe(Int_t ch, Double_t val); //forces specific value. Not erased by calculateS1pe
	void unsetS1pe(Int_t ch);

	S1pe_method get_method(Int_t exp_ch, Int_t ch);//called from PostProcessor
	void set_method(Int_t exp_ch, Int_t ch, S1pe_method method);

	//setters/getters for avr_S1pe and avr_S2pe which are used for s1pe calcualtions
	void set_S1pe(Int_t ch, Int_t exp_index, Double_t val);
	Double_t get_S1pe(Int_t ch, Int_t exp_index);
	void set_S2pe(Int_t ch, Int_t exp_index, Double_t val);
	Double_t get_S2pe(Int_t ch, Int_t exp_index);

	std::deque<std::deque<std::pair<Bool_t, Bool_t>>> &recalibrate(std::deque<std::deque<Double_t>> &S2_S,
		std::deque<std::deque<Double_t>> &Double_I, std::vector<Double_t>& Fields);//sets S2_S and Double_I to N_pe. returns sucess vector
	std::deque<std::pair<Bool_t, Bool_t>> &recalibrate(std::deque<Double_t> &S2_S,
		std::deque<Double_t> &Double_I, std::vector<Double_t>& Fields, Int_t channel);//sets S2_S and Double_I to N_pe. returns sucess vector

	void Save(void);
	void Load(void);
};

#endif