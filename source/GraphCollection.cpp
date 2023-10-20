#include "GraphCollection.h"


GnuplotDrawing::GnuplotDrawing(std::string name) : _name(name), _storage_dir("temp_gnuplot_files/"),
	_dir_to_save(""), _x_range(-DBL_MAX, DBL_MAX), _y_range(-DBL_MAX, DBL_MAX)
{}

GnuplotDrawing::GnuplotDrawing(std::string name, std::string gnuplot_dir, std::string png_dir) :
	_name(name), _storage_dir(gnuplot_dir), _dir_to_save(png_dir),
	_x_range(-DBL_MAX, DBL_MAX), _y_range(-DBL_MAX, DBL_MAX)
{}

std::string GnuplotDrawing::process_title(std::string title) const //Makes gnuplot print title as it is passed into GnuplotDrawing (escape special symbol _)
{
	std::string out;
	char prev_ch = 'a';
	for (std::size_t i = 0, i_end_ = title.size(); i != i_end_; ++i) {
		if ((title[i] == '_') && (prev_ch != '\\')) {
			out.push_back('\\');
			out.push_back(title[i]);
			prev_ch = title[i];
		} else {
			out.push_back(title[i]);
			prev_ch = title[i];
		}
	}
	return out;
}

bool GnuplotDrawing::generate_script(std::string script_name) const
{
	std::ofstream str;
	open_output_file(script_name, str, std::ios_base::trunc);
	if (!str.is_open())
		return false;
	str << "clear" << std::endl;
#if defined(__WIN32__)
	str << "set terminal wxt size "<<std::to_string(gnuplot_width)<< ","
		<< std::to_string(std::min(gnuplot_max_size, gnuplot_pad_size))<<std::endl;
#else
	str << "set terminal qt size " << std::to_string(gnuplot_width) << ","
		<< std::to_string(std::min(gnuplot_max_size, gnuplot_pad_size)) << std::endl;
#endif
	str << "set ytics nomirror" << std::endl;
	str << "set y2tics" << std::endl;
	str << "set key top right" << std::endl;

	str << "set autoscale xy" << std::endl;
	str << "set xrange [ " << (_x_range.first == -DBL_MAX ? "" : std::to_string(_x_range.first)) <<": "
		<< (_x_range.second == DBL_MAX ? "" : std::to_string(_x_range.second)) << " ]" << std::endl;
	str << "set yrange [ " << (_y_range.first == -DBL_MAX ? "" : std::to_string(_y_range.first)) << ": "
		<< (_y_range.second == DBL_MAX ? "" : std::to_string(_y_range.second)) << " ]" << std::endl;
	str << "set xlabel \"" << _x_title << "\"" << std::endl;
	str << "set ylabel \"" << _y_title << "\"" << std::endl;
	str << std::endl;

	bool first_plot = true;
	for (std::size_t i = 0, i_end_ = _funcs.size(); i != i_end_; ++i) {
		if (!_funcs[i]._extra_prefix_script.empty())
			str << _funcs[i]._extra_prefix_script << std::endl;
		if (!_funcs[i]._data_filename.empty()) {
			if (_funcs[i].y_errors) {
				str << (first_plot ? "plot " : "replot ") << _funcs[i]._data_filename << " u 1:2:3 with errorbars title '" << _funcs[i]._title << "' " << _funcs[i]._extra_pars << std::endl;
			} else {
				str << (first_plot ? "plot " : "replot ") << _funcs[i]._data_filename << (_funcs[i].plotting_function ? " title '" : " u 1:2 title '")
					<< _funcs[i]._title << "' " << _funcs[i]._extra_pars << std::endl;
			}
			first_plot = false;
		}
		if (!_funcs[i]._extra_suffix_script.empty())
			str << _funcs[i]._extra_suffix_script << std::endl;
	}

	str << std::endl;
	str << "set grid" << std::endl;
	str << "replot" << std::endl;
	str << "refresh" << std::endl;

	if (!_dir_to_save.empty()) {
		str << "set terminal png size " << std::to_string(gnuplot_width) << ","
			<< std::to_string(std::min(gnuplot_max_size, gnuplot_pad_size)) << std::endl;
		str << "set output \"" << this_path + _dir_to_save + _name + ".png" << "\"" << std::endl;
		str << "set grid" << std::endl;
		str << "replot" << std::endl;
#if defined(__WIN32__)
		str << "set terminal wxt size " << std::to_string(gnuplot_width) << ","
			<< std::to_string(std::min(gnuplot_max_size, gnuplot_pad_size)) << std::endl;
#else
		str << "set terminal qt size " << std::to_string(gnuplot_width) << ","
			<< std::to_string(std::min(gnuplot_max_size, gnuplot_pad_size)) << std::endl;
#endif
		str << "refresh" << std::endl;
	}
#if !defined(__WIN32__)
	if (_dir_to_save.empty())
		str << "pause -1" << std::endl;
#endif
	str.close();
	return true;
}

void GnuplotDrawing::AddToDraw(DVECTOR &xs, DVECTOR &ys, std::string title, std::string extra_txt)
{
	if (xs.size() != ys.size()) {
		std::cerr << "GnuplotDrawing::AddToDraw: Error! x-y size mismatch for plot '" << _name << "': '" << title << "'" << std::endl;
		return;
	}
	title = process_title(title);
	Function ff;
	ff._data_filename = this_path + _storage_dir + _name + "_" + std::to_string(_funcs.size());

	std::ofstream f_data;
	open_output_file(ff._data_filename, f_data, std::ios_base::trunc);
	if (!f_data.is_open())
		return;

	ff._data_filename = "'" + ff._data_filename + "'";
	ff.x_lims = std::pair<double, double>(DBL_MAX, -DBL_MAX);
	ff.y_lims = std::pair<double, double>(DBL_MAX, -DBL_MAX);
	for (std::size_t i = 0, i_end_ = xs.size(); i != i_end_; ++i) {
		f_data << xs[i] << "\t" << ys[i] << std::endl;
		ff.x_lims.first = std::min(xs[i], ff.x_lims.first);
		ff.y_lims.first = std::min(ys[i], ff.y_lims.first);
		ff.x_lims.second = std::max(xs[i], ff.x_lims.second);
		ff.y_lims.second = std::max(ys[i], ff.y_lims.second);
	}
	if (ff.x_lims.first == DBL_MAX)
		ff.x_lims.first = -DBL_MAX;
	if (ff.x_lims.second == -DBL_MAX)
		ff.x_lims.second = DBL_MAX;
	if (ff.y_lims.first == DBL_MAX)
		ff.y_lims.first = -DBL_MAX;
	if (ff.y_lims.second == -DBL_MAX)
		ff.y_lims.second = DBL_MAX;
	ff._extra_pars = extra_txt;
	ff._title = title;
	_funcs.push_back(ff);
}

void GnuplotDrawing::AddToDraw(std::deque<DVECTOR> &xs, std::deque<DVECTOR> &ys, std::string title, std::string extra_txt)
{
	if (xs.size() != ys.size()) {
		std::cerr << "GnuplotDrawing::AddToDraw: Error! x-y size mismatch for plot '" << _name << "': '" << title << "'" << std::endl;
		return;
	}
	if (xs.size()==0) {
		return;
	}
	for (std::size_t a = 0, a_end_ = xs.size(); a!=a_end_; ++a)
		if (xs[a].size()!=ys[a].size()) {
			std::cerr << "GnuplotDrawing::AddToDraw: Error! x-y size mismatch for plot '" << _name << "': '" << title << "'" << std::endl;
			return;
		}
	title = process_title(title);
	Function ff;
	ff._data_filename = this_path + _storage_dir + _name + "_" + std::to_string(_funcs.size());

	std::ofstream f_data;
	open_output_file(ff._data_filename, f_data, std::ios_base::trunc);
	if (!f_data.is_open())
		return;

	ff._data_filename = "'" + ff._data_filename + "'";
	ff.x_lims = std::pair<double, double>(DBL_MAX, -DBL_MAX);
	ff.y_lims = std::pair<double, double>(DBL_MAX, -DBL_MAX);
	for (std::size_t a = 0, a_end_ = xs.size(); a != a_end_; ++a) {
		for (std::size_t i = 0, i_end_ = xs[a].size(); i != i_end_; ++i) {
			f_data << xs[a][i] << "\t" << ys[a][i] << std::endl;
			ff.x_lims.first = std::min(xs[a][i], ff.x_lims.first);
			ff.y_lims.first = std::min(ys[a][i], ff.y_lims.first);
			ff.x_lims.second = std::max(xs[a][i], ff.x_lims.second);
			ff.y_lims.second = std::max(ys[a][i], ff.y_lims.second);
		}
		f_data << std::endl;
	}
	if (ff.x_lims.first == DBL_MAX)
		ff.x_lims.first = -DBL_MAX;
	if (ff.x_lims.second == -DBL_MAX)
		ff.x_lims.second = DBL_MAX;
	if (ff.y_lims.first == DBL_MAX)
		ff.y_lims.first = -DBL_MAX;
	if (ff.y_lims.second == -DBL_MAX)
		ff.y_lims.second = DBL_MAX;
	ff._extra_pars = extra_txt;
	ff._title = title;
	_funcs.push_back(ff);
}

void GnuplotDrawing::AddToDraw(DVECTOR &xs, DVECTOR &ys, DVECTOR &ys_err, std::string title, std::string extra_txt)
{
	if ((xs.size() != ys.size()) || (xs.size() != ys_err.size())) {
		std::cerr << "GnuplotDrawing::AddToDraw: Error! x-y-y_error size mismatch for plot '" << _name << "': '" << title << "'" << std::endl;
		return;
	}
	title = process_title(title);
	Function ff;
	ff._data_filename = this_path + _storage_dir + _name + "_" + std::to_string(_funcs.size());

	std::ofstream f_data;
	open_output_file(ff._data_filename, f_data, std::ios_base::trunc);
	if (!f_data.is_open())
		return;

	ff._data_filename = "'" + ff._data_filename + "'";
	ff.x_lims = std::pair<double, double>(DBL_MAX, -DBL_MAX);
	ff.y_lims = std::pair<double, double>(DBL_MAX, -DBL_MAX);
	for (std::size_t i = 0, i_end_ = xs.size(); i != i_end_; ++i) {
		f_data << xs[i] << "\t" << ys[i] << "\t" << ys_err[i] << std::endl;
		ff.x_lims.first = std::min(xs[i], ff.x_lims.first);
		ff.y_lims.first = std::min(ys[i], ff.y_lims.first);
		ff.x_lims.second = std::max(xs[i], ff.x_lims.second);
		ff.y_lims.second = std::max(ys[i], ff.y_lims.second);
	}
	if (ff.x_lims.first == DBL_MAX)
		ff.x_lims.first = -DBL_MAX;
	if (ff.x_lims.second == -DBL_MAX)
		ff.x_lims.second = DBL_MAX;
	if (ff.y_lims.first == DBL_MAX)
		ff.y_lims.first = -DBL_MAX;
	if (ff.y_lims.second == -DBL_MAX)
		ff.y_lims.second = DBL_MAX;
	ff._extra_pars = extra_txt;
	ff._title = title;
	ff.y_errors = true;
	_funcs.push_back(ff);
}

void GnuplotDrawing::AddToDraw_baseline(double base_line, std::string title, std::string extra_txt)//May add more functions to draw, e.g. gauss
{
	std::string definition_lines = "var" + std::to_string(_funcs.size()) +"(x) = " + std::to_string(base_line);
	AddToDraw(definition_lines, "var" + std::to_string(_funcs.size()) +"(x)", title, extra_txt);
}

void GnuplotDrawing::AddToDraw_vertical(double x_pos, double from_y, double to_y, std::string extra_txt)
{
	Function ff;
	ff._extra_prefix_script = "set arrow from " + std::to_string(x_pos) + "," + (from_y == -DBL_MAX ? "graph 0" : std::to_string(from_y))
			+ " to " + std::to_string(x_pos) + "," + (to_y == DBL_MAX ? "graph 1" : std::to_string(to_y)) + " nohead " + extra_txt;
	ff._extra_suffix_script = "show arrow";
	_funcs.push_back(ff);
}

void GnuplotDrawing::AddToDraw(std::string definition_lines, std::string f_name, std::string title, std::string extra_txt)//any gnuplot function
{
	Function ff;
	title = process_title(title);
	ff._extra_prefix_script = definition_lines;
	ff._data_filename = f_name;
	ff._title = title;
	ff.plotting_function = true;
	ff._extra_pars = extra_txt;
	_funcs.push_back(ff);
}

void GnuplotDrawing::Draw(void) const
{
	std::string script_name = this_path + _storage_dir + _name + ".sc";
	if (generate_script(script_name))
		INVOKE_GNUPLOT(script_name);
}

void GnuplotDrawing::Clear(void) //Does not change _name, _dir_to_save and _storage_dir
{
	std::deque<Function>().swap(_funcs);
	_x_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	_y_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	_x_title = "";
	_y_title = "";
}

void GnuplotDrawing::SetPngDirectory(std::string path)
{
	_dir_to_save = path;
}

void GnuplotDrawing::SetGnuplotDirectory(std::string path) //Does not move already existing files
{
	_storage_dir = path;
}

void GnuplotDrawing::SetName(std::string name)
{
	_name = name;
}

void GnuplotDrawing::SetXrange(double from, double to)
{
	_x_range = std::pair<double, double>(std::min(from, to), std::max(from, to));
}

void GnuplotDrawing::SetYrange(double from, double to)
{
	_y_range = std::pair<double, double>(std::min(from, to), std::max(from, to));
}

void GnuplotDrawing::UnsetXrange(double from, double to) //gnuplot uses automatic one
{
	_x_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
}

void GnuplotDrawing::UnsetYrange(double from, double to) //gnuplot uses automatic one
{
	_y_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
}

void GnuplotDrawing::SetXtitle(std::string title)
{
	_x_title = title;
}

void GnuplotDrawing::SetYtitle(std::string title)
{
	_y_title = title;
}

std::string GnuplotDrawing::GetName(void) const
{
	return _name;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphCollection::GraphCollection(void) : _storage_dir("temp_gnuplot_files/"),
	_dir_to_save(""), _x_range(-DBL_MAX, DBL_MAX), _y_range(-DBL_MAX, DBL_MAX) {}

void GraphCollection::SetPngDirectory(std::string path) //For all GnuplotDrawing, including future ones
{
	_dir_to_save = path;
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		_graphs[i].SetPngDirectory(path);
	}
}

void GraphCollection::SetGnuplotDirectory(std::string path) //For all GnuplotDrawing, including future ones. Does not move already existing files
{
	_storage_dir = path;
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		_graphs[i].SetGnuplotDirectory(path);
	}
}

void GraphCollection::SetXrange(double from, double to)
{
	_x_range = std::pair<double, double>(std::min(from, to), std::max(from, to));
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		_graphs[i].SetXrange(from, to);
	}
}

void GraphCollection::SetYrange(double from, double to)
{
	_y_range = std::pair<double, double>(std::min(from, to), std::max(from, to));
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		_graphs[i].SetYrange(from, to);
	}
}

void GraphCollection::UnsetXrange(double from, double to) //gnuplot uses automatic one
{
	_x_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		_graphs[i].UnsetXrange(from, to);
	}
}

void GraphCollection::UnsetYrange(double from, double to) //gnuplot uses automatic one
{
	_y_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		_graphs[i].UnsetYrange(from, to);
	}
}

std::pair<double, double> GraphCollection::get_x_limits(void) const
{
	std::pair<double, double> out(DBL_MAX, -DBL_MAX);
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		for (std::size_t f = 0, f_end_ = _graphs[i]._funcs.size(); f != f_end_; ++f) {
			if (_graphs[i]._funcs[f].x_lims.first !=-DBL_MAX)
				out.first = std::min(out.first, _graphs[i]._funcs[f].x_lims.first);
			if (_graphs[i]._funcs[f].x_lims.second !=DBL_MAX)
				out.second = std::max(out.second, _graphs[i]._funcs[f].x_lims.second);
		}
	}
	if (out.first == DBL_MAX)
		out.first = -DBL_MAX;
	if (out.second == -DBL_MAX)
		out.second = DBL_MAX;
	return out;
}

std::pair<double, double> GraphCollection::get_y_limits(void) const
{
	std::pair<double, double> out(DBL_MAX, -DBL_MAX);
	for (std::size_t i = 0, i_end_ = _graphs.size(); i != i_end_; ++i) {
		for (std::size_t f = 0, f_end_ = _graphs[i]._funcs.size(); f != f_end_; ++f) {
			if (_graphs[i]._funcs[f].y_lims.first !=-DBL_MAX)
				out.first = std::min(out.first, _graphs[i]._funcs[f].y_lims.first);
			if (_graphs[i]._funcs[f].y_lims.second !=DBL_MAX)
				out.second = std::max(out.second, _graphs[i]._funcs[f].y_lims.second);
		}
	}
	if (out.first == DBL_MAX)
		out.first = -DBL_MAX;
	if (out.second == -DBL_MAX)
		out.second = DBL_MAX;
	return out;
}

GnuplotDrawing* GraphCollection::GetDrawing(std::size_t index)	//if does not exist returns NULL
{
	if (index >= _graphs.size())
		return NULL;
	return &_graphs[index];
}

GnuplotDrawing* GraphCollection::GetDrawing(std::string name)	//if does not exist, creates it
{
	for (auto i = _graphs.begin(), i_end_ = _graphs.end(); i != i_end_; ++i)
		if (i->GetName() == name)
			return &(*i);
	return CreateDrawing(name);
}

GnuplotDrawing* GraphCollection::CreateDrawing(std::string name) //returns existing GnuplotDrawing if its name is the same
{
	for (auto i = _graphs.begin(), i_end_ = _graphs.end(); i != i_end_; ++i)
		if (i->GetName() == name)
			return &(*i);
	if (_graphs.size()>gnuplot_max_pics_number) {
		return NULL;
	}
	_graphs.push_back(GnuplotDrawing(name, _storage_dir, _dir_to_save));
	_graphs.back().SetXrange(_x_range.first, _x_range.second);
	_graphs.back().SetYrange(_y_range.first, _y_range.second);
	return &_graphs.back();
}

void GraphCollection::Draw(void) const
{
	for (auto i = _graphs.begin(), i_end_ = _graphs.end(); i != i_end_; ++i)
		i->Draw();
}

void GraphCollection::Clear(void)
{
	_x_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	_y_range = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	std::deque<GnuplotDrawing>().swap(_graphs);
}

std::size_t GraphCollection::size(void) const
{
	return _graphs.size();
}

void GraphCollection::Merge(const GraphCollection & with)
{
	for (auto i = _graphs.begin(), i_end_ = _graphs.end(); i!=i_end_; ++i)
		for (auto w = with._graphs.begin(), w_end_ = with._graphs.end(); w != w_end_; ++w)
			if (i->GetName() == w->GetName()) {
				std::cerr << "GraphCollection::Merge: Error: Name collision for \"" << i->GetName() << "\" GnuplotDrawing. Not merging" << std::endl;
				return;
			}
	if (_dir_to_save != with._dir_to_save)
		std::cerr << "GraphCollection::Merge: Warning: png output directory mismatch" << std::endl;
	if (_storage_dir != with._storage_dir)
		std::cerr << "GraphCollection::Merge: Warning: gnuplot files' directory mismatch" << std::endl;
	_x_range.first = std::min(_x_range.first, with._x_range.first);
	_x_range.second = std::max(_x_range.second, with._x_range.second);
	_y_range.first = std::min(_y_range.first, with._y_range.first);
	_y_range.second = std::max(_y_range.second, with._y_range.second);
	_graphs.insert(_graphs.end(), with._graphs.begin(), with._graphs.end());
	SetXrange(_x_range.first, _x_range.second);
	SetYrange(_y_range.first, _y_range.second);
}
