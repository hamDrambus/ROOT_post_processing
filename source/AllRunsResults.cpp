#include "AllRunsResults.h"

AllRunsResults::AllRunsResults(/*ParameterPile::*/experiment_area experiment)
{
	_exp = experiment;
	N_of_runs = 0;
	Iteration_N = 0;
	for (auto ex = _exp.experiments.begin(); ex != _exp.experiments.end(); ++ex) {
		for (int ch = _exp.channels.get_next_index(); ch != -1; ch = _exp.channels.get_next_index()){
			std::string prefix_pmt;
			std::string prefix_mppc;
			if (name_scheme_version == name_scheme_v1) {
				prefix_pmt = data_prefix_path + DATA_PMT_VERSION + "/PMT_" + *ex + "/PMT_"+std::to_string(ch)+"/PMT_"+std::to_string(ch)+"_";
				prefix_mppc = data_prefix_path +DATA_MPPC_VERSION + "/MPPCs_" + *ex + "/MPPC_" + std::to_string(ch) + "/MPPC_" + std::to_string(ch) + "_";
			}
			if (name_scheme_version == name_scheme_v2) {
				std::string ch_str = std::to_string(ch);
				prefix_pmt = data_prefix_path + *ex + "/" + DATA_PMT_VERSION + "_" + ch_str+"/"+ DATA_PMT_VERSION + "_" + ch_str + "_";
				prefix_mppc = data_prefix_path + *ex + "/" + DATA_MPPC_VERSION + "_" + ch_str + "/" + DATA_MPPC_VERSION + "_" + ch_str + "_";
			}
			if (test_file(prefix_pmt + "peaks.dat")) {
				pmt_channels.push_back(ch);
				pmt_peaks.push_back(std::deque<std::deque<peak>>());
				pmt_S2_integral.push_back(std::vector<double>());
				vector_from_file(pmt_peaks.back(), prefix_pmt + "peaks.dat");
				if (name_scheme_version == name_scheme_v1)
					vector_from_file(pmt_S2_integral.back(), prefix_pmt + "S2_int.dat"); //pmt_S2_integral is not necessary for all channels
				if (name_scheme_version == name_scheme_v2)
					vector_from_file(pmt_S2_integral.back(), prefix_pmt + "I.dat"); //pmt_S2_integral is not necessary for all channels
				if (pmt_peaks.back().empty()) {
					pmt_channels.pop_back();
					pmt_peaks.pop_back();
					pmt_S2_integral.pop_back();
				} else {
					if (pmt_S2_integral.back().empty()) { //In order to not change post processing program, fill array with dummy values.
						pmt_S2_integral.back().resize(pmt_peaks.back().size(), -1);
					}
					if (N_of_runs == 0) {
						N_of_runs = pmt_peaks.back().size();
					} else {
						if (N_of_runs != pmt_peaks.back().size()) {
							std::cout << "Event number mismatch for channel " << ch << "! It has " << pmt_peaks.back().size() << " events instead of expected " << N_of_runs << std::endl;
							pmt_channels.pop_back();
							pmt_peaks.pop_back();
							pmt_S2_integral.pop_back();
							continue;
						}
					}
					std::cout << "Loaded channel " << ch << std::endl;
					continue;
				}
			}
			if (test_file(prefix_mppc + "peaks.dat")) {
				mppc_peaks.push_back(std::deque<std::deque<peak>>());
				mppc_Double_Is.push_back(std::vector<double>());
				mppc_channels.push_back(ch);

				vector_from_file(mppc_peaks.back(), prefix_mppc + "peaks.dat");
				vector_from_file(mppc_Double_Is.back(), prefix_mppc + "double_I.dat");
				if (mppc_peaks.back().empty()) { //empty files
					mppc_peaks.pop_back();
					mppc_Double_Is.pop_back();
					mppc_channels.pop_back();
				} else {
					if (N_of_runs == 0) {
						N_of_runs = mppc_peaks.back().size();
					} else {
						if (N_of_runs != mppc_peaks.back().size()) {
							std::cout << "Event number mismatch for channel " << ch << "! It has " << mppc_peaks.back().size() << " events instead of expected " << N_of_runs << std::endl;
							mppc_peaks.pop_back();
							mppc_Double_Is.pop_back();
							mppc_channels.pop_back();
							continue;
						}
						if (mppc_Double_Is.back().empty()) { //In order to not change post processing program, fill array with dummy values.
							mppc_Double_Is.back().resize(mppc_peaks.back().size(), -1);
						}
						/*if (N_of_runs != mppc_Double_Is.back().size()) {
							std::cout << "Event number mismatch for channel " << ch << "! peaks have " << N_of_runs << " events and double intgral has " << mppc_Double_Is.back().size() << std::endl;
							mppc_peaks.pop_back();
							mppc_Double_Is.pop_back();
							mppc_channels.pop_back();
							continue;
						}*/
					}
					std::cout << "Loaded channel " << ch << std::endl;
				}
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

bool AllRunsResults::test_file(std::string fname) {
	std::ifstream str;
	str.open(fname, std::ios_base::binary);
	if (!str.is_open())
		return false;
	return true;
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
		vector_to_file(mppc_Double_Is, ch, output_prefix + "double_I.dat", "MPPC_II");
		vector_to_file(mppc_peaks, ch, output_prefix + "peaks.dat", "MPPC_peaks");
	}
	return true;
}

int AllRunsResults::Iteration(void) const
{	return Iteration_N;}

void AllRunsResults::Clear(void)
{
	std::vector<bool>().swap(_valid);
	std::deque<std::vector<double>>().swap(mppc_Double_Is);
	std::deque<int>().swap(mppc_channels);
	std::deque<int>().swap(pmt_channels);
	std::deque<std::deque<std::deque<peak>>>().swap(mppc_peaks);
	std::deque<std::deque<std::deque<peak>>>().swap(pmt_peaks);
	std::deque<std::vector<double>>().swap(pmt_S2_integral);
	N_of_runs = 0;
	//Iteration_N preserved;
}
