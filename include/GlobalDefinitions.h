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

#define DATA_PREFIX std::string("../Data/171123/results/")
#define DATA_NAME_FORMAT "^MPPCs_4_thmV/MPPC_\d{2}/MPPC_33_Double_I.dat$"
#define DATA_MPPC_VERSION "MPPCs_v2"
#define DATA_PMT_VERSION "PMT_v1"
#define CALIBRATION_FILE "PMT_SiPM_48V_171123.dat"

#define OUTPUT_DIR std::string("../Data/171123/results/")
#define OUTPUT_MPPCS_PICS "MPPCs_v2/MPPCs_"
#define OUTPUT_PMT_PICS "PMT_v1/PMT_"
#define OUTPUT_MPPCS "MPPC_"

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

typedef bool(*CUTTER)(std::vector<double>& pars, void* stat_data);

void open_output_file(std::string name, std::ofstream &str);


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
	double t;
	peak();
};


#endif
