#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <string.h>
#include <sstream>

#include <TThread.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TVector.h>
#include <TF1.h>
#include <TMath.h>
#include <Math/Point2D.h>
#include <windows.h>

#undef max
#undef min

#define DATA_PREFIX std::string("../../../../Data/180201/results_x_ray/")
#define DATA_NAME_FORMAT "^MPPCs_4_thmV/MPPC_\d{2}/MPPC_33_double_I.dat$"
#define DATA_MPPC_VERSION "MPPCs_v3"
#define DATA_PMT_VERSION "PMT_v1"

#define OUTPUT_DIR std::string("../../../../Data/180201/results_x_ray/")
#define OUTPUT_MPPCS_PICS "MPPCs_v3/MPPCs_"
#define OUTPUT_PMT_PICS "PMT_v1/PMT_"
#define OUTPUT_MPPCS "MPPC_"

#define _TEMP_CODE
#define STD_CONT std::deque

//#define _USE_DEQUE
#ifdef _USE_DEQUE
#define DVECTOR std::deque<double>
#else
#define DVECTOR std::vector<double>
#endif

#define DITERATOR DVECTOR::iterator
#define D_REV_ITERATOR DVECTOR::reverse_iterator

//#define _PROCESS_GEMS

void open_output_file(std::string name, std::ofstream &str);

class peak
{
public:
	double left;
	double right;
	double S; //Area
	double A; //Amplitude (from baseline)
	peak();
};

#endif