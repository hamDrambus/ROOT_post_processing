#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

//#define __WIN32__

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <string.h>
#include <sstream>

#if defined(__WIN32__)
#include <direct.h>
#else
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#endif

#include <TROOT.h>
#include <TThread.h>
#include <TApplication.h>
#include <TRint.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <Rtypes.h>
#include <TVector.h>
#include <TF1.h>
#include <TMath.h>
#include <Math/Point2D.h>
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStyle.h"
#define _TEMP_CODE

#undef max
#undef min

#define DATA_NAME_FORMAT "^MPPCs_4_thmV/MPPC_\d{2}/MPPC_33_Double_I.dat$"

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

void open_output_file(std::string name, std::ofstream &str, std::ios_base::openmode _mode = std::ios_base::trunc);


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
