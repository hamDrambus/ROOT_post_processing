#include "AllRunsResults.h"

AllRunsResults::AllRunsResults(/*ParameterPile::*/experiment_area experiment)
{
	_exp = experiment;
	N_of_runs = 0;
	Iteration_N = 0;
	for (auto ex = _exp.experiments.begin(); ex != _exp.experiments.end(); ++ex) {
		int mppc_ch = 0;
		for (int ch = _exp.channels.get_next_index(); ch != -1; ch = _exp.channels.get_next_index()){
			if ((ch<32)&&(ch!=2)) {
				std::string prefix = DATA_PREFIX + DATA_PMT_VERSION + "/PMT_" + *ex + "/PMT_"+std::to_string(ch)+"/PMT_"+std::to_string(ch)+"_";
				pmt_channels.push_back(ch);
				pmt_peaks.push_back(std::deque<std::deque<peak>>());
				pmt_S2_integral.push_back(std::vector<double>());
				vector_from_file(pmt_peaks.back(), prefix + "peaks.dat");
				vector_from_file(pmt_S2_integral.back(), prefix + "S2_int.dat");
				if (pmt_peaks.back().empty()||pmt_S2_integral.back().empty()) {
					pmt_channels.pop_back();
					pmt_peaks.pop_back();
					pmt_S2_integral.pop_back();
				} else {
					std::cout << "Loaded channel " << ch << std::endl;
				}
				continue;
			}
			std::string prefix = DATA_PREFIX+"/"+DATA_MPPC_VERSION + "/MPPCs_" + *ex + "/MPPC_" + std::to_string(ch) + "/MPPC_" + std::to_string(ch) + "_";
			
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
