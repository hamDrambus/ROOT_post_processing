#include <stdio.h>
#if defined(__WIN32__)
#include <direct.h>
#endif

#include "GlobalParameters.h"


//TODO: some functions must be moved to PostProcessor class. (then I won't need the std::vector<Double_t>* get_data methods)

peak::peak() : right(-1), left(-1), S(-1), A(-1)
{}

GraphicOutputManager *gr_manager = NULL;
AnalysisManager *manager = NULL;
AllExperimentsResults* g_data = NULL;
PostProcessor* post_processor = NULL;

std::vector<Double_t>::iterator iter_add(std::vector<Double_t>::iterator& to, Int_t what, std::vector<Double_t>::iterator& end)
{
	if (what < 0)
		return end;
	return ((Int_t)(end - to) < what) ? end : to + what;
}

void open_output_file(std::string name, std::ofstream &str)
{
	std::string folder = name;
	while ((folder.back() != '\\') &&(folder.back()!='/') &&!folder.empty())
		folder.pop_back();
	if (!folder.empty())
		folder.pop_back();
#if defined(__WIN32__)
	if (!folder.empty()){
		DWORD ftyp = GetFileAttributesA(folder.c_str());
		if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY) || ftyp == INVALID_FILE_ATTRIBUTES){
				Int_t code = system(("mkdir \"" + folder + "\"").c_str());
				if (code)
					std::cout << "mkdir error: " << GetLastError() << std::endl;
			}
	}
#else
	struct stat st;
	stat(folder.c_str(),&st);
	if(!S_ISDIR(st.st_mode)){
		Int_t code = system(("mkdir \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir error: " << code << std::endl;
	}
#endif //_WIN32__
	str.open(name.c_str(), std::ios_base::trunc);
	if (!str.is_open()){
		std::cout << "Failed to open \"" << name << "\"" << std::endl;
	}
}

void DrawFileData(std::string name, std::vector<Double_t> xs, std::vector<Double_t> ys,/* ParameterPile::*/DrawEngine de)
{
	if (xs.size() != ys.size()){
		std::cout << "DrawFileData::input data size mismatch" << std::endl;
		return;
	}
	if (de == /*ParameterPile::*/DrawEngine::ROOT_){
		Double_t *xxxs = new Double_t[xs.size()];
		Double_t *yyys = new Double_t[ys.size()];
		for (Int_t h = 0; h < xs.size(); h++){
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
		for (Int_t s = 0; s < mod_name.size(); s++)
			if (mod_name[s] == '\\' || mod_name[s] == '/')
				mod_name[s] = '.';
		std::ofstream file;
		open_output_file("temp_gnuplot_files/" + mod_name, file);
		std::cout << "file " << "temp_gnuplot_files/" + mod_name << ".is_open() " << file.is_open() << std::endl;
#if defined(__WIN32__)
		if (!file.is_open())
			std::cout << GetLastError() << std::endl;
#endif //__WIN32__
		for (Int_t h = 0; h < xs.size(); h++)
			file << xs[h] << '\t' << ys[h] << std::endl;
		file.close();
		open_output_file("temp_gnuplot_files/script.sc", file);
		file << "plot '" << /*ParameterPile::*/this_path + "/temp_gnuplot_files/" + mod_name << "' u 1:2" << std::endl;
		file << "pause -1";
		file.close();
		INVOKE_GNUPLOT(this_path + "/temp_gnuplot_files/script.sc");
	}
}

//namespace ParameterPile
//{
	std::deque <experiment_area> areas_to_draw;
	std::string this_path;
	experiment_area exp_area;
	Int_t threads_number = 6; //obv. must be >=1

	Int_t gnuplot_pad_size = 400;
	Int_t gnuplot_max_size = 1600;
	Int_t gnuplot_width = 900; //default for gnuplot is 640

	std::map < std::string, Double_t > experiment_fields;
	std::pair<Int_t, Int_t> calibaration_poInt_ts;

	Bool_t draw_required(/*ParameterPile::*/experiment_area what)
	{
		for (auto i = areas_to_draw.begin(); i != areas_to_draw.end(); ++i)
			if (i->contains(what))
				return kTRUE;
		return kFALSE;
	}

	void Init_globals(void)
	{
		char path[FILENAME_MAX];
#if defined(__WIN32__)
		this_path = _getcwd(path, FILENAME_MAX);
#else
		this_path = getcwd(path, FILENAME_MAX);
#endif //__WIN32__
		TThread::Initialize();
		gStyle->SetOptFit();
		
		Double_t coeff = (600 / 804.0)*(1.54 / (1.54*1.8 + 1.01*0.4));
		/*experiment_fields["X_ray_12kV_SiPM_46V_THGEM_0V_coll_6mm"] = 12;
		experiment_fields["X_ray_14kV_SiPM_46V_THGEM_0V_coll_6mm"] = 14;
		experiment_fields["X_ray_16kV_SiPM_46V_THGEM_0V_coll_6mm"] = 16;
		experiment_fields["X_ray_18kV_SiPM_46V_THGEM_0V_coll_6mm"] = 18;
		experiment_fields["X_ray_20kV_SiPM_46V_THGEM_0V_coll_6mm"] = 20;*/

		experiment_fields["event_x-ray_4_thmV"] = 4;
		experiment_fields["event_x-ray_5_thmV"] = 5;
		experiment_fields["event_x-ray_6_thmV"] = 6;
		experiment_fields["event_x-ray_7_thmV"] = 7;
		experiment_fields["event_x-ray_8_thmV"] = 8;
		experiment_fields["event_x-ray_9_thmV"] = 9;
		experiment_fields["event_x-ray_10_thmV_recalib"] = 10;
		experiment_fields["event_x-ray_12_thmV"] = 12;
		experiment_fields["event_x-ray_14_thmV"] = 14;
		experiment_fields["event_x-ray_16_thmV"] = 16;


		for (auto j = experiment_fields.begin(); j != experiment_fields.end(); ++j)
			j->second *= coeff;

		calibaration_poInt_ts = std::pair<Int_t, Int_t>(0, 4);

		areas_to_draw.push_back(experiment_area());
		/*areas_to_draw.back().experiments.push_back("X_ray_12kV_SiPM_46V_THGEM_0V_coll_6mm");
		areas_to_draw.back().experiments.push_back("X_ray_14kV_SiPM_46V_THGEM_0V_coll_6mm");
		areas_to_draw.back().experiments.push_back("X_ray_16kV_SiPM_46V_THGEM_0V_coll_6mm");
		areas_to_draw.back().experiments.push_back("X_ray_18kV_SiPM_46V_THGEM_0V_coll_6mm");
		areas_to_draw.back().experiments.push_back("X_ray_20kV_SiPM_46V_THGEM_0V_coll_6mm");*/

		areas_to_draw.back().experiments.push_back("event_x-ray_4_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_5_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_6_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_7_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_8_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_9_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_10_thmV_recalib");
		areas_to_draw.back().experiments.push_back("event_x-ray_12_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_14_thmV");
		areas_to_draw.back().experiments.push_back("event_x-ray_16_thmV");


		areas_to_draw.back().runs.push_pair(0, 0);
		areas_to_draw.back().channels.push_pair(32, 62);
		areas_to_draw.back().sub_runs.push_pair(0, 0);

		exp_area.channels.push_pair(0, 0);
		//exp_area.channels.push_pair(2, 2);
		exp_area.channels.push_pair(32, 62);//will load only present channels
		exp_area.runs.push_pair(0, 0);
		exp_area.sub_runs.push_pair(0, 0);

		//exp_area.experiments.push_back("X_ray_12kV_SiPM_46V_THGEM_0V_coll_6mm");
		//exp_area.experiments.push_back("X_ray_14kV_SiPM_46V_THGEM_0V_coll_6mm");
		//exp_area.experiments.push_back("X_ray_16kV_SiPM_46V_THGEM_0V_coll_6mm");
		//exp_area.experiments.push_back("X_ray_18kV_SiPM_46V_THGEM_0V_coll_6mm");
		//exp_area.experiments.push_back("X_ray_20kV_SiPM_46V_THGEM_0V_coll_6mm");
		exp_area.experiments.push_back("event_x-ray_4_thmV");
		exp_area.experiments.push_back("event_x-ray_5_thmV");
		exp_area.experiments.push_back("event_x-ray_6_thmV");
		exp_area.experiments.push_back("event_x-ray_7_thmV");
		exp_area.experiments.push_back("event_x-ray_8_thmV");
		exp_area.experiments.push_back("event_x-ray_9_thmV");
		exp_area.experiments.push_back("event_x-ray_10_thmV_recalib");
		exp_area.experiments.push_back("event_x-ray_12_thmV");
		exp_area.experiments.push_back("event_x-ray_14_thmV");
		exp_area.experiments.push_back("event_x-ray_16_thmV");
	}
//};
