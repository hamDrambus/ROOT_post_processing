#ifndef GRAPH_GROUPT_H
#define GRAPH_GROUPT_H

#include "GlobalParameters.h"

class GnuplotDrawing //Always in single pad. Multiple pad can't be interactive which is the whole purpose of using gnuplot instead of ROOT
{
	//!!! If used in multithread code, make sure there is no _name conflict between threads!!! (normally the case since they process different runs(events))
protected:
	std::string _name; //unique, serves as ID and GnuplotDrawing can be saved as '_name'.png.
	//All datafiles are saved as '_name'1,2,... .dat and gnuplot script is in '_name'.sc
	std::string _dir_to_save; //directory (ending in /) to which .png will be saved. Do not save if "".
	std::string _storage_dir; //directory (ending in /) to which temporary data files and .sc will be saved
	
	class Function {
	public:
		std::string _data_filename; //may be "" which means no data is plotted, but extra script is used for lines
		bool y_errors;
		bool plotting_function;
		std::string _title;
		std::string _extra_pars;
		std::string _extra_prefix_script; //For additional definitions before plot/replot
		std::string _extra_suffix_script;
		std::pair<double, double> x_lims, y_lims; //Must calculate and store them in order to later set all GnuplotDrawing in GraphCollection to the same scale
		Function(void) : y_errors(false), plotting_function(false),
			x_lims(std::pair<double, double>(-DBL_MAX, DBL_MAX)), y_lims(std::pair<double, double>(-DBL_MAX, DBL_MAX)) {}
	};

	std::deque<Function> _funcs;
	std::pair<double, double> _x_range, _y_range; //automatic ones are -DBL_MAX, DBL_MAX
	std::string _x_title, _y_title; //axes

	std::string process_title(std::string title) const; //Makes gnuplot print title as it is passed into GnuplotDrawing (escape special symbol \)
	bool generate_script(std::string script_name) const;
public:
	//Drawing(void);
	GnuplotDrawing(std::string name);
	GnuplotDrawing(std::string name, std::string gnuplot_dir, std::string png_dir);
	void AddToDraw(DVECTOR &xs, DVECTOR &ys, std::string title = "", std::string extra_txt = "");
	void AddToDraw(std::deque<DVECTOR> &xs, std::deque<DVECTOR> &ys, std::string title = "", std::string extra_txt = "");//Disjointed data
	void AddToDraw(DVECTOR &xs, DVECTOR &ys, DVECTOR &ys_err, std::string title = "", std::string extra_txt = "");
	void AddToDraw_baseline(double base_line, std::string title = "baseline", std::string extra_txt = "");//May add more functions to draw, e.g. gauss
	void AddToDraw_vertical(double x_pos, double from_y, double to_y, std::string extra_txt = "");
	void AddToDraw(std::string definition_lines, std::string f_name, std::string title, std::string extra_txt = "");//any gnuplot function

	void Draw(void) const;
	void Clear(void); //Does not change _name, _dir_to_save and _storage_dir

	void SetPngDirectory(std::string path);
	void SetGnuplotDirectory(std::string path); //Does not move already existing files
	void SetName(std::string name);

	void SetXrange(double from, double to);
	void SetYrange(double from, double to);
	void UnsetXrange(double from, double to); //gnuplot uses automatic one
	void UnsetYrange(double from, double to); //gnuplot uses automatic one
	void SetXtitle(std::string title);
	void SetYtitle(std::string title);

	std::string GetName(void) const;
	friend class GraphCollection;
};

class GraphCollection
{
protected:
	std::string _dir_to_save; //directory to which .png will be saved
	std::string _storage_dir; //directory to which temporary data files and .sc will be saved
	std::pair<double, double> _x_range, _y_range; //automatic ones are -DBL_MAX, DBL_MAX
public:
	std::deque<GnuplotDrawing> _graphs;
	GraphCollection(void);

	void SetPngDirectory(std::string path); //For all GnuplotDrawing, including future ones
	void SetGnuplotDirectory(std::string path); //For all GnuplotDrawing, including future ones. Does not move already existing files
	void SetXrange(double from, double to);
	void SetYrange(double from, double to);
	void UnsetXrange(double from, double to); //gnuplot uses automatic one
	void UnsetYrange(double from, double to); //gnuplot uses automatic one
	std::pair<double, double> get_x_limits(void) const;
	std::pair<double, double> get_y_limits(void) const;
	
	GnuplotDrawing* GetDrawing(std::size_t index);	//if does not exist returns NULL
	GnuplotDrawing* GetDrawing(std::string name);	//if does not exist, creates it
	GnuplotDrawing* CreateDrawing(std::string name); //returns existing GnuplotDrawing if its name is the same
	void Draw(void) const;
	void Clear(void);

	void Merge(const GraphCollection & with);

	std::size_t size(void) const;
};

#endif
