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


FitWrapper::FitWrapper(void) : fit_func(NULL)
{
	draw_method = [](TCanvas* can, void* stat_data) {
		if (NULL == stat_data || NULL == can)
			return false;
		DrawData* data = (DrawData*) stat_data;
		double scale = *((double*)data->extra);
		double min_x = can->GetLogx() ? std::pow(10.0, can->GetUxmin()) : can->GetUxmin();
		double max_x = can->GetLogx() ? std::pow(10.0, can->GetUxmax()) : can->GetUxmax();
		can->cd();
		class draw_wrapper {
		public:
			double scale;
			FIT_F f;
			draw_wrapper(double sc, FIT_F func) : scale(sc), f(func)
			{}
			double operator () (const double *x, const double *pars) const
			{	return scale * f(x, pars);}
		} to_draw(scale, data->fit_func);
		TF1 *f1 = new TF1("FitWrapperDraw", to_draw, min_x, max_x, data->fit_pars.size());
		for (int i = 0, i_end_ = data->fit_pars.size(); i!=i_end_; ++i)
			f1->SetParameter(i, data->fit_pars[i]);
		f1->SetNpx(800);
		f1->Draw("same");
		return true;
	};
}

FitWrapper::~FitWrapper()
{}

double FitWrapper::operator () (const double *x, const double *pars) const
{
	if (NULL != fit_func)
		return fit_func(x, pars);
	return DBL_MAX;
}

Bool_t FitWrapper::Draw (TCanvas* can, std::vector<double> fit_pars, void* extra_data) const
{
	if (NULL == draw_method || NULL == fit_func)
		return false;
	DrawData data;
	data.fit_func = fit_func;
	data.fit_pars = fit_pars;
	data.extra = extra_data;
	return draw_method(can, (void*)&data);
}

FIT_F FitWrapper::SetFunction(FIT_F f)
{
	return fit_func = f;
}

bool FitWrapper::isValid(void) const
{
	return (NULL != fit_func);
}
