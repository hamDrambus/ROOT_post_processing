#include "PolynomialFit.h"

//Copy of PolynomialFit.cpp from ThreeDimSimulation project, but boost is hidden from ROOT
//because it won't be able to create dictionary for it.

namespace uBLAS = boost::numeric::ublas;

PolynomialFit::PolynomialFit(std::size_t order)
{
	setOrder(order);
}

PolynomialFit::~PolynomialFit()
{}

std::vector<double> PolynomialFit::operator ()(const std::vector<std::pair<double, double>> &vals_in, boost::optional<double> &in_x0) const {
	return (*this)(vals_in, 0, vals_in.size(), in_x0);
}

std::vector<double> PolynomialFit::operator ()(const std::vector<std::pair<double, double>> &vals,
	int offset, int N_points, boost::optional<double> &in_x0) const//only for a part of a vector
{
	std::vector<double> out;
	if ((vals.size() - offset) < N_points) {
		std::cout << "PolynomialFit::operator(): Error: N points is out of range:" << std::endl;
		std::cout << "\tx.size()=" << vals.size() << " offset=" << offset << " N_points=" << N_points << std::endl;
		return out;
	}
	if (offset < 0) {
		std::cout << "PolynomialFit::operator(): Error: offset is out of range:" << std::endl;
		std::cout << "\tx.size()=" << vals.size() << " offset=" << offset << " N_points=" << N_points << std::endl;
		return out;
	}
	if (N_points < (_order + 1)) {
		std::cout << "PolynomialFit::operator(): Error: no enough N points for fit:" << std::endl;
		std::cout << "\torder=" << _order << " N_points=" << N_points << std::endl;
		return out;
	}
	in_x0 = (in_x0 ? *in_x0 : vals[offset].first); //It is bad to set x0 to some fixed value (e.g. 0) because
	//interpolating too far from it will result in unstable results due to limited precision.
	//Ideally x0 should be set to the point at which we interpolate the data.
	if (1 == _order) {
		out.resize(2);
		out[1] = (vals[offset + 1].second - vals[offset].second) / (vals[offset + 1].first - vals[offset].first);
		out[0] = vals[offset].second + (*in_x0 - vals[offset].first)*out[1];
		//^value at in_x0 point
	} else {
		uBLAS::matrix<double> mat(N_points, _order + 1);
		for (int col = 0, col_end_ = mat.size2(); col < col_end_; ++col)
			for (int row = 0, row_end_ = mat.size1(); row < row_end_; ++row)
				mat(row, col) = pow(vals[offset + row].first - *in_x0, col);
		uBLAS::vector<double> Y(N_points);
		for (int row = 0, row_end_ = Y.size(); row < row_end_; ++row)
			Y[row] = vals[offset + row].second;
		//Solve the equation mat^T*mat*X = mat^T*Y for X via LU decomposition (mat is generally not diagonal)
		Y = uBLAS::prod(uBLAS::trans(mat), Y);
		mat = uBLAS::prod(uBLAS::trans(mat), mat);
		int res = uBLAS::lu_factorize(mat);
		if (res != 0)
			return out;
		uBLAS::inplace_solve(mat, Y, uBLAS::unit_lower_tag());
		uBLAS::inplace_solve(mat, Y, uBLAS::upper_tag());
		out.resize(Y.size());
		std::copy(Y.begin(), Y.end(), out.begin());
	}
	if (out.size() != (_order + 1)) {
		out.resize(0);
		return out;
	}
	return out;
}

//in_x0 - relative to what point carry out fit. Automatic value is set if boost::none is passed.
std::vector<double> PolynomialFit::operator ()(const std::vector<double> &xs, const std::vector<double> &ys, boost::optional<double> &in_x0) const
{
	return (*this)(xs, ys, 0, std::min(xs.size(), ys.size()), in_x0);
}
//Fit only part of a vector. offset+N_points-1 must be in the range of the vector
std::vector<double> PolynomialFit::operator ()(const std::vector<double> &xs, const std::vector<double> &ys, int offset, int N_points, boost::optional<double> &in_x0) const
{
	std::vector<double> out;
	if (xs.size() != ys.size()) {
		std::cout << "PolynomialFit::operator(): Error: x-y size mismatch:" << std::endl;
		std::cout << "\txs.size()=" << xs.size() << "\tys.size()=" << ys.size() << std::endl;
		return out;
	}
	if ((xs.size() - offset) < N_points) {
		std::cout << "PolynomialFit::operator(): Error: N points is out of range:" << std::endl;
		std::cout << "\tx.size()=" << xs.size() << " offset=" << offset << " N_points=" << N_points << std::endl;
		return out;
	}
	if (offset < 0) {
		std::cout << "PolynomialFit::operator(): Error: offset is out of range:" << std::endl;
		std::cout << "\tx.size()=" << xs.size() << " offset=" << offset << " N_points=" << N_points << std::endl;
		return out;
	}
	if (N_points < (_order + 1)) {
		std::cout << "PolynomialFit::operator(): Error: no enough N points for fit:" << std::endl;
		std::cout << "\torder=" << _order << " N_points=" << N_points << std::endl;
		return out;
	}
	in_x0 = (in_x0 ? *in_x0 : xs[offset]); //It is bad to set x0 to some fixed value (e.g. 0) because
	//interpolating too far from it will result in unstable results due to limited precision.
	//Ideally x0 should be set to the point at which we interpolate the data.
	if (1 == _order) {
		out.resize(2);
		out[1] = (ys[offset + 1] - ys[offset]) / (xs[offset + 1] - xs[offset]);
		out[0] = ys[offset] + (*in_x0 - xs[offset])*out[1];
		//^value at in_x0 point
	} else {
		uBLAS::matrix<double> mat(N_points, _order + 1);
		for (int col = 0, col_end_ = mat.size2(); col < col_end_; ++col)
			for (int row = 0, row_end_ = mat.size1(); row < row_end_; ++row)
				mat(row, col) = pow(xs[offset + row] - *in_x0, col);
		uBLAS::vector<double> Y(N_points);
		for (int row = 0, row_end_ = Y.size(); row < row_end_; ++row)
			Y[row] = ys[offset + row];
		//Solve the equation mat^T*mat*X = mat^T*Y for X via LU decomposition (mat is generally not diagonal)
		Y = uBLAS::prod(uBLAS::trans(mat), Y);
		mat = uBLAS::prod(uBLAS::trans(mat), mat);
		int res = uBLAS::lu_factorize(mat);
		if (res != 0)
			return out;
		uBLAS::inplace_solve(mat, Y, uBLAS::unit_lower_tag());
		uBLAS::inplace_solve(mat, Y, uBLAS::upper_tag());
		out.resize(Y.size());
		std::copy(Y.begin(), Y.end(), out.begin());
	}
	if (out.size() != (_order + 1)) {
		out.resize(0);
		return out;
	}
	return out;
}

std::vector<double> PolynomialFit::operator ()(const std::vector<std::pair<double, double>> &vals_in) const
{
	boost::optional<double> tmp(boost::none);
	return (*this)(vals_in, tmp);
}
//Fit only part of a vector. offset+N_points-1 must in the range of the vector
std::vector<double> PolynomialFit::operator ()(const std::vector<std::pair<double, double>> &vals_in, int offset, int N_points) const
{
	boost::optional<double> tmp(boost::none);
	return (*this)(vals_in, offset, N_points, tmp);
}
//in_x0 - relative to what point carry out fit
std::vector<double> PolynomialFit::operator ()(const std::vector<std::pair<double, double>> &vals_in, double in_x0) const
{
	boost::optional<double> tmp(in_x0);
	return (*this)(vals_in, tmp);
}

std::vector<double> PolynomialFit::operator ()(const std::vector<std::pair<double, double>> &vals_in, int offset, int N_points, double in_x0) const
{
	boost::optional<double> tmp(in_x0);
	return (*this)(vals_in, offset, N_points, tmp);
}

std::vector<double> PolynomialFit::operator ()(const std::vector<double> &xs, const std::vector<double> &ys) const
{
	boost::optional<double> tmp(boost::none);
	return (*this)(xs, ys);
}
//Fit only part of a vector. offset+N_points-1 must be in the range of the vector
std::vector<double> PolynomialFit::operator ()(const std::vector<double> &xs, const std::vector<double> &ys, int offset, int N_points) const
{
	boost::optional<double> tmp(boost::none);
	return (*this)(xs, ys, offset, N_points);
}
//in_x0 - relative to what point carry out fit
std::vector<double> PolynomialFit::operator ()(const std::vector<double> &xs, const std::vector<double> &ys, double in_x0) const
{
	boost::optional<double> tmp(in_x0);
	return (*this)(xs, ys, tmp);
}

std::vector<double> PolynomialFit::operator ()(const std::vector<double> &xs, const std::vector<double> &ys, int offset, int N_points, double in_x0) const
{
	boost::optional<double> tmp(in_x0);
	return (*this)(xs, ys, offset, N_points, tmp);
}

//=========================================================

DataVector::DataVector(std::size_t fit_order, std::size_t N_used_) :
	fitter(fit_order), use_left(false), use_right(false), is_left_value(false), is_right_value(false)
{
	setNused(N_used_);
}

DataVector::DataVector(std::vector<double> &xx, std::vector<double> &yy, std::size_t fit_order, std::size_t N_used_) : DataVector(fit_order, N_used_)
{
	initialize(xx, yy, fit_order, N_used_);
}

DataVector::~DataVector() {}

void DataVector::initialize(std::vector<double> &xx, std::vector<double> &yy, std::size_t fit_order, std::size_t N_used_)
{
	setNused(N_used_);
	fitter.setOrder(fit_order);
	if (xx.size() != yy.size()) {
		std::cout << "DataVector::initialize(): Error: x and y data size mismatch!" << std::endl;
		return;
	}
	std::size_t i_end_ = xx.size();
	xys.resize(i_end_);
	for (std::size_t i = 0; i != i_end_; ++i)
		xys[i] = std::pair<double, double>(xx[i], yy[i]);
	std::sort(xys.begin(), xys.end(), [](const std::pair<double, double> &a, const std::pair<double, double> &b)->bool {
		return a.first < b.first;
	});
}

void DataVector::set_leftmost(double val) {
	left_value = val;
	is_left_value = true;
}
void DataVector::unset_leftmost(void) {
	is_left_value = false;
}
void DataVector::set_rightmost(double val) {
	right_value = val;
	is_right_value = true;
}
void DataVector::unset_rightmost(void) {
	is_right_value = false;
}

void DataVector::insert(double x, double y) //do not disrupt order
{
	std::size_t sz = xys.size();
	if (0 == sz) {
		xys.push_back(std::pair<double, double>(x, y));
		return;
	}
	if (x < xys.front().first) {
		xys.insert(xys.begin(), std::pair<double, double>(x, y));
		return;
	}
	if (x > xys.back().first) {
		xys.push_back(std::pair<double, double>(x, y));
		return;
	}
	boost::optional<std::pair<std::size_t, std::size_t>> inds = getX_indices(x);
	if (inds->first == inds->second) //do not insert points with equal x, replace only
		xys[inds->first].second = y;
	else
		xys.insert(xys.begin() + inds->second, std::pair<double, double>(x, y));
}

void DataVector::push_back(double x, double y)//faster version not checking that the new array is ordered.
{
	xys.push_back(std::pair<double, double>(x, y));
}

void DataVector::read(std::ifstream& str, bool must_have_header) //DONE: add try/catch for handling stoi and stod
{
	clear();
	std::string line, word;
	int line_n = 0;
	while (!str.eof() && str.is_open()) {
		std::getline(str, line);
		++line_n;
		if (1 == line_n) {
			//parse "//Order	N_used	use_left use_right is_set_left is_set_right left_value right_value"
			int word_n = 0;
			try {
				double dval;
				std::size_t ival;
				bool is_set_right;
				bool is_set_left;
				if (line.size() < 2 && must_have_header)
					throw std::runtime_error("Header line has wrong format (too small, does not start with \"//\")");
				if (((line[0] != '/') || (line[1] != '/')) && must_have_header)
					throw std::runtime_error("Header line has wrong format (does not start with \"//\")");
				if (!must_have_header && (line.size() < 2 || line[0] != '/' || line[1] != '/')) { //not comment and we don't have to have header
					try {
						word = strtoken(line, "\t ");
						double x = boost::lexical_cast<double>(word);
						word = strtoken(line, "\t ");
						double val = boost::lexical_cast<double>(word);
						insert(x, val);
					}
					catch (boost::bad_lexical_cast &e) {
						continue;
					}
					catch (std::exception &e) {
						std::cerr << "DataVector::read: Unforeseen exception on line " << line_n << std::endl;
						std::cerr << e.what() << std::endl;
						return;
					}
					continue;
				}
				line.erase(line.begin(), line.begin() + 2);
				word = strtoken(line, "\t ");
				++word_n;
				ival = boost::lexical_cast<std::size_t>(word);
				setOrder(ival);

				word = strtoken(line, "\t ");
				++word_n;
				ival = boost::lexical_cast<std::size_t>(word);
				setNused(ival);

				word = strtoken(line, "\t ");
				++word_n;
				ival = std::stoi(word);
				use_leftmost(ival);

				word = strtoken(line, "\t ");
				++word_n;
				ival = std::stoi(word);
				use_rightmost(ival);

				word = strtoken(line, "\t ");
				++word_n;
				ival = std::stoi(word);
				is_set_left = ival;

				word = strtoken(line, "\t ");
				++word_n;
				ival = std::stoi(word);
				is_set_right = ival;

				word = strtoken(line, "\t ");
				++word_n;
				dval = boost::lexical_cast<double>(word);
				if (is_set_left) {
					left_value = dval;
					is_left_value = true;
				}
				else
					is_left_value = false;
				word = strtoken(line, "\t ");
				++word_n;
				dval = boost::lexical_cast<double>(word);
				if (is_set_right) {
					right_value = dval;
					is_right_value = true;
				}
				else
					is_right_value = false;
				continue;
			}
			catch (boost::bad_lexical_cast &e) {
				if (must_have_header) {
					std::cerr << "DataVector::read: Error on line " << line_n << ". Can't convert word #" << word_n << " \"" << word << "\" to numerical value" << std::endl;
					std::cerr << e.what() << std::endl;
					std::cerr << "DataVector::read: bad header" << std::endl;
					use_rightmost(false);
					use_leftmost(false);
					unset_out_value();
					return;
				}
			}
			catch (std::exception &e) {
				if (must_have_header) {
					std::cerr << "DataVector::read: Unforeseen exception on line " << line_n << " word #" << word_n << ":" << std::endl;
					std::cerr << e.what() << std::endl;
					std::cerr << "DataVector::read: bad header" << std::endl;
					use_rightmost(false);
					use_leftmost(false);
					unset_out_value();
					return;
				}
			}
		}
		if (line.size() >= 2) //Ignore simple c style comment
			if ((line[0] == '/') && (line[1] == '/'))
				continue;
		try {
			word = strtoken(line, "\t ");
			double x = boost::lexical_cast<double>(word);
			word = strtoken(line, "\t ");
			double val = boost::lexical_cast<double>(word);
			insert(x, val);
		}
		catch (boost::bad_lexical_cast &e) {
			continue;
		}
		catch (std::exception &e) {
			std::cerr << "DataVector::read: Unforeseen exception on line " << line_n << std::endl;
			std::cerr << e.what() << std::endl;
			return;
		}
	}
}

void DataVector::write(std::string fname, std::string comment) const
{
	std::ofstream str;
	open_output_file(fname, str, std::ios_base::trunc);
	write(str, comment);
	str.close();
}

void DataVector::write(std::ofstream& str, std::string comment) const
{
	//"//Order	N_used	use_left use_right is_set_left is_set_right left_value right_value"
	str << "//" << boost::lexical_cast<std::string>(getOrder())
		<< "\t" << boost::lexical_cast<std::string>(N_used)
		<< "\t" << (use_left ? 1 : 0) << "\t" << (use_right ? 1 : 0)
		<< "\t" << (left_value ? 1 : 0) << "\t" << (right_value ? 1 : 0)
		<< "\t" << boost::lexical_cast<std::string>(is_left_value ? left_value : 0)
		<< "\t" << boost::lexical_cast<std::string>(is_right_value ? right_value : 0) << std::endl;
	if (!comment.empty())
		str << "//" << comment << std::endl;
	for (std::size_t i = 0, i_end_ = xys.size(); i != i_end_; ++i) {
		str << boost::lexical_cast<std::string>(xys[i].first) << "\t"
			<< boost::lexical_cast<std::string>(xys[i].second) << std::endl;
	}
}

double DataVector::operator()(double X_point, boost::optional<double> x0) const
{
	if (!isValid())
		return DBL_MAX;
	if (X_point < xys.front().first) {
		if (use_left)
			return xys.front().second; //TODO: maybe add scaling
		if (is_left_value)
			return left_value;
	}
	if (X_point > xys.back().first) {
		if (use_right)
			return xys.back().second;
		if (is_right_value)
			return right_value;
	}
	boost::optional<std::pair<std::size_t, std::size_t>> indices = getX_indices(X_point);
	if (boost::none == indices)
		return DBL_MAX;
	//expand indices to [n_min, n_max] are used, not [n_min, n_max). N_used == n_max - n_min + 1 >= order + 1
	std::size_t span = (N_used - 1) / 2;  //asymmetrical interpolation range in the case of odd order.
	if (indices->first < span) { //first is too low
		indices = std::pair<std::size_t, std::size_t>(0, N_used - 1);
	} else {
		indices->first = indices->first - span;
		indices->second = indices->first + (N_used - 1);
		std::size_t sz = xys.size();
		if (indices->second >= sz) {
			indices = std::pair<std::size_t, std::size_t>(sz - N_used, sz - 1);
		}
	}
	std::vector<double> coefs = fitter(xys, indices->first, indices->second - indices->first + 1, x0); //i_max-i_min+1==N_used
	if (0 != coefs.size())
		return polynomial_value(X_point, *x0, coefs);
	return DBL_MAX;
}

double DataVector::operator()(double X_point, double x0) const //x0 = point is recommended to use. At least x0 must be close to point, or there will be large errors otherwise
{
	boost::optional<double> temp(x0);
	return (*this)(X_point, temp);
}

double DataVector::operator()(double X_point) const //corresponds to x0=boost::none
{
	boost::optional<double> temp(boost::none);
	return (*this)(X_point, temp);
}

boost::optional<std::pair<std::size_t, std::size_t>> DataVector::getX_indices(double x) const
{
	boost::optional<std::pair<std::size_t, std::size_t>> out;
	std::size_t sz = xys.size();
	if (0 == sz)
		return out;
	if (x <= xys.front().first) {
		out = std::pair<std::size_t, std::size_t>(0, 0);
		return out;
	}
	if (x >= xys.back().first) {
		out = std::pair<std::size_t, std::size_t>(sz - 1, sz - 1);
		return out;
	}
	//find first x which is not less that X_point. That is index bounding X_point: xs[first] <= X_point < xs[first + 1]
	//See std::lower_bound and std::upper_bound
	std::size_t count = sz;
	std::size_t first = 0;
	//std::lower_bound(xys.begin(), xys.end(), [](const std::pair<double, double> &a, const std::pair<double, double> &b)->bool{
	//	return a.first<b.first;
	//});
	while (count > 0) {
		std::size_t step = count / 2;
		std::size_t ind = first + step;
		if (!(x < xys[ind].first)) {
			first = ++ind;
			count -= step + 1;
		} else
			count = step;
	}
	//first is such, that x>=xs[first-1] and x<xs[first]
	//first always != 0 here
	--first;
	if (x == xys[first].first) {
		out = std::pair<std::size_t, std::size_t>(first, first);
		return out;
	}
	out = std::pair<std::size_t, std::size_t>(first, first + 1);
	return out;
}
//I chose to copy getX_indices code here instead of using parameter or lambda value picker function in the fear that it will reduce the performance. I did not test that it would.
boost::optional<std::pair<std::size_t, std::size_t>> DataVector::getY_indices(double y) const
{
	boost::optional<std::pair<std::size_t, std::size_t>> out;
	std::size_t sz = xys.size();
	if (0 == sz)
		return out;
	if (y <= xys.front().second) {
		out = std::pair<std::size_t, std::size_t>(0, 0);
		return out;
	}
	if (y >= xys.back().second) {
		out = std::pair<std::size_t, std::size_t>(sz - 1, sz - 1);
		return out;
	}
	//find first x which is not less that X_point. That is index bounding X_point: xs[first] <= X_point < xs[first + 1]
	//See std::lower_bound and std::upper_bound
	std::size_t count = sz;
	std::size_t first = 0;
	while (count > 0) {
		std::size_t step = count / 2;
		std::size_t ind = first + step;
		if (!(y < xys[ind].second)) {
			first = ++ind;
			count -= step + 1;
		} else
			count = step;
	}
	//first is such, that y>=ys[first-1] and y<ys[first]
	//first always != 0 here
	--first;
	if (y == xys[first].second) {
		out = std::pair<std::size_t, std::size_t>(first, first);
		return out;
	}
	out = std::pair<std::size_t, std::size_t>(first, first + 1);
	return out;
}

std::pair<std::size_t, std::size_t> DataVector::GetX_indices(double X_point) const
{
	boost::optional<std::pair<std::size_t, std::size_t> > res = getX_indices(X_point);
	if (boost::none == res)
		res = std::pair<std::size_t, std::size_t> (std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max());
	return *res;
}

std::pair<std::size_t, std::size_t> DataVector::GetY_indices(double Y_point) const
{
	boost::optional<std::pair<std::size_t, std::size_t> > res = getY_indices(Y_point);
	if (boost::none == res)
		res = std::pair<std::size_t, std::size_t>(std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max());
	return *res;
}

double DataVector::polynomial_value(double x, double x0, const std::vector<double>& coefs) const
{
	std::size_t order = coefs.size();
	double out_ = 0;
	for (std::size_t o_O = 0; o_O < order; ++o_O)
		out_ += coefs[o_O] * pow(x - x0, o_O);
	return out_;
}



void Transformation2D::calculate_matrix(void)
{
	if (!isValid())
		return;

	double in_u0 = uvs[0].first, in_v0 = uvs[0].second;
	double in_x0 = xys[0].first, in_y0 = xys[0].second;//It is bad to set x0 to some fixed value (e.g. 0) because
	//interpolating too far from it will result in unstable results due to limited precision.
	//Ideally x0 should be set to the point at which we interpolate the data.
	std::size_t N_points = xys.size();

	//Least Square Fit - solving
	//(1 x1 y1 x1^2 x1*y1 y1^2) (c0)   (u1, u2, u3, ... uN)
	//(1 x2 y2 x2^2 x2*y2 y2^2) (c1) =
	//( ...                   ) (..)
	//(1 xN yN xN^2 xN*yN yN^2) (c5)
	//for c_i. For (x, y)->u, (x, y)->v, (u, v)->x & (u, v)->y
	uBLAS::matrix<double> mat_xy(N_points, 6);
	for (int row = 0, row_end_ = mat_xy.size1(); row < row_end_; ++row) {
		mat_xy(row, 0) = 1; //c0
		mat_xy(row, 1) = xys[row].first - in_x0; //c1*x
		mat_xy(row, 2) = xys[row].second - in_y0; //c2*y
		mat_xy(row, 3) = pow(xys[row].first - in_x0, 2); //c3*x*x
		mat_xy(row, 4) = (xys[row].first - in_x0) * (xys[row].second - in_y0); //c4*x*y
		mat_xy(row, 5) = pow(xys[row].second - in_y0, 2); //c5*y*y
	}
	uBLAS::matrix<double> mat_uv(N_points, 6);
	for (int row = 0, row_end_ = mat_uv.size1(); row < row_end_; ++row) {
		mat_uv(row, 0) = 1; //c0
		mat_uv(row, 1) = uvs[row].first - in_u0; //c1*u
		mat_uv(row, 2) = uvs[row].second - in_v0; //c2*v
		mat_uv(row, 3) = pow(uvs[row].first - in_u0, 2); //c3*u*u
		mat_uv(row, 4) = (uvs[row].first - in_u0) * (uvs[row].second - in_v0); //c4*u*v
		mat_uv(row, 5) = pow(uvs[row].second - in_v0, 2); //c5*v*v
	}
	uBLAS::vector<double> U(N_points), V(N_points), X(N_points), Y(N_points);
	for (int row = 0; row < N_points; ++row) {
		U[row] = uvs[row].first;
		V[row] = uvs[row].second;
		X[row] = xys[row].first;
		Y[row] = xys[row].second;
	}
	//Solve the equation mat^T*mat*C = mat^T*{U, V, X, Y} for C via LU decomposition (mat is generally not diagonal)
	X = uBLAS::prod(uBLAS::trans(mat_uv), X);
	Y = uBLAS::prod(uBLAS::trans(mat_uv), Y);
	U = uBLAS::prod(uBLAS::trans(mat_xy), U);
	V = uBLAS::prod(uBLAS::trans(mat_xy), V);
	mat_uv = uBLAS::prod(uBLAS::trans(mat_uv), mat_uv);
	mat_xy = uBLAS::prod(uBLAS::trans(mat_xy), mat_xy);
	int res_xy = uBLAS::lu_factorize(mat_xy);
	if (res_xy != 0) {
		u_coefficients.clear();
		v_coefficients.clear();
	} else {
		uBLAS::inplace_solve(mat_xy, U, uBLAS::unit_lower_tag());
		uBLAS::inplace_solve(mat_xy, U, uBLAS::upper_tag());
		u_coefficients.resize(U.size());
		std::copy(U.begin(), U.end(), u_coefficients.begin());
		uBLAS::inplace_solve(mat_xy, V, uBLAS::unit_lower_tag());
		uBLAS::inplace_solve(mat_xy, V, uBLAS::upper_tag());
		v_coefficients.resize(V.size());
		std::copy(V.begin(), V.end(), v_coefficients.begin());
	}
	int res_uv = uBLAS::lu_factorize(mat_uv);
	if (res_uv != 0) {
		x_coefficients.clear();
		y_coefficients.clear();
	} else {
		uBLAS::inplace_solve(mat_uv, X, uBLAS::unit_lower_tag());
		uBLAS::inplace_solve(mat_uv, X, uBLAS::upper_tag());
		x_coefficients.resize(X.size());
		std::copy(X.begin(), X.end(), x_coefficients.begin());
		uBLAS::inplace_solve(mat_uv, Y, uBLAS::unit_lower_tag());
		uBLAS::inplace_solve(mat_uv, Y, uBLAS::upper_tag());
		y_coefficients.resize(Y.size());
		std::copy(Y.begin(), Y.end(), y_coefficients.begin());
	}
}

Transformation2D::Transformation2D()
{}

Transformation2D::Transformation2D(std::vector<double> &xx, std::vector<double> &yy, std::vector<double> &uu, std::vector<double> &vv)
{
	initialize(xx, yy, uu, vv);
}

Transformation2D::~Transformation2D()
{}

void Transformation2D::initialize(std::vector<double> &xx, std::vector<double> &yy, std::vector<double> &uu, std::vector<double> &vv)
{
	if (xx.size()!=yy.size()) {
		std::cout<<"Transformation2D::initialize: Error: x-y data size mismatch!"<<std::endl;
	} else {
		std::size_t i_end_ = xx.size();
		xys.resize(i_end_);
		for (std::size_t i = 0; i != i_end_; ++i)
			xys[i] = std::pair<double, double>(xx[i], yy[i]);
	}
	if (uu.size()!=vv.size()) {
		std::cout<<"Transformation2D::initialize: Error: u-v data size mismatch!"<<std::endl;
	} else {
		std::size_t i_end_ = uu.size();
		uvs.resize(i_end_);
		for (std::size_t i = 0; i != i_end_; ++i)
			uvs[i] = std::pair<double, double>(uu[i], vv[i]);
	}
	if (xys.size() != uvs.size()) {
		std::cout<<"Transformation2D::initialize: Warning: xy-uv data size mismatch!"<<std::endl;
	}
	calculate_matrix();
}

//Checks for duplicates
void Transformation2D::insert(double x, double y, double u, double v)
{
	std::size_t sz_xy = xys.size(), sz_uv = uvs.size();
	std::size_t duplicate_xy = sz_xy, duplicate_uv = sz_uv;
	for (std::size_t i = 0; i!=sz_xy; ++i) {
		if (xys[i].first == x && xys[i].second == y) {
			duplicate_xy = i;
			break;
		}
	}
	for (std::size_t i = 0; i!=sz_uv; ++i) {
		if (uvs[i].first == u && uvs[i].second == v) {
			duplicate_uv = i;
			break;
		}
	}
	if (duplicate_xy != sz_xy && duplicate_uv != sz_uv) {
		if (duplicate_xy != duplicate_uv)
			std::cout<<"Transformation2D::insert: Warning: same u-v corresponds to 2 different x-y points!"<<std::endl;
		else
			return;
	}
	if (duplicate_xy != sz_xy) {
		if (duplicate_xy >= sz_uv) { //There is no (u, v) corresponding to existing (x, y), so we push one to the uvs.
			std::pair<double, double> temp = xys[sz_uv];
			xys[sz_uv] = xys[duplicate_xy];
			xys[duplicate_xy] = temp;
			uvs.push_back(std::pair<double, double>(u, v));
		} else {//Update(u, v) corresponding to existing (x, y).
			uvs[duplicate_xy] = std::pair<double, double>(u, v);
		}
		calculate_matrix();
		return;
	}
	if (duplicate_uv != sz_uv) {
		if (duplicate_uv >= sz_xy) { //There is no (x, y) corresponding to existing (u, v), so we push one to the xys.
			std::pair<double, double> temp = uvs[sz_xy];
			uvs[sz_xy] = uvs[duplicate_uv];
			uvs[duplicate_uv] = temp;
			xys.push_back(std::pair<double, double>(x, y));
		} else {//Update(x, y) corresponding to existing (u, v).
			xys[duplicate_uv] = std::pair<double, double>(x, y);
		}
		calculate_matrix();
		return;
	}
	xys.push_back(std::pair<double, double>(x, y));
	uvs.push_back(std::pair<double, double>(u, v));
	calculate_matrix();
}

void Transformation2D::push_back(double x, double y, double u, double v)
{
	xys.push_back(std::pair<double, double>(x, y));
	uvs.push_back(std::pair<double, double>(u, v));
	calculate_matrix();
}

//returns DBL_MAX if invalid
double Transformation2D::evalX(double u, double v) const
{
	if (!isValid() || x_coefficients.size() < 6)
		return DBL_MAX;
	double in_u0 = uvs[0].first, in_v0 = uvs[0].second;
	u -= in_u0;
	v -= in_v0;
	double out = x_coefficients[0] + x_coefficients[1] * u + x_coefficients[2] * v +
			x_coefficients[3] * u * u + x_coefficients[4] * u * v + x_coefficients[5] * v * v;
	return out;
}
//returns DBL_MAX if invalid
double Transformation2D::evalY(double u, double v) const
{
	if (!isValid() || y_coefficients.size() < 6)
		return DBL_MAX;
	double in_u0 = uvs[0].first, in_v0 = uvs[0].second;
	u -= in_u0;
	v -= in_v0;
	double out = y_coefficients[0] + y_coefficients[1] * u + y_coefficients[2] * v +
			y_coefficients[3] * u * u + y_coefficients[4] * u * v + y_coefficients[5] * v * v;
	return out;
}
//returns DBL_MAX if invalid
double Transformation2D::evalU(double x, double y) const
{
	if (!isValid() || u_coefficients.size() < 6)
		return DBL_MAX;
	double in_x0 = xys[0].first, in_y0 = xys[0].second;
	x -= in_x0;
	y -= in_y0;
	double out = u_coefficients[0] + u_coefficients[1] * x + u_coefficients[2] * y +
			u_coefficients[3] * x * x + u_coefficients[4] * x * y + u_coefficients[5] * y * y;
	return out;
}
//returns DBL_MAX if invalid
double Transformation2D::evalV(double x, double y) const
{
	if (!isValid() || v_coefficients.size() < 6)
		return DBL_MAX;
	double in_x0 = xys[0].first, in_y0 = xys[0].second;
	x -= in_x0;
	y -= in_y0;
	double out = v_coefficients[0] + v_coefficients[1] * x + v_coefficients[2] * y +
			v_coefficients[3] * x * x + v_coefficients[4] * x * y + v_coefficients[5] * y * y;
	return out;
}

