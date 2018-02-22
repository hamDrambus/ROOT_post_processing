#ifndef FUNCTION_WRAPPER_
#define FUNCTION_WRAPPER_

#include "GlobalDefinitions.h"

#ifdef __MAKECINT__
typedef bool(*CUTTER)(std::vector<double>& pars, void* stat_data);
#endif

class FunctionWrapper {
protected:
	void* static_data;//executes capture for lambda function
	CUTTER func;
public:
	FunctionWrapper(void* stat_data);
	~FunctionWrapper();
	Bool_t operator () (std::vector<Double_t> &pars);
	void SetFunction(CUTTER f);
};


#endif
