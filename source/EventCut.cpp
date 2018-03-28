#include "EventCut.h"

EventCut::EventCut(int N_of_runs, CutType _type, std::string name) :
cut_type(_type), cut_name(name), exp_ind(-1), channel(-1), type(AnalysisStates::Type::PMT_S2_S), value_picker(NULL)
{
	if (cut_name == "")
		cut_name = (cut_type == RunCut ? "Run cut" : "Histogram cut");
	//value_picker = [](std::vector<double>& vals){return kTRUE;};
	//value_picker = [](std::vector<double>& vals)
	//{
	//	for (auto h = vals.begin(), _end_ = vals.end(); h != _end_; ++h)
	//		if (*h < 0)
	//			return kFALSE;
	//	return kTRUE; 
	//};
	if (cut_type == RunCut)
		do_accept.resize(N_of_runs, kTRUE);
}

EventCut::~EventCut()
{}
EventCut::EventCut(const EventCut & v)
{
	this->channel = v.channel;
	this->cut_name = v.cut_name;
	this->cut_type = v.cut_type;
	this->do_accept = v.do_accept;
	this->exp_ind = v.exp_ind;
	this->type = v.type;
	this->value_picker = v.value_picker; //TODO: value picker may store some info which is tricky to delete
}


void EventCut::SetName(std::string nm) {	cut_name = nm;}
std::string EventCut::GetName(void) const { return cut_name; }
void EventCut::SetPicker(FunctionWrapper *picker) { value_picker = picker; }
FunctionWrapper* EventCut::GetPicker(void) const { return value_picker; }
void EventCut::SetExperiment(int exp_i) { exp_ind = exp_i; }
int EventCut::GetExperiment(void) const { return exp_ind; }
void EventCut::SetChannel(int ch) { channel = ch; }
int EventCut::GetChannel(void) const { return channel; }
void EventCut::SetType(AnalysisStates::Type t) { type = t; }
AnalysisStates::Type EventCut::GetType(void) const { return type; }

void EventCut::SetAccept(int run, Bool_t accept)
{
	if (do_accept.size()<=run){
		std::cout << "EventCut::SetAccept (" << cut_name <<") warning: run size is bigger than the expected, resizing."<<std::endl;
		do_accept.resize(run + 1, kTRUE);
	}
	do_accept[run] = accept;
}
Bool_t EventCut::GetAccept(int run) const
{
	if (do_accept.size() <= run){
		std::cout << "EventCut::GetAccept (" << cut_name << ") warning: run size is bigger than the expected." << std::endl;
		return kFALSE;
	}
	return do_accept[run];
}

bool EventCut::operator () (std::vector<double> &pars, int run)
{
	return ((NULL == value_picker) ? kTRUE : (*value_picker)(pars, run));
}
