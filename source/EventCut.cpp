#include "EventCut.h"

EventCut::EventCut(int N_of_runs, CutType _type, std::string name) :
cut_type(_type), cut_name(name), exp_ind(-1), channel(-1), type(AnalysisStates::Type::PMT_S2_S)
{
	if (cut_name == "")
		cut_name = (cut_type == RunCut ? "Run cut" : "Histogram cut");
	value_picker = [](DVECTOR& vals){return true;};
	//value_picker = [](DVECTOR& vals)
	//{
	//	for (auto h = vals.begin(), _end_ = vals.end(); h != _end_; ++h)
	//		if (*h < 0)
	//			return false;
	//	return true; 
	//};
	if (cut_type == RunCut)
		do_accept.resize(N_of_runs, true);
}
void EventCut::SetName(std::string nm) {	cut_name = nm;}
std::string EventCut::GetName(void) const { return cut_name; }
void EventCut::SetPicker(std::function<bool(DVECTOR &vals)> &picker) { value_picker = picker; }
std::function<bool(DVECTOR &vals)> EventCut::GetPicker(void) const { return value_picker; }
void EventCut::SetExperiment(int exp_i) { exp_ind = exp_i; }
int EventCut::GetExperiment(void) const { return exp_ind; }
void EventCut::SetChannel(int ch) { channel = ch; }
int EventCut::GetChannel(void) const { return channel; }
void EventCut::SetType(AnalysisStates::Type t) { type = t; }
AnalysisStates::Type EventCut::GetType(void) const { return type; }

void EventCut::SetAccept(int run, bool accept)
{
	if (do_accept.size()<=run){
		std::cout << "EventCut::SetAccept (" << cut_name <<") warning: run size is bigger than the expected, resizing."<<std::endl;
		do_accept.resize(run + 1, true);
	}
	do_accept[run] = accept;
}
bool EventCut::GetAccept(int run) const
{
	if (do_accept.size() <= run){
		std::cout << "EventCut::GetAccept (" << cut_name << ") warning: run size is bigger than the expected." << std::endl;
		return false;
	}
	return do_accept[run];
}
