#ifndef EXPERIMENT_AREA_H
#define EXPERIMENT_AREA_H

#include "GlobalDefinitions.h"

//namespace ParameterPile {

	class area_vector
	{
	protected:
		Bool_t _is_valid;
		std::deque<int> _vec;
		int _last_returned_index;
		std::deque<int>::iterator _last_returned_index_left;
	public:
		area_vector(void);
		int get_order_index_by_index(int ind);
		int get_index_by_order_index(std::size_t ind) const;
		std::size_t size(void) const;
		int get_next_index(void); //for running through all indices
		int get_next_index(int after);
		void push_pair(int left, int right);
		void push_back(int val);
		Bool_t contains(int index);
		Bool_t empty(void);
		Bool_t isValid(void);
		int &back(void);
		int &front(void);
		std::deque<area_vector> split_area(int N);
		area_vector intersect(area_vector with);
		void reset(); //clears _last_returned_index etc.
		void erase(); //clears vector
		//void refine (void); //e.g. [2,3][3,4] to [2,4] |OR| [4,5] [1,7] to [1,7]
	};

	class experiment_area
	{
	public:
		enum Type { Area, Point };
	protected:
		Type _type;
	public:
		experiment_area(Type type = Type::Area);
		experiment_area to_point(void);

		std::deque<std::string> experiments;
		area_vector runs; //contains pairs [from, to]
		area_vector channels; //contains pairs [from, to]
		area_vector sub_runs; //contains pairs [from, to]

		Bool_t isValid(void);
		Bool_t contains(/*ParameterPile::*/experiment_area what);//new draw_required
	};
//};

#endif
