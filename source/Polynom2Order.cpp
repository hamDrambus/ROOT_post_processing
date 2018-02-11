#include "Polynom2Order.h"

Polynom2Order::Polynom2Order() : PolynomialFit(2), _xs_last(NULL), _ys_last(NULL), _x_left(0), _x_right(0), _x0_in(0)
{
}
void Polynom2Order::operator ()(const DVECTOR &xs_in, const DVECTOR &ys_in,
	TVectorD &pars_out, double in_x0) //in_x0 - in what point set zero x (In the SG filter it is convinient to set x_in
	//to the point in which value is calculated
{
	this->operator()(xs_in, ys_in, 0, xs_in.size(), pars_out, in_x0);
}
void Polynom2Order::operator ()(const DVECTOR &xs_in, const DVECTOR &ys_in,
	int offset, int N_points, TVectorD &pars_out, double in_x0) //only for a part of a vector
{
	if (xs_in.size() != ys_in.size())
		return;
	if ((xs_in.size() - offset) < N_points)
		return;
	if (N_points < (_order + 1))
		return;
	_xs_last = &xs_in;
	_ys_last = &ys_in;
	_x_start = xs_in.begin() + offset;
	_x_finish = xs_in.begin() + offset + N_points;
	_x_left = *_x_start;
	_x_right = *(_x_finish - 1);
	_x0_in = in_x0;
	PolynomialFit::operator()(xs_in, ys_in, offset, N_points, pars_out, in_x0);
}
void Polynom2Order::setOrder(int n)
{
	_order = 2;
}

void Polynom2Order::FindMaximum(DVECTOR::const_iterator &x_max, double &x_max_exact, double &y_max_exact)
{
	x_max = _xs_last->end();
	if (_last_coefs[2] >= 0){ //max at the ends of the range
		y_max_exact = Value(_x_left);
		if (y_max_exact > Value(_x_right)){
			x_max_exact = _x_left;
			x_max = _x_start;
			return;
		} else {
			x_max_exact = _x_right;
			x_max = (_x_finish - 1);
			y_max_exact = Value(_x_right);
			return;
		}
	} else { //max is inside the range
		double x_extr = -_last_coefs[1] / (2 * _last_coefs[2]);
		x_extr += _x0_in;//mind that x_extr is calculated in the shifted X values
		if (!(x_extr<_x_left) && !(x_extr>_x_right)){ //extremum inside the range
			y_max_exact = Value(x_extr + _x0_in);
			x_max = find_x_iterator_by_value(x_extr);
			x_max_exact = x_extr;
			return;
		} else { //extremum is outside of the range
			y_max_exact = Value(_x_left);
			if (y_max_exact > Value(_x_right)){
				x_max_exact = _x_left;
				x_max = _x_start;
				return;
			} else {
				x_max_exact = _x_right;
				x_max = (_x_finish - 1);
				y_max_exact = Value(_x_right);
				return;
			}
		}
	}
}
void Polynom2Order::FindMinimum(DVECTOR::const_iterator &x_min, double &x_min_exact, double &y_min_exact)
{
	x_min = _xs_last->end();
	if (_last_coefs[2] <= 0){ //min at the ends of the range
		y_min_exact = Value(_x_left);
		if (y_min_exact < Value(_x_right)){
			x_min_exact = _x_left;
			x_min = _x_start;
			return;
		} else {
			x_min_exact = _x_right;
			x_min = (_x_finish - 1);
			y_min_exact = Value(_x_right);
			return;
		}
	} else { //max is inside the range
		double x_extr = -_last_coefs[1] / (2 * _last_coefs[2]);
		x_extr += _x0_in;//mind that x_extr is calculated in the shifted X values
		if (!(x_extr<_x_left) && !(x_extr>_x_right)){ //extremum inside the range
			y_min_exact = Value(x_extr + _x0_in);
			x_min = find_x_iterator_by_value(x_extr);
			x_min_exact = x_extr;
			return;
		} else { //extremum is outside of the range
			y_min_exact = Value(_x_left);
			if (y_min_exact < Value(_x_right)){
				x_min_exact = _x_left;
				x_min = _x_start;
				return;
			} else {
				x_min_exact = _x_right;
				x_min = (_x_finish - 1);
				y_min_exact = Value(_x_right);
				return;
			}
		}
	}
}

void Polynom2Order::FindIntersection(DVECTOR::const_iterator &x_inter, DVECTOR::const_iterator &x_inter2, double &x_inter_exact, double &x_inter_exact2,
	double threshold) //if not found, returns x_iter=xs.end();
{
	x_inter = _xs_last->end();
	x_inter2 = _xs_last->end();
	double discr = _last_coefs[1] * _last_coefs[1] - 4 * (_last_coefs[0] - threshold)*_last_coefs[2];
	if (discr >= 0){
		double x_intersection = (-_last_coefs[1] + std::sqrt(discr)) / (2 * _last_coefs[2]);
		double x_intersection2 = (-_last_coefs[1] - std::sqrt(discr)) / (2 * _last_coefs[2]);
		x_intersection += _x0_in;
		x_intersection2 += _x0_in;
		if (!(x_intersection<_x_left) && !(x_intersection>_x_right)){
			x_inter_exact = x_intersection;
			x_inter = find_x_iterator_by_value(x_intersection);
		}
		if (!(x_intersection2<_x_left) && !(x_intersection2>_x_right)){
			if (x_inter != _xs_last->end()){
				x_inter_exact2 = x_intersection2;
				x_inter2 = find_x_iterator_by_value(x_intersection2);
			} else {
				x_inter_exact = x_intersection2;
				x_inter = find_x_iterator_by_value(x_intersection2);
			}
		}
	}
}

void Polynom2Order::FindExtremum(DVECTOR::const_iterator &x_extr, double &x_extr_exact, double &y_extr_exact){
	x_extr_exact = -_last_coefs[1] / (2 * _last_coefs[2]);
	x_extr_exact += _x0_in;
	if (!(x_extr_exact<_x_left) && !(x_extr_exact>_x_right)){//extremum inside the range
		y_extr_exact = Value(x_extr_exact);
		x_extr = find_x_iterator_by_value(x_extr_exact);
		return;
	}
	x_extr = _xs_last->end();
}

double Polynom2Order::Value(double X){
	if (_last_coefs.GetNrows() < 3)
		return 0; //TODO: actually it would be better to throw.
	return _last_coefs[0] + _last_coefs[1] * (X - _x0_in) + _last_coefs[2] * (X - _x0_in)*(X - _x0_in);
}

double Polynom2Order::Derivative(double X){
	if (_last_coefs.GetNrows() < 3)
		return 0; //TODO: actually it would be better to throw.
	return _last_coefs[1] + _last_coefs[2] * 2 * (X - _x0_in);
}

DVECTOR::const_iterator Polynom2Order::find_x_iterator_by_value(double x)
{
	for (auto h = _x_start; h != (_x_finish - 1); h++) //find in which point of the vector the maximum realizes
		if (!(*h > x) && !(*(h + 1) < x)){
			if ((x - *h) > (*(h + 1) - x))
				return h + 1;
			else
				return h;
		}
	return _xs_last->end(); // mustn't occur
}
