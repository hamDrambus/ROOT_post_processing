#ifndef EXPERIMENT_AREA_H
#define EXPERIMENT_AREA_H

#include <vector>
#include <deque>
#include <string.h>
#include "GlobalDefinitions.h"

//namespace ParameterPile {

	class area_vector
	{
	protected:
		Bool_t _is_valid;
		std::deque<Int_t> _vec;
		Int_t _last_returned_index;
		std::deque<Int_t>::iterator _last_returned_index_left;
	public:
		area_vector(void);
		Int_t get_order_index_by_index(Int_t ind);
		Int_t get_index_by_order_index(Int_t ind);
		Int_t get_next_index(void); //for running through all indices
		Int_t get_next_index(Int_t after);
		void push_pair(Int_t left, Int_t right);
		void push_back(Int_t val);
		Bool_t contains(Int_t index);
		Bool_t empty(void);
		Bool_t isValid(void);
		Int_t &back(void);
		Int_t &front(void);
		std::deque<area_vector> split_area(Int_t N);
		area_vector Int_tersect(area_vector with);
		void reset(); //clears _last_returned_index etc.
		void erase(); //clears vector
		//void refine (void); //e.g. [2,3][3,4] to [2,4] |OR| [4,5] [1,7] to [1,7]
	};

	class experiment_area //done //TODO - make analysis via this class. //->NextFile?
	{
	public:
		enum Type { Area, PoInt_t };
	protected:
		Type _type;
	public:
		experiment_area(Type type = Type::Area);
		experiment_area to_poInt_t(void);

		std::deque<std::string> experiments;
		area_vector runs; //contains pairs [from, to]
		area_vector channels; //contains pairs [from, to]
		area_vector sub_runs; //contains pairs [from, to]

		Bool_t isValid(void);
		Bool_t contains(/*ParameterPile::*/experiment_area what);//new draw_required
	};
//};

#endif
