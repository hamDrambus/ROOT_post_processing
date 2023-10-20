Dependencies:
1) c++ Boost. Headers are enough, no need for compiling it.
2) CERN Root. Do not forget to run '$source ../bin/thisroot.sh' and to add root's library paths to LD_LIBRARY_PATH. Best to add 'source ../bin/thisroot.sh' to startup script in /etc/profile.d/
3) Data_processing program is a counterpart of this one, producing binary data which are input for this program.

=================================================
To compile this code use CleanCompile.sh script.

Then it can be run, for example as
	root -l // launch CERN root. -l flag removes startup popup logo.
	.x init.cpp	// load compiled library post_processing.so which has many different functions (see main.cpp for interface) for relatively arbitrary analysis.
	.x 221215/init.cpp	// load experimental data (pre-processed with Data_processing with corresponding Manifest221215.cpp) for 2022.12.15. Some electric fields may be removed from there so that RAM is not overloaded.
	.L 1analysis_history.cpp // Load analysis procedure. It can be first developed by hand using root terminal and then committed to the script file.
	.x x_script.cpp // has a loop of analysis_history() calls for all electric fields (change in the electric field is denoted as different "experiments").
The data for all dates (experiments) are analyzed in this manner using analysis_history.cpp files. There may be some alternative for and for exact details one must refer to them.

The plotting of the very final waveforms (average pulse shapes) does not require post_processing.so (.x init.cpp) and can be obtained by simply
	root -l
	.x 221124/compare_forms.cpp

GenEclipseProject.sh script can be used to create Eclipse project for further developing of the analysis tools.

Open in eclipse as [File->Import->General->Existing project in folder]->browse to generated -build folder. Build via [Project Explorer->Build Targets]. Debug as 'C/C++ Attach to Application' or 'C/C++ Application', with set binary location and disabled auto build (if necessary). When any files or libraries are added to/removed from the project, it must be regenerated with GenEclipseProject.sh.

=================================================
TODO:
1) Add loading pre-processed data (experiment/folder) on demand (in nex(), pex()) so that RAM is not overused.
