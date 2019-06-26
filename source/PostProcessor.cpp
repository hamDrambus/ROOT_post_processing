#include "PostProcessor.h"

PostProcessor::PostProcessor(AllExperimentsResults* _data) : 
CanvasSetups(_data->mppc_channels,_data->pmt_channels, _data->exp_area.experiments), calibr_info(this, data_output_path + calibration_file)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	data = _data;
	for (auto exp = data->exp_area.experiments.begin(); exp != data->exp_area.experiments.end(); ++exp) {
		avr_S2_S.push_back(std::deque<double>());
		avr_Double_I.push_back(std::deque<double>());
		PMT3_avr_S2_S.push_back(-1);
		PMT1_avr_S2_S.push_back(-1);
		for (auto ch = data->mppc_channels.begin(); ch != data->mppc_channels.end(); ++ch){
			avr_S2_S.back().push_back(-1);
			avr_Double_I.back().push_back(-1);
		}
	}

	update();
}

std::string PostProcessor::hist_name()
{
	std::string name = (is_PMT_type(current_type) ? "PMT#" : "MPPC#") + (isMultichannel(current_type) ? "All" : std::to_string(current_channel) )
		+ "_" + g_data->exp_area.experiments[current_exp_index];
	name += type_name(current_type);
	return name;
}

void PostProcessor::print_hist(std::string path)
{
	int ch = current_channel;
	Type type = current_type;
	int exp_ind = current_exp_index;
	std::string name = path;
	if (name=="") {
		if (isMultichannel(type)) {
			name = data_output_path + (is_PMT_type(type) ? OUTPUT_PMT_PICS : OUTPUT_MPPCS_PICS)+ type_name(type)+"/" +
					(is_PMT_type(type) ? "PMT_" : "MPPC_")+g_data->exp_area.experiments[exp_ind]
				+ type_name(type);
		} else {
			name = data_output_path + (is_PMT_type(type) ? OUTPUT_PMT_PICS : OUTPUT_MPPCS_PICS) + g_data->exp_area.experiments[exp_ind]
				+ "/" + (is_PMT_type(type) ? "PMT_" : "MPPC_") + std::to_string(ch)
				+ "/" + (is_PMT_type(type) ? "PMT_" : "MPPC_") + std::to_string(ch)
				+ type_name(type);
		}
	}
	std::ofstream str;
	open_output_file(name+".hdata", str, std::ios_base::trunc | std::ios_base::binary);
	std::size_t real_size = numOfFills(false);
	str.write((char*)&real_size, sizeof(std::size_t));
	struct temp_data {
		std::ofstream* str;
		int ch_size;
	} st_data;
	st_data.str = &str;
	st_data.ch_size = is_PMT_type(type) ? PMT_channels.size() : MPPC_channels.size();
	FunctionWrapper* writer_to_file = new FunctionWrapper(&st_data);
	switch (type)
	{
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
	case PMT_Npe_sum:
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
	case MPPC_times:
	case MPPC_times_N:
	case PMT_times:
	case PMT_times_N:
	case MPPC_sum_ts:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[4], sizeof(double));
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
	case MPPC_Double_I:
	case MPPC_S2_S:
	case MPPC_tfinal:
	case MPPC_tstart:
	case MPPC_tboth:
	case PMT_S2_int:
	case PMT_Ss:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[0], sizeof(double));
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
	case PMT_sum_N:
	{
		writer_to_file->SetFunction([](std::vector<double>& pars, int run, void* data) {
			((temp_data*)data)->str->write((char*)&pars[((temp_data*)data)->ch_size], sizeof(double));
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
	LoopThroughData(vec, ch, type);
	delete writer_to_file;
	str.close();
	if (NULL!=get_current_canvas())
		get_current_canvas()->SaveAs((name+".png").c_str(), "png");
}


Bool_t PostProcessor::StateChange(int to_ch, int to_exp, Type to_type, std::size_t to_canvas, int from_ch, int from_exp, Type from_type, std::size_t from_canvas)
{
	if (!CanvasSetups::StateChange(to_ch, to_exp, to_type, to_canvas, from_ch, from_exp, from_type, from_canvas)) {
		return kFALSE; //no change
	}
	Invalidate(invFitFunction|invFit);
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
	std::vector<double> N_pe_result;
	for (std::size_t pt = 0; pt < data->exp_area.experiments.size(); ++pt){
		if (pt <= calibr_info.get_N_calib(channel).second)
			N_pe_result.push_back(data->N_pe_direct[pt][ch_ind]);
		else
			N_pe_result.push_back(data->N_pe_Double_I[pt][ch_ind]);
	}
	std::ofstream file;
	open_output_file(data_output_path + OUTPUT_MPPCS_PICS + std::to_string(channel) + "_Npe.txt", file);
	file << "E[kV/cm]\tN_pe_direct\tN_pe_Double_I\tN_pe_result" << std::endl;
	for (std::size_t pt = 0; pt < data->exp_area.experiments.size(); ++pt)
		file << data->Fields[pt] << "\t" << data->N_pe_direct[pt][ch_ind] << "\t" << data->N_pe_Double_I[pt][ch_ind] << "\t" << N_pe_result[pt] << std::endl;
	file.close();
}

/* PARAMETERS PASSED TO FUNCTION WRAPPER (*operation) AS WELL AS CUTTER
type			[0]			[1]			[2]			[3]			[4]			[5]
.MPPC_Double_I	Double_I	------		------		------		------		------
.MPPC_S2_S		S2_S		------		------		------		------		------
MPPC_Ss			peak.S		peak.A		peak.left	peak.right	peak.t		------
MPPC_t_S		peak.S		peak.A		peak.left	peak.right	peak.t 		------	=== MPPC_Ss
MPPC_A_S		peak.S		peak.A		peak.left	peak.right	peak.t 		------	=== MPPC_Ss
MPPC_times		peak.S		peak.A		peak.left	peak.right	peak.t		------	=== MPPC_Ss
MPPC_times_N	peak.S		peak.A		peak.left	peak.right	peak.t		------	=== MPPC_Ss
MPPC_t_start	time		------		------		------		------		------
MPPC_t_both		time		------		------		------		------		------
MPPC_t_final	time		------		------		------		------		------
MPPC_sum_ts		peak.S		peak.A		peak.left	peak.right	peak.t		------
MPPC_S2																					//composite: cuts for peaks and then derived parameter. Data is derived within loop.
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t					//cuts with corresponding channel, no need to check ==-2
	2nd level:	SUM(peak.S)	-2			-2			-2			-2						//cuts with channel -1 called.
MPPC_coord===MPPC_coord_x===MPPC_coord_y														//composite: cuts for peaks and then derived parameters. Data is derived within loop.
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		------				//per channel, cuts with corresponding channel called.
	2nd level:	Npe[0]	Npe[1]	...	Npe[MPPC_channels.size()-1]		X derived		Y derived	//cuts with channel -1 called.
MPPC_Npe_sum
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		------				//per channel, cuts with corresponding channel called.
	2nd level:	Npe[0]	Npe[1]	...	Npe[MPPC_channels.size()-1]		NpeSum						//cuts with channel -1 called.
PMT_S2_S																				//composite: cuts for peaks and then derived parameter. Data is derived within loop.
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t					//cuts with corresponding channel, no need to check ==-2
	2nd level:	SUM(peak.S)	-2			-2			-2			-2						//cuts with channel -1 called.
PMT_S2_int		S2_int		------		------		------		------		------
PMT_Ss			peak.S		peak.A		peak.left	peak.right	peak.t		------
PMT_t_S			peak.S		peak.A		peak.left	peak.right	peak.t		------	=== PMT_Ss
PMT_times		peak.S		peak.A		peak.left	peak.right	peak.t		------	===	PMT_Ss
PMT_sum_N
	1st level:	peak.S		peak.A		peak.left	peak.right	peak.t		------				//per channel, cuts with corresponding channel called.
	2nd level:	Npeaks[0]	Np[1]	...	Np[PMT_channels.size()-1]		NpSum					//cuts with channel -1 called.
*/
void PostProcessor::LoopThroughData(std::vector<Operation> &operations, int channel, Type type)
{
	if (operations.empty())
		return;
	int ch_ind = channel_to_index(channel, type);
	HistogramSetups* setups = get_hist_setups(current_exp_index, channel, type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *hist_cuts = (NULL==setups ? &empty : &(setups->hist_cuts));
	std::deque<EventCut> *run_cuts = (NULL== get_run_cuts(current_exp_index) ? &empty : get_run_cuts(current_exp_index));
	std::string exp_str = experiments[current_exp_index];
	switch (type)
	{
	case Type::MPPC_Double_I:
	{
		int run_size = data->Double_I[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			cut_data[0] = data->Double_I[current_exp_index][ch_ind][run];
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
		break;
	}
	case Type::MPPC_S2_S:
	{
		int run_size = data->S2_S[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			cut_data[0] = data->S2_S[current_exp_index][ch_ind][run];
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
		break;
	}
	case Type::MPPC_t_S:
	case Type::MPPC_A_S:
	case Type::MPPC_times:
	case Type::MPPC_times_N:
	case Type::MPPC_Ss:
	{
		int run_size = data->mppc_peaks[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][ch_ind][run].size();
					pk != pk_end; ++pk) {
				bool failed_hist_cut = false; //normal cuts
				bool failed_phys_cut = false; //drawn (displayed) cuts only
				cut_data[0] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].right;
				cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][ch_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
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
	case Type::MPPC_tstart:
	case Type::MPPC_tboth:
	{
		int run_size = data->S2_start_t[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			cut_data[0] = data->S2_start_t[current_exp_index][ch_ind][run];
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
		if (type == Type::MPPC_tstart)
			break;
	}
	// no break
	case Type::MPPC_tfinal:
	{
		int run_size = data->S2_finish_t[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			cut_data[0] = data->S2_finish_t[current_exp_index][ch_ind][run];
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
		break;
	}
	case Type::MPPC_S2: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = data->mppc_peaks[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			double S2_no_cut = 0, S2_hist_cut = 0, S2_phys_cut = 0, S2_hist_and_phys_cut = 0;
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][ch_ind][run].size();
					pk != pk_end; ++pk) {
				bool failed_hist_cut = false; //normal cuts
				bool failed_phys_cut = false; //drawn (displayed) cuts only
				cut_data[0] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].S;
				cut_data[1] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].A;
				cut_data[2] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].left;
				cut_data[3] = data->mppc_peaks[current_exp_index][ch_ind][run][pk].right;
				cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][ch_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (cut->GetChannel()==MPPC_channels[ch_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(cut_data, run)) //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
					if (cut->GetChannel()==MPPC_channels[ch_ind] && !cut->GetAffectingHistogram() && !failed_phys_cut)
						if (kFALSE == (*cut)(cut_data, run))
							failed_phys_cut = true;
					if (failed_hist_cut && failed_phys_cut)
						break;
				}
				S2_no_cut += cut_data[0];
				S2_hist_cut += (failed_hist_cut ? 0 : cut_data[0]);
				S2_phys_cut += (failed_phys_cut ? 0 : cut_data[0]);
				S2_hist_and_phys_cut += (failed_hist_cut||failed_phys_cut ? 0 : cut_data[0]);
			}
			//1st parameter cut must be applied only if the rest of parameters are -2 === cut channel ==-1
			//cut_data[0] = S2;
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					cut_data[0] = S2_hist_and_phys_cut;
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist_phys = true;
					cut_data[0] = S2_hist_cut;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist = true;
					cut_data[0] = S2_phys_cut;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					cut_data[0] = S2_hist_cut;
					if (!failed_hist) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					cut_data[0] = S2_phys_cut;
					if (!failed_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					cut_data[0] = S2_hist_and_phys_cut;
					if (!failed_hist_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				cut_data[0] = S2_no_cut;
				(*operations[o].operation)(cut_data, run);
			}
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = data->pmt_peaks[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(5);
		auto entry = PMT_V.find(exp_str);
		double V = (entry == PMT_V.end() ? 0 : entry->second);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			double S2_no_cut = 0, S2_hist_cut = 0, S2_phys_cut = 0, S2_hist_and_phys_cut = 0;
			for (int pk = 0, pk_end = data->pmt_peaks[current_exp_index][ch_ind][run].size(); pk != pk_end; ++pk) {
				bool failed_hist_cut = false; //normal cuts
				bool failed_phys_cut = false; //drawn (displayed) cuts only
				cut_data[0] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].S;
				cut_data[1] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].A;
				cut_data[2] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].left;
				cut_data[3] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].right;
				cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->pmt_peaks[current_exp_index][ch_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (cut->GetChannel()==PMT_channels[ch_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(cut_data, run)) //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
					if (cut->GetChannel()==PMT_channels[ch_ind] && !cut->GetAffectingHistogram() && !failed_phys_cut)
						if (kFALSE == (*cut)(cut_data, run))
							failed_phys_cut = true;
					if (failed_hist_cut && failed_phys_cut)
						break;
				}
				S2_no_cut += cut_data[0];
				S2_hist_cut += (failed_hist_cut ? 0 : cut_data[0]);
				S2_phys_cut += (failed_phys_cut ? 0 : cut_data[0]);
				S2_hist_and_phys_cut += (failed_hist_cut||failed_phys_cut ? 0 : cut_data[0]);
			}
			cut_data[1] = calibr_info.get_S1pe(channel, V);
			S2_no_cut /= (cut_data[1]>0 ? cut_data[1] : 1.0);
			S2_hist_cut /= (cut_data[1]>0 ? cut_data[1] : 1.0);
			S2_phys_cut /= (cut_data[1]>0 ? cut_data[1] : 1.0);
			S2_hist_and_phys_cut /= (cut_data[1]>0 ? cut_data[1] : 1.0);
			//cut_data[0] = cut_data[1]>0 ? S2/cut_data[1] : S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					cut_data[0] = S2_hist_and_phys_cut;
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist_phys = true;
					cut_data[0] = S2_hist_cut;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_hist = true;
					cut_data[0] = S2_phys_cut;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(cut_data, run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					cut_data[0] = S2_hist_cut;
					if (!failed_hist) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					cut_data[0] = S2_phys_cut;
					if (!failed_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					cut_data[0] = S2_hist_and_phys_cut;
					if (!failed_hist_phys) (*operations[o].operation)(cut_data, run);
					continue;
				}
				cut_data[0] = S2_no_cut;
				(*operations[o].operation)(cut_data, run);
			}
		}
		break;
	}
	case Type::PMT_S2_int:
	{
		int run_size = data->PMT_S2_int[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		double S1pe;
		auto entry = PMT_V.find(exp_str);
		double V = (entry == PMT_V.end() ? 0 : entry->second);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			cut_data[0] = data->PMT_S2_int[current_exp_index][ch_ind][run];
			S1pe = calibr_info.get_S1pe(channel, V);
			cut_data[0] = S1pe>0 ? cut_data[0]/S1pe : cut_data[0];
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
		break;
	}
	case Type::PMT_times:
	case Type::PMT_times_N:
	case Type::PMT_t_S:
	case Type::PMT_A_S:
	case Type::PMT_Ss:
	{
		int run_size = data->pmt_peaks[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			for (int pk = 0, pk_end = data->pmt_peaks[current_exp_index][ch_ind][run].size(); pk != pk_end; ++pk) {
				cut_data[0] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].S;
				cut_data[1] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].A;
				cut_data[2] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].left;
				cut_data[3] = data->pmt_peaks[current_exp_index][ch_ind][run][pk].right;
				cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->pmt_peaks[current_exp_index][ch_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
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
	case Type::MPPC_sum_ts:
	{
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			bool failed_run_cut = false;
			bool failed_hist_cut = false; //normal cuts
			bool failed_phys_cut = false; //drawn (displayed) cuts only
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}
			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind!=_ch_ind_end_;++chan_ind) {
				for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][chan_ind][run].size(); pk != pk_end; ++pk) {
					cut_data[0] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].S;
					cut_data[1] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].A;
					cut_data[2] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].left;
					cut_data[3] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].right;
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][chan_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
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
		}
		break;
	}
	case Type::MPPC_coord_x:
	case Type::MPPC_coord_y:
	case Type::MPPC_coord:
	{
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		//[0] - data for no cuts, [1] - data for histogram cuts, [2] - for physical cuts and [3] - for both histogram and physical
		for (auto run = 0; run != run_size; ++run) {
			std::vector<std::vector<double> > Npes (4, std::vector<double>(MPPC_channels.size()+2, 0));
			std::vector<double> Npe_sum(4, 0), Npe_max(4, 0);
			std::vector<double> cut_data(5);
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}

			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind<_ch_ind_end_; ++chan_ind) {
				std::vector<double> S2(4, 0);
				for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					bool failed_phys_cut = false; //drawn (displayed) cuts only
					cut_data[0] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].S;
					cut_data[1] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].A;
					cut_data[2] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].left;
					cut_data[3] = data->mppc_peaks[current_exp_index][chan_ind][run][pk].right;
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->mppc_peaks[current_exp_index][chan_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==MPPC_channels[ch_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
						if (cut->GetChannel()==MPPC_channels[ch_ind] && !cut->GetAffectingHistogram() && !failed_phys_cut)
							if (kFALSE == (*cut)(cut_data, run))
								failed_phys_cut = true;
						if (failed_hist_cut && failed_phys_cut)
							break;
					}
					S2[0] += cut_data[0];
					S2[1] += (failed_hist_cut ? 0 : cut_data[0]);
					S2[2] += (failed_phys_cut ? 0 : cut_data[0]);
					S2[3] += (failed_hist_cut||failed_phys_cut ? 0 : cut_data[0]);
				}
				auto entry = MPPC_V.find(exp_str);
				double V = (entry == MPPC_V.end() ? 0 : entry->second);
				double s1pe = calibr_info.get_S1pe(MPPC_channels[chan_ind], V);
				for (std::size_t s = 0, s_end_ = S2.size(); s!=s_end_; ++s) {
					S2[s] = (s1pe > 0 ? S2[s]/s1pe : 0);
					Npes[s][chan_ind] = std::round(S2[s]);
					if (MPPC_coords.find(MPPC_channels[chan_ind])!=MPPC_coords.end()) {
						Npe_sum[s]+=Npes[s][chan_ind];
#ifdef WEIGHTED_COORD
						Npes[s][MPPC_channels.size()] += Npes[s][chan_ind]*MPPC_coords.find(MPPC_channels[chan_ind])->second.first;//x
						Npes[s][MPPC_channels.size()+1] += Npes[s][chan_ind]*MPPC_coords.find(MPPC_channels[chan_ind])->second.second;//y
#else
						if (Npe_max[s]<Npes[s][chan_ind]){
							Npe_max[s] = Npes[s][chan_ind];
							Npes[s][MPPC_channels.size()] = MPPC_coords.find(MPPC_channels[chan_ind])->second.first;//x
							Npes[s][MPPC_channels.size()+1] = MPPC_coords.find(MPPC_channels[chan_ind])->second.second;//y
						}
#endif //WEIGHTED_COORD
					}
				}
			}
#ifdef WEIGHTED_COORD
			for (std::size_t s = 0, s_end_ = S2.size(); s!=s_end_; ++s) {
				Npes[s][MPPC_channels.size()] /= Npe_sum[s];
				Npes[s][MPPC_channels.size()+1] /= Npe_sum[s];
			}
#endif
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(Npes[3], run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Npes[1], run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Npes[2], run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist) (*operations[o].operation)(Npes[1], run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(Npes[2], run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(Npes[3], run);
					continue;
				}
				(*operations[o].operation)(Npes[0], run);
			}
		}
		break;
	}
	case Type::MPPC_Npe_sum:
	case Type::PMT_Npe_sum:
	{
		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		double V = 0;
		if (is_PMT_type(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
			auto entry = PMT_V.find(exp_str);
			V = (entry == PMT_V.end() ? 0 : entry->second);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
			auto entry = MPPC_V.find(exp_str);
			double V = (entry == MPPC_V.end() ? 0 : entry->second);
		}
		int run_size = (*peaks)[0].size();
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			//[0] - data for no cuts, [1] - data for histogram cuts, [2] - for physical cuts and [3] - for both histogram and physical
			std::vector<std::vector<double> >  Npes(4, std::vector<double>(channels->size()+1, 0));
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}

			for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
				std::vector<double> S2(4, 0);
				for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					bool failed_phys_cut = false; //drawn (displayed) cuts only
					cut_data[0] = (*peaks)[chan_ind][run][pk].S;
					cut_data[1] = (*peaks)[chan_ind][run][pk].A;
					cut_data[2] = (*peaks)[chan_ind][run][pk].left;
					cut_data[3] = (*peaks)[chan_ind][run][pk].right;
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							(*peaks)[chan_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==(*channels)[ch_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
						if (cut->GetChannel()==(*channels)[ch_ind] && !cut->GetAffectingHistogram() && !failed_phys_cut)
							if (kFALSE == (*cut)(cut_data, run))
								failed_phys_cut = true;
						if (failed_hist_cut && failed_phys_cut)
							break;
					}
					S2[0] += cut_data[0];
					S2[1] += (failed_hist_cut ? 0 : cut_data[0]);
					S2[2] += (failed_phys_cut ? 0 : cut_data[0]);
					S2[3] += (failed_hist_cut||failed_phys_cut ? 0 : cut_data[0]);
				}
				double s1pe = calibr_info.get_S1pe((*channels)[chan_ind], V);
				for (std::size_t s = 0, s_end_ = S2.size(); s!=s_end_; ++s) {
					S2[s] = (s1pe > 0 ? S2[s]/s1pe : 0);
					Npes[s][chan_ind] = std::round(S2[s]);
				}
			}
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(Npes[3], run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Npes[1], run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Npes[2], run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist) (*operations[o].operation)(Npes[1], run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(Npes[2], run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(Npes[3], run);
					continue;
				}
				(*operations[o].operation)(Npes[0], run);
			}
		}
		break;
	}
	case Type::Correlation: //it has its own cuts.
	{
		int ch_x_ind = channel_to_index(_x_corr_ch, _x_corr);
		int ch_y_ind = channel_to_index(_y_corr_ch, _y_corr);
		std::size_t run_size = (is_PMT_type(_x_corr) ? data->pmt_peaks[current_exp_index][ch_x_ind].size() : data->mppc_peaks[current_exp_index][ch_x_ind].size());
		if (run_size !=(is_PMT_type(_y_corr) ? data->pmt_peaks[current_exp_index][ch_y_ind].size() : data->mppc_peaks[current_exp_index][ch_y_ind].size())) {
			std::cout<<"LoopThroughData Error: run size mismatch";
			break;
		}
		std::vector<double>* vals_x, *vals_y;
		std::vector<short> *cuts;
		vals_x = new std::vector<double>;
		vals_y = new std::vector<double>;
		cuts = new std::vector<short>;
		vals_x->resize(run_size);
		vals_y->resize(run_size);
		cuts->resize(run_size, 0);

		struct temp_data {
			std::vector<double> * vals;
			std::vector<short> * cuts;
			int ch_size;
		} stat_data_x, stat_data_y;
		stat_data_x.vals = vals_x;
		stat_data_x.cuts = cuts;
		stat_data_x.ch_size = MPPC_channels.size();
		stat_data_y.vals = vals_y;
		stat_data_y.cuts = cuts;
		stat_data_y.ch_size = MPPC_channels.size();
		FunctionWrapper* X_filler = new FunctionWrapper(&stat_data_x);
		FunctionWrapper* Y_filler = new FunctionWrapper(&stat_data_y);

		if (Type::MPPC_coord_y==_x_corr) {
			X_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
				std::vector<double> * vals = ((temp_data*)data)->vals;
				std::vector<short> * cuts = ((temp_data*)data)->cuts;
				(*cuts)[run] += 1;
				(*vals)[run] = pars[((temp_data*)data)->ch_size+1];
				return true;
			});
		} else {
			if ((Type::MPPC_coord_x==_x_corr)||(Type::MPPC_Npe_sum==_x_corr)) {
				X_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
					std::vector<double> * vals = ((temp_data*)data)->vals;
					std::vector<short> * cuts = ((temp_data*)data)->cuts;
					(*cuts)[run] += 1;
					(*vals)[run] = pars[((temp_data*)data)->ch_size];
					return true;
				});
			} else {
				X_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
					std::vector<double> * vals = ((temp_data*)data)->vals;
					std::vector<short> * cuts = ((temp_data*)data)->cuts;
					(*cuts)[run] += 1;
					(*vals)[run] = pars[0];
					return true;
				});
			}
		}
		if (Type::MPPC_coord_y==_y_corr) {
			Y_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
				std::vector<double> * vals = ((temp_data*)data)->vals;
				std::vector<short> * cuts = ((temp_data*)data)->cuts;
				(*cuts)[run] += 1;
				(*vals)[run] = pars[((temp_data*)data)->ch_size+1];
				return true;
			});
		} else {
			if ((Type::MPPC_coord_x==_y_corr)||(Type::MPPC_Npe_sum==_y_corr)) {
				Y_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
					std::vector<double> * vals = ((temp_data*)data)->vals;
					std::vector<short> * cuts = ((temp_data*)data)->cuts;
					(*cuts)[run] += 1;
					(*vals)[run] = pars[((temp_data*)data)->ch_size];
					return true;
				});
			} else {
				Y_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
					std::vector<double> * vals = ((temp_data*)data)->vals;
					std::vector<short> * cuts = ((temp_data*)data)->cuts;
					(*cuts)[run] += 1;
					(*vals)[run] = pars[0];
					return true;
				});
			}
		}

		Operation opx, opy;
		opx.operation = X_filler;
		opx.apply_run_cuts = true;
		opx.apply_hist_cuts = true;
		opx.apply_phys_cuts = false;
		opy.operation = Y_filler;
		opy.apply_run_cuts = true;
		opy.apply_hist_cuts = true;
		opy.apply_phys_cuts = false;
		std::vector<Operation> vec;
		vec.push_back(opx);
		LoopThroughData(vec, _x_corr_ch, _x_corr);
		vec.clear();
		vec.push_back(opy);
		LoopThroughData(vec, _y_corr_ch, _y_corr);

		std::vector<double> vals(2);
		for (std::size_t run = 0; run != run_size; ++run) {
			if (2==(*cuts)[run]) {
				vals[0] = (*vals_x)[run];
				vals[1] = (*vals_y)[run];
				bool failed_run_cut = false;
				bool failed_hist_cut = false; //normal cuts
				bool failed_phys_cut = false; //drawn (displayed) cuts only
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
					if (cut->GetAffectingHistogram() && !failed_hist_cut)
						if (kFALSE == (*cut)(vals, run)) //more expensive than GetAffectingHistogram
							failed_hist_cut = true;
					if (!cut->GetAffectingHistogram() && !failed_phys_cut)
						if (kFALSE == (*cut)(vals, run))
							failed_phys_cut = true;
					if (failed_hist_cut && failed_phys_cut)
						break;
				}
				for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
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
		delete cuts;
		delete vals_x;
		delete vals_y;
		break;
	}
	case Type::CorrelationAll: //it has its own cuts.
	{
		int ch_x_ind = channel_to_index(_x_corr_ch, _x_corr);
		int ch_y_ind = channel_to_index(_y_corr_ch, _y_corr);
		int actual_current_exp = current_exp_index;
		struct temp_data {
			std::vector<double> * vals;
			std::vector<short> * cuts;
			int ch_size;
		} stat_data_x, stat_data_y;
		std::vector<double> vals(2);
		for (int exp_ind=0,_end_exp = experiments.size(); exp_ind!=_end_exp; ++exp_ind) {
			std::size_t run_size = (is_PMT_type(_x_corr) ? data->pmt_peaks[exp_ind][ch_x_ind].size() : data->mppc_peaks[exp_ind][ch_x_ind].size());
			if (run_size !=(is_PMT_type(_y_corr) ? data->pmt_peaks[exp_ind][ch_y_ind].size() : data->mppc_peaks[exp_ind][ch_y_ind].size())) {
				std::cout<<"LoopThroughData Error: run size mismatch";
				break;
			}
			std::vector<double>* vals_x, *vals_y;
			std::vector<short> *cuts;
			vals_x = new std::vector<double>;
			vals_y = new std::vector<double>;
			cuts = new std::vector<short>;
			vals_x->resize(run_size);
			vals_y->resize(run_size);
			cuts->resize(run_size, 0);

			stat_data_x.vals = vals_x;
			stat_data_x.cuts = cuts;
			stat_data_x.ch_size = MPPC_channels.size();
			stat_data_y.vals = vals_y;
			stat_data_y.cuts = cuts;
			stat_data_y.ch_size = MPPC_channels.size();
			FunctionWrapper* X_filler = new FunctionWrapper(&stat_data_x);
			FunctionWrapper* Y_filler = new FunctionWrapper(&stat_data_y);

			if (Type::MPPC_coord_y==_x_corr) {
				X_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
					std::vector<double> * vals = ((temp_data*)data)->vals;
					std::vector<short> * cuts = ((temp_data*)data)->cuts;
					(*cuts)[run] += 1;
					(*vals)[run] = pars[((temp_data*)data)->ch_size+1];
					return true;
				});
			} else {
				if ((Type::MPPC_coord_x==_x_corr)||(Type::MPPC_Npe_sum==_x_corr)) {
					X_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
						std::vector<double> * vals = ((temp_data*)data)->vals;
						std::vector<short> * cuts = ((temp_data*)data)->cuts;
						(*cuts)[run] += 1;
						(*vals)[run] = pars[((temp_data*)data)->ch_size];
						return true;
					});
				} else {
					X_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
						std::vector<double> * vals = ((temp_data*)data)->vals;
						std::vector<short> * cuts = ((temp_data*)data)->cuts;
						(*cuts)[run] += 1;
						(*vals)[run] = pars[0];
						return true;
					});
				}
			}
			if (Type::MPPC_coord_y==_y_corr) {
				Y_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
					std::vector<double> * vals = ((temp_data*)data)->vals;
					std::vector<short> * cuts = ((temp_data*)data)->cuts;
					(*cuts)[run] += 1;
					(*vals)[run] = pars[((temp_data*)data)->ch_size+1];
					return true;
				});
			} else {
				if ((Type::MPPC_coord_x==_y_corr)||(Type::MPPC_Npe_sum==_y_corr)) {
					Y_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
						std::vector<double> * vals = ((temp_data*)data)->vals;
						std::vector<short> * cuts = ((temp_data*)data)->cuts;
						(*cuts)[run] += 1;
						(*vals)[run] = pars[((temp_data*)data)->ch_size];
						return true;
					});
				} else {
					Y_filler->SetFunction([](std::vector<double> &pars, int run, void* data){
						std::vector<double> * vals = ((temp_data*)data)->vals;
						std::vector<short> * cuts = ((temp_data*)data)->cuts;
						(*cuts)[run] += 1;
						(*vals)[run] = pars[0];
						return true;
					});
				}
			}

			current_exp_index = exp_ind; //TODO: add passing of exp_index to LoopThroughData instead of this trick.
			Operation opx, opy;
			opx.operation = X_filler;
			opx.apply_run_cuts = true;
			opx.apply_hist_cuts = true;
			opx.apply_phys_cuts = false;
			opy.operation = Y_filler;
			opy.apply_run_cuts = true;
			opy.apply_hist_cuts = true;
			opy.apply_phys_cuts = false;
			std::vector<Operation> vec;
			vec.push_back(opx);
			LoopThroughData(vec, _x_corr_ch, _x_corr);
			vec.clear();
			vec.push_back(opy);
			LoopThroughData(vec, _y_corr_ch, _y_corr);

			for (std::size_t run = 0; run != run_size; ++run) {
				if (2==(*cuts)[run]) {
					vals[0] = (*vals_x)[run];
					vals[1] = (*vals_y)[run];
					bool failed_run_cut = false;
					bool failed_hist_cut = false; //normal cuts
					bool failed_phys_cut = false; //drawn (displayed) cuts only
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(vals, run)) //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
						if (!cut->GetAffectingHistogram() && !failed_phys_cut)
							if (kFALSE == (*cut)(vals, run))
								failed_phys_cut = true;
						if (failed_hist_cut && failed_phys_cut)
							break;
					}
					for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
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
			delete cuts;
			delete vals_x;
			delete vals_y;
		}
		current_exp_index = actual_current_exp;
		break;
	}
	case Type::PMT_sum_N:
	{
		//channel->run->peak_itself:
		std::deque<std::deque<std::deque<peak> > > *peaks = NULL;
		std::deque<int> *channels = NULL;
		if (is_PMT_type(type)) {
			channels = &PMT_channels;
			peaks = &(data->pmt_peaks[current_exp_index]);
		} else {
			channels = &MPPC_channels;
			peaks = &(data->mppc_peaks[current_exp_index]);
		}
		int run_size = (*peaks)[0].size();
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			//[0] - data for no cuts, [1] - data for histogram cuts, [2] - for physical cuts and [3] - for both histogram and physical
			std::vector<std::vector<double> >  Ns(4, std::vector<double>(channels->size()+1, 0));
			bool failed_run_cut = false;
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); cut != c_end_; ++cut)
				if (kFALSE == cut->GetAccept(run)) {
					failed_run_cut = true;
					break;
				}

			for (int chan_ind=0,_ch_ind_end_= channels->size(); chan_ind<_ch_ind_end_;++chan_ind) {
				std::vector<std::size_t> N(4, 0);
				for (int pk = 0, pk_end = (*peaks)[chan_ind][run].size(); pk != pk_end; ++pk) {
					bool failed_hist_cut = false; //normal cuts
					bool failed_phys_cut = false; //drawn (displayed) cuts only
					cut_data[0] = (*peaks)[chan_ind][run][pk].S;
					cut_data[1] = (*peaks)[chan_ind][run][pk].A;
					cut_data[2] = (*peaks)[chan_ind][run][pk].left;
					cut_data[3] = (*peaks)[chan_ind][run][pk].right;
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							(*peaks)[chan_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut) {
						if (cut->GetChannel()==(*channels)[ch_ind] && cut->GetAffectingHistogram() && !failed_hist_cut)
							if (kFALSE == (*cut)(cut_data, run)) //more expensive than GetAffectingHistogram
								failed_hist_cut = true;
						if (cut->GetChannel()==(*channels)[ch_ind] && !cut->GetAffectingHistogram() && !failed_phys_cut)
							if (kFALSE == (*cut)(cut_data, run))
								failed_phys_cut = true;
						if (failed_hist_cut && failed_phys_cut)
							break;
					}
					N[0] += 1;
					N[1] += (failed_hist_cut ? 0 : 1);
					N[2] += (failed_phys_cut ? 0 : 1);
					N[3] += (failed_hist_cut||failed_phys_cut ? 0 : 1);
				}
				for (std::size_t s = 0, s_end_ = Ns.size(); s!=s_end_; ++s) {
					Ns[s][chan_ind] = N;
					Ns[s][PMT_channels.size()]+=Ns[s][chan_ind];
				}
			}
			bool failed_hist_phys = false, failed_hist = false, failed_phys = false;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (-1 == cut->GetChannel()) {
					if (!failed_hist_phys)
						if (kFALSE == (*cut)(Ns[3], run))
							failed_hist_phys = true;
					if (!failed_hist && cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Ns[1], run))
							failed_hist = true;
					if (!failed_phys && !cut->GetAffectingHistogram())
						if (kFALSE == (*cut)(Ns[2], run))
							failed_phys = true;
				}
			for (std::size_t o = 0, o_end_ = operations.size(); o!=o_end_; ++o) {
				if (operations[o].apply_run_cuts && failed_run_cut)
					continue;
				if (operations[o].apply_hist_cuts && !operations[o].apply_phys_cuts) {
					if (!failed_hist) (*operations[o].operation)(Ns[1], run);
					continue;
				}
				if (!operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_phys) (*operations[o].operation)(Ns[2], run);
					continue;
				}
				if (operations[o].apply_hist_cuts && operations[o].apply_phys_cuts) {
					if (!failed_hist_phys) (*operations[o].operation)(Ns[3], run);
					continue;
				}
				(*operations[o].operation)(Ns[0], run);
			}
		}
		break;
	}
	}
}

bool PostProcessor::Invalidate(unsigned int label)
{
	return CanvasSetups::Invalidate(label);
}

//Calls maximum of 2 LoopThroughData
bool PostProcessor::update(void)
{
	HistogramSetups * setups = get_hist_setups();
	bool set_default_setups = false;
	if (NULL == setups) {
		set_default_setups = true;
		setups = new HistogramSetups;
		setups->filled_hist = false;
		setups->fitted = false;
		//default_hist_setups(setups);
		if (false==set_hist_setups(setups, current_exp_index, current_channel, current_type)) {
			delete setups;
			return false;
		}
		delete setups;
		setups = get_hist_setups();
	}
	struct hist_fill_data_ {
		void* phist;
		int ch_size;
	} hist_fill_data;
	hist_fill_data.phist = is_TH1D_hist(current_type) ? get_current_hist1() : get_current_hist2();
	if (NULL == hist_fill_data.phist)
		std::cerr<<"PostProcessor::Validate: Error: NULL histogram pointer"<<std::endl;
	hist_fill_data.ch_size = is_PMT_type(current_type) ? PMT_channels.size() : MPPC_channels.size();
	FunctionWrapper histogram_filler(&hist_fill_data);

	std::size_t num_of_fills = 0;
	std::size_t num_of_drawn_fills = 0;
	FunctionWrapper fills_counter(&num_of_fills);
	fills_counter.SetFunction([](std::vector<double>& pars, int run, void* data) {
		++(*(std::size_t*)data);
		return true;
	});
	FunctionWrapper drawn_fills_counter(&num_of_drawn_fills);
	drawn_fills_counter.SetFunction([](std::vector<double>& pars, int run, void* data) {
		++(*(std::size_t*)data);
		return true;
	});

	struct limits_data_ {
		std::pair<double,double> x_mm;
		std::pair<double,double> y_mm;
		int ch_size;
	} limits, drawn_limits;
	limits.y_mm = limits.x_mm = std::pair<double,double>(DBL_MAX, -DBL_MAX);
	drawn_limits.y_mm = drawn_limits.x_mm = limits.y_mm;
	drawn_limits.ch_size = limits.ch_size = (is_PMT_type(current_type) ? PMT_channels.size() : MPPC_channels.size());
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
	mvar_drawn_data.ch_size = mvar_data.ch_size = (is_PMT_type(current_type) ? PMT_channels.size() : MPPC_channels.size());
	FunctionWrapper mean_taker(&mvar_data);
	FunctionWrapper variance_taker(&mvar_data);
	FunctionWrapper drawn_mean_taker(&mvar_drawn_data);
	FunctionWrapper drawn_variance_taker(&mvar_drawn_data);

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
	case Type::PMT_S2_int:
	case Type::PMT_Ss:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[0];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += (pars[0] - meanX) * (pars[0] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
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
	case Type::MPPC_sum_ts: {
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
	case Type::MPPC_Npe_sum:
	case Type::MPPC_coord_x:
	case Type::PMT_sum_N:
	case Type::PMT_Npe_sum:
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
	case Type::PMT_times:
	case Type::MPPC_times:
	{
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
			std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
			p->first = std::min(p->first, pars[4]);
			p->second = std::max(p->second, pars[4]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[4], pars[0]);
			return true;
		});
		break;

	}
	case Type::PMT_times_N:
	case Type::MPPC_times_N:
	{
		drawn_mean_taker.SetFunction(
		mean_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			(((mean_variance_data_*)data)->mean_x) += pars[4];
			return true;
		}));
		drawn_variance_taker.SetFunction(
		variance_taker.SetFunction([](std::vector<double>& pars, int run, void* data) {
			double meanX = ((mean_variance_data_*)data)->mean_x;
			(((mean_variance_data_*)data)->variance_x) += (pars[4] - meanX) * (pars[4] - meanX);
			return true;
		}));
		drawn_limits_finder.SetFunction(
		limits_finder.SetFunction([](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = &((limits_data_*)data)->x_mm;
			p->first = std::min(p->first, pars[4]);
			p->second = std::max(p->second, pars[4]);
			return true;
		}));
		histogram_filler.SetFunction([](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((hist_fill_data_*)data)->phist)->Fill(pars[4]);
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
		std::cerr << "PostProcessor::Validate: Warning! Not implemented type "<<std::endl;
	}
	}
	if (NULL == hist_fill_data.phist)
		histogram_filler.SetFunction(NULL);

	Operation op_fill_count(&fills_counter, true, true, false);
	Operation op_drawn_fill_count(&drawn_fills_counter, true, true, false);
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
	if (op_mean_taker.operation->isValid() && (!setups->x_mean || !setups->y_mean)) {
		setups->x_mean = (mvar_data.stat_weight == 0 ?
				((setups->num_of_fills <= 0 || setups->num_of_fills == boost::none) ? boost::none : mvar_data.mean_x / *setups->num_of_fills)
				: mvar_data.mean_x / mvar_data.stat_weight);
		setups->y_mean = (mvar_data.stat_weight == 0 ?
				((setups->num_of_fills <= 0 || setups->num_of_fills == boost::none) ? boost::none : mvar_data.mean_y / *setups->num_of_fills)
				: mvar_data.mean_y / mvar_data.stat_weight);
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
		setups->x_drawn_mean = (mvar_drawn_data.stat_weight <= 0 ?
				((setups->num_of_drawn_fills <= 0 || setups->num_of_drawn_fills == boost::none) ?
						boost::none : mvar_drawn_data.mean_x / *setups->num_of_drawn_fills)
				: mvar_drawn_data.mean_x / mvar_drawn_data.stat_weight);
		setups->y_drawn_mean = (mvar_drawn_data.stat_weight <= 0 ?
				((setups->num_of_drawn_fills <= 0 || setups->num_of_drawn_fills == boost::none) ?
						boost::none : mvar_drawn_data.mean_y / *setups->num_of_drawn_fills)
				: mvar_drawn_data.mean_y / mvar_drawn_data.stat_weight);
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
	//Now can set default setups using values calculated in the first LoopThroughData.
	if (set_default_setups) {
		default_hist_setups(setups);
		if (setups->use_fit)
			Invalidate(invFit | invFitFunction);
	}
	//Prepare canvas and histogram for plotting
	TCanvas * canvas = get_current_canvas();
	if (NULL==canvas) {
		std::cerr<<"PostProcessor::update: Error: NULL canvas"<<std::endl;
	} else {
		if (!setups->filled_hist) {
			canvas->cd();
			canvas->SetTitle(hist_name().c_str());
			canvas->Clear();
			std::pair<double, double> x_lims = hist_x_limits();
			x_lims.second+=(x_lims.second-x_lims.first)/setups->N_bins;
			std::pair<double, double> y_lims = hist_y_limits();
			if (is_TH1D_hist(current_type)) {
				TH1D* hist = get_current_hist1();
				if (NULL == hist) {
					TH1D new_hist1(hist_name().c_str(), hist_name().c_str(), setups->N_bins,
							(is_zoomed().first ? get_current_x_zoom().first :  x_lims.first),
							(is_zoomed().first ? get_current_x_zoom().second :  x_lims.second));
					set_hist1(&new_hist1);
				} else {
					hist->SetTitle(hist_name().c_str());
					hist->Reset("M");
					hist->SetBins(setups->N_bins,
						(is_zoomed().first ? get_current_x_zoom().first : x_lims.first),
						(is_zoomed().first ? get_current_x_zoom().second : x_lims.second));
				}
			} else {
				TH2D* hist = get_current_hist2();
				if (NULL == hist) {
					TH2D new_hist2(hist_name().c_str(), hist_name().c_str(), setups->N_bins,
						(is_zoomed().first ? get_current_x_zoom().first : x_lims.first),
						(is_zoomed().first ? get_current_x_zoom().second : x_lims.second), setups->N_bins,
						(is_zoomed().second ? get_current_y_zoom().first : y_lims.first),
						(is_zoomed().second ? get_current_y_zoom().second : y_lims.second));
					set_hist2(&new_hist2);
				} else {
					hist->SetTitle(hist_name().c_str());
					hist->Reset("M");
					hist->SetBins(setups->N_bins,
						(is_zoomed().first ? get_current_x_zoom().first : x_lims.first),
						(is_zoomed().first ? get_current_x_zoom().second : x_lims.second), setups->N_bins,
						(is_zoomed().second ? get_current_y_zoom().first : y_lims.first),
						(is_zoomed().second ? get_current_y_zoom().second : y_lims.second));
				}
			}
		}
	}
	hist_fill_data.phist = is_TH1D_hist(current_type) ? get_current_hist1() : get_current_hist2();
	if (NULL == hist_fill_data.phist) {
		std::cerr<<"PostProcessor::Validate: Error: NULL histogram pointer"<<std::endl;
		histogram_filler.SetFunction(NULL);
	}

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

	if (op_hist_fill.operation->isValid() && !setups->filled_hist)
		setups->filled_hist = true;
	if (op_variance_taker.operation->isValid() && (!setups->x_variance|| !setups->y_variance)) {
		setups->x_variance = (mvar_data.stat_weight <= 0 ?
				((setups->num_of_fills <= 1 || setups->num_of_fills == boost::none) ?
						boost::none : std::sqrt(mvar_data.variance_x / ((*setups->num_of_fills -1) * *setups->num_of_fills)))
				: mvar_data.variance_x / mvar_data.stat_weight);
		setups->y_variance = (mvar_data.stat_weight <= 0 ?
				((setups->num_of_fills <= 1 || setups->num_of_fills == boost::none) ?
						boost::none : std::sqrt(mvar_data.variance_y / ((*setups->num_of_fills -1) * *setups->num_of_fills)))
				: mvar_data.variance_y / mvar_data.stat_weight);
	}
	if (op_drawn_variance_taker.operation->isValid() && (!setups->x_drawn_variance || !setups->y_drawn_variance)) {
		setups->x_drawn_variance = (mvar_drawn_data.stat_weight <= 0 ?
				((setups->num_of_drawn_fills <= 1 || setups->num_of_drawn_fills == boost::none) ?
						boost::none : std::sqrt(mvar_drawn_data.variance_x / ((*setups->num_of_drawn_fills -1) * *setups->num_of_drawn_fills)))
				: mvar_drawn_data.variance_x / mvar_drawn_data.stat_weight);
		setups->y_drawn_variance = (mvar_drawn_data.stat_weight <= 0 ?
				((setups->num_of_drawn_fills <= 1 || setups->num_of_drawn_fills == boost::none) ?
						boost::none : std::sqrt(mvar_drawn_data.variance_y / ((*setups->num_of_drawn_fills -1) * *setups->num_of_drawn_fills)))
				: mvar_drawn_data.variance_y / mvar_drawn_data.stat_weight);
	}
	if (!setups->x_max || !setups->y_max) {
		if (is_TH1D_hist(current_type)) {
			TH1D *hist = get_current_hist1();
			if (NULL == hist) {
				std::cerr<<"PostProcessor::Validate: Error: NULL 1D histogram"<<std::endl;
			} else {
				Int_t bin = hist->GetMaximumBin();
				setups->x_max = hist->GetBinCenter(bin);
				setups->y_max = hist->GetBinContent(bin);
			}
		} else {
			TH2D *hist = get_current_hist2();
			if (NULL == hist) {
				std::cerr<<"PostProcessor::Validate: Error: NULL 2D histogram"<<std::endl;
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
	if (set_default_setups) {
		default_hist_setups(setups);
		if (setups->use_fit)
			Invalidate(invFit | invFitFunction);
	}

	if (is_TH1D_hist(current_type)) {
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
				for (int par = 0; par < setups->par_val.size(); ++par)
					setups->par_val[par] = ff->GetParameter(par);
			}
			setups->fitted = true;
		}
	}

	if (NULL!=canvas) {
		if (is_TH1D_hist(current_type)) {
			TH1D* hist = get_current_hist1();
			if (hist)
				hist->Draw("hist");
		} else {
			TH2D* hist = get_current_hist2();
			if (hist)
				hist->Draw("colz"/*"lego"*/);
		}
		canvas->Update(); //required for updates axes which are used in drawing cuts
		TF1* ff = get_current_fit_function();
		if (ff && setups->fitted)
			ff->Draw("same");
		for (auto cut = setups->hist_cuts.begin(), cut_end_ = setups->hist_cuts.end(); cut!=cut_end_; ++cut) {
			if (!cut->GetAffectingHistogram()) //no point in drawing already applied to histogram cuts
				cut->Draw(canvas);
		}
		canvas->Update();
	}

	update_physical();
	update_Npe();
	return true;
}

std::size_t PostProcessor::numOfFills(bool consider_displayed_cuts)
{
	std::size_t ret = 0;
	HistogramSetups* setups = get_hist_setups(current_exp_index, current_channel, current_type);
	if (NULL == setups) {
		std::cerr<<"PostProcessor::numOfFills: Error: NULL histogram setups"<<std::endl;
		return ret;
	}
	if (consider_displayed_cuts) {
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

//Run cuts are applied!
std::size_t PostProcessor::numOfRuns (void)
{
	std::size_t run_n = 0;
	int ch_ind = channel_to_index(current_channel, current_type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *run_cuts = (NULL== get_run_cuts(current_exp_index) ? &empty : get_run_cuts(current_exp_index));
	std::size_t run_size = is_PMT_type(current_type) ? data->pmt_peaks[current_exp_index][ch_ind].size() : data->mppc_peaks[current_exp_index][ch_ind].size();
	for (std::size_t run = 0; run != run_size; ++run){
		for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
			if (kFALSE == cut->GetAccept(run))//not calculating it here!
				goto _cutted;
		++run_n;
	_cutted:;
	}
	return run_n;
}

std::pair<double, double> PostProcessor::hist_y_limits(bool consider_displayed_cuts) //considering cuts
{
	std::pair<double, double> ret(DBL_MAX, -DBL_MAX);
	HistogramSetups* setups = get_hist_setups(current_exp_index, current_channel, current_type);
	if (NULL == setups) {
		std::cerr<<"PostProcessor::hist_x_limits: Error: NULL histogram setups"<<std::endl;
		return ret;
	}
	if (consider_displayed_cuts) {
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

std::pair<double, double> PostProcessor::hist_x_limits(bool consider_displayed_cuts) //valid only for 2d plots
{
	std::pair<double, double> ret(DBL_MAX, -DBL_MAX);
	HistogramSetups* setups = get_hist_setups(current_exp_index, current_channel, current_type);
	if (NULL == setups) {
		std::cerr<<"PostProcessor::hist_x_limits: Error: NULL histogram setups"<<std::endl;
		return ret;
	}
	if (consider_displayed_cuts) {
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

void PostProcessor::default_hist_setups(HistogramSetups* setups)//does not affect cuts
{
	if (NULL==setups) {
		std::cerr<<"PostProcessor::default_hist_setups: Error: NULL HistogramSetups*"<<std::endl;
		return;
	}
	int _N_ = numOfFills(false);
	setups->N_bins = _N_;
	setups->N_bins = std::max(4,(int)std::round(std::sqrt(setups->N_bins)));
	std::pair<double, double> x_lims = hist_x_limits();
	x_lims.second+=(x_lims.second-x_lims.first)/setups->N_bins;

	setups->use_fit = false;
	setups->fitted = false;
	setups->is_valid_fit_function = false;
	setups->filled_hist = false;
	switch (current_type)
	{
	case Type::MPPC_tboth:
	{
		setups->N_gauss = 2;
		setups->par_val.resize(6, 0);
		setups->par_left_limits.resize(6, 0);
		setups->par_right_limits.resize(6, 0);

		setups->par_left_limits[0] = 0;
		setups->par_right_limits[0] = std::max(1, 2 * (int)std::sqrt(_N_));
		setups->par_val[0] = (int)std::sqrt(_N_);

		setups->par_left_limits[1] = x_lims.first;
		setups->par_right_limits[1] = x_lims.second;
		setups->par_val[1] = 0.2*(setups->par_right_limits[1] + setups->par_left_limits[1]);

		setups->par_left_limits[2] = (x_lims.second - x_lims.first) / setups->N_bins;
		setups->par_right_limits[2] = (x_lims.second - x_lims.first);
		setups->par_val[2] = 0.2*(setups->par_left_limits[2] + setups->par_right_limits[2]);

		setups->par_left_limits[3] = 0;
		setups->par_right_limits[3] = std::max(1, (int)(2*std::sqrt(_N_)));
		setups->par_val[3] = (int)std::sqrt(_N_);

		setups->par_left_limits[4] = x_lims.first;
		setups->par_right_limits[4] = x_lims.second;
		setups->par_val[4] = 0.8*(setups->par_right_limits[4] + setups->par_left_limits[4]);

		setups->par_left_limits[5] = (x_lims.second - x_lims.first) / setups->N_bins;
		setups->par_right_limits[5] = (x_lims.second - x_lims.first);
		setups->par_val[5] = 0.2*(setups->par_left_limits[5] + setups->par_right_limits[5]);
		break;
	}
	default:
	{
		setups->N_gauss = 0;
		setups->par_val.resize(0, 0);
		setups->par_left_limits.resize(0, 0);
		setups->par_right_limits.resize(0, 0);
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

	std::ofstream str;
	open_output_file(data_output_path+DATA_MPPC_VERSION+"/S2_manual.dat", str);
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

	open_output_file(data_output_path+DATA_MPPC_VERSION+"/double_I.dat", str);
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

	open_output_file(data_output_path+DATA_MPPC_VERSION+"/Npe_direct.dat", str);
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

	open_output_file(data_output_path+DATA_MPPC_VERSION+"/Npe_result.dat", str);
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

void PostProcessor::saveAs(std::string path)
{
	print_hist(path);
}

void PostProcessor::clear(void)	//clear cuts for current histogram. Run cuts derived from it are not touched
{
	HistogramSetups def_setups;
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
	for (std::size_t c = 0, c_end_=canvases.size(); c!=c_end_ ; ++c) {
		canvases[c]->Clear();
		canvases[c]->Update();
	}
	update();
}

void PostProcessor::plot_N_pe(int channel, GraphicOutputManager* gr_man)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	int ch_ind = data->get_ch_index(channel);
	if (ch_ind < 0)
		return;
	std::vector<double> N_pe_result;
	std::vector<double> N_pe_Double_I_result;
	std::vector<double> N_pe_direct_result;
	for (std::size_t pt = 0; pt < data->exp_area.experiments.size(); ++pt){
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
	dr->AddToDraw(data->Fields, N_pe_Double_I_result, "MPPC#" + std::to_string(channel) + "N pe double I", "with line lc rgb '#000000'");
	dr->DrawData();
}

//TODO: actually it is more logical to move the code below to CalibrationInfo, but then I'll need to add friends
void PostProcessor::update_Npe(void)
{
	for (int exp = 0, exp_end_ = data->N_pe_direct.size(); exp != exp_end_; ++exp) {
		for (int ch = 0, ch_end_ = MPPC_channels.size(); ch != ch_end_; ++ch) {
			data->N_pe_direct[exp][ch] = avr_S2_S[exp][ch];
			data->N_pe_Double_I[exp][ch] = avr_Double_I[exp][ch];
		}
	}
	//calibr_info.recalibrate(data->N_pe_direct, data->N_pe_Double_I, data->Fields);
	for (int exp = 0, exp_end_ = data->N_pe_direct.size(); exp != exp_end_; ++exp) {
		for (int ch = 0, ch_end_ = MPPC_channels.size(); ch != ch_end_; ++ch) {
			if (exp > calibr_info.get_N_calib(MPPC_channels[ch]).first)
				data->N_pe_result[exp][ch] = data->N_pe_Double_I[exp][ch];
			else
				data->N_pe_result[exp][ch] = data->N_pe_direct[exp][ch];
		}
		auto entry = PMT_V.find(experiments[exp]);
		double V = (entry == PMT_V.end() ? 0 : entry->second);
		if (!data->N_pe_PMT3.empty())
			if (calibr_info.get_S1pe(0, V)>0)
				data->N_pe_PMT3[exp] = PMT3_avr_S2_S[exp] / calibr_info.get_S1pe(0, V);
		if (!data->N_pe_PMT1.empty())
			if (calibr_info.get_S1pe(1, V)>0)
				data->N_pe_PMT1[exp] = PMT1_avr_S2_S[exp] / calibr_info.get_S1pe(1, V);
	}
}

//updates derived values such as S2 amplitude, S of single photoelectron etc.
void PostProcessor::update_physical(void)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::update_physical: Error: NULL setups"<<std::endl;
	}
	struct temp_data {
		double val;
		int weight;
	} stat_data;
	stat_data.val = 0;
	stat_data.weight = 0;
	FunctionWrapper* mean_taker = new FunctionWrapper(&stat_data);
	mean_taker->SetFunction([](std::vector<double>& pars, int run, void* data) { //same for all types
		++(((temp_data*)data)->weight);
		(((temp_data*)data)->val)+=pars[0];
		return true;
	});
	Operation op;
	op.operation = mean_taker;
	op.apply_run_cuts = true;
	op.apply_hist_cuts = true;
	op.apply_phys_cuts = true;
	std::vector<Operation> operations(1, op);
	std::string exp_str = experiments[current_exp_index];
	switch (current_type){
	case Type::MPPC_Double_I:
	{
		if (setups->N_gauss>0 && setups->fitted)
			avr_Double_I[current_exp_index][mppc_channel_to_index(current_channel)] = setups->par_val[1];
		else {
			if (0==setups->N_gauss) {
				LoopThroughData(operations, current_channel, current_type);
				if (0==stat_data.weight)
					std::cout << "Warning! No mean double integral value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else {
					std::cout << "Mean = "<<stat_data.val/stat_data.weight<<std::endl;
					avr_Double_I[current_exp_index][mppc_channel_to_index(current_channel)] = stat_data.val/stat_data.weight;
				}
			} else
				std::cout << "Warning! No double integral for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	case Type::MPPC_S2:	//TODO: Warning!: these two types (MPPC_S2 and MPPC_S2_S) overwrite each other
	{
		if (setups->N_gauss>0 && setups->fitted)
			avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = setups->par_val[1];
		else {
			if (0==setups->N_gauss) { //Use mean then
				LoopThroughData(operations, current_channel, current_type);
				if (0==stat_data.weight)
					std::cout << "Warning! No mean S2 value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else {
					std::cout<<"Mean = "<<stat_data.val/stat_data.weight<<std::endl;
					avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = stat_data.val/stat_data.weight;
				}
			} else
				std::cout << "Warning! No S2 area for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		if (setups->N_gauss>0 && setups->fitted)
			avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = setups->par_val[1];
		else {
			if (0==setups->N_gauss) { //Use mean then
				LoopThroughData(operations, current_channel, current_type);
				if (0==stat_data.weight)
					std::cout << "Warning! No mean S2 value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else {
					std::cout << "Mean = "<<stat_data.val/stat_data.weight << std::endl;
					avr_S2_S[current_exp_index][mppc_channel_to_index(current_channel)] = stat_data.val/stat_data.weight;
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
		LoopThroughData(operations, current_channel, current_type);
		if (0==stat_data.weight)
			std::cout << "Warning! No mean Ss value for current histogram: " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		else
			std::cout << "Current mean value = "<<stat_data.val / (double)stat_data.weight << std::endl;
		if (meth == CalibrationInfo::Ignore) {
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = "<<calibr_info.get_S1pe(current_channel, V) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::UsingMean) {
			if (0 != stat_data.weight)
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, stat_data.val / (double)stat_data.weight, stat_data.weight);
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = "<<calibr_info.get_S1pe(current_channel, V) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::Using1pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>0 && setups->fitted)
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, setups->par_val[1], 1/*TODO: add calculation of N under fit*/);
			else {
				std::cout << "Warning! No calibration S1pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		if (meth == CalibrationInfo::Using2pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>=2 && setups->fitted)
				calibr_info.set_S2pe_exp(current_channel, current_exp_index, setups->par_val[4], 1/*TODO: add calculation of N under fit*/);
			else {
				std::cout << "Warning! No calibration S2pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		calibr_info.calculateS1pe(current_channel, V);
		std::cout << "Resulting calibration S1pe = "<<calibr_info.get_S1pe(current_channel, V) << std::endl;
		break;
	}
	case Type::PMT_Ss:
	{
		CalibrationInfo::S1pe_method meth = calibr_info.get_method(current_channel, current_exp_index);
		auto entry = PMT_V.find(exp_str);
		double V = (entry == PMT_V.end() ? 0 : entry->second);
		LoopThroughData(operations, current_channel, current_type);
		if (0==stat_data.weight)
			std::cout << "Warning! No mean calibration Ss value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		else
			std::cout << "S1pe(mean) = "<<stat_data.val / (double)stat_data.weight << std::endl;
		if (meth == CalibrationInfo::Ignore) {
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = " << calibr_info.get_S1pe(current_channel, V) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::UsingMean) {
			if (0 != stat_data.weight)
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, stat_data.val / (double)stat_data.weight, stat_data.weight);
			calibr_info.calculateS1pe(current_channel, V);
			std::cout << "Resulting calibration S1pe = " << calibr_info.get_S1pe(current_channel, V) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::Using1pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>0 && setups->fitted)
				calibr_info.set_S1pe_exp(current_channel, current_exp_index, setups->par_val[1], 1/*TODO: add calculation of N under fit*/);
			else {
				std::cout << "Warning! No calibration S1pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		if (meth == CalibrationInfo::Using2pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss >= 2 && setups->fitted)
				calibr_info.set_S2pe_exp(current_channel, current_exp_index, setups->par_val[4], 1/*TODO: add calculation of N under fit*/);
			else {
				std::cout << "Warning! No calibration S2pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		calibr_info.calculateS1pe(current_channel, V);
		std::cout << "Resulting calibration S1pe = " << calibr_info.get_S1pe(current_channel, V) << std::endl;
		break;
	}
	case Type::PMT_S2_S:
	{
		if (setups->N_gauss > 0 && setups->fitted) {
			if (0 == current_channel)
				PMT3_avr_S2_S[current_exp_index] = setups->par_val[1];
			if (1 == current_channel)
				PMT1_avr_S2_S[current_exp_index] = setups->par_val[1];
		}
		else {
			if (0==setups->N_gauss) { //Use mean then
				LoopThroughData(operations, current_channel, current_type);
				if (0==stat_data.weight)
					std::cout << "Warning! No S2 area for PMT " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else {
					std::cout<<"Mean = "<<stat_data.val/stat_data.weight<<std::endl;
					if (0 == current_channel)
						PMT3_avr_S2_S[current_exp_index] = stat_data.val/stat_data.weight;
					if (1 == current_channel)
						PMT1_avr_S2_S[current_exp_index] = stat_data.val/stat_data.weight;
				}
			}
		}
		break;
	}
	case Type::PMT_S2_int:
	{
		if (setups->N_gauss > 0 && setups->fitted){
			if (0 == current_channel)
				PMT3_avr_S2_S[current_exp_index] = setups->par_val[1];
			if (1 == current_channel)
				PMT1_avr_S2_S[current_exp_index] = setups->par_val[1];
		}
		else {
			if (0==setups->N_gauss) { //Use mean then
				LoopThroughData(operations, current_channel, current_type);
				if (0==stat_data.weight)
					std::cout << "Warning! No S2 area for PMT " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
				else {
					std::cout<<"Mean = "<<stat_data.val/stat_data.weight<<std::endl;
					if (0 == current_channel)
						PMT3_avr_S2_S[current_exp_index] = stat_data.val/stat_data.weight;
					if (1 == current_channel)
						PMT1_avr_S2_S[current_exp_index] = stat_data.val/stat_data.weight;
				}
			}
		}
		break;
	}
	case Type::MPPC_times:
	case Type::MPPC_t_S:
	case Type::PMT_t_S:
	case Type::PMT_times:
	{
		LoopThroughData(operations, current_channel, current_type); //TODO: A LOT of loops here. Rework the code to avoid this (store more info/call all relevant loops in one place)
		if (0==stat_data.weight)
			std::cout << "Warning! No peaks selected ch " << current_channel << std::endl;
		else {
			std::pair<double, double> drawn_limits = hist_x_limits(true);
			std::cout <<"N runs = "<< numOfRuns() << " S  = "<<stat_data.val<< " Npeaks = "<<stat_data.weight<<" dT = "
					<<drawn_limits.second - drawn_limits.first<<std::endl;
		}
		break;
	}
	case Type::PMT_times_N:
	case Type::MPPC_times_N:
	{
		LoopThroughData(operations, current_channel, current_type);
		if (0==stat_data.weight)
			std::cout << "Warning! No peaks selected ch " << current_channel << std::endl;
		else {
			std::pair<double, double> drawn_limits = hist_x_limits(true);
			std::cout <<"N runs = "<< numOfRuns() << " Npeaks = "<<stat_data.weight<<" dT = "
					<<drawn_limits.second - drawn_limits.first<<std::endl;
		}
		break;
	}
	default:
		break;
	}
	delete mean_taker;
}

void PostProcessor::add_hist_cut(FunctionWrapper* picker, std::string name, bool affect_hist)
{
	if (isMultichannel(current_type)){
		std::cout << "PostProcessor::add_hist_cut(FunctionWrapper*, std::string) Error: for type '"<<type_name(current_type)<<"' channel must be specified."<<std::endl;
		std::cout << "\t Use add_hist_cut(FunctionWrapper*, std::string, int channel) instead."<<std::endl;
		return;
	}
	add_hist_cut(picker, name, isComposite(current_type) ? -1 : current_channel, affect_hist);
}

void PostProcessor::add_hist_cut(FunctionWrapper* picker, std::string name, int channel, bool affect_hist)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::add_hist_cut: Error: NULL setups"<<std::endl;
	}
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (!isComposite(current_type)&&!isMultichannel(current_type)&&(channel!=current_channel || -1==channel)) {
		std::cout<<"PostProcessor::add_hist_cut(FunctionWrapper*, std::string, int, bool) Warning: No such channel for type '"<<type_name(current_type)<<"'. Channel set to "<<current_channel<<std::endl;
		channel = current_channel;
	}
	if (-1!=channel) {
		if (is_PMT_type(current_type)) {
			int ch_ind = pmt_channel_to_index(channel);
			if (ch_ind<0) {
				std::cout<<"PostProcessor::add_hist_cut(FunctionWrapper*, std::string, int, bool) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
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
				std::cout<<"PostProcessor::add_hist_cut(FunctionWrapper*, std::string, int) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
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
	if (NULL==found_cut) {
		setups->hist_cuts.push_back(EventCut(0, EventCut::HistCut, name));
		found_cut = &(setups->hist_cuts.back());
	}
	found_cut->SetPicker(picker);
	found_cut->SetChannel(channel);
	int top_level_channel = (isComposite(current_type) || isMultichannel(current_type)) ? -1 : current_channel;
	if ((channel!=top_level_channel)&&!affect_hist) {
		std::cout<<"PostProcessor::add_hist_cut(FunctionWrapper*, std::string, int) Warning: Cut for such channel and type '"<<type_name(current_type)<<"' can't be drawn."<<std::endl;
	}
	affect_hist = affect_hist || channel!=top_level_channel;
	found_cut->SetAffectingHistogram(affect_hist);
	update();
}

void PostProcessor::remove_hist_cut(int index)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::remove_hist_cut: Error: NULL setups"<<std::endl;
	}
	if (index<0 || index >= setups->hist_cuts.size()) {
		std::cout<<"PostProcessor::remove_hist_cut(int) Error: cut index is out of range"<<std::endl;
		return;
	}
	setups->hist_cuts.erase(setups->hist_cuts.begin() + index);
	update();
}

void PostProcessor::remove_hist_cut(std::string name)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::remove_hist_cut: Error: NULL setups"<<std::endl;
	}
	while (true) {
		for (auto i = setups->hist_cuts.begin(); i != setups->hist_cuts.end(); ++i) {
			if (i->GetName() == name){
				setups->hist_cuts.erase(i);
				goto anew;
			}
		}
		update();
		break;
		anew:;
	}
}

int PostProcessor::list_hist_cuts(void)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL == setups) {
		std::cout << "PostProcessor::list_hist_cuts: Error: NULL setups" << std::endl;
		return 0;
	}
	std::cout << "\tHistogram cuts [" << setups->hist_cuts.size() << "]: \"name\":channel# | "<<std::endl;
	for (auto i = setups->hist_cuts.begin(), _end_ = setups->hist_cuts.end(); i != _end_; ++i)
		std::cout <<"\""<< i->GetName()<<"\"" << (i->GetAffectingHistogram() ? ":" : "(Only shown):")<<i->GetChannel() << (((i == (_end_ - 1)) ? "" : " | "));
	std::cout << std::endl;
	return setups->hist_cuts.size();
}

int PostProcessor::list_run_cuts(void)
{
	HistogramSetups *setups = get_hist_setups();
	if (NULL == setups) {
		std::cout << "PostProcessor::list_run_cuts: Error: NULL setups" << std::endl;
		return 0;
	}
	std::deque<EventCut> *RunCuts = get_run_cuts(current_exp_index);
	if (NULL == RunCuts) {
		std::cout << "PostProcessor::list_run_cuts: Error: NULL RunCuts" << std::endl;
		return 0;
	}
	std::cout << "RunCuts [" << RunCuts->size() << "]: ";
	for (auto i = RunCuts->begin(), _end_ = RunCuts->end(); i != _end_; ++i)
		std::cout << (i->GetName()) << ((i == (_end_ - 1)) ? "" : " | ");
	std::cout << std::endl;
	return RunCuts->size();
}

void PostProcessor::remove_hist_cut(std::string name, int ch)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (!isComposite(current_type)&&!isMultichannel(current_type)&&(ch!=current_channel || -1==ch)) {
		std::cout<<"PostProcessor::remove_hist_cut(FunctionWrapper*, std::string, int, bool) Warning: No such channel for type '"<<type_name(current_type)<<"'. Channel set to "<<current_channel<<std::endl;
		ch = current_channel;
	}
	if (-1!=ch) {
		if (is_PMT_type(current_type)) {
			int ch_ind = pmt_channel_to_index(ch);
			if (ch_ind<0) {
				std::cout<<"PostProcessor::remove_hist_cut(FunctionWrapper*, std::string, int, bool) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
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
				std::cout<<"PostProcessor::add_hist_cut(FunctionWrapper*, std::string, int) Error: No such channel for type '"<<type_name(current_type)<<"'. No cut set."<<std::endl;
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
		std::cout<<"PostProcessor::remove_hist_cut: Error: NULL setups"<<std::endl;
	}
	while (true) {
		for (auto i = setups->hist_cuts.begin(); i != setups->hist_cuts.end(); ++i) {
			if ((i->GetName() == name)&&(i->GetChannel()==ch)){
				setups->hist_cuts.erase(i);
				goto anew;
			}
		}
		update();
		break;
		anew:;
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
	int run_size = is_PMT_type(current_type) ? 
		data->pmt_peaks[current_exp_index][0].size()
		:data->mppc_peaks[current_exp_index][0].size();
	std::deque<EventCut> *RunCuts = get_run_cuts(current_exp_index);
	if (NULL==RunCuts) {
		return;
	}
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
	std::string exp_str = experiments[current_exp_index];
	LoopThroughData(operations, current_channel, current_type);
	update();
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
		update();
		break;
		anew:;
	}
}

void PostProcessor::do_fit(bool do_fit)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (!is_TH1D_hist(current_type)) {
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
	update();
}

void PostProcessor::set_N_bins(int N)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::set_N_bins: Error: NULL setups"<<std::endl;
	}
	setups->N_bins = std::max(N, 1);
	Invalidate(invHistogram);
	update();
}

void PostProcessor::set_zoom (double xl, double xr)
{
	std::pair<double, double> x_zoom, y_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	x_zoom.first = std::min(xl, xr);
	x_zoom.second = std::max(xl, xr);
	CanvasSetups::set_zoom(x_zoom, y_zoom);
	update();
}

void PostProcessor::set_zoom_y (double yl, double yr)
{
	if (is_TH1D_hist(current_type)){
		std::cout<<"can't set y zoom for TH1D histogram"<<std::endl;
		return;
	}
	std::pair<double, double> y_zoom, x_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	y_zoom.first = std::min(yl, yr);
	y_zoom.second = std::max(yl, yr);
	CanvasSetups::set_zoom(x_zoom, y_zoom);
	update();
}

void PostProcessor::set_zoom (double xl, double xr, double yl, double yr)
{
	std::pair<double, double> y_zoom, x_zoom;
	x_zoom.first = std::min(xl, xr);
	x_zoom.second = std::max(xl, xr);
	y_zoom.first = std::min(yl, yr);
	y_zoom.second = std::max(yl, yr);
	CanvasSetups::set_zoom(x_zoom, y_zoom);
	update();
}

void PostProcessor::unset_zoom(bool do_update)
{
	CanvasSetups::unset_zoom();
	if (do_update)
		update();
}

void PostProcessor::set_fit_gauss(int N)
{
	if (!isValid()) {
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::set_fit_gauss: Error: NULL setups"<<std::endl;
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

void PostProcessor::set_parameter_val(int index, double val)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::set_parameter_val: Error: NULL setups"<<std::endl;
	}
	if ((index < 0) || (index >= 3 * setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	setups->par_val[index] = val;
	Invalidate(invFit|invFitFunction);
	update();
}

void PostProcessor::set_parameter_limits(int index, double left, double right)
{
	if (!isValid()){
		std::cout << "Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	HistogramSetups *setups = get_hist_setups();
	if (NULL==setups) {
		std::cout<<"PostProcessor::set_parameter_limits: Error: NULL setups"<<std::endl;
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
