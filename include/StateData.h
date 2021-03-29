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
protected:
	bool static_instance;
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

#endif
