#include <stdio.h>
#if defined(__WIN32__)
#include <direct.h>
#endif

#include "GlobalParameters.h"
#include "AllRunsResults.h"

//TODO: some functions must be moved to PostProcessor class. (then I won't need the std::vector<double>* get_data methods)

peak::peak() : right(-1), left(-1), S(-1), A(-1)
#ifdef PEAK_AVR_TIME
,t(-1)
#endif
{}

peak_processed::peak_processed(const double& iS, const double& iA, const double& ileft, const double& iright, const double& it, const int& iNpe) :
		S(iS), A(iA), left(ileft), right(iright), t(it), Npe(iNpe)
{}

peak_processed::peak_processed(const peak& pk, const int& iNpe)	:
		S(pk.S), A(pk.A), left(pk.left), right(pk.right),
#ifdef PEAK_AVR_TIME
		t(pk.t),
#else
		t(0.5*(pk.left+pk.right)),
#endif
		Npe(iNpe)
{}

GraphicOutputManager *gr_manager = NULL;
AnalysisManager *manager = NULL;
AllExperimentsResults* g_data = NULL;
PostProcessor* post_processor = NULL;

std::vector<double>::iterator iter_add(std::vector<double>::iterator& to, int what, std::vector<double>::iterator& end)
{
	if (what < 0)
		return end;
	return ((int)(end - to) < what) ? end : to + what;
}

int getIndex(const std::deque<int>& channels, int ch)
{
	for (std::size_t i = 0, i_end_ = channels.size(); i!=i_end_; ++i)
		if (channels[i]==ch)
			return i;
	return -1;
}

int getIndex(const std::vector<int>& channels, int ch)
{
	for (std::size_t i = 0, i_end_ = channels.size(); i!=i_end_; ++i)
		if (channels[i]==ch)
			return i;
	return -1;
}

std::string int_to_str(int num)
{
	return boost::lexical_cast<std::string>(num);
}

std::string int_to_str(std::size_t num)
{
	return boost::lexical_cast<std::string>(num);
}

std::string int_to_str(int num, std::size_t decimals)
{
	std::string out = boost::lexical_cast<std::string>(num);
	if (num < 0) {
		while ((out.size()-1)<decimals) {
			out = "0" + out;
		}
	} else {
		while (out.size()<decimals) {
			out = "0" + out;
		}
	}
	return out;
}

std::string int_to_str(std::size_t num, std::size_t decimals)
{
	std::string out = boost::lexical_cast<std::string>(num);
	while (out.size()<decimals) {
		out = "0" + out;
	}
	return out;
}

std::string dbl_to_str (double val, int precision)
{
	std::stringstream ss;
	ss<<std::fixed<<std::setprecision(precision)<<val;
	return ss.str();
}

std::string strtoken(std::string &in, std::string break_symbs)
{
	std::string out;
	while (!in.empty())
	{
		char a = in.front();
		in.erase(in.begin());
		bool break_ = false;
		for (auto h = break_symbs.begin(); h != break_symbs.end(); ++h)
			if (a == *h) {
				break_ = true;
				break;
			}
		if ((break_) && (out.empty()))
			continue;
		if (break_)
			return out;
		out.push_back(a);
	}
	return out;
}

double fast_pown(double val, unsigned int n)
{
	if (0==val)
		return val;
	double result = 1;
	while (true) {
		if (n & 1)
			result *= val;
		n >>= 1;
		if (!n)
			break;
		val *= val;
	}
	return result;
}

//For threading. Splits [min, max) range into n parts as equal as possible.
std::vector<std::pair<int, int>> split_range(int min, int max, int number)
{
	std::vector<std::pair<int, int>> result;
	if (min == max)
		return result;
	unsigned int n_effective = (number > (max - min) ? (max - min) : number);
	n_effective = (n_effective <= 0 ? 1 : n_effective);
	int N_extra = (max - min) % n_effective;
	std::vector<int> Ns(n_effective, 0);
	for (unsigned int n = 0u; n < n_effective; ++n) { //distribute events among the processes as evenly as possible
		Ns[n] = (max - min) / n_effective + ((N_extra>0) ? 1 : 0);
		--N_extra;
	}
	int current_min = min;
	for (unsigned int n = 0u; n < n_effective; ++n) {
		result.push_back(std::pair<int, int>(current_min, current_min + Ns[n]));
		current_min = current_min + Ns[n];
	}
	return result;
}

TLatex* CreateStatBoxLine (std::string name, double v)
{
	std::string val = Form("%g", v);
	TLatex *myt = new TLatex(0,0, (name + " = " + val).c_str());// " = " is a control character
	myt->SetTextFont(0);
	myt->SetTextSize(0);
	myt->SetTextAlign(0);
	return myt;
}
TLatex* CreateStatBoxLine (std::string name, int v)
{
	std::string val = Form("%d", v);
	TLatex *myt = new TLatex(0,0, (name + " = " + val).c_str());// " = " is a control character
	myt->SetTextFont(0);
	myt->SetTextSize(0);
	myt->SetTextAlign(0);
	return myt;
}
TLatex* CreateStatBoxLine (std::string name)
{
	TLatex *myt = new TLatex(0,0, name.c_str());// " = " is a control character
	myt->SetTextFont(0);
	myt->SetTextSize(0);
	myt->SetTextAlign(0);
	return myt;
}

void test_ROOT_threads(void)
{
	int run_size = 25;
	std::deque<int> results(run_size, -1);
	std::vector<std::pair<int, int>> thread_indices = split_range(0, run_size, threads_number);
	int thread_n = thread_indices.size();
	std::vector<std::thread> threads;
	for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
		threads.push_back(std::thread([&]()
		{
			for (int run = thread_indices[n_th].first; run!=thread_indices[n_th].second; ++run) {
				results[run] = run;
			}
		}));
	}
	for (unsigned int n_th = 0u; n_th < thread_n; ++n_th) {
		threads[n_th].join();
	}
	std::cout<<"test_ROOT_threads finished:"<<std::endl;
	std::cout<<"{";
	for (int run = 0; run!=run_size; ++run) {
		std::cout<<results[run]<<", ";
	}
	std::cout<<"}";
	std::cout<<std::endl;
}

//To use for ShapeFitData
//par[0] - width of rectangle, par[1] - center position
double rectangle_pdf (const double *x, const double *pars)
{
	double w = std::fabs(pars[0]);
	double amplitude = 1.0/w;
	if (x[0] < (pars[1] - 0.5*w) || x[0] > (pars[1] + 0.5*w))
		return 0;
	return amplitude;
}

//par[0] - width of rectangle, par[1] - center position, par[2] - fraction of bkg, (<1 !)
//par[3] - start time for bkg (fix to 0us), par[4] - finish time for bkg (fix to 160us),
double rectangle_bkg_pdf (const double *x, const double *pars)
{
	double dt = std::fabs(pars[4] - pars[3]);
	if (0 == dt)
		return rectangle_pdf(x, pars);
	double w = std::fabs(pars[0]);
	double amp_bkg = std::fabs(pars[2])/dt;
	double amp_sig = 1/w + amp_bkg * (w - dt) / w; // > amp_bkg if fraction pars[2] < 1
	double t1 = std::min(pars[4], pars[3]);
	double t2 = std::max(pars[4], pars[3]);
	if (x[0] < t1 || x[0] > t2)
		return 0.0;
	if (x[0] < (pars[1] - 0.5*w) || x[0] > (pars[1] + 0.5*w))
		return amp_bkg;
	return amp_sig;
}

//par[0] - width of triangle, par[1] - left front position, par[2] - fraction of bkg,
//par[3] - start time for bkg (fix to 0us), par[4] - finish time for bkg (fix to 160us),
double triangle_bkg_pdf (const double *x, const double *pars)
{
	double dt = std::fabs(pars[4] - pars[3]);
	double w = std::fabs(pars[0]);
	double amp_bkg = std::fabs(pars[2])/dt;
	double amp_sig = (2/w + amp_bkg * (w - 2*dt) / w); // > amp_bkg if fraction pars[2] < 1
	double t1 = std::min(pars[4], pars[3]);
	double t2 = std::max(pars[4], pars[3]);
	if (x[0] < t1 || x[0] > t2)
		return 0.0;
	if (x[0] < (pars[1]) || x[0] > (pars[1] + w))
		return amp_bkg;
	return amp_sig + (x[0] - pars[1])*(amp_bkg - amp_sig)/w;
}

//par[0] - width of rectangle, par[1] - center of rectangle, par[2] - fraction(area) of triangle,
//par[3] - width of triangle
double rect_tria_pdf (const double *x, const double *pars)
{
	double w1 = std::fabs(pars[0]);
	double w2 = std::fabs(pars[3]);
	double s = std::fabs(pars[2]);
	if (s == 0 || w2 == 0) {
		double amplitude = 1.0/w1;
		if (x[0] < (pars[1] - 0.5*w1) || x[0] > (pars[1] + 0.5*w1))
			return 0;
		return amplitude;
	}
	double ampl1 = (1 - s) / w1;
	double ampl2 = 2*s / w2;
	if (x[0] >= (pars[1] - 0.5*w1) && x[0] <= (pars[1] + 0.5*w1))
		return ampl1;
	if (x[0] > (pars[1] + 0.5*w1) && x[0] <= (pars[1] + 0.5*w1 + w2)) {
		return ampl2 - ampl2 * (x[0] - (pars[1] + 0.5*w1)) / w2;
	}
	return 0.0;
}

//par[0] - sigma, par[1] - center position, par[2] - fraction of bkg,
//par[3] - start time for bkg (fix to 0us), par[4] - finish time for bkg (fix to 160us),
double gauss_bkg_pdf (const double *x, const double *pars)
{
	double ampl_bkg = std::fabs(pars[2])/(pars[4] - pars[3]);
	double ret = (1 - std::fabs(pars[2])) * ROOT::Math::gaussian_pdf(x[0], pars[0], pars[1]);
	if (x[0] >= pars[3] && x[0] < pars[4])
		ret += ampl_bkg;
	return ret;
}

//par[0] - sigma, par[1] - center position, par[2] - fraction of bkg,
//par[3] - start time for bkg (fix to 0us), par[4] - finish time for bkg (fix to 160us),
//par[5] - 2nd gauss sigma/sigma1 (>1), par[6] - fraction of 2nd gauss
double gauss_gauss_bkg_pdf (const double *x, const double *pars)
{
	if (!(x[0] >= pars[3] && x[0] < pars[4]))
		return 0;
	double ampl_bkg = pars[2]/(pars[4] - pars[3]);
	double ret = (1 - pars[2] - pars[6]) * ROOT::Math::gaussian_pdf(x[0], pars[0], pars[1]);
	ret += (pars[6]) * ROOT::Math::gaussian_pdf(x[0], pars[0]*pars[5], pars[1]);
	ret += ampl_bkg;
	return ret;
}

void open_output_file(std::string name, std::ofstream &str, std::ios_base::openmode _mode)
{
	ensure_file(name);
	str.open(name.c_str(), _mode);
	if (!str.is_open()){
		std::cout << "Failed to open \"" << name << "\"" << std::endl;
	}
}

void ensure_file(std::string fname)
{
	std::string folder = fname;
	while ((folder.back() != '\\') &&(folder.back()!='/') &&!folder.empty())
		folder.pop_back();
	if (!folder.empty())
		folder.pop_back();
	ensure_folder(folder);
}

void ensure_folder(std::string folder)
{
#if defined(__WIN32__)
	if (!folder.empty()) {
		DWORD ftyp = GetFileAttributesA(folder.c_str());
		if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY) || ftyp == INVALID_FILE_ATTRIBUTES) {
			int code = system(("mkdir \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir error: " << GetLastError() << std::endl;
		}
	}
#else
	struct stat st;
	if (-1==stat(folder.c_str(), &st)) {
		int err = errno;
		switch (err) {
		case (EACCES): {
			std::cout<<"Access error"<<std::endl;
			break;
		}
		case (ENAMETOOLONG): {
			std::cout<<"Path is too long"<<std::endl;
			break;
		}
		case (ENOENT) :
		case (ENOTDIR): {
			int code = system(("mkdir -p \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir -p error: " << code << ", " << errno << std::endl;
			break;
		}
		default:{
			std::cout<<"stat(\""<<folder<<"\") returned -1; errno == "<<err<<std::endl;
			break;
		}
		}
	} else {
		if (!S_ISDIR(st.st_mode)) {
			int code = system(("mkdir -p \"" + folder + "\"").c_str());
			if (code)
				std::cout << "mkdir -p error: " << code << std::endl;
		}
	}
#endif //_WIN32__
}

bool confirm_action (std::string action)
{
	std::cout<<"Confirm "<<action<<": y/n"<<std::endl;
	std::string a;
	std::cin>>a;
	if (a=="y"||a=="Y"||a=="yes"||a=="Yes") {
		return true;
	}
	std::cout<<action<<" denied"<<std::endl;
	return false;
}

TestSignalGenerator::TestSignalGenerator(std::string prefix)
{
	double t_from = 0, t_to = 110;
	std::vector<std::string> exp0 = { "10V_exp", "15V_exp" };
	std::vector<int> chs = { 2, 3, 4, 5, 6 }; //PMTs
	for (int i = 32; i <= 48; chs.push_back(i++)); //MPPCs
	TRandom1 *rand = new TRandom1(42);
	boost::random::hellekalek1995 rand2;
	//ROOT::Math::TRandomEngine rand2; - root v6 on windows doest not have GSL libraries. v5 does though.
	std::vector<std::vector<double> > Nphe_signal_avr;
	std::vector<std::vector<double> > Nphe_noise_avr;
	std::vector<std::vector<double> > signal_time;
	for (std::size_t e = 0, e_end_ = exp0.size(); e != e_end_; ++e) { //set expected signal and noise values for each exp and channel
		Nphe_signal_avr.push_back(std::vector<double>());
		Nphe_noise_avr.push_back(std::vector<double>());
		signal_time.push_back(std::vector<double>());
		for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
			if (chs[c] < 6) {
				Nphe_signal_avr[e].push_back(rand->Poisson(12.5 *(1 + e / 4)));
				Nphe_noise_avr[e].push_back(rand->Poisson(2.5));
				signal_time[e].push_back(40 * (1 + e / 4));
				continue;
			}
			if (chs[c] == 6) {
				Nphe_signal_avr[e].push_back(10 * (1 + e / 4));
				Nphe_noise_avr[e].push_back(4);
				signal_time[e].push_back(43 * (1 + e / 4));
				continue;
			}
			Nphe_signal_avr[e].push_back(rand->Poisson((15 + chs[c] / 10))*(1 + e / 3));
			Nphe_noise_avr[e].push_back(rand->Poisson(3 + chs[c] / 30));
			signal_time[e].push_back(30 * (1 + e / 4));
		}
	}
	//peak parameters (A, S, 1phe-2phe distr, signal form, dt peak)
	//per channel. See their meaning in the generation
	std::vector<double> dts(chs.size());
	std::vector<double> dts_sigma(chs.size());
	std::vector<std::pair<double, double> > dts_mm(chs.size());
	std::vector<double> SoA(chs.size()); //not precise, sigma is linear from the smallest S to the largest ones
	std::vector<double> SoA_sigma(chs.size()); //maximum value
	std::vector<double> S1phe(chs.size()); //S1phe is distributed according to gamma. This value is mean a= k= 2.3
	std::vector<boost::random::gamma_distribution<double> > gamma_generators; //each channel has different, but fixed parameters.
	std::vector<double> signal_time_sigma(chs.size()); //noise sigma is fixed at 30.
	std::vector<double> Nphe(chs.size()); //poisson distribution over number of 1phe peak, 2phe peak ...
	for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
		if (chs[c] < 6) {
			dts[c] = std::fabs(rand->Gaus(1.0, 0.1));
			dts_sigma[c] = 0.3;
			dts_mm[c] = std::pair<double, double>(0.3, 2.0);
			SoA[c] = std::fabs(rand->Gaus(1.0, 0.1));
			SoA_sigma[c] = 0.08;
			S1phe[c] = 6;
			gamma_generators.push_back(boost::random::gamma_distribution<double>(2.3, S1phe[c]/2.3)); //(k, theta) E[x] = k*th == S1phe[c]
			signal_time_sigma[c] = 12;
			Nphe[c] = 0.25;
			continue;
		}
		if (chs[c] == 6) {
			dts[c] = std::fabs(rand->Gaus(1.0, 0.1));
			dts_sigma[c] = 0.3;
			dts_mm[c] = std::pair<double, double>(0.3, 2.0);
			SoA[c] = std::fabs(rand->Gaus(1.0, 0.1));
			SoA_sigma[c] = 0.1;
			S1phe[c] = 5;
			gamma_generators.push_back(boost::random::gamma_distribution<double>(2.3, S1phe[c]/2.3)); //(k, theta) E[x] = k*th == S1phe[c]
			signal_time_sigma[c] = 14;
			Nphe[c] = 0.10;
			continue;
		}
		dts[c] = std::fabs(rand->Gaus(0.4, 0.06));
		dts_sigma[c] = 0.2;
		dts_mm[c] = std::pair<double, double>(0.01, 1.0);
		SoA[c] = (1 + rand->Poisson(5.0)) / 6;
		SoA_sigma[c] = (1 + rand->Poisson(5.0)) / 200;
		S1phe[c] = 6;
		gamma_generators.push_back(boost::random::gamma_distribution<double>(2.3, S1phe[c]/2.3)); //(k, theta) E[x] = k*th == S1phe[c]
		signal_time_sigma[c] = 10;
		Nphe[c] = 0.35;
	}
	//generate events into memory
	experiment_area area;
	std::deque<AllRunsResults> data;
	for (std::size_t e = 0, e_end_ = exp0.size(); e != e_end_; ++e) {
		data.push_back(AllRunsResults(area));
		std::size_t event_end_ = (e == 0 ? 21000 : 30000);
		data.back()._valid.resize(event_end_, true);
		for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
			if (chs[c] < 32) {
				data.back().pmt_channels.push_back(chs[c]);
				data.back().pmt_peaks.push_back(std::deque<std::deque<peak> >(event_end_));
				data.back().pmt_S2_integral.push_back(std::vector<double>(event_end_));
			} else {
				data.back().mppc_channels.push_back(chs[c]);
				data.back().mppc_peaks.push_back(std::deque<std::deque<peak> >(event_end_));
				data.back().mppc_Double_Is.push_back(std::vector<double>(event_end_));
			}
		}
		for (std::size_t event = 0; event != event_end_; ++event) {
			bool is_noise = (rand->Uniform() < 0.4);
			bool is_gaus_noise = (rand->Uniform() < 0.3); //time distribution of noise
			std::size_t mppc_index = 0;
			std::size_t pmt_index = 0;
			for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
				int Nphe_ = (is_noise ? rand->Poisson(Nphe_noise_avr[e][c]) : rand->Poisson(Nphe_signal_avr[e][c]));
				bool is_pmt = (chs[c] < 32);
				for (int phe = 0; phe <= Nphe_; ++phe) {
					peak pk;
					pk.t = (is_noise ? (is_gaus_noise ? rand->Gaus(signal_time[e][c], 30) : t_from + rand->Uniform()*(t_to - t_from))
						: rand->Gaus(signal_time[e][c], signal_time_sigma[c]));
					if (pk.t > t_to || pk.t < t_from)
						continue;
					double dt = rand->Gaus(dts[c], dts_sigma[c]);
					pk.left = std::max(pk.t - 0.5*dt, t_from);
					pk.right = std::min(pk.t + 0.5*dt, t_to);
					dt = pk.right - pk.left;
					if (dt > dts_mm[c].second || dt < dts_mm[c].first)
						continue;
					double SoA_ = rand->Gaus(SoA[c], (dt - dts_mm[c].first) *SoA_sigma[c] / (dts_mm[c].second - dts_mm[c].first)); //signal shape factor.
					pk.A = gamma_generators[c](rand2);
					//pk.A = rand2.Gamma(2.3, 2.3 / S1phe[c]);
					pk.S = pk.A*SoA_*dt;
					int Nphe_in_peak = std::max(1 + rand->Poisson(Nphe[c]), 4);
					pk.A *= Nphe_in_peak;
					pk.S *= Nphe_in_peak;
					if (is_pmt)
						data.back().pmt_peaks[pmt_index][event].push_back(pk);
					else
						data.back().mppc_peaks[mppc_index][event].push_back(pk);
				}
				double t_zoom_from = 25, t_zoom_to = 45;
				double Ssum = 0, dt_avr = 0; //average distance between peaks inside zoom;
				unsigned int Npks = 0;
				std::deque<peak> peaks = (is_pmt ? data.back().pmt_peaks[pmt_index][event] : data.back().mppc_peaks[mppc_index][event]);
				std::vector<double> ts;
				for (auto p = peaks.begin(), p_end_ = peaks.end(); p != p_end_; ++p) {
					if (p->t<t_zoom_to && p->t>t_zoom_from) {
						++Npks;
						Ssum += p->S;
						ts.push_back(p->t);
					}
				}
				std::sort(ts.begin(), ts.end());
				for (std::size_t t = 0, t_end_ = ts.size(); t != t_end_; ++t) {
					if (t != 0)
						dt_avr += ts[t] - ts[t - 1];
				}
				dt_avr = (Npks <= 1 ? 0 : dt_avr / (Npks - 1));
				double F = 20 / 15.0 / dt_avr;
				F = F * 1 / (1 + std::exp((F - 0.8) / 0.02));
				F = F * 1 / (1 + std::exp(-1 * (F - 0.1) / 0.02));
				//F is between ~0.1 and ~0.8. dt large - dt small
				if (is_pmt) {
					data.back().pmt_S2_integral[pmt_index][event] = Ssum* (1.1 - std::pow(F, 0.8));
				} else {
					data.back().mppc_Double_Is[mppc_index][event] = Ssum* (1.1 - std::pow(F, 0.4)) *
						(ts.size() > 1 ? (ts.back() - ts.front()) : 0);
				}
				if (is_pmt)
					++pmt_index;
				else
					++mppc_index;
			}
		}
	}
	//write events as in Data_processing 
	for (std::size_t e = 0, e_end_ = exp0.size(); e != e_end_; ++e) {
		data[e]._exp.experiments.push_back(exp0[e]);
		data[e].SaveTo(prefix);
	}
}

//Merges only rectangular (step-like) peaks!
peak TestFastSignalGenerator::MergePeaks(std::deque<peak> &peaks_cluster) // peaks_cluster must be not empty!
{
	peak merged_pk;
	if (peaks_cluster.empty())
		return merged_pk;
	std::deque<peak>::iterator left = std::min_element(peaks_cluster.begin(), peaks_cluster.end(), [](const peak& a, const peak &b)->bool {
		return a.left < b.left;
	});
	std::deque<peak>::iterator right = std::min_element(peaks_cluster.begin(), peaks_cluster.end(), [](const peak& a, const peak &b)->bool {
		return a.right > b.right;
	});
	double t_left = left->left;
	double t_right = right->right;

	merged_pk.left = t_left;
	merged_pk.right = t_right;
	merged_pk.S = 0;
	merged_pk.A = 0;
	merged_pk.t = 0;
	std::deque<std::pair<double, double>> fronts; //necessary to correctly determine amplitude of peaks sum
	for (std::size_t a = 0, a_end_ = peaks_cluster.size(); a != a_end_; ++a) {
		merged_pk.S += peaks_cluster[a].S;
		merged_pk.t += peaks_cluster[a].S * peaks_cluster[a].t;
		fronts.push_back(std::pair<double, double>(peaks_cluster[a].left, peaks_cluster[a].A));
		fronts.push_back(std::pair<double, double>(peaks_cluster[a].right, -peaks_cluster[a].A));
	}
	std::sort(fronts.begin(), fronts.end(), [](const std::pair<double, bool>& a, const std::pair<double, bool> &b)->bool {
		return a.first < b.second;
	});
	double A = 0, A_max = 0;
	for (std::size_t a = 0, a_end_ = fronts.size(); a != a_end_; ++a) {
		A += fronts[a].second;
		if (fronts[a].second > 0) {
			A_max = std::max(A_max, A);
		}
	}
	merged_pk.A = A_max;
	return merged_pk;
}

TestFastSignalGenerator::TestFastSignalGenerator(std::string prefix) //Generate fast component signal in 5 channels (4 separate and thier sum)
//for testing trigger jitter adjustment algorithm performance on ideal (step) signal.
{
	//Single peak characteristics:
	double dt = 0.05; //us
	double A = 1;
	double S = A * dt;
	std::pair<double, double> time(20, 22.6);
	std::pair<std::size_t, std::size_t> Npes (5, 12); //in a single PMT channel (1-4)
	double jitter = 0;
	double max_jitter = 5;
	std::string folder = {"20kV_exp"};
	std::vector<int> chs = {1, 2, 3, 4, 0}; //PMTs
	
	TRandom1 *rand = new TRandom1(42);

	//generate events into memory
	experiment_area area;
	std::deque<AllRunsResults> data;
	data.push_back(AllRunsResults(area));
	std::size_t event_end_ = 10000;
	data.back()._valid.resize(event_end_, true);
	for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) {
		data.back().pmt_channels.push_back(chs[c]);
		data.back().pmt_peaks.push_back(std::deque<std::deque<peak> >(event_end_));
		data.back().pmt_S2_integral.push_back(std::vector<double>(event_end_));
	}
	for (std::size_t event = 0; event != event_end_; ++event) {
		double jitter_offset = rand->Gaus(0, jitter);
		jitter_offset = std::min(jitter_offset, max_jitter);
		jitter_offset = std::max(jitter_offset, -max_jitter);
		for (std::size_t c = 0, c_end_ = chs.size() - 1; c != c_end_; ++c) { //the last channel is a sum of all previous
			std::size_t Nphe_ = Npes.first + rand->Integer(Npes.second - Npes.first + 1);
			for (int phe = 0; phe <= Nphe_; ++phe) {
				peak pk;
				pk.t = rand->Uniform(time.first, time.second) + jitter_offset;
				pk.left = pk.t - 0.5* dt;
				pk.right = pk.t - 0.5* dt;
				pk.A = A;
				pk.S = S;
				data.back().pmt_peaks[c][event].push_back(pk);
				data.back().pmt_peaks[c_end_][event].push_back(pk); //0 channel is a sum of all channels
			}
		}
		for (std::size_t c = 0, c_end_ = chs.size(); c != c_end_; ++c) { //merge superimposed peaks
			std::deque<peak> peaks = data.back().pmt_peaks[c][event];
			data.back().pmt_S2_integral[c][event] = peaks.size();
			std::sort(peaks.begin(), peaks.end(), [](const peak& a, const peak &b)->bool {
				return a.left < b.left;
			});

			std::deque<peak> processed_peaks;
			std::deque<peak> peak_cluster; //superimposed peaks
			double cluster_right_time = 0;
			for (std::size_t pk = 0, pk_end_ = peaks.size(); pk != pk_end_; ++pk) {
				if (peak_cluster.empty()) {
					peak_cluster.push_back(peaks[pk]);
					cluster_right_time = peaks[pk].right;
					continue;
				}
				if (cluster_right_time >= peaks[pk].left) {
					peak_cluster.push_back(peaks[pk]);
					cluster_right_time = std::max(cluster_right_time, peaks[pk].right);
					continue;
				} else {
					peak merged_pk = MergePeaks(peak_cluster);
					processed_peaks.push_back(merged_pk);
					peak_cluster.clear();
					peak_cluster.push_back(peaks[pk]);
					cluster_right_time = peaks[pk].right;
				}
			}
			if (!peak_cluster.empty()) {
				peak merged_pk = MergePeaks(peak_cluster);
				processed_peaks.push_back(merged_pk);
			}
			data.back().pmt_peaks[c][event] = processed_peaks;
		}
	}

	//write events as in Data_processing 
	data.back()._exp.experiments.push_back(folder);
	data.back().SaveTo(prefix);
}

void DrawFileData(std::string name, std::vector<double> xs, std::vector<double> ys,/* ParameterPile::*/DrawEngine de)
{
	if (xs.size() != ys.size()){
		std::cout << "DrawFileData::input data size mismatch" << std::endl;
		return;
	}
	if (de == /*ParameterPile::*/DrawEngine::ROOT_){
		double *xxxs = new double[xs.size()];
		double *yyys = new double[ys.size()];
		for (int h = 0; h < xs.size(); h++){
			xxxs[h] = xs.at(h);
			yyys[h] = ys.at(h);
		}
		TGraph* gr = new TGraph(xs.size(), xxxs, yyys);
		TCanvas* can = new TCanvas(name.c_str(), name.c_str());
		can->SetWindowPosition(100, 150);
		can->Update();
		gr->Draw();
		delete[] xxxs;
		delete[] yyys;
	} else {
		std::string mod_name = name;
		for (int s = 0; s < mod_name.size(); s++)
			if (mod_name[s] == '\\' || mod_name[s] == '/')
				mod_name[s] = '.';
		std::ofstream file;
		open_output_file("temp_gnuplot_files/" + mod_name, file);
		std::cout << "file " << "temp_gnuplot_files/" + mod_name << ".is_open() " << file.is_open() << std::endl;
#if defined(__WIN32__)
		if (!file.is_open())
			std::cout << GetLastError() << std::endl;
#endif //__WIN32__
		for (int h = 0; h < xs.size(); h++)
			file << xs[h] << '\t' << ys[h] << std::endl;
		file.close();
		open_output_file("temp_gnuplot_files/script.sc", file);
		file << "plot '" << /*ParameterPile::*/this_path + "/temp_gnuplot_files/" + mod_name << "' u 1:2" << std::endl;
		file << "pause -1";
		file.close();
		INVOKE_GNUPLOT(this_path + "/temp_gnuplot_files/script.sc");
	}
}

viewRegion::viewRegion(double x_min, double y_min, double x_max, double y_max) //constructs rectangular view region
{
	view_xs.push_back(x_min);
	view_ys.push_back(y_min);
	view_xs.push_back(x_min);
	view_ys.push_back(y_max);
	view_xs.push_back(x_max);
	view_ys.push_back(y_max);
	view_xs.push_back(x_max);
	view_ys.push_back(y_min);
}

viewRegion::viewRegion(std::vector<double> &xs, std::vector<double> &ys) //constructs rectangular from polyline defined by xs,ys
{
	if (xs.size() != ys.size()) {
		std::cout << "viewRegion::viewRegion: Warning x-y size mismatch, points are ignored";
		return;
	}
	view_xs = xs;
	view_ys = ys;
}

viewRegion::viewRegion() //no view region
{}

void viewRegion::view_push(double x, double y)
{
	view_xs.push_back(x);
	view_ys.push_back(y);
}

bool viewRegion::get_view(std::size_t index, double &x, double &y) const
{
	if (index >= view_xs.size())
		return false;
	x = view_xs[index];
	y = view_ys[index];
	return true;
}

bool viewRegion::set_view(std::size_t index, double x, double y)
{
	if (index >= view_xs.size()) {
		view_xs.resize(index+1, x);
		view_ys.resize(index+1, y);
		return true;
	}
	view_xs[index] = x;
	view_ys[index] = y;
	return true;
}

std::size_t viewRegion::get_view_size(void) const
{
	return view_xs.size();
}

void viewRegion::polyline_push(double x, double y)
{
	line_xs.push_back(x);
	line_ys.push_back(y);
}

bool viewRegion::get_polyline(std::size_t ind, double &x, double &y) const
{
	if (ind >= line_xs.size())
		return false;
	x = line_xs[ind];
	y = line_ys[ind];
	return true;
}

bool viewRegion::set_polyline(std::size_t ind, double x, double y)
{
	if (ind >= line_xs.size()) {
		line_xs.resize(ind + 1, x);
		line_ys.resize(ind + 1, y);
		return true;
	}
	line_xs[ind] = x;
	line_ys[ind] = y;
	return true;
}

void viewRegion::set_polyline(const std::vector<double> &x, const std::vector<double> &y)
{
	if (x.size() != y.size()) {
		std::cout << "viewRegion::set_polyline: Warning x-y size mismatch, points are ignored";
		return;
	}
	line_xs = x;
	line_ys = y;
}

std::size_t viewRegion::get_polyline_size(void) const
{
	return line_xs.size();
}

TPolyLine *viewRegion::get_clipped_polyline(void) const
{
	std::vector<double> clipped_xs, clipped_ys;
	ClipToView(line_xs, line_ys, clipped_xs, clipped_ys);
	TPolyLine* ret = new TPolyLine(clipped_xs.size(), &clipped_xs[0], &clipped_ys[0]);
	return ret;
}

void viewRegion::clear_polyline(void)
{
	line_xs.clear();
	line_ys.clear();
}

//0 - not crossed, 1 - crossed, 2 - crossed at ends
//Supports lines with infinite points
//Modified code taken from Garfield++
int viewRegion::LinesCrossed(double x1, double y1, double x2, double y2,
	double u1, double v1, double u2, double v2, double& xc, double& yc) {

	if (!IsValidLine(x1, y1, x2, y2) || !IsValidLine(u1, v1, u2, v2))
		return 0;
	// Set the tolerances.
	double xtol = 1.0e-10 * std::max(std::abs(IsInf(x1) ? 0.0 : x1),
			std::max(std::abs(IsInf(x2) ? 0.0 : x2),
			std::max(std::abs(IsInf(u1) ? 0.0 : u1),
					std::abs(IsInf(u2) ? 0.0 : u2))));
	double ytol = 1.0e-10 * std::max(std::abs(IsInf(y1) ? 0.0 : y1),
			std::max(std::abs(IsInf(y2) ? 0.0 : y2),
			std::max(std::abs(IsInf(v1) ? 0.0 : v1),
					std::abs(IsInf(v2) ? 0.0 : v2))));
	if (xtol <= 0) xtol = 1.0e-10;
	if (ytol <= 0) ytol = 1.0e-10;

	//Check for crossing because one of the endpoints is on both lines.
	if (OnLine(x1, y1, x2, y2, u1, v1)) {
		xc = u1;
		yc = v1;
		return 2;
	} else if (OnLine(x1, y1, x2, y2, u2, v2)) {
		xc = u2;
		yc = v2;
		return 2;
	} else if (OnLine(u1, v1, u2, v2, x1, y1)) {
		xc = x1;
		yc = y1;
		return 2;
	} else if (OnLine(u1, v1, u2, v2, x2, y2)) {
		xc = x2;
		yc = y2;
		return 2;
	}
	//First, handle singularity cases
	if (IsInf(x1) || IsInf(x2)) {
		if (x1 == x2) //checked endpoint crossings already
			return 0;
		//y1==y2 && !IsInf(y1) && !IsInf(y2) === First line is horizontal
		if (IsInf(u1) || IsInf(u2)) //Second line is singular as well
			return 0; //checked endpoint crossings already
		if (IsInf(v1) || IsInf(v2)) { //Second line is singular as well
			//u1==u2 && !IsInf(u1) && !IsInf(u2)
			if (InRange(y1, v1, v2, ytol) && InRange(u1, x1, x2, xtol)) { //lines are orthogonal
				xc = u1;
				yc = y1;
				return 1;
			}
			return 0;
		}
		//Second line is not singular
		if ((y1 + ytol) <= std::min(v1, v2) || (y1 - ytol) >= std::max(v1, v2))
			return 0;
		xc = u1 + (u2 - u1)*(y1 - v1)/(v2 - v1); //v2 == v1 case is considered in OnLine
		if (InRange(xc, x1, x2, xtol)) {
			yc = y1;
			return 1;
		}
		return 0;
	}
	if (IsInf(u1) || IsInf(u2)) {
		if (u1 == u2) //checked endpoint crossings already
			return 0;
		//v1==v2 && !IsInf(v1) && !IsInf(v2) === Second line is horizontal
		if (IsInf(x1) || IsInf(x2)) //First line is singular as well
			return 0; //checked endpoint crossings already
		if (IsInf(y1) || IsInf(y2)) { //First line is singular as well
			//u1==u2 && !IsInf(u1) && !IsInf(u2)
			if (InRange(v1, y1, y2, ytol) && InRange(x1, u1, u2, xtol)) { //lines are orthogonal
				xc = x1;
				yc = v1;
				return 1;
			}
			return 0;
		}
		//First line is not singular
		if ((v1 + ytol) <= std::min(y1, y2) || (v1 - ytol) >= std::max(y1, y2))
			return 0;
		xc = x1 + (x2 - x1)*(v1 - y1)/(y2 - y1); //y2 == y1 case is considered in OnLine
		if (InRange(xc, u1, u2, xtol)) {
			yc = v1;
			return 1;
		}
		return 0;
	}
	if (IsInf(y1) || IsInf(y2)) {
		if (y1 == y2) //checked endpoint crossings already
			return 0;
		//x1==x2 && !IsInf(x1) && !IsInf(x2) === First line is horizontal
		if (IsInf(v1) || IsInf(v2)) //Second line is singular as well
			return 0; //checked endpoint crossings already
		if (IsInf(u1) || IsInf(u2)) { //Second line is singular as well
			//v1==v2 && !IsInf(v1) && !IsInf(v2)
			if (InRange(x1, u1, u2, xtol) && InRange(v1, y1, y2, ytol)) { //lines are orthogonal
				xc = x1;
				yc = v1;
				return 1;
			}
			return 0;
		}
		//Second line is not singular
		if ((x1 + xtol) <= std::min(u1, u2) || (x1 - xtol) >= std::max(u1, u2))
			return 0;
		yc = v1 + (v2 - v1)*(x1 - u1)/(u2 - u1); //u2 == u1 case is considered in OnLine
		if (InRange(yc, y1, y2, ytol)) {
			xc = x1;
			return 1;
		}
		return 0;
	}
	if (IsInf(v1) || IsInf(v2)) {
		if (v1 == v2) //checked endpoint crossings already
			return 0;
		//u1==u2 && !IsInf(u1) && !IsInf(u2) === Second line is horizontal
		if (IsInf(y1) || IsInf(y2)) //First line is singular as well
			return 0; //checked endpoint crossings already
		if (IsInf(x1) || IsInf(x2)) { //First line is singular as well
			//v1==v2 && !IsInf(v1) && !IsInf(v2)
			if (InRange(u1, x1, x2, xtol) && InRange(y1, v1, v2, ytol)) { //lines are orthogonal
				xc = u1;
				yc = y1;
				return 1;
			}
			return 0;
		}
		//First line is not singular
		if ((u1 + xtol) <= std::min(x1, x2) || (u1 - xtol) >= std::max(x1, x2))
			return 0;
		yc = y1 + (y2 - y1)*(u1 - x1)/(x2 - x1); //x2 == x1 case is considered in OnLine
		if (InRange(yc, v1, v2, ytol)) {
			xc = u1;
			return 1;
		}
		return 0;
	}
	// Compute the distances and determinant (dx,dy) x (du,dv).
	double dy = y2 - y1;
	double dv = v2 - v1;
	double dx = x1 - x2;
	double du = u1 - u2;
	double det = dy * du - dx * dv;
	// Check if the lines are parallel (zero determinant).
	if (std::abs(det) < xtol * ytol)
		return 0;
	else {// No special case: compute point of intersection.
		// Solve crossing equations.
		xc = (du * (x1 * y2 - x2 * y1) - dx * (u1 * v2 - u2 * v1)) / det;
		yc = ((-1 * dv) * (x1 * y2 - x2 * y1) + dy * (u1 * v2 - u2 * v1)) / det;
		// Determine if this point is on both lines.
		if (OnLine(x1, y1, x2, y2, xc, yc) && OnLine(u1, v1, u2, v2, xc, yc))
			return 1;
	}
	return 0;
}

// Ported from Garfield (method GRCONV):
// Clip the specified polygon to the view region; return the clipped polygon.
// px: the x-vertices of the polygon
// py: the y-vertices of the polygon
// cx: to contain the x-vertices of the clipped polygon
// cy: to contain the y-vertices of the clipped polygon
void viewRegion::ClipToView(const std::vector<double>& px, const std::vector<double>& py,
                            std::vector<double>& cx, std::vector<double>& cy) const {

  // Get the number and coordinates of the polygon vertices.
  int pN = (int)px.size();

  // Clear the vectors to contain the final polygon.
  cx.clear();
  cy.clear();

  // Set up the view vertices.
  std::vector<double> vx = view_xs, vy = view_ys;

  int vN = (int)vx.size();

  // Do nothing if we have less than 2 points.
  if (pN < 2) return;

  // Loop over the polygon vertices. (Omit last index to avoid unnecessary loop)
  for (int i = 0; i < pN; ++i) {
    std::vector <double> tx, ty;
    // Loop over the view vertices.
    for (int j = 0; j < vN; ++j) {

      // Determine whether this vertex lies on a view edge:
      //  if so add the vertex to the final polygon.
      if (OnLine(vx[j % vN], vy[j % vN], vx[(j + 1) % vN], vy[(j + 1) % vN],
                 px[i], py[i])) {
        // Add the vertex.
        tx.push_back(px[i]);
        ty.push_back(py[i]);
      }

      // Determine whether a corner of the view area lies on this edge:
      //  if so add the corner to the final polygon.
      if (OnLine(px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN],
                 vx[j], vy[j])) {

        // Add the vertex.
        tx.push_back(vx[j]);
        ty.push_back(vy[j]);
      }
    }

    // Loop over the view vertices.
    for (int j = 0; (j < vN)&&(i!=pN-1); ++j) {
		// Check for a crossing with this edge;
		//  if one exists, add the crossing point.
		double xc = 0., yc = 0.;
		if (LinesCrossed(vx[j % vN], vy[j % vN], vx[(j + 1) % vN],
						 vy[(j + 1) % vN], px[i % pN], py[i % pN],
						 px[(i + 1) % pN], py[(i + 1) % pN], xc, yc)) {

		  // Add a vertex.
		  tx.push_back(xc);
		  ty.push_back(yc);
		}
	}
    // Test whether this vertex is inside the view polygon.
   //  If so, add it to the final line.
    bool edge = false;
    if (IsInPolygon(px[i], py[i], vx, vy, edge)) {
	  // Add the view vertex.
	  tx.push_back(px[i]);
	  ty.push_back(py[i]);
	}

    //in case several points are added to tx ty there is need to sort them according to p[j] pv[j+1]
    if (std::fabs(px[(i + 1) % vN] - px[i % vN]) > std::fabs(py[(i + 1) % vN] - py[i % vN])) {
		bool direction = px[(i + 1) % vN] > px[i % vN];
		while (tx.size()!=0) {
			std::size_t first_point = 0;
			for (std::size_t b=0, b_end_ = tx.size(); b!=b_end_; ++b) { //can modify it to splitting polyline into several
				if ((direction && tx[first_point] > tx[b])||(!direction && tx[first_point] < tx[b]))
					first_point = b;
			}
			cx.push_back(tx[first_point]);
			cy.push_back(ty[first_point]);
			tx.erase(tx.begin()+first_point);
			ty.erase(ty.begin()+first_point);
		}
    } else {
    	bool direction = py[(i + 1) % vN] > py[i % vN];
		while (tx.size()!=0) {
			std::size_t first_point = 0;
			for (std::size_t b=0, b_end_ = tx.size(); b!=b_end_; ++b) { //can modify it to splitting polyline into several
				if ((direction && ty[first_point] > ty[b])||(!direction && ty[first_point] < ty[b]))
					first_point = b;
			}
			cx.push_back(tx[first_point]);
			cy.push_back(ty[first_point]);
			tx.erase(tx.begin()+first_point);
			ty.erase(ty.begin()+first_point);
		}
    }
  }
}

// Modified code taken from Garfield++
// Returns true if the point (x,y) is inside of the specified polygon (or on its edge).
// Supports polygon with edges going to the infinity (untested)
// x: the x-coordinate
// y: the y-coordinate
// px: the x-vertices of the polygon
// py: the y-vertices of the polygon
// edge: a variable set to true if the point is located on the polygon edge
bool viewRegion::IsInPolygon(double x, double y, const std::vector<double>& px, const std::vector<double>& py, bool& edge) {
	// Get the number and coordinates of the polygon vertices.
	std::size_t pN = px.size();
	// Handle the special case of less than 2 vertices.
	if (pN < 2) return false;
	// Handle the special case of exactly 2 vertices (a line).
	if (pN == 2) return OnLine(px[0], py[0], px[1], py[1], x, y);
	double px_average = 0, py_average = 0;
	std::size_t px_finite_num = 0, py_finite_num = 0;
	for (std::size_t i = 0; i < pN; ++i) {
		if (!IsInf(px[i])) {
			++px_finite_num;
			px_average += px[i];
		}
		if (!IsInf(py[i])) {
			++py_finite_num;
			py_average += py[i];
		};
	}
	px_average = (px_finite_num == 0 ? 0 : px_average/px_finite_num);
	py_average = (py_finite_num == 0 ? 0 : py_average/py_finite_num);
	// Set the minimum and maximum coordinates of all polygon vertices.
	double px_min = DBL_MAX, py_min = DBL_MAX;
	double px_max = -DBL_MAX, py_max = -DBL_MAX;
	double px_true_min = DBL_MAX, py_true_min = DBL_MAX;
	double px_true_max = -DBL_MAX, py_true_max = -DBL_MAX;
	for (std::size_t i = 0; i < pN; ++i) {
		px_min = std::min(px_min, IsInf(px[i]) ? DBL_MAX : px[i] - px_average);
		py_min = std::min(py_min, IsInf(py[i]) ? DBL_MAX : py[i] - py_average);
		px_max = std::max(px_max, IsInf(px[i]) ? -DBL_MAX : px[i] - px_average);
		py_max = std::max(py_max, IsInf(py[i]) ? -DBL_MAX : py[i] - py_average);
		px_true_min = std::min(px_true_min, px[i]);
		py_true_min = std::min(py_true_min, py[i]);
		px_true_max = std::max(px_true_max, px[i]);
		py_true_max = std::max(py_true_max, py[i]);
	}
	// Set the tolerances
	double xtol = 1.0e-10 * std::max(std::abs(px_min), std::abs(px_max));
	double ytol = 1.0e-10 * std::max(std::abs(py_min), std::abs(py_max));
	if (xtol <= 0) xtol = 1.0e-10;
	if (ytol <= 0) ytol = 1.0e-10;
	// If we have essentially one x value, check to see if y is in range.
	if (std::abs(px_max - px_min) < xtol && px_max == px_true_max && px_min == px_true_min) {
		edge = ((y + ytol) > px_true_min && (y - ytol) < py_true_max &&
				std::abs(px_max + px_min + 2 * px_average  - 2 * x) < xtol);
		return edge;
	}
	// If we have essentially one y value, check to see if x is in range.
	if (std::abs(py_max - py_min) < ytol  && py_max == py_true_max && py_min == py_true_min) {
		edge = ((x + xtol) > px_true_min && (x - xtol) < px_true_max &&
				std::abs(py_max + py_min + 2 * py_average- 2 * y) < ytol);
		return edge;
	}
	//Need 4 counters to account for polygon points going to the infinity
	std::size_t nXcross_left = 0, nXcross_right = 0, nYcross_up = 0, nYcross_down = 0;

    for (int i = 0; i < pN; ++i) {
    	if (OnLine(px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN], x, y)) {
			edge = true;
			return true;
    	}
		// Determine whether this edge is crossed; if so increment the counter.
		double xc = 0., yc = 0.;
		int cross_type = LinesCrossed(x, y, DBL_MAX, y, px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN], xc, yc);
		if (cross_type) {
			if (1 == cross_type) {
				nXcross_right += 1;
			} else {
				if (xc == px[(i + 1) % pN] && yc == py[(i + 1) % pN]) {
					if ((yc - py[i])*(py[(i + 2) % pN] - yc) > 0)
						nXcross_right += 1;
				} else {
					if (xc != px[i] || yc != py[i])
						nXcross_right += 1;
				}
			}
		}
		cross_type = LinesCrossed(x, y, -DBL_MAX, y, px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN], xc, yc);
		if (cross_type) {
			if (1 == cross_type) {
				nXcross_left += 1;
			} else {
				if (xc == px[(i + 1) % pN] && yc == py[(i + 1) % pN]) {
					if ((yc - py[i])*(py[(i + 2) % pN] - yc) > 0)
						nXcross_left += 1;
				} else {
					if (xc != px[i] || yc != py[i])
						nXcross_left += 1;
				}
			}
		}
		cross_type = LinesCrossed(x, y, x, DBL_MAX, px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN], xc, yc);
		if (cross_type) {
			if (1 == cross_type) {
				nYcross_up += 1;
			} else {
				if (xc == px[(i + 1) % pN] && yc == py[(i + 1) % pN]) {
					if ((xc - px[i])*(px[(i + 2) % pN] - xc) > 0)
						nYcross_up += 1;
				} else {
					if (xc != px[i] || yc != py[i])
						nYcross_up += 1;
				}
			}
		}
		cross_type = LinesCrossed(x, y, x, -DBL_MAX, px[i % pN], py[i % pN], px[(i + 1) % pN], py[(i + 1) % pN], xc, yc);
		if (cross_type) {
			if (1 == cross_type) {
				nYcross_down += 1;
			} else {
				if (xc == px[(i + 1) % pN] && yc == py[(i + 1) % pN]) {
					if ((xc - px[i])*(px[(i + 2) % pN] - xc) > 0)
						nYcross_down += 1;
				} else {
					if (xc != px[i] || yc != py[i])
						nYcross_down += 1;
				}
			}
		}
	}
	// Point is inside for an odd, nonzero number of crossings.
	return ((nYcross_up + nYcross_down + nXcross_left + nXcross_right) >=2) &&
			((nYcross_up % 2)||(nYcross_down % 2)||(nXcross_left % 2)||(nXcross_right % 2));
}

//
// Determines whether the point (u,v) lies on the line connecting
//  points (x1,y1) and (x2,y2).
//
//  Supports horizontal/vertical lines going to the infinity (+-DBL_MAX)
//  Modification of code taken from Garfield++
bool viewRegion::OnLine(double x1, double y1, double x2, double y2, double u, double v) {
	//Check that input line is valid
	if (!IsValidLine(x1, y1, x2, y2))
		return false;
	// Set the tolerances
	double xtol = 1.0e-10 * std::max(std::abs(IsInf(x1) ? 0.0 : x1), std::max(std::abs(IsInf(x2) ? 0.0 : x2), std::abs(IsInf(u) ? 0.0 : u)));
	double ytol = 1.0e-10 * std::max(std::abs(IsInf(y1) ? 0.0 : y1), std::max(std::abs(IsInf(y2) ? 0.0 : y2), std::abs(IsInf(v) ? 0.0 : v)));
	if (xtol <= 0) xtol = 1.0e-10;
	if (ytol <= 0) ytol = 1.0e-10;
	//First, handle singularity cases
	if (IsInf(x1) || IsInf(x2)) {
		if (x1 == x2)
			return (u == x1 && InRange(v, y1, y2, ytol));
		return std::abs(y1 - v) <= ytol && InRange(u, x1, x2, xtol);
	}
	if (IsInf(y1) || IsInf(y2)) {
		if (y1 == y2)
			return (v == y1 && InRange(u, x1, x2, xtol));
		return std::abs(x1 - u) <= xtol && InRange(v, y1, y2, ytol);
	}
	if (IsInf(u) || IsInf(v))
		return false;
	//Now all points are not singular
	//To store the coordinates of the comparison point
	double xc = 0, yc = 0;

	// Check if (u,v) is the point (x1,y1) or (x2,y2)
	if ((std::abs(x1 - u) <= xtol && std::abs(y1 - v) <= ytol) || (std::abs(x2 - u) <= xtol && std::abs(y2 - v) <= ytol))
		return true;
	else {
		if (std::abs(x1 - x2) <= xtol && std::abs(y1 - y2) <= ytol) // Check if the line is actually a point
			return false;
		else {
			if (std::abs(u - x1) + std::abs(v - y1) < std::abs(u - x2) + std::abs(v - y2)) { // Choose (x1,y1) as starting point if closer to (u,v)
				// Compute the component of the line from (x1,y1) to (u,v)
				//  along the line from (x1,y1) to (x2,y2)
				double dpar = ((u - x1) * (x2 - x1) + (v - y1) * (y2 - y1)) /
							  ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				// Determine the point on the line to which to compare (u,v)
				if (dpar < 0.0) {
					xc = x1;
					yc = y1;
				} else {
					if (dpar > 1.0) {
						xc = x2;
						yc = y2;
					} else {
						xc = x1 + dpar * (x2 - x1);
						yc = y1 + dpar * (y2 - y1);
					}
				}
			} else { // Choose (x2,y2) as starting point if closer to (u,v)
				// Compute the component of the line from (x2,y2) to (u,v)
				//  along the line from (x2,y2) to (x1,y1)
				double dpar = ((u - x2) * (x1 - x2) + (v - y2) * (y1 - y2)) /
							  ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				// Determine the point on the line to which to compare (u,v)
				if (dpar < 0.0) {
					xc = x2;
					yc = y2;
				} else {
					if (dpar > 1.0) {
						xc = x1;
						yc = y1;
					} else {
						xc = x2 + dpar * (x1 - x2);
						yc = y2 + dpar * (y1 - y2);
					}
				}
			}
		}
	}
	// Compare the calculated point to (u,v)
	if (std::abs(u - xc) < xtol && std::abs(v - yc) < ytol)
		return true;
	return false;
}

//TODO: tolerances should be here as well (e.g. y1!=y2 condition when x1 is infinite)
bool viewRegion::IsValidLine(double x1, double y1, double x2, double y2) {
	if (IsInf(x1))
		if ((x1 != x2) && (y1 != y2 || IsInf(y1) || IsInf(y2)))
			return false;
	if (IsInf(x2))
		if ((x1 != x2) && (y1 != y2 || IsInf(y1) || IsInf(y2)))
			return false;
	if (IsInf(y1))
		if ((y1 != y2) && (x1 != x2 || IsInf(x1) || IsInf(x2)))
			return false;
	if (IsInf(y2))
		if ((y1 != y2) && (x1 != x2 || IsInf(x1) || IsInf(x2)))
			return false;
	return true;
}

bool IsInf(double val) {
	return val == DBL_MAX || val == -DBL_MAX;
}

bool viewRegion::InRange(double val, double a, double b, double tolerance) {
	double max = std::max(a, b);
	double min = std::min(a, b);
	if (IsInf(val))
		return max == val || min == val;
	return val + tolerance >= min && val - tolerance <= max;
}

bool viewRegion::RangesIntersect(double x1, double x2, double x3, double x4, double tolerance, double &p) {
	double r1 = std::min(x1, x2);
	double r2 = std::max(x1, x2);
	double r3 = std::min(x3, x4);
	double r4 = std::max(x3, x4);
	double comp1 = IsInf(r1) ? r1 : r1 - tolerance;
	double comp2 = IsInf(r2) ? r2 : r2 + tolerance;
	double comp3 = IsInf(r3) ? r3 : r3 - tolerance;
	double comp4 = IsInf(r4) ? r4 : r4 + tolerance;
	if (comp3 <= r2 && r2 <= comp4) {
		p = r2;
		return true;
	}
	if (comp1 <= r3 && r3 <= comp2) {
		p = r3;
		return true;
	}
	return false;
}

std::deque <experiment_area> areas_to_draw;
std::string this_path;

std::string data_prefix_path;
std::string calibration_file;
std::string data_output_path;

std::string DATA_MPPC_VERSION;
std::string DATA_PMT_VERSION;

experiment_area exp_area;
int threads_number = 8; //obv. must be >=1

int gnuplot_pad_size = 400;
int gnuplot_max_size = 1600;
int gnuplot_width = 900; //default for gnuplot is 640

TriggerVersion trigger_version;
NamingScheme name_scheme_version;
std::map < std::string, double > experiment_fields;
std::map < std::string, double > PMT_V;
std::map < std::string, double > MPPC_V;
std::map < std::string, channel_info<dB_info> > dBs;
std::pair<int, int> calibaration_points;
std::map < int, std::pair<double,double> > MPPC_coords;


std::string Vdrift_data_fname = "ArDriftVel.txt";
DataVector Vdrift(4, 5); //e drift speed in gaseous Ar as a function of Td (in m/s)
const double bolzmann_SI = 1.38064852e-23; //SI
const double Td_is_Vcm2 = 1e-17; //1 Townsend = 1e-17 V*cm^2
const double LAr_epsilon = 1.54; //doi: 10.1016/j.nima.2019.162431
double full_gap_length = 2.2; //cm, the distance between THGEM0 and THGEM1
double R3 = 10; //MOhm, THGEM0 resistance. 4 MOhm in experiments before ~ Feb 2019.
double Rgap = 600; //MOhm, resistance defining E field in EL gap
double Rrest = 200; //MOhm
const double T = 87; //temperature in K
const double P = 1.015e5; //pressure in Pa

//LAr_layer must be in [0, full_gap_length]
double gasE_from_kV (double kV, double gasAr_layer) //gasAr_layer is in [cm], returns E in gaseous Ar in [V/cm]
{
	if (gasAr_layer < 0)
		gasAr_layer = 0;
	if (gasAr_layer > full_gap_length)
		gasAr_layer = full_gap_length;
	return 1e3*kV*(Rgap / (Rgap + Rrest + R3))*LAr_epsilon / (LAr_epsilon*gasAr_layer + (full_gap_length - gasAr_layer));
}

double Td_from_E (double E) //E is in [V/cm]
{
	double N = 1e-6*P / (T*bolzmann_SI); //in cm^-3
	return E / N / Td_is_Vcm2;
}

double E_from_Td(double Td) //E is in [V/cm]
{
	double N = 1e-6*P / (T*bolzmann_SI); //in cm^-3
	return Td * N * Td_is_Vcm2;
}

double Vdr_from_E (double E) //E is in [V/cm], returns drift velocity in gaseous Ar in [cm/s]
{
	double Td = Td_from_E(E);
	Td = std::fabs(Td);
	double Vdr = 1e2 * Vdrift(Td);
	return Vdr;
}

double Vdr_from_kV (double kV, double gasAr_layer) //LAr_layer is in [cm], returns drift velocity in gaseous Ar in [cm/s]
{
	if (!Vdrift.isValid()) {
		std::ifstream str;
		str.open(Vdrift_data_fname);
		if (!str.is_open()) {
			std::cerr << "Error: Failed to open file with V drfit data \"" << Vdrift_data_fname << "\"!" << std::endl;
			return DBL_MAX;
		}
		Vdrift.read(str);
	}
	if (!Vdrift.isValid()) {
		std::cerr << "Error: Failed to load file with V drfit data \"" << Vdrift_data_fname << "\"!" << std::endl;
		return DBL_MAX;
	}
	double E = gasE_from_kV(kV, gasAr_layer);
	return Vdr_from_E(E);
}

double drift_time_from_kV(double kV, double gasAr_layer) //gasAr_layer is in [cm], returns drift time in gaseous Ar in [microseconds]
{
	double Vdr = Vdr_from_kV(kV, gasAr_layer);
	if (DBL_MAX == Vdr)
		return 0;
	return 1e6*gasAr_layer / Vdr;
}

Bool_t draw_required(/*ParameterPile::*/experiment_area what)
{
	for (auto i = areas_to_draw.begin(); i != areas_to_draw.end(); ++i)
		if (i->contains(what))
			return kTRUE;
	return kFALSE;
}

void Init_globals(bool full)
{
	char path[FILENAME_MAX];
#if defined(__WIN32__)
	this_path = _getcwd(path, FILENAME_MAX);
#else
	this_path = getcwd(path, FILENAME_MAX);
#endif //__WIN32__
	TThread::Initialize();
	gStyle->SetOptFit();
	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.9);
	//ROOT::EnableThreadSafety();
	calibaration_points = std::pair<int, int>(3, 6);

	double SiPM_size = 10; //mm
	//<x,y>
	MPPC_coords.clear();
	MPPC_coords[32]= std::pair<double,double>(-2,-2);
	MPPC_coords[33]= std::pair<double,double>(-1,-2);
	MPPC_coords[34]= std::pair<double,double>(2,-2);
	MPPC_coords[35]= std::pair<double,double>(-2,-1);
	MPPC_coords[36]= std::pair<double,double>(1,-1);
	MPPC_coords[37]= std::pair<double,double>(2,-1);
	MPPC_coords[38]= std::pair<double,double>(0,0);
	MPPC_coords[39]= std::pair<double,double>(1,0);
	MPPC_coords[40]= std::pair<double,double>(-1,1);
	MPPC_coords[41]= std::pair<double,double>(0,1);
	MPPC_coords[42]= std::pair<double,double>(-2,2);
	MPPC_coords[43]= std::pair<double,double>(-1,2);
	MPPC_coords[44]= std::pair<double,double>(2,2);
	MPPC_coords[48]= std::pair<double,double>(0,-2);
	MPPC_coords[49]= std::pair<double,double>(1,-2);
	MPPC_coords[50]= std::pair<double,double>(-1,-1);
	MPPC_coords[51]= std::pair<double,double>(0,-1);
	MPPC_coords[52]= std::pair<double,double>(-2,0);
	MPPC_coords[53]= std::pair<double,double>(-1,0);
	MPPC_coords[54]= std::pair<double,double>(2,0);
	MPPC_coords[55]= std::pair<double,double>(-2,1);
	MPPC_coords[56]= std::pair<double,double>(1,1);
	MPPC_coords[57]= std::pair<double,double>(2,1);
	MPPC_coords[58]= std::pair<double,double>(0,2);
	MPPC_coords[59]= std::pair<double,double>(1,2);
	for (auto i = MPPC_coords.begin();i!=MPPC_coords.end();++i){
		i->second.first *=SiPM_size;
		i->second.second*= -SiPM_size;
	}

	if (!full)
		return;
	trigger_version = trigger_v2;
	name_scheme_version = name_scheme_v1;
	if (areas_to_draw.empty())
		areas_to_draw.push_back(experiment_area());
	areas_to_draw.back().channels.erase();
	areas_to_draw.back().runs.erase();
	areas_to_draw.back().sub_runs.erase();
	areas_to_draw.back().runs.push_pair(0, 0);
	areas_to_draw.back().channels.push_pair(0, 1);
	areas_to_draw.back().channels.push_pair(32, 62);
	areas_to_draw.back().sub_runs.push_pair(0, 0);

	exp_area.channels.erase();
	exp_area.runs.erase();
	exp_area.sub_runs.erase();
	exp_area.channels.push_pair(0, 1);
	exp_area.channels.push_pair(8, 8);
	exp_area.channels.push_pair(12,12);
	//exp_area.channels.push_pair(2, 2);
	exp_area.channels.push_pair(32, 62);//will load only present channels
	exp_area.runs.push_pair(0, 0);
	exp_area.sub_runs.push_pair(0, 0);


	data_prefix_path = "../Data/180215/results/";
	calibration_file = "PMT_SiPM_48V_180215.dat";
	data_output_path = "180215/results/";

	if (name_scheme_version == name_scheme_v1) {
		DATA_MPPC_VERSION = "MPPCs_v1";
		DATA_PMT_VERSION = "PMT_v1";
	} else {
		DATA_MPPC_VERSION = "SiPM";
		DATA_PMT_VERSION = "PMT";
	}

	PMT_V.clear();
	PMT_V["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
	PMT_V["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
	PMT_V["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
	PMT_V["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
	PMT_V["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;
	PMT_V["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 750;

	channel_info<dB_info> atten0;
	atten0.push(0, dB_info(12));
	dBs.clear();
	dBs["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = atten0;
	dBs["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = atten0;

	experiment_fields.clear();
	experiment_fields["X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 6;
	experiment_fields["X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 8;
	experiment_fields["X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 10;
	experiment_fields["X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 11;
	experiment_fields["X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 12;
	experiment_fields["X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray"] = 14;


	areas_to_draw.back().experiments.clear();
	areas_to_draw.back().experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	areas_to_draw.back().experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	areas_to_draw.back().experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	areas_to_draw.back().experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	areas_to_draw.back().experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	areas_to_draw.back().experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");


	exp_area.experiments.clear();
	exp_area.experiments.push_back("X-ray_6kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	exp_area.experiments.push_back("X-ray_8kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	exp_area.experiments.push_back("X-ray_10kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	exp_area.experiments.push_back("X-ray_11kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	exp_area.experiments.push_back("X-ray_12kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
	exp_area.experiments.push_back("X-ray_14kV_PMT_SiPM_48V_THGEM_0V_coll_6mm_trig_xray");
}
