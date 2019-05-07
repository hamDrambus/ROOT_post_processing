#include "AllRunsResults.h"

AllRunsResults::AllRunsResults(/*ParameterPile::*/experiment_area experiment)
{
	_exp = experiment;
	N_of_runs = 0;
	Iteration_N = 0;
	for (auto ex = _exp.experiments.begin(); ex != _exp.experiments.end(); ++ex) {
		int mppc_ch = 0;
		for (int ch = _exp.channels.get_next_index(); ch != -1; ch = _exp.channels.get_next_index()){
			if (ch<32) {
				std::string prefix = data_prefix_path + DATA_PMT_VERSION + "/PMT_" + *ex + "/PMT_"+std::to_string(ch)+"/PMT_"+std::to_string(ch)+"_";
				pmt_channels.push_back(ch);
				pmt_peaks.push_back(std::deque<std::deque<peak>>());
				pmt_S2_integral.push_back(std::vector<double>());
				vector_from_file(pmt_peaks.back(), prefix + "peaks.dat");
				vector_from_file(pmt_S2_integral.back(), prefix + "S2_int.dat"); //pmt_S2_integral is not necessary for all channels
				if (pmt_peaks.back().empty()) {
					pmt_channels.pop_back();
					pmt_peaks.pop_back();
					pmt_S2_integral.pop_back();
				} else {
					if (pmt_S2_integral.back().empty()) { //In order to not change post processing program, fill array with dummy values.
						pmt_S2_integral.back().resize(pmt_peaks.back().size(), -1);
					}
					std::cout << "Loaded channel " << ch << std::endl;
				}
				continue;
			}
			std::string prefix = data_prefix_path+"/"+DATA_MPPC_VERSION + "/MPPCs_" + *ex + "/MPPC_" + std::to_string(ch) + "/MPPC_" + std::to_string(ch) + "_";
			
			mppc_peaks.push_back(std::deque<std::deque<peak>>());
			mppc_S2_S.push_back(std::vector<double>());
			mppc_S2_start_time.push_back(std::vector<double>());
			mppc_S2_finish_time.push_back(std::vector<double>());
			mppc_Double_Is.push_back(std::vector<double>());
			mppc_channels.push_back(ch);

			vector_from_file(mppc_peaks.back(), prefix + "peaks.dat");
			vector_from_file(mppc_S2_S.back(), prefix + "S2_S.dat");
			vector_from_file(mppc_Double_Is.back(), prefix + "double_I.dat");
			vector_from_file(mppc_S2_start_time.back(), prefix + "S2_start_t.dat");
			vector_from_file(mppc_S2_finish_time.back(), prefix + "S2_finish_t.dat");

			if (mppc_peaks.back().empty()|| mppc_S2_S.back().empty() || mppc_Double_Is.back().empty()
				||mppc_S2_finish_time.back().empty()||mppc_S2_start_time.back().empty()){ //empty files
				mppc_peaks.pop_back();
				mppc_Double_Is.pop_back();
				mppc_S2_S.pop_back();
				mppc_channels.pop_back();
				mppc_S2_start_time.pop_back();
				mppc_S2_finish_time.pop_back();
			} else {
				++mppc_ch;
				std::cout << "Loaded channel " << ch << std::endl;
				N_of_runs = std::max(N_of_runs, (int)std::max(mppc_Double_Is.back().size(), mppc_S2_S.back().size()));
			}
		}
	}
}

void AllRunsResults::processAllRuns(std::deque<SingleRunResults> &single_results)
{}


void AllRunsResults::Merge(AllRunsResults* with)
{
}

void AllRunsResults::Merged(void)
{	
	++Iteration_N;
}

void AllRunsResults::vector_from_file(std::vector<double> &what, std::string fname)
{
	what.clear();
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (!str.is_open())
		return;
	double val;
	size_t size;
	while (!str.eof()) {
		str.read((char*)&size,sizeof(std::size_t));
		std::size_t counter = 0;
		while (!str.eof() && counter != size){
			str.read((char*)&val,sizeof(double));
			what.push_back(val);
			++counter;
		}
		break;
	}
	str.close();
}

void AllRunsResults::vector_from_file(std::deque<std::deque<peak>> &pks, std::string fname)
{
	pks.clear();
	std::ifstream str;
	str.open(fname,std::ios_base::binary);
	if (!str.is_open())
		return;
	std::size_t run_size;
	while (!str.eof()){
		str.read((char*)&run_size,sizeof(std::size_t));
		std::size_t run_counter = 0;
		while (!str.eof() && run_counter != run_size){
			pks.push_back(std::deque<peak>());
			std::size_t size;
			str.read((char*)&size,sizeof(std::size_t));
			std::size_t counter = 0;
			while (!str.eof() && counter != size){
				peak pk;
				str.read((char*)&pk.left, sizeof(double));
				str.read((char*)&pk.right, sizeof(double));
				str.read((char*)&pk.S, sizeof(double));
				str.read((char*)&pk.A, sizeof(double));
#ifdef PEAK_AVR_TIME
				str.read((char*)&pk.t, sizeof(double));
#endif
				pks.back().push_back(pk);
				++counter;
			}
			++run_counter;
		}
		break;
	}
	str.close();
}


void vector_to_file(DVECTOR &xs, DVECTOR &ys, DVECTOR &ydisps, std::string fname, std::string title)
{
	std::ofstream str;
	open_output_file(fname, str, std::ios_base::trunc);
	str << "//" << title << std::endl;
	if ((xs.size() != ys.size()) || (xs.size() != ydisps.size()))
		str << "//x-y-y_disp size mismatch" << std::endl;
	else
		for (std::size_t i = 0, i_end_ = xs.size(); i != i_end_; ++i)
			str << xs[i] << "\t" << ys[i] << "\t" << ydisps[i] << std::endl;
	str.close();
}

void AllRunsResults::vector_to_file(std::deque<std::deque<std::deque<peak> > > &pks, int ch_ind, std::string fname, std::string title)
{
	std::ofstream str;
	open_output_file(fname, str, std::ios_base::trunc | std::ios_base::binary);
	std::size_t real_size = 0;
	for (std::size_t run = 0, run_end_ = pks[ch_ind].size(); run != run_end_; ++run)
		if (_valid[run])
			++real_size;
	str.write((char*)&real_size, sizeof(std::size_t));
	for (std::size_t run = 0, run_end_ = pks[ch_ind].size(); run != run_end_; ++run) {
		if (!_valid[run])
			continue;
		std::size_t pk_end_ = pks[ch_ind][run].size();
		str.write((char*)&pk_end_, sizeof(std::size_t));
		for (std::size_t p = 0; p != pk_end_; ++p) {
			str.write((char*)&pks[ch_ind][run][p].left, sizeof(double));
			str.write((char*)&pks[ch_ind][run][p].right, sizeof(double));
			str.write((char*)&pks[ch_ind][run][p].S, sizeof(double));
			str.write((char*)&pks[ch_ind][run][p].A, sizeof(double));
			str.write((char*)&pks[ch_ind][run][p].t, sizeof(double));
		}
	}
	str.close();
}

void AllRunsResults::vector_to_file(std::deque<std::vector<double> > &what, int ch_ind, std::string fname, std::string title)
{
	std::ofstream str;
	open_output_file(fname, str, std::ios_base::trunc | std::ios_base::binary);
	std::size_t real_size = 0;
	for (std::size_t run = 0, run_end_ = what[ch_ind].size(); run != run_end_; ++run)
		if (_valid[run])
			++real_size;
	str.write((char*)&real_size, sizeof(std::size_t));
	for (std::size_t run = 0, run_end_ = what[ch_ind].size(); run != run_end_; ++run)
		if (_valid[run])
			str.write((char*)&what[ch_ind][run], sizeof(double));
	str.close();
}

void AllRunsResults::vector_to_file(std::vector<double> &xs, std::vector<double> &ys, std::vector<double> &ydisps, std::string fname, std::string title)
{
	std::ofstream str;
	open_output_file(fname, str, std::ios_base::trunc);
	str << "//" << title << std::endl;
	str << "//t [us]\tsignal\terror" << std::endl;
	for (auto x = xs.begin(), y = ys.begin(), d = ydisps.begin(), x_end_ = xs.end(), y_end_ = ys.end(), d_end_ = ydisps.end(); (x != x_end_) && (y != y_end_) && (d != d_end_); ++x, ++y, ++d)
		str << *x << '\t' << *y << '\t' << *d << std::endl;
	str.close();
}

//prefix must end in '/'
bool AllRunsResults::SaveTo(std::string prefix)
{
	if (_exp.experiments.empty()) {
		std::cout << "Nothing to save" << std::endl;
		return false;
	}
	std::string exp_str = _exp.experiments.front();
	for (std::size_t ch = 0; ch < pmt_channels.size(); ++ch) {
		std::stringstream ch_str;
		ch_str << pmt_channels[ch];
		std::string output_prefix = prefix + DATA_PMT_VERSION + "/PMT_" + exp_str + "/PMT_" + std::to_string(pmt_channels[ch]) + "/PMT_" + std::to_string(pmt_channels[ch]) + "_";
		vector_to_file(pmt_peaks, ch, output_prefix + "peaks.dat");
		vector_to_file(pmt_S2_integral, ch, output_prefix + "S2_int.dat");
	}
	for (int ch = 0; ch < mppc_channels.size(); ++ch) {
		std::string output_prefix = prefix + "/" + DATA_MPPC_VERSION + "/MPPCs_" + exp_str + "/MPPC_" + std::to_string(mppc_channels[ch]) + "/MPPC_" + std::to_string(mppc_channels[ch]) + "_";
		vector_to_file(mppc_S2_S, ch, output_prefix + "S2_S.dat", "MPPC_S2");
		vector_to_file(mppc_S2_start_time, ch, output_prefix + "S2_start_t.dat", "MPPC_st");
		vector_to_file(mppc_S2_finish_time, ch, output_prefix + "S2_finish_t.dat", "MPPC_fin");
		vector_to_file(mppc_Double_Is, ch, output_prefix + "double_I.dat", "MPPC_II");
		vector_to_file(mppc_peaks, ch, output_prefix + "peaks.dat", "MPPC_peaks");
	}
	return true;
}

int AllRunsResults::Iteration(void) const
{	return Iteration_N;}

void AllRunsResults::Clear(void)
{
	std::deque<std::vector<double>>().swap(mppc_S2_S);
	std::deque<std::deque<std::deque<peak>>>().swap(mppc_peaks);
	std::deque<std::deque<std::deque<peak>>>().swap(pmt_peaks);
	std::deque<std::vector<double>>().swap(mppc_Double_Is);
	std::deque<int>().swap(mppc_channels);
	std::deque<int>().swap(pmt_channels);
	N_of_runs = 0;
	//Iteration_N preserved;
}
