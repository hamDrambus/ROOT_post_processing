#ifndef POLYNOMIAL_FIT_H
#define POLYNOMIAL_FIT_H

#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>
#ifndef __ROOTCLING__
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/lu.hpp>
#endif
#include "GlobalParameters.h"

//Copy of PolynomialFit.h from ThreeDimSimulation project, but boost is hidden from ROOT
//because it won't be able to create dictionary for it.

class PolynomialFit {
protected:
	std::size_t _order;
public:
	PolynomialFit(std::size_t order);
	~PolynomialFit();
	void setOrder(std::size_t n) {
		_order = n;
	}
	std::size_t getOrder(void) const {
		return _order;
	}
#ifndef __ROOTCLING__
	//in_x0 - relative to what point carry out fit. Automatic value is set if boost::none is passed.
	std::vector<double> operator ()(const std::vector<std::pair<double, double>> &vals_in, boost::optional<double> &in_x0) const;
	//Fit only part of a vector. offset+N_points-1 must be in the range of the vector
	std::vector<double> operator ()(const std::vector<std::pair<double, double>> &vals_in, int offset, int N_points, boost::optional<double> &in_x0) const;
	//in_x0 - relative to what point carry out fit. Automatic value is set if boost::none is passed.
	std::vector<double> operator ()(const std::vector<double> &xs, const std::vector<double> &ys, boost::optional<double> &in_x0) const;
	//Fit only part of a vector. offset+N_points-1 must be in the range of the vector
	std::vector<double> operator ()(const std::vector<double> &xs, const std::vector<double> &ys, int offset, int N_points, boost::optional<double> &in_x0) const;
#endif
	//public interface for ROOT
	std::vector<double> operator ()(const std::vector<std::pair<double, double>> &vals_in) const;
	//Fit only part of a vector. offset+N_points-1 must be in the range of the vector
	std::vector<double> operator ()(const std::vector<std::pair<double, double>> &vals_in, int offset, int N_points) const;
	//in_x0 - relative to what point carry out fit
	std::vector<double> operator ()(const std::vector<std::pair<double, double>> &vals_in, double in_x0) const;
	std::vector<double> operator ()(const std::vector<std::pair<double, double>> &vals_in, int offset, int N_points, double in_x0) const;

	std::vector<double> operator ()(const std::vector<double> &xs, const std::vector<double> &ys) const;
	//Fit only part of a vector. offset+N_points-1 must be in the range of the vector
	std::vector<double> operator ()(const std::vector<double> &xs, const std::vector<double> &ys, int offset, int N_points) const;
	//in_x0 - relative to what point carry out fit
	std::vector<double> operator ()(const std::vector<double> &xs, const std::vector<double> &ys, double in_x0) const;
	std::vector<double> operator ()(const std::vector<double> &xs, const std::vector<double> &ys, int offset, int N_points, double in_x0) const;
};

//Wraps PolynomialFit: stores raw data, N points used in every fit and last region (cache_n_from, cache_n_to) in which fit/interpolation took place.
//The latter is required for optimization, because polynomial coefficients are not updated unless necessary (x moved from previous region)
class DataVector {
	//TODO: add thread-local cache for getX_indices and getY_indices
protected:
	std::vector<std::pair<double, double>> xys;
	PolynomialFit fitter;
	std::size_t N_used;

	bool use_left, use_right;
#ifndef __ROOTCLING__
	boost::optional<double> left_value, right_value;
#endif
public:
	DataVector(std::size_t fit_order = 1, std::size_t N_used = 2);
	DataVector(std::vector<double> &xx, std::vector<double> &yy, std::size_t fit_order, std::size_t N_used);
	virtual ~DataVector();

	void initialize(std::vector<double> &xx, std::vector<double> &yy, std::size_t fit_order, std::size_t N_used);

	void setOrder(std::size_t ord) {
		fitter.setOrder(ord);
	}
	std::size_t getOrder(void) const {
		return fitter.getOrder();
	}
	void setNused(std::size_t N) {
		N_used = std::max((std::size_t)1, N);
	}
	std::size_t getNused(void) const {
		return N_used;
	}
	//precedence goes to use_left-/right-most methods.
	void use_leftmost(bool use) {
		use_left = use;
	}
	void use_rightmost(bool use) {
		use_right = use;
	}
	void set_leftmost(double val) {
		left_value = val;
	}
	void unset_leftmost(void) {
		left_value = boost::none;
	}
	void set_rightmost(double val) {
		right_value = val;
	}
	void unset_rightmost(void) {
		right_value = boost::none;
	}
	void set_out_value(double val) {
		set_leftmost(val);
		set_rightmost(val);
	}
	void unset_out_value(void) {
		unset_leftmost();
		unset_rightmost();
	}
	void erase(std::size_t n) {
		xys.erase(xys.begin() + n);
	}
	void erase() {
		xys.clear();
	}
	void clear(void) {
		xys.clear();
	}
	std::size_t size(void) const {
		return xys.size();
	}
	double getX(std::size_t n) const {
		return xys[n].first;
	}
	double getY(std::size_t n) const {
		return xys[n].second;
	}
	std::pair<double, double> getXY(std::size_t n) const {
		return xys[n];
	}
	std::pair<double, double>& operator[](std::size_t ind) {
		return xys[ind];
	}
	void resize(std::size_t sz) {
		xys.resize(sz);
	}
	bool isValid(void) const {
		return (N_used > fitter.getOrder()) && N_used <= size();
	}

	void insert(double x, double y);
	void push_back(double x, double y);

	//save/load full state except cache from file
	void read(std::ifstream& str);
	void write(std::string fname, std::string comment = "") const;
	void write(std::ofstream& str, std::string comment = "") const;

#ifndef __ROOTCLING__
	double operator()(double X_point, boost::optional<double> x0 = boost::none) const; //x0 = point is recommended to use. At least x0 must be close to point, or there will be large errors otherwise
	//Warning! These functions have defined behaviour only when X/Y values are sorted in the ascending order.
	//DataVector's X values are supposed to be always sorted, but there is no guarantee about Ys;
	boost::optional<std::pair<std::size_t, std::size_t>> getX_indices(double X_point) const; //[n_min, n_max] are used, not [n_min, n_max).N_used == n_max - n_min + 1 >= order + 1
	boost::optional<std::pair<std::size_t, std::size_t>> getY_indices(double Y_point) const; //[n_min, n_max] are used, not [n_min, n_max).N_used == n_max - n_min + 1 >= order + 1
#endif
	//public interface for ROOT
	double operator()(double X_point, double x0) const; //x0 = point is recommended to use. At least x0 must be close to point, or there will be large errors otherwise
	double operator()(double X_point) const; //corresponds to x0=boost::none
	//Warning! These functions have defined behaviour only when X/Y values are sorted in the ascending order.
	//DataVector's X values are supposed to be always sorted, but there is no guarantee about Ys;
	std::pair<std::size_t, std::size_t> GetX_indices(double X_point) const; //[n_min, n_max] are used, not [n_min, n_max).N_used == n_max - n_min + 1 >= order + 1
	std::pair<std::size_t, std::size_t> GetY_indices(double Y_point) const; //[n_min, n_max] are used, not [n_min, n_max).N_used == n_max - n_min + 1 >= order + 1
protected:
	double polynomial_value(double x, double x0, const std::vector<double>& coefs) const;
};

#endif
