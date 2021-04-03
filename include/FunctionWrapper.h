#ifndef FUNCTION_WRAPPER_
#define FUNCTION_WRAPPER_

#include "GlobalDefinitions.h"

#ifdef __MAKECINT__
typedef bool(*CUTTER)(std::vector<double>& pars, int run_n, void* stat_data);
typedef double(*FIT_F)(double *x, double *pars, void* stat_data);
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
	CUTTER SetFunction(CUTTER f);
	void SetDrawFunction(CUTTER_DRAW f);
	bool isValid(void) const;
};

class FitWrapper {
public:
	struct DrawData {
		std::vector<double> fit_pars;
		FIT_F fit_func = NULL;
		void* extra = NULL;
	};
protected:
	FIT_F fit_func;
	CUTTER_DRAW draw_method;
public:
	FitWrapper(void);
	~FitWrapper();
	double operator () (const double *x, const double *pars) const;
	Bool_t Draw (TCanvas* can, std::vector<double> fit_pars, void* extra_data) const;
	FIT_F SetFunction(FIT_F f);
	//void SetDrawFunction(CUTTER_DRAW f);
	bool isValid(void) const;
};

#endif
