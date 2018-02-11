#ifndef SAVITZKY_GOLAY_FILTER_H
#define SAVITZKY_GOLAY_FILTER_H

#include "GlobalParameters.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "PolynomialFit.h"
//TODO: add const S y(x)dx condition

class SavitzkyGolayFilter
{
protected:
	int _n_points;
	int _n_iterations;
	int _order;
public:
	SavitzkyGolayFilter(int n_points = 10, int order = 4, int n_iterations = 1);
	void setNPoints(int n);
	void setOrder(int n);
	void setNIter(int n);
	void setPars(int n_points = 10, int order = 4, int n_iterations = 1);

	int getNPoints(void) const;
	int getOrder(void) const;
	int getNIter(void) const;
	void getPars(int &n_points, int &order, int &n_iterations) const;

	void operator ()(const DVECTOR &xs_in, const DVECTOR &ys_in,
		DVECTOR &xs_out, DVECTOR &ys_out) const;
};

#endif
