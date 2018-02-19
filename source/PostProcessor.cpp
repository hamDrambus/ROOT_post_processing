#include "PostProcessor.h"

PostProcessor::PostProcessor(AllExperimentsResults* _data) : AnalysisStates(_data), calibr_info(this)
{
	current_setups = NULL;
	current_vert_line0 = NULL;
	current_vert_line1 = NULL;
	current_canvas = NULL;
	current_hist1 = NULL;
	current_hist2 = NULL;
	current_fit_func = NULL;

	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}

	data = _data;
	current_canvas = new TCanvas((std::string("Inter_canvas_") + std::to_string(canvas_n)).c_str(), (std::string("Inter_canvas_") + std::to_string(canvas_n)).c_str());
	current_canvas->cd();
	++canvas_n;
	for (auto exp = data->exp_area.experiments.begin(); exp != data->exp_area.experiments.end(); ++exp){
		manual_setups.push_back(STD_CONT<STD_CONT<HistogramSetups*>>());
		avr_S2_S.push_back(STD_CONT<double>());
		avr_double_I.push_back(STD_CONT<double>());
		PMT3_avr_S2_S.push_back(-1);
		PMT1_avr_S2_S.push_back(-1);
		for (auto ch = data->mppc_channels.begin(); ch != data->mppc_channels.end(); ++ch){
			manual_setups.back().push_back(STD_CONT<HistogramSetups*>());
			for (int h = MPPC_Ss; h != ((int)PMT_t_S + 1);++h)
				manual_setups.back().back().push_back(NULL);
			avr_S2_S.back().push_back(-1);
			avr_double_I.back().push_back(-1);
		}
	}

	current_setups = new HistogramSetups();
	set_default_hist_setups();
	/*if (is_TH1D_hist()){
		current_hist1 = new TH1D(hist_name().c_str(),hist_name().c_str(),
			current_setups->N_bins,current_setups->left_limit,current_setups->right_limit);
		FillHist(current_hist1);
	} else {
		current_hist2 = new TH2D(hist_name().c_str(), hist_name().c_str(),
			current_setups->N_bins, current_setups->left_limit, current_setups->right_limit,
			current_setups->N_bins, current_setups->bottom_limit, current_setups->top_limit);
		FillHist(current_hist2);
	}*/ //moved to update
	//current_fit_func = create_fit_function(current_setups);
	update_fit_function();
	do_fit(true);
	//^calls update();
}

bool PostProcessor::is_TH1D_hist()
{
	return !((current_type == Type::PMT_t_S) || (current_type == Type::MPPC_t_S));
}
std::string PostProcessor::hist_name()
{
	std::string name = (is_PMT_type(current_type) ? "PMT#" : "MPPC#") + std::to_string(current_channel)
		+ "_" + g_data->exp_area.experiments[current_exp_index];
	name += type_name(current_type);
	return name;
}

std::string PostProcessor::type_name(Type type)
{
	std::string name;
	switch (type){
	case Type::MPPC_Ss:{
		name += "_Ss";
		break;
	}
	case Type::MPPC_S2_S:{
		name += "_S2_S";
		break;
	}
	case Type::MPPC_double_I:{
		name += "_double_I";
		break;
	}
	case Type::MPPC_tboth:{
		name += "_t_both";
		break;
	}
	case Type::MPPC_tfinal:{
		name += "_t_final";
		break;
	}
	case Type::MPPC_tstart:{
		name += "_t_start";
		break;
	}
	case Type::MPPC_t_S:{
		name += "_t_S";
		break;
	}
	case Type::PMT_S2_S:{
		name += "_S2_S";
		break;
	}
	case Type::PMT_Ss:{
		name += "_Ss";
		break;
	}
	case Type::PMT_t_S:{
		name += "_t_S";
		break;
	}
	}
	return name;
}
void PostProcessor::print_hist(void)
{
	std::string name = OUTPUT_DIR + (is_PMT_type(current_type) ? OUTPUT_PMT_PICS : OUTPUT_MPPCS_PICS) + g_data->exp_area.experiments[current_exp_index]
		+ "/" + (is_PMT_type(current_type) ? "PMT_" : "MPPC_") + std::to_string(current_channel)
		+ "/" + (is_PMT_type(current_type) ? "PMT_" : "MPPC_") + std::to_string(current_channel)
		+ type_name(current_type)+".png";
	current_canvas->SaveAs(name.c_str(), "png");
}

bool PostProcessor::StateChange(int to_ch, int to_exp, Type to_type, int from_ch, int from_exp, Type from_type, bool save)
{
	if (save)
		post_processor->set_hist_setups(current_setups, data->exp_area.experiments[from_exp], from_ch, from_type);
	if (!AnalysisStates::StateChange(to_ch, to_exp, to_type, from_ch, from_exp, from_type,save)){
		print_hist();
		return false; //no change
	}
	current_setups = get_hist_setups(data->exp_area.experiments[to_exp], to_ch, to_type);
	if (NULL == current_setups){
		current_setups = new HistogramSetups();
		set_default_hist_setups();
	}
	update_fit_function();
	update();
	print_hist();
	return true;
}

void PostProcessor::set_hist_setups(HistogramSetups* setups, std::string experiment, int channel, Type type)//does not call update
{
	int exp_ind = data->get_exp_index(experiment);
	if (exp_ind < 0)
		return;
	int ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
	if ((NULL != manual_setups[exp_ind][ch_ind][type]) && (setups != manual_setups[exp_ind][ch_ind][type]))
		delete manual_setups[exp_ind][ch_ind][type];
	manual_setups[exp_ind][ch_ind][type] = setups;
}

HistogramSetups* PostProcessor::get_hist_setups(std::string experiment, int channel, Type type)
{
	int exp_ind = data->get_exp_index(experiment);
	if (exp_ind < 0)
		return NULL;
	int ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return NULL;
	return manual_setups[exp_ind][ch_ind][type];
}

void PostProcessor::save(int channel)
{
	int ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
	DVECTOR N_pe_result;
	for (int pt = 0; pt < data->exp_area.experiments.size(); ++pt){
		if (pt <= calibr_info.get_N_calib(channel).second)
			N_pe_result.push_back(data->N_pe_direct[pt][ch_ind]);
		else
			N_pe_result.push_back(data->N_pe_double_I[pt][ch_ind]);
	}
	std::ofstream file;
	open_output_file(OUTPUT_DIR + OUTPUT_MPPCS_PICS + std::to_string(channel) + "_Npe.txt", file);
	file << "E[kV/cm]\tN_pe_direct\tN_pe_double_I\tN_pe_result" << std::endl;
	for (int pt = 0; pt < data->exp_area.experiments.size(); ++pt)
		file << data->Fields[pt] << "\t" << data->N_pe_direct[pt][ch_ind] << "\t" << data->N_pe_double_I[pt][ch_ind] << "\t" << N_pe_result[pt] << std::endl;
	file.close();
}

//see function for DVECTOR &vals usage in cuts' picker
void PostProcessor::FillHist(void* p_hist)//considers cuts and histogram tipe (void*)==either TH1D* or TH2D*
{
	switch (current_type)
	{
	case Type::MPPC_double_I:
	{
		int run_size = data->double_I[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->double_I[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _0cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _0cutted;
			}
			((TH1D*)p_hist)->Fill(cut_data[0]);
			_0cutted:;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		int run_size = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _1cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _1cutted;
			}
			((TH1D*)p_hist)->Fill(cut_data[0]);
		_1cutted:;
		}
		break;
	}
	case Type::MPPC_Ss:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _2cutted;
			}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _2cutted1;
				}
				((TH1D*)p_hist)->Fill(cut_data[0]);
			_2cutted1:;
			}
		_2cutted:;
		}
		break;
	}
	case Type::MPPC_tstart:
	{}
	case Type::MPPC_tboth:
	{
		int run_size = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _3cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _3cutted;
			}
			((TH1D*)p_hist)->Fill(cut_data[0]);
		_3cutted:;
		}
		if (current_type == Type::MPPC_tstart)
			break;
	}
	case Type::MPPC_tfinal:
	{
		int run_size = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _4cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _4cutted;
			}
			((TH1D*)p_hist)->Fill(cut_data[0]);
		_4cutted:;
		}
		break;
	}
	case Type::MPPC_t_S:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _5cutted;
			}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _5cutted1;
				}
				((TH2D*)p_hist)->Fill(0.5*(cut_data[2] + cut_data[3]), cut_data[0]);
			_5cutted1:;
			}
		_5cutted:;
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(5);
		for (auto run = 0; run != run_size; ++run){
			double S2 = 0;
			cut_data[0] = 0; //reserved for S2 cut
			cut_data[1] = 0; //the 4 below are for S2 selection
			cut_data[2] = 0;
			cut_data[3] = 0;
			cut_data[4] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _6cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[4] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _6cutted1;
				}
				S2 += cut_data[1];
			_6cutted1:;
			}
			cut_data[0] = S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _6cutted;
			}
			((TH1D*)p_hist)->Fill(cut_data[0]);
		_6cutted:;
		}
		break;
	}
	case Type::PMT_Ss:
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _7cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _7cutted1;
				}
			((TH1D*)p_hist)->Fill(cut_data[0]);
		_7cutted1:;
			}
		_7cutted:;
		}
		break;
	}
	case Type::PMT_t_S:
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _8cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _8cutted1;
				}
				((TH2D*)p_hist)->Fill(0.5*(cut_data[2] + cut_data[3]), cut_data[0]);
			_8cutted1:;
			}
		_8cutted:;
		}
		break;
	}
	}
}
int PostProcessor::numOfFills(void)
{
	int ret = 0;
	switch (current_type)
	{
	case Type::MPPC_double_I:
	{
		int run_size = data->double_I[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->double_I[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _0cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _0cutted;
			}
			++ret;
		_0cutted:;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		int run_size = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _1cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _1cutted;
			}
			++ret;
		_1cutted:;
		}
		break;
	}
	case Type::MPPC_Ss:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _2cutted;
			}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _2cutted1;
				}
				++ret;
			_2cutted1:;
			}
		_2cutted:;
		}
		break;
	}
	case Type::MPPC_tstart:
	{}
	case Type::MPPC_tboth:
	{
		int run_size = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _3cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _3cutted;
			}
			++ret;
		_3cutted:;
		}
		if (current_type == Type::MPPC_tstart)
			break;
	}
	case Type::MPPC_tfinal:
	{
		int run_size = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _4cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _4cutted;
			}
			++ret;
		_4cutted:;
		}
		break;
	}
	case Type::MPPC_t_S:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _5cutted;
			}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _5cutted1;
				}
				++ret;
			_5cutted1:;
			}
		_5cutted:;
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(5);
		for (auto run = 0; run != run_size; ++run){
			double S2 = 0;
			cut_data[0] = 0; //reserved for S2 cut
			cut_data[1] = 0; //the 4 below are for S2 selection
			cut_data[2] = 0;
			cut_data[3] = 0;
			cut_data[4] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _6cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[4] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _6cutted1;
				}
				S2 += cut_data[1];
			_6cutted1:;
			}
			cut_data[0] = S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _6cutted;
			}
			++ret;
		_6cutted:;
		}
		break;
	}
	case Type::PMT_Ss:
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _7cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _7cutted1;
				}
				++ret;
			_7cutted1:;
			}
		_7cutted:;
		}
		break;
	}
	case Type::PMT_t_S:
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _8cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _8cutted1;
				}
				++ret;
			_8cutted1:;
			}
		_8cutted:;
		}
		break;
	}
	}
	return ret;
}
std::pair<double, double> PostProcessor::hist_y_limits(void) //considering cuts
{
	std::pair<double,double> ret(DBL_MAX,-DBL_MAX);
	switch (current_type)
	{
	case Type::MPPC_double_I:
	{
		break;
	}
	case Type::MPPC_S2_S:
	{
		break;
	}
	case Type::MPPC_Ss:
	{
		break;
	}
	case Type::MPPC_tstart:
	{}
	case Type::MPPC_tboth:
	{
		break;
	}
	case Type::MPPC_tfinal:
	{
		break;
	}
	case Type::MPPC_t_S:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _0cutted;
			}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _0cutted1;
				}
				ret.first = std::min(ret.first, cut_data[0]);
				ret.second = std::max(ret.second, cut_data[0]);
			_0cutted1:;
			}
		_0cutted:;
		}
		break;
	}
	case Type::PMT_S2_S:
	{
		break;
	}
	case Type::PMT_Ss:
	{
		break;
	}
	case Type::PMT_t_S:
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _1cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _1cutted1;
				}
				ret.first = std::min(ret.first, cut_data[0]);
				ret.second = std::max(ret.second, cut_data[0]);
			_1cutted1:;
			}
		_1cutted:;
		}
		break;
	}
	}
	return ret;
}
std::pair<double, double> PostProcessor::hist_x_limits(void) //valid only for 2d plots
{
	std::pair<double, double> ret(DBL_MAX, -DBL_MAX);
	switch (current_type)
	{
	case Type::MPPC_double_I:
	{
		int run_size = data->double_I[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->double_I[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _0cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _0cutted;
			}
			ret.first = std::min(ret.first, cut_data[0]);
			ret.second = std::max(ret.second, cut_data[0]);
		_0cutted:;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		int run_size = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _1cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _1cutted;
			}
			ret.first = std::min(ret.first, cut_data[0]);
			ret.second = std::max(ret.second, cut_data[0]);
		_1cutted:;
		}
		break;
	}
	case Type::MPPC_Ss:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _2cutted;
			}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _2cutted1;
				}
				ret.first = std::min(ret.first, cut_data[0]);
				ret.second = std::max(ret.second, cut_data[0]);
			_2cutted1:;
			}
		_2cutted:;
		}
		break;
	}
	case Type::MPPC_tstart:
	{}
	case Type::MPPC_tboth:
	{
		int run_size = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _3cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _3cutted;
			}
			ret.first = std::min(ret.first, cut_data[0]);
			ret.second = std::max(ret.second, cut_data[0]);
		_3cutted:;
		}
		if (current_type == Type::MPPC_tstart)
			break;
	}
	case Type::MPPC_tfinal:
	{
		int run_size = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _4cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _4cutted;
			}
			ret.first = std::min(ret.first, cut_data[0]);
			ret.second = std::max(ret.second, cut_data[0]);
		_4cutted:;
		}
		break;
	}
	case Type::MPPC_t_S:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))
					goto _5cutted;
			}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _5cutted1;
				}
				ret.first = std::min(ret.first, 0.5*(cut_data[2] + cut_data[3]));
				ret.second = std::max(ret.second, 0.5*(cut_data[2] + cut_data[3]));
			_5cutted1:;
			}
		_5cutted:;
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(5);
		for (auto run = 0; run != run_size; ++run){
			double S2 = 0;
			cut_data[0] = 0; //reserved for S2 cut
			cut_data[1] = 0; //the 4 below are for S2 selection
			cut_data[2] = 0;
			cut_data[3] = 0;
			cut_data[4] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _6cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[4] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _6cutted1;
				}
				S2 += cut_data[1];
			_6cutted1:;
			}
			cut_data[0] = S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _6cutted;
			}
			ret.first = std::min(ret.first, cut_data[0]);
			ret.second = std::max(ret.second, cut_data[0]);
		_6cutted:;
		}
		break;
	}
	case Type::PMT_Ss: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _7cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _7cutted1;
				}
				ret.first = std::min(ret.first, cut_data[0]);
				ret.second = std::max(ret.second, cut_data[0]);
			_7cutted1:;
			}
		_7cutted:;
		}
		break;
	}
	case Type::PMT_t_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (false == cut->GetAccept(run))//not calculating it here!
					goto _8cutted;
			}
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _8cutted1;
				}
				ret.first = std::min(ret.first, 0.5*(cut_data[2] + cut_data[3]));
				ret.second = std::max(ret.second, 0.5*(cut_data[2] + cut_data[3]));
			_8cutted1:;
			}
		_8cutted:;
		}
		break;
	}
	}
	return ret;
}
void PostProcessor::set_default_hist_setups(void)//does not affect cuts
{
	std::pair<double, double> x_lims = hist_x_limits();
	current_setups->left_limit = x_lims.first;
	current_setups->right_limit = x_lims.second;
	current_setups->left_drawn_limit = x_lims.first;
	current_setups->right_drawn_limit = x_lims.second;
	x_lims = hist_y_limits();
	current_setups->bottom_limit = std::min(x_lims.second, x_lims.first);
	current_setups->top_limit = std::max(x_lims.second, x_lims.first);
	int _N_ = numOfFills();
	current_setups->N_bins = _N_;
	current_setups->N_bins = std::max(1,(int)std::round(std::sqrt(current_setups->N_bins)));
	current_setups->fitted = false;

	switch (current_type)
	{
	case Type::MPPC_double_I:
	case Type::MPPC_S2_S:
	case Type::MPPC_tstart:
	case Type::MPPC_tfinal:
	case Type::PMT_S2_S:
	{
		current_setups->N_gauss = 1;
		current_setups->par_val.resize(3, 0);
		current_setups->par_left_limits.resize(3, 0);
		current_setups->par_right_limits.resize(3, 0);
		
		current_setups->par_left_limits[0] = 0;
		current_setups->par_right_limits[0] = std::max(1, 2*(int)std::sqrt(_N_));
		current_setups->par_val[0] = (int)std::sqrt(_N_);

		current_setups->par_left_limits[1] = current_setups->left_limit;
		current_setups->par_right_limits[1] = current_setups->right_limit;
		current_setups->par_val[1] = 0.5*(current_setups->par_left_limits[1] + current_setups->par_right_limits[1]);

		current_setups->par_left_limits[2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[2] = (current_setups->par_right_limits[1] - current_setups->par_left_limits[1]);
		current_setups->par_val[2] = 0.5*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);
		break;
	}
	case Type::MPPC_Ss:
	{
		current_setups->N_gauss = 2;
		current_setups->par_val.resize(6, 0);
		current_setups->par_left_limits.resize(6, 0);
		current_setups->par_right_limits.resize(6, 0);

		current_setups->par_left_limits[0] = 0;
		current_setups->par_right_limits[0] = std::max(1, 2 * (int)std::sqrt(_N_));
		current_setups->par_val[0] = (int)std::sqrt(_N_);

		current_setups->par_left_limits[1] = 0.001;
		current_setups->par_right_limits[1] = 0.003;
		current_setups->par_val[1] = 0.5*(current_setups->par_right_limits[1] + current_setups->par_left_limits[1]);

		current_setups->par_left_limits[2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[2] = (current_setups->right_limit - current_setups->left_limit);
		current_setups->par_val[2] = 0.2*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);

		current_setups->par_left_limits[3] = 0;
		current_setups->par_right_limits[3] = std::max(1, (int)(1.5*std::sqrt(_N_)));
		current_setups->par_val[3] = (int)std::sqrt(_N_);

		current_setups->par_left_limits[4] =0.0028;
		current_setups->par_right_limits[4] = 0.006;
		current_setups->par_val[4] = 0.5*(current_setups->par_right_limits[4] + current_setups->par_left_limits[4]);

		current_setups->par_left_limits[5] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[5] = (current_setups->right_limit - current_setups->left_limit);
		current_setups->par_val[5] = 0.2*(current_setups->par_left_limits[5] + current_setups->par_right_limits[5]);
		break;
	}
	case Type::MPPC_tboth:
	{
		current_setups->N_gauss = 2;
		current_setups->par_val.resize(6, 0);
		current_setups->par_left_limits.resize(6, 0);
		current_setups->par_right_limits.resize(6, 0);

		current_setups->par_left_limits[0] = 0;
		current_setups->par_right_limits[0] = std::max(1, 2 * (int)std::sqrt(_N_));
		current_setups->par_val[0] = (int)std::sqrt(_N_);

		current_setups->par_left_limits[1] = current_setups->left_limit;
		current_setups->par_right_limits[1] = current_setups->right_limit;
		current_setups->par_val[1] = 0.2*(current_setups->par_right_limits[1] + current_setups->par_left_limits[1]);

		current_setups->par_left_limits[2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[2] = (current_setups->right_limit - current_setups->left_limit);
		current_setups->par_val[2] = 0.2*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);

		current_setups->par_left_limits[3] = 0;
		current_setups->par_right_limits[3] = std::max(1, (int)(2*std::sqrt(_N_)));
		current_setups->par_val[3] = (int)std::sqrt(_N_);

		current_setups->par_left_limits[4] = current_setups->left_limit;
		current_setups->par_right_limits[4] = current_setups->right_limit;
		current_setups->par_val[4] = 0.8*(current_setups->par_right_limits[4] + current_setups->par_left_limits[4]);

		current_setups->par_left_limits[5] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[5] = (current_setups->right_limit - current_setups->left_limit);
		current_setups->par_val[5] = 0.2*(current_setups->par_left_limits[5] + current_setups->par_right_limits[5]);
		break;
	}
	case Type::PMT_Ss:
	{
		current_setups->N_gauss = 1;
		current_setups->par_val.resize(3, 0);
		current_setups->par_left_limits.resize(3, 0);
		current_setups->par_right_limits.resize(3, 0);

		current_setups->par_left_limits[0] = 0;
		current_setups->par_right_limits[0] = std::max(1, 2 * (int)std::sqrt(_N_));
		current_setups->par_val[0] = (int)std::sqrt(_N_);

		current_setups->par_left_limits[1] = current_setups->left_limit;
		current_setups->par_right_limits[1] = current_setups->right_limit;
		current_setups->par_val[1] = 0.5*(current_setups->par_left_limits[1] + current_setups->par_right_limits[1]);

		current_setups->par_left_limits[2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[2] = (current_setups->par_right_limits[1] - current_setups->par_left_limits[1]);
		current_setups->par_val[2] = 0.5*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);
		break;
	}
	case Type::MPPC_t_S:
	case Type::PMT_t_S:
	{
		current_setups->N_gauss = 0;
		current_setups->par_val.resize(0, 0);
		current_setups->par_left_limits.resize(0, 0);
		current_setups->par_right_limits.resize(0, 0);
		break;
	}
	}
}


void PostProcessor::save_all(void)
{
	for (int ch = data->exp_area.channels.get_next_index(); ch != -1; ch = data->exp_area.channels.get_next_index())
		save(ch);
	std::ofstream calibration;
	open_output_file(OUTPUT_DIR + OUTPUT_MPPCS_PICS + "calibration.txt", calibration);
	calibration << "MPPC\tS1pe" << std::endl;
	for (int i = 0; i < data->mppc_channels.size(); ++i)
		calibration << data->mppc_channels[i] << "\t" << calibr_info.getS1pe(data->mppc_channels[i]) << std::endl;
	calibration.close();
}

void PostProcessor::plot_N_pe(int channel, GraphicOutputManager* gr_man)
{
	int ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
	DVECTOR N_pe_result;
	DVECTOR N_pe_double_I_result;
	DVECTOR N_pe_direct_result;
	for (int pt = 0; pt < data->exp_area.experiments.size(); ++pt){
		N_pe_result.push_back(data->N_pe_result[pt][ch_ind]);
		N_pe_double_I_result.push_back(data->N_pe_double_I[pt][ch_ind]);
		N_pe_direct_result.push_back(data->N_pe_direct[pt][ch_ind]);
	}
	Drawing *dr = gr_man->GetDrawing("Npe MPPC#" + std::to_string(channel), ch_ind, ParameterPile::DrawEngine::Gnuplot);
	std::string prefix_lines;
	prefix_lines = "set xlabel \"Electric field in EL gap [kV/cm]\"\n";
	prefix_lines += "set tmargin 2\n";
	prefix_lines += "set ylabel \"N of pe\" rotate by 0 offset 4.5, 7.5\n";
	prefix_lines += "set yrange[1:1000]\n";
	prefix_lines += "set xrange[3:21]\n";
	prefix_lines += "set logscale y\n";
	prefix_lines += "unset key\n";
	prefix_lines += "set key inside top left\n";

	dr->AddToDraw(data->Fields, N_pe_direct_result, "MPPC#" + std::to_string(channel) + "N pe direct", "pt 4 ps 1.5 lc rgb '#000000'");
	dr->AddToDraw(data->Fields, N_pe_double_I_result, "MPPC#" + std::to_string(channel) + "N pe double I", "with line lc rgb '#000000'");
	dr->DrawData();
}

void PostProcessor::update(void)//TODO: optimize it?
{
	current_canvas->cd();
	current_canvas->SetTitle(hist_name().c_str());
	current_canvas->Clear();
	if (is_TH1D_hist()){
		if (current_hist1)
			current_hist1->Delete();
		current_hist1 = new TH1D(hist_name().c_str(),hist_name().c_str(),
			current_setups->N_bins,current_setups->left_limit,current_setups->right_limit);
		FillHist(current_hist1);
	} else {
		if (current_hist2)
			current_hist2->Delete();
		current_hist2 = new TH2D(hist_name().c_str(), hist_name().c_str(),
			current_setups->N_bins, current_setups->left_limit, current_setups->right_limit,
			current_setups->N_bins, current_setups->bottom_limit, current_setups->top_limit);
		FillHist(current_hist2);
	}
	if ((current_fit_func)&&(current_setups->fitted))
		current_fit_func->Draw();
	if (current_setups->left_drawn_limit > current_setups->left_limit){
		if (NULL == current_vert_line0)
			current_vert_line0 = new TLine();
		current_vert_line0->SetX1(current_setups->left_drawn_limit);
		current_vert_line0->SetX2(current_setups->left_drawn_limit);
		current_vert_line0->SetY1(current_canvas->GetUymin());
		current_vert_line0->SetY2(current_canvas->GetUymax());
		current_vert_line0->SetLineColor(kRed);
		current_vert_line0->Draw();
	}
	if (current_setups->right_drawn_limit < current_setups->right_limit){
		if (NULL == current_vert_line1)
			current_vert_line1 = new TLine();
		current_vert_line1->SetX1(current_setups->right_drawn_limit);
		current_vert_line1->SetX2(current_setups->right_drawn_limit);
		current_vert_line1->SetY1(current_canvas->GetUymin());
		current_vert_line1->SetY2(current_canvas->GetUymax());
		current_vert_line1->SetLineColor(kRed);
		current_vert_line1->Draw();
	}
	current_canvas->Update();

	update_physical();
	update_Npe();
}

//TODO: actually it is more logical to move the code below to CalibrationInfo, but then I'll need to add friends
void PostProcessor::update_Npe(void)
{
	for (int exp = 0, exp_end_ = data->N_pe_direct.size(); exp != exp_end_; ++exp) {
		for (int ch = 0, ch_end_ = MPPC_channels.size(); ch != ch_end_; ++ch) {
			data->N_pe_direct[exp][ch] = avr_S2_S[exp][ch];
			data->N_pe_double_I[exp][ch] = avr_double_I[exp][ch];
		}
	}
	calibr_info.recalibrate(data->N_pe_direct, data->N_pe_double_I, data->Fields);
	for (int exp = 0, exp_end_ = data->N_pe_direct.size(); exp != exp_end_; ++exp) {
		for (int ch = 0, ch_end_ = MPPC_channels.size(); ch != ch_end_; ++ch) {
			if (exp > calibr_info.get_N_calib(ch).second)
				data->N_pe_result[exp][ch] = data->N_pe_double_I[exp][ch];
			else
				data->N_pe_result[exp][ch] = data->N_pe_direct[exp][ch];
		}
		if (!data->N_pe_PMT3.empty())
			data->N_pe_PMT3[exp] = PMT3_avr_S2_S[exp] / calibr_info.getPMT_S1pe(0);
		if (!data->N_pe_PMT1.empty())
			data->N_pe_PMT1[exp] = PMT1_avr_S2_S[exp] / calibr_info.getPMT_S1pe(1);
	}
}

void PostProcessor::update_physical(void)
{
	switch (current_type){
	case Type::MPPC_double_I:
	{
		if (current_setups->N_gauss>0 && current_setups->fitted)
			avr_double_I[current_exp_index][mppc_channel_to_index(current_channel)] = current_setups->par_val[1];
		else {
			std::cout << "Warning! No double integral for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		if (current_setups->N_gauss>0 && current_setups->fitted)
			avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = current_setups->par_val[1];
		else {
			std::cout << "Warning! No S2 area for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	case Type::MPPC_Ss:
	{
		CalibrationInfo::S1pe_method meth = calibr_info.get_method(current_exp_index, current_channel);
		if (meth == CalibrationInfo::Ignore)
			break;
		if (meth == CalibrationInfo::UsingMean){
			double val = 0;
			int weight = 0;
			int run_size = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)].size();
			DVECTOR cut_data(1);
			for (auto run = 0; run != run_size; ++run){
				cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
				for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
					if (false == cut->GetAccept(run))
						goto cutted;
				}
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto cutted;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto cutted;
				}//applies drawn limits
				++weight;
				val += cut_data[0];
			cutted:;
			}
			if (0==weight)
				std::cout << "Warning! No mean calibration Ss value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			else
				calibr_info.set_S1pe(current_channel, current_exp_index, val / (double)weight);
			break;
		}
		if (meth == CalibrationInfo::Using1pe || meth == CalibrationInfo::Using1pe2pe){
			if (current_setups->N_gauss>0 && current_setups->fitted)
				calibr_info.set_S1pe(current_channel, current_exp_index, current_setups->par_val[1]);
			else {
				std::cout << "Warning! No calibration S1pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		if (meth == CalibrationInfo::Using2pe || meth == CalibrationInfo::Using1pe2pe){
			if (current_setups->N_gauss>=2 && current_setups->fitted)
				calibr_info.set_S2pe(current_channel, current_exp_index, current_setups->par_val[4]);
			else {
				std::cout << "Warning! No calibration S2pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		break;
	}
	case Type::PMT_S2_S:
	{
		if (current_setups->N_gauss > 0 && current_setups->fitted){
			if (0 == current_channel)
				PMT3_avr_S2_S[current_exp_index] = current_setups->par_val[1];
			if (0 == current_channel)
				PMT1_avr_S2_S[current_exp_index] = current_setups->par_val[1];
		}
		else {
			std::cout << "Warning! No S2 area for PMT " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	}
}

TF1* PostProcessor::create_fit_function(HistogramSetups* func)
{
	TF1 *fit_func;
	std::string func_name;
	for (int ng = 0; ng < current_setups->N_gauss; ++ng){
		func_name += "gaus(" + std::to_string(ng * 3) + ")";
		if (ng != current_setups->N_gauss - 1)
			func_name += "+";
	}
	if (func_name.empty())
		return NULL;
	else {
		fit_func = new TF1(func_name.c_str(), current_setups->left_drawn_limit, current_setups->right_drawn_limit, current_setups->N_gauss * 3);
		for (int par = 0; (par < current_setups->N_gauss * 3) && (par < current_setups->par_val.size()); ++par){
			fit_func->SetParLimits(par, current_setups->par_left_limits[par], current_setups->par_right_limits[par]);
			fit_func->SetParameter(par, current_setups->par_val[par]);
		}
	}
	return fit_func;
}

void PostProcessor::update_fit_function(void)
{
	current_setups->fitted = false;
	if (NULL != current_fit_func)
		current_fit_func->Delete();
	current_fit_func = create_fit_function(current_setups);
}

void PostProcessor::add_hist_cut(std::function<bool(DVECTOR &vals)> &picker, std::string name)
{
	current_setups->display_hist_cuts.push_back(EventCut(0, EventCut::HistCut, name));
	current_setups->display_hist_cuts.back().SetPicker(picker);
	std::pair<double, double> x_lims = hist_x_limits();
	current_setups->left_limit = x_lims.first;
	current_setups->right_limit = x_lims.second;
	update();
}
void PostProcessor::remove_hist_cut(std::string name)
{
	for (auto i = current_setups->display_hist_cuts.rbegin(); i != current_setups->display_hist_cuts.rend(); ++i){
		if (i->GetName() == name){
			current_setups->display_hist_cuts.erase(i.base());
			std::pair<double, double> x_lims = hist_x_limits();
			current_setups->left_limit = x_lims.first;
			current_setups->right_limit = x_lims.second;
			return;
		}
	}
	if (!current_setups->display_hist_cuts.empty()) //in case not found by name
		current_setups->display_hist_cuts.erase(current_setups->display_hist_cuts.end()-1);
	std::pair<double, double> x_lims = hist_x_limits();
	current_setups->left_limit = x_lims.first;
	current_setups->right_limit = x_lims.second;
	update();
}
void PostProcessor::set_as_run_cut(std::string name)//adds current drawn_limits in HistogramSetups to runs cut (from current exp, channel and type)
{
	int run_size = is_PMT_type(current_type) ? 
		(data->PMT3_peaks.empty() ? data->PMT1_peaks[current_exp_index].size() : data->PMT3_peaks[current_exp_index].size())
		:data->double_I[current_exp_index][mppc_channel_to_index(current_channel)].size();
	RunCuts[current_exp_index].push_back(EventCut(run_size, EventCut::RunCut, name));
	RunCuts[current_exp_index].back().SetChannel(current_channel);
	RunCuts[current_exp_index].back().SetExperiment(current_exp_index);
	RunCuts[current_exp_index].back().SetType(current_type);
	//RunCuts[current_exp_index].back().SetPicker();//TODO: figure out how can I set picker here (not necessary at the moment)
	switch (current_type)
	{
	case Type::MPPC_double_I:
	{
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			cut_data[0] = data->double_I[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _0cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _0cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, true);
		_0cutted:;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _1cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _1cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, true);
		_1cutted:;
		}
		break;
	}
	case Type::MPPC_Ss:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _2cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _2cutted1;
				}
				RunCuts[current_exp_index].back().SetAccept(run, true);
			_2cutted1:;
			}
		}
		break;
	}
	case Type::MPPC_tstart:
	{}
	case Type::MPPC_tboth:
	{
		int run_size = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			cut_data[0] = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _3cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _3cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, true);
		_3cutted:;
		}
		if (current_type == Type::MPPC_tstart)
			break;
	}
	case Type::MPPC_tfinal:
	{
		int run_size = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			cut_data[0] = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _4cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _4cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, true);
		_4cutted:;
		}
		break;
	}
	case Type::MPPC_t_S:
	{
		int run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _5cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _5cutted1;
				}
				RunCuts[current_exp_index].back().SetAccept(run, true);
			_5cutted1:;
			}
		_5cutted:;
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(5);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			double S2 = 0;
			cut_data[0] = 0; //reserved for S2 cut
			cut_data[1] = 0; //the 4 below are for S2 selection
			cut_data[2] = 0;
			cut_data[3] = 0;
			cut_data[4] = 0;
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[4] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _6cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _6cutted1;
				}
				S2 += cut_data[1];
			_6cutted1:;
			}
			cut_data[0] = S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _6cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (false == cut->GetPicker()(cut_data))
					goto _6cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, true);
		_6cutted:;
		}
		break;
	}
	case Type::PMT_Ss:
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _7cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _7cutted1;
				}
				RunCuts[current_exp_index].back().SetAccept(run, true);
			_7cutted1:;
			}
		_7cutted:;
		}
		break;
	}
	case Type::PMT_t_S:
	{
		int run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		DVECTOR cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, false);
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (int pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _8cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (false == cut->GetPicker()(cut_data))
						goto _8cutted;
				}
				RunCuts[current_exp_index].back().SetAccept(run, true);
			_8cutted1:;
			}
		_8cutted:;
		}
		break;
	}
	}
	update();
}
//if a single exp,ch,type produces several EventCuts, unset must be called respective amount of times
void PostProcessor::unset_as_run_cut(std::string name)//deletes current exp,ch and type from current cuts (if present) deletes from back, that is
{
	for (auto i = RunCuts[current_exp_index].rbegin(); i != RunCuts[current_exp_index].rend(); ++i){
		if (i->GetName() == name){
			RunCuts[current_exp_index].erase(i.base());
			update();
			return;
		}
	}
	if (!RunCuts[current_exp_index].empty()) { //in case not found by name
		RunCuts[current_exp_index].erase(current_setups->display_hist_cuts.end() - 1);
		update();
	}
}
void PostProcessor::do_fit(bool upd_vis)
{
	update_fit_function();
	if (NULL != current_fit_func){
		if (is_TH1D_hist()){
			current_hist1->Fit(current_fit_func);
			current_setups->fitted = true;
			for (int par = 0; par < current_setups->par_val.size(); ++par)
				current_setups->par_val[par] = current_fit_func->GetParameter(par);
			//if (upd_vis)
				update();
		}
	}
}

void PostProcessor::set_N_bins(int N)
{
	current_setups->N_bins = std::max(N, 1);
	update();
}

void PostProcessor::set_limits(double left, double right)
{
	double _left = std::min(left, right);
	double _right = std::max(left, right);
	
	std::function<bool(DVECTOR &vals)> picker;
	if (current_type == Type::PMT_S2_S) {//TODO: do not forget to modify this function when new types are added
		picker = [_left, _right](DVECTOR &vals) {
			if ((vals[1] == -2) && (vals[2] == -2) && (vals[3] == -2) && (vals[4] == -2))
				return ((vals[0] <= _right) && (vals[0] >= _left));//S2 value selection (derived from accepted peaks)
			else
				return true;//individual peak selection
		};
	}
	else {
		picker = [_left, _right](DVECTOR &vals)
		{	return ((vals[0] <= _right) && (vals[0] >= _left));	};
	}

	std::string name = "_histogram_limits_";
	for (auto i = current_setups->display_hist_cuts.rbegin(); i != current_setups->display_hist_cuts.rend(); ++i)
		if (i->GetName() == name){
			current_setups->display_hist_cuts.erase(i.base());
			return;
		}
	add_hist_cut(picker, name);
	current_setups->left_limit = _left;
	current_setups->right_limit = _right;
	update();
}

void PostProcessor::set_drawn_limits(double left, double right)
{
	double _left = std::min(left, right);
	double _right = std::max(left, right);
	std::function<bool(DVECTOR &vals)> picker;
	if (current_type == Type::PMT_S2_S) {//TODO: do not forget to modify this function when new types are added
		picker = [_left, _right](DVECTOR &vals) {
			if ((vals[1] == -2) && (vals[2] == -2) && (vals[3] == -2) && (vals[4] == -2))
				return ((vals[0] <= _right) && (vals[0] >= _left));//S2 value selection (derived from accepted peaks)
			else
				return true;//individual peak selection
		};
	}
	else {
		picker = [_left, _right](DVECTOR &vals)
		{	return ((vals[0] <= _right) && (vals[0] >= _left));	};
	}

	std::string name = "_histogram_drawn_limits_";
	for (auto i = current_setups->phys_hist_cuts.rbegin(); i != current_setups->phys_hist_cuts.rend(); ++i)
		if (i->GetName() == name){
			current_setups->phys_hist_cuts.erase(i.base());
			return;
		}
	current_setups->phys_hist_cuts.push_back(EventCut(0, EventCut::HistCut, name));
	current_setups->phys_hist_cuts.back().SetPicker(picker);
	current_setups->left_drawn_limit = std::max(current_setups->left_limit,_left);
	current_setups->right_drawn_limit = std::min(current_setups->right_limit, _right);
	update();
}

void PostProcessor::unset_limits(void)
{
	remove_hist_cut("_histogram_limits_");
	update();
}

void PostProcessor::unset_drawn_limits(void)
{
	remove_hist_cut("_histogram_drawn_limits_");
	update();
}

void PostProcessor::set_fit_gauss(int N)
{
	N = std::max(N, 0);
	int was_N = current_setups->N_gauss;
	current_setups->N_gauss = N;
	current_setups->par_val.resize(current_setups->N_gauss * 3);
	current_setups->par_left_limits.resize(current_setups->N_gauss * 3);
	current_setups->par_right_limits.resize(current_setups->N_gauss * 3);
	if (was_N < N){
		int _N_in_hist = numOfFills();
		for (int nn = was_N; nn != current_setups->N_gauss; ++nn){
			current_setups->par_left_limits[nn] = 0;
			current_setups->par_right_limits[nn] = std::max(1, 2 * (int)std::sqrt(_N_in_hist));
			current_setups->par_val[nn] = (int)std::sqrt(_N_in_hist);

			current_setups->par_left_limits[nn + 1] = current_setups->left_limit;
			current_setups->par_right_limits[nn + 1] = current_setups->right_limit;
			current_setups->par_val[nn + 1] = 0.5*(current_setups->par_left_limits[1] + current_setups->par_right_limits[1]);

			current_setups->par_left_limits[nn + 2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
			current_setups->par_right_limits[nn + 2] = (current_setups->par_right_limits[1] - current_setups->par_left_limits[1]);
			current_setups->par_val[nn + 2] = 0.5*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);
		}
	}
	update_fit_function();
}

void PostProcessor::set_parameter_val(int index, double val)
{
	if ((index < 0) || (index >= 3 * current_setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	current_setups->par_val[index] = val;
}

void PostProcessor::set_parameter_limits(int index, double left, double right)
{
	if ((index < 0) || (index >= 3 * current_setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	current_setups->par_left_limits[index] = std::min(left, right);
	current_setups->par_left_limits[index] = std::max(left, right);
}

void PostProcessor::new_canvas(void)
{
	current_canvas = new TCanvas((std::string("Inter_canvas_") + std::to_string(canvas_n)).c_str(), (std::string("Inter_canvas_") + std::to_string(canvas_n)).c_str());
	++canvas_n;
	update();
}

void PostProcessor::status(bool full)
{
	std::cout << "Current experiment: " << data->exp_area.experiments[current_exp_index]
		<< " (" << data->Fields[current_exp_index] << " kV)" << std::endl;
	std::cout << "Current channel: " << current_channel<< std::endl;
	std::cout << "Current type: " << type_name(current_type)<<std::endl;

	if (full){
		std::cout << "Current_setups: " << std::hex << current_setups << std::dec << std::endl;
		std::cout << "\tleft_limit " << current_setups->left_limit << std::endl;
		std::cout << "\tright_limit " << current_setups->right_limit << std::endl;
		std::cout << "\tdisplay_hist_cuts [" << current_setups->display_hist_cuts.size() << "]: ";
		for (auto i = current_setups->display_hist_cuts.begin(), _end_ = current_setups->display_hist_cuts.end(); i != _end_; ++i)
			std::cout << i->GetName() << (i == (_end_ - 1)) ? "" : " ";

		std::cout << "\tleft_drawn_limit " << current_setups->left_drawn_limit << std::endl;
		std::cout << "\tright_drawn_limit " << current_setups->right_drawn_limit << std::endl;
		std::cout << "\tphys_hist_cuts [" << current_setups->phys_hist_cuts.size() << "]: ";
		for (auto i = current_setups->phys_hist_cuts.begin(), _end_ = current_setups->phys_hist_cuts.end(); i != _end_; ++i)
			std::cout << i->GetName() << (i == (_end_ - 1)) ? "" : " ";

		std::cout << "\tN_gauss " << current_setups->N_gauss << std::endl;
		std::cout << "\tN_bins " << current_setups->N_bins << std::endl;
		std::cout << "\tpar_val [" << current_setups->par_val.size() << "]: ";
		for (auto i = current_setups->par_val.begin(), _end_ = current_setups->par_val.end(); i != _end_; ++i)
			std::cout << *i << (i == (_end_ - 1)) ? "" : " ";
		std::cout << std::endl;
		std::cout << "\tpar_left_limits [" << current_setups->par_left_limits.size() << "]: ";
		for (auto i = current_setups->par_left_limits.begin(), _end_ = current_setups->par_left_limits.end(); i != _end_; ++i)
			std::cout << *i << (i == (_end_ - 1)) ? "" : " ";
		std::cout << std::endl;
		std::cout << "\tpar_right_limits [" << current_setups->par_right_limits.size() << "]: ";
		for (auto i = current_setups->par_right_limits.begin(), _end_ = current_setups->par_right_limits.end(); i != _end_; ++i)
			std::cout << *i << (i == (_end_ - 1)) ? "" : " ";
		std::cout << std::endl;
		std::cout << "\tfitted " << current_setups->fitted << std::endl;
	}
}
