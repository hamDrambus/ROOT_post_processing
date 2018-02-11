#ifndef  POLYNOM_2_ORDER_H
#define  POLYNOM_2_ORDER_H

#include "PolynomialFit.h"

class Polynom2Order : public PolynomialFit
{
protected:
	const DVECTOR *_xs_last;
	const DVECTOR *_ys_last; //thees are supposed to stay valid pointers and vectors before 'this' destruction
	//moreover vector is supposed to stay the same between Fit(...) calls
	DVECTOR::const_iterator _x_start;
	DVECTOR::const_iterator _x_finish;
	double _x_left, _x_right;
	double _x0_in; //the point in which x is set to be 0, this is for convinience and in order to avoid too large coefficients

	DVECTOR::const_iterator find_x_iterator_by_value(double x);
public:
	Polynom2Order();
	virtual void operator ()(const DVECTOR &xs_in, const DVECTOR &ys_in, TVectorD &pars_out, double in_x0 = 0) override;
	//^in_x0 - in what point set zero x (In the SG filter it is convinient to set x_in
	//to the point in which value is calculated
	virtual void operator ()(const DVECTOR &xs_in, const DVECTOR &ys_in, int offset, int N_points, TVectorD &pars_out, double in_x0 = 0) override;
	//only for a part of a vector
	virtual void setOrder(int n) override;

	void FindMaximum(DVECTOR::const_iterator &x_max, double &x_max_exact, double &y_max_exact);
	void FindMinimum(DVECTOR::const_iterator &x_min, double &x_min_exact, double &y_min_exact);
	void FindIntersection(DVECTOR::const_iterator &x_inter, DVECTOR::const_iterator &x_inter2, double &x_inter_exact, double &x_inter_exact2,
		double threshold); //if not found, returns x_iter=xs.end();
	void FindExtremum(DVECTOR::const_iterator &x_extr, double &x_extr_exact, double &y_extr_exact);
	double Value(double X);
	double Derivative(double X);
};

#endif
