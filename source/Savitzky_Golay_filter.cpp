#include "Savitzky_Golay_filter.h"

SavitzkyGolayFilter::SavitzkyGolayFilter(int n_points, int order, int n_iterations)
{
	setOrder(order);
	setNPoints(n_points);
	setNIter(n_iterations);
}
void SavitzkyGolayFilter::setNPoints(int n)
{
	if ((n < 1) || !(_order < n))
		_n_points = _order + 1;
	else
		_n_points = n;
}
void SavitzkyGolayFilter::setOrder(int n)
{
	if (n < 0)
		_order = 4;
	else
		_order = n;
}
void SavitzkyGolayFilter::setNIter(int n)
{
	if (n < 0)
		_n_iterations = 1;
	else
		_n_iterations = n;
}
void SavitzkyGolayFilter::setPars(int n_points, int order, int n_iterations)
{
	setOrder(order);
	setNPoints(n_points);
	setNIter(n_iterations);
}

int SavitzkyGolayFilter::getNPoints(void) const
{	return _n_points;}
int SavitzkyGolayFilter::getOrder(void) const
{	return _order;}
int SavitzkyGolayFilter::getNIter(void) const
{	return _n_iterations;}
void SavitzkyGolayFilter::getPars(int &n_points, int &order, int &n_iterations) const
{
	n_points = _n_points;
	order = _order;
	n_iterations = _n_iterations;
}

void SavitzkyGolayFilter::operator ()(const DVECTOR &xs_in, const DVECTOR &ys_in,
	DVECTOR &xs_out, DVECTOR &ys_out) const
{
	if (xs_in.size() != ys_in.size())
		return;
	xs_out = xs_in;
	ys_out = ys_in;
	if (xs_in.size() < _n_points)
		return;
	DVECTOR ys_in_copy;
	for (int iter = 0; iter < _n_iterations; iter++) {
		ys_in_copy = ys_out;
		int start_index = 0;
		for (int h = 0; h < xs_in.size(); h++) {
			start_index = h - _n_points / 2;
			start_index = start_index < 0 ? 0 : start_index;
			if (start_index > xs_in.size() - _n_points)
				start_index = xs_in.size() - _n_points;

			PolynomialFit fit(_order);
			TVectorD A;
			fit(xs_in, ys_in, start_index, _n_points, A, xs_in[h]);
			ys_out[h] = A[0]; //I moved X coordinates to the point of interest (xs_in[h]) in the matrix construction
			/*xs_out[h] = 0;
			for (int row = 0; row < A.GetNrows(); row++)
			xs_out[h] += A[row] * pow(xs_in[h] - xs_in[h], row);*/
		}
	}
}