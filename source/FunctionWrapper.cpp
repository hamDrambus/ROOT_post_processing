#include "FunctionWrapper.h"

FunctionWrapper::FunctionWrapper(void* stat_data)
	:static_data(stat_data), func(NULL), draw_method(NULL)
{
}
FunctionWrapper::~FunctionWrapper()
{}
Bool_t FunctionWrapper::operator () (std::vector<double> &pars, int run_n)
{
	return ((NULL==func) ? kTRUE : func(pars, run_n, static_data));
}

Bool_t FunctionWrapper::Draw (TCanvas* can)
{
	return ((NULL==draw_method) ? kFALSE : draw_method(can, static_data));
}

CUTTER FunctionWrapper::SetFunction(CUTTER f)
{
	return func = f;//no deletions as it may be the pointer to both static and lambda function
	//to remove memory leak lambda functions must be deleted elsewhere
	//TODO: add destructor as pointer as well (always lambda hence destructed in ~FunctionWrapper()
}

void FunctionWrapper::SetDrawFunction(CUTTER_DRAW f)
{
	draw_method = f;//no deletions as it may be the pointer to both static and lambda function
	//to remove memory leak lambda functions must be deleted elsewhere
	//TODO: add destructor as pointer as well (always lambda hence destructed in ~FunctionWrapper()
}

bool FunctionWrapper::isValid(void) const
{
	return (NULL!=static_data) && (NULL!= func);
}
