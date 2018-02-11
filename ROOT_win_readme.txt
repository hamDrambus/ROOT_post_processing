---------------------- -------------------- --------------------- ------------------- window_width: --------------------------|

0) Installing of the ROOT on windows follows the standard guide as far as I remember.

1) I used root_v5.34.34 and ran scripts in the root.exe via .L command. (before launching vsvars32.bat run is required)

2) In order to compile c++ code with root as library in visual studio, some settings are required:
//"root compile as library visual studio" in google-> http://www.pp.rhul.ac.uk/~connelly/tutorial/
It is not checked that all of the following steps are required. 'root' is the path to ROOT installation
	1) Add 'root\include' to Project Preferences->Configuration Properties->Include Directories,
	'root\lib' and 'root\bin' to Configuration Properties->Library Directories,
	source directory for user code (if is in not default vs folder) to Configuration Properties->Source Directories

	2) Add 'root\include' to c/c++->General->Additional Include Directories
	
	3) Add _CRT_SECURE_NO_WARNINGS to c/c++->Preprocessor->Preprocessor Definitions

	4) Add 'root\include\w32pragma.h' to c/c++->Advanced->Forced Include Files

	5) Add 'root\lib' and 'root\bin' to Linker->General->Additional Library Directories

	6) Add 'root_v5.34.34\lib\*.lib' to Linker->Input->Additional Dependencies

3) check code:

#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>


int main(int argc, char *argv[])
{
	int n_par = 0;
	char **f = NULL;
	TApplication* app = new TApplication("test_app",&n_par,f);
	TCanvas* c1 = new TCanvas("test", "test_title", 800, 500);
	TF1 *func = new TF1("test_func", "sin(x)+5*x", 0, 10);
	func->Draw();
	app->Run();
	delete app;
	return 0;
}
	