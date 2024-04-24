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
	use_default_setups(true), N_bins(0), N_bins_y(0), bins_logscale_x(false),
	bins_logscale_y(false), N_gauss(0), use_fit(false), extra_data(NULL),
	logscale_x(false), logscale_y(false), logscale_z(false), draw_option("hist"),
	stats_xs(gStyle->GetStatX() - gStyle->GetStatW(), gStyle->GetStatX()),
	stats_ys(gStyle->GetStatY() - gStyle->GetStatH(), gStyle->GetStatY()),
	draw_stats(true)
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
	bins_logscale_x = setups.bins_logscale_x;
	bins_logscale_y = setups.bins_logscale_y;
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
	draw_option = setups.draw_option;
	stats_xs = setups.stats_xs;
	stats_ys = setups.stats_ys;
	draw_stats = setups.draw_stats;

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
	if (isTH1Dhist(type)) {
		setups->draw_option = "hist";
	} else {
		setups->draw_option = "colz";
		if (type == MPPC_Npe_profile) {
			setups->draw_option = "lego";
		}
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
	hists_1D[canvas_ind] = (NULL==hist ? NULL : hist);
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
	hists_2D[canvas_ind] = (NULL==hist ? NULL : hist);
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

void CanvasSetups::set_log_bins_x(bool is_log, bool do_update)
{
	if (!is_log)
		unset_log_bins_x();
	HistogramSetups* setups = get_hist_setups();
	if (NULL == setups)
		return;
	bool invalidate = (setups->bins_logscale_x != true);
	setups->bins_logscale_x = true;
	if (invalidate)
		Invalidate(invHistogram);
	if (do_update)
		update();
}

void CanvasSetups::unset_log_bins_x(bool do_update)
{
	HistogramSetups* setups = get_hist_setups();
	if (NULL == setups)
		return;
	bool invalidate = (setups->bins_logscale_x != false);
	setups->bins_logscale_x = false;
	if (invalidate)
		Invalidate(invHistogram);
	if (do_update)
		update();
}

void CanvasSetups::set_log_bins_y(bool is_log, bool do_update)
{
	if (!is_log)
		unset_log_bins_y();
	HistogramSetups* setups = get_hist_setups();
	if (NULL == setups)
		return;
	bool invalidate = (setups->bins_logscale_y != true);
	setups->bins_logscale_y = true;
	if (invalidate)
		Invalidate(invHistogram);
	if (do_update)
		update();
}

void CanvasSetups::unset_log_bins_y(bool do_update)
{
	HistogramSetups* setups = get_hist_setups();
	if (NULL == setups)
		return;
	bool invalidate = (setups->bins_logscale_y != false);
	setups->bins_logscale_y = false;
	if (invalidate)
		Invalidate(invHistogram);
	if (do_update)
		update();
}

void CanvasSetups::set_log_bins_xy(bool is_log, bool do_update)
{
	set_log_bins_x(is_log, false);
	set_log_bins_y(is_log, false);
	if (do_update)
		update();
}

void CanvasSetups::unset_log_bins_xy(bool do_update)
{
	unset_log_bins_x(false);
	unset_log_bins_y(false);
	if (do_update)
		update();
}

void CanvasSetups::set_log_x(bool is_log)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	curr_hist->logscale_x=is_log;
	update();
}

void CanvasSetups::set_log_y(bool is_log)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	bool invalidate = curr_hist->logscale_y!=is_log;
	curr_hist->logscale_y=is_log;
	update();
}

void CanvasSetups::set_log_z(bool is_log)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	bool invalidate = curr_hist->logscale_z!=is_log;
	curr_hist->logscale_z=is_log;
	update();
}

void CanvasSetups::set_draw_option(std::string option)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	bool invalidate = curr_hist->draw_option!=option;
	curr_hist->draw_option=option;
	update();
}

void CanvasSetups::set_hist_stats(const std::string& location)
{
	double x1, x2, y1, y2;
	std::string loc = location;
	std::transform(loc.begin(), loc.end(), loc.begin(),
    		[](unsigned char c){ return std::tolower(c); });
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL == curr_hist) {
		std::cerr << "CanvasSetups::set_hist_stats:Error: NULL histogram setups" << std::endl;
		return;
	}
	if (loc == "tl" || loc == "lt") {
		x1 = 0.1;
		x2 = x1 + curr_hist->stats_xs.second - curr_hist->stats_xs.first;
		y2 = 0.9;
		y1 = y2 - curr_hist->stats_ys.second + curr_hist->stats_ys.first;
	} else if (loc == "tr" || loc == "rt") {
		x2 = 0.9;
		x1 = x2 - curr_hist->stats_xs.second + curr_hist->stats_xs.first;
		y2 = 0.9;
		y1 = y2 - curr_hist->stats_ys.second + curr_hist->stats_ys.first;
	} else if (loc == "br" || loc == "rb") {
		x2 = 0.9;
		x1 = x2 - curr_hist->stats_xs.second + curr_hist->stats_xs.first;
		y1 = 0.1;
		y2 = y1 + curr_hist->stats_ys.second - curr_hist->stats_ys.first;
	} else if (loc == "bl" || loc == "lb") {
		x1 = 0.1;
		x2 = x1 + curr_hist->stats_xs.second - curr_hist->stats_xs.first;
		y1 = 0.1;
		y2 = y1 + curr_hist->stats_ys.second - curr_hist->stats_ys.first;
	} else {
		std::cout << __PRETTY_FUNCTION__ <<": option \"" << location << "\" is not suppoted.\n";
		std::cout << "Available options are \"tl\", \"tr\", \"br\" and \"bl\"." << std::endl;
		return;
	}
	curr_hist->stats_xs = std::make_pair(x1, x2);
	curr_hist->stats_ys = std::make_pair(y1, y2);
	update();
}

void CanvasSetups::set_hist_stats(double x, double y)
{
	double x1 = x, x2, y1 = y, y2;
	HistogramSetups * setups = get_hist_setups();
	if (NULL == setups) {
		std::cerr << "CanvasSetups::set_hist_stats:Error: NULL histogram setups" << std::endl;
		return;
	}
	x2 = x1 + setups->stats_xs.second - setups->stats_xs.first;
	y2 = y1 + setups->stats_ys.second - setups->stats_ys.first;
	setups->stats_xs = std::make_pair(x1, x2);
	setups->stats_ys = std::make_pair(y1, y2);
	update();
}

void CanvasSetups::set_hist_stats(bool on)
{
	HistogramSetups * setups = get_hist_setups();
	if (NULL == setups) {
		std::cerr << "CanvasSetups::set_hist_stats:Error: NULL histogram setups" << std::endl;
		return;
	}
	setups->draw_stats = on;
	update();
}

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

std::string CanvasSetups::hist_name(void) const
{
	std::string name;
	if (name_scheme_version == name_scheme_v1) {
		name = (isPMTtype(current_type) ? "PMT#" : "MPPC#");
	}
	if (name_scheme_version == name_scheme_v2) {
		name = (isPMTtype(current_type) ? DATA_PMT_VERSION + "#" : DATA_MPPC_VERSION + "#");
	}
	name += (isMultichannel(current_type) ? "All" : std::to_string(current_channel) )
				+ "_" + g_data->exp_area.experiments[current_exp_index];
	name += "_" + type_name(current_type);
	return name;
}

std::pair<double, double> CanvasSetups::hist_y_limits(bool consider_displayed_cuts) //valid only for 2d plots
{
	std::pair<double, double> ret(DBL_MAX, -DBL_MAX);
	HistogramSetups* setups = get_hist_setups(current_exp_index, current_channel, current_type);
	if (NULL == setups)
		return ret;
	if (!consider_displayed_cuts) {
		if (setups->y_lims)
			return *setups->y_lims;
		return ret;
	} else {
		if (setups->y_drawn_lims)
			return *setups->y_drawn_lims;
		return ret;
	}
	return ret;
}

std::pair<double, double> CanvasSetups::hist_x_limits(bool consider_displayed_cuts) //considering cuts
{
	std::pair<double, double> ret(DBL_MAX, -DBL_MAX);
	HistogramSetups* setups = get_hist_setups(current_exp_index, current_channel, current_type);
	if (NULL == setups)
		return ret;
	if (!consider_displayed_cuts) {
		if (setups->x_lims)
			return *setups->x_lims;
		return ret;
	} else {
		if (setups->x_drawn_lims)
			return *setups->x_drawn_lims;
		return ret;
	}
	return ret;
}

CanvasSetups::bins_t_1d CanvasSetups::create_bins(int N_bins, double min_v, double max_v, bool logscale) const
{
	if (logscale) {
		if (min_v == max_v)
			return auto_bins_t(N_bins, min_v, max_v);
		if (max_v <= 0.0) {
			custom_bins_t bins = std::get<custom_bins_t>(create_bins(N_bins, -max_v, -min_v, true));
			for (int i=0; i<=(std::get<0>(bins)+1); ++i)
				std::get<1>(bins)[i] *= -1;
			return bins;
		}
		double* bins = new double[N_bins+1];
   		double log_max_v = std::log(max_v);
		if (min_v <= 0)
			min_v = std::exp(log_max_v / (N_bins+1));
		double log_min_v = std::log(min_v);
		double dx = (log_max_v - log_min_v) / N_bins;
		for (int i=0; i <= N_bins; ++i)
			bins[i] = std::exp(log_min_v + i*dx);
		return custom_bins_t(N_bins, bins);
	} else {
		return auto_bins_t(N_bins, min_v, max_v);
	}
}

bool CanvasSetups::prepare_histogram(void)
{
	HistogramSetups* setups = get_hist_setups();
	if (NULL == setups) {
		std::cerr<<__PRETTY_FUNCTION__<<": Error: NULL histogram setups."<<std::endl;
		return false;
	}
	TCanvas* canvas = get_current_canvas();
	if (NULL==canvas) {
		std::cerr<<__PRETTY_FUNCTION__<<": Error: NULL canvas."<<std::endl;
		return false;
	}

	if (!setups->filled_hist) {
		canvas->cd();
		canvas->SetTitle(hist_name().c_str());
		canvas->Clear();
		std::pair<double, double> x_lims = hist_x_limits();
		x_lims.second+=(x_lims.second-x_lims.first)/setups->N_bins;
		if (isTH1Dhist(current_type)) {
			TH1D* hist = get_current_hist1();
			if (NULL == hist) {
				hist = new TH1D();
				set_hist1(hist);
			}
			hist->SetTitle(hist_name().c_str());
			hist->Reset("M");
			bins_t bins = create_bins(setups->N_bins,
					(is_zoomed().first ? get_current_x_zoom().first : x_lims.first),
					(is_zoomed().first ? get_current_x_zoom().second : x_lims.second),
					setups->bins_logscale_x);
			SetHistogramBins(hist, bins);
		} else {
			std::pair<double, double> y_lims = hist_y_limits();
			TH2D* hist = get_current_hist2();
			if (NULL == hist) {
				hist = new TH2D();
				set_hist2(hist);
			}
			hist->SetTitle(hist_name().c_str());
			hist->Reset("M");
			bins_t_1d bins_x = create_bins(setups->N_bins,
					(is_zoomed().first ? get_current_x_zoom().first : x_lims.first),
					(is_zoomed().first ? get_current_x_zoom().second : x_lims.second),
					setups->bins_logscale_x);
			bins_t_1d bins_y = create_bins(setups->N_bins_y,
					(is_zoomed().first ? get_current_y_zoom().first : y_lims.first),
					(is_zoomed().first ? get_current_y_zoom().second : y_lims.second),
					setups->bins_logscale_y);
			bins_t bins = std::make_pair(bins_x, bins_y);
			SetHistogramBins(hist, bins);
		}
	}
	return true;
}

bool CanvasSetups::fit_and_draw_histogram(void)
{
	HistogramSetups* setups = get_hist_setups();
	if (NULL == setups) {
		std::cerr<<__PRETTY_FUNCTION__<<": Error: NULL histogram setups."<<std::endl;
		return false;
	}
	TCanvas* canvas = get_current_canvas();
	if (NULL == canvas) {
		std::cerr<<__PRETTY_FUNCTION__<<": Error: NULL canvas."<<std::endl;
		return false;
	}
	
	if (isTH1Dhist(current_type)) {
		if (!setups->is_valid_fit_function && setups->use_fit) {
			std::pair<double, double> x_drawn_lims = hist_x_limits(true);
			TF1* ff = create_fit_function(setups, x_drawn_lims);
			set_fit_function(ff); //creates internal copy, hence ff->Delete()
			if (NULL!=ff)
				ff->Delete();
			setups->is_valid_fit_function = true;
		}
		if (!setups->fitted && setups->use_fit) {
			TH1D* hist = get_current_hist1();
			TF1* ff = get_current_fit_function();
			if (NULL != ff && NULL!=hist) {
				hist->Fit(ff, "RQ");
				setups->fitted = kTRUE;
				for (std::size_t par = 0; par < setups->par_val.size(); ++par)
					setups->par_val[par] = ff->GetParameter(par);
			}
			setups->fitted = true;
		}
	}

	if (NULL!=canvas) {
		if (isTH1Dhist(current_type)) {
			TH1D* hist = get_current_hist1();
			if (hist) {
				hist->GetXaxis()->SetTitle(setups->x_axis_title.c_str());
				hist->GetYaxis()->SetTitle(setups->y_axis_title.c_str());
				hist->Draw(setups->draw_option.c_str());
			}
		} else { 
			TH2D* hist = get_current_hist2();
			if (hist) {
				hist->GetXaxis()->SetTitle(setups->x_axis_title.c_str());
				hist->GetYaxis()->SetTitle(setups->y_axis_title.c_str());
				hist->Draw(setups->draw_option.c_str()/*"lego"*/);
			}
		}
		canvas->Update(); // required for updates axes which are used in drawing cuts
		TH1* hist = isTH1Dhist(current_type) ? (TH1*)get_current_hist1() : (TH1*)get_current_hist2();
		TPaveStats* st = hist ? (TPaveStats*)hist->FindObject("stats") : nullptr;
		if (!st) { // ROOT quirk. Need update once more.
			hist->Draw(setups->draw_option.c_str());
			canvas->Update();
			st = hist ? (TPaveStats*)hist->FindObject("stats") : nullptr;
		}
		if (st) {
			st->SetX1NDC(setups->stats_xs.first);
			st->SetX2NDC(setups->stats_xs.second);
			st->SetY1NDC(setups->stats_ys.first);
			st->SetY2NDC(setups->stats_ys.second);
		}
		TF1* ff = get_current_fit_function();
		if (ff && setups->fitted)
			ff->Draw("same");
		for (auto cut = setups->hist_cuts.begin(), cut_end_ = setups->hist_cuts.end(); cut!=cut_end_; ++cut) {
			if (!cut->GetAffectingHistogram()) // no point in drawing cuts that were already applied to histogram
				cut->Draw(canvas);
		}
		canvas->Update();
		TVirtualPad* pad = canvas->cd();
		pad->SetLogx(setups->logscale_x ? 1 : 0);
		pad->SetLogy(setups->logscale_y ? 1 : 0);
		pad->SetLogz(setups->logscale_z ? 1 : 0);
	}
	return true;
}

void CanvasSetups::off_ch(int channel)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL == setups) {
		std::cout << "CanvasSetups::off_ch: Error: NULL setups" << std::endl;
	}
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	bool *active = setups->active_channels.info(channel);
	if (active == NULL) {
		std::cout << "CanvasSetups::off_ch: Error: no such channel ("<<channel<<") for current type" << std::endl;
		return;
	}
	if (*active == true) {
		*active = false;
		Invalidate(invCuts);
	}
	if (!isMultichannel(current_type))
		update();
}

void CanvasSetups::on_ch(int channel)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL == setups) {
		std::cout << "CanvasSetups::on_ch: Error: NULL setups" << std::endl;
	}
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	bool *active = setups->active_channels.info(channel);
	if (active == NULL) {
		std::cout << "CanvasSetups::on_ch: Error: no such channel (" << channel << ") for current type" << std::endl;
		return;
	}
	if (*active == false) {
		*active = true;
		Invalidate(invCuts);
	}
	if (!isMultichannel(current_type))
		update();
}

int CanvasSetups::list_hist_cuts(void)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL == setups) {
		std::cout << "CanvasSetups::list_hist_cuts: Error: NULL setups" << std::endl;
		return 0;
	}
	std::cout << "\tActive channels ["<< setups->active_channels.size() << "]: " << std::endl;
	for (std::size_t ind =0, ind_end_ = setups->active_channels.size(); ind != ind_end_; ++ind)
		std::cout << setups->active_channels.channel(ind) << " " << (setups->active_channels[ind] ? "on  " : "off") << (((ind == (ind_end_ - 1)) ? "" : " | "));
	std::cout << std::endl;
	std::cout << "\tHistogram cuts [" << setups->hist_cuts.size() << "]: \"name\":channel# | "<<std::endl;
	for (auto i = setups->hist_cuts.begin(), _end_ = setups->hist_cuts.end(); i != _end_; ++i)
		std::cout <<"\""<< i->GetName()<<"\"" << (i->GetAffectingHistogram() ? ":" : "(Only shown):")<<i->GetChannel() << (((i == (_end_ - 1)) ? "" : " | "));
	std::cout << std::endl;
	return setups->hist_cuts.size();
}

int CanvasSetups::list_run_cuts(void)
{
	std::deque<EventCut> *RunCuts = get_run_cuts(current_exp_index);
	if (NULL == RunCuts) {
		std::cout << "CanvasSetups::list_run_cuts: Error: NULL RunCuts" << std::endl;
		return 0;
	}
	std::cout << "RunCuts [" << RunCuts->size() << "]: \"name\":number_of_rejected_events |"<<std::endl;
	for (auto i = RunCuts->begin(), _end_ = RunCuts->end(); i != _end_; ++i)
		std::cout << (i->GetName()) <<":"<<i->GetRejectedN() << ((i == (_end_ - 1)) ? "" : " | ");
	std::cout << std::endl;
	return RunCuts->size();
}

void CanvasSetups::set_N_bins(int N)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::set_N_bins: Error: NULL setups"<<std::endl;
	}
	if (setups->N_bins!=std::max(N, 1) || setups->N_bins_y!=std::max(N, 1)) {
		setups->N_bins = std::max(N, 1);
		setups->N_bins_y = std::max(N, 1);
		Invalidate(invHistogram);
	}
	update();
}

void CanvasSetups::set_N_bins(int from, int to)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::set_N_bins: Error: NULL setups"<<std::endl;
	}
	if (isTH1Dhist(current_type)) {
		if (setups->N_bins!=std::max(std::abs(to-from), 1)) {
			setups->N_bins = std::max(std::abs(to-from), 1);
			Invalidate(invHistogram);
		}
		std::pair<double, double> x_zoom(from, to), y_zoom(-DBL_MAX, DBL_MAX);
		set_zoom(x_zoom, y_zoom);
	} else {
		if (setups->N_bins!=std::max(from, 1)) {
			setups->N_bins = std::max(from, 1);
			Invalidate(invHistogram);
		}
		if (setups->N_bins_y!=std::max(to, 1)) {
			setups->N_bins_y = std::max(to, 1);
			Invalidate(invHistogram);
		}
	}
	update();
}

void CanvasSetups::set_zoom (double xl, double xr)
{
	std::pair<double, double> x_zoom, y_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	x_zoom.first = std::min(xl, xr);
	x_zoom.second = std::max(xl, xr);
	set_zoom(x_zoom, y_zoom);
	update();
}

void CanvasSetups::set_zoom_y (double yl, double yr)
{
	if (isTH1Dhist(current_type)){
		std::cout<<"Can't set y zoom for TH1D histogram"<<std::endl;
		return;
	}
	std::pair<double, double> y_zoom, x_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	y_zoom.first = std::min(yl, yr);
	y_zoom.second = std::max(yl, yr);
	set_zoom(x_zoom, y_zoom);
	update();
}

void CanvasSetups::set_zoom (double xl, double xr, double yl, double yr)
{
	std::pair<double, double> y_zoom, x_zoom;
	x_zoom.first = std::min(xl, xr);
	x_zoom.second = std::max(xl, xr);
	y_zoom.first = std::min(yl, yr);
	y_zoom.second = std::max(yl, yr);
	set_zoom(x_zoom, y_zoom);
	update();
}

void CanvasSetups::unset_zoom(bool do_update)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL==curr_hist)
		return;
	bool invalidate = false;
	if (curr_hist->is_zoomed.first || curr_hist->is_zoomed.second)
		invalidate = true;
	curr_hist->is_zoomed.first = false;
	curr_hist->is_zoomed.second = false;
	curr_hist->x_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	curr_hist->y_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	if (invalidate)
		Invalidate(invHistogram);
	if (do_update)
		update();
}

bool CanvasSetups::set_X_title(std::string text)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL == curr_hist)
		return false;
	curr_hist->x_axis_title = text;
	update();
	return true;
}

bool CanvasSetups::set_Y_title(std::string text)
{
	HistogramSetups* curr_hist = get_hist_setups();
	if (NULL == curr_hist)
		return false;
	curr_hist->y_axis_title = text;
	update();
	return true;
}

void CanvasSetups::add_hist_cut(FunctionWrapper* picker, std::string name, bool affect_hist)
{
	if (isMultichannel(current_type)){
		std::cout << "CanvasSetups::add_hist_cut(FunctionWrapper*, std::string) Error: for type '"<<type_name(current_type)<<"' channel must be specified."<<std::endl;
		std::cout << "\t Use add_hist_cut(FunctionWrapper*, std::string, int channel) instead."<<std::endl;
		return;
	}
	add_hist_cut(picker, name, isComposite(current_type) ? -1 : current_channel, affect_hist);
}

void CanvasSetups::add_hist_cut(FunctionWrapper* picker, std::string name, int channel, bool affect_hist)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::add_hist_cut: Error: NULL setups"<<std::endl;
		return;
	}
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (!isComposite(current_type)&&!isMultichannel(current_type)&&(channel!=current_channel || -1==channel)) {
		std::cout<<"CanvasSetups::add_hist_cut(FunctionWrapper*, std::string, int, bool) Warning: No such channel for type '"<<type_name(current_type)<<"'. Channel set to "<<current_channel<<std::endl;
		channel = current_channel;
	}
	if (-1!=channel) {
		if (isPMTtype(current_type)) {
			int ch_ind = pmt_channel_to_index(channel);
			if (ch_ind<0) {
				std::cout<<"CanvasSetups::add_hist_cut(FunctionWrapper*, std::string, int, bool) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
				std::cout<<"\tAvailable channels: -1 (for top level cut)";
				if (0!=PMT_channels.size()) {
					std::cout<<", ";
					for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
						std::cout<<*i<<(((i+1) ==_end_) ?"" : ", ");
				}
				std::cout<<std::endl;
				return;
			}
		} else {
			int ch_ind = mppc_channel_to_index(channel);
			if (ch_ind<0) {
				std::cout<<"CanvasSetups::add_hist_cut(FunctionWrapper*, std::string, int) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
				std::cout<<"\tAvailable channels: -1 (for top level cut)";
				if (0!=MPPC_channels.size()) {
					std::cout<<", ";
					for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
						std::cout<<*i<<(((i+1) ==_end_) ?"" : ", ");
				}
				std::cout<<std::endl;
				return;
			}
		}
	}
	EventCut *found_cut = NULL;
	for (auto i = setups->hist_cuts.begin(), i_end_ = setups->hist_cuts.end(); i!=i_end_; ++i){
		if ((i->GetName() == name) && (i->GetChannel() == channel)){
			found_cut = &(*i);
			break;
		}
	}
	bool replacing_cut = true;
	if (NULL==found_cut) {
		replacing_cut = false;
		setups->hist_cuts.push_back(EventCut(0, EventCut::HistCut, name));
		found_cut = &(setups->hist_cuts.back());
	}
	found_cut->SetPicker(picker);
	found_cut->SetChannel(channel);
	int top_level_channel = (isComposite(current_type) || isMultichannel(current_type)) ? -1 : current_channel;
	if ((channel!=top_level_channel)&&!affect_hist) {
		std::cout<<"CanvasSetups::add_hist_cut(FunctionWrapper*, std::string, int) Warning: Cut for such channel and type '"<<type_name(current_type)<<"' can't be drawn."<<std::endl;
	}
	affect_hist = affect_hist || channel!=top_level_channel;
	if (replacing_cut && found_cut->GetAffectingHistogram())
		Invalidate(invCuts);
	if (replacing_cut && !found_cut->GetAffectingHistogram())
		Invalidate(invDisplayedCuts);
	found_cut->SetAffectingHistogram(affect_hist);
	if (affect_hist)
		Invalidate(invCuts);
	else
		Invalidate(invDisplayedCuts);
}

void CanvasSetups::remove_hist_cut(int index)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::remove_hist_cut: Error: NULL setups"<<std::endl;
		return;
	}
	if (index<0 || index >= setups->hist_cuts.size()) {
		std::cout<<"CanvasSetups::remove_hist_cut(int) Error: cut index is out of range"<<std::endl;
		return;
	}
	bool affect_hist = (setups->hist_cuts.begin() + index)->GetAffectingHistogram();
	setups->hist_cuts.erase(setups->hist_cuts.begin() + index);
	if (affect_hist)
		Invalidate(invCuts);
	else
		Invalidate(invDisplayedCuts);
}

void CanvasSetups::remove_hist_cut(std::string name)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::remove_hist_cut: Error: NULL setups"<<std::endl;
		return;
	}
	bool affect_hist_cut = false, not_affect_hist_cut = false;
	while (true) {
		for (auto i = setups->hist_cuts.begin(); i != setups->hist_cuts.end(); ++i) {
			if (i->GetName() == name){
				if (i->GetAffectingHistogram())
					affect_hist_cut = true;
				else
					not_affect_hist_cut = true;
				setups->hist_cuts.erase(i);
				goto anew;
			}
		}
		if (affect_hist_cut)
			Invalidate(invCuts);
		if (not_affect_hist_cut)
			Invalidate(invDisplayedCuts);
		break;
		anew:;
	}
}

void CanvasSetups::remove_hist_cut(std::string name, int ch)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (!isComposite(current_type)&&!isMultichannel(current_type)&&(ch!=current_channel || -1==ch)) {
		std::cout<<"CanvasSetups::remove_hist_cut(FunctionWrapper*, std::string, int, bool) Warning: No such channel for type '"<<type_name(current_type)<<"'. Channel set to "<<current_channel<<std::endl;
		ch = current_channel;
	}
	if (-1!=ch) {
		if (isPMTtype(current_type)) {
			int ch_ind = pmt_channel_to_index(ch);
			if (ch_ind<0) {
				std::cout<<"CanvasSetups::remove_hist_cut(FunctionWrapper*, std::string, int, bool) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
				std::cout<<"\tAvailable channels: -1 (for top level cut)";
				if (0!=PMT_channels.size()) {
					std::cout<<", ";
					for (auto i = PMT_channels.begin(), _end_ = PMT_channels.end(); i != _end_; ++i)
						std::cout<<*i<<(((i+1) ==_end_) ?"" : ", ");
				}
				std::cout<<std::endl;
				return;
			}
		} else {
			int ch_ind = mppc_channel_to_index(ch);
			if (ch_ind<0) {
				std::cout<<"CanvasSetups::add_hist_cut(FunctionWrapper*, std::string, int) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
				std::cout<<"\tAvailable channels: -1 (for top level cut)";
				if (0!=MPPC_channels.size()) {
					std::cout<<", ";
					for (auto i = MPPC_channels.begin(), _end_ = MPPC_channels.end(); i != _end_; ++i)
						std::cout<<*i<<(((i+1) ==_end_) ?"" : ", ");
				}
				std::cout<<std::endl;
				return;
			}
		}
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::remove_hist_cut: Error: NULL setups"<<std::endl;
	}
	while (true) {
		for (auto i = setups->hist_cuts.begin(); i != setups->hist_cuts.end(); ++i) {
			if ((i->GetName() == name)&&(i->GetChannel()==ch)){
				setups->hist_cuts.erase(i);
				goto anew;
			}
		}
		Invalidate(invCuts);
		break;
		anew:;
	}
}

void CanvasSetups::set_fit_gauss(int N)
{
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::set_fit_gauss: Error: NULL setups"<<std::endl;
		return;
	}
	N = std::max(N, 0);
	int was_N = setups->N_gauss;
	std::pair<double, double> x_lims = hist_x_limits();
	setups->N_gauss = N;
	setups->par_val.resize(setups->N_gauss * 3);
	setups->par_left_limits.resize(setups->N_gauss * 3);
	setups->par_right_limits.resize(setups->N_gauss * 3);
	if (was_N < N){
		int _N_in_hist = numOfFills(false);
		for (int nn = was_N; nn != setups->N_gauss; ++nn){
			setups->par_left_limits[nn] = 0;
			setups->par_right_limits[nn] = std::max(1, 2 * (int)std::sqrt(_N_in_hist));
			setups->par_val[nn] = (int)std::sqrt(_N_in_hist);

			setups->par_left_limits[nn + 1] = x_lims.first;
			setups->par_right_limits[nn + 1] = x_lims.second;
			setups->par_val[nn + 1] = 0.5*(setups->par_left_limits[1] + setups->par_right_limits[1]);

			setups->par_left_limits[nn + 2] = (x_lims.second - x_lims.first) / setups->N_bins;
			setups->par_right_limits[nn + 2] = (setups->par_right_limits[1] - setups->par_left_limits[1]);
			setups->par_val[nn + 2] = 0.5*(setups->par_left_limits[2] + setups->par_right_limits[2]);
		}
	}
	Invalidate(invFit|invFitFunction);
	update();
}

void CanvasSetups::set_parameter_val(int index, double val)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::set_parameter_val: Error: NULL setups"<<std::endl;
		return;
	}
	if ((index < 0) || (index >= 3 * setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	setups->par_val[index] = val;
	Invalidate(invFit|invFitFunction);
	update();
}

void CanvasSetups::set_parameter_limits(int index, double left, double right)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"CanvasSetups::set_parameter_limits: Error: NULL setups"<<std::endl;
		return;
	}
	if ((index < 0) || (index >= 3 * setups->N_gauss)) {
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	setups->par_left_limits[index] = std::min(left, right);
	setups->par_right_limits[index] = std::max(left, right);
	Invalidate(invFit|invFitFunction);
	update();
}

std::size_t CanvasSetups::numOfFills(bool consider_displayed_cuts)
{
	std::size_t ret = 0;
	HistogramSetups* setups = get_hist_setups(current_exp_index, current_channel, current_type);
	if (NULL == setups) {
		std::cerr<<"CanvasSetups::numOfFills: Error: NULL histogram setups"<<std::endl;
		return ret;
	}
	if (!consider_displayed_cuts) {
		if (setups->num_of_fills)
			return *setups->num_of_fills;
		return ret;
	} else {
		if (setups->num_of_drawn_fills)
			return *setups->num_of_drawn_fills;
		return ret;
	}
	return ret;
}
