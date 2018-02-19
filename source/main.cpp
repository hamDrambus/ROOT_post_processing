#include "GlobalParameters.h"
#include "AnalysisManager.h"
#include "GraphicOutputManager.h"
#include "PostProcessor.h"

//interactive methods
void Initialize(void);
void Ne(bool save_state = true); //Next (E == experiment ==field)
void Pe(bool save_state = true); //Previous (E == experiment ==field)
void Nch(bool save_state = true); //Next channel
void Pch(bool save_state = true); //Previous channel
void GotoCh(int ch, bool save_state = true); //go to specific channel
void Nt(bool save_state = true); //Next type (Ss, S2_S, double_intergral)
void Pt(bool save_state = true); //Prev type (Ss, S2_S, double_intergral)
void update();
void status(bool full = false);// displays current state
void state(bool full); // displays current state
void set_fit_1_gauss(void);
void set_fit_2_gauss(void);
void set_fit_3_gauss(void);
void set_fit_gauss(int N);
void set_parameter_val(int index, double val);
void set_parameter_limits(int index, double left, double right);
void do_fit(bool update_vis);
void draw_limits(double left, double right);
void impose_limits(double left, double right);
void unset_limits(void);
void next_canvas(void); //creates new canvas, so the current one will stay unchanged
void set_calib_N(int from, int to);//in order to set default use invalid values
void draw_Npe(void);
void add_1peS(void);
void add_2peS(void);
void rem_1peS(void);
void rem_2peS(void);
void set_1peS(double val);
void unset_1peS(void);
void use_mean(bool do_use = true); //uses mean value of data instead of gauss' mean. May be usefull for S2_S
void set_N_bin(int n);

void Initialize(void)
{
	ParameterPile::Init_globals();
	manager = new AnalysisManager(ParameterPile::exp_area);
	manager->processAllExperiments();
	g_data = manager->getAllExperimentsResults();
	post_processor = new PostProcessor(g_data);
	gr_manager = new GraphicOutputManager();

	do_fit(true);
}

void Ne(bool save_state) //Next (E == experiment ==field)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->NextExp(save_state);
}
void Pe(bool save_state) //Previous (E == experiment ==field)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->PrevExp(save_state);
}
void Nch(bool save_state) //Next channel
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->NextCh(save_state);
}
void Pch(bool save_state) //Previous channel
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->PrevCh(save_state);
}

void GotoCh(int ch, bool save_state) //go to specific channel
{
	if (NULL == g_data){
		state(false);
		return;
	}
	std::cout << "TODO: not implemented" << std::endl;
}

void Nt(bool save_state) //Next type (Ss, S2_S, double_intergral)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->NextType(save_state);
}

void Pt(bool save_state) //Prev type (Ss, S2_S, double_intergral)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->PrevType(save_state);
}

void update()
{
	if (NULL == post_processor){
		state(false);
		return;
	}
	post_processor->update();
}

void status(bool full){// displays current state
	state(full);
}

void state(bool full) // displays current state
{
	if (NULL == manager){
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	if (NULL == g_data){
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	if (NULL == post_processor) {
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	post_processor->status(full);
}

void set_fit_1_gauss(void)
{
	if (NULL == post_processor){
		state(false);
		return;
	}
	post_processor->set_fit_gauss(1);
}

void set_fit_2_gauss(void)
{
	if (NULL == post_processor){
		state(false);
		return;
	}
	post_processor->set_fit_gauss(2);
}

void set_fit_3_gauss(void)
{
	if (NULL == post_processor){
		state(false);
		return;
	}
	post_processor->set_fit_gauss(3);
}

void set_fit_gauss(int N)
{
	if (NULL == post_processor){
		state(false);
		return;
	}
	post_processor->set_fit_gauss(N);
}

void set_parameter_val(int index, double val)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->set_parameter_val(index, val);
}

void set_parameter_limits(int index, double left, double right)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->set_parameter_limits(index, left, right);
}

void do_fit(bool update_vis)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->do_fit();
}

void draw_limits(double left, double right)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->set_drawn_limits(left, right);
}

void impose_limits(double left, double right)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->set_limits(left, right);
}

void unset_limits(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->unset_limits();
}

void next_canvas(void) //creates new canvas, so the current one will stay unchanged
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->new_canvas();
}

void set_calib_N(int from, int to)//in order to set default use invalid values
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->calibr_info.set_N_calib(post_processor->current_channel, from, to);
}

void draw_Npe(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->plot_N_pe(post_processor->current_channel, gr_manager);
}
void add_1peS(void)
{
	if (NULL == g_data){
		state(false);
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
	if (NULL == g_data){
		state(false);
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
	if (NULL == g_data){
		state(false);
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
	if (NULL == g_data){
		state(false);
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
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->calibr_info.setS1pe(post_processor->current_channel,val);
}
void unset_1peS(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->calibr_info.unsetS1pe(post_processor->current_channel);
}
void use_mean(bool do_use) //uses mean value of data instead of gauss' mean. May be usefull for S2_S
{
	if (NULL == g_data){
		state(false);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
	if (do_use)
		prev_method = CalibrationInfo::UsingMean;
	if ((!do_use) && (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
}
void set_N_bin(int n)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->set_N_bins(n);
}
//interactive methods

int main(int argc, char *argv[])
{
	//ParameterPile::Init_globals();
	int n_par = 0;
	char **f = NULL;
	TApplication* app = new TApplication("test_app",&n_par,f);
	TCanvas* c1 = new TCanvas("test", "test_title", 800, 500);
	TF1 *func = new TF1("test_func", "sin(x)+3*x", 0, 10);
	func->Draw();
	
	//SIMULATING SCRIPT
	Initialize();
	state(true);
	Ne(false);
	do_fit(true);
	set_fit_1_gauss();
	do_fit(true);
	add_1peS();
	state(true);
	//draw_Npe();
	//next_canvas();
	Nt(true);
	set_fit_1_gauss();
	do_fit(true);
	//END OF SIMULATING SCRIPT
	std::cout<<std::endl<<"========================================="<<std::endl << "Finished" << std::endl;

	app->Run();
	delete app;
	return 0;
}
