#include <stdio.h>
#include <direct.h>

#include "GlobalParameters.h"

#include <windows.h>

//TODO: some functions must be moved to PostProcessor class. (then I won't need the DVECTOR* get_data methods)

peak::peak() : right(-1), left(-1), S(-1), A(-1)
{}

GraphicOutputManager *gr_manager = NULL;
AnalysisManager *manager = NULL;
AllExperimentsResults* g_data = NULL;
PostProcessor* post_processor = NULL;

DITERATOR iter_add(DITERATOR& to, int what, DITERATOR& end)
{
	if (what < 0)
		return end;
	return ((int)(end - to) < what) ? end : to + what;
}

void open_output_file(std::string name, std::ofstream &str)
{
	std::string folder = name;
	while ((folder.back() != '\\') &&(folder.back()!='/') &&!folder.empty())
		folder.pop_back();
	if (!folder.empty())
		folder.pop_back();
	if (!folder.empty()){
		DWORD ftyp = GetFileAttributesA(folder.c_str());
		if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY) || ftyp == INVALID_FILE_ATTRIBUTES){
				int code = system(("mkdir \"" + folder + "\"").c_str());
				if (code)
					std::cout << "mkdir error: " << GetLastError() << std::endl;
			}
	}
	str.open(name, std::ios_base::trunc);
	if (!str.is_open())
		std::cout << "Failed to open \"" << name << "\"" << std::endl;
}

void DrawFileData(std::string name, std::vector<double> xs, std::vector<double> ys, ParameterPile::DrawEngine de)
{
	if (xs.size() != ys.size()){
		std::cout << "DrawFileData::input data size mismatch" << std::endl;
		return;
	}
	if (de == ParameterPile::DrawEngine::ROOT){
		double *xxxs = new double[xs.size()];
		double *yyys = new double[ys.size()];
		for (int h = 0; h < xs.size(); h++){
			xxxs[h] = xs.at(h);
			yyys[h] = ys.at(h);
		}
		TGraph* gr = new TGraph(xs.size(), xxxs, yyys);
		TCanvas* can = new TCanvas(name.c_str(), name.c_str());
		can->SetWindowPosition(100, 150);
		can->Update();
		gr->Draw();
		delete[] xxxs;
		delete[] yyys;
	} else {
		std::string mod_name = name;
		for (int s = 0; s < mod_name.size(); s++)
			if (mod_name[s] == '\\' || mod_name[s] == '/')
				mod_name[s] = '.';
		std::ofstream file;
		open_output_file("temp_gnuplot_files\\" + mod_name, file);
		std::cout << "file " << "temp_gnuplot_files\\" + mod_name << ".is_open() " << file.is_open() << std::endl;
		if (!file.is_open())
			std::cout << GetLastError() << std::endl;
		for (int h = 0; h < xs.size(); h++)
			file << xs[h] << '\t' << ys[h] << std::endl;
		file.close();
		open_output_file("temp_gnuplot_files\\script.sc", file);
		file << "plot '" << ParameterPile::this_path + "\\temp_gnuplot_files\\" + mod_name << "' u 1:2" << std::endl;
		file << "pause -1";
		file.close();
		system(("start \"\" \"%GNUPLOT%\\gnuplot.exe\" -c \"" + ParameterPile::this_path + "\\temp_gnuplot_files\\script.sc\"").c_str());
		//std::cout << "Gnuplot is not supported yet" << std::endl;
	}
}

namespace ParameterPile
{
	STD_CONT <experiment_area> areas_to_draw;
	std::string this_path;
	experiment_area exp_area;
	int threads_number = 6; //obv. must be >=1

	int gnuplot_pad_size = 400;
	int gnuplot_max_size = 1600;
	int gnuplot_width = 900; //default for gnuplot is 640

	std::map < std::string, double > experiment_fields;
	std::pair<int, int> calibaration_points;

	bool draw_required(ParameterPile::experiment_area what)
	{
		for (auto i = areas_to_draw.begin(); i != areas_to_draw.end(); ++i)
			if (i->contains(what))
				return true;
		return false;
	}

	void Init_globals(void)
	{
		char path[FILENAME_MAX];
		this_path = _getcwd(path, FILENAME_MAX);

		TThread::Initialize();
		
		double coeff = (600 / 808.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
		experiment_fields["4_thmV"] = 4;
		experiment_fields["5_thmV"] = 5;
		experiment_fields["6_thmV"] = 6;
		experiment_fields["7_thmV"] = 7;
		experiment_fields["8_thmV"] = 8;
		experiment_fields["9_thmV"] = 9;
		experiment_fields["10_thmV"] = 9.99;
		experiment_fields["10_thmV_recalib"] = 10.01;
		experiment_fields["12_thmV"] = 12;
		experiment_fields["14_thmV"] = 14;
		experiment_fields["16_thmV"] = 16;
		experiment_fields["18_thmV"] = 18;
		experiment_fields["20_thmV"] = 20;

		for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
			j->second *= coeff;

		calibaration_points = std::pair<int, int>(0, 4);

		areas_to_draw.push_back(experiment_area());
		areas_to_draw.back().experiments.push_back("4_thmV");
		areas_to_draw.back().experiments.push_back("5_thmV");
		areas_to_draw.back().experiments.push_back("6_thmV");
		areas_to_draw.back().experiments.push_back("7_thmV");
		areas_to_draw.back().experiments.push_back("8_thmV");
		areas_to_draw.back().experiments.push_back("9_thmV");
		areas_to_draw.back().experiments.push_back("10_thmV");
		areas_to_draw.back().experiments.push_back("10_thmV_recalib");
		areas_to_draw.back().experiments.push_back("12_thmV");
		areas_to_draw.back().experiments.push_back("14_thmV");
		areas_to_draw.back().experiments.push_back("16_thmV");
		areas_to_draw.back().experiments.push_back("18_thmV");
		areas_to_draw.back().experiments.push_back("20_thmV");
		areas_to_draw.back().runs.push_pair(0, 0);
		areas_to_draw.back().channels.push_pair(32, 62);
		areas_to_draw.back().sub_runs.push_pair(0, 0);

		exp_area.runs.push_pair(0, 0);
		exp_area.channels.push_pair(0, 1);
		//exp_area.channels.push_pair(2, 2);
		exp_area.channels.push_pair(32, 62);
		exp_area.sub_runs.push_pair(0, 0);

		exp_area.experiments.push_back("14_thmV");
		//exp_area.experiments.push_back("5_thmV");
		//exp_area.experiments.push_back("6_thmV");
		//exp_area.experiments.push_back("7_thmV");
		//exp_area.experiments.push_back("8_thmV");
		//exp_area.experiments.push_back("9_thmV");
		//exp_area.experiments.push_back("10_thmV");
		//exp_area.experiments.push_back("10_thmV_recalib");
		//exp_area.experiments.push_back("12_thmV");
		//exp_area.experiments.push_back("14_thmV");
		//exp_area.experiments.push_back("16_thmV");
		//exp_area.experiments.push_back("18_thmV");
		//exp_area.experiments.push_back("20_thmV");
	}
};
