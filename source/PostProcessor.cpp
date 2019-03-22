#include "PostProcessor.h"

PostProcessor::PostProcessor(AllExperimentsResults* _data) : 
CanvasSetups(_data->mppc_channels,_data->pmt_channels, _data->exp_area.experiments), calibr_info(this)
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

	update(All);
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
	print_hist(current_channel, current_exp_index, current_type, path);
}

void PostProcessor::print_hist(int ch, int exp_ind, Type type, std::string path)
{
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
	std::size_t real_size = numOfFills(ch, type);
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
	LoopThroughData(writer_to_file, ch, type, false, true);
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
	update(All);
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
void PostProcessor::LoopThroughData(FunctionWrapper* operation, int channel, Type type, bool apply_phys_cuts, bool apply_run_cuts, bool apply_hist_cuts)
{
	int ch_ind = channel_to_index(channel, type);
	HistogramSetups* setups = get_hist_setups(current_exp_index, channel, type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *hist_cuts = (NULL==setups ? &empty : &(setups->hist_cuts));
	std::deque<EventCut> *run_cuts = (NULL== get_run_cuts(current_exp_index) ? &empty : get_run_cuts(current_exp_index));
	switch (type)
	{
	case Type::MPPC_Double_I:
	{
		int run_size = data->Double_I[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))
					goto _0cutted;
			cut_data[0] = data->Double_I[current_exp_index][ch_ind][run];
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
					if (kFALSE == (*cut)(cut_data, run))
						goto _0cutted;
			(*operation)(cut_data, run);
			_0cutted:;
		}
		break;
	}
	case Type::MPPC_S2_S:
	{
		int run_size = data->S2_S[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))
					goto _1cutted;
			cut_data[0] = data->S2_S[current_exp_index][ch_ind][run];
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
					if (kFALSE == (*cut)(cut_data, run))
						goto _1cutted;
			(*operation)(cut_data, run);
		_1cutted:;
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
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))
					goto _2cutted;
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][ch_ind][run].size();
				pk != pk_end; ++pk) {
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
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
						if (kFALSE == (*cut)(cut_data, run))
							goto _2cutted1;
				(*operation)(cut_data, run);
			_2cutted1:;
			}
		_2cutted:;
		}
		break;
	}
	case Type::MPPC_tstart:
	case Type::MPPC_tboth:
	{
		int run_size = data->S2_start_t[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))
					goto _3cutted;
			cut_data[0] = data->S2_start_t[current_exp_index][ch_ind][run];
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
					if (kFALSE == (*cut)(cut_data, run))
						goto _3cutted;
			(*operation)(cut_data, run);
		_3cutted:;
		}
		if (type == Type::MPPC_tstart)
			break;
	}
	/* no break */
	case Type::MPPC_tfinal:
	{
		int run_size = data->S2_finish_t[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))
					goto _4cutted;
			cut_data[0] = data->S2_finish_t[current_exp_index][ch_ind][run];
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
					if (kFALSE == (*cut)(cut_data, run))
						goto _4cutted;
			(*operation)(cut_data, run);
		_4cutted:;
		}
		break;
	}
	case Type::MPPC_S2: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = data->mppc_peaks[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(5);
		double S2 = 0;
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _5cutted;
			S2 = 0;
			for (int pk = 0, pk_end = data->mppc_peaks[current_exp_index][ch_ind][run].size();
				pk != pk_end; ++pk){
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
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if (cut->GetChannel()==MPPC_channels[ch_ind]&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
						if (kFALSE == (*cut)(cut_data, run))
							goto _5cutted1;
				S2 += cut_data[0];
			_5cutted1:;
			}
			//1st parameter cut must be applied only if the rest of parameters are -2 === cut channel ==-1
			cut_data[0] = S2;
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if (cut->GetChannel()==-1&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
					if (kFALSE == (*cut)(cut_data, run))
						goto _5cutted;
			(*operation)(cut_data, run);
		_5cutted:;
		}
		break;
	}
	case Type::PMT_S2_S: //cuts for this one are tricky: they are 2-stage: first peaks selection for S2 and then cuts for S2 themselves
	{
		int run_size = data->pmt_peaks[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(5);
		double S2 = 0;
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _6cutted;
			S2 = 0;
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
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if ((cut->GetChannel()==PMT_channels[ch_ind])&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
						if (kFALSE == (*cut)(cut_data, run))
							goto _6cutted1;
				S2 += cut_data[0];
			_6cutted1:;
			}
			cut_data[1] = calibr_info.getPMT_S1pe(channel, current_exp_index);
			cut_data[0] = cut_data[1]>0 ? S2/cut_data[1] : S2;//1st parameter cut must be applied only if the rest 4 parameters are -2. -1 is reserved for invalid peaks
			cut_data[1] = -2;
			cut_data[2] = -2;
			cut_data[3] = -2;
			cut_data[4] = -2;
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((cut->GetChannel()==-1)&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
					if (kFALSE == (*cut)(cut_data, run))
						goto _6cutted;
			(*operation)(cut_data, run);
		_6cutted:;
		}
		break;
	}
	case Type::PMT_S2_int:
	{
		int run_size = data->PMT_S2_int[current_exp_index][ch_ind].size();
		std::vector<double> cut_data(1);
		double S1pe;
		for (auto run = 0; run != run_size; ++run){
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _7cutted;
			cut_data[0] = data->PMT_S2_int[current_exp_index][ch_ind][run];
			S1pe = calibr_info.getPMT_S1pe(channel,current_exp_index);
			cut_data[0] = S1pe>0 ? cut_data[0]/S1pe : cut_data[0];
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
					if (kFALSE == (*cut)(cut_data, run))
						goto _7cutted;
			(*operation)(cut_data, run);
			_7cutted:;
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
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _8cutted;
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
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
						if (kFALSE == (*cut)(cut_data, run))
							goto _8cutted1;
				(*operation)(cut_data, run);
				_8cutted1:;
			}
		_8cutted:;
		}
		break;
	}
	case Type::MPPC_sum_ts:
	{
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _9cutted;
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
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if ((cut->GetChannel()==MPPC_channels[chan_ind])&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
							if (kFALSE == (*cut)(cut_data, run))
								goto _9cutted1;
					(*operation)(cut_data, run);
					_9cutted1:;
				}
			}
			_9cutted:;
		}
		break;
	}
	case Type::MPPC_coord_x:
	case Type::MPPC_coord_y:
	case Type::MPPC_coord:
	{
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		std::vector<double> Npes (MPPC_channels.size()+2);
		std::vector<double> cut_data(5);
		double Npe_sum, Npe_max;
		for (auto run = 0; run != run_size; ++run) {
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _10cutted;
			Npe_sum = 0;
			Npe_max = 0;
			Npes[MPPC_channels.size()] =0;
			Npes[MPPC_channels.size()+1] = 0;
			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind<_ch_ind_end_; ++chan_ind) {
				double S2 = 0;
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
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if ((cut->GetChannel()==MPPC_channels[chan_ind])&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
							if (kFALSE == (*cut)(cut_data, run))
								goto _10cutted1;
					S2+=cut_data[0];
					_10cutted1:;
				}
				S2=(calibr_info.getS1pe(MPPC_channels[chan_ind]) > 0 ? S2/calibr_info.getS1pe(MPPC_channels[chan_ind]) : 0);
				Npes[chan_ind] = std::round(S2);
				if (MPPC_coords.find(MPPC_channels[chan_ind])!=MPPC_coords.end()){
					Npe_sum+=Npes[chan_ind];
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
			Npes[MPPC_channels.size()] /= Npe_sum;
			Npes[MPPC_channels.size()+1] /= Npe_sum;
#endif
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((cut->GetChannel()==-1)&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
					if (kFALSE == (*cut)(Npes, run))
						goto _10cutted;
			(*operation)(Npes, run);
			_10cutted:;
		}
		break;
	}
	case Type::PMT_Npe_sum:
	{
		int run_size = data->pmt_peaks[current_exp_index][0].size();
		std::vector<double> Npes (PMT_channels.size()+1);
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _11_5_cutted;
			Npes[PMT_channels.size()]=0;
			for (int chan_ind=0,_ch_ind_end_= PMT_channels.size(); chan_ind<_ch_ind_end_;++chan_ind) {
				double S2 = 0;
				for (int pk = 0, pk_end = data->pmt_peaks[current_exp_index][chan_ind][run].size(); pk != pk_end; ++pk) {
					cut_data[0] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].S;
					cut_data[1] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].A;
					cut_data[2] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].left;
					cut_data[3] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].right;
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->pmt_peaks[current_exp_index][chan_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if ((cut->GetChannel()==PMT_channels[chan_ind])&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
							if (kFALSE == (*cut)(cut_data, run))
								goto _11_5_cutted1;
					S2+=cut_data[0];
					_11_5_cutted1:;
				}
				S2=(calibr_info.getPMT_S1pe(PMT_channels[chan_ind], current_exp_index) > 0 ? S2/calibr_info.getPMT_S1pe(PMT_channels[chan_ind], current_exp_index) : 0);
				Npes[chan_ind] = S2;//std::round(S2);
				Npes[PMT_channels.size()]+=Npes[chan_ind];
			}
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((cut->GetChannel()==-1)&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
					if (kFALSE == (*cut)(Npes, run))
						goto _11_5_cutted;
			(*operation)(Npes, run);
			_11_5_cutted:;
		}
		break;
	}
	case Type::MPPC_Npe_sum:
	{
		int run_size = data->mppc_peaks[current_exp_index][0].size();
		std::vector<double> Npes (MPPC_channels.size()+1);
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _11cutted;
			Npes[MPPC_channels.size()]=0;
			for (int chan_ind=0,_ch_ind_end_= MPPC_channels.size(); chan_ind<_ch_ind_end_;++chan_ind) {
				double S2 = 0;
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
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if ((cut->GetChannel()==MPPC_channels[chan_ind])&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
							if (kFALSE == (*cut)(cut_data, run))
								goto _11cutted1;
					S2+=cut_data[0];
					_11cutted1:;
				}
				S2=(calibr_info.getS1pe(MPPC_channels[chan_ind]) > 0 ? S2/calibr_info.getS1pe(MPPC_channels[chan_ind]) : 0);
				Npes[chan_ind] = std::round(S2);
				Npes[MPPC_channels.size()]+=Npes[chan_ind];
			}
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((cut->GetChannel()==-1)&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
					if (kFALSE == (*cut)(Npes, run))
						goto _11cutted;
			(*operation)(Npes, run);
			_11cutted:;
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

		LoopThroughData(X_filler, _x_corr_ch, _x_corr, false, true, true);
		LoopThroughData(Y_filler, _y_corr_ch, _y_corr, false, true, true);

		std::vector<double> vals(2);
		for (auto run = 0; run != run_size; ++run) {
			if (2==(*cuts)[run]) {
				vals[0] = (*vals_x)[run];
				vals[1] = (*vals_y)[run];
				for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
					if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
						if (kFALSE == (*cut)(vals, run))
							goto _12cutted;
				(*operation)(vals, run);
				_12cutted:;
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
			int run_size = (is_PMT_type(_x_corr) ? data->pmt_peaks[exp_ind][ch_x_ind].size() : data->mppc_peaks[exp_ind][ch_x_ind].size());
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
			LoopThroughData(X_filler, _x_corr_ch, _x_corr, false, true, true);
			LoopThroughData(Y_filler, _y_corr_ch, _y_corr, false, true, true);

			for (auto run = 0; run != run_size; ++run) {
				if (2==(*cuts)[run]) {
					vals[0] = (*vals_x)[run];
					vals[1] = (*vals_y)[run];
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if ((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram()))
							if (kFALSE == (*cut)(vals, run))
								goto _13cutted;
					(*operation)(vals, run);
					_13cutted:;
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
		int run_size = data->pmt_peaks[current_exp_index][0].size();
		std::vector<double> Ns (PMT_channels.size()+1);
		std::vector<double> cut_data(5);
		for (auto run = 0; run != run_size; ++run) {
			for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_)&&apply_run_cuts; ++cut)
				if (kFALSE == cut->GetAccept(run))//not calculating it here!
					goto _14cutted;
			Ns[PMT_channels.size()]=0;
			for (int chan_ind=0,_ch_ind_end_= PMT_channels.size(); chan_ind<_ch_ind_end_;++chan_ind) {
				double N = 0;
				for (int pk = 0, pk_end = data->pmt_peaks[current_exp_index][chan_ind][run].size(); pk != pk_end; ++pk) {
					cut_data[0] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].S;
					cut_data[1] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].A;
					cut_data[2] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].left;
					cut_data[3] = data->pmt_peaks[current_exp_index][chan_ind][run][pk].right;
					cut_data[4] =
#ifdef PEAK_AVR_TIME
							data->pmt_peaks[current_exp_index][chan_ind][run][pk].t;
#else
							0.5*(cut_data[3]+cut_data[2]);
#endif
					for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
						if ((cut->GetChannel()==PMT_channels[chan_ind])&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
							if (kFALSE == (*cut)(cut_data, run))
								goto _14cutted1;
					++N;
					_14cutted1:;
				}
				Ns[chan_ind] = N;
				Ns[PMT_channels.size()]+=Ns[chan_ind];
			}
			for (auto cut = hist_cuts->begin(), c_end_ = hist_cuts->end(); (cut != c_end_); ++cut)
				if ((cut->GetChannel()==-1)&&((apply_hist_cuts&&cut->GetAffectingHistogram())||(apply_phys_cuts&&!cut->GetAffectingHistogram())))
					if (kFALSE == (*cut)(Ns, run))
						goto _14cutted;
			(*operation)(Ns, run);
			_14cutted:;
		}
		break;
	}
	}
}
//see function for std::vector<double> &vals usage in cuts' picker
void PostProcessor::FillHist(void* p_hist)//considers cuts and histogram tipe (void*)==either TH1D* or TH2D*
{
	struct temp_data {
		void* phist;
		int ch_size;
	} st_data;
	st_data.phist = p_hist;
	st_data.ch_size = is_PMT_type(current_type) ? PMT_channels.size() : MPPC_channels.size();
	FunctionWrapper* histogram_filler = new FunctionWrapper(&st_data);
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
	case Type::PMT_S2_int:
	case Type::PMT_Ss:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((temp_data*)data)->phist)->Fill(pars[0]);
			return true;
		};
		break;
	}
	case Type::PMT_t_S:
	case Type::MPPC_sum_ts:
	case Type::MPPC_t_S:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((temp_data*)data)->phist)->Fill(pars[4], pars[0]);
			return true;
		};
		break;
	}
	case Type::PMT_A_S:
	case Type::MPPC_A_S:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((temp_data*)data)->phist)->Fill(pars[1], pars[0]);
			return true;
		};
		break;
	}
	case Type::MPPC_coord:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((temp_data*)data)->phist)->Fill(pars[((temp_data*)data)->ch_size], pars[((temp_data*)data)->ch_size+1]);
			return true;
		};
		break;
	}
	case Type::MPPC_Npe_sum:
	case Type::MPPC_coord_x:
	case Type::PMT_sum_N:
	case Type::PMT_Npe_sum:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((temp_data*)data)->phist)->Fill(pars[((temp_data*)data)->ch_size]);
			return true;
		};
		break;
	}
	case Type::MPPC_coord_y:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((temp_data*)data)->phist)->Fill(pars[((temp_data*)data)->ch_size+1]);
			return true;
		};
		break;
	}
	case Type::PMT_times:
	case Type::MPPC_times:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((temp_data*)data)->phist)->Fill(pars[4],pars[0]);
			return true;
		};
		break;

	}
	case Type::PMT_times_N:
	case Type::MPPC_times_N:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH1D*)((temp_data*)data)->phist)->Fill(pars[4]);
			return true;
		};
		break;

	}
	case Type::Correlation:
	case Type::CorrelationAll:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			((TH2D*)((temp_data*)data)->phist)->Fill(pars[0],pars[1]);
			return true;
		};
		break;
	}
	}
	histogram_filler->SetFunction(filler_op);
	LoopThroughData(histogram_filler, current_channel, current_type, false, true, true);
	delete histogram_filler;
}

int PostProcessor::numOfFills(int channel, Type type) //TODO: maybe for the case of filling histograms with weights (MPPC_times and PMT_times) make summing its weights.
{
	int ret = 0;
	FunctionWrapper* histogram_filler = new FunctionWrapper(&ret);
	CUTTER filler_op;
	filler_op = [](std::vector<double>& pars, int run, void* data) {
		++(*(int*)data);
		return true;
	};
	histogram_filler->SetFunction(filler_op);
	LoopThroughData(histogram_filler, channel, type, false, true, true);
	delete histogram_filler;
	return ret;
}

//Run cuts are applied!
int PostProcessor::numOfRuns (void)
{
	int run_n = 0;
	int ch_ind = channel_to_index(current_channel, current_type);
	std::deque<EventCut> empty;
	std::deque<EventCut> *run_cuts = (NULL== get_run_cuts(current_exp_index) ? &empty : get_run_cuts(current_exp_index));
	int run_size = is_PMT_type(current_type) ? data->pmt_peaks[current_exp_index][ch_ind].size() : data->mppc_peaks[current_exp_index][ch_ind].size();
	for (auto run = 0; run != run_size; ++run){
		for (auto cut = run_cuts->begin(), c_end_ = run_cuts->end(); (cut != c_end_); ++cut)
			if (kFALSE == cut->GetAccept(run))//not calculating it here!
				goto _cutted;
		++run_n;
	_cutted:;
	}
	return run_n;
}

std::pair<double, double> PostProcessor::hist_y_limits(void) //considering cuts
{
	std::pair<double,double> ret(DBL_MAX,-DBL_MAX);
	struct temp_data {
		std::pair<double,double>* mm;
		int ch_size;
	} st_data;
	st_data.mm = &ret;
	st_data.ch_size = MPPC_channels.size();
	switch (current_type)
	{
	case Type::PMT_A_S:
	case Type::MPPC_A_S:
	case Type::PMT_t_S:
	case Type::MPPC_t_S:
	{
		FunctionWrapper* histogram_filler = new FunctionWrapper(&st_data);
		CUTTER filler_op = [](std::vector<double> &pars, int run, void* data){
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[0]);
			p->second = std::max(p->second, pars[0]);
			return true;
		};
		histogram_filler->SetFunction(filler_op);
		LoopThroughData(histogram_filler, current_channel, current_type, false, true);
		delete histogram_filler;
		break;
	}
	case Type::CorrelationAll:
	case Type::Correlation:
	{
		FunctionWrapper* histogram_filler = new FunctionWrapper(&st_data);
		CUTTER filler_op = [](std::vector<double> &pars, int run, void* data){
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[1]);
			p->second = std::max(p->second, pars[1]);
			return true;
		};
		histogram_filler->SetFunction(filler_op);
		LoopThroughData(histogram_filler, current_channel, current_type, false, true);
		delete histogram_filler;
		break;
	}
	case Type::MPPC_coord:
	{
		FunctionWrapper* histogram_filler = new FunctionWrapper(&st_data);
		CUTTER filler_op = [](std::vector<double> &pars, int run, void* data){
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[((temp_data*)data)->ch_size+1]);
			p->second = std::max(p->second, pars[((temp_data*)data)->ch_size+1]);
			return true;
		};
		histogram_filler->SetFunction(filler_op);
		LoopThroughData(histogram_filler, current_channel, current_type, false, true, true);
		delete histogram_filler;
		break;
	}
	default:
		break;
	}
	return ret;
}

std::pair<double, double> PostProcessor::hist_x_limits(bool consider_displayed_cuts) //valid only for 2d plots
{
	std::pair<double, double> ret(DBL_MAX, -DBL_MAX);
	struct temp_data {
		std::pair<double,double>* mm;
		int ch_size;
	} st_data;
	st_data.mm = &ret;
	st_data.ch_size = is_PMT_type(current_type) ? PMT_channels.size() : MPPC_channels.size();
	FunctionWrapper* histogram_filler = new FunctionWrapper(&st_data);
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
	case Type::PMT_S2_int:
	case Type::PMT_Ss:
	case Type::Correlation:
	case Type::CorrelationAll:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[0]);
			p->second = std::max(p->second, pars[0]);
			return true;
		};
		break;
	}
	case Type::PMT_times:
	case Type::PMT_times_N:
	case Type::MPPC_times:
	case Type::MPPC_times_N:
	case Type::PMT_t_S:
	case Type::MPPC_sum_ts:
	case Type::MPPC_t_S:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[4]);
			p->second = std::max(p->second, pars[4]);
			return true;
		};
		break;
	}
	case Type::PMT_A_S:
	case Type::MPPC_A_S:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[1]);
			p->second = std::max(p->second, pars[1]);
			return true;
		};
		break;
	}
	case Type::MPPC_Npe_sum:
	case Type::MPPC_coord_x:
	case Type::MPPC_coord:
	case Type::PMT_sum_N:
	case Type::PMT_Npe_sum:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[((temp_data*)data)->ch_size]);
			p->second = std::max(p->second, pars[((temp_data*)data)->ch_size]);
			return true;
		};
		break;
	}
	case Type::MPPC_coord_y:
	{
		filler_op = [](std::vector<double>& pars, int run, void* data) {
			std::pair<double,double>* p = ((temp_data*)data)->mm;
			p->first = std::min(p->first, pars[((temp_data*)data)->ch_size+1]);
			p->second = std::max(p->second, pars[((temp_data*)data)->ch_size+1]);
			return true;
		};
		break;
	}
	}
	histogram_filler->SetFunction(filler_op);
	LoopThroughData(histogram_filler, current_channel, current_type, consider_displayed_cuts, true, true);
	delete histogram_filler;
	return ret;
}

void PostProcessor::default_hist_setups(HistogramSetups* setups)//does not affect cuts
{
	if (NULL==setups) {
		std::cerr<<"PostProcessor::default_hist_setups: Error: NULL HistogramSetups*"<<std::endl;
		return;
	}
	int _N_ = numOfFills(current_channel, current_type);
	setups->N_bins = _N_;
	setups->N_bins = std::max(4,(int)std::round(std::sqrt(setups->N_bins)));
	std::pair<double, double> x_lims = hist_x_limits();
	x_lims.second+=(x_lims.second-x_lims.first)/setups->N_bins;

	setups->use_fit = kFALSE;
	setups->fitted = kFALSE;

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
	calibr_info.Save();

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
	update(All);
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
	update(All);
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

void PostProcessor::update(UpdateState to_update)//TODO: optimize it?
{
	if (!isValid()) {
		std::cerr << "PostProcessor::update: Wrong input data: no channels or experiments from AnalysisManager" << std::endl;
		return;
	}
	if (NULL==get_hist_setups()) {
		HistogramSetups setups;
		default_hist_setups(&setups);
		if (false==set_hist_setups(&setups, current_exp_index, current_channel, current_type))
			return;
	}
	HistogramSetups* setups = get_hist_setups();
	TCanvas * canvas = get_current_canvas();
	if (NULL==canvas) {
		std::cerr<<"PostProcessor::update: Error: NULL canvas"<<std::endl;
	} else {
		canvas->cd();
		canvas->SetTitle(hist_name().c_str());
		canvas->Clear();

		std::pair<double, double> x_lims = hist_x_limits();
		x_lims.second+=(x_lims.second-x_lims.first)/setups->N_bins;
		std::pair<double, double> y_lims = hist_y_limits();
		if (to_update&UpdateState::Histogram) {
			if (is_TH1D_hist(current_type)) {
				TH1D new_hist1(hist_name().c_str(), hist_name().c_str(), setups->N_bins,
						(is_zoomed().first ? get_current_x_zoom().first :  x_lims.first),
						(is_zoomed().first ? get_current_x_zoom().second :  x_lims.second));
				set_hist1(&new_hist1);
				FillHist(get_current_hist1());
			} else {
				TH2D new_hist2 (hist_name().c_str(), hist_name().c_str(), setups->N_bins,
						(is_zoomed().first ? get_current_x_zoom().first :  x_lims.first),
						(is_zoomed().first ? get_current_x_zoom().second :  x_lims.second), setups->N_bins,
						(is_zoomed().second ? get_current_y_zoom().first : y_lims.first),
						(is_zoomed().second ? get_current_y_zoom().second :  y_lims.second));
				set_hist2(&new_hist2);
				FillHist(get_current_hist2());
			}
		}
	}

	if (is_TH1D_hist(current_type)) {
		if (to_update&UpdateState::FitFunction) {
			std::pair<double, double> x_drawn_lims = hist_x_limits(true);
			TF1* ff = create_fit_function(setups, x_drawn_lims);
			set_fit_function(ff); //creates internal copy, hence ff->Delete()
			if (NULL!=ff)
				ff->Delete();
		}
		if ((to_update&UpdateState::Fit) || (to_update&UpdateState::FitFunction)) {
			TH1D* hist = get_current_hist1();
			TF1* ff = get_current_fit_function();
			if (NULL != ff && setups->use_fit && NULL!=hist) {
				hist->Fit(ff, "RQ");
				setups->fitted = kTRUE;
				for (int par = 0; par < setups->par_val.size(); ++par)
					setups->par_val[par] = ff->GetParameter(par);
			}
		}
	}
	if (NULL!=canvas) {
		if (is_TH1D_hist(current_type)) {
			TH1D* hist = get_current_hist1();
			if (hist)
				hist->Draw();
		} else {
			TH2D* hist = get_current_hist2();
			if (hist)
				hist->Draw(/*"lego"*/);
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
	if (to_update&UpdateState::Results) {
		update_physical();
		update_Npe();
	}
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
	calibr_info.recalibrate(data->N_pe_direct, data->N_pe_Double_I, data->Fields);
	for (int exp = 0, exp_end_ = data->N_pe_direct.size(); exp != exp_end_; ++exp) {
		for (int ch = 0, ch_end_ = MPPC_channels.size(); ch != ch_end_; ++ch) {
			if (exp > calibr_info.get_N_calib(MPPC_channels[ch]).first)
				data->N_pe_result[exp][ch] = data->N_pe_Double_I[exp][ch];
			else
				data->N_pe_result[exp][ch] = data->N_pe_direct[exp][ch];
		}
		if (!data->N_pe_PMT3.empty())
			if (calibr_info.getPMT_S1pe(0, exp)>0)
				data->N_pe_PMT3[exp] = PMT3_avr_S2_S[exp] / calibr_info.getPMT_S1pe(0, exp);
		if (!data->N_pe_PMT1.empty())
			if (calibr_info.getPMT_S1pe(1, exp)>0)
				data->N_pe_PMT1[exp] = PMT1_avr_S2_S[exp] / calibr_info.getPMT_S1pe(1, exp);
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
	switch (current_type){
	case Type::MPPC_Double_I:
	{
		if (setups->N_gauss>0 && setups->fitted)
			avr_Double_I[current_exp_index][mppc_channel_to_index(current_channel)] = setups->par_val[1];
		else {
			if (0==setups->N_gauss) {
				LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
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
				LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
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
				LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
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
		CalibrationInfo::S1pe_method meth = calibr_info.get_method(current_exp_index, current_channel);
		if (meth == CalibrationInfo::Ignore) {
			calibr_info.calculateS1pe(current_channel);
			std::cout << "S1pe = "<<calibr_info.getS1pe(current_channel) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::UsingMean) {
			LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
			if (0==stat_data.weight)
				std::cout << "Warning! No mean calibration Ss value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			else
				calibr_info.set_S1pe(current_channel, current_exp_index, stat_data.val / (double)stat_data.weight, stat_data.weight);
			calibr_info.calculateS1pe(current_channel);
			std::cout << "S1pe = "<<calibr_info.getS1pe(current_channel) << std::endl;
			break;
		}
		if (meth == CalibrationInfo::Using1pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>0 && setups->fitted)
				calibr_info.set_S1pe(current_channel, current_exp_index, setups->par_val[1], 1/*TODO: add calculation of N under fit*/);
			else {
				std::cout << "Warning! No calibration S1pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		if (meth == CalibrationInfo::Using2pe || meth == CalibrationInfo::Using1pe2pe) {
			if (setups->N_gauss>=2 && setups->fitted)
				calibr_info.set_S2pe(current_channel, current_exp_index, setups->par_val[4], 1/*TODO: add calculation of N under fit*/);
			else {
				std::cout << "Warning! No calibration S2pe value for  " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
			}
		}
		calibr_info.calculateS1pe(current_channel);
		std::cout << "S1pe = "<<calibr_info.getS1pe(current_channel) << std::endl;
		break;
	}
	case Type::PMT_Ss:
	{
		LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
		if (0==stat_data.weight)
			std::cout << "Warning! No mean calibration Ss value for " << data->exp_area.experiments[current_exp_index] << " ch " << current_channel << std::endl;
		else {
			//calibr_info.set_S1pe(current_channel, current_exp_index, stat_data.val / (double)stat_data.weight, stat_data.weight);
			std::cout << "S1pe(mean) = "<<stat_data.val / (double)stat_data.weight << std::endl;
		}
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
				LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
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
				LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
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
		LoopThroughData(mean_taker, current_channel, current_type, true, true, true); //TODO: A LOT of loops here. Rework the code to avoid this (store more info/call all relevant loops in one place)
		if (0==stat_data.weight)
			std::cout << "Warning! No peaks selected ch " << current_channel << std::endl;
		else {
			std::pair<double, double> drawn_limits = hist_x_limits(true);
			std::cout <<"N runs = "<< numOfRuns() << " S  = "<<stat_data.val<< " Npeaks = "<<stat_data.weight<<" dT = "
					<<drawn_limits.second - drawn_limits.first;
		}
		break;
	}
	case Type::PMT_times_N:
	case Type::MPPC_times_N:
	{
		LoopThroughData(mean_taker, current_channel, current_type, true, true, true);
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
	//update(All);
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
	//update(All);
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
		//update(All);
		break;
		anew:;
	}
}

void PostProcessor::remove_hist_cut(std::string name, int ch)
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
			if ((i->GetName() == name)&&(i->GetChannel()==ch)){
				setups->hist_cuts.erase(i);
				goto anew;
			}
		}
		//update(All);
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
		data->pmt_peaks[current_exp_index][pmt_channel_to_index(current_channel)].size()
		:data->mppc_peaks[current_exp_index][mppc_channel_to_index(current_channel)].size();
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

	FunctionWrapper* cut_calculator = new FunctionWrapper(&(RunCuts->back()));
	cut_calculator->SetFunction([](std::vector<double> &pars, int run, void *data) {
		((EventCut*)data)->SetAccept(run, true);
		return true;
	});
	LoopThroughData(cut_calculator, current_channel, current_type, true, false);
	update(All);
	delete cut_calculator;
}

void PostProcessor::unset_as_run_cut(std::string name)
{
	if (!isValid()){
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
		update(All);
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
	TF1* fit_func = get_current_fit_function();
	if (NULL==fit_func && do_fit) {
		std::pair<double, double> drawn_limits = hist_x_limits(true);
		fit_func = create_fit_function(setups, drawn_limits);
		set_fit_function(fit_func);
		fit_func->Delete();
		fit_func = get_current_fit_function();
	}
	//update_fit_function();//considered updated
	if ((NULL != fit_func) && is_TH1D_hist(current_type) && do_fit ) {
		hist1->Fit(fit_func,"RQ");
		setups->fitted = kTRUE;
		for (std::size_t par = 0; par < setups->par_val.size(); ++par)
			setups->par_val[par] = fit_func->GetParameter(par);
	}
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
	update(All);
}

void PostProcessor::set_zoom (double xl, double xr)
{
	std::pair<double, double> x_lims = hist_x_limits(), x_zoom, y_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	//std::pair<double, double> y_lims = hist_y_limits();
	x_zoom.first = std::min(xl, xr);
	x_zoom.second = std::max(xl, xr);
	x_zoom.first = std::max(x_zoom.first, x_lims.first);
	x_zoom.second = std::min(x_zoom.second, x_lims.second);
	CanvasSetups::set_zoom(x_zoom, y_zoom);
	update(Histogram);
}

void PostProcessor::set_zoom_y (double yl, double yr)
{
	if (is_TH1D_hist(current_type)){
		std::cout<<"can't set y zoom for TH1D histogram"<<std::endl;
		return;
	}
	std::pair<double, double> y_lims = hist_y_limits(), y_zoom, x_zoom = std::pair<double, double>(-DBL_MAX, DBL_MAX);
	//std::pair<double, double> y_lims = hist_y_limits();
	y_zoom.first = std::min(yl, yr);
	y_zoom.second = std::max(yl, yr);
	y_zoom.first = std::max(y_zoom.first, y_lims.first);
	y_zoom.second = std::min(y_zoom.second, y_lims.second);
	CanvasSetups::set_zoom(x_zoom, y_zoom);
	update(Histogram);
}

void PostProcessor::set_zoom (double xl, double xr, double yl, double yr)
{
	std::pair<double, double> x_lims = hist_x_limits(), y_lims = hist_y_limits(), y_zoom, x_zoom;
	x_zoom.first = std::min(xl, xr);
	x_zoom.second = std::max(xl, xr);
	x_zoom.first = std::max(x_zoom.first, x_lims.first);
	x_zoom.second = std::min(x_zoom.second, x_lims.second);
	y_zoom.first = std::min(yl, yr);
	y_zoom.second = std::max(yl, yr);
	y_zoom.first = std::max(y_zoom.first, y_lims.first);
	y_zoom.second = std::min(y_zoom.second, y_lims.second);
	CanvasSetups::set_zoom(x_zoom, y_zoom);
	update(Histogram);
}

void PostProcessor::unset_zoom(bool do_update)
{
	CanvasSetups::unset_zoom();
	if (do_update)
		update(Histogram);
}

void PostProcessor::set_fit_gauss(int N)
{
	if (!isValid()){
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
		int _N_in_hist = numOfFills(current_channel, current_type);
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
	update(FitFunction);
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
	update(FitFunction);
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
	if ((index < 0) || (index >= 3 * setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	setups->par_left_limits[index] = std::min(left, right);
	setups->par_right_limits[index] = std::max(left, right);
	update(FitFunction);
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
		std::deque<EventCut> *RunCuts = get_run_cuts(current_exp_index);
		if (NULL==RunCuts) {
			std::cout<<"PostProcessor::status: Error: NULL RunCuts"<<std::endl;
		} else {
			std::cout<<"RunCuts ["<< RunCuts->size()<<"]: ";
			for (auto i = RunCuts->begin(), _end_ = RunCuts->end(); i != _end_; ++i)
				std::cout << (i->GetName()) << ((i == (_end_ - 1)) ? "" : " | ");
			std::cout << std::endl;
		}

		std::pair<double, double> x_lims = hist_x_limits(), x_drawn_lims = hist_x_limits(true);
		std::cout << "Current_setups: " << std::hex << setups << std::dec << std::endl;
		std::cout << "\tleft_limit: " << x_lims.first << std::endl;
		std::cout << "\tright_limit: " << x_lims.second << std::endl;
		std::cout << "\tleft_drawn_limit: " << x_drawn_lims.first << std::endl;
		std::cout << "\tright_drawn_limit: " << x_drawn_lims.second << std::endl;
		std::cout << "\thist_cuts [" << setups->hist_cuts.size() << "]: ";
		for (auto i = setups->hist_cuts.begin(), _end_ = setups->hist_cuts.end(); i != _end_; ++i)
			std::cout << i->GetName() << (i->GetAffectingHistogram() ? "" : "(Only shown)") << (((i == (_end_ - 1)) ? "" : " | "));
		std::cout << std::endl;

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
	}
}
