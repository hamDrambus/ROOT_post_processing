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
	fitter(fit_order), use_left(false), use_right(false)
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
}
void DataVector::unset_leftmost(void) {
	left_value = boost::none;
}
void DataVector::set_rightmost(double val) {
	right_value = val;
}
void DataVector::unset_rightmost(void) {
	right_value = boost::none;
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

void DataVector::read(std::ifstream& str) //DONE: add try/catch for handling stoi and stod
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
				if (line.size() < 2)
					throw std::runtime_error("Header line has wrong format (too small, does not start with \"//\")");
				if ((line[0] != '/') || (line[1] != '/'))
					throw std::runtime_error("Header line has wrong format (does not start with \"//\")");
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
				if (is_set_left)
					left_value = dval;
				else
					left_value = boost::none;
				word = strtoken(line, "\t ");
				++word_n;
				dval = boost::lexical_cast<double>(word);
				if (is_set_right)
					right_value = dval;
				else
					right_value = boost::none;
				continue;
			}
			catch (boost::bad_lexical_cast &e) {
				std::cerr << "DataVector::read: Error on line " << line_n << ". Can't convert word #" << word_n << " \"" << word << "\" to numerical value" << std::endl;
				std::cerr << e.what() << std::endl;
				std::cerr << "DataVector::read: bad header" << std::endl;
				use_rightmost(false);
				use_leftmost(false);
				unset_out_value();
				return;
			}
			catch (std::exception &e) {
				std::cerr << "DataVector::read: Unforeseen exception on line " << line_n << " word #" << word_n << ":" << std::endl;
				std::cerr << e.what() << std::endl;
				std::cerr << "DataVector::read: bad header" << std::endl;
				use_rightmost(false);
				use_leftmost(false);
				unset_out_value();
				return;
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
		<< "\t" << boost::lexical_cast<std::string>(left_value ? *left_value : 0)
		<< "\t" << boost::lexical_cast<std::string>(right_value ? *right_value : 0) << std::endl;
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
		if (left_value)
			return *left_value;
	}
	if (X_point > xys.back().first) {
		if (use_right)
			return xys.back().second;
		if (right_value)
			return *right_value;
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
