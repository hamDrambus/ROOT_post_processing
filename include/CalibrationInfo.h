#ifndef CALIBRATION_INFO_H
#define CALIBRATION_INFO_H

#include "GlobalParameters.h"
#include "PolynomialFit.h"
#include "AnalysisStates.h"

class AnalysisStates;

//TODO: is AnalysisStates inheritance required?
class CalibrationInfo {//recalculation of MPPC and PMT areas to photoelectrons as well as Npe from double integral
	//this class does not access fit parameters and such, but specifies how avr_s1pe must be calculated for each experiment(x)mppc_channel  
public:
	enum S1pe_method {Ignore, Using1pe, Using2pe, Using1pe2pe, UsingMean};
	typedef std::map<double, std::pair<bool, double> > S1pe; //{V, (is_forced, S1pe[V*us])}
	//is_forced==false means S1pe calculated during analysis overrides values read from the calibration file.
	//typedef std::vector<std::pair<int/*ch*/, double/*S1pe*/> > MPPC_info_;
protected:
	std::deque<S1pe> s1pe_; //for each channel. First go PMT channels, then MPPC
	class S1pe_exp {
	public:
		//per channel
		std::deque<S1pe_method> method;
		std::deque<double> avr_S1pe;//from manual fitting/analyzing of histograms and with cuts applied
		std::deque<double> avr_S2pe;
		std::deque<int> avr_S1pe_w; //weight
		std::deque<int> avr_S2pe_w;
		//setters/getters for avr_S1pe and avr_S2pe which are used for s1pe calculations
		void set_S1pe_exp(int ch, double val, int weight);
		double get_S1pe_exp(int ch) const;
		int get_S1pe_w_exp(int ch) const;
		void set_S2pe_exp(int ch, double val, int weight);
		double get_S2pe_exp(int ch) const;
		int get_S2pe_w_exp(int ch) const;
		S1pe_method get_method(int ch) const;//called from PostProcessor
		void set_method(int ch, S1pe_method method);
	};
	std::deque<S1pe_exp> s1pe_exp_; //for each experiment.
protected:
	const AnalysisStates* state_info; //for channels only
	int ch_to_index(int ch) const;
	int ch_index_to_ch(int ch_index) const;

	std::deque<std::pair<int, int> > N_used_in_calibration; //that is for calculating s1pe (S of 1 photoelectron)
	bool read_file(std::ifstream &str, std::deque<S1pe> & to_table) const; //if to_table is not empty, it modifies its content. Does not override forced values. 
	void add_calibration_info (std::deque<S1pe> & to_table, int ch, double V, double S1pe) const; //preserves sorting, updates if necessary
	bool write_to_file (std::ofstream &str, std::deque<S1pe>& table) const;

	std::vector<int> translate_V_to_exp (int ch, double V) const;
	double translate_exp_to_V (int ch, int exp_index) const;
public:
	CalibrationInfo(const AnalysisStates* data, std::string fname);
	double get_S1pe(int ch, double V) const; //TODO: rewrite with boost::optional
	double set_S1pe(int ch, double V, double val);
	void force_S1pe(int ch, double pmt_v, double val); //forces specific value which is not erased by calculateS1pe
	void unforce_S1pe(int ch, double pmt_v); //forces specific value which is not erased by calculateS1pe

	double calculateS1pe(int ch, double V); //from experimental avr_S1pe
	void calculateS1pe(int ch); //for all V present.
	void calculateS1pe(void); //for all channels and V.

	//setters/getters for avr_S1pe and avr_S2pe which are used for s1pe calculations
	S1pe_method get_method(int ch, int exp_ch) const;//called from PostProcessor
	void set_method(int ch, int exp_ch, S1pe_method method);
	void set_S1pe_exp(int ch, int exp_index, double val, int weight);
	double get_S1pe_exp(int ch, int exp_index) const;
	int get_S1pe_weight_exp(int ch, int exp_index) const;
	void set_S2pe_exp(int ch, int exp_index, double val, int weight);
	double get_S2pe_exp(int ch, int exp_index) const;
	int get_S2pe_weight_exp(int ch, int exp_index) const;

	void Save(std::string fname) const; //Adds info to the file, not overrides it entirely
	bool Load(std::string fname); //Does not override already calculated or set manually values.
	bool isFull(bool dispaly_bad) const;
	void status(bool dispaly_all) const;

	//For recalibration of double integral to Npe
	//All methods below are for MPPC only
	void set_N_calib(int ch, int from, int to);
	void set_N_calib(int ch, std::pair<int, int>);
	std::pair<int, int> get_N_calib(int ch) const;
	/* //TODO: implement. Possibly change function declarations
	std::deque<std::deque<std::pair<Bool_t, Bool_t> > > &recalibrate(std::deque<std::deque<double> > &S2_S,
		std::deque<std::deque<double> >  &Double_I, std::vector<double>& Fields);//sets S2_S and Double_I to N_pe. returns success vector
	std::deque<std::pair<Bool_t, Bool_t> > &recalibrate(std::deque<double> &S2_S,
		std::deque<double> &Double_I, std::vector<double>& Fields, int channel);//sets S2_S and Double_I to N_pe. returns success vector
	*/
};

#endif
