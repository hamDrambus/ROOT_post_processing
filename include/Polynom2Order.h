#ifndef  POLYNOM_2_ORDER_H
#define  POLYNOM_2_ORDER_H

#include "PolynomialFit.h"

class Polynom2Order : public PolynomialFit
{
protected:
	std::vector<Double_t> *_xs_last;
	std::vector<Double_t> *_ys_last; //thees are supposed to stay valid poInt_ters and vectors before 'this' destruction
	//moreover vector is supposed to stay the same between Fit(...) calls
	std::vector<Double_t>::iterator _x_start;
	std::vector<Double_t>::iterator _x_finish;
	Double_t _x_left, _x_right;
	Double_t _x0_in; //the poInt_t in which x is set to be 0, this is for convinience and in order to avoid too large coefficients

	std::vector<Double_t>::iterator find_x_iterator_by_value(Double_t x);
public:
	Polynom2Order();
	virtual void operator ()(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in, TVectorD &pars_out, Double_t in_x0 = 0);//TODO: make const vector references
	//^in_x0 - in what poInt_t set zero x (In the SG filter it is convinient to set x_in
	//to the poInt_t in which value is calculated
	virtual void operator ()(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in, Int_t offset, Int_t N_poInt_ts, TVectorD &pars_out, Double_t in_x0 = 0); //TODO: make const vector references
	//only for a part of a vector
	virtual void setOrder(Int_t n);

	void FindMaximum(std::vector<Double_t>::iterator &x_max, Double_t &x_max_exact, Double_t &y_max_exact);
	void FindMinimum(std::vector<Double_t>::iterator &x_min, Double_t &x_min_exact, Double_t &y_min_exact);
	void FindInt_tersection(std::vector<Double_t>::iterator &x_Int_ter, std::vector<Double_t>::iterator &x_Int_ter2, Double_t &x_Int_ter_exact, Double_t &x_Int_ter_exact2,
		Double_t threshold); //if not found, returns x_iter=xs.end();
	void FindExtremum(std::vector<Double_t>::iterator &x_extr, Double_t &x_extr_exact, Double_t &y_extr_exact);
	Double_t Value(Double_t X);
	Double_t Derivative(Double_t X);
};

#endif
