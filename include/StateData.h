#ifndef STATE_DATA_H
#define STATE_DATA_H

#include "GlobalDefinitions.h"
#include "GlobalParameters.h"
#include "PolynomialFit.h"
#include "AnalysisStates.h"
#include "HistogramSetups.h"
#include "PostProcessor.h"

//TODO: create correlation data as well, a lot of rework in that case.

//This class contains data/settings specific to certain analysis states only
class AStates;
class HistogramSetups;
class CanvasSetups;

class StateData
{
public:
	static bool IsForState(CanvasSetups *state); //I want this method to be virtual AND static
	static bool IsForState(CanvasSetups *state, int channel, AStates::Type type); //I want this method to be virtual AND static
	virtual bool IsForState_virt(CanvasSetups *state);
	virtual bool IsForState_virt(CanvasSetups *state, int channel, AStates::Type type);

	StateData();
	StateData(const CanvasSetups *for_state);
	virtual ~StateData();
	virtual void SetDefaultSettings(const CanvasSetups *for_state);
	virtual StateData * Clone ();

	//I want this method to be virtual AND static
	static StateData* GetData(CanvasSetups *setups, int channel, AStates::Type type); //returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
	//I want this method to be virtual AND static
	static StateData* GetData(void);//returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
	//I want this method to be virtual AND static
	static void DataChanged(void);


	virtual void* GetData_virt(CanvasSetups *setups, int channel, AStates::Type type); //returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
	virtual void* GetData_virt(void);//returns NULL if current setups do not contain this data class. Returns castable pointer otherwise
	virtual void DataChanged_virt(void);
};

class TriggerData: public StateData
{
public:
	double trigger_window;
	static bool IsForState(CanvasSetups *state);
	static bool IsForState(CanvasSetups *state, int channel, AStates::Type type);
	virtual bool IsForState_virt(CanvasSetups *state, int channel, AStates::Type type);  //override

	TriggerData();
	TriggerData(const CanvasSetups *for_state);
	virtual void SetDefaultSettings(const CanvasSetups *for_state); //override
	virtual StateData * Clone ();

	//I want this method to be virtual AND static
	static TriggerData* GetData(CanvasSetups *setups, int channel, AStates::Type type); //returns NULL if current setups do not contain this data class.
	//I want this method to be virtual AND static
	static TriggerData* GetData(void);//returns NULL if current setups do not contain this data class.
	//I want this method to be virtual AND static
	static void DataChanged(void);
	//CINT interface:
	static void SetShaping(double val); //in microseconds
	static double GetShaping(void); //in microseconds
};

class TriggerFitData: public StateData
{
public:
	enum TriggerType : int {tbNpeaks = 0, tbNpe = 1, tbS = 2}; //tb = trigger by
	DataVector exp_pulse_shape; //experimental, set from previous iteration of analysis
	TriggerType trigger_type;
	double t0_precision; //target precision of trigger position
	double scan_dt; //delta t for initial pulse-shape scan

	static bool IsForState(CanvasSetups *state);
	static bool IsForState(CanvasSetups *state, int channel, AStates::Type type);
	virtual bool IsForState_virt(CanvasSetups *state, int channel, AStates::Type type); //override

	TriggerFitData();
	TriggerFitData(const CanvasSetups *for_state);
	virtual void SetDefaultSettings(const CanvasSetups *for_state); //override
	virtual StateData * Clone (); //override

	bool IsValid() const;

	//I want this method to be virtual AND static
	static TriggerFitData* GetData(CanvasSetups *setups, int channel, AStates::Type type); //returns NULL if current setups do not contain this data class.
	static TriggerFitData* GetData(void);//returns NULL if current setups do not contain this data class.
	//I want this method to be virtual AND static
	static void DataChanged(void);
	//CINT interface:
	static void SetPulseShape(DataVector &pulse_shape);
	static void SetPulseShape(std::string fname);
	static DataVector GetPulseShape(void);
	static void SetTriggerType(TriggerType type);
	static TriggerType GetTriggerType(void);
	static void SetTriggerPrecision(double precision);
	static double GetTriggerPrecision(void);
	static void SetTriggerFirstScant(double dt);
	static double GetTriggerFirstScant(void);
};

class TriggerAvgTData: public StateData
{
public:
	enum TriggerType : int {tbNpeaks = 0, tbNpe = 1, tbS = 2}; //tb = trigger by
	TriggerType trigger_type;

	static bool IsForState(CanvasSetups *state);
	static bool IsForState(CanvasSetups *state, int channel, AStates::Type type);
	virtual bool IsForState_virt(CanvasSetups *state, int channel, AStates::Type type); //override

	TriggerAvgTData();
	TriggerAvgTData(const CanvasSetups *for_state);
	virtual void SetDefaultSettings(const CanvasSetups *for_state); //override
	virtual StateData * Clone (); //override

	bool IsValid() const;

	//I want this method to be virtual AND static
	static TriggerAvgTData* GetData(CanvasSetups *setups, int channel, AStates::Type type); //returns NULL if current setups do not contain this data class.
	static TriggerAvgTData* GetData(void);//returns NULL if current setups do not contain this data class.
	//I want this method to be virtual AND static
	static void DataChanged(void);
	//CINT interface:
	static void SetTriggerType(TriggerType type);
	static TriggerType GetTriggerType(void);
};

class ShapeFitData: public StateData
{
public:
	enum PeakType : int {ptNpeaks = 0, ptNpe = 1, ptS = 2};
	PeakType peak_type;
	int n_parameters;
	std::vector<std::pair<double, double>> par_bounds;
	std::vector<double> par_precisions;
	FitWrapper fit_function;
	int parameter_to_plot; //for PostProcessor
	std::vector<std::string> par_names; //for PostProcessor

	static bool IsForState(CanvasSetups *state);
	static bool IsForState(CanvasSetups *state, int channel, AStates::Type type);
	virtual bool IsForState_virt(CanvasSetups *state, int channel, AStates::Type type); //override

	ShapeFitData();
	ShapeFitData(const CanvasSetups *for_state);
	virtual void SetDefaultSettings(const CanvasSetups *for_state); //override
	virtual StateData * Clone (); //override

	bool IsValid() const;

	//I want this method to be virtual AND static
	static ShapeFitData* GetData(CanvasSetups *setups, int channel, AStates::Type type); //returns NULL if current setups do not contain this data class.
	static ShapeFitData* GetData(void);//returns NULL if current setups do not contain this data class.
	//I want this method to be virtual AND static
	static void DataChanged(void);
	virtual void DataChanged_virt(void); //override
	//returns n parameters + likelihood
	void PeaksToXY(std::deque<peak_processed> &peaks, std::vector<double> &xs, std::vector<double> &ys) const;
	std::vector<double> Fit(std::deque<peak_processed> &peaks) const;
	//CINT interface:
	static void SetPeakType(PeakType type);
	static PeakType GetPeakType(void);
	static void SetNPars(int n);
	static int GetNPars(void);
	static void SetPrecision(int n, double precision);
	static double GetPrecision(int n);
	static void SetPrecisions(std::vector<double> precisions);
	static std::vector<double> GetPrecisions(void);
	static void SetBound(int n, double min, double max);
	static void SetBound(int n, std::pair<double, double> bound);
	static void SetBound(int n, double fixed);
	static std::pair<double, double> GetBound(int n);
	static void SetBounds(std::vector<double> mins, std::vector<double> maxs);
	static void SetBounds(std::vector<std::pair<double, double>> bounds);
	static std::vector<std::pair<double, double>> GetBounds(void);
	static void SetPlotParameter(int index);
	static int GetPlotParameter(void);
	static void SetParameterName(int n, std::string name);
	static std::string GetParameterName(int n);
	static void SetParameterNames(std::vector<std::string> names);
	static std::vector<std::string> GetParameterNames(void);
	static FIT_F SetFitFunction(FIT_F f); //ignore run_n and stat_data.
};

#endif
