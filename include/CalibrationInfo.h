#ifndef CALIBRATION_INFO_H
#define CALIBRATION_INFO_H

#include "GlobalParameters.h"
#include "PolynomialFit.h"
#include "AnalysisStates.h"

class AnalysisStates;

//TODO: some cleanup and light restructure is required - too much of repeating the code
class CalibrationInfo {//recalculation of MPPC and PMT areas to photoelectrons as well as Npe from double integral
	//this class does not access fit parameters and such, but specifies how avr_s1pe must be calculated for each experiment(x)mppc_channel  
public:
	enum S1pe_method {Ignore, Using1pe, Using2pe, Using1pe2pe, UsingMean};
	class S1pe_exp {
	public:
		//per channel
		std::deque<S1pe_method> method;
		std::deque<double> avr_S1pe;//from manual fitting/analyzing of histograms and with cuts applied
		std::deque<double> avr_S2pe;
		std::deque<int> avr_S1pe_w; //weight
		std::deque<int> avr_S2pe_w;
		//setters/getters for avr_S1pe and avr_S2pe which are used for s1pe calculations
		void set_S1pe_exp(std::size_t ch, double val, int weight);
		double get_S1pe_exp(std::size_t ch) const;
		int get_S1pe_w_exp(std::size_t ch) const;
		void set_S2pe_exp(std::size_t ch, double val, int weight);
		double get_S2pe_exp(std::size_t ch) const;
		int get_S2pe_w_exp(std::size_t ch) const;
		S1pe_method get_method(std::size_t ch) const;//called from PostProcessor
		void set_method(std::size_t ch, S1pe_method method);
	};
	class S1pe_table {
	public:
		typedef std::map<double, std::pair<bool, double> > S1pe; //{V, (is_forced, S1pe[V*us])}
		//is_forced==false means S1pe calculated during analysis overrides values read from the calibration file.
		std::deque<S1pe> s1pe_; //for each channel (may be more channels than in state_info). First go PMT channels, then MPPC
		std::deque<int> channels_;//Has separate channel information from state_info, because calibrations read from file and current analysis may be incompatible
		int ch_to_index(int ch) const;
		int ch_index_to_ch(int ch_index) const;
		std::pair<std::size_t, std::size_t> ch_bound_indices(int ch, bool &failed) const; //can't use boost, so have to use &failed
		S1pe_table();
		bool push(int ch); //does not erase already existing ch. returns true if channel is created. Preserves sorting by channel
		bool push(int ch, double V, double S1pe, bool forced, bool forced_by_default); //creates or updates entry, returns true if new entry is created
		void remove(int ch);
		//void force_S1pe(int ch, double pmt_v, double val); //forces specific value which is not erased by calculateS1pe
		bool unforce_S1pe(int ch, double V); //returns true if recalculation is required
		bool isforced_S1pe(int ch, double V);
		double get_S1pe(int ch, double V) const; //~~TODO~~ ROOT's CINT can't handle boost: rewrite with boost::optional
	};
protected:
	S1pe_table s1pe_table_;
	std::deque<S1pe_exp> s1pe_exp_; //for each experiment. Same amount of channels as in state_info
protected:
	const AnalysisStates* state_info; //for channels only
	int ch_to_index(int ch) const;
	int ch_index_to_ch(int ch_index) const;

	std::deque<std::pair<int, int> > N_used_in_calibration; //that is for calculating s1pe (S of 1 photoelectron)
	bool read_file(std::ifstream &str, S1pe_table& to_table) const; //if to_table is not empty, it modifies its content. Does not override forced values.
	bool write_to_file (std::ofstream &str, S1pe_table& table) const;

	std::vector<int> translate_V_to_exp (int ch, double V) const;
	double translate_exp_to_V (int ch, int exp_index) const;
public:
	CalibrationInfo(const AnalysisStates* data, std::string fname);
	double get_S1pe(int ch, double V) const; //~~TODO~~ ROOT's CINT can't handle boost: rewrite with boost::optional
	void force_S1pe(int ch, double V, double val); //forces specific value which is not erased by calculateS1pe
	void unforce_S1pe(int ch, double V); //forces specific value which is not erased by calculateS1pe

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
	bool Load(std::string fname); //Overrides already calculated or set manually (forced) values.
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
