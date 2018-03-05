#include "GraphicOutputManager.h"

Drawing::Drawing(std::string name, /*ParameterPile::*/DrawEngine de, Int_t id_index):
_name(name), _id_index(id_index), _de(de)
{
	for (Int_t s = 0; s < _name.size(); s++)
		if (_name[s] == '\\' || _name[s] == '/')
			_name[s] = '.';
	_script_fname = _name + ".sc";
	_script_lines.push_back("clear");
	_script_lines.push_back("set multiplot");
	_script_lines.push_back("set ytics nomirror");
	_script_lines.push_back("set y2tics");
	_script_lines.push_back("###");
	_script_lines.push_back("###");
	_script_lines.push_back("unset multiplot");
	_script_lines.push_back("pause -1");
}

Int_t Drawing::get_index_of_pad_marker(Int_t pad_index)//before which line enter the script;
{
	if (pad_index < 0)
		return -1;

	Int_t marker_n = 0;
	Int_t off = 0;
	Int_t last_pos = 0;
	for (auto i = _script_lines.begin(); (i != _script_lines.end()) && (marker_n < (pad_index + 2)); i++, off++)
		if ((*i) == std::string("###")){
			marker_n++;
			last_pos = off;
		}
	last_pos++;
	off--;//before which line enter the script;
	while (marker_n < (pad_index + 2)){
		_script_lines.insert(_script_lines.begin() + last_pos, "###");
		marker_n++;
		last_pos++;
		off = last_pos - 1;
	}
	return off;
}

void Drawing::AddToDraw(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::string title, std::string extra_txt, Int_t pad_index, std::string prefix_lines)
{
	Int_t off = get_index_of_pad_marker(pad_index);
	if (off < 0)
		return;
	
	_data_fnames.push_back(_name +"_"+ std::to_string(_data_fnames.size()));
	std::string line = "plot '"+/*ParameterPile::*/this_path + "/temp_gnuplot_files/" + _data_fnames.back() +"' u 1:2 title '"+title+"' " + extra_txt;
	std::vector<std::string> new_lines;
	if (!prefix_lines.empty())
		new_lines.push_back(prefix_lines);
	new_lines.push_back(line);
	_script_lines.insert(_script_lines.begin() + off, new_lines.begin(), new_lines.end());

	line = "temp_gnuplot_files/" + _data_fnames.back();
	std::ofstream f_data;
	open_output_file(line, f_data);
	for (auto i = xs.begin(), j = ys.begin(); (i != xs.end()) && (j != ys.end()); ++i, ++j)
		f_data<< *i << '\t' << *j << std::endl;
	f_data.close();
}

void Drawing::AddToDraw_baseline(Double_t base_line, std::string title, std::string extra_txt, Int_t pad_index)
{
	std::string definition_lines;
	//definition_lines=("a = " + std::to_string(base_line)+"\n");
	//definition_lines+="base(x) = a";
	AddToDraw(definition_lines, std::to_string(base_line), title, extra_txt, pad_index);
}

void Drawing::AddToDraw_vertical(Double_t x_pos, Double_t from_y, Double_t to_y, std::string extra_txt, Int_t pad_index)
{
	Int_t off = get_index_of_pad_marker(pad_index);
	if (off < 0)
		return;
	std::vector<std::string> new_lines;
	new_lines.push_back("set arrow from "+std::to_string(x_pos)+","+std::to_string(from_y)+" to "+std::to_string(x_pos)+","+std::to_string(to_y)+" nohead "
		+extra_txt);
	new_lines.push_back("show arrow");
	_script_lines.insert(_script_lines.begin() + off, new_lines.begin(), new_lines.end());
}

void Drawing::AddToDraw(std::string definition_lines, std::string f_name, std::string title, std::string extra_txt, Int_t pad_index)//any gnuplot function
{
	Int_t off = get_index_of_pad_marker(pad_index);
	if (off < 0)
		return;
	std::deque<std::string> new_lines;
	new_lines.push_back(definition_lines);
	new_lines.push_back("plot "+f_name+" title '"+title+"' "+extra_txt);
	_script_lines.insert(_script_lines.begin() + off, new_lines.begin(), new_lines.end());
}

void Drawing::DrawData(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::string title, std::string extra_txt)//draws only this vector
{
	if (xs.size() != ys.size()){
		std::cout << "Drawing::DrawData::input data size mismatch" << std::endl;
		return;
	}
	if (_de == /*ParameterPile::*/DrawEngine::ROOT_){
		Double_t *xxxs = new Double_t[xs.size()];
		Double_t *yyys = new Double_t[ys.size()];
		for (Int_t h = 0; h < xs.size(); h++){
			xxxs[h] = xs.at(h);
			yyys[h] = ys.at(h);
		}
		TGraph* gr = new TGraph(xs.size(), xxxs, yyys);
		TCanvas* can = new TCanvas(_name.c_str(), title.c_str());
		can->SetWindowPosition(100, 150);
		can->Update();
		gr->Draw();
		delete[] xxxs;
		delete[] yyys;
	} else {
		std::string mod_name = _name;
		for (Int_t s = 0; s < mod_name.size(); s++)
			if (mod_name[s] == '\\' || mod_name[s] == '/')
				mod_name[s] = '.';
		std::ofstream file;
		open_output_file("temp_gnuplot_files/" + mod_name, file);
		std::cout << "file " << "temp_gnuplot_files/" + mod_name << ".is_open() " << file.is_open() << std::endl;
		//if (!file.is_open())
		//	std::cout << GetLastError() << std::endl;
		for (Int_t h = 0; h < xs.size(); h++)
			file << xs[h] << '\t' << ys[h] << std::endl;
		file.close();
		open_output_file("temp_gnuplot_files\\"+_script_fname, file);
		file << "plot '" << /*ParameterPile::*/this_path + "/temp_gnuplot_files/" + mod_name << "' u 1:2 title '" << title<<"' "<<extra_txt<< std::endl;
		file << "pause -1";
		file.close();
		INVOKE_GNUPLOT(this_path + "/temp_gnuplot_files/" + _script_fname);
	}
}

void Drawing::DrawData(void)
{
	//Int_t N_marks =0;
	Int_t N_pads = 0;
	Bool_t is_empty = kFALSE;
	//this is set to kFALSE in order to account properly for the first "###"
	for (auto i = _script_lines.begin(); i != _script_lines.end(); i++){
		if ((*i) == "###"){
			if (!is_empty)
				N_pads++;
			is_empty = kTRUE;
		} else 
			is_empty = (*i).empty() ? is_empty : kFALSE;
	}
	N_pads--; //must still be >=1, so no check here
	if (1 == N_pads){//no need in multiplot
		_script_lines[1].insert(0,"#");
	}

#if defined(__WIN32__)
	_script_lines.insert(_script_lines.begin() + 1, "set terminal wxt size " + std::to_string(/*ParameterPile::*/gnuplot_width) + ","
		+ std::to_string(std::min(/*ParameterPile::*/gnuplot_max_size , N_pads*/*ParameterPile::*/gnuplot_pad_size)));
#else
	_script_lines.insert(_script_lines.begin() + 1, "set terminal x11 size " + std::to_string(/*ParameterPile::*/gnuplot_width) + ","
			+ std::to_string(std::min(/*ParameterPile::*/gnuplot_max_size , N_pads*/*ParameterPile::*/gnuplot_pad_size)));
#endif
	Int_t set_pads = 0;
	while (set_pads < N_pads){ //because inserting the lines invalidates iterators
		Int_t pads_off = set_pads + 1;//+1 in order to account for the first "###"
		for (auto i = _script_lines.begin(); i != _script_lines.end(); i++)
			if ((*i) == std::string("###")){
				is_empty = kTRUE;
				Bool_t has_plot = kFALSE;
				for (auto j = (i + 1); (j != _script_lines.end()) && ((*j) != "###"); j++) {//scan the pad
					//There should be only one 'plot' in one pad and the rest are 'replot'. Otherwise the same color is used and title is simply overriden 
					if (0 == (*j).find("plot")){
						if (has_plot)
							(*j).insert(0, "set yrange restore\nre");
						else {
							(*j).insert(0, "set yrange [] writeback\n"); //so my y scale in each pad is defined by the first added function
							has_plot = kTRUE;
						}
					}
					is_empty = (*j).empty() ? is_empty : kFALSE;
				}
				if (!is_empty){
					pads_off--;
					if (0 == pads_off){
						std::deque<std::string> pad_lines;
						pad_lines.push_back("unset arrow");
						pad_lines.push_back("set origin 0,"+std::to_string(set_pads*(1.0/N_pads)));
						pad_lines.push_back("set size 1," + std::to_string(1.0 / N_pads));
						_script_lines.insert(i + 1, pad_lines.begin(), pad_lines.end());
						set_pads++;
						break;
					}
				}
			}
	}
	std::ofstream file;
	open_output_file("temp_gnuplot_files/" + _script_fname, file);
	for (auto i = _script_lines.begin(); i != _script_lines.end(); i++)
		file << *i << std::endl;
	file.close();
	INVOKE_GNUPLOT(this_path + "/temp_gnuplot_files/" + _script_fname);
}

void Drawing::Clear(void)
{

}

std::string Drawing::get_name(void) const
{	return _name;}
Int_t Drawing::get_id(void) const
{	return _id_index;}

GraphicOutputManager::GraphicOutputManager(void)
{}

Drawing* GraphicOutputManager::GetDrawing(Int_t index) //if does not exist, doest not create it
{
	for (auto i = _graphs.begin(); i != _graphs.end(); i++)
		if ((*i).get_id() == index)
			return &(*i);
	return NULL;
}

Drawing* GraphicOutputManager::GetDrawing(std::string name, Int_t index, /*ParameterPile::*/DrawEngine de)//if does not exist, creates it
{
	for (auto i = _graphs.begin(); i != _graphs.end(); i++)
		if ((*i).get_name() == name || (*i).get_id() == index)
			return &(*i);
	return CreateDrawing(name, index, de);
}

Drawing* GraphicOutputManager::CreateDrawing(std::string name, Int_t index,/* ParameterPile::*/DrawEngine de)
{
	for (auto i = _graphs.begin(); i != _graphs.end(); i++)
		if ((*i).get_name() == name || (*i).get_id() == index)
			return NULL;
	_graphs.push_back(Drawing(name, de, index));
	return &_graphs.back();
}

void GraphicOutputManager::Draw(void)
{
	for (auto i = _graphs.begin(); i != _graphs.end(); i++)
		(*i).DrawData();
}

void GraphicOutputManager::Clear(void)
{
#ifdef _HOTFIX_CLEAR_MEMORY
	std::deque<Drawing>().swap(_graphs);
#else
	_graphs.clear();
#endif
}
