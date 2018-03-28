#include "FunctionWrapper.h"

FunctionWrapper::FunctionWrapper(void* stat_data)
	:static_data(stat_data), func(NULL)
{
}
FunctionWrapper::~FunctionWrapper()
{}
Bool_t FunctionWrapper::operator () (std::vector<double> &pars, int run_n)
{
	return ((NULL==func) ? kTRUE : func(pars, run_n, static_data));
}
void FunctionWrapper::SetFunction(CUTTER f)
{
	func = f;//no deletions as it may be the pointer to both static and lambda function
	//to remove memory leak lambda functions must be deleted elsewhere
	//TODO: add destructor as pointer as well (always lambda hence destructed in ~FunctionWrapper()
}
