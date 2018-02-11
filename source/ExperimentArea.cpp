#include "ExperimentArea.h"

namespace ParameterPile {

	area_vector::area_vector(void) : _is_valid(false), _last_returned_index(-1)
	{
		_last_returned_index_left = _vec.end();
	}
	int area_vector::get_order_index_by_index(int ind)
	{
		if (!_is_valid)
			return -1;
		bool even = true;
		int l = -1, r = -1, N = 0;
		for (auto i = _vec.begin(); i != _vec.end(); i++, even = !even){
			if (even)
				l = *i;
			else {
				r = *i;
				if (!(ind<l) && !(ind>r))
					return  N + (ind - l);
				N += r - l + 1;
			}
		}
		return -1;
	}
	int area_vector::get_index_by_order_index(int ind)
	{
		if (!_is_valid)
			return -1;
		bool even = true;
		int l = -1, r = -1, N = 0;
		for (auto i = _vec.begin(); i != _vec.end(); i++, even = !even){
			if (even)
				l = *i;
			else {
				r = *i;
				N += r - l + 1;
				if (N >= ind)
					return r - N + ind + 1;
			}
		}
		return -1;
	}

	int area_vector::get_next_index(void) //for running through all indices
	{
		if (!_is_valid)
			return -1;
		if ((_last_returned_index < 0) || _last_returned_index_left == _vec.end()){
			_last_returned_index_left = _vec.begin();
			return _last_returned_index = *_last_returned_index_left;
		}
		STD_CONT<int>::iterator right = _last_returned_index_left + 1;
		_last_returned_index++;
		if (_last_returned_index>*right){
			_last_returned_index_left = right+1;
			if (_last_returned_index_left == _vec.end())
				return _last_returned_index = -1;
			return _last_returned_index = *_last_returned_index_left;
		}
		return _last_returned_index;
	}

	int area_vector::get_next_index(int after)
	{
		if (!_is_valid)
			return -1;
		if (-1 == after)
			return *_vec.begin();
		bool even = true;
		int L = -1, R = -1;
		for (auto sub = _vec.begin(); sub != _vec.end(); (sub == _vec.end() ? sub : sub++), even = !even) {
			if (even)
				L = *sub;
			else {
				R = *sub;
				if (!(after < L) && (after < R))
					return after + 1;
				if (after == R) {
					if (++sub != _vec.end()){ //go to the next area, but check it is valid
						int out = *sub;
						if ((sub++) != _vec.end())
							return out;
					}
				}
			}
		}
		return -1;
	}

	void area_vector::push_pair(int left, int right)
	{
		if (_vec.empty())
			_is_valid = true;
		if (left > right) {
			_vec.push_back(right);
			_vec.push_back(left);
		} else {
			_vec.push_back(left);
			_vec.push_back(right);
		}
		reset();
	}

	void area_vector::push_back(int val)
	{
		if (!_vec.empty())
			_is_valid = !_is_valid;
		_vec.push_back(val);
		reset();
	}

	bool area_vector::contains(int index)
	{
		return !(get_order_index_by_index(index) < 0);
	}

	bool area_vector::empty(void)
	{
		return _vec.empty();
	}

	int &area_vector::back(void)
	{	return _vec.back();}
	int &area_vector::front(void)
	{	return *_vec.begin();}

	STD_CONT<area_vector> area_vector::split_area(int N)
	{
		STD_CONT <area_vector> out_;
		for (int h = 0; h < N; h++){
			out_.push_back(area_vector());
		}
		int N_runs = 0;
		bool even = true;
		int l, r;
		for (auto h = _vec.begin(); h != _vec.end(); ++h, even = !even){
			if (even){
				l = *h;
			} else {
				r = *h;
				N_runs += r - l + 1;
			}
		}
		int N_accum = 0;
		int curr_index = 0;
		even = true;
		double delta = N_runs / (double)N;
		for (auto h = _vec.begin(); h != _vec.end(); ++h, even = !even) {
			if (even) {
				l = *h;
			} else {
				r = *h;
				N_accum += r - l + 1;
				int new_l = l, new_r = r;
				while (N_accum > (int)((curr_index + 1)*delta)) {
					if ((N_accum - (int)((curr_index + 1)*delta)) > (int)(delta))
						new_r = new_l + (int)(delta)-1;
					else
						new_r = N_accum - (int)((curr_index + 1)*delta) + new_l - 1;
					out_[curr_index].push_pair(new_l, new_r);
					new_l = new_r + 1;
					curr_index++;
				}
				new_r = r;
				if (N_accum <= (int)((curr_index + 1)*delta) && (new_l <= new_r))
					out_[curr_index].push_pair(new_l, new_r);
			}
		}
		return out_;
	}
	area_vector area_vector::intersect(area_vector with)
	{
		area_vector out;
		area_vector temp = *this;
		bool even_out = true;
		int left_out, right_out;
		bool finished = false;
		for (auto g = temp._vec.begin(); g != temp._vec.end(); ++g, even_out = !even_out){
			if (even_out){
				left_out = *g;
			} else {
				right_out = *g;
				bool even = true;
				int left, right;
				bool has_this_area = false;
				double temp_left_out, temp_right_out;
				for (auto h = with._vec.begin(); h != with._vec.end(); ++h, even = !even){
					if (even){
						if (has_this_area)
							out.push_pair(temp_left_out, temp_right_out);
						temp_left_out = left_out;
						temp_right_out = right_out;
						has_this_area = false;
						left = *h;
					} else {
						right = *h;
						if ((right_out > right) && (left_out <= right)){
							temp_right_out = right;
							has_this_area = true;
						}
						if ((left_out < left) && (right_out >= left)){
							temp_left_out = left;
							has_this_area = true;
						}
						if ((left <= left_out) && (right >= right_out)){
							has_this_area = true;
						}
					}
				}
				if (has_this_area) {
					out.push_pair(temp_left_out, temp_right_out);
				}
			}
		}
		return out;
	}
	void area_vector::reset() //clears _last_returned_index etc.
	{
		_last_returned_index = -1;
		_last_returned_index_left = _vec.end();
	}
	void area_vector::erase() //clears vector
	{
#ifdef _HOTFIX_CLEAR_MEMORY
		STD_CONT<int>().swap(_vec);
#else
		_vec.clear();
#endif
		reset();
	}
	bool area_vector::isValid(void)
	{
		return _is_valid;
	}
	//void area_vector::refine (void); //[2,3][3,4] to [2,4]|OR| [4,5] [1,7] to 
	experiment_area::experiment_area(Type type) : _type(type)
	{}
	experiment_area experiment_area::to_point(void){
		experiment_area out(Type::Point);
		if (!experiments.empty())
			out.experiments.push_back(experiments.back());
		if (!runs.empty())
			out.runs.push_back(runs.back());
		if (!sub_runs.empty())
			out.sub_runs.push_back(sub_runs.back());
		if (!channels.empty())
			out.channels.push_back(channels.back());
		return out;
	}
	bool experiment_area::isValid(void)
	{
		return (_type == Type::Point) ?
			(!experiments.empty()) && (!runs.empty()) && (!sub_runs.empty()) && (!channels.empty()) :
			(!experiments.empty()) && (runs.isValid()) && (sub_runs.isValid()) && (channels.isValid());
	}
	bool experiment_area::contains(ParameterPile::experiment_area what)
	{
		if ((what._type != Type::Point) || Type::Area != _type)
			return false; //not implemented intersection of two areas
		if (!isValid())
			return false;
		for (auto exp = experiments.begin(); exp != experiments.end(); exp++) {
			if ((*exp) == what.experiments.back()) {
				if (what.runs.empty())
					return true;
				else {
					if (runs.contains(what.runs.back())){
						if (what.channels.empty() && what.sub_runs.empty())
							return true;
						if (!what.channels.empty() && !what.sub_runs.empty())
							if (channels.contains(what.channels.back()) && sub_runs.contains(what.sub_runs.back()))
								return true;
					}
				}
			}
		}
		return false;
	}

}
