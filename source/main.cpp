#include "main.h"
//interactive methods (interface)
void Initialize(bool pars_in_script)
{
	Init_globals(!pars_in_script);
	std::cout << "This: " <<this_path << std::endl;
	manager = new AnalysisManager(exp_area);
	manager->processAllExperiments();
	g_data = manager->getAllExperimentsResults();
	post_processor = new PostProcessor(g_data);
	gr_manager = new GraphicOutputManager();

}

void GenTest(std::string prefix)
{
	std::cout<<"Generating tests..."<<std::endl;
	TestSignalGenerator gen(prefix);
	std::cout<<"Tests are generated."<<std::endl;
}

//Go to channel. Does not save previous hist in file but does save parameters such as cuts
void ch(int ch)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (post_processor->isMultichannel(post_processor->current_type)) {
		int pmt_ind = post_processor->pmt_channel_to_index(ch);
		int mppc_ind = post_processor->mppc_channel_to_index(ch);
		if (pmt_ind>=0 && post_processor->is_PMT_type(post_processor->current_type)) {
			for (auto i = post_processor->PMT_channels.begin(), i_end_ = post_processor->PMT_channels.end(); i!=i_end_; ++i)
				off_ch(*i);
			on_ch(ch);
			std::cout<<"Only "<<ch<<" ch is turned on"<<std::endl;
			update();
			return;
		}
		if (mppc_ind>=0 && ! post_processor->is_PMT_type(post_processor->current_type)) {
			for (auto i = post_processor->MPPC_channels.begin(), i_end_ = post_processor->MPPC_channels.end(); i!=i_end_; ++i)
				off_ch(*i);
			on_ch(ch);
			std::cout<<"Only "<<ch<<" ch is turned on"<<std::endl;
			update();
			return;
		}
		std::cout<<"Error: no such channel for either PMT or MPPC"<<std::endl;
		return;
	}
	post_processor->GotoCh(ch);
}

void ty(AStates::Type to_type)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->GotoT(to_type);
}

void nex(void) //next experiment
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->NextExp();
}

void pex(void) //previous experiment
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->PrevExp();
}

void nch(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->NextCh();
}

void pch(void) //previous channel;
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->PrevCh();
}

void update(void)
{
	if (NULL == post_processor) {
		state(kFALSE);
		return;
	}
	post_processor->update();
}

void saveas(std::string path) //"" - use default path: "Data/results/{PMT_v1|MPPC_v1}/experiment/{PMT_|MPPC_}ch/{pic&data}
{
	if (NULL == post_processor) {
		state(kFALSE);
		return;
	}
	post_processor->saveAs(path);
}

void calib_status(Bool_t uncalibrated_only)
{
	if (NULL == post_processor) {
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.status(!uncalibrated_only);
}

void status(Bool_t full) //displays current state
{
	state(full);
}

void state(Bool_t full) //same as status(Bool_t)
{
	if (NULL == manager) {
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	if (NULL == g_data) {
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	if (NULL == post_processor) {
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	post_processor->status(full);
}

void set_fit_gauss(int N)
{
	if (NULL == post_processor) {
		state(kFALSE);
		return;
	}
	post_processor->set_fit_gauss(N);
}

void set_parameter_val(int index, double val)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_parameter_val(index, val);
}

void set_parameter_limits(int index, double left, double right)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_parameter_limits(index, left, right);
}

void do_fit(bool is_on) //always updates visuals
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->do_fit(is_on);
}

void set_bins(int n)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_N_bins(n);
}

void set_bins(int from, int to)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_N_bins(from, to);
}

void set_zoom (double xl, double xr)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_zoom(xl, xr);
}

void set_zoom_y (double yl, double yr)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_zoom_y(yl, yr);
}

void set_zoom (double xl, double xr, double yl, double yr)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_zoom(xl, xr, yl, yr);
}

void unset_zoom(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->unset_zoom();
}

void next_canvas(void) //creates new canvas or goes to the next existing. The current one will stay unchanged.
//Have independent cuts but the new one inherits the previous ones
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->next_canvas();
}

void prev_canvas(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->previous_canvas();
}

void set_corr (AStates::Type x_t, AStates::Type y_t, int chx, int chy)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->SetCorrelation(x_t, y_t, chx, chy);
}

void add_hist_cut(FunctionWrapper *picker, std::string name, int ch, bool draw)
//^If draw==false it shows cut via DrawCut method of FunctionWrapper
//^and does not change histogram itself.
//^In case there is no draw method do_replot is forced to true and warning is issued.
//^In case "name" is present in existing cut list the old cut is replaced.
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->add_hist_cut(picker, name, ch, !draw);
}

void add_hist_cut(FunctionWrapper *picker, std::string name, bool draw)
//same as above but ch is set automatically if possible.
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->add_hist_cut(picker, name, !draw);
}

int list_hist_cuts (void)//returns number of cuts
{
	if (NULL == g_data) {
		state(kFALSE);
		return 0;
	}
	return post_processor->list_hist_cuts();
}

void remove_hist_cut(int index)
//if index is larger than number of cuts the last cut is removed (popped)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->remove_hist_cut(index);
}

void remove_hist_cut(std::string name, int ch)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->remove_hist_cut(name, ch);
}

void remove_hist_cut(std::string name) //same as above but for each channel possible
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->remove_hist_cut(name);
}

void set_as_run_cut(std::string name)
//^use all cuts for current histogram as run cut. In ambiguous cases such as cuts on S_t
//^if not a single peak is accepted in the run then the run is excluded.
//^In general such cuts are useless so the implementation of this case in not important.
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->set_as_run_cut(name);
}

int list_run_cuts (void) //returns number of cuts
{
	if (NULL == g_data) {
		state(kFALSE);
		return 0;
	}
	return post_processor->list_run_cuts();
}

void unset_as_run_cut(std::string name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->unset_as_run_cut(name);
}

void print_accepted_events (std::string file, int run_offset, int sub_runs)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->print_accepted_events(file, run_offset, sub_runs);
}

void clear(void)	//clear cuts for current histogram. Run cuts derived from it are not touched
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->clear();
}

void clearAll(void) //clear everything, return to initial state (leaves all existing histograms empty)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->clearAll();
}

//Calibration methods:
void draw_Npe(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->plot_N_pe(post_processor->current_channel, gr_manager);
}

void add_1peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_channel, post_processor->current_exp_index);
	if (prev_method == CalibrationInfo::Using2pe)
		prev_method = CalibrationInfo::Using1pe2pe;
	else
		prev_method = CalibrationInfo::Using1pe;
	post_processor->calibr_info.set_method(post_processor->current_channel, post_processor->current_exp_index, prev_method);
}

void add_2peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_channel, post_processor->current_exp_index);
	if (prev_method == CalibrationInfo::Using1pe)
		prev_method = CalibrationInfo::Using1pe2pe;
	else
		prev_method = CalibrationInfo::Using2pe;
	post_processor->calibr_info.set_method(post_processor->current_channel, post_processor->current_exp_index, prev_method);
}

void rem_1peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_channel, post_processor->current_exp_index);
	if (prev_method == CalibrationInfo::Using1pe2pe)
		prev_method = CalibrationInfo::Using2pe;
	if ((prev_method == CalibrationInfo::Using1pe) || (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_channel, post_processor->current_exp_index, prev_method);
}

void rem_2peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_channel, post_processor->current_exp_index);
	if (prev_method == CalibrationInfo::Using1pe2pe)
		prev_method = CalibrationInfo::Using1pe;
	if ((prev_method == CalibrationInfo::Using2pe) || (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_channel, post_processor->current_exp_index, prev_method);
}

void set_1peS(double val)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (post_processor->isMultichannel(post_processor->current_type)) {
		std::cerr << "Can't set s1pe for multichannel type" << std::endl;
		return;
	}
	double V = 0;
	if (post_processor->is_PMT_type(post_processor->current_type)) {
		auto entry = PMT_V.find(post_processor->experiments[post_processor->current_exp_index]);
		if (entry != PMT_V.end())
			V = entry->second;
	} else {
		auto entry = MPPC_V.find(post_processor->experiments[post_processor->current_exp_index]);
		if (entry != MPPC_V.end())
			V = entry->second;
	}
	if (V!=0)
		post_processor->calibr_info.force_S1pe(post_processor->current_channel, V, val);
	else
		std::cerr << "Can't find voltage for current experiment and channel" << std::endl;
}

void unset_1peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (post_processor->isMultichannel(post_processor->current_type)) {
		std::cerr << "Can't set s1pe for multichannel type" << std::endl;
		return;
	}
	double V = 0;
	if (post_processor->is_PMT_type(post_processor->current_type)) {
		auto entry = PMT_V.find(post_processor->experiments[post_processor->current_exp_index]);
		if (entry != PMT_V.end())
			V = entry->second;
	} else {
		auto entry = MPPC_V.find(post_processor->experiments[post_processor->current_exp_index]);
		if (entry != MPPC_V.end())
			V = entry->second;
	}
	if (V != 0)
		post_processor->calibr_info.unforce_S1pe(post_processor->current_channel, V);
	else
		std::cerr << "Can't find voltage for current experiment and channel" << std::endl;
}

void set_use_mean(Bool_t do_use) //uses mean value of data instead of gauss' mean. May be usefull for S2_S
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_channel, post_processor->current_exp_index);
	if (do_use)
		prev_method = CalibrationInfo::UsingMean;
	if ((!do_use) && (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_channel, post_processor->current_exp_index, prev_method);
	update();
}

void calib_load(std::string fname) //if fname=="" uses file location from global parameters
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (fname == "")
		fname = calibration_file;
	post_processor->calibr_info.Load(fname);
}

void calib_save(std::string fname)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (fname == "")
		fname = calibration_file;
	post_processor->calibr_info.Save(fname); 
}

//TODO: remove?
void set_calib_N(int from, int to)//in order to set default use invalid values
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.set_N_calib(post_processor->current_channel, from, to);
	post_processor->update();
}

void Exit(Bool_t save)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (save)
		post_processor->save_all();
}

FunctionWrapper* create_vertical_lines_cut(double left, double right) //do not call from the CINT
{
	struct temp_data {
		std::pair<double,double> mm;
		int ch_size;
	};
	temp_data * st_data = new temp_data;
	st_data->mm = std::pair<double, double>(left, right);
	st_data->ch_size = post_processor->is_PMT_type(post_processor->current_type) ? post_processor->PMT_channels.size() : post_processor->MPPC_channels.size();
	FunctionWrapper *picker = new FunctionWrapper (st_data);
	switch (post_processor->current_type)
	{
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_Npe_sum:
	case AStates::PMT_sum_N:
	case AStates::PMT_Npe_sum:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			return ((vals[((temp_data*)data)->ch_size] <= ((temp_data*)data)->mm.second) && (vals[((temp_data*)data)->ch_size] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AStates::MPPC_coord_y:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			return ((vals[((temp_data*)data)->ch_size+1] <= ((temp_data*)data)->mm.second) && (vals[((temp_data*)data)->ch_size+1] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_t_S:
	case AStates::PMT_t_S:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			return ((vals[4] <= ((temp_data*)data)->mm.second) && (vals[4] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AStates::MPPC_A_S:
	case AStates::PMT_A_S:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			return ((vals[1] <= ((temp_data*)data)->mm.second) && (vals[1] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AStates::MPPC_As:
	case AStates::PMT_As:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			return ((vals[1] <=((temp_data*)data)->mm.second) && (vals[1] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AStates::PMT_S2_S:
	case AStates::MPPC_S2:
	case AStates::MPPC_Ss:
	case AStates::MPPC_Double_I:
	case AStates::PMT_S2_int:
	case AStates::PMT_Ss:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			return ((vals[0] <=((temp_data*)data)->mm.second) && (vals[0] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	}
	picker->SetDrawFunction([](TCanvas *can, void* Data) {
		if (NULL==can || NULL==Data)
			return false;
		temp_data* data = (temp_data*) Data;
		if (can->GetUxmin()<=data->mm.first) {
			TLine *line1 = new TLine();
			line1->SetX1(data->mm.first);
			line1->SetX2(data->mm.first);
			line1->SetY1(can->GetUymin());
			line1->SetY2(can->GetUymax());
			line1->SetLineColor(kRed);
			line1->Draw("same");
		}
		if (can->GetUxmax()>=data->mm.second) {
			TLine *line2 = new TLine();
			line2->SetX1(data->mm.second);
			line2->SetX2(data->mm.second);
			line2->SetY1(can->GetUymin());
			line2->SetY2(can->GetUymax());
			line2->SetLineColor(kRed);
			line2->Draw("same");
		}
		return true;
	});
	return picker;
}

void set_limits(double left, double right) //2 tier method
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	std::string name = "_histogram_limits_";
	double _left = std::min(left, right);
	double _right = std::max(left, right);
	FunctionWrapper *picker = create_vertical_lines_cut(_left, _right);

	if (post_processor->isComposite(post_processor->current_type))
		post_processor->add_hist_cut(picker, name, -1, true);
	else {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			for (std::size_t chi=0; chi!=post_processor->MPPC_channels.size(); ++chi)
				post_processor->add_hist_cut(picker, name, post_processor->MPPC_channels[chi], true);
		} else {
			post_processor->add_hist_cut(picker, name, true);
		}
	}
	update();
}

void draw_limits(double left, double right) //2 tier method
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	std::string name = "_histogram_drawn_limits_";
	double _left = std::min(left, right);
	double _right = std::max(left, right);
	FunctionWrapper *picker = create_vertical_lines_cut(_left, _right);

	if (post_processor->isComposite(post_processor->current_type))
		post_processor->add_hist_cut(picker, name, -1, false);
	else {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			for (std::size_t chi=0; chi!=post_processor->MPPC_channels.size(); ++chi)
				post_processor->add_hist_cut(picker, name, post_processor->MPPC_channels[chi], false);
		} else {
			post_processor->add_hist_cut(picker, name, false);
		}
	}
	update();
}

void unset_limits(void) //2 tier method
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	std::string name = "_histogram_limits_";
	post_processor->remove_hist_cut(name);
	update();
}

void unset_draw_limits(void) //2 tier method
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	std::string name = "_histogram_drawn_limits_";
	post_processor->remove_hist_cut(name);
	update();
}

//region is {t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
FunctionWrapper* create_S_t_rect_exclude_cut(std::vector<double> region) //do not call from the CINT
{	
	struct temp_data {
		std::vector<double> reg;
		int ch_size;
	};
	temp_data * st_data = new temp_data;
	st_data->reg = region;
	st_data->ch_size = post_processor->is_PMT_type(post_processor->current_type) ? post_processor->PMT_channels.size() : post_processor->MPPC_channels.size();
	FunctionWrapper *picker = new FunctionWrapper (st_data);
	switch (post_processor->current_type)
	{
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_coord_y:
	case AStates::PMT_t_S:
	case AStates::PMT_Ss:
	case AStates::PMT_As:
	case AStates::PMT_A_S:
	case AStates::PMT_S2_S:
	case AStates::PMT_sum_N:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::MPPC_t_S:
	case AStates::MPPC_A_S:
	case AStates::MPPC_S2:
	case AStates::MPPC_Ss:
	case AStates::MPPC_As:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::MPPC_Npe_sum:
	case AStates::PMT_Npe_sum:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			temp_data* da = (temp_data*)data;
			for (int i=0, _end_=da->reg.size()/4; i!=_end_; ++i){
			    if ((vals[4]>=da->reg[4*i])&&(vals[4]<= da->reg[4*i+1])&&(vals[0]>= da->reg[4*i+2])&&(vals[0]<= da->reg[4*i+3]))
			      return false;
			  }
			return true;
		});
		break;
	}
	case AStates::PMT_S2_int:
	case AStates::MPPC_Double_I:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	{
		delete picker;
		return NULL;
	}
	default:
	{
		std::cout<<"Error: unknown type - you forgot to implement it in \"create_S_t_rect_exclude_cut\""<<std::endl;
		delete picker;
		return NULL;
	}
	}
	picker->SetDrawFunction([](TCanvas *can, void* Data) {
		if (NULL==can || NULL==Data)
			return false;
		temp_data* da = (temp_data*) Data;
		viewRegion region(can->GetUxmin(), can->GetUymin(), can->GetUxmax(), can->GetUymax());
		for (int i = 0, _end_ = da->reg.size() / 4; i != _end_; ++i) {
			region.clear_polyline();
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 2]);
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 3]);
			region.polyline_push(da->reg[4 * i + 1], da->reg[4 * i + 3]);
			region.polyline_push(da->reg[4 * i + 1], da->reg[4 * i + 2]);
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 2]);
			TPolyLine *line = region.get_clipped_polyline();
			line->SetLineColor(kRed);
			line->Draw("same");
		}
		return true;
	});
	return picker;
}

void cut_S_t_rect_exclude(double t_min, double t_max, double S_min, double S_max, bool drawn, int channel, std::string _name)
{
	std::vector<double> region;
	region.push_back(t_min);
	region.push_back(t_max);
	region.push_back(S_min);
	region.push_back(S_max);
	cut_S_t_rect_exclude(region, drawn, channel, _name);
}

//region is {t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
void cut_S_t_rect_exclude(std::vector<double> region, bool drawn, int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : "<<(channel = post_processor->current_channel)<<std::endl;
	}
	std::string name = ((_name=="") ? "_S_t_exclude_" : _name);
	FunctionWrapper *picker = create_S_t_rect_exclude_cut(region);
	if (NULL == picker) {
		std::cout << "This cut is impossible for current type ("<<post_processor->type_name(post_processor->current_type)<<")"<<std::endl;
		return;
	}
	post_processor->add_hist_cut(picker, name, channel, !drawn);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}
//remcut is for remove_cut
void remcut_S_t_rect_exclude(int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_S_t_exclude_" : _name);
	post_processor->remove_hist_cut(name, channel);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

//region is {t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
FunctionWrapper* create_S_t_rect_select_cut(std::vector<double> region) //do not call from the CINT
{
	struct temp_data {
		std::vector<double> reg;
		int ch_size;
	};
	temp_data * st_data = new temp_data;
	st_data->reg = region;
	st_data->ch_size = post_processor->is_PMT_type(post_processor->current_type) ? post_processor->PMT_channels.size() : post_processor->MPPC_channels.size();
	FunctionWrapper *picker = new FunctionWrapper(st_data);
	switch (post_processor->current_type)
	{
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_coord_y:
	case AStates::PMT_t_S:
	case AStates::PMT_Ss:
	case AStates::PMT_As:
	case AStates::PMT_A_S:
	case AStates::PMT_S2_S:
	case AStates::PMT_sum_N:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::MPPC_t_S:
	case AStates::MPPC_A_S:
	case AStates::MPPC_S2:
	case AStates::MPPC_Ss:
	case AStates::MPPC_As:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::MPPC_Npe_sum:
	case AStates::PMT_Npe_sum:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			temp_data* da = (temp_data*)data;
			for (int i = 0, _end_ = da->reg.size() / 4; i != _end_; ++i) {
				if ((vals[4] >= da->reg[4 * i]) && (vals[4] <= da->reg[4 * i + 1]) && (vals[0] >= da->reg[4 * i + 2]) && (vals[0] <= da->reg[4 * i + 3]))
					return true;
			}
			return false;
		});
		break;
	}
	case AStates::PMT_S2_int:
	case AStates::MPPC_Double_I:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	{
		delete picker;
		return NULL;
	}
	default:
	{
		std::cout << "Error: unknown type - you forgot to implement it in \"create_S_t_rect_select_cut\"" << std::endl;
		delete picker;
		return NULL;
	}
	}
	picker->SetDrawFunction([](TCanvas *can, void* Data) {
		if (NULL == can || NULL == Data)
			return false;
		temp_data* da = (temp_data*)Data;
		viewRegion region(can->GetUxmin(), can->GetUymin(), can->GetUxmax(), can->GetUymax());
		for (int i = 0, _end_ = da->reg.size() / 4; i != _end_; ++i) {
			region.clear_polyline();
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 2]);
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 3]);
			region.polyline_push(da->reg[4 * i + 1], da->reg[4 * i + 3]);
			region.polyline_push(da->reg[4 * i + 1], da->reg[4 * i + 2]);
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 2]);
			TPolyLine *line = region.get_clipped_polyline();
			line->SetLineColor(kRed);
			line->Draw("same");
		}
		return true;
	});
	return picker;
}

void cut_S_t_rect_select(double t_min, double t_max, double S_min, double S_max, bool drawn, int channel, std::string _name)
{
	std::vector<double> region;
	region.push_back(t_min);
	region.push_back(t_max);
	region.push_back(S_min);
	region.push_back(S_max);
	cut_S_t_rect_select(region, drawn, channel, _name);
}
//region is {t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
void cut_S_t_rect_select(std::vector<double> region, bool drawn, int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_S_t_select_" : _name);
	FunctionWrapper *picker = create_S_t_rect_select_cut(region);
	if (NULL == picker) {
		std::cout << "This cut is impossible for current type (" << post_processor->type_name(post_processor->current_type) << ")" << std::endl;
		return;
	}
	post_processor->add_hist_cut(picker, name, channel, !drawn);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

void remcut_S_t_rect_select(int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_S_t_select_" : _name);
	post_processor->remove_hist_cut(name, channel);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

//region is {A_min0, A_max0, S_min0, S_max0, A_min1, A_max1 ...}
FunctionWrapper* create_A_S_rect_exclude_cut(std::vector<double> region) //do not call from the CINT
{
	struct temp_data {
		std::vector<double> reg;
		int ch_size;
	};
	temp_data * st_data = new temp_data;
	st_data->reg = region;
	st_data->ch_size = post_processor->is_PMT_type(post_processor->current_type) ? post_processor->PMT_channels.size() : post_processor->MPPC_channels.size();
	FunctionWrapper *picker = new FunctionWrapper(st_data);
	switch (post_processor->current_type)
	{
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_coord_y:
	case AStates::PMT_t_S:
	case AStates::PMT_Ss:
	case AStates::PMT_As:
	case AStates::PMT_A_S:
	case AStates::PMT_S2_S:
	case AStates::PMT_sum_N:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::MPPC_t_S:
	case AStates::MPPC_A_S:
	case AStates::MPPC_S2:
	case AStates::MPPC_Ss:
	case AStates::MPPC_As:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::MPPC_Npe_sum:
	case AStates::PMT_Npe_sum:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			temp_data* da = (temp_data*)data;
			for (int i = 0, _end_ = da->reg.size() / 4; i != _end_; ++i) {
				if ((vals[1] >= da->reg[4 * i]) && (vals[1] <= da->reg[4 * i + 1]) && (vals[0] >= da->reg[4 * i + 2]) && (vals[0] <= da->reg[4 * i + 3]))
					return false;
			}
			return true;
		});
		break;
	}
	case AStates::PMT_S2_int:
	case AStates::MPPC_Double_I:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	{
		delete picker;
		return NULL;
	}
	default:
	{
		std::cout << "Error: unknown type - you forgot to implement it in \"create_S_t_rect_select_cut\"" << std::endl;
		delete picker;
		return NULL;
	}
	}
	picker->SetDrawFunction([](TCanvas *can, void* Data) {
		if (NULL == can || NULL == Data)
			return false;
		temp_data* da = (temp_data*)Data;
		viewRegion region(can->GetUxmin(), can->GetUymin(), can->GetUxmax(), can->GetUymax());
		for (int i = 0, _end_ = da->reg.size() / 4; i != _end_; ++i) {
			region.clear_polyline();
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 2]);
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 3]);
			region.polyline_push(da->reg[4 * i + 1], da->reg[4 * i + 3]);
			region.polyline_push(da->reg[4 * i + 1], da->reg[4 * i + 2]);
			region.polyline_push(da->reg[4 * i + 0], da->reg[4 * i + 2]);
			TPolyLine *line = region.get_clipped_polyline();
			line->SetLineColor(kRed);
			line->Draw("same");
		}
		return true;
	});
	return picker;
}

void cut_A_S_rect_exclude(double A_min, double A_max, double S_min, double S_max, bool drawn, int channel, std::string _name)
{
	std::vector<double> region;
	region.push_back(A_min);
	region.push_back(A_max);
	region.push_back(S_min);
	region.push_back(S_max);
	cut_A_S_rect_exclude(region, drawn, channel, _name);
}
//region is {A_min0, A_max0, S_min0, S_max0, A_min1, A_max1 ...}
void cut_A_S_rect_exclude(std::vector<double> region, bool drawn, int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel: " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_A_S_exclude_" : _name);
	FunctionWrapper *picker = create_A_S_rect_exclude_cut(region);
	if (NULL == picker) {
		std::cout << "This cut is impossible for current type (" << post_processor->type_name(post_processor->current_type) << ")" << std::endl;
		return;
	}
	post_processor->add_hist_cut(picker, name, channel, !drawn);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

void remcut_A_S_rect_exclude(int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_A_S_exclude_" : _name);
	post_processor->remove_hist_cut(name, channel);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

void remcut(int channel, std::string name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	post_processor->remove_hist_cut(name, channel);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

void cut_S(double S_min, double S_max, bool drawn, int channel, std::string _name)
{
	std::string name = ((_name == "") ? "_S_cut_" : _name);
	cut_S_t_rect_select(-DBL_MAX, DBL_MAX, S_min, S_max, drawn, channel, name);
}

void remcut_S(int channel, std::string _name)
{
	std::string name = ((_name == "") ? "_S_cut_" : _name);
	remcut_S_t_rect_select(channel, name);
}

void cut_t(double t_min, double t_max, bool drawn, int channel, std::string _name)
{
	std::string name = ((_name == "") ? "_t_cut_" : _name);
	cut_S_t_rect_select(t_min, t_max, -DBL_MAX, DBL_MAX, drawn, channel, name);
}

void remcut_t(int channel, std::string _name)
{
	std::string name = ((_name == "") ? "_t_cut_" : _name);
	remcut_S_t_rect_select(channel, name);
}

FunctionWrapper* create_off_ch_cut(int channel) //do not call from the CINT
{
	struct temp_data {
		int channel;
		int ch_size;
	};
	temp_data * st_data = new temp_data;
	st_data->channel = channel;
	st_data->ch_size = post_processor->is_PMT_type(post_processor->current_type) ? post_processor->PMT_channels.size() : post_processor->MPPC_channels.size();
	FunctionWrapper *picker = new FunctionWrapper(st_data);
	switch (post_processor->current_type)
	{
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_coord_y:
	case AStates::PMT_t_S:
	case AStates::PMT_Ss:
	case AStates::PMT_As:
	case AStates::PMT_A_S:
	case AStates::PMT_S2_S:
	case AStates::PMT_sum_N:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::MPPC_t_S:
	case AStates::MPPC_A_S:
	case AStates::MPPC_S2:
	case AStates::MPPC_Ss:
	case AStates::MPPC_As:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::MPPC_Npe_sum:
	case AStates::PMT_Npe_sum:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			return false;
		});
		break;
	}
	case AStates::PMT_S2_int:
	case AStates::MPPC_Double_I:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	{
		delete picker;
		return NULL;
	}
	default:
	{
		std::cout << "Error: unknown type - you forgot to implement it in \"create_S_t_rect_select_cut\"" << std::endl;
		delete picker;
		return NULL;
	}
	}
	return picker;
}

//for multichannel types (e.g. signal form of all SiPMs (MPPCs)). TODO: single channel case may be implemented with multichannel one - decrease the number of types
void off_ch(int ch)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (!post_processor->isMultichannel(post_processor->current_type)) {
		std::cout << "Error: can't use this functoin for single channel type" << std::endl;
		return;
	}
	FunctionWrapper* picker = create_off_ch_cut(ch);
	if (NULL == picker) {
		std::cout << "This cut is impossible for current type (" << post_processor->type_name(post_processor->current_type) << ")" << std::endl;
		return;
	}
	add_hist_cut(picker, "ch_off", ch, false);
	//update(); it is required oftentimes to remove many channels.
}

void on_ch(int ch)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	remove_hist_cut("ch_off", ch);
}

//region is {A_min, A0, S0, A1, S1, A_max}
FunctionWrapper* create_A_S_fastPMT_cut(std::vector<double> region) //do not call from the CINT
{
	struct temp_data {
		std::vector<double> reg;
		int ch_size;
	};
	temp_data * st_data = new temp_data;
	st_data->reg = region;
	st_data->ch_size = post_processor->is_PMT_type(post_processor->current_type) ? post_processor->PMT_channels.size() : post_processor->MPPC_channels.size();
	FunctionWrapper *picker = new FunctionWrapper(st_data);
	switch (post_processor->current_type)
	{
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_coord_y:
	case AStates::PMT_t_S:
	case AStates::PMT_Ss:
	case AStates::PMT_As:
	case AStates::PMT_A_S:
	case AStates::PMT_S2_S:
	case AStates::PMT_sum_N:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::MPPC_t_S:
	case AStates::MPPC_A_S:
	case AStates::MPPC_S2:
	case AStates::MPPC_Ss:
	case AStates::MPPC_As:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::MPPC_Npe_sum:
	case AStates::PMT_Npe_sum:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			//{A_min, A0, S0, A1, S1, A_max}
			std::vector <double> reg = ((temp_data*)data)->reg;
			if (6 > reg.size())
				return true;
			double A_min = reg[0];
			double A0 = reg[1];
			double S0 = reg[2];
			double A1 = reg[3];
			double S1 = reg[4];
			double A_max = reg[5];
			if (vals[1]<A_min)
				return false;
			if ((vals[1]<A_max) && (vals[0]< (S0 + (S1 - S0)*(vals[1] - A0) / (A1 - A0))))
				return false;
			return true;
		});
		break;
	}
	case AStates::PMT_S2_int:
	case AStates::MPPC_Double_I:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	{
		delete picker;
		return NULL;
	}
	default:
	{
		std::cout << "Error: unknown type - you forgot to implement it in \"create_S_t_rect_select_cut\"" << std::endl;
		delete picker;
		return NULL;
	}
	}
	picker->SetDrawFunction([](TCanvas *can, void* Data) {
		if (NULL == can || NULL == Data)
			return false;
		//{A_min, A0, S0, A1, S1, A_max}
		std::vector <double> reg = ((temp_data*)Data)->reg;
		if (6 > reg.size())
			return false;
		double A_min = reg[0];
		double A0 = reg[1];
		double S0 = reg[2];
		double A1 = reg[3];
		double S1 = reg[4];
		double A_max = reg[5];
		double S_intersect_A_min = S0 + (S1 - S0)*(A_min - A0) / (A1 - A0);
		double S_intersect_A_max = S0 + (S1 - S0)*(A_max - A0) / (A1 - A0);
		viewRegion region(can->GetUxmin(), can->GetUymin(), can->GetUxmax(), can->GetUymax());
		region.polyline_push(A_min, DBL_MAX);
		region.polyline_push(A_min, S_intersect_A_min);
		region.polyline_push(A_max, S_intersect_A_max);
		region.polyline_push(A_max, -DBL_MAX);
		TPolyLine *line = region.get_clipped_polyline();
		line->SetLineColor(kRed);
		line->Draw("same");
		return true;
	});
	return picker;
}

//region is {A0, S0, A1, S1} only points above the line are excluded
FunctionWrapper* create_A_S_upper_cut(std::vector<double> region) //do not call from the CINT
{
	struct temp_data {
		std::vector<double> reg;
		int ch_size;
	};
	temp_data * st_data = new temp_data;
	st_data->reg = region;
	st_data->ch_size = post_processor->is_PMT_type(post_processor->current_type) ? post_processor->PMT_channels.size() : post_processor->MPPC_channels.size();
	FunctionWrapper *picker = new FunctionWrapper(st_data);
	switch (post_processor->current_type)
	{
	case AStates::MPPC_coord:
	case AStates::MPPC_coord_x:
	case AStates::MPPC_coord_y:
	case AStates::PMT_t_S:
	case AStates::PMT_Ss:
	case AStates::PMT_As:
	case AStates::PMT_A_S:
	case AStates::PMT_S2_S:
	case AStates::PMT_sum_N:
	case AStates::PMT_tbS:
	case AStates::PMT_tbN:
	case AStates::MPPC_t_S:
	case AStates::MPPC_A_S:
	case AStates::MPPC_S2:
	case AStates::MPPC_Ss:
	case AStates::MPPC_As:
	case AStates::MPPC_tbS_sum:
	case AStates::MPPC_tbN_sum:
	case AStates::MPPC_tbS:
	case AStates::MPPC_tbN:
	case AStates::MPPC_Npe_sum:
	case AStates::PMT_Npe_sum:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			//{A0, S0, A1, S1}
			std::vector <double> reg = ((temp_data*)data)->reg;
			if (4 > reg.size())
				return true;
			double A0 = reg[0];
			double S0 = reg[1];
			double A1 = reg[2];
			double S1 = reg[3];
			if (vals[1]<A0 || vals[1] > A1)
				return true;
			if ((vals[0] > (S0 + (S1 - S0)*(vals[1] - A0) / (A1 - A0))))
				return false;
			return true;
		});
		break;
	}
	case AStates::PMT_S2_int:
	case AStates::MPPC_Double_I:
	case AStates::Correlation:
	case AStates::CorrelationAll:
	{
		delete picker;
		return NULL;
	}
	default:
	{
		std::cout << "Error: unknown type - you forgot to implement it in \"create_A_S_upper_cut\"" << std::endl;
		delete picker;
		return NULL;
	}
	}
	picker->SetDrawFunction([](TCanvas *can, void* Data) {
		if (NULL == can || NULL == Data)
			return false;
		//{A0, S0, A1, S1}
		std::vector <double> reg = ((temp_data*)Data)->reg;
		if (4 > reg.size())
			return false;
		double A0 = reg[0];
		double S0 = reg[1];
		double A1 = reg[2];
		double S1 = reg[3];
		double S_intersect_A_min = S0;
		double S_intersect_A_max = S1;
		viewRegion region(can->GetUxmin(), can->GetUymin(), can->GetUxmax(), can->GetUymax());
		region.polyline_push(A0, DBL_MAX);
		region.polyline_push(A0, S_intersect_A_min);
		region.polyline_push(A1, S_intersect_A_max);
		region.polyline_push(A1, DBL_MAX);
		TPolyLine *line = region.get_clipped_polyline();
		line->SetLineColor(kRed);
		line->Draw("same");
		return true;
	});
	return picker;
}

//region is {A_min, A0, S0, A1, S1, A_max}, draw it for clarification, e.g.:
//ch(7); add_S_t_fast_PMT(region, true); //- will display cuts with red lines
void cut_A_S_fast_PMT(std::vector<double> region, bool drawn, int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_A_S_fastPMT_" : _name);
	FunctionWrapper *picker = create_A_S_fastPMT_cut(region);
	if (NULL == picker) {
		std::cout << "This cut is impossible for current type (" << post_processor->type_name(post_processor->current_type) << ")" << std::endl;
		return;
	}
	post_processor->add_hist_cut(picker, name, channel, !drawn);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

void cut_A_S_upper(double A_min, double S_min, double A_max, double S_max, bool drawn, int channel, std::string _name)
{
	std::vector<double> region;
	region.push_back(A_min);
	region.push_back(S_min);
	region.push_back(A_max);
	region.push_back(S_max);
	cut_A_S_upper(region, drawn, channel, _name);
}

//region is {A0, S0, A1, S1}, draw it for clarification, e.g.:
void cut_A_S_upper(std::vector<double> region, bool drawn, int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_A_S_upper_" : _name);
	FunctionWrapper *picker = create_A_S_upper_cut(region);
	if (NULL == picker) {
		std::cout << "This cut is impossible for current type (" << post_processor->type_name(post_processor->current_type) << ")" << std::endl;
		return;
	}
	post_processor->add_hist_cut(picker, name, channel, !drawn);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

void remcut_A_S_fast_PMT(int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_A_S_fastPMT_" : _name);
	post_processor->remove_hist_cut(name, channel);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

void remcut_A_S_upper(int channel, std::string _name)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (-1 == channel) {
		if (post_processor->isMultichannel(post_processor->current_type)) {
			std::cout << "Can't use -1 channel for this cut and multichannel type" << std::endl;
			return;
		}
		std::cout << "Using default channel : " << (channel = post_processor->current_channel) << std::endl;
	}
	std::string name = ((_name == "") ? "_A_S_upper_" : _name);
	post_processor->remove_hist_cut(name, channel);
	if (!post_processor->isMultichannel(post_processor->current_type))
		update();
}

