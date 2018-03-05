#ifdef __MAKECINT__
#pragma link C++ all global;
//#pragma link C++ all class;
//#pragma link off class shared_ptr<TFitResult>;
#pragma link off all class;
//#pragma link C++ class peak;
//#pragma link C++ class std::vector<double>;
//#pragma link C++ class std::deque<double>;
//#pragma link C++ class std::deque<std::deque<double>>;
//#pragma link C++ class std::deque<std::deque<std::deque<double>>>;
//#pragma link C++ class std::deque<peak>;
//#pragma link C++ class std::deque<std::deque<peak>>;
//#pragma link C++ class std::deque<std::deque<std::deque<peak>>>;
//#pragma link C++ class std::string;
//#pragma link C++ class std::deque<std::string>;


#pragma link C++ defined_in "GlobalDefinitions.h";
#pragma link C++ defined_in "ExperimentArea.h";
#pragma link C++ defined_in "GlobalParameters.h";
#pragma link C++ defined_in "AnalysisStates.h";
#pragma link C++ defined_in "FunctionWrapper.h";
#pragma link C++ defined_in "EventCut.h";
#pragma link C++ defined_in "Savitzky_Golay_filter.h";
#pragma link C++ defined_in "SignalOperations.h";
#pragma link C++ defined_in "PolynomialFit.h";
#pragma link C++ defined_in "Polynom2Order.h";
#pragma link C++ defined_in "AllRunsResults.h";
#pragma link C++ defined_in "AllExperimentResults.h";
#pragma link C++ defined_in "CalibrationInfo.h";
#pragma link C++ defined_in "HistogramSetups.h";
#pragma link C++ defined_in "PostProcessor.h";
#pragma link C++ defined_in "main.h";

#pragma link C++ all function;

#pragma link C++ namespace SignalOperations;
#pragma link C++ defined_in namespace SignalOperations;

#pragma link C++ nestedclasses;
//#pragma link C++ nestedfunction;
#pragma link C++ nestedtypedef;

#endif
