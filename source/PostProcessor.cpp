#include "PostProcessor.h"

PostProcessor::PostProcessor(AllExperimentsResults* _data) : 
AnalysisStates(_data->mppc_channels,_data->pmt_channels, _data->exp_area.experiments), calibr_info(this)
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
	current_canvas = new TCanvas((std::string("Int_ter_canvas_") + std::to_string(canvas_n)).c_str(), (std::string("Int_ter_canvas_") + std::to_string(canvas_n)).c_str());
	current_canvas->cd();
	++canvas_n;
	for (auto exp = data->exp_area.experiments.begin(); exp != data->exp_area.experiments.end(); ++exp){
		manual_setups.push_back(std::deque<std::deque<HistogramSetups*>>());
		avr_S2_S.push_back(std::deque<Double_t>());
		avr_Double_I.push_back(std::deque<Double_t>());
		PMT3_avr_S2_S.push_back(-1);
		PMT1_avr_S2_S.push_back(-1);
		RunCuts.push_back(std::deque<EventCut>());
		for (auto ch = data->mppc_channels.begin(); ch != data->mppc_channels.end(); ++ch){
			manual_setups.back().push_back(std::deque<HistogramSetups*>());
			for (Int_t h = MPPC_Ss; h != ((Int_t)PMT_t_S + 1);++h)
				manual_setups.back().back().push_back(NULL);
			avr_S2_S.back().push_back(-1);
			avr_Double_I.back().push_back(-1);
		}
	}

	current_setups = new HistogramSetups();
	set_default_hist_setups();
	update(All);
}

Bool_t PostProcessor::is_TH1D_hist()
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

void PostProcessor::print_hist(void)
{
	print_hist(current_channel,current_exp_index,current_type);
}

void PostProcessor::print_hist(int ch, int exp_ind, Type type)
{
	std::string name = OUTPUT_DIR + (is_PMT_type(type) ? OUTPUT_PMT_PICS : OUTPUT_MPPCS_PICS) + g_data->exp_area.experiments[exp_ind]
		+ "/" + (is_PMT_type(type) ? "PMT_" : "MPPC_") + std::to_string(ch)
		+ "/" + (is_PMT_type(type) ? "PMT_" : "MPPC_") + std::to_string(ch)
		+ type_name(type)+".png";
	current_canvas->SaveAs(name.c_str(), "png");
}


Bool_t PostProcessor::StateChange(Int_t to_ch, Int_t to_exp, Type to_type, Int_t from_ch, Int_t from_exp, Type from_type, Bool_t save)
{
	if (save){
		print_hist(from_ch,from_exp,from_type);
		post_processor->set_hist_setups(current_setups, data->exp_area.experiments[from_exp], from_ch, from_type);
	}
	if (!AnalysisStates::StateChange(to_ch, to_exp, to_type, from_ch, from_exp, from_type,save)){
		return kFALSE; //no change
	}
	current_setups = get_hist_setups(data->exp_area.experiments[to_exp], to_ch, to_type);
	if (NULL == current_setups){
		current_setups = new HistogramSetups();
		set_default_hist_setups();
	}
	update(All);
	return kTRUE;
}

void PostProcessor::set_hist_setups(HistogramSetups* setups, std::string experiment, Int_t channel, Type type)//does not call update
{
	Int_t exp_ind = data->get_exp_index(experiment);
	if (exp_ind < 0)
		return;
	Int_t ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
	if ((NULL != manual_setups[exp_ind][ch_ind][type]) && (setups != manual_setups[exp_ind][ch_ind][type]))
		delete manual_setups[exp_ind][ch_ind][type];
	manual_setups[exp_ind][ch_ind][type] = setups;
}

HistogramSetups* PostProcessor::get_hist_setups(std::string experiment, Int_t channel, Type type)
{
	Int_t exp_ind = data->get_exp_index(experiment);
	if (exp_ind < 0)
		return NULL;
	Int_t ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return NULL;
	return manual_setups[exp_ind][ch_ind][type];
}

void PostProcessor::save(Int_t channel)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	Int_t ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
	std::vector<Double_t> N_pe_result;
	for (Int_t pt = 0; pt < data->exp_area.experiments.size(); ++pt){
		if (pt <= calibr_info.get_N_calib(channel).second)
			N_pe_result.push_back(data->N_pe_direct[pt][ch_ind]);
		else
			N_pe_result.push_back(data->N_pe_Double_I[pt][ch_ind]);
	}
	std::ofstream file;
	open_output_file(OUTPUT_DIR + OUTPUT_MPPCS_PICS + std::to_string(channel) + "_Npe.txt", file);
	file << "E[kV/cm]\tN_pe_direct\tN_pe_Double_I\tN_pe_result" << std::endl;
	for (Int_t pt = 0; pt < data->exp_area.experiments.size(); ++pt)
		file << data->Fields[pt] << "\t" << data->N_pe_direct[pt][ch_ind] << "\t" << data->N_pe_Double_I[pt][ch_ind] << "\t" << N_pe_result[pt] << std::endl;
	file.close();
}

/* PARAMETERS PASSED TO FUNCTION WRAPPER AS WELL AS CUTTER
type			[0]			[1]			[2]			[3]			[4]
MPPC_Double_I	Double_I	------		------		------		------
MPPC_S2_S		S2_S		------		------		------		------
MPPC_Ss			peak.S		peak.A		peak.left	peak.right	------
MPPC_t_S		peak.S		peak.A		peak.left	peak.right	------ 		=== MPPC_Ss
MPPC_times		peak.S		peak.A		peak.left	peak.right	------		=== MPPC_Ss
MPPC_t_start	time		------		------		------		------
MPPC_t_both		time		------		------		------		------
MPPC_t_final	time		------		------		------		------
MPPC_S2			SUM(peak.S)	peak.S		peak.A		peak.left	peak.right	//composite: cuts for peaks and then derived parameter. Data is derived within loop.
PMT_S2_S		SUM(peak.S)	peak.S		peak.A		peak.left	peak.right	//composite: cuts for peaks and then derived parameter. Data is derived within loop.
PMT_Ss			peak.S		peak.A		peak.left	peak.right	------
PMT_t_S			peak.S		peak.A		peak.left	peak.right	------		=== PMT_Ss
PMT_times		peak.S		peak.A		peak.left	peak.right	------		===	PMT_Ss
*/
void PostProcessor::LoopThroughData(FunctionWrapper* operation)
{
	switch (current_type)
	{
	case Type::MPPC_Double_I:
	{
		Int_t run_size = data->Double_I[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->Double_I[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (kFALSE == cut->GetAccept(run))
					goto _0cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _0cutted;
			}
			(*operation)(cut_data);
			_0cutted:;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		Int_t run_size = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (kFALSE == cut->GetAccept(run))
					goto _1cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _1cutted;
			}
			(*operation)(cut_data);
		_1cutted:;
		}
		break;
	}
	case Type::MPPC_t_S:
	case Type::MPPC_times:
	case Type::MPPC_Ss:
	{
		Int_t run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (kFALSE == cut->GetAccept(run))
					goto _2cutted;
			}
			for (Int_t pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _2cutted1;
				}
				(*operation)(cut_data);
			_2cutted1:;
			}
		_2cutted:;
		}
		break;
	}
	case Type::MPPC_tstart:
	case Type::MPPC_tboth:
	{
		Int_t run_size = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (kFALSE == cut->GetAccept(run))
					goto _3cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _3cutted;
			}
			(*operation)(cut_data);
		_3cutted:;
		}
		if (current_type == Type::MPPC_tstart)
			break;
	}
	case Type::MPPC_tfinal:
	{
		Int_t run_size = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (kFALSE == cut->GetAccept(run))
					goto _4cutted;
			}
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _4cutted;
			}
			(*operation)(cut_data);
		_4cutted:;
		}
		break;
	}
	case Type::MPPC_S2: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
		{
			Int_t run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
			std::vector<Double_t> cut_data(5);
			for (auto run = 0; run != run_size; ++run){
				Double_t S2 = 0;
				cut_data[0] = 0; //reserved for S2 cut
				cut_data[1] = 0; //the 4 below are for S2 selection
				cut_data[2] = 0;
				cut_data[3] = 0;
				cut_data[4] = 0;
				for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
					if (kFALSE == cut->GetAccept(run))//not calculating it here!
						goto _5cutted;
				}
				for (Int_t pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
					pk != pk_end; ++pk){
					cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
					cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
					cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
					cut_data[4] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
					for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut) {
						if (kFALSE == (*cut)(cut_data))
							goto _5cutted1;
					}
					S2 += cut_data[1];
				_5cutted1:;
				}
				cut_data[0] = S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
				cut_data[1] = -2;
				cut_data[2] = -2;
				cut_data[3] = -2;
				cut_data[4] = -2;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _5cutted;
				}
				(*operation)(cut_data);
			_5cutted:;
			}
			break;
		}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		Int_t run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		std::vector<Double_t> cut_data(5);
		for (auto run = 0; run != run_size; ++run){
			Double_t S2 = 0;
			cut_data[0] = 0; //reserved for S2 cut
			cut_data[1] = 0; //the 4 below are for S2 selection
			cut_data[2] = 0;
			cut_data[3] = 0;
			cut_data[4] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _6cutted;
			}
			for (Int_t pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[4] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
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
				if (kFALSE == (*cut)(cut_data))
					goto _6cutted;
			}
			(*operation)(cut_data);
		_6cutted:;
		}
		break;
	}
	case Type::PMT_times:
	case Type::PMT_t_S:
	case Type::PMT_Ss:
	{
		Int_t run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		std::vector<Double_t> cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _7cutted;
			}
			for (Int_t pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run].size() : data->PMT1_peaks[current_exp_index][run].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _7cutted1;
				}
			(*operation)(cut_data);
		_7cutted1:;
			}
		_7cutted:;
		}
		break;
	}
	}
}
//see function for std::vector<Double_t> &vals usage in cuts' picker
void PostProcessor::FillHist(void* p_hist)//considers cuts and histogram tipe (void*)==either TH1D* or TH2D*
{
	FunctionWrapper* histogram_filler = new FunctionWrapper(p_hist);
	CUTTER filler_op;
	switch (current_type)
	{
	case Type::MPPC_Double_I:
	case Type::MPPC_S2_S:
	case Type::MPPC_Ss:
	case Type::MPPC_tstart:
	case Type::MPPC_tboth:
	case Type::MPPC_tfinal:
	case Type::MPPC_S2:
	case Type::PMT_S2_S:
	case Type::PMT_Ss:
	{
		filler_op = [](std::vector<double>& pars, void* data) {
			((TH1D*)data)->Fill(pars[0]);
			return true;
		};
		break;
	}
	case Type::PMT_t_S:
	case Type::MPPC_t_S:
	{
		filler_op = [](std::vector<double>& pars, void* data) {
			((TH2D*)data)->Fill(0.5*(pars[2]+pars[3]),pars[0]);
			return true;
		};
		break;
	}
	case Type::PMT_times:
	case Type::MPPC_times:
	{
		filler_op = [](std::vector<double>& pars, void* data) {
			((TH1D*)data)->Fill(0.5*(pars[2]+pars[3]),pars[0]);
			return true;
		};
		break;

	}
	}
	histogram_filler->SetFunction(filler_op);
	LoopThroughData(histogram_filler);
	delete histogram_filler;
}

Int_t PostProcessor::numOfFills(void) //TODO: maybe for the case of filling histograms with weights (MPPC_times and PMT_times) make summing if weights.
{
	int ret = 0;
	FunctionWrapper* histogram_filler = new FunctionWrapper(&ret);
	CUTTER filler_op;
	filler_op = [](std::vector<double>& pars, void* data){
		++(*(int*)data);
		return true;
	};
	histogram_filler->SetFunction(filler_op);
	LoopThroughData(histogram_filler);
	delete histogram_filler;
	return ret;
}

std::pair<Double_t, Double_t> PostProcessor::hist_y_limits(void) //considering cuts
{
	std::pair<Double_t,Double_t> ret(DBL_MAX,-DBL_MAX);
	switch (current_type)
	{
	case Type::PMT_t_S:
	case Type::MPPC_t_S:
	{
		FunctionWrapper* histogram_filler = new FunctionWrapper(&ret);
		CUTTER filler_op = [](std::vector<double> &pars, void* data){
			std::pair<double,double>* p = (std::pair<double,double>*)data;
			p->first = std::min(p->first, pars[0]);
			p->second = std::max(p->second, pars[0]);
			return true;
		};
		histogram_filler->SetFunction(filler_op);
		LoopThroughData(histogram_filler); //not run cuts are always applied here.
		delete histogram_filler;
		break;
	}
	default:
		break;
	}
	return ret;
}

std::pair<Double_t, Double_t> PostProcessor::hist_x_limits(void) //valid only for 2d plots
{
	std::pair<Double_t, Double_t> ret(DBL_MAX, -DBL_MAX);
	FunctionWrapper* histogram_filler = new FunctionWrapper(&ret);
	CUTTER filler_op;
	switch (current_type)
	{
	case Type::MPPC_Double_I:
	case Type::MPPC_S2_S:
	case Type::MPPC_Ss:
	case Type::MPPC_tstart:
	case Type::MPPC_tboth:
	case Type::MPPC_tfinal:
	case Type::MPPC_S2:
	case Type::PMT_S2_S:
	case Type::PMT_Ss:
	{
		filler_op = [](std::vector<double>& pars, void* data) {
			std::pair<double,double>* p = (std::pair<double,double>*)data;
			p->first = std::min(p->first, pars[0]);
			p->second = std::max(p->second, pars[0]);
			return true;
		};
		break;
	}
	case Type::PMT_times:
	case Type::MPPC_times:
	case Type::PMT_t_S:
	case Type::MPPC_t_S:
	{
		filler_op = [](std::vector<double>& pars, void* data) {
			std::pair<double,double>* p = (std::pair<double,double>*)data;
			p->first = std::min(p->first, 0.5*(pars[2]+pars[3]));
			p->second = std::max(p->second, 0.5*(pars[2]+pars[3]));
			return true;
		};
		break;
	}
	}
	histogram_filler->SetFunction(filler_op);
	LoopThroughData(histogram_filler);
	delete histogram_filler;
	return ret;
}

void PostProcessor::set_default_hist_setups(void)//does not affect cuts
{
	std::pair<Double_t, Double_t> x_lims = hist_x_limits();
	current_setups->left_limit = x_lims.first;
	current_setups->right_limit = x_lims.second;
	current_setups->left_drawn_limit = x_lims.first;
	current_setups->right_drawn_limit = x_lims.second;
	x_lims = hist_y_limits();
	current_setups->bottom_limit = std::min(x_lims.second, x_lims.first);
	current_setups->top_limit = std::max(x_lims.second, x_lims.first);
	Int_t _N_ = numOfFills();
	current_setups->N_bins = _N_;
	current_setups->N_bins = std::max(1,(Int_t)std::round(std::sqrt(current_setups->N_bins)));
	current_setups->fitted = kFALSE;

	switch (current_type)
	{
	case Type::MPPC_Double_I:
	case Type::MPPC_S2_S:
	case Type::MPPC_tstart:
	case Type::MPPC_tfinal:
	case Type::MPPC_S2:
	case Type::PMT_S2_S:
	{
		current_setups->N_gauss = 1;
		current_setups->par_val.resize(3, 0);
		current_setups->par_left_limits.resize(3, 0);
		current_setups->par_right_limits.resize(3, 0);
		
		current_setups->par_left_limits[0] = 0;
		current_setups->par_right_limits[0] = std::max(1, 2*(Int_t)std::sqrt(_N_));
		current_setups->par_val[0] = (Int_t)std::sqrt(_N_);

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
		current_setups->par_right_limits[0] = std::max(1, 2 * (Int_t)std::sqrt(_N_));
		current_setups->par_val[0] = (Int_t)std::sqrt(_N_);

		current_setups->par_left_limits[1] = 0.001;
		current_setups->par_right_limits[1] = 0.003;
		current_setups->par_val[1] = 0.5*(current_setups->par_right_limits[1] + current_setups->par_left_limits[1]);

		current_setups->par_left_limits[2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[2] = (current_setups->right_limit - current_setups->left_limit);
		current_setups->par_val[2] = 0.2*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);

		current_setups->par_left_limits[3] = 0;
		current_setups->par_right_limits[3] = std::max(1, (Int_t)(1.5*std::sqrt(_N_)));
		current_setups->par_val[3] = (Int_t)std::sqrt(_N_);

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
		current_setups->par_right_limits[0] = std::max(1, 2 * (Int_t)std::sqrt(_N_));
		current_setups->par_val[0] = (Int_t)std::sqrt(_N_);

		current_setups->par_left_limits[1] = current_setups->left_limit;
		current_setups->par_right_limits[1] = current_setups->right_limit;
		current_setups->par_val[1] = 0.2*(current_setups->par_right_limits[1] + current_setups->par_left_limits[1]);

		current_setups->par_left_limits[2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[2] = (current_setups->right_limit - current_setups->left_limit);
		current_setups->par_val[2] = 0.2*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);

		current_setups->par_left_limits[3] = 0;
		current_setups->par_right_limits[3] = std::max(1, (Int_t)(2*std::sqrt(_N_)));
		current_setups->par_val[3] = (Int_t)std::sqrt(_N_);

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
		current_setups->par_right_limits[0] = std::max(1, 2 * (Int_t)std::sqrt(_N_));
		current_setups->par_val[0] = (Int_t)std::sqrt(_N_);

		current_setups->par_left_limits[1] = current_setups->left_limit;
		current_setups->par_right_limits[1] = current_setups->right_limit;
		current_setups->par_val[1] = 0.5*(current_setups->par_left_limits[1] + current_setups->par_right_limits[1]);

		current_setups->par_left_limits[2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
		current_setups->par_right_limits[2] = (current_setups->par_right_limits[1] - current_setups->par_left_limits[1]);
		current_setups->par_val[2] = 0.5*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);
		break;
	}
	case Type::MPPC_t_S:
	case Type::MPPC_times:
	case Type::PMT_times:
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
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	for (Int_t ch = data->exp_area.channels.get_next_index(); ch != -1; ch = data->exp_area.channels.get_next_index())
		save(ch);
	calibr_info.Save();

	std::ofstream str;
	open_output_file(OUTPUT_DIR+DATA_MPPC_VERSION+"/S2_manual.dat", str);
	str<<"ch\\E"<<"\t";
	for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
		str<<data->Fields[ee]<<((ee==(_end_exp-1))? "\n": "\t");
	for (int ch=0,_end_ch=MPPC_channels.size();ch!=_end_ch;++ch){
		str<<MPPC_channels[ch]<<"\t";
		for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
			str<<avr_S2_S[ee][ch]<<((ee==(_end_exp-1))? "": "\t");
		str<<std::endl;
	}
	str.close();

	open_output_file(OUTPUT_DIR+DATA_MPPC_VERSION+"/double_I.dat", str);
	str<<"ch\\E"<<"\t";
	for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
		str<<data->Fields[ee]<<((ee==(_end_exp-1))? "\n": "\t");
	for (int ch=0,_end_ch=MPPC_channels.size();ch!=_end_ch;++ch){
		str<<MPPC_channels[ch]<<"\t";
		for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
			str<<avr_Double_I[ee][ch]<<((ee==(_end_exp-1))? "": "\t");
		str<<std::endl;
	}
	str.close();

	open_output_file(OUTPUT_DIR+DATA_MPPC_VERSION+"/Npe_direct.dat", str);
	str<<"ch\\E"<<"\t";
	for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
		str<<data->Fields[ee]<<((ee==(_end_exp-1))? "\n": "\t");
	for (int ch=0,_end_ch=MPPC_channels.size();ch!=_end_ch;++ch){
		str<<MPPC_channels[ch]<<"\t";
		for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
			str<<data->N_pe_direct[ee][ch]<<((ee==(_end_exp-1))? "": "\t");
		str<<std::endl;
	}
	str.close();

	open_output_file(OUTPUT_DIR+DATA_MPPC_VERSION+"/Npe_result.dat", str);
	str<<"ch\\E"<<"\t";
	for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
		str<<data->Fields[ee]<<((ee==(_end_exp-1))? "\n": "\t");
	for (int ch=0,_end_ch=MPPC_channels.size();ch!=_end_ch;++ch){
		str<<MPPC_channels[ch]<<"\t";
		for (int ee=0,_end_exp=experiments.size();ee!=_end_exp;++ee)
			str<<data->N_pe_result[ee][ch]<<((ee==(_end_exp-1))? "": "\t");
		str<<std::endl;
	}
	str.close();
}

void PostProcessor::plot_N_pe(Int_t channel, GraphicOutputManager* gr_man)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	Int_t ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
	std::vector<Double_t> N_pe_result;
	std::vector<Double_t> N_pe_Double_I_result;
	std::vector<Double_t> N_pe_direct_result;
	for (Int_t pt = 0; pt < data->exp_area.experiments.size(); ++pt){
		N_pe_result.push_back(data->N_pe_result[pt][ch_ind]);
		N_pe_Double_I_result.push_back(data->N_pe_Double_I[pt][ch_ind]);
		N_pe_direct_result.push_back(data->N_pe_direct[pt][ch_ind]);
	}
	Drawing *dr = gr_man->GetDrawing("Npe MPPC#" + std::to_string(channel), ch_ind, /*ParameterPile::*/DrawEngine::Gnuplot);
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
	dr->AddToDraw(data->Fields, N_pe_Double_I_result, "MPPC#" + std::to_string(channel) + "N pe Double_t I", "with line lc rgb '#000000'");
	dr->DrawData();
}

void PostProcessor::update(UpdateState to_update)//TODO: optimize it?
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	current_canvas->cd();
	current_canvas->SetTitle(hist_name().c_str());
	current_canvas->Clear();
	if (to_update&UpdateState::Histogram){
		if (is_TH1D_hist()){
			if (current_hist1)
				current_hist1->Delete();
			current_hist1 = new TH1D(hist_name().c_str(), hist_name().c_str(),
				current_setups->N_bins, current_setups->left_limit, current_setups->right_limit);
			FillHist(current_hist1);
		}
		else {
			if (current_hist2)
				current_hist2->Delete();
			current_hist2 = new TH2D(hist_name().c_str(), hist_name().c_str(),
				current_setups->N_bins, current_setups->left_limit, current_setups->right_limit,
				current_setups->N_bins, current_setups->bottom_limit, current_setups->top_limit);
			FillHist(current_hist2);
		}
	}
	if (to_update&UpdateState::FitFunction){
		update_fit_function();
	}
	if (to_update&UpdateState::Fit) {
		do_fit(kFALSE);
	}
	if (is_TH1D_hist()){
		if (current_hist1)
			current_hist1->Draw();
	} else {
		if (current_hist2)
			current_hist2->Draw();
	}
	if ((current_fit_func)&&(current_setups->fitted))
		current_fit_func->Draw("same");
	if (current_setups->left_drawn_limit > current_setups->left_limit){
		if (NULL == current_vert_line0)
			current_vert_line0 = new TLine();
		current_vert_line0->SetX1(current_setups->left_drawn_limit);
		current_vert_line0->SetX2(current_setups->left_drawn_limit);
		current_vert_line0->SetY1(current_canvas->GetUymin());
		current_vert_line0->SetY2(current_canvas->GetUymax());
		current_vert_line0->SetLineColor(kRed);
		current_vert_line0->Draw("same");
	}
	if (current_setups->right_drawn_limit < current_setups->right_limit){
		if (NULL == current_vert_line1)
			current_vert_line1 = new TLine();
		current_vert_line1->SetX1(current_setups->right_drawn_limit);
		current_vert_line1->SetX2(current_setups->right_drawn_limit);
		current_vert_line1->SetY1(current_canvas->GetUymin());
		current_vert_line1->SetY2(current_canvas->GetUymax());
		current_vert_line1->SetLineColor(kRed);
		current_vert_line1->Draw("same");
	}
	current_canvas->Update();

	if (to_update&UpdateState::Results){
		update_physical();
		update_Npe();
	}
}

//TODO: actually it is more logical to move the code below to CalibrationInfo, but then I'll need to add friends
void PostProcessor::update_Npe(void)
{
	for (Int_t exp = 0, exp_end_ = data->N_pe_direct.size(); exp != exp_end_; ++exp) {
		for (Int_t ch = 0, ch_end_ = MPPC_channels.size(); ch != ch_end_; ++ch) {
			data->N_pe_direct[exp][ch] = avr_S2_S[exp][ch];
			data->N_pe_Double_I[exp][ch] = avr_Double_I[exp][ch];
		}
	}
	calibr_info.recalibrate(data->N_pe_direct, data->N_pe_Double_I, data->Fields);
	for (Int_t exp = 0, exp_end_ = data->N_pe_direct.size(); exp != exp_end_; ++exp) {
		for (Int_t ch = 0, ch_end_ = MPPC_channels.size(); ch != ch_end_; ++ch) {
			if (exp > calibr_info.get_N_calib(MPPC_channels[ch]).first)
				data->N_pe_result[exp][ch] = data->N_pe_Double_I[exp][ch];
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
	case Type::MPPC_Double_I:
	{
		if (current_setups->N_gauss>0 && current_setups->fitted)
			avr_Double_I[current_exp_index][mppc_channel_to_index(current_channel)] = current_setups->par_val[1];
		else {
			if (0==current_setups->N_gauss) {
				Double_t val = 0;
				Int_t weight = 0;
				Int_t run_size = data->Double_I[current_exp_index][mppc_channel_to_index(current_channel)].size();
				std::vector<Double_t> cut_data(1);
				for (auto run = 0; run != run_size; ++run){
					for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
						if (kFALSE == cut->GetAccept(run))
							goto _10cutted;
					}
					cut_data[0] = data->Double_I[current_exp_index][mppc_channel_to_index(current_channel)][run];
					for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _10cutted;
					}
					for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _10cutted;
					}
					++weight;
					val+= cut_data[0];
					_10cutted:;
				}
				if (0==weight)
					std::cout << "Warning! No mean double integral value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else
					avr_Double_I[current_exp_index][mppc_channel_to_index(current_channel)] = val/weight;
			} else
				std::cout << "Warning! No double integral for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	case Type::MPPC_S2:	//TODO: Warning!: these two types overwrite each other
	{
		if (current_setups->N_gauss>0 && current_setups->fitted)
			avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = current_setups->par_val[1];
		else {
			if (0==current_setups->N_gauss) { //Use mean then
				Double_t val = 0;
				Int_t weight = 0;
				Int_t run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
				std::vector<Double_t> cut_data(5);
				double S2;
				for (auto run = 0; run != run_size; ++run){
					for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
						if (kFALSE == cut->GetAccept(run))
							goto _0cutted;
					}
					S2=0;
					for (Int_t pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
						pk != pk_end; ++pk){
						cut_data[0] = 0;
						cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
						cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
						cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
						cut_data[4] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
						for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
							if (kFALSE == (*cut)(cut_data))
								goto _0cutted1;
						}
						for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
							if (kFALSE == (*cut)(cut_data))
								goto _0cutted1;
						}
						S2+=cut_data[1];
					_0cutted1:;
					}
					cut_data[0] = S2;
					cut_data[1] = -2;
					cut_data[2] = -2;
					cut_data[3] = -2;
					cut_data[4] = -2;
					for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _0cutted;
					}
					for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _0cutted;
					}
					++weight;
					val+= cut_data[0];
					_0cutted:;
				}
				if (0==weight)
					std::cout << "Warning! No mean S2 value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else
					avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = val/weight;
			} else
				std::cout << "Warning! No S2 area for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		if (current_setups->N_gauss>0 && current_setups->fitted)
			avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = current_setups->par_val[1];
		else {
			if (0==current_setups->N_gauss) { //Use mean then
				Double_t val = 0;
				Int_t weight = 0;
				Int_t run_size = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)].size();
				std::vector<Double_t> cut_data(1);
				for (auto run = 0; run != run_size; ++run){
					for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
						if (kFALSE == cut->GetAccept(run))
							goto _1cutted;
					}
					cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
					for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _1cutted;
					}
					for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _1cutted;
					}
					++weight;
					val+= cut_data[0];
					_1cutted:;
				}
				if (0==weight)
					std::cout << "Warning! No mean S2 value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else
									avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = val/weight;
			} else
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
			Double_t val = 0;
			Int_t weight = 0;
			Int_t run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
			std::vector<Double_t> cut_data(4);
			for (auto run = 0; run != run_size; ++run){
				for (auto cut = RunCuts[current_exp_index].begin(), c_end_ = RunCuts[current_exp_index].end(); cut != c_end_; ++cut){
					if (kFALSE == cut->GetAccept(run))
						goto _2cutted;
				}
				for (Int_t pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
					pk != pk_end; ++pk){
					cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
					cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
					cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
					cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
					for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _2cutted1;
					}
					for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _2cutted1;
					}
					++weight;
					val+= cut_data[0];
				_2cutted1:;
				}
			_2cutted:;
			}
			if (0==weight)
				std::cout << "Warning! No mean calibration Ss value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			else
				calibr_info.set_S1pe(current_channel, current_exp_index, val / (Double_t)weight);
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
	default:
		break;
	}
}

TF1* PostProcessor::create_fit_function(HistogramSetups* func)
{
	TF1 *fit_func;
	std::string func_name;
	for (Int_t ng = 0; ng < current_setups->N_gauss; ++ng){
		func_name += "gaus(" + std::to_string(ng * 3) + ")";
		if (ng != current_setups->N_gauss - 1)
			func_name += "+";
	}
	if (func_name.empty())
		return NULL;
	else {
		fit_func = new TF1("ff",func_name.c_str(), current_setups->left_drawn_limit, current_setups->right_drawn_limit);
		for (Int_t par = 0; (par < current_setups->N_gauss * 3) && (par < current_setups->par_val.size()); ++par){
			fit_func->SetParLimits(par, current_setups->par_left_limits[par], current_setups->par_right_limits[par]);
			fit_func->SetParameter(par, current_setups->par_val[par]);
		}
	}
	return fit_func;
}

void PostProcessor::update_fit_function(void)
{
	current_setups->fitted = kFALSE;
	if (NULL != current_fit_func)
		current_fit_func->Delete();
	current_fit_func = create_fit_function(current_setups);
}

void PostProcessor::add_hist_cut(FunctionWrapper* picker, std::string name)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	current_setups->display_hist_cuts.push_back(EventCut(0, EventCut::HistCut, name));
	current_setups->display_hist_cuts.back().SetPicker(picker);
	std::pair<Double_t, Double_t> x_lims = hist_x_limits();
	current_setups->left_limit = x_lims.first;
	current_setups->right_limit = x_lims.second;
	if (current_setups->phys_hist_cuts.empty()) { //no drawn limit
		current_setups->left_drawn_limit = x_lims.first;
		current_setups->right_drawn_limit = x_lims.second;
	}
	update(All);
}
void PostProcessor::remove_hist_cut(std::string name)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	for (auto i = current_setups->display_hist_cuts.rbegin(); i != current_setups->display_hist_cuts.rend(); ++i){
		if (i->GetName() == name){
			current_setups->display_hist_cuts.erase(i.base());
			std::pair<Double_t, Double_t> x_lims = hist_x_limits();
			current_setups->left_limit = x_lims.first;
			current_setups->right_limit = x_lims.second;
			return;
		}
	}
	if (!current_setups->display_hist_cuts.empty()) //in case not found by name
		current_setups->display_hist_cuts.erase(current_setups->display_hist_cuts.end()-1);
	std::pair<Double_t, Double_t> x_lims = hist_x_limits();
	current_setups->left_limit = x_lims.first;
	current_setups->right_limit = x_lims.second;
	update(All);
}
void PostProcessor::set_as_run_cut(std::string name)//adds current drawn_limits in HistogramSetups to runs cut (from current exp, channel and type)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	Int_t run_size = is_PMT_type(current_type) ? 
		(data->PMT3_peaks.empty() ? data->PMT1_peaks[current_exp_index].size() : data->PMT3_peaks[current_exp_index].size())
		:data->Double_I[current_exp_index][mppc_channel_to_index(current_channel)].size();
	RunCuts[current_exp_index].push_back(EventCut(run_size, EventCut::RunCut, name));
	RunCuts[current_exp_index].back().SetChannel(current_channel);
	RunCuts[current_exp_index].back().SetExperiment(current_exp_index);
	RunCuts[current_exp_index].back().SetType(current_type);
	//RunCuts[current_exp_index].back().SetPicker();//TODO: figure out how can I set picker here (not necessary at the moment)
	//Can't use LoopThroughData as there is no phys_hist_cuts there. (TODO: may pass additional bool there)
	switch (current_type)
	{
	case Type::MPPC_Double_I:
	{
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
			cut_data[0] = data->Double_I[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _0cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _0cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
		_0cutted:;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
			cut_data[0] = data->S2_S[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _1cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _1cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
		_1cutted:;
		}
		break;
	}
	case Type::MPPC_t_S:
	case Type::MPPC_times:
	case Type::MPPC_Ss:
	{
		Int_t run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
			for (Int_t pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
				pk != pk_end; ++pk){
				cut_data[0] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _2cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _2cutted1;
				}
				RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
			_2cutted1:;
			}
		}
		break;
	}
	case Type::MPPC_tstart:
	case Type::MPPC_tboth:
	{
		Int_t run_size = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
			cut_data[0] = data->S2_start_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _3cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _3cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
		_3cutted:;
		}
		if (current_type == Type::MPPC_tstart)
			break;
	}
	case Type::MPPC_tfinal:
	{
		Int_t run_size = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)].size();
		std::vector<Double_t> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
			cut_data[0] = data->S2_finish_t[current_exp_index][mppc_channel_to_index(current_channel)][run];
			for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _4cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _4cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
		_4cutted:;
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		Int_t run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		std::vector<Double_t> cut_data(5);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
			Double_t S2 = 0;
			cut_data[0] = 0; //reserved for S2 cut
			cut_data[1] = 0; //the 4 below are for S2 selection
			cut_data[2] = 0;
			cut_data[3] = 0;
			cut_data[4] = 0;
			for (Int_t pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size());
				pk != pk_end; ++pk){
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[4] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _6cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
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
				if (kFALSE == (*cut)(cut_data))
					goto _6cutted;
			}
			for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
				if (kFALSE == (*cut)(cut_data))
					goto _6cutted;
			}
			RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
		_6cutted:;
		}
		break;
	}
	case Type::PMT_t_S:
	case Type::PMT_times:
	case Type::PMT_Ss:
	{
		Int_t run_size = (current_channel == 0) ? data->PMT3_peaks[current_exp_index].size() : data->PMT1_peaks[current_exp_index].size();
		std::vector<Double_t> cut_data(4);
		for (auto run = 0; run != run_size; ++run){
			RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
			cut_data[0] = 0;
			cut_data[1] = 0;
			cut_data[2] = 0;
			cut_data[3] = 0;
			for (Int_t pk = 0, pk_end = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run].size() : data->PMT1_peaks[current_exp_index][run].size());
				pk != pk_end; ++pk){
				cut_data[0] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].S : data->PMT1_peaks[current_exp_index][run][pk].S);
				cut_data[1] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].A : data->PMT1_peaks[current_exp_index][run][pk].A);
				cut_data[2] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].left : data->PMT1_peaks[current_exp_index][run][pk].left);
				cut_data[3] = ((current_channel == 0) ? data->PMT3_peaks[current_exp_index][run][pk].right : data->PMT1_peaks[current_exp_index][run][pk].right);
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _7cutted1;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _7cutted1;
				}
				RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
			_7cutted1:;
			}
		_7cutted:;
		}
		break;
	}
	case Type::MPPC_S2: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
		{
			Int_t run_size = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
			std::vector<Double_t> cut_data(5);
			for (auto run = 0; run != run_size; ++run){
				RunCuts[current_exp_index].back().SetAccept(run, kFALSE);
				Double_t S2 = 0;
				cut_data[0] = 0; //reserved for S2 cut
				cut_data[1] = 0; //the 4 below are for S2 selection
				cut_data[2] = 0;
				cut_data[3] = 0;
				cut_data[4] = 0;
				for (Int_t pk = 0, pk_end = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run].size();
					pk != pk_end; ++pk){
					cut_data[1] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].S;
					cut_data[2] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].A;
					cut_data[3] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].left;
					cut_data[4] = data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)][run][pk].right;
					for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _8cutted1;
					}
					for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
						if (kFALSE == (*cut)(cut_data))
							goto _8cutted1;
					}
					S2 += cut_data[1];
				_8cutted1:;
				}
				cut_data[0] = S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
				cut_data[1] = -2;
				cut_data[2] = -2;
				cut_data[3] = -2;
				cut_data[4] = -2;
				for (auto cut = current_setups->display_hist_cuts.begin(), c_end_ = current_setups->display_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _8cutted;
				}
				for (auto cut = current_setups->phys_hist_cuts.begin(), c_end_ = current_setups->phys_hist_cuts.end(); cut != c_end_; ++cut){
					if (kFALSE == (*cut)(cut_data))
						goto _8cutted;
				}
				RunCuts[current_exp_index].back().SetAccept(run, kTRUE);
			_8cutted:;
			}
			break;
		}
	}
	update(All);
}
//if a single exp,ch,type produces several EventCuts, unset must be called respective amount of times
void PostProcessor::unset_as_run_cut(std::string name)//deletes current exp,ch and type from current cuts (if present) deletes from back, that is
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	for (auto i = RunCuts[current_exp_index].rbegin(); i != RunCuts[current_exp_index].rend(); ++i){
		if (i->GetName() == name){
			RunCuts[current_exp_index].erase(i.base());
			update(All);
			return;
		}
	}
	if (!RunCuts[current_exp_index].empty()) { //in case not found by name
		RunCuts[current_exp_index].erase(current_setups->display_hist_cuts.end() - 1);
		update(All);
	}
}
void PostProcessor::do_fit(Bool_t upd_vis)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	//update_fit_function();//considered updated
	if (NULL != current_fit_func){
		if (is_TH1D_hist()){
			current_hist1->Fit(current_fit_func,"RQ");
			current_setups->fitted = kTRUE;
			for (Int_t par = 0; par < current_setups->par_val.size(); ++par)
				current_setups->par_val[par] = current_fit_func->GetParameter(par);
			if (upd_vis)
				update(All);
		}
	}
}

void PostProcessor::set_N_bins(Int_t N)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	current_setups->N_bins = std::max(N, 1);
	update(All);
}

//TODO: do not forget to modify this function when new types are added
void PostProcessor::set_limits(Double_t left, Double_t right)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	Double_t _left = std::min(left, right);
	Double_t _right = std::max(left, right);
	
	FunctionWrapper *picker = new FunctionWrapper(new std::pair<double, double>(_left, _right));
	if (current_type == Type::PMT_S2_S||current_type == Type::MPPC_S2) {
		picker->SetFunction(
		[](std::vector<Double_t> &vals, void* data) {
			if ((vals[1] == -2) && (vals[2] == -2) && (vals[3] == -2) && (vals[4] == -2))
				return ((vals[0] <= ((std::pair<double, double>*)data)->second) && (vals[0] >= ((std::pair<double, double>*)data)->first));//S2 value selection (derived from accepted peaks)
			else
				return kTRUE;//individual peak selection
		});
	}
	else {
		if (current_type == Type::MPPC_times||current_type == Type::PMT_times){
			picker->SetFunction(
				[](std::vector<Double_t> &vals, void* data) {
					return ((0.5*(vals[2]+vals[3]) <= ((std::pair<double, double>*)data)->second) && (0.5*(vals[2]+vals[3]) >= ((std::pair<double, double>*)data)->first));
			});
		} else {
			picker->SetFunction(
				[](std::vector<Double_t> &vals, void* data) {
					return ((vals[0] <= ((std::pair<double, double>*)data)->second) && (vals[0] >= ((std::pair<double, double>*)data)->first));
			});
		}
	}

	std::string name = "_histogram_limits_";
	for (auto i = current_setups->display_hist_cuts.rbegin(); i != current_setups->display_hist_cuts.rend(); ++i)
		if (i->GetName() == name){
			current_setups->display_hist_cuts.erase(i.base());
			break;
		}
	add_hist_cut(picker, name);
	current_setups->left_limit = _left;
	current_setups->right_limit = _right;
	if (current_setups->phys_hist_cuts.empty()) { //no drawn limit
			current_setups->left_drawn_limit = _left;
			current_setups->right_drawn_limit = _right;
		}
	update(All);
}

//TODO: do not forget to modify this function when new types are added
void PostProcessor::set_drawn_limits(Double_t left, Double_t right)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	Double_t _left = std::min(left, right);
	Double_t _right = std::max(left, right);
	FunctionWrapper *picker = new FunctionWrapper(new std::pair<double, double>(_left, _right));
	if (current_type == Type::PMT_S2_S||current_type == Type::MPPC_S2) {
		picker->SetFunction(
			[](std::vector<Double_t> &vals, void* data) {
			if ((vals[1] == -2) && (vals[2] == -2) && (vals[3] == -2) && (vals[4] == -2))
				return ((vals[0] <= ((std::pair<double, double>*)data)->second) && (vals[0] >= ((std::pair<double, double>*)data)->first));//S2 value selection (derived from accepted peaks)
			else
				return kTRUE;//individual peak selection
		});
	}
	else {
		if (current_type == Type::MPPC_times||current_type == Type::PMT_times){
			picker->SetFunction(
				[](std::vector<Double_t> &vals, void* data) {
					return ((0.5*(vals[2]+vals[3]) <= ((std::pair<double, double>*)data)->second) && (0.5*(vals[2]+vals[3]) >= ((std::pair<double, double>*)data)->first));
			});
		} else {
			picker->SetFunction(
				[](std::vector<Double_t> &vals, void* data) {
					return ((vals[0] <= ((std::pair<double, double>*)data)->second) && (vals[0] >= ((std::pair<double, double>*)data)->first));
			});
		}
	}

	std::string name = "_histogram_drawn_limits_";
	for (auto i = current_setups->phys_hist_cuts.rbegin(); i != current_setups->phys_hist_cuts.rend(); ++i)
		if (i->GetName() == name){
			current_setups->phys_hist_cuts.erase(i.base());
			break;
		}
	current_setups->phys_hist_cuts.push_back(EventCut(0, EventCut::HistCut, name));
	current_setups->phys_hist_cuts.back().SetPicker(picker);
	current_setups->left_drawn_limit = std::max(current_setups->left_limit,_left);
	current_setups->right_drawn_limit = std::min(current_setups->right_limit, _right);
	update(All);
}

void PostProcessor::unset_limits(void)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	remove_hist_cut("_histogram_limits_");
	update(All);
}

void PostProcessor::unset_drawn_limits(void)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	remove_hist_cut("_histogram_drawn_limits_");
	update(All);
}

void PostProcessor::set_fit_gauss(Int_t N)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	N = std::max(N, 0);
	Int_t was_N = current_setups->N_gauss;
	current_setups->N_gauss = N;
	current_setups->par_val.resize(current_setups->N_gauss * 3);
	current_setups->par_left_limits.resize(current_setups->N_gauss * 3);
	current_setups->par_right_limits.resize(current_setups->N_gauss * 3);
	if (was_N < N){
		Int_t _N_in_hist = numOfFills();
		for (Int_t nn = was_N; nn != current_setups->N_gauss; ++nn){
			current_setups->par_left_limits[nn] = 0;
			current_setups->par_right_limits[nn] = std::max(1, 2 * (Int_t)std::sqrt(_N_in_hist));
			current_setups->par_val[nn] = (Int_t)std::sqrt(_N_in_hist);

			current_setups->par_left_limits[nn + 1] = current_setups->left_limit;
			current_setups->par_right_limits[nn + 1] = current_setups->right_limit;
			current_setups->par_val[nn + 1] = 0.5*(current_setups->par_left_limits[1] + current_setups->par_right_limits[1]);

			current_setups->par_left_limits[nn + 2] = (current_setups->right_limit - current_setups->left_limit) / current_setups->N_bins;
			current_setups->par_right_limits[nn + 2] = (current_setups->par_right_limits[1] - current_setups->par_left_limits[1]);
			current_setups->par_val[nn + 2] = 0.5*(current_setups->par_left_limits[2] + current_setups->par_right_limits[2]);
		}
	}
	update(FitFunction);
}

void PostProcessor::set_parameter_val(Int_t index, Double_t val)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	if ((index < 0) || (index >= 3 * current_setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	current_setups->par_val[index] = val;
	update(FitFunction);
}

void PostProcessor::set_parameter_limits(Int_t index, Double_t left, Double_t right)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	if ((index < 0) || (index >= 3 * current_setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	current_setups->par_left_limits[index] = std::min(left, right);
	current_setups->par_right_limits[index] = std::max(left, right);
	update(FitFunction);
}

void PostProcessor::new_canvas(void)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}
	current_canvas = new TCanvas((std::string("Int_ter_canvas_") + std::to_string(canvas_n)).c_str(), (std::string("Int_ter_canvas_") + std::to_string(canvas_n)).c_str());
	++canvas_n;
	update(All);
}

void PostProcessor::status(Bool_t full)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		return;
	}

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
			std::cout << (i->GetName()) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;

		std::cout << "\tleft_drawn_limit " << current_setups->left_drawn_limit << std::endl;
		std::cout << "\tright_drawn_limit " << current_setups->right_drawn_limit << std::endl;
		std::cout << "\tphys_hist_cuts [" << current_setups->phys_hist_cuts.size() << "]: ";
		for (auto i = current_setups->phys_hist_cuts.begin(), _end_ = current_setups->phys_hist_cuts.end(); i != _end_; ++i)
			std::cout << (i->GetName()) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;

		std::cout << "\tN_gauss " << current_setups->N_gauss << std::endl;
		std::cout << "\tN_bins " << current_setups->N_bins << std::endl;
		std::cout << "\tpar_val [" << current_setups->par_val.size() << "]: ";
		for (auto i = current_setups->par_val.begin(), _end_ = current_setups->par_val.end(); i != _end_; ++i)
			std::cout << (*i) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;
		std::cout << "\tpar_left_limits [" << current_setups->par_left_limits.size() << "]: ";
		for (auto i = current_setups->par_left_limits.begin(), _end_ = current_setups->par_left_limits.end(); i != _end_; ++i)
			std::cout << (*i) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;
		std::cout << "\tpar_right_limits [" << current_setups->par_right_limits.size() << "]: ";
		for (auto i = current_setups->par_right_limits.begin(), _end_ = current_setups->par_right_limits.end(); i != _end_; ++i)
			std::cout << (*i) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;
		std::cout << "\tfitted " << current_setups->fitted << std::endl;
	}
}
