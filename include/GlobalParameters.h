#ifndef _GLOBAL_PARAMETERS_H
#define _GLOBAL_PARAMETERS_H

#include "GlobalDefinitions.h"
#include "ExperimentArea.h"
#include "PolynomialFit.h"

#define ROOT_BL_CALL_V0 find_background_v_0(f_ys, ys.size(), 80,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE,2);
#define ROOT_BL_CALL_V2 find_background_v_0(f_ys, ys.size(), 60,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE,2);
#define ROOT_BL_CALL_V3 find_background_v_0(f_ys, ys.size(), 70,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE,2);
#define ROOT_BL_CALL_V4 find_background_v_raw(f_ys, ys.size(), 60,	TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder2, kTRUE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V5 find_background_v_raw(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V6 find_background_v_raw(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V7 find_background_v_raw(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);
#define ROOT_BL_CALL_V8 find_background_v_raw(f_ys, ys.size(), 20,	TSpectrum::kBackIncreasingWindow, TSpectrum::kBackOrder2, kFALSE, TSpectrum::kBackSmoothing3, kFALSE);


class viewRegion { //helper class for drawing cuts (clipping lines to pad axes)
protected:
	std::vector<double> view_xs;
	std::vector<double> view_ys;
	std::vector<double> line_xs;
	std::vector<double> line_ys;
public:
	viewRegion(double x_min, double y_min, double x_max, double y_max); //constructs rectangular view region
	viewRegion(std::vector<double> &xs, std::vector<double> &ys); //constructs rectangular from polyline defined by xs,ys
	viewRegion(); //no view region
	void view_push(double x, double y);
	bool get_view(std::size_t index, double &x, double &y) const;
	bool set_view(std::size_t index, double x, double y);
	std::size_t get_view_size(void) const;
	void ClipToView(const std::vector<double>& px, const std::vector<double>& py, std::vector<double>& cx, std::vector<double>& cy) const;
	static bool IsInPolygon(double x, double y, const std::vector<double>& px, const std::vector<double>& py, bool& edge);
	//0 - not crossed, 1 - crossed, 2 - crossed at ends
	static int LinesCrossed(double x1, double y1, double x2, double y2,
		double u1, double v1, double u2, double v2, double& xc, double& yc);
	static bool OnLine(double x1, double y1, double x2, double y2, double u, double v);
	static bool IsInf(double val);
	static bool InRange(double val, double a, double b, double tolerance);
	static bool RangesIntersect(double x1, double x2, double x3, double x4, double tolerance, double &p);
	static bool IsValidLine(double x1, double y1, double x2, double y2);

	void polyline_push(double x, double y);
	bool get_polyline(std::size_t ind, double &x, double &y) const;
	bool set_polyline(std::size_t ind, double x, double y);
	void set_polyline(const std::vector<double> &x, const std::vector<double> &y);
	std::size_t get_polyline_size(void) const;
	void clear_polyline(void);

	TPolyLine* get_clipped_polyline(void) const;
};

class AnalysisManager;
class AllExperimentsResults;
class PostProcessor;
class GraphicOutputManager;

extern GraphicOutputManager *gr_manager;
extern AnalysisManager *manager;
extern AllExperimentsResults* g_data;
extern PostProcessor* post_processor;

//namespace ParameterPile
//{
	enum DrawEngine { Gnuplot, ROOT_ };
	enum TriggerVersion { trigger_v1, trigger_v2, trigger_v3};
	enum NamingScheme { name_scheme_v1, name_scheme_v2}; //v1 - before 2019.12.06 refactoring of Data_processing, v2 - after

	double gasE_from_kV (double kV, double gasAr_layer); //gasAr_layer is in [cm], returns E in gaseous Ar in [V/cm]
	double Td_from_E (double E); //E is in [V/cm]
	double E_from_Td(double Td); //E is in [V/cm]
	double Vdr_from_E (double E); //E is in [V/cm], returns drift velocity in gaseous Ar in [cm/s]
	double Vdr_from_kV (double kV, double gasAr_layer); //LAr_layer is in [cm], returns drift velocity in gaseous Ar in [cm/s]
	double drift_time_from_kV(double kV, double gasAr_layer); //gasAr_layer is in [cm], returns drift time in gaseous Ar in [microseconds]
	Bool_t draw_required(/*ParameterPile::*/experiment_area what);

	extern std::deque <experiment_area> areas_to_draw;
	extern std::string this_path;
	
	extern std::string Vdrift_data_fname;
	extern DataVector Vdrift; //e drift speed in gaseous Ar as a function of Td (in m/s)
	extern const double bolzmann_SI; //SI
	extern const double Td_is_Vcm2; //1 Townsend = 1e-17 V*cm^2
	extern const double LAr_epsilon; //doi: 10.1016/j.nima.2019.162431
	extern double full_gap_length; //cm, the distance between THGEM0 and THGEM1
	extern double R3; //MOhm, THGEM0 resistance. 4 MOhm in experiments before ~ Feb 2019.
	extern double Rgap; //MOhm, resistance defining E field in EL gap
	extern double Rrest; //MOhm
	extern const double T; //temperature in K
	extern const double P; //pressure in Pa

	extern std::string data_prefix_path;
	extern std::string calibration_file;
	extern std::string data_output_path;

	extern std::string DATA_MPPC_VERSION;
	extern std::string DATA_PMT_VERSION;

	extern experiment_area exp_area;
	extern int threads_number;

	extern int gnuplot_pad_size;
	extern int gnuplot_max_size;
	extern int gnuplot_width;

	extern TriggerVersion trigger_version;
	extern NamingScheme name_scheme_version;
	extern std::map < std::string, double > experiment_fields;
	extern std::map < std::string, double > PMT_V;
	extern std::map < std::string, double > MPPC_V;
	extern std::map < std::string, channel_info<dB_info> > dBs;
	extern std::pair<int, int> calibaration_points;
	extern std::map < int, std::pair<double,double> > MPPC_coords;

	void Init_globals(bool full);
//};
void DrawFileData(std::string name, std::vector<double> xs, std::vector<double> ys, /*ParameterPile::*/DrawEngine de = /*ParameterPile::*/ ROOT_);

#endif
