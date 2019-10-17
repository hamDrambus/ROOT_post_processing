#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <string.h>
#include <sstream>
#include <climits>
#include <algorithm>
//#include <functional>

#if defined (__WIN32__)
#include <sehmap.h>
#include <direct.h>
#include <Windows4Root.h>
#else
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#endif

#undef max
#undef min

#ifndef __ROOTCLING__
#include <boost/random/gamma_distribution.hpp>
#include <boost/random/inversive_congruential.hpp>
#include <boost/optional.hpp>
#endif //__CINT__

#include <TROOT.h>
#include <TThread.h>
#include <TApplication.h>
#include <TRint.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <Rtypes.h>
#include <TVector.h>
#include "TMatrixD.h"
#include "TVectorD.h"
#include <TF1.h>
#include <TMath.h>
#include <Math/Point2D.h>
#include <TRandom1.h>
#include "TH1D.h"
#include "TH2D.h"
#include "TH1I.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TPolyLine.h"
#include "TStyle.h"
//#include "Math/Unuran.h"

#define _TEMP_CODE

//#define _COMPLIE_MAIN_

#define PEAK_AVR_TIME
//#define WEIGHTED_COORD

//#define STD_CONT std::deque

//#define _USE_DEQUE
#ifdef _USE_DEQUE
#define DVECTOR std::deque<double>
#else
#define DVECTOR std::vector<double>
#endif

#define DITERATOR std::vector<double>::iterator
#define D_REV_ITERATOR std::vector<double>::reverse_iterator

typedef bool(*CUTTER)(std::vector<double>& pars, int run_n, void* stat_data);
typedef bool(*CUTTER_DRAW)(TCanvas* can, void* stat_data);

void open_output_file(std::string name, std::ofstream &str, std::ios_base::openmode _mode = std::ios_base::trunc);
void ensure_folder(std::string folder);
void ensure_file(std::string fname);
bool confirm_action (std::string action);
int getIndex(const std::vector<int>& channels, int ch); //TODO: template
int getIndex(const std::deque<int>& channels, int ch);

class TestSignalGenerator {
public:
	TestSignalGenerator(std::string prefix);
};

#if defined(__WIN32__)
#define INVOKE_GNUPLOT(a) system(("start \"\" \"%GNUPLOT%\\gnuplot.exe\" -c \"" + a + "\"").c_str())
#else
#define INVOKE_GNUPLOT(a) system(("konsole -e gnuplot \"" + a +"\"").c_str());
#endif //__WIN32__


class peak
{
public:
	double left;
	double right;
	double S; //Area
	double A; //Amplitude (from baseline)
#ifdef PEAK_AVR_TIME
	double t;
#endif
	peak();
};

#endif
