#ifndef POLYNOMIAL_FIT_H
#define POLYNOMIAL_FIT_H

#include "GlobalParameters.h"
#include "TMatrixD.h"
#include "TVectorD.h"

//parameters are [0]+[1]*x+[2]*x^2+...
class PolynomialFit {
protected:
	Int_t _order;
	TVectorD _last_coefs;
public:
	PolynomialFit(Int_t order);
	virtual void setOrder(Int_t n); //TODO: actualluy is is a bad practice to call virtual method from the constructor
	//but is is ok here, since derivative class only limits setOrder() possible values to {2}

	Int_t getOrder(void) const;
	void getCoefs(TVectorD &pars) const;

	virtual void operator ()(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in,
		TVectorD &pars_out, Double_t in_x0=0); //in_x0 - in what poInt_t set zero x (In the SG filter it is convinient to set x_in
	//to the poInt_t in which value is calculated
	virtual void operator ()(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in,
		Int_t offset, Int_t N_poInt_ts, TVectorD &pars_out, Double_t in_x0=0); //only for a part of a vector
};

#endif
