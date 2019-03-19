#ifndef FUNCTION_WRAPPER_
#define FUNCTION_WRAPPER_

#include "GlobalDefinitions.h"

#ifdef __MAKECINT__
typedef bool(*CUTTER)(std::vector<double>& pars, int run_n, void* stat_data);
#endif

class FunctionWrapper {
protected:
	void* static_data;//executes capture for lambda function
	CUTTER func;
	CUTTER_DRAW draw_method;
public:
	FunctionWrapper(void* stat_data);
	~FunctionWrapper();
	Bool_t operator () (std::vector<double> &pars, int run_n);
	Bool_t Draw (TCanvas* can);
	void SetFunction(CUTTER f);
	void SetDrawFunction(CUTTER_DRAW f);
};

#endif
