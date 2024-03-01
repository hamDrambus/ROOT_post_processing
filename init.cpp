//call gInterpreter->AddIncludePath("C:\\Users\\Elisa\\Desktop\\Top\\Data_processing\\post_processing\\include")  before
//this loads the project in the same order VisualStudio compiles it (.x init.cpp)
{
	gSystem->SetIncludePath("-I$ROOTSYS/include -I$HOME/Documents/Post_processing/include -I$HOME/Documents/Post_processing/source -I$HOME/Documents/Post_processing/Debug");
	gSystem->AddDynamicPath("$HOME/Documents/Post_processing/RelWithDebInfo");
	gSystem->AddDynamicPath("$HOME/Documents/Post_processing/source");

	gSystem->Load("libpost_processing.so");
	std::vector<double> x_y_regions;
	double d_S2_start, d_S2_finish, d_S1_start, d_S1_finish;

 }
