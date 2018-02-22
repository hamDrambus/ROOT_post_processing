#ifndef GRAPHIC_OUTPUT_MANAGER_H
#define GRAPHIC_OUTPUT_MANAGER_H

#include "GlobalParameters.h"

class Drawing
{
protected:
	std::string _name;
	Int_t _id_index;
	/*ParameterPile::*/DrawEngine _de;
	std::deque<std::string> _data_fnames;
	std::string _script_fname;
	std::deque<std::string> _script_lines;

	Int_t get_index_of_pad_marker(Int_t pad);//before which line enter the script;

public:
	//Drawing(void);
	Drawing(std::string name, /*ParameterPile::*/DrawEngine de, Int_t id_index);//can't change DrawEngine - too much of a trouble
	void AddToDraw(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::string title = "", std::string extra_txt = "", Int_t pad_index = 0, std::string prefix_lines="");
	
	void AddToDraw_baseline(Double_t base_line, std::string title = "baseline", std::string extra_txt = "", Int_t pad_index = 0);//May add more functions to draw, e.g. gauss
	void AddToDraw_vertical(Double_t x_pos, Double_t from_y, Double_t to_y, std::string extra_txt="", Int_t pad_index = 0);
	void AddToDraw(std::string definition_lines, std::string f_name, std::string title, std::string extra_txt = "", Int_t pad_index = 0);//any gnuplot function
	
	void DrawData(std::vector<Double_t> &xs, std::vector<Double_t> &ys, std::string title = "", std::string extra_txt = "");//draws only this vector
	void DrawData(void);
	void Clear(void);

	std::string get_name(void) const;
	Int_t get_id(void) const;
};

class GraphicOutputManager
{
public:
	std::deque<Drawing> _graphs;
	GraphicOutputManager(void);
	Drawing* GetDrawing(Int_t index); //if does not exist, doest not create it
	Drawing* GetDrawing(std::string name, Int_t index, /*ParameterPile::*/DrawEngine de);//if does not exist, creates it
	Drawing* CreateDrawing(std::string name, Int_t index, /*ParameterPile::*/DrawEngine de);
	void Draw (void);
	void Clear (void);
};

#endif