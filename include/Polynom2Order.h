#ifndef  POLYNOM_2_ORDER_H
#define  POLYNOM_2_ORDER_H

#include "PolynomialFit.h"

class Polynom2Order
{
protected:
	PolynomialFit fitter;
	const std::vector<double> *_xs_last;
	const std::vector<double> *_ys_last; //these are supposed to stay valid pointer before 'this' destruction
	//moreover vector is supposed to stay the same between Fit(...) calls
	std::vector<double>::const_iterator _x_start;
	std::vector<double>::const_iterator _x_finish;
	double _x_left, _x_right;
	double _x0_in; //the point in which x is set to be 0, this is for precision and in order to avoid too large coefficients
	std::vector<double> _last_coefs;

	std::vector<double>::const_iterator find_x_iterator_by_value(double x);
public:
	Polynom2Order();
#ifndef __ROOTCLING__
	std::vector<double> operator ()(const std::vector<double> &xs_in, const std::vector<double> &ys_in, boost::optional<double> &in_x0);
	std::vector<double> operator ()(const std::vector<double> &xs_in, const std::vector<double> &ys_in, int offset, int N_points, boost::optional<double> &in_x0);
	//only for a part of a vector
#endif
	//public interface for ROOT
	std::vector<double> operator ()(const std::vector<double> &xs_in, const std::vector<double> &ys_in);
	//Fit only part of a vector. offset+N_points-1 must be in the range of the vector
	std::vector<double> operator ()(const std::vector<double> &xs_in, const std::vector<double> &ys_in, int offset, int N_points);
	//in_x0 - relative to what point carry out fit
	std::vector<double> operator ()(const std::vector<double> &xs_in, const std::vector<double> &ys_in, double in_x0);
	std::vector<double> operator ()(const std::vector<double> &xs_in, const std::vector<double> &ys_in, int offset, int N_points, double in_x0);

	void FindMaximum(std::vector<double>::const_iterator &x_max, double &x_max_exact, double &y_max_exact);
	void FindMinimum(std::vector<double>::const_iterator &x_min, double &x_min_exact, double &y_min_exact);
	void Findintersection(std::vector<double>::const_iterator &x_inter, std::vector<double>::const_iterator &x_inter2, double &x_inter_exact, double &x_inter_exact2,
		double threshold); //if not found, returns x_iter=xs.end();
	void FindExtremum(std::vector<double>::const_iterator &x_extr, double &x_extr_exact, double &y_extr_exact);
	double Value(double X);
	double Derivative(double X);
};

#endif
