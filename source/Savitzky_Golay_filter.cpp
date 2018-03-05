#include "Savitzky_Golay_filter.h"

SavitzkyGolayFilter::SavitzkyGolayFilter(Int_t n_poInt_ts, Int_t order, Int_t n_iterations)
{
	setOrder(order);
	setNPoInt_ts(n_poInt_ts);
	setNIter(n_iterations);
}
void SavitzkyGolayFilter::setNPoInt_ts(Int_t n)
{
	if ((n < 1) || !(_order < n))
		_n_poInt_ts = _order + 1;
	else
		_n_poInt_ts = n;
}
void SavitzkyGolayFilter::setOrder(Int_t n)
{
	if (n < 0)
		_order = 4;
	else
		_order = n;
}
void SavitzkyGolayFilter::setNIter(Int_t n)
{
	if (n < 0)
		_n_iterations = 1;
	else
		_n_iterations = n;
}
void SavitzkyGolayFilter::setPars(Int_t n_poInt_ts, Int_t order, Int_t n_iterations)
{
	setOrder(order);
	setNPoInt_ts(n_poInt_ts);
	setNIter(n_iterations);
}

Int_t SavitzkyGolayFilter::getNPoInt_ts(void) const
{	return _n_poInt_ts;}
Int_t SavitzkyGolayFilter::getOrder(void) const
{	return _order;}
Int_t SavitzkyGolayFilter::getNIter(void) const
{	return _n_iterations;}
void SavitzkyGolayFilter::getPars(Int_t &n_poInt_ts, Int_t &order, Int_t &n_iterations) const
{
	n_poInt_ts = _n_poInt_ts;
	order = _order;
	n_iterations = _n_iterations;
}

void SavitzkyGolayFilter::operator ()(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in,
	std::vector<Double_t> &xs_out, std::vector<Double_t> &ys_out) const
{
	if (xs_in.size() != ys_in.size())
		return;
	xs_out = xs_in;
	ys_out = ys_in;
	if (xs_in.size() < _n_poInt_ts)
		return;
	std::vector<Double_t> ys_in_copy;
	for (Int_t iter = 0; iter < _n_iterations; iter++) {
		ys_in_copy = ys_out;
		Int_t start_index = 0;
		for (Int_t h = 0; h < xs_in.size(); h++) {
			start_index = h - _n_poInt_ts / 2;
			start_index = start_index < 0 ? 0 : start_index;
			if (start_index > xs_in.size() - _n_poInt_ts)
				start_index = xs_in.size() - _n_poInt_ts;

			PolynomialFit fit(_order);
			TVectorD A;
			fit(xs_in, ys_in, start_index, _n_poInt_ts, A, xs_in[h]);
			ys_out[h] = A[0]; //I moved X coordinates to the poInt_t of Int_terest (xs_in[h]) in the matrix construction
			/*xs_out[h] = 0;
			for (Int_t row = 0; row < A.GetNrows(); row++)
			xs_out[h] += A[row] * pow(xs_in[h] - xs_in[h], row);*/
		}
	}
}
