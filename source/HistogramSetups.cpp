#include "HistogramSetups.h"
#include "StateData.h"

class StateData;
class TriggerData;

HistogramSetups::HistogramSetups(const std::deque<int>& channels) :
	filled_hist(false), fitted(false), x_max(boost::none), y_max(boost::none),
	x_drawn_max(boost::none), y_drawn_max(boost::none), num_of_runs(boost::none),
	num_of_fills(boost::none), num_of_drawn_fills(boost::none), stat_weight(boost::none),
	stat_drawn_weight(boost::none), x_lims(boost::none),
	y_lims(boost::none), x_drawn_lims(boost::none), y_drawn_lims(boost::none),
	x_mean(boost::none), y_mean(boost::none), x_drawn_mean(boost::none),
	y_drawn_mean(boost::none), x_variance(boost::none), x_drawn_variance(boost::none),
	y_variance(boost::none), y_drawn_variance(boost::none), is_valid_fit_function(false),
	use_default_setups(true), N_bins(0), N_bins_y(0),  N_gauss(0), use_fit(false), extra_data(NULL),
	logscale_x(false), logscale_y(false), logscale_z(false)
{
	for (std::size_t ind = 0, ind_end_ = channels.size(); ind != ind_end_; ++ind)
		active_channels.push(channels[ind], true);
}

HistogramSetups::HistogramSetups(const HistogramSetups& setups)
{
	hist_cuts = setups.hist_cuts;
	active_channels = setups.active_channels;
	N_bins = setups.N_bins;
	N_bins_y = setups.N_bins_y;
	x_zoom = setups.x_zoom;
	y_zoom = setups.y_zoom;
	is_zoomed = setups.is_zoomed;
	//Fit info:
	N_gauss = setups.N_gauss;
	par_val = setups.par_val;
	par_left_limits = setups.par_left_limits;
	par_right_limits = setups.par_right_limits;
	use_fit = setups.use_fit;
	//Following values represent status, not input parameters
	fitted = setups.fitted;
	x_axis_title = setups.x_axis_title;
	y_axis_title = setups.y_axis_title;
	is_valid_fit_function = setups.is_valid_fit_function;
	use_default_setups = setups.use_default_setups;

	logscale_x = setups.logscale_x;
	logscale_y = setups.logscale_y;
	logscale_z = setups.logscale_z;

	//1st tier parameters of distribution: (stored in order to minimize calls of LoopThroughData to recalculate them)
	num_of_runs = setups.num_of_runs;
	num_of_fills = setups.num_of_fills;
	num_of_drawn_fills = setups.num_of_drawn_fills;
	stat_weight = setups.stat_weight;
	stat_drawn_weight = setups.stat_drawn_weight;
	x_lims = setups.x_lims;
	y_lims = setups.y_lims;
	x_drawn_lims = setups.x_drawn_lims;
	y_drawn_lims = setups.y_drawn_lims;
	x_mean = setups.x_mean;
	y_mean = setups.y_mean;
	x_drawn_mean = setups.x_drawn_mean;
	y_drawn_mean = setups.y_drawn_mean;
	//2nd tier parameters (require 2 calls to LoopThroughData)
	x_max = setups.x_max;
	y_max = setups.y_max; //Bin with maximum value y. Require filled histogram
	x_drawn_max = setups.x_drawn_max;
	y_drawn_max = setups.y_drawn_max;
	x_variance = setups.x_variance;
	x_drawn_variance = setups.x_drawn_variance;
	y_variance = setups.y_variance;
	y_drawn_variance = setups.y_drawn_variance;

	filled_hist = setups.filled_hist; //1st call is to determine default N bins, x-y range, etc.

	if (setups.extra_data)
		extra_data = setups.extra_data->Clone();
	else
		extra_data = NULL;
}

HistogramSetups::~HistogramSetups()
{
	if (extra_data)
		delete extra_data;
}

std::deque<EventCut>* CanvasSetups::get_run_cuts (int exp_ind)
{
	if (RunCuts.size()!=manual_setups.size()) {
		std::cerr<<"CanvasSetups::get_run_cuts: Error: RunCuts-canvases size mismatch"<<std::endl;
		return NULL;
	}
	if (canvas_ind>=manual_setups.size()) {
		std::cerr<<"CanvasSetups::get_run_cuts: Error: canvas index ("<<canvas_ind<<") is out of range"<<std::endl;
		return NULL;
	}
	std::size_t exp_sz = RunCuts[canvas_ind].size();
	if ((exp_ind<0)||(exp_ind>=exp_sz)) {
		std::cerr<<"CanvasSetups::get_run_cuts: Error: experiment index ("<<exp_ind<<") is out of range"<<std::endl;
		return NULL;
	}
	return & (RunCuts[canvas_ind][exp_ind]);
}

HistogramSetups* CanvasSetups::get_hist_setups(void) const
{
	return get_hist_setups(current_exp_index, current_channel, current_type);
}

HistogramSetups* CanvasSetups::get_hist_setups(int exp_ind, int channel, Type type) const
{
	if (canvas_ind>=manual_setups.size()) {
		std::cerr<<"CanvasSetups::get_hist_setups: Error: canvas index ("<<canvas_ind<<") is out of range"<<std::endl;
		return NULL;
	}
	std::size_t ty_sz = manual_setups[canvas_ind].size();
	std::size_t exp_sz = manual_setups[canvas_ind][type].size();
	if ((exp_ind>=exp_sz)||(exp_ind<0)) {
		std::cerr<<"CanvasSetups::get_hist_setups: Error: experiment index ("<<exp_ind<<") is out of range"<<std::endl;
		return NULL;
	}
	std::size_t ch_sz = manual_setups[canvas_ind][type][exp_ind].size();
	int ch_ind = channel_to_index(channel, type);
	if (ch_ind<0) {
		std::cerr<<"CanvasSetups::get_hist_setups: Error: channel ("<<channel<<") is out of range"<<std::endl;
		return NULL;
	}
	return manual_setups[canvas_ind][type][exp_ind][ch_ind];
}

bool CanvasSetups::set_hist_setups(HistogramSetups* setups, int exp_ind, int channel, Type type)
{
	std::size_t ty_sz = manual_setups[canvas_ind].size();
	if (type >= manual_setups[canvas_ind].size()) {
		std::cerr<<"CanvasSetups::set_hist_setups: Error during initialization - manual_setups size is less than the number of types"<<std::endl;
		return false;
	}
	std::size_t exp_sz = manual_setups[canvas_ind][type].size();
	if ((exp_ind<0)||(exp_ind>=exp_sz)) {
		std::cerr<<"CanvasSetups::set_hist_setups: Error: experiment index ("<<exp_ind<<") is out of range"<<std::endl;
		return false;
	}
	int ch_ind = channel_to_index(channel, type);
	if (ch_ind < 0) {
		std::cerr<<"CanvasSetups::set_hist_setups: Error: channel ("<<channel<<") is out of range"<<std::endl;
		return false;
	}
	if ((NULL != manual_setups[canvas_ind][type][exp_ind][ch_ind]) && (setups != manual_setups[canvas_ind][type][exp_ind][ch_ind]))
		delete manual_setups[canvas_ind][type][exp_ind][ch_ind];
	manual_setups[canvas_ind][type][exp_ind][ch_ind] = (NULL==setups ? NULL : new HistogramSetups(*setups));
	setups = manual_setups[canvas_ind][type][exp_ind][ch_ind];
	if (NULL != setups) {
		//TODO: quite ugly approach
		if (NULL==setups->extra_data && ShapeFitData::IsForState(this))
			setups->extra_data = new ShapeFitData(this);
		if (NULL==setups->extra_data && TriggerAvgTData::IsForState(this))
			setups->extra_data = new TriggerAvgTData(this);
		if (NULL==setups->extra_data && TriggerFitData::IsForState(this))
			setups->extra_data = new TriggerFitData(this);
		if (NULL==setups->extra_data && TriggerData::IsForState(this))
			setups->extra_data = new TriggerData(this);
		if (NULL==setups->extra_data && StateData::IsForState(this))
			setups->extra_data = new StateData(this);
	}
	return true;
}

TCanvas * CanvasSetups::get_current_canvas (void)
{
	if (manual_setups.size()!=canvases.size()) {
		std::cerr<<"CanvasSetups::get_current_canvas: Error size mismatch"<<std::endl;
		return NULL;
	}
	if (canvas_ind>=canvases.size())
		return NULL;
	return canvases[canvas_ind];
}

bool CanvasSetups::set_fit_function(TF1* func)
{
	if (manual_setups.size()!=fit_functions.size()) {
		std::cerr<<"CanvasSetups::set_fit_function: Error size mismatch"<<std::endl;
		return false;
	}
	if (NULL != fit_functions[canvas_ind] && (func != fit_functions[canvas_ind]))
		fit_functions[canvas_ind]->Delete();
	fit_functions[canvas_ind] = (NULL==func ? NULL : new TF1(*func));
	return true;
}

TF1 * CanvasSetups::get_current_fit_function(void)
{
	if (manual_setups.size()!=fit_functions.size()) {
		std::cerr<<"CanvasSetups::get_current_fit_func: Error size mismatch"<<std::endl;
		return NULL;
	}
	if (canvas_ind>=manual_setups.size())
		return NULL;
	return fit_functions[canvas_ind];
}

TF1* CanvasSetups::create_fit_function(HistogramSetups* setups, std::pair<double, double> domain)
{
	TF1 *fit_func;
	std::string func_name;
	if (!setups->use_fit)
		return NULL;
	for (int ng = 0; ng < setups->N_gauss; ++ng){
		func_name += "gaus(" + std::to_string(ng * 3) + ")";
		if (ng != setups->N_gauss - 1)
			func_name += "+";
	}
	if (func_name.empty())
		return NULL;
	else {
		fit_func = new TF1("ff", func_name.c_str(), domain.first, domain.second);
		for (int par = 0; (par < setups->N_gauss * 3) && (par < setups->par_val.size()); ++par){
			fit_func->SetParLimits(par, setups->par_left_limits[par], setups->par_right_limits[par]);
			fit_func->SetParameter(par, setups->par_val[par]);
		}
	}
	return fit_func;
}

TH1D * CanvasSetups::get_current_hist1 (void)
{
	if (manual_setups.size()!=hists_1D.size()) {
		std::cerr<<"CanvasSetups::get_current_hist1: Error size mismatch"<<std::endl;
		return NULL;
	}
	if (canvas_ind>=manual_setups.size())
		return NULL;
	return hists_1D[canvas_ind];
}

bool CanvasSetups::set_hist1 (TH1D * hist)
{
	if (manual_setups.size()!=hists_1D.size()) {
		std::cerr<<"CanvasSetups::set_hist1: Error size mismatch"<<std::endl;
		return false;
	}
	if (NULL != hists_1D[canvas_ind] && (hist != hists_1D[canvas_ind]))
		hists_1D[canvas_ind]->Delete();
	hists_1D[canvas_ind] = (NULL==hist ? NULL : new TH1D(*hist));
	return true;
}

TH2D * CanvasSetups::get_current_hist2 (void)
{
	if (manual_setups.size()!=hists_2D.size()) {
		std::cerr<<"CanvasSetups::get_current_hist2: Error size mismatch"<<std::endl;
		return NULL;
	}
	if (canvas_ind>=manual_setups.size())
		return NULL;
	return hists_2D[canvas_ind];
}

bool CanvasSetups::set_hist2 (TH2D * hist)
{
	if (manual_setups.size()!=hists_2D.size()) {
		std::cerr<<"CanvasSetups::set_hist2: Error size mismatch"<<std::endl;
		return false;
	}
	if (NULL != hists_2D[canvas_ind] && (hist != hists_2D[canvas_ind]))
		hists_2D[canvas_ind]->Delete();
	hists_2D[canvas_ind] = (NULL==hist ? NULL : new TH2D(*hist));
	return true;
}

Bool_t CanvasSetups::StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type)
{
	return StateChange(to_ch, to_exp, to_type, canvas_ind, from_ch, from_exp, from_type, canvas_ind);
}

Bool_t CanvasSetups::StateChange(int to_ch, int to_exp, Type to_type, std::size_t to_canvas, int from_ch, int from_exp, Type from_type, std::size_t from_canvas)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL == setups) {
		Invalidate(invDefault);
		setups = new HistogramSetups(channel_list(to_type));
		set_hist_setups(setups, to_exp, to_ch, to_type);
		delete setups;
	}
	return ((from_canvas!=to_canvas)||AStates::StateChange(to_ch, to_exp, to_type, from_ch, from_exp, from_type));
}

Bool_t CanvasSetups::CorrelationXChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type)
{
	if (!AStates::CorrelationXChange(exp_index, to_ch, to_type, from_ch, from_type)) {
		return kFALSE;
	}
	if (to_type != from_type) {
		for (std::size_t exp = 0, exp_end = manual_setups[canvas_ind][Correlation_x].size(); exp != exp_end; ++exp) {
			manual_setups[canvas_ind][Correlation_x][exp].clear();
			loop_channels_reset();
			for (int ch = 0, ch_ind = 0; loop_channels((AStates::Type)Correlation_x, ch, ch_ind); ) {
				manual_setups[canvas_ind][Correlation_x][exp].push_back(NULL);
			}
		}
	}
	return kTRUE;
}

Bool_t CanvasSetups::CorrelationYChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type)
{
	if (!AStates::CorrelationYChange(exp_index, to_ch, to_type, from_ch, from_type)) {
		return kFALSE;
	}
	if (to_type != from_type) {
		for (std::size_t exp = 0, exp_end = manual_setups[canvas_ind][Correlation_x].size(); exp != exp_end; ++exp) {
			manual_setups[canvas_ind][Correlation_y][exp_index].clear();
			loop_channels_reset();
			for (int ch = 0, ch_ind = 0; loop_channels((AStates::Type)Correlation_y, ch, ch_ind); ) {
				manual_setups[canvas_ind].back().back().push_back(NULL);
			}
		}
	}
	return kTRUE;
}

bool CanvasSetups::Invalidate(unsigned int label)
{
	HistogramSetups* setups = get_hist_setups();
	if (NULL == setups) {
		return true; //the case when switched to new channel/type and default histogram setups are not created yet
	}
	if (label & invHistogram) { //need to redraw histogram when switching to old analysis state or when number on bins has changed
		setups->filled_hist = false;
		setups->fitted = false;
		setups->x_max = boost::none;
		setups->y_max = boost::none;
		setups->x_drawn_max = boost::none;
		setups->y_drawn_max = boost::none;
	}
	if ((label & invCuts) || (label & invData)) {
		setups->fitted = false;
		setups->filled_hist = false;
		setups->x_max = boost::none;
		setups->y_max = boost::none;
		setups->x_drawn_max = boost::none;
		setups->y_drawn_max = boost::none;
		setups->num_of_runs = boost::none;
		setups->num_of_fills = boost::none;
		setups->num_of_drawn_fills = boost::none;
		setups->stat_weight = boost::none;
		setups->stat_drawn_weight = boost::none;
		setups->x_lims = boost::none;
		setups->y_lims = boost::none;
		setups->x_drawn_lims = boost::none;
		setups->y_drawn_lims = boost::none;
		setups->x_mean = boost::none;
		setups->y_mean = boost::none;
		setups->x_drawn_mean = boost::none;
		setups->y_drawn_mean = boost::none;
		setups->x_variance = boost::none;
		setups->x_drawn_variance = boost::none;
		setups->y_variance = boost::none;
		setups->y_drawn_variance = boost::none;
	}
	if (label & invDisplayedCuts) {
		setups->fitted = false;
		setups->x_drawn_max = boost::none;
		setups->y_drawn_max = boost::none;
		setups->num_of_drawn_fills = boost::none;
		setups->stat_drawn_weight = boost::none;
		setups->x_drawn_lims = boost::none;
		setups->y_drawn_lims = boost::none;
		setups->x_drawn_mean = boost::none;
		setups->y_drawn_mean = boost::none;
		setups->x_drawn_variance = boost::none;
		setups->y_drawn_variance = boost::none;
	}
	if (label & invFit) {
		setups->fitted = false;
	}
	if (label & invFitFunction) {
		setups->fitted = false;
		setups->is_valid_fit_function = false;
	}
	if (label & invDefault) {
		setups->use_default_setups = true;
	}
	return true;
}

CanvasSetups::~CanvasSetups()
{
	for (std::size_t c =0, c_end_ = manual_setups.size(); c!=c_end_; ++c) {
		for (std::size_t h = 0, h_end_ = manual_setups[c].size(); h!=h_end_; ++h) {
			for (std::size_t exp = 0, exp_end_ = manual_setups[c][h].size(); exp != exp_end_; ++exp) {
				for (std::size_t ch=0, ch_end_ = manual_setups[c][h][exp].size(); ch!=ch_end_; ++ch) {
					if (NULL!=manual_setups[c][h][exp][ch])
						delete manual_setups[c][h][exp][ch];
				}
			}
		}
	}
	for (std::size_t f = 0, f_end_ = fit_functions.size(); f!=f_end_; ++f) {
		if (NULL!=fit_functions[f])
			fit_functions[f]->Delete();
	}
	for (std::size_t h = 0, h_end_ = hists_1D.size(); h!=h_end_; ++h) {
		if (NULL!=hists_1D[h])
			hists_1D[h]->Delete();
	}
	for (std::size_t h = 0, h_end_ = hists_2D.size(); h!=h_end_; ++h) {
		if (NULL!=hists_2D[h])
			hists_2D[h]->Delete();
	}
	for (std::size_t c = 0, c_end_ = canvases.size(); c!=c_end_; ++c) {
		if (NULL!=canvases[c])
			canvases[c]->Delete();
	}
}

CanvasSetups::CanvasSetups(std::deque<int> &mppc_channels_, std::deque<int> &pmt_channels_, std::deque<std::string>& experiments_):
		AStates(mppc_channels_, pmt_channels_, experiments_)
{
	canvas_ind=0;
	TCanvas* new_c = new TCanvas((std::string("inter_canvas_") + std::to_string(canvas_ind)).c_str(), (std::string("inter_canvas_") + std::to_string(canvas_ind)).c_str());
	canvases.push_back(new_c);
	stateS ind_state;
	ind_state.channel = current_channel;
	ind_state.exp_index = current_exp_index;
	ind_state.type = current_type;
	canvases_states.push_back(ind_state);
	new_c->cd();
	new_c->SetGrid();
	new_c->SetTicks();
	new_c->ToggleEventStatus();
	new_c->ToggleToolBar();
	fit_functions.push_back(NULL);
	hists_1D.push_back(NULL);
	hists_2D.push_back(NULL);
	manual_setups.push_back(std::deque<std::deque<std::deque<HistogramSetups*>>>());
	for (std::size_t h = _first_state; h != ((std::size_t)_last_state + 1); ++h) {
		manual_setups[canvas_ind].push_back(std::deque<std::deque<HistogramSetups*>>());
		for (auto exp = experiments_.begin(); exp != experiments_.end(); ++exp) {
			manual_setups[canvas_ind].back().push_back(std::deque<HistogramSetups*>());
			loop_channels_reset();
			for (int ch=0, ch_ind=0; loop_channels ((AStates::Type)h, ch, ch_ind); ) {
				manual_setups[canvas_ind].back().back().push_back(NULL);
			}
		}
	}

	RunCuts.push_back(std::deque<std::deque<EventCut> >());
	for (auto exp = experiments_.begin(); exp != experiments_.end(); ++exp) {
		RunCuts[canvas_ind].push_back(std::deque<EventCut>());
	}
}

std::pair<double, double> CanvasSetups::get_current_x_zoom(void)
{
	HistogramSetups* curr_hist = get_hist_setups();
	std::pair<double, double> ret = (NULL== curr_hist ? std::pair<double, double> (0,0) : curr_hist->x_zoom);
	return ret;
}

std::pair<double, double> CanvasSetups::get_current_y_zoom(void)
{
	HistogramSetups* curr_hist = get_hist_setups();
	std::pair<double, double> ret = (NULL== curr_hist ? std::pair<double, double> (0,0) : curr_hist->y_zoom);
	return ret;
}

std::pair<bool, bool> CanvasSetups::is_zoomed(void)
{
	HistogramSetups* curr_hist = get_hist_setups();
	std::pair<bool, bool> ret = (NULL== curr_hist ? std::pair<bool, bool> (false, false) : curr_hist->is_zoomed);
	return ret;
}

bool CanvasSetups::set_zoom(std::pair<double, double> x_z, std::pair<double, double> y_z)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return false;
	bool invalidate = false;
	curr_hist->is_zoomed.first = (x_z.first!=-DBL_MAX)||(x_z.second!=DBL_MAX);
	curr_hist->is_zoomed.second = (y_z.first!=-DBL_MAX)||(y_z.second!=DBL_MAX);
	if (curr_hist->x_zoom != x_z || curr_hist->y_zoom != y_z)
		invalidate = true;
	curr_hist->x_zoom = x_z;
	curr_hist->y_zoom = y_z;
	if (invalidate)
		Invalidate(invHistogram);
	return true;
}
bool CanvasSetups::unset_zoom(void)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return false;
	bool invalidate = false;
	if (curr_hist->is_zoomed.first || curr_hist->is_zoomed.second)
		invalidate = true;
	curr_hist->is_zoomed.first = false;
	curr_hist->is_zoomed.second = false;
	curr_hist->x_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	curr_hist->y_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	if (invalidate)
		Invalidate(invHistogram);
	return true;
}

void CanvasSetups::set_log_x(bool is_log)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	curr_hist->logscale_x=is_log;
}
void CanvasSetups::set_log_y(bool is_log)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	bool invalidate = curr_hist->logscale_y!=is_log;
	curr_hist->logscale_y=is_log;
}
void CanvasSetups::set_log_z(bool is_log)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	bool invalidate = curr_hist->logscale_z!=is_log;
	curr_hist->logscale_z=is_log;
}

//public (interfaces)
void CanvasSetups::next_canvas(void)
{
	std::size_t prev_can = canvas_ind;
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		StateChange(current_channel, current_exp_index, current_type, canvas_ind, current_channel, current_exp_index, current_type, prev_can);
		return;
	}
	canvases_states[canvas_ind].channel = current_channel;
	canvases_states[canvas_ind].type = current_type;
	canvases_states[canvas_ind].exp_index = current_exp_index;
	stateS ind_new_state = canvases_states[canvas_ind], ind_prev_state = canvases_states[canvas_ind];
	if ((canvas_ind+1)==canvases.size()) {
		++canvas_ind;
		TCanvas* new_c = new TCanvas((std::string("inter_canvas_") + std::to_string(canvas_ind)).c_str(), (std::string("inter_canvas_") + std::to_string(canvas_ind)).c_str());
		canvases.push_back(new_c);
		stateS ind_state;
		ind_state.channel = current_channel;
		ind_state.exp_index = current_exp_index;
		ind_state.type = current_type;
		canvases_states.push_back(ind_state);
		new_c->cd();
		new_c->SetGrid();
		new_c->SetTicks();
		new_c->ToggleEventStatus();
		new_c->ToggleToolBar();
		fit_functions.push_back(NULL);
		hists_1D.push_back(NULL);
		hists_2D.push_back(NULL);

		//Copy all setups from previous canvas (cuts included, but their void* data is not deeply copied!)
		manual_setups.push_back(std::deque<std::deque<std::deque<HistogramSetups*>>>());
		for (std::size_t h = _first_state; h != ((std::size_t)_last_state + 1); ++h) {
			manual_setups[canvas_ind].push_back(std::deque<std::deque<HistogramSetups*>>());
			for (std::size_t exp = 0, exp_end_= manual_setups[prev_can][h].size(); exp != exp_end_; ++exp) {
				manual_setups[canvas_ind][h].push_back(std::deque<HistogramSetups*>());
				loop_channels_reset();
				for (int ch=0, ch_ind=0; loop_channels ((AStates::Type)h, ch, ch_ind); ) {
					HistogramSetups* setup_to_copy = manual_setups[prev_can][h][exp][ch_ind];
					setup_to_copy = (NULL==setup_to_copy ? NULL : new HistogramSetups(*setup_to_copy));
					manual_setups[canvas_ind][h][exp].push_back(setup_to_copy);
				}
			}
		}
		RunCuts.push_back(std::deque<std::deque<EventCut> >());
		RunCuts[canvas_ind] = RunCuts[prev_can];

	} else {
		++canvas_ind; //already created the canvas
		ind_new_state = canvases_states[canvas_ind];
		current_channel = ind_new_state.channel;
		current_exp_index = ind_new_state.exp_index;
		current_type = ind_new_state.type;
	}
	StateChange(ind_new_state.channel, ind_new_state.exp_index, ind_new_state.type, canvas_ind, ind_prev_state.channel, ind_prev_state.exp_index, ind_prev_state.type, prev_can);
}

void CanvasSetups::previous_canvas(void)
{
	std::size_t prev_can = canvas_ind;
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		StateChange(current_channel, current_exp_index, current_type, canvas_ind, current_channel, current_exp_index, current_type, prev_can);
		return;
	}
	canvases_states[canvas_ind].channel = current_channel;
	canvases_states[canvas_ind].type = current_type;
	canvases_states[canvas_ind].exp_index = current_exp_index;
	stateS ind_new_state = canvases_states[canvas_ind], ind_prev_state = canvases_states[canvas_ind];
	if (0==canvas_ind) {
		std::cout << "No previous canvas" << std::endl;
		return;
	} else {
		--canvas_ind;
		ind_new_state = canvases_states[canvas_ind];
		current_channel = ind_new_state.channel;
		current_exp_index = ind_new_state.exp_index;
		current_type = ind_new_state.type;
	}
	StateChange(ind_new_state.channel, ind_new_state.exp_index, ind_new_state.type, canvas_ind, ind_prev_state.channel, ind_prev_state.exp_index, ind_prev_state.type, prev_can);
}
