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
//Go to channel. Does not save previous hist in file but does save parameters such as cuts
void ch(int ch)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->GotoCh(ch);
}

void ty(AnalysisStates::Type to_type)
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

void unset_zoom(bool do_update)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->unset_zoom(do_update);
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

void set_corr (AnalysisStates::Type x_t, AnalysisStates::Type y_t, int chx, int chy)
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
	post_processor->add_hist_cut(picker, name, ch, draw);
}

void add_hist_cut(FunctionWrapper *picker, std::string name, bool draw)
//same as above but ch is set automatically if possible.
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->add_hist_cut(picker, name, draw);
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
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
	if (prev_method == CalibrationInfo::Using2pe)
		prev_method = CalibrationInfo::Using1pe2pe;
	else
		prev_method = CalibrationInfo::Using1pe;
	post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
}

void add_2peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
	if (prev_method == CalibrationInfo::Using1pe)
		prev_method = CalibrationInfo::Using1pe2pe;
	else
		prev_method = CalibrationInfo::Using2pe;
	post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
}

void rem_1peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
	if (prev_method == CalibrationInfo::Using1pe2pe)
		prev_method = CalibrationInfo::Using2pe;
	if ((prev_method == CalibrationInfo::Using1pe) || (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
}

void rem_2peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
	if (prev_method == CalibrationInfo::Using1pe2pe)
		prev_method = CalibrationInfo::Using1pe;
	if ((prev_method == CalibrationInfo::Using2pe) || (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
}

void set_1peS(double val)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.setS1pe(post_processor->current_channel,val);
}

void unset_1peS(void)
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.unsetS1pe(post_processor->current_channel);
}

void use_mean(Bool_t do_use) //uses mean value of data instead of gauss' mean. May be usefull for S2_S
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	if (AnalysisStates::Type::MPPC_Ss==post_processor->current_type) {
		CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
		if (do_use)
			prev_method = CalibrationInfo::UsingMean;
		if ((!do_use) && (prev_method == CalibrationInfo::UsingMean))
			prev_method = CalibrationInfo::Ignore;
		post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
	}
}

//TODO: remove?
void set_calib_N(int from, int to)//in order to set default use invalid values
{
	if (NULL == g_data) {
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.set_N_calib(post_processor->current_channel, from, to);
	post_processor->update(PostProcessor::UpdateState::Results);//TODO: encapsulate
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
	case AnalysisStates::MPPC_coord:
	case AnalysisStates::MPPC_coord_x:
	case AnalysisStates::MPPC_Npe_sum:
	case AnalysisStates::PMT_sum_N:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			return ((vals[((temp_data*)data)->ch_size] <= ((temp_data*)data)->mm.second) && (vals[((temp_data*)data)->ch_size] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AnalysisStates::MPPC_coord_y:
	{
		picker->SetFunction([](std::vector<double> &vals, int run, void* data) {
			return ((vals[((temp_data*)data)->ch_size+1] <= ((temp_data*)data)->mm.second) && (vals[((temp_data*)data)->ch_size+1] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AnalysisStates::MPPC_times:
	case AnalysisStates::MPPC_times_N:
	case AnalysisStates::PMT_times:
	case AnalysisStates::PMT_times_N:
	case AnalysisStates::MPPC_sum_ts:
	case AnalysisStates::MPPC_t_S:
	case AnalysisStates::PMT_t_S:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			return ((vals[4] <= ((temp_data*)data)->mm.second) && (vals[4] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AnalysisStates::MPPC_A_S:
	case AnalysisStates::PMT_A_S:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			return ((vals[1] <= ((temp_data*)data)->mm.second) && (vals[1] >= ((temp_data*)data)->mm.first));
		});
		break;
	}
	case AnalysisStates::PMT_S2_S:
	case AnalysisStates::MPPC_S2:
	case AnalysisStates::MPPC_Ss:
	case AnalysisStates::MPPC_Double_I:
	case AnalysisStates::MPPC_S2_S:
	case AnalysisStates::MPPC_tfinal:
	case AnalysisStates::MPPC_tstart:
	case AnalysisStates::MPPC_tboth:
	case AnalysisStates::PMT_S2_int:
	case AnalysisStates::PMT_Ss:
	case AnalysisStates::Correlation:
	case AnalysisStates::CorrelationAll:
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
	case AnalysisStates::MPPC_coord:
	case AnalysisStates::MPPC_coord_x:
	case AnalysisStates::MPPC_coord_y:
	case AnalysisStates::PMT_t_S:
	case AnalysisStates::PMT_Ss:
	case AnalysisStates::PMT_A_S:
	case AnalysisStates::PMT_S2_S:
	case AnalysisStates::PMT_sum_N:
	case AnalysisStates::PMT_times:
	case AnalysisStates::PMT_times_N:
	case AnalysisStates::MPPC_t_S:
	case AnalysisStates::MPPC_A_S:
	case AnalysisStates::MPPC_S2:
	case AnalysisStates::MPPC_Ss:
	case AnalysisStates::MPPC_sum_ts:
	case AnalysisStates::MPPC_times:
	case AnalysisStates::MPPC_times_N:
	case AnalysisStates::MPPC_Npe_sum:
	{
		picker->SetFunction( [](std::vector<double> &vals, int run, void* data) {
			temp_data* da = (temp_data*)data;
			for (int i=0, _end_=da->reg.size()/4; i!=_end_; ++i){
			    if ((vals[4]>=da->reg[4*i])&&(vals[4]<=da->reg[4*i+1])&&(vals[0]>=da->reg[4*i+2])&&(vals[0]<=da->reg[4*i+3]))
			      return false;
			  }
			return true;
		});
		break;
	}
	case AnalysisStates::PMT_S2_int:
	case AnalysisStates::MPPC_S2_S:
	case AnalysisStates::MPPC_Double_I:
	case AnalysisStates::MPPC_tfinal:
	case AnalysisStates::MPPC_tstart:
	case AnalysisStates::MPPC_tboth:
	case AnalysisStates::Correlation:
	case AnalysisStates::CorrelationAll:
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
		temp_data* data = (temp_data*) Data;
		for (int i=0, _end_=data->reg.size()/4; i!=_end_; ++i) {
				copy functions from my additions to garfield
				TPolyLine
				if ((vals[4]>=da->reg[4*i])&&(vals[4]<=da->reg[4*i+1])&&(vals[0]>=da->reg[4*i+2])&&(vals[0]<=da->reg[4*i+3]))
				  return false;
			  }
			return true;
		double x_inter1 = data->reg[0], y_inter1;
		double x_inter2 = data->reg[5], y_inter2;
		x_inter1 = da
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


