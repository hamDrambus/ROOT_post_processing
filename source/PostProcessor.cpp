#include <algorithm>
#include "PostProcessor.h"
#include "StateData.h"

PostProcessor::PostProcessor(AllExperimentsResults* _data) : 
CanvasSetups(_data->mppc_channels,_data->pmt_channels, _data->exp_area.experiments), calibr_info(this, calibration_file), event_canvas(NULL)
{
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	data = _data;
	StateChange(current_channel, current_exp_index, current_type, canvas_ind, current_channel, current_exp_index, current_type, canvas_ind); //To create HistogramSetups at the start of the program
	update();
}

void PostProcessor::print_hist(std::string path, bool png_only)
{
	int ch = current_channel;
	Type type = current_type;
	if (type == Correlation_x)
		type = _x_corr;
	if (type == Correlation_y)
		type = _y_corr;
	int exp_ind = current_exp_index;
	std::string name = path;
	std::string device;
	if (name_scheme_version == name_scheme_v1) {
		device = (isPMTtype(current_type) ? "PMT" : "MPPC");
	}
	if (name_scheme_version == name_scheme_v2) {
		device = (isPMTtype(current_type) ? DATA_PMT_VERSION : DATA_MPPC_VERSION);
	}
	if (name=="") {
		if (isMultichannel(type)) {
			name = data_output_path + g_data->exp_area.experiments[exp_ind] + "/" + device + "_" + type_name(type);
		} else {
			name = data_output_path + g_data->exp_area.experiments[exp_ind] + "/" + device + "_" + std::to_string(ch) + "_" + type_name(type);
		}
	}
	std::ofstream str;
	if (!png_only) {
		open_output_file(name+".hdata", str, std::ios_base::trunc | std::ios_base::binary);
		std::size_t real_size = numOfFills(false);
		str.write((char*)&real_size, sizeof(std::size_t));
	}
	struct temp_data {
		std::ofstream* str;
		int ch_size;
	} st_data;
	st_data.str = &str;
	st_data.ch_size = isPMTtype(type) ? PMT_channels.size() : MPPC_channels.size();
	FunctionWrapper* writer_to_file = new FunctionWrapper(&st_data);
	switch (type)
	{
	case MPPC_Npe_profile:
	{
		writer_to_file->SetFunction([](std::vector<double> &pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[1], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[2], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_Npe_profile_x:
	{
		writer_to_file->SetFunction([](std::vector<double> &pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[2], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_Npe_profile_y:
	{
		writer_to_file->SetFunction([](std::vector<double> &pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[1], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[2], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_coord:
	{
		writer_to_file->SetFunction([](std::vector<double> &pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[((temp_data*)data)->ch_size], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[((temp_data*)data)->ch_size+1], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_coord_x:
	case MPPC_Npe_sum:
	case MPPC_N_sum:
	case MPPC_S_sum:
	case PMT_Npe_sum:
	case PMT_S_sum:
	case PMT_sum_N:
	{
		writer_to_file->SetFunction([](std::vector<double> &pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[((temp_data*)data)->ch_size], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_coord_y:
	{
		writer_to_file->SetFunction([](std::vector<double> &pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[((temp_data*)data)->ch_size+1], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_coord_disp:
	{
		writer_to_file->SetFunction([](std::vector<double> &pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[((temp_data*)data)->ch_size+2], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_tbS:
	case PMT_tbS:
	case MPPC_tbS_sum:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[4], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_tbN:
	case PMT_tbN:
	case PMT_tbNpe:
	case MPPC_tbNpe_sum:
	case MPPC_tbN_sum:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			double weight = pars[5] >= 0 ? pars[5] : 0;
			((temp_data*)data)->str->write((char*)&pars[4], sizeof(double));
			((temp_data*)data)->str->write((char*)&weight, sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_t_S:
	case PMT_t_S:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[4], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_A_S:
	case PMT_A_S:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[1], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
			return true;
		});
		break;
	}
	case PMT_S2_S:
	case MPPC_S2:
	case MPPC_Ss:
	case PMT_Ss:
	case PMT_trigger_bNpe:
	case PMT_trigger_bNpeaks:
	case PMT_trigger_bS:
	case PMT_trigger_fit:
	case PMT_trigger_fit_chi2:
	case MPPC_trigger_fit:
	case MPPC_trigger_fit_chi2:
	case MPPC_trigger_avg:
	case PMT_T_sum:
	case MPPC_shape_fit:
	case PMT_shape_fit:
	case EventNumber:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
			return true;
		});
		break;
	}
	case MPPC_As:
	case PMT_As:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[1], sizeof(double));
			return true;
		});
		break;
	}
	case Correlation:
	case CorrelationAll:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
			((temp_data*)data)->str->write((char*)&pars[1], sizeof(double));
			return true;
		});
		break;
	}
	}
	Operation op;
	op.operation = writer_to_file;
	op.apply_run_cuts = true;
	op.apply_hist_cuts = true;
	op.apply_phys_cuts = false;
	std::vector<Operation> vec(1, op);
	if (!png_only)
		LoopThroughData(vec, ch, current_type);
	delete writer_to_file;
	str.close();
	if (NULL!=get_current_canvas()) {
		ensure_file(name+".png");
		get_current_canvas()->SaveAs((name+".png").c_str(), "png");
	}
}

Bool_t PostProcessor::StateChange(int to_ch, int to_exp, Type to_type, std::size_t to_canvas, int from_ch, int from_exp, Type from_type, std::size_t from_canvas)
{
	if (!CanvasSetups::StateChange(to_ch, to_exp, to_type, to_canvas, from_ch, from_exp, from_type, from_canvas)) {
		return kFALSE; //no change
	}
	Invalidate(invFitFunction|invFit|invHistogram);
	update();
	return kTRUE;
}

Bool_t PostProcessor::CorrelationXChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type)
{
	if (!CanvasSetups::CorrelationXChange(exp_index, to_ch, to_type, from_ch, from_type)) {
		HistogramSetups *setups = get_hist_setups(exp_index, to_ch, to_type);
		if (NULL != setups) { //Replace current setups with those of new type-channel if there are any
			set_hist_setups(setups, exp_index, to_ch, AStates::Correlation_x);
			Invalidate(invAll);
			update();
		}
		return kFALSE;
	}
	HistogramSetups *setups = get_hist_setups(exp_index, to_ch, to_type);
	if (NULL != setups) { //Replace current setups with those of new type-channel if there are any or create new ones otherwise
		set_hist_setups(setups, exp_index, to_ch, AStates::Correlation_x);
	} else {
		Invalidate(invDefault);
		setups = new HistogramSetups(channel_list(AStates::Correlation_x));
		set_hist_setups(setups, exp_index, to_ch, AStates::Correlation_x);
		delete setups;
	}
	Invalidate(invAll);
	update();
	return kTRUE;
}

Bool_t PostProcessor::CorrelationYChange(int exp_index, int to_ch, Type to_type, int from_ch, Type from_type)
{
	if (!CanvasSetups::CorrelationYChange(exp_index, to_ch, to_type, from_ch, from_type)) {
		HistogramSetups *setups = get_hist_setups(exp_index, to_ch, to_type);
		if (NULL != setups) { //Replace current setups with those of new type-channel if there are any
			set_hist_setups(setups, exp_index, to_ch, AStates::Correlation_y);
			Invalidate(invAll);
			update();
		}
		return kFALSE;
	}
	HistogramSetups *setups = get_hist_setups(exp_index, to_ch, to_type);
	if (NULL != setups) { //Replace current setups with those of new type-channel if there are any or create new ones otherwise
		set_hist_setups(setups, exp_index, to_ch, AStates::Correlation_y);
	} else {
		Invalidate(invDefault);
		setups = new HistogramSetups(channel_list(AStates::Correlation_y));
		set_hist_setups(setups, exp_index, to_ch, AStates::Correlation_y);
		delete setups;
	}
	Invalidate(invAll);
	update();
	return kTRUE;
}

void PostProcessor::save(int channel)
{
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	int ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
}

/* PARAMETERS PASSED TO FUNCTION WRAPPER (*operation) AS WELL AS CUTTER
type			[0]			[1]			[2]			[3]			[4]			[5]
MPPC_Ss			peak.S		peak.A		peak.left	peak.right	peak.t		Npe
MPPC_t_S		peak.S		peak.A		peak.left	peak.right	peak.t 		Npe		=== MPPC_Ss
MPPC_A_S		peak.S		peak.A		peak.left	peak.right	peak.t 		Npe		=== MPPC_Ss
MPPC_tbS		peak.S		peak.A		peak.left	peak.right	peak.t		Npe		=== MPPC_Ss
MPPC_tbN		peak.S		peak.A		peak.left	peak.right	peak.t		Npe		=== MPPC_Ss
MPPC_tbS_sum	peak.S		peak.A		peak.left	peak.right	peak.t		Npe
MPPC_tbNpe_sum	peak.S		peak.A		peak.left	peak.right	peak.t		Npe
MPPC_tbN_sum	peak.S		peak.A		peak.left	peak.right	peak.t		Npe
MPPC_S2																					//composite: cuts for peaks and then derived parameter. Data is derived within loop.
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		Npe			//cuts with corresponding channel, no need to check ==-2
	2nd level:	SUM(peak.S)	-2			-2			-2			-2						//cuts with channel -1 called.
MPPC_coord===MPPC_coord_x===MPPC_coord_y==MPPC_coord_disp								//composite: cuts for peaks and then derived parameters. Data is derived within loop.
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		Npe			//per channel, cuts with corresponding channel called.
	2nd level:	Npe[0]	Npe[1]	...	Npe[MPPC_channels.size()-1]		X derived		Y derived	2D_Dispersion	//cuts with channel -1 called.
MPPC_Npe_sum
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		Npe			//per channel, cuts with corresponding channel called.
	2nd level:	Npe[0]	Npe[1]	...	Npe[MPPC_channels.size()-1]		NpeSum				//cuts with channel -1 called.
PMT_S2_S																				//composite: cuts for peaks and then derived parameter. Data is derived within loop.
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		Npe			//cuts with corresponding channel, no need to check ==-2
	2nd level:	SUM(peak.S)	-2			-2			-2			-2						//cuts with channel -1 called.
PMT_Ss			peak.S		peak.A		peak.left	peak.right	peak.t		Npe
PMT_tbS			peak.S		peak.A		peak.left	peak.right	peak.t		Npe		=== PMT_Ss
PMT_tbN			peak.S		peak.A		peak.left	peak.right	peak.t		Npe		===	PMT_Ss
PMT_sum_N
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		Npe			//per channel, cuts with corresponding channel called.
	2nd level:	Npeaks[0]	Np[1]	...	Np[PMT_channels.size()-1]		NpSum			//cuts with channel -1 called.
MPPC_Npe_profile	X		Y			Npe			S_total		Npeaks		channel
MPPC_Npe_profile_x	X		Y			Npe			S_total		Npeaks		channel
MPPC_Npe_profile_y	X		Y			Npe			S_total		Npeaks		channel
EventNumber		event#		------		------		------		------		------
*/
void PostProcessor::LoopThroughData(std::vector<Operation> &operations, int channel, Type type)
{
	if (operations.empty())
		return;
	HistogramSetups* setups = get_hist_setups(current_exp_index, channel, type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *hist_cuts = (NULL==setups ? &empty : &(setups->hist_cuts));
	std::deque<EventCut> *run_cuts = (NULL== get_run_cuts(current_exp_index) ? &empty : get_run_cuts(current_exp_index));
	std::string exp_str = experiments[current_exp_index];
	if (type == Correlation_x)
		type = _x_corr;
	if (type == Correlation_y)
		type = _y_corr;
	int ch_ind = channel_to_index(channel, type);
	switch (type)
	{
	case Type::MPPC_t_S:
	case Type::MPPC_A_S:
	case Type::MPPC_tbS:
	case Type::MPPC_tbN:
	case Type::MPPC_Ss:
	case Type::MPPC_As:
	case Type::PMT_t_S:
	case Type::PMT_A_S:
	case Type::PMT_Ss:
	case Type::PMT_As:
	case Type::PMT_tbS:
	case Type::PMT_tbN:
	case Type::PMT_tbNpe:
	{
		bool use_Npe = !(type == Type::MPPC_tbN || type == Type::PMT_tbN);
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		if (isPMTtype(type)) {
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[ch_ind].size();
		double s1pe = calibr_info.get_S1pe(channel, current_exp_index);
		std::vector<double> cut_data(6);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			for (int pk = 0, pk_end = (*peaks)[ch_ind][run].size(); pk != pk_end; ++pk) {
				bool failed_hist_cut = false; //normal cuts
				bool failed_phys_cut = false; //drawn (displayed) cuts only
				cut_data[0] = (*peaks)[ch_ind][run][pk].S;
				cut_data[1] = (*peaks)[ch_ind][run][pk].A;
				cut_data[2] = (*peaks)[ch_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
				cut_data[3] = (*peaks)[ch_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
				cut_data[4] =
#ifdef PEAK_AVR_TIME
						(*peaks)[ch_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
				cut_data[5] =  use_Npe ? (s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1) : 1;
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(cut_data, run)) //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
					if (!cut->GetAffectingHistogram() && !failed_phys_cut)
						if (kFALSE == (*cut)(cut_data, run))
							failed_phys_cut = true;
					if (failed_hist_cut && failed_phys_cut)
						break;
				}
				for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
					if (operations[o].apply_run_cuts && failed_run_cut)
						continue;
					if (operations[o].apply_hist_cuts && failed_hist_cut)
						continue;
					if (operations[o].apply_phys_cuts && failed_phys_cut)
						continue;
					(*operations[o].operation)(cut_data, run);
				}
			}
		}
		break;
	}
	case Type::PMT_trigger_bNpe:
	case Type::PMT_trigger_bNpeaks:
	case Type::PMT_trigger_bS: {
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		TriggerData* trigger_data = TriggerData::GetData(this, channel, type);
		if (NULL == trigger_data) {
			std::cout<<"PostProcessor::LoopThroughData: No trigger data for exp "<<current_exp_index<<" channel "<<channel<<" type "<<type_name(type)<<std::endl;
			std::cout<<"\tDefualt TriggerData constructor is used"<<std::endl;
			trigger_data = new TriggerData(this); //TODO: No cleanup
		}
		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (isPMTtype(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[0].size();
		std::vector<double> cut_data(6);
		for (auto run = 0; run != run_size; ++run) {
			//cuts on peaks (low level cuts) are always applied (those affecting histogram only)
			std::deque<peak_processed> accepted_peaks;
			std::vector<double> trigger_offset(1, 0);
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			if (failed_run_cut && ignore_no_run_cut)
				continue;

			for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
				if (NULL != setups) {
					bool *active = setups->active_channels.info((*channels)[chan_ind]);
					if (NULL == active)
						continue;
					if (!(*active))
						continue;
				}
				double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], current_exp_index);
				for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					cut_data[0] = (*peaks)[chan_ind][run][pk].S;
					cut_data[1] = (*peaks)[chan_ind][run][pk].A;
					cut_data[2] = (*peaks)[chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
					cut_data[3] = (*peaks)[chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							(*peaks)[chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==(*channels)[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
								break;
							}
					}
					if (!failed_hist_cut)
						accepted_peaks.push_back(peak_processed(cut_data[0], cut_data[1], cut_data[2], cut_data[3], cut_data[4], cut_data[5]));
				}
			}

			if (type==PMT_trigger_bS) {
				trigger_offset[0] = SignalOperations::find_trigger_S_v2(accepted_peaks, trigger_data->trigger_window);
			} else {
				switch (trigger_version) {
				case trigger_v1: {
					trigger_offset[0] = SignalOperations::find_trigger_v1(accepted_peaks, trigger_data->trigger_window,
						(type == PMT_trigger_bNpe) ? true : false);
					break;
				}
				case trigger_v2: {
					trigger_offset[0] = SignalOperations::find_trigger_v2(accepted_peaks, trigger_data->trigger_window,
						(type == PMT_trigger_bNpe) ? true : false);
					break;
				}
				case trigger_v3: {
					trigger_offset[0] = SignalOperations::find_trigger_v3(accepted_peaks, trigger_data->trigger_window,
						(type == PMT_trigger_bNpe) ? true : false);
					break;
				}
				default:
					trigger_offset[0] = 0;
				}
			}

			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(trigger_offset, run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(trigger_offset, run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(trigger_offset, run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist)
						(*operations[o].operation)(trigger_offset, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(trigger_offset, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(trigger_offset, run);
					continue;
				}
				(*operations[o].operation)(trigger_offset, run);
			}
		}
		break;
	}
	case Type::PMT_trigger_fit:
	case Type::PMT_trigger_fit_chi2:
	case Type::MPPC_trigger_fit:
	case Type::MPPC_trigger_fit_chi2:
	{
		bool is_chi2 = (type == Type::PMT_trigger_fit_chi2 || type == Type::MPPC_trigger_fit_chi2);
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		TriggerFitData* trigger_data = TriggerFitData::GetData(this, channel, type);
		if (NULL == trigger_data) {
			std::cout<<"PostProcessor::LoopThroughData: No trigger data for exp "<<current_exp_index<<" channel "<<channel<<" type "<<type_name(type)<<std::endl;
			std::cout<<"\tDefualt TriggerFitData constructor is used"<<std::endl;
			trigger_data = new TriggerFitData(this); //TODO: No cleanup
		} else if (!trigger_data->IsValid())
			std::cout<<"PostProcessor::LoopThroughData: Trigger data for exp "<<current_exp_index<<" channel "<<channel<<" type "<<type_name(type) <<" is invalid"<<std::endl;

		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (isPMTtype(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[0].size();
		struct event_result {
			bool failed_run_cut = false;
			bool failed_hist_phys = false;
			bool failed_hist = false;
			bool failed_phys = false;
			std::vector<double> trigger_offset = std::vector<double>(1, 0);
			std::vector<double> chi2_value = std::vector<double>(1, 0);
			int event_number = -1;
		};
		std::vector<event_result> results(run_size);
		std::vector<std::pair<int, int>> thread_indices = split_range(0, run_size, threads_number);
		int thread_n = thread_indices.size();
		std::vector<std::thread> threads;
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads.push_back(std::thread([&](int n_th) {
			for (int run = thread_indices[n_th].first; run != thread_indices[n_th].second; ++run) {
				std::vector<double> cut_data(6);
				results[run].event_number = run;
				//cuts on peaks (low level cuts) are always applied (those affecting histogram only)
				std::deque<peak_processed> accepted_peaks;
				for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
					if (kFALSE == cut->GetAccept(run)) {
						results[run].failed_run_cut = true;
						break;
					}
				if (results[run].failed_run_cut && ignore_no_run_cut)
					continue;

				for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
					if (NULL != setups) {
						bool *active = setups->active_channels.info((*channels)[chan_ind]);
						if (NULL == active)
							continue;
						if (!(*active))
							continue;
					}
					double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], current_exp_index);
					for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
						bool failed_hist_cut = false; //normal cuts
						cut_data[0] = (*peaks)[chan_ind][run][pk].S;
						cut_data[1] = (*peaks)[chan_ind][run][pk].A;
						cut_data[2] = (*peaks)[chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
						cut_data[3] = (*peaks)[chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
						cut_data[4] =
	#ifdef PEAK_AVR_TIME
								(*peaks)[chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
	#else
								0.5*(cut_data[3]+cut_data[2]);
	#endif
						cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
						for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
							if (cut->GetChannel()==(*channels)[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
								if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
									failed_hist_cut = true;
									break;
								}
						}
						if (!failed_hist_cut)
							accepted_peaks.push_back(peak_processed(cut_data[0], cut_data[1], cut_data[2], cut_data[3], cut_data[4], cut_data[5]));
					}
				}

				//TODO: cache is necessary
				if (trigger_data->IsValid()) {
					results[run].trigger_offset[0] = SignalOperations::find_trigger_by_fit(accepted_peaks, trigger_data->exp_pulse_shape,
							(int) trigger_data->trigger_type, trigger_data->scan_dt, trigger_data->t0_precision);
					results[run].chi2_value[0] = SignalOperations::get_likelihood(accepted_peaks, trigger_data->exp_pulse_shape, (int)trigger_data->trigger_type, results[run].trigger_offset[0]);
				} else {
					results[run].trigger_offset[0] = 0;
					results[run].chi2_value[0] = DBL_MAX;
				}

				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if (-1 == cut->GetChannel()) {
						if (!results[run].failed_hist_phys)
							if (kFALSE == (*cut)(is_chi2 ? results[run].chi2_value : results[run].trigger_offset, run))
								results[run].failed_hist_phys = true;
						if (!results[run].failed_hist && cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(is_chi2 ? results[run].chi2_value : results[run].trigger_offset, run))
								results[run].failed_hist = true;
						if (!results[run].failed_phys && !cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(is_chi2 ? results[run].chi2_value : results[run].trigger_offset, run))
								results[run].failed_phys = true;
					}
			}
			}, n_th));
		}
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads[n_th].join();
		}
		for (auto run = 0; run != run_size; ++run) {
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && results[run].failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist)
						(*operations[o].operation)(is_chi2 ? results[run].chi2_value : results[run].trigger_offset, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_phys) (*operations[o].operation)(is_chi2 ? results[run].chi2_value : results[run].trigger_offset, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist_phys) (*operations[o].operation)(is_chi2 ? results[run].chi2_value : results[run].trigger_offset, run);
					continue;
				}
				(*operations[o].operation)(is_chi2 ? results[run].chi2_value : results[run].trigger_offset, run);
			}
		}
		break;
	}
	case Type::PMT_shape_fit:
	case Type::MPPC_shape_fit:
	{
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		ShapeFitData* fitter = ShapeFitData::GetData(this, channel, type);
		if (NULL == fitter) {
			std::cout<<"PostProcessor::LoopThroughData: No ShapeFitData for exp "<<current_exp_index<<" channel "<<channel<<" type "<<type_name(type)<<std::endl;
			return;
		} else if (!fitter->IsValid()) {
			std::cout<<"PostProcessor::LoopThroughData: shape fit data for exp "<<current_exp_index<<" channel "<<channel<<" type "<<type_name(type) <<" is invalid"<<std::endl;
			return;
		}

		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (isPMTtype(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[0].size();
		struct event_result {
			bool failed_run_cut = false;
			bool failed_hist_phys = false;
			bool failed_hist = false;
			bool failed_phys = false;
			std::vector<double> plotted_value = std::vector<double>(1, 0);
		};
		int plot_par_index = fitter->parameter_to_plot;
		std::vector<event_result> results(run_size);
		std::vector<std::pair<int, int>> thread_indices = split_range(0, run_size, threads_number);
		int thread_n = thread_indices.size();
		std::vector<std::thread> threads;
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads.push_back(std::thread([&](int n_th) {
			for (int run = thread_indices[n_th].first; run != thread_indices[n_th].second; ++run) {
				std::vector<double> cut_data(6);
				//cuts on peaks (low level cuts) are always applied (those affecting histogram only)
				std::deque<peak_processed> accepted_peaks;
				for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
					if (kFALSE == cut->GetAccept(run)) {
						results[run].failed_run_cut = true;
						break;
					}
				if (results[run].failed_run_cut && ignore_no_run_cut)
					continue;

				for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
					if (NULL != setups) {
						bool *active = setups->active_channels.info((*channels)[chan_ind]);
						if (NULL == active)
							continue;
						if (!(*active))
							continue;
					}
					double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], current_exp_index);
					for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
						bool failed_hist_cut = false; //normal cuts
						cut_data[0] = (*peaks)[chan_ind][run][pk].S;
						cut_data[1] = (*peaks)[chan_ind][run][pk].A;
						cut_data[2] = (*peaks)[chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
						cut_data[3] = (*peaks)[chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
						cut_data[4] =
	#ifdef PEAK_AVR_TIME
								(*peaks)[chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
	#else
								0.5*(cut_data[3]+cut_data[2]);
	#endif
						cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
						for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
							if (cut->GetChannel()==(*channels)[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
								if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
									failed_hist_cut = true;
									break;
								}
						}
						if (!failed_hist_cut)
							accepted_peaks.push_back(peak_processed(cut_data[0], cut_data[1], cut_data[2], cut_data[3], cut_data[4], cut_data[5]));
					}
				}
				std::vector<double> pars = fitter->Fit(accepted_peaks);
				results[run].plotted_value[0] = pars[plot_par_index];

				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if (-1 == cut->GetChannel()) {
						if (!results[run].failed_hist_phys)
							if (kFALSE == (*cut)(results[run].plotted_value, run))
								results[run].failed_hist_phys = true;
						if (!results[run].failed_hist && cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(results[run].plotted_value, run))
								results[run].failed_hist = true;
						if (!results[run].failed_phys && !cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(results[run].plotted_value, run))
								results[run].failed_phys = true;
					}
			}
			}, n_th));
		}
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads[n_th].join();
		}
		for (auto run = 0; run != run_size; ++run) {
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && results[run].failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist)
						(*operations[o].operation)(results[run].plotted_value, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_phys) (*operations[o].operation)(results[run].plotted_value, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist_phys) (*operations[o].operation)(results[run].plotted_value, run);
					continue;
				}
				(*operations[o].operation)(results[run].plotted_value, run);
			}
		}
		break;
	}
	case Type::MPPC_trigger_avg:
	{
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		TriggerAvgTData* trigger_data = TriggerAvgTData::GetData(this, channel, type);
		if (NULL == trigger_data) {
			std::cout<<"PostProcessor::LoopThroughData: No trigger data for exp "<<current_exp_index<<" channel "<<channel<<" type "<<type_name(type)<<std::endl;
			std::cout<<"\tDefualt TriggerAvgTData constructor is used"<<std::endl;
			trigger_data = new TriggerAvgTData(this); //TODO: No cleanup
		} else if (!trigger_data->IsValid())
			std::cout<<"PostProcessor::LoopThroughData: Trigger data for exp "<<current_exp_index<<" channel "<<channel<<" type "<<type_name(type) <<" is invalid"<<std::endl;

		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (isPMTtype(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[0].size();
		std::vector<double> cut_data(6);
		for (auto run = 0; run != run_size; ++run) {
			//cuts on peaks (low level cuts) are always applied (those affecting histogram only)
			std::deque<peak_processed> accepted_peaks;
			std::vector<double> trigger_offset(1, 0);
			double weight = 0, average_t = 0;
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			if (failed_run_cut && ignore_no_run_cut)
				continue;

			for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
				if (NULL != setups) {
					bool *active = setups->active_channels.info((*channels)[chan_ind]);
					if (NULL == active)
						continue;
					if (!(*active))
						continue;
				}
				double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], current_exp_index);
				for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					cut_data[0] = (*peaks)[chan_ind][run][pk].S;
					cut_data[1] = (*peaks)[chan_ind][run][pk].A;
					cut_data[2] = (*peaks)[chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
					cut_data[3] = (*peaks)[chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							(*peaks)[chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==(*channels)[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
								break;
							}
					}
					if (!failed_hist_cut) {
						double w = 0;
						switch (trigger_data->trigger_type) {
						case TriggerAvgTData::tbNpe: {
							w = cut_data[5] > 0 ? cut_data[5] : 0;
							break;
						}
						case TriggerAvgTData::tbS: {
							w = cut_data[0];
							break;
						}
						case TriggerAvgTData::tbNpeaks: {
							w = 1;
							break;
						}
						}
						weight += w;
						average_t += w * cut_data[4];
					}
				}
			}

			if (weight > 0){
				trigger_offset[0] = average_t / weight;
			} else {
				trigger_offset[0] = 0;
			}

			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(trigger_offset, run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(trigger_offset, run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(trigger_offset, run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist)
						(*operations[o].operation)(trigger_offset, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(trigger_offset, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(trigger_offset, run);
					continue;
				}
				(*operations[o].operation)(trigger_offset, run);
			}
		}
		break;
	}
	case Type::PMT_T_sum:
	{
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (isPMTtype(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[0].size();
		struct event_result {
			bool failed_run_cut = false;
			bool failed_hist_phys = false;
			bool failed_hist = false;
			bool failed_phys = false;
			std::vector<double> time_length = std::vector<double>(1, 0);
			int event_number = -1;
		};
		std::deque<event_result> results(run_size);
		std::vector<std::pair<int, int>> thread_indices = split_range(0, run_size, threads_number);
		int thread_n = thread_indices.size();
		std::vector<std::thread> threads;
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads.push_back(std::thread([&](int n_th) {
			for (int run = thread_indices[n_th].first; run != thread_indices[n_th].second; ++run) {
				std::vector<double> cut_data(6);
				//cuts on peaks (low level cuts) are always applied (those affecting histogram only)
				int n_active_channels = 0;
				for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
					if (kFALSE == cut->GetAccept(run)) {
						results[run].failed_run_cut = true;
						break;
					}
				if (results[run].failed_run_cut && ignore_no_run_cut)
					continue;

				for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
					if (NULL != setups) {
						bool *active = setups->active_channels.info((*channels)[chan_ind]);
						if (NULL == active)
							continue;
						if (!(*active))
							continue;
					}
					n_active_channels += 1;
					double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], current_exp_index);
					for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
						bool failed_hist_cut = false; //normal cuts
						cut_data[0] = (*peaks)[chan_ind][run][pk].S;
						cut_data[1] = (*peaks)[chan_ind][run][pk].A;
						cut_data[2] = (*peaks)[chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
						cut_data[3] = (*peaks)[chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
						cut_data[4] =
	#ifdef PEAK_AVR_TIME
								(*peaks)[chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
	#else
								0.5*(cut_data[3]+cut_data[2]);
	#endif
						cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
						for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
							if (cut->GetChannel()==(*channels)[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
								if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
									failed_hist_cut = true;
									break;
								}
						}
						if (!failed_hist_cut)
							results[run].time_length[0] += cut_data[3] - cut_data[2];
					}
				}

				if (0!=n_active_channels) {
					results[run].time_length[0] /= n_active_channels;
				}

				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if (-1 == cut->GetChannel()) {
						if (!results[run].failed_hist_phys)
							if (kFALSE == (*cut)(results[run].time_length, run))
								results[run].failed_hist_phys = true;
						if (!results[run].failed_hist && cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(results[run].time_length, run))
								results[run].failed_hist = true;
						if (!results[run].failed_phys && !cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(results[run].time_length, run))
								results[run].failed_phys = true;
					}
			}
			}, n_th));
		}
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads[n_th].join();
		}
		for (auto run = 0; run != run_size; ++run) {
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && results[run].failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist)
						(*operations[o].operation)(results[run].time_length, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_phys) (*operations[o].operation)(results[run].time_length, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist_phys) (*operations[o].operation)(results[run].time_length, run);
					continue;
				}
				(*operations[o].operation)(results[run].time_length, run);
			}
		}
		break;
	}
	case Type::MPPC_tbS_sum:
	case Type::MPPC_tbNpe_sum:
	case Type::MPPC_tbN_sum:
	{
		bool use_Npe = !(type == Type::MPPC_tbN_sum);
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		std::vector<double> cut_data(6);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind!=_ch_ind_end_;++chan_ind) {
				if (NULL != setups) {
					bool *active = setups->active_channels.info(MPPC_channels[chan_ind]);
					if (NULL == active)
						continue;
					if (!(*active))
						continue;
				}
				double s1pe = calibr_info.get_S1pe(MPPC_channels[chan_ind], current_exp_index);
				for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					bool failed_phys_cut = false; //drawn (displayed) cuts only
					cut_data[0] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].S;
					cut_data[1] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].A;
					cut_data[2] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
					cut_data[3] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					cut_data[5] =  use_Npe ? (s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1) : 1;
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==MPPC_channels[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
						if (cut->GetChannel()==MPPC_channels[chan_ind] && !cut->GetAffectingHistogram() && !failed_phys_cut)
							if (kFALSE == (*cut)(cut_data, run))
								failed_phys_cut = true;
						if (failed_hist_cut && failed_phys_cut)
							break;
					}
					for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
						if (operations[o].apply_run_cuts && failed_run_cut)
							continue;
						if (operations[o].apply_hist_cuts && failed_hist_cut)
							continue;
						if (operations[o].apply_phys_cuts && failed_phys_cut)
							continue;
						(*operations[o].operation)(cut_data, run);
					}
				}
			}
		}
		break;
	}
	case Type::MPPC_S2: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = data->mppc_peaks[current_exp_index][ch_ind].size();
		double s1pe = calibr_info.get_S1pe(channel, current_exp_index);
		std::vector<double> cut_data(6);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			double S2 = 0;
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][ch_ind][run].size();
					pk != pk_end; ++pk) {
				bool failed_hist_cut = false; //normal cuts
				cut_data[0] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
				cut_data[3] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
				cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][ch_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
				cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (cut->GetChannel()==MPPC_channels[ch_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
							break;
						}
				}
				if (!failed_hist_cut)
					S2 += cut_data[0];
			}
			//1st parameter cut must be applied only if the rest of parameters are -2 === cut channel ==-1
			cut_data[0] = S2;
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			cut_data[5] = -2;
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				(*operations[o].operation)(cut_data, run);
			}
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = data->pmt_peaks[current_exp_index][ch_ind].size();
		double s1pe = calibr_info.get_S1pe(channel, current_exp_index);
		std::vector<double> cut_data(6);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			double S2 = 0;
			for (int pk = 0, pk_end = data->pmt_peaks[current_exp_index][ch_ind][run].size(); pk != pk_end; ++pk) {
				bool failed_hist_cut = false; //normal cuts
				cut_data[0] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].S;
				cut_data[1] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].A;
				cut_data[2] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
				cut_data[3] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
				cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->pmt_peaks[current_exp_index][ch_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
				cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (cut->GetChannel()==PMT_channels[ch_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
							break;
						}
				}
				if (!failed_hist_cut)
					S2 += cut_data[0];
			}
			cut_data[0] = S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			cut_data[5] = -2;
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				(*operations[o].operation)(cut_data, run);
			}
		}
		break;
	}
	case Type::MPPC_coord_x:
	case Type::MPPC_coord_y:
	case Type::MPPC_coord:
	case Type::MPPC_coord_disp:
	{
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		//[0] - data for no cuts, [1] - data for histogram cuts, [2] - for physical cuts and [3] - for both histogram and physical
		for (auto run = 0; run != run_size; ++run) {
			std::vector<double> Npes (MPPC_channels.size()+3, 0);
			double Npe_sum = 0, Npe_max = 0;
			std::vector<double> cut_data(6);
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}

			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind<_ch_ind_end_; ++chan_ind) {
				if (NULL != setups) {
					bool *active = setups->active_channels.info(MPPC_channels[chan_ind]);
					if (NULL == active)
						continue;
					if (!(*active))
						continue;
				}
				double s1pe = calibr_info.get_S1pe(MPPC_channels[chan_ind], current_exp_index);
				double S2 = 0;
				for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					cut_data[0] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].S;
					cut_data[1] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].A;
					cut_data[2] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
					cut_data[3] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==MPPC_channels[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
								break;
							}
					}
					if (!failed_hist_cut)
						S2 += cut_data[0];
				}
				S2 = (s1pe > 0 ? S2/s1pe : 0);
				Npes[chan_ind] = std::round(S2);
				if (MPPC_coords.find(MPPC_channels[chan_ind])!=MPPC_coords.end()) {
					Npe_sum += Npes[chan_ind];
#ifdef WEIGHTED_COORD
					Npes[MPPC_channels.size()] += Npes[chan_ind]*MPPC_coords.find(MPPC_channels[chan_ind])->second.first;//x
					Npes[MPPC_channels.size()+1] += Npes[chan_ind]*MPPC_coords.find(MPPC_channels[chan_ind])->second.second;//y
#else
					if (Npe_max<Npes[chan_ind]){
						Npe_max = Npes[chan_ind];
						Npes[MPPC_channels.size()] = MPPC_coords.find(MPPC_channels[chan_ind])->second.first;//x
						Npes[MPPC_channels.size()+1] = MPPC_coords.find(MPPC_channels[chan_ind])->second.second;//y
					}
#endif //WEIGHTED_COORD
				}
			}
#ifdef WEIGHTED_COORD
			Npes[MPPC_channels.size()] /= (Npe_sum == 0 ? 1: Npe_sum);
			Npes[MPPC_channels.size()+1] /= (Npe_sum == 0 ? 1: Npe_sum);
			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind<_ch_ind_end_; ++chan_ind) {
				if (MPPC_coords.find(MPPC_channels[chan_ind])!=MPPC_coords.end()) {
					double d_x = Npes[MPPC_channels.size()] - MPPC_coords.find(MPPC_channels[chan_ind])->second.first;
					double d_y = Npes[MPPC_channels.size()+1] - MPPC_coords.find(MPPC_channels[chan_ind])->second.second;
					Npes[MPPC_channels.size()+2] += Npes[chan_ind]*(d_x * d_x + d_y * d_y);
				}
			}
			Npes[MPPC_channels.size()+2] /= (Npe_sum == 0 ? 1: Npe_sum);
#endif
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(Npes, run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Npes, run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Npes, run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist) (*operations[o].operation)(Npes, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(Npes, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(Npes, run);
					continue;
				}
				(*operations[o].operation)(Npes, run);
			}
		}
		break;
	}
	case Type::MPPC_Npe_profile:
	case Type::MPPC_Npe_profile_x:
	case Type::MPPC_Npe_profile_y:
	{
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o != o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		//[0] - data for no cuts, [1] - data for histogram cuts, [2] - for physical cuts and [3] - for both histogram and physical
		for (auto run = 0; run != run_size; ++run) {
			std::vector<double> cut_data(6, 0);
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}

			if (failed_run_cut && ignore_no_run_cut)
				continue;

			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind<_ch_ind_end_; ++chan_ind) {
				if (NULL != setups) {
					bool *active = setups->active_channels.info(MPPC_channels[chan_ind]);
					if (NULL == active)
						continue;
					if (!(*active))
						continue;
				}
				if (MPPC_coords.find(MPPC_channels[chan_ind]) == MPPC_coords.end())
					continue;
				std::vector<double> MPPC_result (6, 0); //X, Y, Npe, S_total, Npeaks, channel
				MPPC_result[0] = MPPC_coords.find(MPPC_channels[chan_ind])->second.first;
				MPPC_result[1] = MPPC_coords.find(MPPC_channels[chan_ind])->second.second;
				MPPC_result[5] = MPPC_channels[chan_ind];
				double s1pe = calibr_info.get_S1pe(MPPC_channels[chan_ind], current_exp_index);
				for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					cut_data[0] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].S;
					cut_data[1] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].A;
					cut_data[2] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
					cut_data[3] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==MPPC_channels[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
								break;
							}
					}
					if (!failed_hist_cut) {
						MPPC_result[3] += cut_data[0];
						MPPC_result[4] += 1;
					}
				}
				MPPC_result[2] = (s1pe > 0 ? std::round(MPPC_result[3]/s1pe) : MPPC_result[4]);
				bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if (-1 == cut->GetChannel()) {
						if (!failed_hist_phys)
							if (kFALSE == (*cut)(MPPC_result, run))
								failed_hist_phys = true;
						if (!failed_hist && cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(MPPC_result, run))
								failed_hist = true;
						if (!failed_phys && !cut->GetAffectingHistogram())
							if (kFALSE == (*cut)(MPPC_result, run))
								failed_phys = true;
					}
				for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
					if (operations[o].apply_run_cuts && failed_run_cut)
						continue;
					if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
						if (!failed_hist) (*operations[o].operation)(MPPC_result, run);
						continue;
					}
					if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
						if (!failed_phys) (*operations[o].operation)(MPPC_result, run);
						continue;
					}
					if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
						if (!failed_hist_phys) (*operations[o].operation)(MPPC_result, run);
						continue;
					}
					(*operations[o].operation)(MPPC_result, run);
				}
			} //channel loop
		}//run loop
		break;
	}
	case Type::MPPC_Npe_sum:
	case Type::MPPC_N_sum:
	case Type::MPPC_S_sum:
	case Type::PMT_Npe_sum:
	case Type::PMT_S_sum:
	case Type::PMT_sum_N:
	{
		bool use_Npe = (type == Type::MPPC_Npe_sum || type == Type::PMT_Npe_sum);
		bool use_Npeaks = (type == Type::MPPC_N_sum || type == Type::PMT_sum_N);
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o != o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (isPMTtype(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		struct event_result {
			bool failed_run_cut = false;
			bool failed_hist_phys = false;
			bool failed_hist = false;
			bool failed_phys = false;
			std::vector<double> Npes;
			int event_number = -1;
		};
		int run_size = (*peaks)[0].size();
		event_result def_res;
		def_res.Npes = std::vector<double>(channels->size()+1, 0);
		std::deque<event_result> results(run_size, def_res);
		std::vector<std::pair<int, int>> thread_indices = split_range(0, run_size, threads_number);
		int thread_n = thread_indices.size();
		std::vector<std::thread> threads;
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads.push_back(std::thread([&](int n_th) {
				for (int run = thread_indices[n_th].first; run != thread_indices[n_th].second; ++run) {
					std::vector<double> cut_data(6);
					results[run].event_number = run;
					for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
						if (kFALSE == cut->GetAccept(run)) {
							results[run].failed_run_cut = true;
							break;
						}

					if (results[run].failed_run_cut && ignore_no_run_cut)
						continue;

					for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
						if (NULL != setups) {
							bool *active = setups->active_channels.info((*channels)[chan_ind]);
							if (NULL == active)
								continue;
							if (!(*active))
								continue;
						}
						double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], current_exp_index);
						double S2 = 0;
						for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
							bool failed_hist_cut = false; //normal cuts
							cut_data[0] = (*peaks)[chan_ind][run][pk].S;
							cut_data[1] = (*peaks)[chan_ind][run][pk].A;
							cut_data[2] = (*peaks)[chan_ind][run][pk].left + data->trigger_offset[current_exp_index][run];
							cut_data[3] = (*peaks)[chan_ind][run][pk].right + data->trigger_offset[current_exp_index][run];
							cut_data[4] =
		#ifdef PEAK_AVR_TIME
									(*peaks)[chan_ind][run][pk].t + data->trigger_offset[current_exp_index][run];
		#else
									0.5*(cut_data[3]+cut_data[2]);
		#endif
							cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
							for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
								if (cut->GetChannel()==(*channels)[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
									if (kFALSE == (*cut)(cut_data, run)) { //more expensive than GetAffectingHistogram
										failed_hist_cut = true;
										break;
									}
							}
							if (!failed_hist_cut)
								S2 += use_Npeaks ? 1.0 : cut_data[0];

						}
						S2 = (use_Npe ? (s1pe > 0 ? S2/s1pe : 0) : S2);
						results[run].Npes[chan_ind] = ((use_Npe || use_Npeaks ) ? std::round(S2) : S2);
						results[run].Npes[channels->size()] += results[run].Npes[chan_ind];
					}
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if (-1 == cut->GetChannel()) {
							if (!results[run].failed_hist_phys)
								if (kFALSE == (*cut)(results[run].Npes, run))
									results[run].failed_hist_phys = true;
							if (!results[run].failed_hist && cut->GetAffectingHistogram())
								if (kFALSE == (*cut)(results[run].Npes, run))
									results[run].failed_hist = true;
							if (!results[run].failed_phys && !cut->GetAffectingHistogram())
								if (kFALSE == (*cut)(results[run].Npes, run))
									results[run].failed_phys = true;
						}
				} //event loop
			}, n_th));
		}
		for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
			threads[n_th].join();
		}
		for (auto run = 0; run != run_size; ++run) {
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && results[run].failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist) (*operations[o].operation)(results[run].Npes, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_phys) (*operations[o].operation)(results[run].Npes, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist_phys) (*operations[o].operation)(results[run].Npes, run);
					continue;
				}
				(*operations[o].operation)(results[run].Npes, run);
			}
		}
		break;
	}
	case Type::EventNumber:
	{
		bool ignore_no_run_cut = true;
		for (std::size_t o = 0, o_end_ = operations.size(); o != o_end_; ++o) {
			if (!operations[o].apply_run_cuts) {
				ignore_no_run_cut = false;
				break;
			}
		}
		struct event_result {
			bool failed_run_cut = false;
			bool failed_hist_phys = false;
			bool failed_hist = false;
			bool failed_phys = false;
			std::vector<double> number;
			int event_number = -1;
		};
		std::size_t run_size = events_number();
		event_result def_res;
		def_res.number = std::vector<double>(1, 0);
		std::deque<event_result> results(run_size, def_res);
		std::vector<std::pair<std::size_t, std::size_t>> thread_indices = split_range(std::size_t(0), run_size, threads_number);
		std::size_t thread_n = thread_indices.size();
		std::vector<std::thread> threads;
		for (std::size_t n_th = 0u; n_th < thread_n; ++n_th) {
			threads.push_back(std::thread([&](std::size_t n_th) {
				for (std::size_t run = thread_indices[n_th].first; run != thread_indices[n_th].second; ++run) {
					std::vector<double> cut_data(6);
					results[run].number[0] = run;
					results[run].event_number = run;
					for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
						if (kFALSE == cut->GetAccept(run)) {
							results[run].failed_run_cut = true;
							break;
						}

					if (results[run].failed_run_cut && ignore_no_run_cut)
						continue;

					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if (-1 == cut->GetChannel()) {
							if (!results[run].failed_hist_phys)
								if (kFALSE == (*cut)(results[run].number, run))
									results[run].failed_hist_phys = true;
							if (!results[run].failed_hist && cut->GetAffectingHistogram())
								if (kFALSE == (*cut)(results[run].number, run))
									results[run].failed_hist = true;
							if (!results[run].failed_phys && !cut->GetAffectingHistogram())
								if (kFALSE == (*cut)(results[run].number, run))
									results[run].failed_phys = true;
						}
				} //event loop
			}, n_th));
		}
		for (std::size_t n_th = 0u; n_th < thread_n; ++n_th) {
			threads[n_th].join();
		}
		for (std::size_t run = 0; run != run_size; ++run) {
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && results[run].failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist) (*operations[o].operation)(results[run].number, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_phys) (*operations[o].operation)(results[run].number, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!results[run].failed_hist_phys) (*operations[o].operation)(results[run].number, run);
					continue;
				}
				(*operations[o].operation)(results[run].number, run);
			}
		}
		break;
	}
	case Type::Correlation: //it has its own cuts.
	{
		int ch_x_ind = channel_to_index(_x_corr_ch, _x_corr);
		int ch_y_ind = channel_to_index(_y_corr_ch, _y_corr);
		std::size_t run_size = (isPMTtype(_x_corr) ? data->pmt_peaks[current_exp_index][ch_x_ind].size() : data->mppc_peaks[current_exp_index][ch_x_ind].size());
		if (run_size !=(isPMTtype(_y_corr) ? data->pmt_peaks[current_exp_index][ch_y_ind].size() : data->mppc_peaks[current_exp_index][ch_y_ind].size())) {
			std::cout<<"LoopThroughData:Correlation:Error: run size mismatch" << std::endl;
			break;
		}
		//{run_cuts on, run cuts off}->run
		std::deque<std::vector<boost::optional<double> > > vals_x(2, std::vector<boost::optional<double> >(run_size, boost::none)),
				vals_y(2, std::vector<boost::optional<double> >(run_size, boost::none));

		correlation_data data_x, data_y, data_x_nrc, data_y_nrc; //nrc = no run cuts
		data_x.vals = &(vals_x[0]);
		data_x.ch_size = isPMTtype(_x_corr) ? PMT_channels.size() : MPPC_channels.size();
		data_y.vals = &(vals_y[0]);
		data_y.ch_size = isPMTtype(_y_corr) ? PMT_channels.size() : MPPC_channels.size();
		data_x_nrc.vals = &(vals_x[1]);
		data_x_nrc.ch_size = isPMTtype(_x_corr) ? PMT_channels.size() : MPPC_channels.size();
		data_y_nrc.vals = &(vals_y[1]);
		data_y_nrc.ch_size = isPMTtype(_y_corr) ? PMT_channels.size() : MPPC_channels.size();
		FunctionWrapper* X_filler = new FunctionWrapper(&data_x);
		FunctionWrapper* Y_filler = new FunctionWrapper(&data_y);
		FunctionWrapper* X_filler_nrc = new FunctionWrapper(&data_x_nrc);
		FunctionWrapper* Y_filler_nrc = new FunctionWrapper(&data_y_nrc);
		if (!set_correlation_filler(X_filler, _x_corr) || !set_correlation_filler(Y_filler, _y_corr) ||
				!set_correlation_filler(X_filler_nrc, _x_corr) || !set_correlation_filler(Y_filler_nrc, _y_corr)) {
			std::cout << "LoopThroughData:Correlation:Error: failed to set fillers" << std::endl;
			break;
		}
		
		Operation opx, opy, opx_nrc, opy_nrc;
		opx.operation = X_filler;
		opx.apply_run_cuts = true;
		opx.apply_hist_cuts = true;
		opx.apply_phys_cuts = false;
		opy.operation = Y_filler;
		opy.apply_run_cuts = true;
		opy.apply_hist_cuts = true;
		opy.apply_phys_cuts = false;
		opx_nrc.operation = X_filler_nrc;
		opx_nrc.apply_run_cuts = false;
		opx_nrc.apply_hist_cuts = true;
		opx_nrc.apply_phys_cuts = false;
		opy_nrc.operation = Y_filler_nrc;
		opy_nrc.apply_run_cuts = false;
		opy_nrc.apply_hist_cuts = true;
		opy_nrc.apply_phys_cuts = false;
		std::vector<Operation> vec;
		vec.push_back(opx);
		vec.push_back(opx_nrc);
		LoopThroughData(vec, _x_corr_ch, Correlation_x);
		vec.clear();
		vec.push_back(opy);
		vec.push_back(opy_nrc);
		LoopThroughData(vec, _y_corr_ch, Correlation_y);

		std::vector<double> vals(2); //{x, y}
		std::vector<double> vals_nrc(2); //{x, y} for no run cuts
		for (std::size_t run = 0; run != run_size; ++run) {
			bool failed_run_cut = (boost::none == vals_x[0][run] || boost::none == vals_y[0][run]);
			if (boost::none == vals_x[1][run] || boost::none == vals_y[1][run]) {
				std::cerr<<"LoopThroughData:Warning! Missing data for run "<<run<<" in Type::Correlation"<<std::endl;
				continue;
			}
			vals_nrc[0] = *(vals_x[1][run]);
			vals_nrc[1] = *(vals_y[1][run]);
			if (!failed_run_cut) {
				vals[0] = *(vals_x[0][run]);
				vals[1] = *(vals_y[0][run]);
			}
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			bool failed_hist_cut_nrc = false; //normal cuts
			bool failed_phys_cut_nrc = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
				if (!failed_run_cut) {
					if (cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(vals, run)) //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
					if (!cut->GetAffectingHistogram() && !failed_phys_cut)
						if (kFALSE == (*cut)(vals, run))
							failed_phys_cut = true;
				}
				if (cut->GetAffectingHistogram() && !failed_hist_cut_nrc)
					if (kFALSE == (*cut)(vals_nrc, run)) //more expensive than GetAffectingHistogram
						failed_hist_cut_nrc = true;
				if (!cut->GetAffectingHistogram() && !failed_phys_cut_nrc)
					if (kFALSE == (*cut)(vals_nrc, run))
						failed_phys_cut_nrc = true;
				if (failed_hist_cut && failed_phys_cut && failed_hist_cut_nrc && failed_phys_cut_nrc)
					break;
			}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (!operations[o].apply_run_cuts) {
					if (operations[o].apply_hist_cuts && failed_hist_cut_nrc)
						continue;
					if (operations[o].apply_phys_cuts && failed_phys_cut_nrc)
						continue;
					(*operations[o].operation)(vals_nrc, run);
				} else {
					if (operations[o].apply_hist_cuts && failed_hist_cut)
						continue;
					if (operations[o].apply_phys_cuts && failed_phys_cut)
						continue;
					(*operations[o].operation)(vals, run);
				}
			}
		}
		delete X_filler;
		delete Y_filler;
		break;
	}
	case Type::CorrelationAll: //it has its own cuts.
	{
		int actual_current_exp = current_exp_index;
		for (int exp_ind=0,_end_exp = experiments.size(); exp_ind!=_end_exp; ++exp_ind) {
			int ch_x_ind = channel_to_index(_x_corr_ch, _x_corr);
			int ch_y_ind = channel_to_index(_y_corr_ch, _y_corr);
			std::size_t run_size = (isPMTtype(_x_corr) ? data->pmt_peaks[current_exp_index][ch_x_ind].size() : data->mppc_peaks[current_exp_index][ch_x_ind].size());
			if (run_size !=(isPMTtype(_y_corr) ? data->pmt_peaks[current_exp_index][ch_y_ind].size() : data->mppc_peaks[current_exp_index][ch_y_ind].size())) {
				std::cout<<"LoopThroughData:Correlation:Error: run size mismatch" << std::endl;
				break;
			}
			//{run_cuts on, run cuts off}->run
			std::deque<std::vector<boost::optional<double> > > vals_x(2, std::vector<boost::optional<double> >(run_size, boost::none)),
					vals_y(2, std::vector<boost::optional<double> >(run_size, boost::none));

			correlation_data data_x, data_y, data_x_nrc, data_y_nrc; //nrc = no run cuts
			data_x.vals = &(vals_x[0]);
			data_x.ch_size = isPMTtype(_x_corr) ? PMT_channels.size() : MPPC_channels.size();
			data_y.vals = &(vals_y[0]);
			data_y.ch_size = isPMTtype(_x_corr) ? PMT_channels.size() : MPPC_channels.size();
			data_x_nrc.vals = &(vals_x[1]);
			data_x_nrc.ch_size = isPMTtype(_x_corr) ? PMT_channels.size() : MPPC_channels.size();
			data_y_nrc.vals = &(vals_y[1]);
			data_y_nrc.ch_size = isPMTtype(_x_corr) ? PMT_channels.size() : MPPC_channels.size();
			FunctionWrapper* X_filler = new FunctionWrapper(&data_x);
			FunctionWrapper* Y_filler = new FunctionWrapper(&data_y);
			FunctionWrapper* X_filler_nrc = new FunctionWrapper(&data_x_nrc);
			FunctionWrapper* Y_filler_nrc = new FunctionWrapper(&data_y_nrc);
			if (!set_correlation_filler(X_filler, _x_corr) || !set_correlation_filler(Y_filler, _y_corr) ||
					!set_correlation_filler(X_filler_nrc, _x_corr) || !set_correlation_filler(Y_filler_nrc, _y_corr)) {
				std::cout << "LoopThroughData:Correlation:Error: failed to set fillers" << std::endl;
				break;
			}

			Operation opx, opy, opx_nrc, opy_nrc;
			opx.operation = X_filler;
			opx.apply_run_cuts = true;
			opx.apply_hist_cuts = true;
			opx.apply_phys_cuts = false;
			opy.operation = Y_filler;
			opy.apply_run_cuts = true;
			opy.apply_hist_cuts = true;
			opy.apply_phys_cuts = false;
			opx_nrc.operation = X_filler_nrc;
			opx_nrc.apply_run_cuts = false;
			opx_nrc.apply_hist_cuts = true;
			opx_nrc.apply_phys_cuts = false;
			opy_nrc.operation = Y_filler_nrc;
			opy_nrc.apply_run_cuts = false;
			opy_nrc.apply_hist_cuts = true;
			opy_nrc.apply_phys_cuts = false;
			std::vector<Operation> vec;
			vec.push_back(opx);
			vec.push_back(opx_nrc);
			LoopThroughData(vec, _x_corr_ch, Correlation_x);
			vec.clear();
			vec.push_back(opy);
			vec.push_back(opy_nrc);
			LoopThroughData(vec, _y_corr_ch, Correlation_y);

			std::vector<double> vals(2); //{x, y}
			std::vector<double> vals_nrc(2); //{x, y} for no run cuts
			for (std::size_t run = 0; run != run_size; ++run) {
				bool failed_run_cut = (boost::none == vals_x[0][run] || boost::none == vals_y[0][run]);
				if (boost::none == vals_x[1][run] || boost::none == vals_y[1][run]) {
					std::cerr<<"LoopThroughData:Warning! Missing data for run "<<run<<" in Type::Correlation"<<std::endl;
					continue;
				}
				vals_nrc[0] = *(vals_x[1][run]);
				vals_nrc[1] = *(vals_y[1][run]);
				if (!failed_run_cut) {
					vals[0] = *(vals_x[0][run]);
					vals[1] = *(vals_y[0][run]);
				}
				bool failed_hist_cut = false; //normal cuts
				bool failed_phys_cut = false; //drawn (displayed) cuts only
				bool failed_hist_cut_nrc = false; //normal cuts
				bool failed_phys_cut_nrc = false; //drawn (displayed) cuts only
				for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
					if (kFALSE == cut->GetAccept(run)) {
						failed_run_cut = true;
						break;
					}
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (!failed_run_cut) {
						if (cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(vals, run)) //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
						if (!cut->GetAffectingHistogram() && !failed_phys_cut)
							if (kFALSE == (*cut)(vals, run))
								failed_phys_cut = true;
					}
					if (cut->GetAffectingHistogram() && !failed_hist_cut_nrc)
						if (kFALSE == (*cut)(vals_nrc, run)) //more expensive than GetAffectingHistogram
							failed_hist_cut_nrc = true;
					if (!cut->GetAffectingHistogram() && !failed_phys_cut_nrc)
						if (kFALSE == (*cut)(vals_nrc, run))
							failed_phys_cut_nrc = true;
					if (failed_hist_cut && failed_phys_cut && failed_hist_cut_nrc && failed_phys_cut_nrc)
						break;
				}
				for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
					if (operations[o].apply_run_cuts && failed_run_cut)
						continue;
					if (!operations[o].apply_run_cuts) {
						if (operations[o].apply_hist_cuts && failed_hist_cut_nrc)
							continue;
						if (operations[o].apply_phys_cuts && failed_phys_cut_nrc)
							continue;
						(*operations[o].operation)(vals_nrc, run);
					} else {
						if (operations[o].apply_hist_cuts && failed_hist_cut)
							continue;
						if (operations[o].apply_phys_cuts && failed_phys_cut)
							continue;
						(*operations[o].operation)(vals, run);
					}
				}
			}
			delete X_filler;
			delete Y_filler;
		}
		current_exp_index = actual_current_exp;
		break;
	}
	}
}

void PostProcessor::view_event(int event_index, int N_bins, double x_min, double x_max)
{
	if (NULL == event_canvas) {
		event_canvas = new TCanvas((std::string("event_canvas_")).c_str(), (std::string("event_canvas_")).c_str());
		event_canvas->SetGrid();
		event_canvas->SetTicks();
		event_canvas->ToggleEventStatus();
		event_canvas->ToggleToolBar();
	}
	event_canvas->cd();
	event_canvas->SetTitle((hist_name() + "_ev#" + int_to_str(event_index)).c_str());
	event_canvas->Clear();

	Type type = current_type;
	HistogramSetups* setups = get_hist_setups(current_exp_index, current_channel, current_type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *hist_cuts = (NULL==setups ? &empty : &(setups->hist_cuts));
	std::string exp_str = experiments[current_exp_index];
	if (type == Correlation_x)
		type = _x_corr;
	if (type == Correlation_y)
		type = _y_corr;
	switch (type)
	{
	case Type::MPPC_shape_fit:
	case Type::PMT_shape_fit:
	{
		ShapeFitData* fitter = ShapeFitData::GetData(this, current_channel, type);
		if (NULL == fitter) {
			std::cout<<"PostProcessor::view_event: No ShapeFitData for exp "<<current_exp_index<<" channel "<<current_channel<<" type "<<type_name(type)<<std::endl;
			return;
		} else if (!fitter->IsValid()) {
			std::cout<<"PostProcessor::view_event: shape fit data for exp "<<current_exp_index<<" channel "<<current_channel<<" type "<<type_name(type) <<" is invalid"<<std::endl;
			return;
		}
		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (isPMTtype(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[0].size();
		if (event_index >= run_size) {
			std::cout<<"PostProcessor::view_event: index "<<event_index<<" is out of bounds [0, "<<run_size<<")"<<std::endl;
			return;
		}
		std::vector<double> fit_pars;
		std::vector<double> cut_data(6);
		//cuts on peaks (low level cuts) are always applied (those affecting histogram only)
		std::deque<peak_processed> accepted_peaks;
		for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
			if (NULL != setups) {
				bool *active = setups->active_channels.info((*channels)[chan_ind]);
				if (NULL == active)
					continue;
				if (!(*active))
					continue;
			}
			double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], current_exp_index);
			for (int pk = 0, pk_end = (*peaks)[chan_ind][event_index].size(); pk != pk_end; ++pk) {
				bool failed_hist_cut = false; //normal cuts
				cut_data[0] = (*peaks)[chan_ind][event_index][pk].S;
				cut_data[1] = (*peaks)[chan_ind][event_index][pk].A;
				cut_data[2] = (*peaks)[chan_ind][event_index][pk].left + data->trigger_offset[current_exp_index][event_index];
				cut_data[3] = (*peaks)[chan_ind][event_index][pk].right + data->trigger_offset[current_exp_index][event_index];
				cut_data[4] =
#ifdef PEAK_AVR_TIME
						(*peaks)[chan_ind][event_index][pk].t + data->trigger_offset[current_exp_index][event_index];
#else
						0.5*(cut_data[3]+cut_data[2]);
#endif
				cut_data[5] = s1pe > 0 ? std::round(cut_data[0]/s1pe) : -1;
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (cut->GetChannel()==(*channels)[chan_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(cut_data, event_index)) { //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
							break;
						}
				}
				if (!failed_hist_cut)
					accepted_peaks.push_back(peak_processed(cut_data[0], cut_data[1], cut_data[2], cut_data[3], cut_data[4], cut_data[5]));
			}
		}
		fit_pars = fitter->Fit(accepted_peaks);
		std::vector<double> xs, ys;
		fitter->PeaksToXY(accepted_peaks, xs, ys);

		double max_val = 0;
		TH1D* hist = (TH1D*)gDirectory->FindObject("event_view");
		if (NULL!=hist)
			hist->Delete();
		hist = NULL;
		if (0 >= N_bins) {
			for (int i = 0, i_end_ = xs.size(); i != i_end_; ++i)
				max_val = std::max(max_val, ys[i]);
		} else {
			hist = new TH1D("event_view", "event_view", N_bins, x_min, x_max);
			for (int i = 0, i_end_ = xs.size(); i != i_end_; ++i) {
				hist->Fill(xs[i], ys[i]);
			}
			max_val = hist->GetMaximum();
		}
		TH2F* frame = (TH2F*)gDirectory->FindObject("Frame");
		if (NULL!=frame)
			frame->Delete();
		frame = new TH2F("Frame", (hist_name() + "_ev#"+int_to_str(event_index)).c_str(), 500, x_min, x_max, 500, 0, 1.2 * max_val);
		frame->GetXaxis()->SetTitle("Time [#mus]");
		frame->GetYaxis()->SetTitle("PE peaks");
		frame->Draw();
		event_canvas->Update();

		if (NULL == hist) {
			TGraph* peaks_graph = new TGraph(xs.size(), &xs[0], &ys[0]);
			peaks_graph->SetMarkerStyle(kFullCircle);
			peaks_graph->SetMarkerSize(1);
			peaks_graph->SetLineWidth(1);
			peaks_graph->SetMarkerColor(kBlack);
			peaks_graph->Draw("psame");
		} else
			hist->Draw("samehist");
		double y_scale = 0.6 * max_val / fitter->fit_function(&fit_pars[1], &fit_pars[0]);//TODO: universal maximum finder
		if (!fitter->fit_function.Draw(event_canvas, fit_pars, &y_scale)) {
			std::cout<<"Could not draw fitted function"<<std::endl;
		}
		event_canvas->Update();
		TPaveStats *ps = (TPaveStats*)frame->FindObject("stats");
		if (NULL != ps && NULL != frame) {
			ps->SetName("mystats");
			TList *list = ps->GetListOfLines();
			list->Clear();
			list->Add(CreateStatBoxLine(std::string("Event #") + int_to_str(event_index)));
			list->Add(CreateStatBoxLine("Entries", (int)xs.size()));
			for (int i = 0, i_end_ = fit_pars.size(); i!=i_end_; ++i) {
				std::string name = fitter->GetParameterName(i);
				if ("" == name)
					name = "par[" + int_to_str(i) + "]";
				list->Add(CreateStatBoxLine(name, fit_pars[i]));
			}
			frame->SetStats(0);//Needed to avoid the automatic redrawing of stats
			event_canvas->Modified();
			event_canvas->Update();
		}
		break;
	}
	default: {
		std::cout<<"Type "<< type_name(type) <<" is not implemented for event viewing"<<std::endl;
		return;
	}
	}
}

bool PostProcessor::set_correlation_filler(FunctionWrapper* operation, Type type)
{
	if (NULL == operation)
		return false;
	switch (type) {
	case Type::MPPC_S2:
	case Type::PMT_S2_S:
	case Type::PMT_trigger_bNpe:
	case Type::PMT_trigger_bNpeaks:
	case Type::PMT_trigger_bS:
	case Type::PMT_trigger_fit:
	case Type::PMT_trigger_fit_chi2:
	case Type::MPPC_trigger_fit:
	case Type::MPPC_trigger_fit_chi2:
	case Type::MPPC_trigger_avg:
	case Type::PMT_T_sum:
	case Type::MPPC_shape_fit:
	case Type::PMT_shape_fit:
	case Type::EventNumber:
	{
		operation->SetFunction([](std::vector<double>& pars, int run, void* data) {
			(*((correlation_data*)data)->vals)[run] = pars[0];
			return true;
		});
		break;
	}
	case Type::MPPC_Npe_sum:
	case Type::MPPC_N_sum:
	case Type::MPPC_S_sum:
	case Type::MPPC_coord_x:
	case Type::PMT_sum_N:
	case Type::PMT_Npe_sum:
	case Type::PMT_S_sum:
	{
		operation->SetFunction([](std::vector<double>& pars, int run, void* data) {
			(*((correlation_data*)data)->vals)[run] = pars[((correlation_data*)data)->ch_size];
			return true;
		});
		break;
	}
	case Type::MPPC_coord_y:
	{
		operation->SetFunction([](std::vector<double>& pars, int run, void* data) {
			(*((correlation_data*)data)->vals)[run] = pars[((correlation_data*)data)->ch_size + 1];
			return true;
		});
		break;
	}
	case Type::MPPC_coord_disp:
	{
		operation->SetFunction([](std::vector<double>& pars, int run, void* data) {
			(*((correlation_data*)data)->vals)[run] = pars[((correlation_data*)data)->ch_size + 2];
			return true;
		});
		break;
	}
	default: {
		std::cerr << "PostProcessor::set_correlation_filler: Warning! Not implemented type. Either it is 2D/not per event or forgotten to be implemented" << std::endl;
		return false;
	}
	}
	return true;
}

//Calls maximum of 2 LoopThroughData
bool PostProcessor::update(void)
{
	HistogramSetups * setups = get_hist_setups();
	if (NULL == setups) {
		std::cerr << "PostProcessor::update:Error: NULL histogram setups" << std::endl;
		return false;
	}

	//Set default setups before calling first LoopThroughData. Sets extra_data in HistogramSetups
	if (setups->use_default_setups) {
		default_hist_setups(setups);
		setups->use_default_setups = true;
	}

	struct hist_fill_data_ {
		void* phist;
		int ch_size;
	} hist_fill_data; //initialized right before LoopThroughData which fills the histogram
	FunctionWrapper histogram_filler(&hist_fill_data);

	std::size_t num_of_fills = 0;
	std::size_t num_of_drawn_fills = 0;
	FunctionWrapper fills_counter(&num_of_fills);
	FunctionWrapper drawn_fills_counter(&num_of_drawn_fills);
	fills_counter.SetFunction(
		drawn_fills_counter.SetFunction([](std::vector<double>& pars, int run, void* data) {
			++(*(std::size_t*)data);
			return true;
		})
	);

	struct limits_data_ {
		std::pair<double,double> x_mm;
		std::pair<double,double> y_mm;
		int ch_size;
	} limits, drawn_limits;
	limits.y_mm = limits.x_mm = std::pair<double,double>(DBL_MAX, -DBL_MAX);
	drawn_limits.y_mm = drawn_limits.x_mm = limits.y_mm;
	drawn_limits.ch_size = limits.ch_size = (isPMTtype(current_type) ? PMT_channels.size() : MPPC_channels.size());
	FunctionWrapper limits_finder(&limits);
	FunctionWrapper drawn_limits_finder(&drawn_limits);

	struct mean_variance_data_ { //number is calculated in num of fills
		long double mean_x;
		long double mean_y;
		long double variance_x;
		long double variance_y;
		long double stat_weight; //used only when num_of_fills/num_of_drawn_fills do not apply
		int ch_size;
	} mvar_data, mvar_drawn_data;
	mvar_data.variance_x = mvar_data.mean_x = 0;
	mvar_data.variance_y = mvar_data.mean_y = 0;
	mvar_drawn_data.variance_x = mvar_drawn_data.mean_x = 0;
	mvar_drawn_data.variance_y = mvar_drawn_data.mean_y = 0;
	mvar_drawn_data.stat_weight = mvar_data.stat_weight = 0;
	mvar_drawn_data.ch_size = mvar_data.ch_size = (isPMTtype(current_type) ? PMT_channels.size() : MPPC_channels.size());
	FunctionWrapper mean_taker(&mvar_data);
	FunctionWrapper variance_taker(&mvar_data);
	FunctionWrapper drawn_mean_taker(&mvar_drawn_data);
	FunctionWrapper drawn_variance_taker(&mvar_drawn_data);

	Type type = current_type;
	if (type == Correlation_x)
		type = _x_corr;
	if (type == Correlation_y)
		type = _y_corr;
	switch (type)
	{
	case Type::MPPC_Ss:
	case Type::MPPC_S2:
	case Type::PMT_S2_S:
	case Type::PMT_Ss:
	case Type::PMT_trigger_bNpe:
	case Type::PMT_trigger_bNpeaks:
	case Type::PMT_trigger_bS:
	case Type::PMT_trigger_fit:
	case Type::PMT_trigger_fit_chi2:
	case Type::MPPC_trigger_fit:
	case Type::MPPC_trigger_fit_chi2:
	case Type::MPPC_trigger_avg:
	case Type::PMT_T_sum:
	case Type::MPPC_shape_fit:
	case Type::PMT_shape_fit:
	case Type::EventNumber:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			if (pars[0] == DBL_MAX || pars[0] == -DBL_MAX)
				return true;
			(((mean_variance_data_*)data)->mean_x) += pars[0];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			if (pars[0] == DBL_MAX || pars[0] == -DBL_MAX)
				return true;
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += (pars[0] - meanX) * (pars[0] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			if (pars[0] == DBL_MAX || pars[0] == -DBL_MAX)
				return true;
			std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
			p->first = std::min(p->first, pars[0]);
			p->second = std::max(p->second, pars[0]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[0]);
			return true;
		});
		break;
	}
	case Type::MPPC_As:
	case Type::PMT_As:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[1];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += (pars[1] - meanX) * (pars[1] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
			p->first = std::min(p->first, pars[1]);
			p->second = std::max(p->second, pars[1]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[1]);
			return true;
		});
		break;
	}
	case Type::PMT_t_S:
	case Type::MPPC_t_S:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[4];
			(((mean_variance_data_*)data)->mean_y) += pars[0];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			double meanY = ((mean_variance_data_*)data)->mean_y;
			(((mean_variance_data_*)data)->variance_x) += (pars[4] - meanX) * (pars[4] - meanX);
			(((mean_variance_data_*)data)->variance_y) += (pars[0] - meanY) * (pars[0] - meanY);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			std::pair<double,double>* y = &((limits_data_*)data)->y_mm;
			x->first = std::min(x->first, pars[4]);
			x->second = std::max(x->second, pars[4]);
			y->first = std::min(y->first, pars[0]);
			y->second = std::max(y->second, pars[0]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((hist_fill_data_*)data)->phist)->Fill(pars[4], pars[0]);
			return true;
		});
		break;
	}
	case Type::PMT_A_S:
	case Type::MPPC_A_S:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[1];
			(((mean_variance_data_*)data)->mean_y) += pars[0];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			double meanY = ((mean_variance_data_*)data)->mean_y;
			(((mean_variance_data_*)data)->variance_x) += (pars[1] - meanX) * (pars[1] - meanX);
			(((mean_variance_data_*)data)->variance_y) += (pars[0] - meanY) * (pars[0] - meanY);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			std::pair<double,double>* y = &((limits_data_*)data)->y_mm;
			x->first = std::min(x->first, pars[1]);
			x->second = std::max(x->second, pars[1]);
			y->first = std::min(y->first, pars[0]);
			y->second = std::max(y->second, pars[0]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((hist_fill_data_*)data)->phist)->Fill(pars[1], pars[0]);
			return true;
		});
		break;
	}
	case Type::MPPC_coord:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[((mean_variance_data_*)data)->ch_size];
			(((mean_variance_data_*)data)->mean_y) += pars[((mean_variance_data_*)data)->ch_size + 1];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			double meanY = ((mean_variance_data_*)data)->mean_y;
			int ch_sz = ((mean_variance_data_*)data)->ch_size;
			(((mean_variance_data_*)data)->variance_x) += (pars[ch_sz] - meanX) * (pars[ch_sz] - meanX);
			(((mean_variance_data_*)data)->variance_y) += (pars[ch_sz + 1] - meanY) * (pars[ch_sz + 1] - meanY);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			std::pair<double,double>* y = &((limits_data_*)data)->y_mm;
			x->first = std::min(x->first, pars[((limits_data_*)data)->ch_size]);
			x->second = std::max(x->second, pars[((limits_data_*)data)->ch_size]);
			y->first = std::min(y->first, pars[((limits_data_*)data)->ch_size + 1]);
			y->second = std::max(y->second, pars[((limits_data_*)data)->ch_size + 1]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((hist_fill_data_*)data)->phist)->Fill(pars[((hist_fill_data_*)data)->ch_size], pars[((hist_fill_data_*)data)->ch_size+1]);
			return true;
		});
		break;
	}
	case Type::MPPC_Npe_profile:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[0] * pars[2];
			(((mean_variance_data_*)data)->mean_y) += pars[1] * pars[2];
			(((mean_variance_data_*)data)->stat_weight) += pars[2];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			double meanY = ((mean_variance_data_*)data)->mean_y;
			(((mean_variance_data_*)data)->variance_x) += (pars[0] - meanX) * (pars[0] - meanX) * pars[2];
			(((mean_variance_data_*)data)->variance_y) += (pars[1] - meanY) * (pars[1] - meanY) * pars[2];
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			std::pair<double,double>* y = &((limits_data_*)data)->y_mm;
			x->first = std::min(x->first, pars[0]);
			x->second = std::max(x->second, pars[0]);
			y->first = std::min(y->first, pars[1]);
			y->second = std::max(y->second, pars[1]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((hist_fill_data_*)data)->phist)->Fill(pars[0], pars[1], pars[2]);
			return true;
		});
		break;
	}
	case Type::MPPC_Npe_profile_x:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[0] * pars[2];
			(((mean_variance_data_*)data)->stat_weight) += pars[2];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += (pars[0] - meanX) * (pars[0] - meanX) * pars[2];
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			x->first = std::min(x->first, pars[0]);
			x->second = std::max(x->second, pars[0]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[0], pars[2]);
			return true;
		});
		break;
	}
	case Type::MPPC_Npe_profile_y:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[1] * pars[2];
			(((mean_variance_data_*)data)->stat_weight) += pars[2];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += (pars[1] - meanX) * (pars[1] - meanX) * pars[2];
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			x->first = std::min(x->first, pars[1]);
			x->second = std::max(x->second, pars[1]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[1], pars[2]);
			return true;
		});
		break;
	}
	case Type::MPPC_Npe_sum:
	case Type::MPPC_N_sum:
	case Type::MPPC_S_sum:
	case Type::MPPC_coord_x:
	case Type::PMT_sum_N:
	case Type::PMT_Npe_sum:
	case Type::PMT_S_sum:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[((mean_variance_data_*)data)->ch_size];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			int ch_sz = ((mean_variance_data_*)data)->ch_size;
			(((mean_variance_data_*)data)->variance_x) += (pars[ch_sz] - meanX) * (pars[ch_sz] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
			p->first = std::min(p->first, pars[((limits_data_*)data)->ch_size]);
			p->second = std::max(p->second, pars[((limits_data_*)data)->ch_size]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[((hist_fill_data_*)data)->ch_size]);
			return true;
		});
		break;
	}
	case Type::MPPC_coord_y:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[((mean_variance_data_*)data)->ch_size + 1];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			int ch_sz = ((mean_variance_data_*)data)->ch_size + 1;
			(((mean_variance_data_*)data)->variance_x) += (pars[ch_sz] - meanX) * (pars[ch_sz] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
			p->first = std::min(p->first, pars[((limits_data_*)data)->ch_size + 1]);
			p->second = std::max(p->second, pars[((limits_data_*)data)->ch_size + 1]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[((hist_fill_data_*)data)->ch_size + 1]);
			return true;
		});
		break;
	}
	case Type::MPPC_coord_disp:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[((mean_variance_data_*)data)->ch_size + 2];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			int ch_sz = ((mean_variance_data_*)data)->ch_size + 2;
			(((mean_variance_data_*)data)->variance_x) += (pars[ch_sz] - meanX) * (pars[ch_sz] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
			p->first = std::min(p->first, pars[((limits_data_*)data)->ch_size + 2]);
			p->second = std::max(p->second, pars[((limits_data_*)data)->ch_size + 2]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[((hist_fill_data_*)data)->ch_size + 2]);
			return true;
		});
		break;
	}
	case Type::PMT_tbS:
	case Type::MPPC_tbS:
	case Type::MPPC_tbS_sum: {
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[4] * pars[0];
			(((mean_variance_data_*)data)->stat_weight) += pars[0];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += pars[0] * (pars[4] - meanX) * (pars[4] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			x->first = std::min(x->first, pars[4]);
			x->second = std::max(x->second, pars[4]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[4], pars[0]);
			return true;
		});
		break;
	}
	case Type::PMT_tbN:
	case Type::PMT_tbNpe:
	case Type::MPPC_tbN:
	case Type::MPPC_tbNpe_sum:
	case Type::MPPC_tbN_sum:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double weight = pars[5] >= 0 ? pars[5] : 0; //pars[5] is < 0 when there is no calibration. In that case ignore input to avoid confusion
			(((mean_variance_data_*)data)->mean_x) += weight*pars[4];
			(((mean_variance_data_*)data)->stat_weight) += weight;
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double weight = pars[5] >= 0 ? pars[5] : 0;
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += weight*(pars[4] - meanX) * (pars[4] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			int weight = pars[5] >= 0 ? pars[5] : 0;
			if (0!=weight) {
				std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
				p->first = std::min(p->first, pars[4]);
				p->second = std::max(p->second, pars[4]);
			}
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double weight = pars[5] >= 0 ? pars[5] : 1;
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[4], weight);
			return true;
		});
		break;

	}
	case Type::Correlation:
	case Type::CorrelationAll:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[0];
			(((mean_variance_data_*)data)->mean_y) += pars[1];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			double meanY = ((mean_variance_data_*)data)->mean_y;
			(((mean_variance_data_*)data)->variance_x) += (pars[0] - meanX) * (pars[0] - meanX);
			(((mean_variance_data_*)data)->variance_y) += (pars[1] - meanY) * (pars[1] - meanY);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* x = &((limits_data_*)data)->x_mm;
			std::pair<double,double>* y = &((limits_data_*)data)->y_mm;
			x->first = std::min(x->first, pars[0]);
			x->second = std::max(x->second, pars[0]);
			y->first = std::min(y->first, pars[1]);
			y->second = std::max(y->second, pars[1]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((hist_fill_data_*)data)->phist)->Fill(pars[0], pars[1]);
			return true;
		});
		break;
	}
	default: {
		std::cerr << "PostProcessor::update: Warning! Not implemented type " << type_name(current_type) << std::endl;
	}
	}

	Operation op_fill_count(&fills_counter, true, true, false);
	Operation op_drawn_fill_count(&drawn_fills_counter, true, true, true);
	Operation op_limits(&limits_finder, true, true, false);
	Operation op_drawn_limits(&drawn_limits_finder, true, true, true);
	Operation op_mean_taker(&mean_taker, true, true, false);
	Operation op_drawn_mean_taker(&drawn_mean_taker, true, true, true);

	std::vector<Operation> vec;
	if (op_fill_count.operation->isValid() && !setups->num_of_fills)
		vec.push_back(op_fill_count);
	if (op_drawn_fill_count.operation->isValid() && !setups->num_of_drawn_fills)
		vec.push_back(op_drawn_fill_count);
	if (op_limits.operation->isValid() && (!setups->x_lims || !setups->y_lims))
		vec.push_back(op_limits);
	if (op_drawn_limits.operation->isValid() && (!setups->x_drawn_lims || !setups->y_drawn_lims))
		vec.push_back(op_drawn_limits);
	if (op_mean_taker.operation->isValid() && (!setups->x_mean || !setups->y_mean))
		vec.push_back(op_mean_taker);
	if (op_drawn_mean_taker.operation->isValid() && (!setups->x_drawn_mean || !setups->y_drawn_mean))
		vec.push_back(op_drawn_mean_taker);

	LoopThroughData(vec, current_channel, current_type);
	vec.clear();

	if (op_fill_count.operation->isValid() && !setups->num_of_fills)
		setups->num_of_fills = num_of_fills;
	if (op_drawn_fill_count.operation->isValid() && !setups->num_of_drawn_fills)
		setups->num_of_drawn_fills = num_of_drawn_fills;
	if (!setups->num_of_runs)
		setups->num_of_runs = numOfRuns(); //No point (and problematic) calculating it through LoopThroughData
	if (op_limits.operation->isValid() && (!setups->x_lims || !setups->y_lims)) {
		setups->x_lims = limits.x_mm;
		setups->y_lims = limits.y_mm;
	}
	if (op_drawn_limits.operation->isValid() && (!setups->x_drawn_lims || !setups->y_drawn_lims)) {
		setups->x_drawn_lims = drawn_limits.x_mm;
		setups->y_drawn_lims = drawn_limits.y_mm;
	}
	const std::size_t zero = 0;
	if (op_mean_taker.operation->isValid() && (!setups->x_mean || !setups->y_mean)) {
		setups->stat_weight = mvar_data.stat_weight == 0 ? boost::none : (boost::optional<long double>) mvar_data.stat_weight;
		setups->x_mean = (setups->stat_weight == boost::none ? ((setups->num_of_fills <= zero || setups->num_of_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) (mvar_data.mean_x / *setups->num_of_fills))
				: mvar_data.mean_x / *setups->stat_weight);
		setups->y_mean = (setups->stat_weight == boost::none ? ((setups->num_of_fills <= zero || setups->num_of_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) (mvar_data.mean_y / *setups->num_of_fills))
				: mvar_data.mean_y / *setups->stat_weight);
		if (setups->x_mean == boost::none) {
			setups->x_variance = boost::none;
			mvar_data.mean_x = 0;
			variance_taker.SetFunction(NULL);
		}
		if (setups->y_mean == boost::none) {
			setups->y_variance = boost::none;
			mvar_data.mean_y = 0;
		}
	}
	if (op_drawn_mean_taker.operation->isValid() && (!setups->x_drawn_mean || !setups->y_drawn_mean)) {
		setups->stat_drawn_weight = mvar_drawn_data.stat_weight == 0 ? boost::none : (boost::optional<long double>) mvar_drawn_data.stat_weight;
		setups->x_drawn_mean = (setups->stat_drawn_weight == boost::none ? ((setups->num_of_drawn_fills <= zero || setups->num_of_drawn_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) (mvar_drawn_data.mean_x / *setups->num_of_drawn_fills))
				: mvar_drawn_data.mean_x / *setups->stat_drawn_weight);
		setups->y_drawn_mean = (setups->stat_drawn_weight == boost::none ? ((setups->num_of_drawn_fills <= zero || setups->num_of_drawn_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) (mvar_drawn_data.mean_y / *setups->num_of_drawn_fills))
				: mvar_drawn_data.mean_y / *setups->stat_drawn_weight);
		if (setups->x_drawn_mean == boost::none) {
			setups->x_drawn_variance = boost::none;
			mvar_drawn_data.mean_x = 0;
			drawn_variance_taker.SetFunction(NULL);
		}
		if (setups->y_drawn_mean == boost::none) {
			setups->y_drawn_variance = boost::none;
			mvar_drawn_data.mean_y = 0;
		}
	}
	// Now can set default setups using values calculated in the first LoopThroughData.
	if (setups->use_default_setups) {
		default_hist_setups(setups);
		if (setups->use_fit)
			Invalidate(invFit | invFitFunction);
		setups->use_default_setups = true;
	}
	
	prepare_histogram();

	hist_fill_data.phist = isTH1Dhist(current_type) ? (void*) get_current_hist1() : (void*) get_current_hist2();
	if (NULL == hist_fill_data.phist) {
		std::cerr<<"PostProcessor::update: Error: NULL histogram pointer"<<std::endl;
		histogram_filler.SetFunction(NULL);
	}
	hist_fill_data.ch_size = isPMTtype(current_type) ? PMT_channels.size() : MPPC_channels.size();

	Operation op_hist_fill(&histogram_filler, true, true, false);
	Operation op_variance_taker(&variance_taker, true, true, false);
	Operation op_drawn_variance_taker(&drawn_variance_taker, true, true, true);

	if (op_hist_fill.operation->isValid() && !setups->filled_hist)
		vec.push_back(op_hist_fill);
	if (op_variance_taker.operation->isValid() && (!setups->x_variance || !setups->y_variance))
		vec.push_back(op_variance_taker);
	if (op_drawn_variance_taker.operation->isValid() && (!setups->x_drawn_variance || !setups->y_drawn_variance))
		vec.push_back(op_drawn_variance_taker);

	LoopThroughData(vec, current_channel, current_type);
	vec.clear();

	const std::size_t one = 1;
	if (op_hist_fill.operation->isValid() && !setups->filled_hist)
		setups->filled_hist = true;
	if (op_variance_taker.operation->isValid() && (!setups->x_variance|| !setups->y_variance)) {
		setups->x_variance = (setups->stat_weight == boost::none ? ((setups->num_of_fills <= one || setups->num_of_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) std::sqrt(mvar_data.variance_x / ((*setups->num_of_fills -1) * *setups->num_of_fills)))
				: mvar_data.variance_x / *setups->stat_weight);
		setups->y_variance = (setups->stat_weight == boost::none ? ((setups->num_of_fills <= one || setups->num_of_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) std::sqrt(mvar_data.variance_y / ((*setups->num_of_fills -1) * *setups->num_of_fills)))
				: mvar_data.variance_y / *setups->stat_weight);
	}
	if (op_drawn_variance_taker.operation->isValid() && (!setups->x_drawn_variance || !setups->y_drawn_variance)) {
		setups->x_drawn_variance = (setups->stat_drawn_weight == boost::none ? ((setups->num_of_drawn_fills <= one || setups->num_of_drawn_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) std::sqrt(mvar_drawn_data.variance_x / ((*setups->num_of_drawn_fills -1) * *setups->num_of_drawn_fills)))
				: mvar_drawn_data.variance_x / *setups->stat_drawn_weight);
		setups->y_drawn_variance = (setups->stat_drawn_weight == boost::none ? ((setups->num_of_drawn_fills <= one || setups->num_of_drawn_fills == boost::none) ?
				(boost::optional<double>) boost::none : (boost::optional<double>) std::sqrt(mvar_drawn_data.variance_y / ((*setups->num_of_drawn_fills -1) * *setups->num_of_drawn_fills)))
				: mvar_drawn_data.variance_y / *setups->stat_drawn_weight);
	}
	if (!setups->x_max || !setups->y_max) {
		if (isTH1Dhist(current_type)) {
			TH1D *hist = get_current_hist1();
			if (NULL == hist) {
				std::cerr<<"PostProcessor::update: Error: NULL 1D histogram"<<std::endl;
			} else {
				Int_t bin = hist->GetMaximumBin();
				setups->x_max = hist->GetBinCenter(bin);
				setups->y_max = hist->GetBinContent(bin);
			}
		} else {
			TH2D *hist = get_current_hist2();
			if (NULL == hist) {
				std::cerr<<"PostProcessor::update: Error: NULL 2D histogram"<<std::endl;
			} else {
				Int_t bin = hist->GetMaximumBin();
				Int_t XI, YI, ZI;
				hist->GetBinXYZ(bin, XI, YI, ZI);
				setups->x_max = hist->GetXaxis()->GetBinCenter(XI);
				setups->y_max = hist->GetYaxis()->GetBinCenter(YI);
			}
		}
	}
	//Last of default parameters are fit parameters' domains, which may depend on histogram, hence another call after 2nd Loop
	if (setups->use_default_setups) {
		default_hist_setups(setups);
		if (setups->use_fit)
			Invalidate(invFit | invFitFunction);
	}

	post_fill_transform();

	fit_and_draw_histogram();

	update_physical();
	return true;
}

std::size_t PostProcessor::events_number(void) const
{
	return MPPC_channels.empty() ? data->pmt_peaks[current_exp_index][0].size() :
				data->mppc_peaks[current_exp_index][0].size();
}

// Run cuts are applied!
std::size_t PostProcessor::numOfRuns(void)
{
	std::size_t run_n = 0;
	int ch_ind = channel_to_index(current_channel, current_type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *run_cuts = (NULL== get_run_cuts(current_exp_index) ? &empty : get_run_cuts(current_exp_index));
	std::size_t run_size = isPMTtype(current_type) ? data->pmt_peaks[current_exp_index][ch_ind].size() : data->mppc_peaks[current_exp_index][ch_ind].size();
	for (std::size_t run = 0; run != run_size; ++run){
		for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
			if (kFALSE == cut->GetAccept(run)) // Not calculating it here! Evaluation is done in set_as_run_cut
				goto _cutted;
		++run_n;
	_cutted:;
	}
	return run_n;
}

void PostProcessor::default_hist_setups(HistogramSetups* setups)//does not affect cuts
{
	if (NULL==setups) {
		std::cerr<<"PostProcessor::default_hist_setups: Error: NULL HistogramSetups*"<<std::endl;
		return;
	}

	int SiPM_n = std::round(std::sqrt(MPPC_coords.size()));
	double SiPM_x_min = 0, SiPM_x_max = 0, SiPM_y_min = 0, SiPM_y_max = 0;
	for (auto i = MPPC_coords.begin(); i!=MPPC_coords.end(); ++i) {
		SiPM_x_min = std::min(SiPM_x_min, i->second.first);
		SiPM_x_max = std::max(SiPM_x_max, i->second.first);
		SiPM_y_min = std::min(SiPM_y_min, i->second.second);
		SiPM_y_max = std::max(SiPM_y_max, i->second.second);
	}

	setups->use_default_setups = false;
	int _N_ = numOfFills(false);
	setups->N_bins = _N_;
	setups->N_bins = std::max(4,(int)std::round(std::sqrt(setups->N_bins)));
	setups->N_bins_y = setups->N_bins;

	setups->use_fit = false;
	setups->fitted = false;
	setups->is_valid_fit_function = false;
	setups->filled_hist = false;

	setups->N_gauss = 0;
	setups->par_val.resize(0, 0);
	setups->par_left_limits.resize(0, 0);
	setups->par_right_limits.resize(0, 0);
	switch (current_type)
	{
	case MPPC_Npe_profile: {
		double bin_size_x = (SiPM_x_max - SiPM_x_min) / (2*SiPM_n - 2);
		double bin_size_y = (SiPM_y_max - SiPM_y_min) / (2*SiPM_n - 2);
		setups->N_bins = 2*SiPM_n + 1;
		setups->N_bins_y = setups->N_bins;
		setups->x_zoom = std::pair<double, double> (SiPM_x_min - bin_size_x * 1.5, SiPM_x_max + bin_size_x * 1.5);
		setups->y_zoom = std::pair<double, double> (SiPM_y_min - bin_size_y * 1.5, SiPM_y_max + bin_size_y * 1.5);
		setups->is_zoomed = std::pair<bool, bool> (true, true);
		break;
	}
	case MPPC_Npe_profile_y: {
		double bin_size_x = (SiPM_y_max - SiPM_y_min) / (2*SiPM_n - 2);
		setups->N_bins = 2*SiPM_n + 1;
		setups->x_zoom = std::pair<double, double> (SiPM_y_min - bin_size_x * 1.5, SiPM_y_max + bin_size_x * 1.5);
		setups->is_zoomed.first = true;
		break;
	}
	case MPPC_Npe_profile_x: {
		double bin_size_x = (SiPM_x_max - SiPM_x_min) / (2*SiPM_n - 2);
		setups->N_bins = 2*SiPM_n + 1;
		setups->x_zoom = std::pair<double, double> (SiPM_x_min - bin_size_x * 1.5, SiPM_x_max + bin_size_x * 1.5);
		setups->is_zoomed.first = true;
		break;
	}
	default: {
	}
	}
}

void PostProcessor::save_all(void)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	for (int ch = data->exp_area.channels.get_next_index(); ch != -1; ch = data->exp_area.channels.get_next_index())
		save(ch);
	calibr_info.Save(data_output_path + calibration_file);
}

void PostProcessor::saveAs(std::string path, bool png_only)
{
	print_hist(path, png_only);
}

void PostProcessor::clear(void)	//clear cuts for current histogram. Run cuts derived from it are not touched
{
	HistogramSetups def_setups(channel_list(current_type));
	unset_trigger_offsets();
	default_hist_setups(&def_setups);
	set_hist_setups(&def_setups, current_exp_index, current_channel, current_type); //Creates copy!
	update();
}

void PostProcessor::clearAll(void) //clear everything, return to initial state (leaves all existing histograms empty)
{
	for (std::size_t c =0, c_end_ = manual_setups.size(); c!=c_end_; ++c) {
		for (std::size_t h = 0, h_end_ = manual_setups[c].size(); h!=h_end_; ++h) {
			for (std::size_t exp = 0, exp_end_ = manual_setups[c][h].size(); exp != exp_end_; ++exp) {
				for (std::size_t ch=0, ch_end_ = manual_setups[c][h][exp].size(); ch!=ch_end_; ++ch) {
					if (NULL!=manual_setups[c][h][exp][ch]) {
						delete manual_setups[c][h][exp][ch];
						manual_setups[c][h][exp][ch] = NULL;
					}
				}
			}
		}
	}
	for (std::size_t c =0, c_end_ = RunCuts.size(); c!=c_end_; ++c) {
		for (std::size_t exp = 0, exp_end_ = RunCuts[c].size(); exp != exp_end_; ++exp) {
			RunCuts[c][exp].clear();
		}
	}
	for (std::size_t exp = 0, exp_end_ = data->trigger_offset.size(); exp != exp_end_; ++exp) {
		for (auto ev = data->trigger_offset[exp].begin(), ev_end_ = data->trigger_offset[exp].end(); ev != ev_end_; ++ev)
			(*ev) = 0;
	}
	for (std::size_t c = 0, c_end_=canvases.size(); c!=c_end_ ; ++c) {
		canvases[c]->Clear();
		canvases[c]->Update();
	}
	StateChange(current_channel, current_exp_index, current_type, canvas_ind, current_channel, current_exp_index, current_type, canvas_ind); //To create HistogramSetups as at the start of the program
	update();
}

void PostProcessor::post_fill_transform(void)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cerr<<__PRETTY_FUNCTION__<<": Error: NULL setups"<<std::endl;
	}
	std::string exp_str = experiments[current_exp_index];
	switch (current_type) {
	case Type::MPPC_Npe_profile:
	{
		if (setups->num_of_runs!=boost::none && *setups->num_of_runs > 0) {
			TH2D * hist = get_current_hist2();
			if (NULL == hist) {
				std::cerr<<__PRETTY_FUNCTION__<<": Error! NULL histogram for " << data->exp_area.experiments[current_exp_index] << std::endl;
				break;
			}
			hist->Scale(1.0/(*setups->num_of_runs));
			hist->SetLineColor(TColor::GetColorDark(kBlue));
			hist->SetFillStyle(0);
			hist->SetStats(setups->draw_stats);
		} else {
			std::cerr<<__PRETTY_FUNCTION__<<": Error! 0 runs for " << data->exp_area.experiments[current_exp_index] << std::endl;
		}
		break;
	}
	case Type::MPPC_Npe_profile_x:
	case Type::MPPC_Npe_profile_y:
	{
		if (setups->num_of_runs!=boost::none && *setups->num_of_runs > 0) {
			TH1D * hist = get_current_hist1();
			if (NULL == hist) {
				std::cerr<<__PRETTY_FUNCTION__<<": Error! NULL histogram for " << data->exp_area.experiments[current_exp_index] << std::endl;
				break;
			}
			hist->Scale(1.0/(*setups->num_of_runs));
			hist->SetLineColor(kBlack);
			hist->SetFillStyle(3335);
			hist->SetFillColor(kBlack);
			hist->SetStats(setups->draw_stats);
		} else {
			std::cerr<<__PRETTY_FUNCTION__<<": Error! 0 runs for " << data->exp_area.experiments[current_exp_index] << std::endl;
		}
		break;
	}
	default: {
		if (isTH1Dhist(current_type)) {
			TH1D * hist = get_current_hist1();
			if (NULL == hist) {
				std::cerr<<__PRETTY_FUNCTION__<<": Error! NULL histogram for " << data->exp_area.experiments[current_exp_index] << std::endl;
				break;
			}
			hist->SetLineColor(TColor::GetColorDark(kBlue));
			hist->SetFillStyle(0);
			hist->SetStats(setups->draw_stats);
		} else {
			TH2D * hist = get_current_hist2();
			if (NULL == hist) {
				std::cerr<<__PRETTY_FUNCTION__<<": Error! NULL histogram for " << data->exp_area.experiments[current_exp_index] << std::endl;
				break;
			}
			hist->SetStats(setups->draw_stats);
		}
		break;
	}
	}
}


//updates derived values such as S2 amplitude, S of single photoelectron etc.
void PostProcessor::update_physical(void)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::update_physical: Error: NULL setups"<<std::endl;
	}
	std::string exp_str = experiments[current_exp_index];
	switch (current_type) {
	case Type::MPPC_S2:	//TODO: Warning!: these two types (MPPC_S2) overwrite each other
	{
		if (!(setups->N_gauss>0 && setups->fitted)) {
			if (0==setups->N_gauss) { //Use mean then
				if (setups->x_drawn_mean == boost::none || (setups->stat_drawn_weight == boost::none && setups->num_of_drawn_fills == boost::none))
					std::cout << "Warning! No mean S2 value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else {
					std::cout<<"Mean = "<<*setups->x_drawn_mean<<std::endl;
				}
			} else
				std::cout << "Warning! No S2 area for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	case Type::MPPC_Ss:
	{
		CalibrationInfo::S1pe_method meth = calibr_info.get_method(current_channel, current_exp_index);
		auto entry = MPPC_V.find(exp_str);
		double V = (entry == MPPC_V.end() ? 0 : entry->second);
		if (setups->x_drawn_mean == boost::none || (setups->stat_drawn_weight == boost::none && setups->num_of_drawn_fills == boost::none))
			std::cout << "Warning! No mean Ss value for current histogram: " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		else
			std::cout << "Current mean value = "<< *setups->x_drawn_mean << std::endl;
		if (meth == CalibrationInfo::Ignore) {
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = "<<calibr_info.get_S1pe(current_channel, current_exp_index) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::UsingMean) {
			if (setups->x_drawn_mean != boost::none && (setups->stat_drawn_weight != boost::none || setups->num_of_drawn_fills != boost::none))
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, *setups->x_drawn_mean,
						setups->stat_drawn_weight == boost::none ? *setups->num_of_drawn_fills : *setups->stat_drawn_weight); //TODO!!! implement accepting double
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = "<<calibr_info.get_S1pe(current_channel, current_exp_index) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::Using1pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>0 && setups->fitted)
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, setups->par_val[1], 1); //TODO: add calculation of N (weight) under fit
			else {
				std::cout << "Warning! No calibration S1pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		if (meth == CalibrationInfo::Using2pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>=2 && setups->fitted)
				calibr_info.set_S2pe_exp(current_channel, current_exp_index, setups->par_val[4], 1); //TODO: add calculation of N (weight) under fit
			else {
				std::cout << "Warning! No calibration S2pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		calibr_info.calculateS1pe(current_channel, V);
		std::cout << "Resulting calibration S1pe = "<<calibr_info.get_S1pe(current_channel, current_exp_index) << std::endl;
		break;
	}
	case Type::PMT_Ss:
	{
		CalibrationInfo::S1pe_method meth = calibr_info.get_method(current_channel, current_exp_index);
		auto entry = PMT_V.find(exp_str);
		double V = (entry == PMT_V.end() ? 0 : entry->second);
		if (setups->x_drawn_mean == boost::none || (setups->stat_drawn_weight == boost::none && setups->num_of_drawn_fills == boost::none))
			std::cout << "Warning! No mean calibration Ss value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		else
			std::cout << "S1pe(mean) = "<< *setups->x_drawn_mean << std::endl;
		if (meth == CalibrationInfo::Ignore) {
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = " << calibr_info.get_S1pe(current_channel, current_exp_index) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::UsingMean) {
			if (setups->x_drawn_mean != boost::none && (setups->stat_drawn_weight != boost::none || setups->num_of_drawn_fills != boost::none))
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, *setups->x_drawn_mean,
						setups->stat_drawn_weight == boost::none ? *setups->num_of_drawn_fills : *setups->stat_drawn_weight); //TODO!!! implement accepting double
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = " << calibr_info.get_S1pe(current_channel, current_exp_index) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::Using1pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>0 && setups->fitted)
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, setups->par_val[1], 1); //TODO: add calculation of N (weight) under fit
			else {
				std::cout << "Warning! No calibration S1pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		if (meth == CalibrationInfo::Using2pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss >= 2 && setups->fitted)
				calibr_info.set_S2pe_exp(current_channel, current_exp_index, setups->par_val[4], 1); //TODO: add calculation of N (weight) under fit
			else {
				std::cout << "Warning! No calibration S2pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		calibr_info.calculateS1pe(current_channel, V);
		std::cout << "Resulting calibration S1pe = " << calibr_info.get_S1pe(current_channel, current_exp_index) << std::endl;
		break;
	}
	case Type::PMT_S2_S:
	{
		if (setups->N_gauss > 0 && setups->fitted) {
		}
		else {
			if (0==setups->N_gauss) { //Use mean then
				if (setups->x_drawn_mean == boost::none || (setups->stat_drawn_weight == boost::none && setups->num_of_drawn_fills == boost::none))
					std::cout << "Warning! No S2 area for PMT " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else {
					std::cout<<"Mean = "<< *setups->x_drawn_mean <<std::endl;
				}
			}
		}
		break;
	}
	case Type::MPPC_tbS:
	case Type::MPPC_tbS_sum:
	case Type::MPPC_t_S:
	case Type::PMT_t_S:
	case Type::PMT_tbS:
	{
		/*
		LoopThroughData(operations, current_channel, current_type); //DONE: A LOT of loops here. Rework the code to avoid this (store more info/call all relevant loops in one place)
		if (0==stat_data.weight)
			std::cout << "Warning! No peaks selected ch " << current_channel << std::endl;
		else {
			std::pair<double, double> drawn_limits = hist_x_limits(true);
			std::cout <<"N runs = "<< numOfRuns() << " S  = "<<stat_data.val<< " Npeaks = "<<stat_data.weight<<" dT = "
					<<drawn_limits.second - drawn_limits.first<<std::endl;
		}*/
		break;
	}
	case Type::PMT_tbN:
	case Type::MPPC_tbN:
	case Type::MPPC_tbNpe_sum:
	{
		/*
		LoopThroughData(operations, current_channel, current_type);
		if (0==stat_data.weight)
			std::cout << "Warning! No peaks selected ch " << current_channel << std::endl;
		else {
			std::pair<double, double> drawn_limits = hist_x_limits(true);
			std::cout <<"N runs = "<< numOfRuns() << " Npeaks = "<<stat_data.weight<<" dT = "
					<<drawn_limits.second - drawn_limits.first<<std::endl;
		}*/
		break;
	}
	default:
		break;
	}
}

void PostProcessor::set_as_run_cut(std::string name)//adds current drawn_limits in HistogramSetups to runs cut (from current exp, channel and type)
{
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (current_type==CorrelationAll) {
		std::cout<<"Can't use multi E data for run cut."<<std::endl;
		return;
	}
	if (!isPerRun(current_type)) {
		std::cout<<"Can't use current type ("<<type_name(current_type)<<") for event (run) cut"<<std::endl;
		return;
	}
	std::deque<EventCut> *RunCuts = get_run_cuts(current_exp_index);
	if (NULL==RunCuts) {
		return;
	}
	unset_as_run_cut(name);
	int run_size = isPMTtype(current_type) ?
		data->pmt_peaks[current_exp_index][0].size()
		:data->mppc_peaks[current_exp_index][0].size();
	RunCuts->push_back(EventCut(run_size, EventCut::RunCut, name));
	RunCuts->back().SetChannel(current_channel);
	RunCuts->back().SetExperiment(current_exp_index);
	RunCuts->back().SetType(current_type);
	for (auto run = 0; run != run_size; ++run)
		RunCuts->back().SetAccept(run, kFALSE);

	FunctionWrapper cut_calculator(&(RunCuts->back()));
	cut_calculator.SetFunction([](std::vector<double> &pars, int run, void *data) { //if operation is run at least once, the run is accepted.
		((EventCut*)data)->SetAccept(run, true);
		return true;
	});
	Operation op;
	op.operation = &cut_calculator;
	op.apply_run_cuts = false;
	op.apply_hist_cuts = true;
	op.apply_phys_cuts = true;
	std::vector<Operation> operations(1, op);
	LoopThroughData(operations, current_channel, current_type);
	Invalidate(invCuts);
}

void PostProcessor::unset_as_run_cut(std::string name)
{
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	std::deque<EventCut> *RunCuts = get_run_cuts(current_exp_index);
	if (NULL==RunCuts) {
		return;
	}
	while (true) {
		for (auto i = RunCuts->begin(); i != RunCuts->end(); ++i){
			if (i->GetName() == name){
				RunCuts->erase(i);
				goto anew;
			}
		}
		Invalidate(invCuts);
		break;
		anew:;
	}
}

void PostProcessor::print_events (std::string file, int run_offset, int sub_runs, bool accepted) //DONE: impossible because trigger offset values are different: compress the output to intervals
{
	std::ofstream str;
	open_output_file(file, str, std::ios_base::trunc);
	std::size_t run_n = 0;
	int ch_ind = channel_to_index(current_channel, current_type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *run_cuts = (NULL== get_run_cuts(current_exp_index) ? &empty : get_run_cuts(current_exp_index));
	std::size_t run_size = isPMTtype(current_type) ? data->pmt_peaks[current_exp_index][ch_ind].size() : data->mppc_peaks[current_exp_index][ch_ind].size();
	str<<"//run\tsubrun\ttrigger"<<std::endl;
	for (std::size_t run = 0; run != run_size; ++run) {
		int true_run = run_offset + (int)run_n/sub_runs;
		int sub_run = (run_n)%(sub_runs);
		for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
			if ((!accepted) == cut->GetAccept(run))//not calculating it here!
				goto _cutted;
		str<< true_run <<"\t"<<sub_run<<"\t"<<data->trigger_offset[current_exp_index][run]<<std::endl;
	_cutted:;
		++run_n;
	}
}

void PostProcessor::do_fit(bool do_fit)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (!isTH1Dhist(current_type)) {
		std::cout << "PostProcessor::do_fit: fit is ignored for 2D histogram" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	TH1D* hist1 = get_current_hist1();
	if (NULL==setups) {
		std::cout<<"PostProcessor::do_fit: Error: NULL setups"<<std::endl;
	}
	if (NULL==hist1) {
		std::cout<<"PostProcessor::do_fit: Error: NULL histogram"<<std::endl;
	}
	setups->use_fit = do_fit;
	Invalidate(invFit);
}

void PostProcessor::status(Bool_t full)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}

	std::cout << "Current experiment: " << data->exp_area.experiments[current_exp_index]
		<< " (" << data->Fields[current_exp_index] << " kV)" << std::endl;
	std::cout << "Current channel: " << current_channel<< std::endl;
	std::cout << "Current type: " << type_name(current_type)<<std::endl;
	if (Type::Correlation==current_type)
		std::cout << "\t" << type_name(_x_corr)<<" Ch#"<<_x_corr_ch<<" & "<< type_name(_y_corr)<<" Ch#"<<_y_corr_ch<<std::endl;

	if (full) {
		HistogramSetups *setups = get_hist_setups();
		if (NULL==setups) {
			std::cout<<"PostProcessor::status: Error: NULL setups"<<std::endl;
			return;
		}
		list_run_cuts();
	
		std::pair<double, double> x_lims = hist_x_limits(), x_drawn_lims = hist_x_limits(true);
		std::cout << "Current_setups: " << std::hex << setups << std::dec << std::endl;
		std::cout << "\tEvent number: " << setups->num_of_runs << std::endl;
		std::cout << "\tNumber of fills: " << setups->num_of_fills << std::endl;
		std::cout << "\tNumber of drawn fills: " << setups->num_of_drawn_fills << std::endl;
		std::cout << "\tleft_limit: " << x_lims.first << std::endl;
		std::cout << "\tright_limit: " << x_lims.second << std::endl;
		std::cout << "\tleft_drawn_limit: " << x_drawn_lims.first << std::endl;
		std::cout << "\tright_drawn_limit: " << x_drawn_lims.second << std::endl;
		std::cout << "\tN_gauss: " << setups->N_gauss << std::endl;
		std::cout << "\tN_bins: " << setups->N_bins << std::endl;
		std::cout << "\tpar_val [" << setups->par_val.size() << "]: ";
		for (auto i = setups->par_val.begin(), _end_ = setups->par_val.end(); i != _end_; ++i)
			std::cout << (*i) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;
		std::cout << "\tpar_left_limits [" << setups->par_left_limits.size() << "]: ";
		for (auto i = setups->par_left_limits.begin(), _end_ = setups->par_left_limits.end(); i != _end_; ++i)
			std::cout << (*i) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;
		std::cout << "\tpar_right_limits [" << setups->par_right_limits.size() << "]: ";
		for (auto i = setups->par_right_limits.begin(), _end_ = setups->par_right_limits.end(); i != _end_; ++i)
			std::cout << (*i) << ((i == (_end_ - 1)) ? "" : " ");
		std::cout << std::endl;
		std::cout << "\tusing fit: " << setups->use_fit << std::endl;
		std::cout << "\tfitted: " << setups->fitted << std::endl;
		list_hist_cuts();
	}
}

bool PostProcessor::unset_trigger_offsets(void)
{
	for (auto ev = data->trigger_offset[current_exp_index].begin(), ev_end_ = data->trigger_offset[current_exp_index].end(); ev != ev_end_; ++ev)
		(*ev) = 0;
	return true;
}

bool PostProcessor::set_trigger_offsets(double extra_offset) //uses trigger type histogram only
{
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return false;
	}
	if ((!TriggerData::IsForState(this) && !TriggerFitData::IsForState(this) && !TriggerAvgTData::IsForState(this))
			|| (current_type==Type::MPPC_trigger_fit_chi2 || current_type==Type::PMT_trigger_fit_chi2)) {
		std::cerr << "PostProcessor::set_trigger_offsets:Error: Can't calculate trigger adjustment for not trigger type histogram (" << type_name(current_type) << ")" << std::endl;
		return false;
	}
	unset_trigger_offsets();
	struct temp_data {
		std::vector<double>* offsets;
		double extra_offset;
	} offset_data;
	offset_data.offsets = &(data->trigger_offset[current_exp_index]);
	offset_data.extra_offset = extra_offset;
	FunctionWrapper offset_setter(&offset_data);
	offset_setter.SetFunction([](std::vector<double> &pars, int run, void *data) {
		if (pars[0] != DBL_MAX && pars[0] != -DBL_MAX)
			(*((temp_data*)data)->offsets)[run] = ((temp_data*)data)->extra_offset - pars[0];
		return true;
	});
	Operation op;
	op.operation = &offset_setter;
	op.apply_run_cuts = false;
	op.apply_hist_cuts = true;
	op.apply_phys_cuts = false;
	std::vector<Operation> operations(1, op);
	LoopThroughData(operations, current_channel, current_type);
	Invalidate(invAll);
	update();
	return true;
}

