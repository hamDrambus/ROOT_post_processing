//call gInterpreter->AddIncludePath("C:\\Users\\Elisa\\Desktop\\Top\\Data_processing\\post_processing\\include")  before
//this loads the project in the same order VisualStudio compiles it (.x loader.cpp)
{
	gSystem->SetIncludePath("-I$ROOTSYS/include -I/home/frolov/Documents/Post_processing/include -I/home/frolov/Documents/Post_processing/source -I/home/frolov/Documents/Post_processing/Debug");
	gSystem->AddDynamicPath("/home/frolov/Documents/Post_processing/Debug");
	gSystem->AddDynamicPath("/home/frolov/Documents/Post_processing/source");
	
	gSystem->Load("libpost_processing.so");
	
	/*gROOT->ProcessLine(".L source/ExperimentArea.cpp+");
	gROOT->ProcessLine(".L source/GlobalParameters.cpp+");
	gROOT->ProcessLine(".L source/SingleRunData.cpp+");
	gROOT->ProcessLine(".L source/SingleRunResults.cpp+");
	gROOT->ProcessLine(".L source/SignalOperations.cpp+");
	gROOT->ProcessLine(".L source/Savitzky_Golay_filter.cpp+");
	gROOT->ProcessLine(".L source/PostProcessor.cpp+");
	gROOT->ProcessLine(".L source/PolynomialFit.cpp+");
	gROOT->ProcessLine(".L source/Polynom2Order.cpp+");
	gROOT->ProcessLine(".L source/main.cpp+");
	gROOT->ProcessLine(".L source/HistogramSetups.cpp+");
	gROOT->ProcessLine(".L source/GraphicOutputManger.cpp+");
	gROOT->ProcessLine(".L source/FunctionWrapper.cpp+");
	gROOT->ProcessLine(".L source/EventCut.cpp+");
	gROOT->ProcessLine(".L source/CalibrationInfo.cpp+");
	gROOT->ProcessLine(".L source/AnalysisStates.cpp+");
	gROOT->ProcessLine(".L source/AnalysisManager.cpp+");
	gROOT->ProcessLine(".L source/AllRunsResults.cpp+");
	gROOT->ProcessLine(".L source/AllExperimnetsResults.cpp+");*/

 }
