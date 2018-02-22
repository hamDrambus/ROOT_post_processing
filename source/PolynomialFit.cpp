#include "PolynomialFit.h"

PolynomialFit::PolynomialFit(Int_t order)
{
	setOrder(order);
}

void PolynomialFit::setOrder(Int_t n)
{
	if (n < 0)
		_order = 2;
	else
		_order = n;
}

Int_t PolynomialFit::getOrder(void) const
{
	return _order;
}

void PolynomialFit::getCoefs(TVectorD &pars) const
{
	pars.ResizeTo(_last_coefs);
	pars = _last_coefs;
}

void PolynomialFit::operator ()(const std::vector<Double_t> &xs_in, const std::vector<Double_t> &ys_in,
	TVectorD &pars_out, Double_t in_x0){
	return (*this)(xs_in, ys_in, 0, xs_in.size(), pars_out, in_x0);
}

void PolynomialFit::operator ()(const std::vector<Double_t> &xs_in, const std::vector<Double_t> &ys_in,
	Int_t offset, Int_t N_poInt_ts, TVectorD &pars_out, Double_t in_x0) //only for a part of a vector
{
	if (xs_in.size() != ys_in.size())
		return;
	if ((xs_in.size()-offset) < N_poInt_ts)
		return;
	if (N_poInt_ts < (_order + 1))
		return;

	TMatrixD mat(N_poInt_ts, _order + 1);
	for (Int_t col = 0; col < mat.GetNcols(); col++)
		for (Int_t row = 0; row < mat.GetNrows(); row++)
			mat[row][col] = pow(xs_in[offset+ row] - in_x0, col);
	TVectorD Y(N_poInt_ts);
	for (Int_t row = 0; row < Y.GetNrows(); row++)
		Y[row] = ys_in[offset + row];
	TMatrixD mT = mat;
	mT.T();
	_last_coefs.ResizeTo(_order + 1);
	_last_coefs = ((mT*mat).Invert())*mT*Y;
	pars_out.ResizeTo(_last_coefs);
	pars_out = _last_coefs;
}
