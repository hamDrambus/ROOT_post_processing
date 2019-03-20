#include <stdio.h>
#if defined(__WIN32__)
#include <direct.h>
#endif

#include "GlobalParameters.h"


//TODO: some functions must be moved to PostProcessor class. (then I won't need the std::vector<double>* get_data methods)

peak::peak() : right(-1), left(-1), S(-1), A(-1)
#ifdef PEAK_AVR_TIME
,t(-1)
#endif
{}

GraphicOutputManager *gr_manager = NULL;
AnalysisManager *manager = NULL;
AllExperimentsResults* g_data = NULL;
PostProcessor* post_processor = NULL;

std::vector<double>::iterator iter_add(std::vector<double>::iterator& to, int what, std::vector<double>::iterator& end)
{
	if (what < 0)
		return end;
	return ((int)(end - to) < what) ? end : to + what;
}


void open_output_file(std::string name, std::ofstream &str, std::ios_base::openmode _mode)
{
	std::string folder = name;
	while ((folder.back() != '\\') &&(folder.back()!='/') &&!folder.empty())
		folder.pop_back();
	if (!folder.empty())
		folder.pop_back();
#if defined(__WIN32__)
	if (!folder.empty()){
		DWORD ftyp = GetFileAttributesA(folder.c_str());
		if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY) || ftyp == INVALID_FILE_ATTRIBUTES){
				int code = system(("mkdir \"" + folder + "\"").c_str());
				if (code)
					std::cout << "mkdir error: " << GetLastError() << std::endl;
			}
	}
#else
	struct stat st;
	stat(folder.c_str(),&st);
	if(!S_ISDIR(st.st_mode)){
		int code = system(("mkdir -p \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir -p error: " << code << std::endl;
	}
#endif //__WIN32__
	str.open(name.c_str(), std::ios_base::trunc);
	if (!str.is_open()){
		std::cout << "Failed to open \"" << name << "\"" << std::endl;
	}
}

bool confirm_action (std::string action)
{
	std::cout<<"Confirm "<<action<<": y/n"<<std::endl;
	std::string a;
	std::cin>>a;
	if (a=="y"||a=="Y"||a=="yes"||a=="Yes") {
		return true;
	}
	std::cout<<action<<" denied"<<std::endl;
	return false;
}

void DrawFileData(std::string name, std::vector<double> xs, std::vector<double> ys,/* ParameterPile::*/DrawEngine de)
{
	if (xs.size() != ys.size()){
		std::cout << "DrawFileData::input data size mismatch" << std::endl;
		return;
	}
	if (de == /*ParameterPile::*/DrawEngine::ROOT_){
		double *xxxs = new double[xs.size()];
		double *yyys = new double[ys.size()];
		for (int h = 0; h < xs.size(); h++){
			xxxs[h] = xs.at(h);
			yyys[h] = ys.at(h);
		}
		TGraph* gr = new TGraph(xs.size(), xxxs, yyys);
		TCanvas* can = new TCanvas(name.c_str(), name.c_str());
		can->SetWindowPosition(100, 150);
		can->Update();
		gr->Draw();
		delete[] xxxs;
		delete[] yyys;
	} else {
		std::string mod_name = name;
		for (int s = 0; s < mod_name.size(); s++)
			if (mod_name[s] == '\\' || mod_name[s] == '/')
				mod_name[s] = '.';
		std::ofstream file;
		open_output_file("temp_gnuplot_files/" + mod_name, file);
		std::cout << "file " << "temp_gnuplot_files/" + mod_name << ".is_open() " << file.is_open() << std::endl;
#if defined(__WIN32__)
		if (!file.is_open())
			std::cout << GetLastError() << std::endl;
#endif //__WIN32__
		for (int h = 0; h < xs.size(); h++)
			file << xs[h] << '\t' << ys[h] << std::endl;
		file.close();
		open_output_file("temp_gnuplot_files/script.sc", file);
		file << "plot '" << /*ParameterPile::*/this_path + "/temp_gnuplot_files/" + mod_name << "' u 1:2" << std::endl;
		file << "pause -1";
		file.close();
		INVOKE_GNUPLOT(this_path + "/temp_gnuplot_files/script.sc");
	}
}

viewRegion::viewRegion(double x_min, double y_min, double x_max, double y_max) //constructs rectangular view region
{
	view_xs.push_back(x_min);
	view_ys.push_back(y_min);
	view_xs.push_back(x_min);
	view_ys.push_back(y_max);
	view_xs.push_back(x_max);
	view_ys.push_back(y_max);
	view_xs.push_back(x_max);
	view_ys.push_back(y_min);
}

viewRegion::viewRegion(std::vector<double> &xs, std::vector<double> &ys) //constructs rectangular from polyline defined by xs,ys
{
	if (xs.size() != ys.size()) {
		std::cout << "viewRegion::viewRegion: Warning x-y size mismatch, points are ignored";
		return;
	}
	view_xs = xs;
	view_ys = ys;
}

viewRegion::viewRegion() //no view region
{}

void viewRegion::view_push(double x, double y)
{
	view_xs.push_back(x);
	view_ys.push_back(y);
}

bool viewRegion::get_view(std::size_t index, double &x, double &y) const
{
	if (index >= view_xs.size())
		return false;
	x = view_xs[index];
	y = view_ys[index];
	return true;
}

bool viewRegion::set_view(std::size_t index, double x, double y)
{
	if (index >= view_xs.size()) {
		view_xs.resize(index+1, x);
		view_ys.resize(index+1, y);
		return true;
	}
	view_xs[index] = x;
	view_ys[index] = y;
	return true;
}

std::size_t viewRegion::get_view_size(void) const
{
	return view_xs.size();
}

void viewRegion::polyline_push(double x, double y)
{
	line_xs.push_back(x);
	line_ys.push_back(y);
}

bool viewRegion::get_polyline(std::size_t ind, double &x, double &y) const
{
	if (ind >= line_xs.size())
		return false;
	x = line_xs[ind];
	y = line_ys[ind];
	return true;
}

bool viewRegion::set_polyline(std::size_t ind, double x, double y)
{
	if (ind >= line_xs.size()) {
		line_xs.resize(ind + 1, x);
		line_ys.resize(ind + 1, y);
		return true;
	}
	line_xs[ind] = x;
	line_ys[ind] = y;
	return true;
}

void viewRegion::set_polyline(const std::vector<double> &x, const std::vector<double> &y)
{
	if (x.size() != y.size()) {
		std::cout << "viewRegion::set_polyline: Warning x-y size mismatch, points are ignored";
		return;
	}
	line_xs = x;
	line_ys = y;
}

std::size_t viewRegion::get_polyline_size(void) const
{
	return line_xs.size();
}

TPolyLine viewRegion::get_clipped_polyline(void) const
{
	std::vector<double> clipped_xs, clipped_ys;
	ClipToView(line_xs, line_ys, clipped_xs, clipped_ys);
	TPolyLine ret(clipped_xs.size(), &clipped_xs[0], &clipped_ys[0]);
	return ret;
}

void viewRegion::clear_polyline(void)
{
	line_xs.clear();
	line_ys.clear();
}

//0 - not crossed, 1 - crossed, 2 - crossed at ends
int viewRegion::LinesCrossed(double x1, double y1, double x2, double y2,
	double u1, double v1, double u2, double v2, double& xc, double& yc)
{
	// Set the tolerances.
	double xtol =
		1.0e-10 *
		std::max(std::abs(x1),
			std::max(std::abs(x2), std::max(std::abs(u1), std::abs(u2))));
	double ytol =
		1.0e-10 *
		std::max(std::abs(y1),
			std::max(std::abs(y2), std::max(std::abs(v1), std::abs(v2))));
	if (xtol <= 0) xtol = 1.0e-10;
	if (ytol <= 0) ytol = 1.0e-10;

	// Compute the distances and determinant (dx,dy) x (du,dv).
	double dy = y2 - y1;
	double dv = v2 - v1;
	double dx = x1 - x2;
	double du = u1 - u2;
	double det = dy * du - dx * dv;

	// Check for crossing because one of the endpoints is on both lines.
	if (OnLine(x1, y1, x2, y2, u1, v1)) {
		xc = u1;
		yc = v1;
		return 2;
	}
	else if (OnLine(x1, y1, x2, y2, u2, v2)) {
		xc = u2;
		yc = v2;
		return 2;
	}
	else if (OnLine(u1, v1, u2, v2, x1, y1)) {
		xc = x1;
		yc = y1;
		return 2;
	}
	else if (OnLine(u1, v1, u2, v2, x2, y2)) {
		xc = x2;
		yc = y2;
		return 2;
	}
	// Check if the lines are parallel (zero determinant).
	else if (std::abs(det) < xtol * ytol)
		return 0;
	// No special case: compute point of intersection.
	else {

		// Solve crossing equations.
		xc = (du * (x1 * y2 - x2 * y1) - dx * (u1 * v2 - u2 * v1)) / det;
		yc = ((-1 * dv) * (x1 * y2 - x2 * y1) + dy * (u1 * v2 - u2 * v1)) / det;

		// Determine if this point is on both lines.
		if (OnLine(x1, y1, x2, y2, xc, yc) && OnLine(u1, v1, u2, v2, xc, yc))
			return 1;
	}
	// The lines do not cross if we have reached this point.
	return 0;
}

// Ported from Garfield (method GRCONV):
// Clip the specified polygon to the view region; return the clipped polygon.
// px: the x-vertices of the polygon
// py: the y-vertices of the polygon
// cx: to contain the x-vertices of the clipped polygon
// cy: to contain the y-vertices of the clipped polygon
void viewRegion::ClipToView(const std::vector<double>& px, const std::vector<double>& py,
                            std::vector<double>& cx, std::vector<double>& cy) const {

  // Get the number and coordinates of the polygon vertices.
  int pN = (int)px.size();

  // Clear the vectors to contain the final polygon.
  cx.clear();
  cy.clear();

  // Set up the view vertices.
  std::vector<double> vx = view_xs, vy = view_ys;

  int vN = (int)vx.size();

  // Do nothing if we have less than 2 points.
  if (pN < 2) return;

  // Loop over the polygon vertices.
  for (int i = 0; i < pN; ++i) {

    // Flag for skipping check for edge intersection.
    bool skip = false;

    // Loop over the view vertices.
    for (int j = 0; j < vN; ++j) {

      // Determine whether this vertex lies on a view edge:
      //  if so add the vertex to the final polygon.
      if (OnLine(vx[j % vN], vy[j % vN], vx[(j + 1) % vN], vy[(j + 1) % vN],
                 px[i], py[i])) {

        // Add the vertex.
        cx.push_back(px[i]);
        cy.push_back(py[i]);

        // Skip edge intersection check in this case.
        skip = true;
      }

      // Determine whether a corner of the view area lies on this edge:
      //  if so add the corner to the final polygon.
      if (OnLine(px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN],
                 vx[j], vy[j])) {

        // Add the vertex.
        cx.push_back(vx[j]);
        cy.push_back(vy[j]);

        // Skip edge intersection check in this case.
        skip = true;
      }
    }

    // If we have not skipped the edge intersection check, look for an
    // intersection between this edge and the view edges.
    if (!skip) {

      // Loop over the view vertices.
      for (int j = 0; j < vN; ++j) {

        // Check for a crossing with this edge;
        //  if one exists, add the crossing point.
        double xc = 0., yc = 0.;
        if (LinesCrossed(vx[j % vN], vy[j % vN], vx[(j + 1) % vN],
                         vy[(j + 1) % vN], px[i % pN], py[i % pN],
                         px[(i + 1) % pN], py[(i + 1) % pN], xc, yc)) {

          // Add a vertex.
          cx.push_back(xc);
          cy.push_back(yc);
        }
      }
    }
  }

  // Find all view field vertices inside the polygon.
  for (int j = 0; j < vN; ++j) {

    // Test whether this vertex is inside the polygon.
    //  If so, add it to the final polygon.
    bool edge = false;
    if (IsInPolygon(vx[j], vy[j], px, py, edge)) {

      // Add the view vertex.
      cx.push_back(vx[j]);
      cy.push_back(vy[j]);
    }
  }

  // Find all polygon vertices inside the box.
  for (int i = 0; i < pN; ++i) {

    // Test whether this vertex is inside the view.
    //  If so, add it to the final polygon.
    bool edge = false;
    if (IsInPolygon(px[i], py[i], vx, vy, edge)) {

      // Add the polygon vertex.
      cx.push_back(px[i]);
      cy.push_back(py[i]);
    }
  }
}

// Ported from Garfield (function INTERD):
// Returns true if the point (x,y) is inside of the specified polygon.
// x: the x-coordinate
// y: the y-coordinate
// px: the x-vertices of the polygon
// py: the y-vertices of the polygon
// edge: a variable set to true if the point is located on the polygon edge
bool viewRegion::IsInPolygon(double x, double y, const std::vector<double>& px,
                             const std::vector<double>& py, bool& edge) {

  // Get the number and coordinates of the polygon vertices.
  int pN = (int)px.size();

  // Handle the special case of less than 2 vertices.
  if (pN < 2) return false;
  // Handle the special case of exactly 2 vertices (a line).
  if (pN == 2) return OnLine(px[0], py[0], px[1], py[1], x, y);

  // Set the minimum and maximum coordinates of all polygon vertices.
  double px_min = px[0], py_min = py[0];
  double px_max = px[0], py_max = py[0];
  for (int i = 0; i < pN; ++i) {
    px_min = std::min(px_min, px[i]);
    py_min = std::min(py_min, py[i]);
    px_max = std::max(px_max, px[i]);
    py_max = std::max(py_max, py[i]);
  }

  // Set the tolerances
  double xtol = 1.0e-10 * std::max(std::abs(px_min), std::abs(px_max));
  double ytol = 1.0e-10 * std::max(std::abs(py_min), std::abs(py_max));
  if (xtol <= 0) xtol = 1.0e-10;
  if (ytol <= 0) ytol = 1.0e-10;

  // If we have essentially one x value, check to see if y is in range.
  if (std::abs(px_max - px_min) < xtol) {
    edge = (y > (py_min - ytol) && y < (py_max + ytol) &&
            std::abs(px_max + px_min - 2 * x) < xtol);
    return false;
  }
  // If we have essentially one y value, check to see if x is in range.
  if (std::abs(py_max - py_min) < ytol) {
    edge = (x > (px_min - xtol) && x < (px_max + xtol) &&
            std::abs(py_max + py_min - 2 * y) < ytol);
    return false;
  }

  // Set "infinity" points.
  double xinf = px_min - std::abs(px_max - px_min);
  double yinf = py_min - std::abs(py_max - py_min);

  // Loop until successful or maximum iterations (100) reached.
  int niter = 0;
  bool done = false;
  int ncross = 0;

  TRandom *rand = new TRandom1();
  while (!done && niter < 100) {

    // Assume we will finish on this loop.
    done = true;

    // Loop over all edges, counting the number of edges crossed by a line
    //  extending from (x,y) to (xinf,yinf).
    ncross = 0;
    for (int i = 0; (done && i < pN); ++i) {

      // Determine whether the point lies on the edge.
      if (OnLine(px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN], x,
                 y)) {

        edge = true;
        return false;
      }

      // Determine whether this edge is crossed; if so increment the counter.
      double xc = 0., yc = 0.;
      if (LinesCrossed(x, y, xinf, yinf, px[i % pN], py[i % pN],
                       px[(i + 1) % pN], py[(i + 1) % pN], xc, yc))
        ++ncross;

      // Ensure this vertex is not crossed by the line from (x,y)
      //  to (xinf,yinf); if so recompute (xinf,yinf) and start over.
      if (OnLine(x, y, xinf, yinf, px[i], py[i])) {

        // Recompute (xinf,yinf).
		xinf = px_min - rand->Uniform() * std::abs(px_max - xinf);
        yinf = py_min - rand->Uniform() * std::abs(py_max - yinf);

        // Start over.
        done = false;
        ++niter;
      }
    }
  }

  // If we failed to finish iterating, return false.
  if (niter >= 100) {
    std::cerr <<"viewRegion::IsInPolygon: unable to determine whether ("
              << x << ", " << y << ") is inside a polygon.  Returning false.\n";
    return false;
  }

  // Point is inside for an odd, nonzero number of crossings.
  return (ncross != 2 * (ncross / 2));
}

//
// Determines whether the line connecting points (x1,y1) and (x2,y2)
//  and the line connecting points (u1,v1) and (u2,v2) cross somewhere
//  between the 4 points.  Sets the crossing point in (xc, yc).
//
// Ported from Garfield function CROSSD
//
//0 - not crossed, 1 - crossed, 2 - crossed at ends
int viewRegion::LinesCrossed(double x1, double y1, double x2, double y2,
                              double u1, double v1, double u2, double v2,
                              double& xc, double& yc) {

  // Set the tolerances.
  double xtol =
      1.0e-10 *
      std::max(std::abs(x1),
               std::max(std::abs(x2), std::max(std::abs(u1), std::abs(u2))));
  double ytol =
      1.0e-10 *
      std::max(std::abs(y1),
               std::max(std::abs(y2), std::max(std::abs(v1), std::abs(v2))));
  if (xtol <= 0) xtol = 1.0e-10;
  if (ytol <= 0) ytol = 1.0e-10;

  // Compute the distances and determinant (dx,dy) x (du,dv).
  double dy = y2 - y1;
  double dv = v2 - v1;
  double dx = x1 - x2;
  double du = u1 - u2;
  double det = dy * du - dx * dv;

  // Check for crossing because one of the endpoints is on both lines.
  if (OnLine(x1, y1, x2, y2, u1, v1)) {
    xc = u1;
    yc = v1;
    return 2;
  } else if (OnLine(x1, y1, x2, y2, u2, v2)) {
    xc = u2;
    yc = v2;
    return 2;
  } else if (OnLine(u1, v1, u2, v2, x1, y1)) {
    xc = x1;
    yc = y1;
    return 2;
  } else if (OnLine(u1, v1, u2, v2, x2, y2)) {
    xc = x2;
    yc = y2;
    return 2;
  }
  // Check if the lines are parallel (zero determinant).
  else if (std::abs(det) < xtol * ytol)
    return 0;
  // No special case: compute point of intersection.
  else {

    // Solve crossing equations.
    xc = (du * (x1 * y2 - x2 * y1) - dx * (u1 * v2 - u2 * v1)) / det;
    yc = ((-1 * dv) * (x1 * y2 - x2 * y1) + dy * (u1 * v2 - u2 * v1)) / det;

    // Determine if this point is on both lines.
    if (OnLine(x1, y1, x2, y2, xc, yc) && OnLine(u1, v1, u2, v2, xc, yc))
      return 1;
  }

  // The lines do not cross if we have reached this point.
  return 0;
}

//
// Determines whether the point (u,v) lies on the line connecting
//  points (x1,y1) and (x2,y2).
//
// Ported from Garfield function ONLIND
//
bool viewRegion::OnLine(double x1, double y1, double x2, double y2, double u,
                        double v) {
  // Set the tolerances
  double xtol =
      1.0e-10 * std::max(std::abs(x1), std::max(std::abs(x2), std::abs(u)));
  double ytol =
      1.0e-10 * std::max(std::abs(y1), std::max(std::abs(y2), std::abs(v)));
  if (xtol <= 0) xtol = 1.0e-10;
  if (ytol <= 0) ytol = 1.0e-10;

  // To store the coordinates of the comparison point
  double xc = 0, yc = 0;

  // Check if (u,v) is the point (x1,y1) or (x2,y2)
  if ((std::abs(x1 - u) <= xtol && std::abs(y1 - v) <= ytol) ||
      (std::abs(x2 - u) <= xtol && std::abs(y2 - v) <= ytol)) {
    return true;
  }
  // Check if the line is actually a point
  else if (std::abs(x1 - x2) <= xtol && std::abs(y1 - y2) <= ytol) {
    return false;
  }
  // Choose (x1,y1) as starting point if closer to (u,v)
  else if (std::abs(u - x1) + std::abs(v - y1) <
           std::abs(u - x2) + std::abs(v - y2)) {

    // Compute the component of the line from (x1,y1) to (u,v)
    //  along the line from (x1,y1) to (x2,y2)
    double dpar = ((u - x1) * (x2 - x1) + (v - y1) * (y2 - y1)) /
                  ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    // Determine the point on the line to which to compare (u,v)
    if (dpar < 0.0) {
      xc = x1;
      yc = y1;
    } else if (dpar > 1.0) {
      xc = x2;
      yc = y2;
    } else {
      xc = x1 + dpar * (x2 - x1);
      yc = y1 + dpar * (y2 - y1);
    }
  }
  // Choose (x2,y2) as starting point if closer to (u,v)
  else {

    // Compute the component of the line from (x2,y2) to (u,v)
    //  along the line from (x2,y2) to (x1,y1)
    double dpar = ((u - x2) * (x1 - x2) + (v - y2) * (y1 - y2)) /
                  ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    // Determine the point on the line to which to compare (u,v)
    if (dpar < 0.0) {
      xc = x2;
      yc = y2;
    } else if (dpar > 1.0) {
      xc = x1;
      yc = y1;
    } else {
      xc = x2 + dpar * (x1 - x2);
      yc = y2 + dpar * (y1 - y2);
    }
  }

  // Compare the calculated point to (u,v)
  if (std::abs(u - xc) < xtol && std::abs(v - yc) < ytol) return true;

  return false;
}

//namespace ParameterPile
//{
	std::deque <experiment_area> areas_to_draw;
	std::string this_path;

	std::string data_prefix_path;
	std::string calibration_file;
	std::string data_output_path;

	std::string DATA_MPPC_VERSION;
	std::string DATA_PMT_VERSION;

	std::string OUTPUT_MPPCS_PICS;
	std::string OUTPUT_PMT_PICS;
	std::string OUTPUT_MPPCS;

	experiment_area exp_area;
	int threads_number = 6; //obv. must be >=1

	int gnuplot_pad_size = 400;
	int gnuplot_max_size = 1600;
	int gnuplot_width = 900; //default for gnuplot is 640

	std::map < std::string, double > experiment_fields;
	std::map < std::string, double > PMT_V;
	std::map < std::string, double > PMT_dB;
	std::pair<int, int> calibaration_points;
	std::map < int, std::pair<double,double> > MPPC_coords;

	Bool_t draw_required(/*ParameterPile::*/experiment_area what)
	{
		for (auto i = areas_to_draw.begin(); i != areas_to_draw.end(); ++i)
			if (i->contains(what))
				return kTRUE;
		return kFALSE;
	}

	void Init_globals(bool full)
	{
		char path[FILENAME_MAX];
#if defined(__WIN32__)
		this_path = _getcwd(path, FILENAME_MAX);
#else
		this_path = getcwd(path, FILENAME_MAX);
#endif //__WIN32__
		TThread::Initialize();
		gStyle->SetOptFit();
		gStyle->SetStatY(0.9);
		gStyle->SetStatX(0.9);
		calibaration_points = std::pair<int, int>(3, 6);

		double SiPM_size = 10; //mm
		//<x,y>
		MPPC_coords.clear();
		MPPC_coords[32]= std::pair<double,double>(-2,-2);
		MPPC_coords[33]= std::pair<double,double>(-1,-2);
		MPPC_coords[34]= std::pair<double,double>(2,-2);
		MPPC_coords[35]= std::pair<double,double>(-2,-1);
		MPPC_coords[36]= std::pair<double,double>(1,-1);
		MPPC_coords[37]= std::pair<double,double>(2,-1);
		MPPC_coords[38]= std::pair<double,double>(0,0);
		MPPC_coords[39]= std::pair<double,double>(1,0);
		MPPC_coords[40]= std::pair<double,double>(-1,1);
		MPPC_coords[41]= std::pair<double,double>(0,1);
		MPPC_coords[42]= std::pair<double,double>(-2,2);
		MPPC_coords[43]= std::pair<double,double>(-1,2);
		MPPC_coords[44]= std::pair<double,double>(2,2);
		MPPC_coords[48]= std::pair<double,double>(0,-2);
		MPPC_coords[49]= std::pair<double,double>(1,-2);
		MPPC_coords[50]= std::pair<double,double>(-1,-1);
		MPPC_coords[51]= std::pair<double,double>(0,-1);
		MPPC_coords[52]= std::pair<double,double>(-2,0);
		MPPC_coords[53]= std::pair<double,double>(-1,0);
		MPPC_coords[54]= std::pair<double,double>(2,0);
		MPPC_coords[55]= std::pair<double,double>(-2,1);
		MPPC_coords[56]= std::pair<double,double>(1,1);
		MPPC_coords[57]= std::pair<double,double>(2,1);
		MPPC_coords[58]= std::pair<double,double>(0,2);
		MPPC_coords[59]= std::pair<double,double>(1,2);
		for (auto i = MPPC_coords.begin();i!=MPPC_coords.end();++i){
			i->second.first *=SiPM_size;
			i->second.second*=SiPM_size;
		}

		if (!full)
			return;
		if (areas_to_draw.empty())
			areas_to_draw.push_back(experiment_area());
		areas_to_draw.back().channels.erase();
		areas_to_draw.back().runs.erase();
		areas_to_draw.back().sub_runs.erase();
		areas_to_draw.back().runs.push_pair(0, 0);
		areas_to_draw.back().channels.push_pair(0, 1);
		areas_to_draw.back().channels.push_pair(32, 62);
		areas_to_draw.back().sub_runs.push_pair(0, 0);

		exp_area.channels.erase();
		exp_area.runs.erase();
		exp_area.sub_runs.erase();
		exp_area.channels.push_pair(0, 1);
		exp_area.channels.push_pair(8, 8);
		exp_area.channels.push_pair(12,12);
		//exp_area.channels.push_pair(2, 2);
		exp_area.channels.push_pair(32, 62);//will load only present channels
		exp_area.runs.push_pair(0, 0);
		exp_area.sub_runs.push_pair(0, 0);


		data_prefix_path = "../Data/180215/results/";
		calibration_file = "PMT_SiPM_48V_180215.dat";
		data_output_path = "../Data/180215/results/";

		DATA_MPPC_VERSION = "MPPCs_v1";
		DATA_PMT_VERSION = "PMT_v1";

		OUTPUT_MPPCS_PICS = "MPPCs_v1/MPPCs_";
		OUTPUT_PMT_PICS = "PMT_v1/PMT_";
		OUTPUT_MPPCS = "MPPC_";

		PMT_V.clear();
		PMT_V["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
		PMT_V["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;

		PMT_dB.clear();
		PMT_dB["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 3.98; //ratio, not actual dB
		PMT_dB["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 3.98;

		double coeff = (600 / 804.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
		experiment_fields.clear();
		experiment_fields["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 6;
		experiment_fields["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 8;
		experiment_fields["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 10;
		experiment_fields["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 11;
		experiment_fields["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 12;
		experiment_fields["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 14;

		for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
			j->second *= coeff;

		areas_to_draw.back().experiments.clear();
		areas_to_draw.back().experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		areas_to_draw.back().experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");


		exp_area.experiments.clear();
		exp_area.experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
		exp_area.experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	}
//};
