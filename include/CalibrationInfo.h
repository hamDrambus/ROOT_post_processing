#ifndef CALIBRATION_INFO_H
#define CALIBRATION_INFO_H

#include "GlobalParameters.h"
#include "AnalysisStates.h"

class AnalysisStates;

//TODO: is AnalysisStates inheritance required?
class CalibrationInfo {//recalculation of MPPC and PMT areas to photoelcetrons as well as Npe from double integral
	//this class does not access fit parameters and such, but specifies how avr_s1pe must be calculated for each experiment(x)mppc_channel  
public:
	enum S1pe_method {Ignore, Using1pe, Using2pe, Using1pe2pe, UsingMean};
protected:
	AnalysisStates* state_info; //for channels only
	//for each mppc channel
	STD_CONT<std::pair<int, int>> N_used_in_calibration; //that is for calculating s1pe (S of 1 photoelectron)
	//experiment->channel
	STD_CONT<STD_CONT<S1pe_method> > method;
	STD_CONT<STD_CONT<double> > avr_S1pe;//from manual fitting/analysing of histograms and with cuts applied
	STD_CONT<STD_CONT<double> > avr_S2pe;

	STD_CONT<bool> forced_s1pe;
	STD_CONT<double> s1pe;	//for each mppc channel
	double s1pe_PMT3;		//ch 0
	double s1pe_PMT1;		//ch 1
public:
	CalibrationInfo(AnalysisStates* data);
	double getPMT_S1pe(int ch);
	void setPMT_S1pe(int ch, double val);
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

	STD_CONT<STD_CONT<std::pair<bool, bool>>> &recalibrate(STD_CONT<STD_CONT<double>> &S2_S,
		STD_CONT<STD_CONT<double>> &double_I, DVECTOR& Fields);//sets S2_S and double_I to N_pe. returns sucess vector
	STD_CONT<std::pair<bool, bool>> &recalibrate(STD_CONT<double> &S2_S,
		STD_CONT<double> &double_I, DVECTOR& Fields, int channel);//sets S2_S and double_I to N_pe. returns sucess vector
};

#endif