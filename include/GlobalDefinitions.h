#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <string.h>
#include <sstream>

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
#define _TEMP_CODE
#ifndef _TEMP_CODE
#include <windows.h>
#endif //_TEMP_CODE

#undef max
#undef min

#define DATA_PREFIX std::string("../../../../Data/180201/results_x_ray/")
#define DATA_NAME_FORMAT "^MPPCs_4_thmV/MPPC_\d{2}/MPPC_33_Double_I.dat$"
#define DATA_MPPC_VERSION "MPPCs_v3"
#define DATA_PMT_VERSION "PMT_v1"
#define CALIBRATION_FILE "PMT_550V_SiPM_46V_v1.dat"

#define OUTPUT_DIR std::string("../../../../Data/180201/results_x_ray/")
#define OUTPUT_MPPCS_PICS "MPPCs_v3/MPPCs_"
#define OUTPUT_PMT_PICS "PMT_v1/PMT_"
#define OUTPUT_MPPCS "MPPC_"

//#define STD_CONT std::deque

//#define _USE_DEQUE
#ifdef _USE_DEQUE
#define DVECTOR std::deque<Double_t>
#else
#define DVECTOR std::vector<Double_t>
#endif

#define DITERATOR std::vector<Double_t>::iterator
#define D_REV_ITERATOR std::vector<Double_t>::reverse_iterator

typedef bool(*CUTTER)(std::vector<double>& pars, void* stat_data);

void open_output_file(std::string name, std::ofstream &str);

class peak
{
public:
	Double_t left;
	Double_t right;
	Double_t S; //Area
	Double_t A; //Amplitude (from baseline)
	peak();
};


#endif