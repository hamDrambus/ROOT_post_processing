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
	Int_t _n_poInt_ts;
	Int_t _n_iterations;
	Int_t _order;
public:
	SavitzkyGolayFilter(Int_t n_poInt_ts = 10, Int_t order = 4, Int_t n_iterations = 1);
	void setNPoInt_ts(Int_t n);
	void setOrder(Int_t n);
	void setNIter(Int_t n);
	void setPars(Int_t n_poInt_ts = 10, Int_t order = 4, Int_t n_iterations = 1);

	Int_t getNPoInt_ts(void) const;
	Int_t getOrder(void) const;
	Int_t getNIter(void) const;
	void getPars(Int_t &n_poInt_ts, Int_t &order, Int_t &n_iterations) const;

	void operator ()(std::vector<Double_t> &xs_in, std::vector<Double_t> &ys_in,
		std::vector<Double_t> &xs_out, std::vector<Double_t> &ys_out) const;//TODO: make const vector references
};

#endif
