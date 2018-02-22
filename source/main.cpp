#include "main.h"
//Int_teractive methods

void Initialize(void)
{
	/*ParameterPile::*/Init_globals();
	std::cout << "This: " <</* ParameterPile::*/this_path << std::endl;
	manager = new AnalysisManager(/*ParameterPile::*/exp_area);
	manager->processAllExperiments();
	g_data = manager->getAllExperimentsResults();
	post_processor = new PostProcessor(g_data);
	gr_manager = new GraphicOutputManager();

	//do_fit(kTRUE);
}

void Ne(Bool_t save_state) //Next (E == experiment ==field)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->NextExp(save_state);
}
void Pe(Bool_t save_state) //Previous (E == experiment ==field)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->PrevExp(save_state);
}
void Nch(Bool_t save_state) //Next channel
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->NextCh(save_state);
}
void Pch(Bool_t save_state) //Previous channel
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->PrevCh(save_state);
}

void GotoCh(Int_t ch, Bool_t save_state) //go to specific channel
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	std::cout << "TODO: not implemented" << std::endl;
}

void Nt(Bool_t save_state) //Next type (Ss, S2_S, Double_Int_tergral)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->NextType(save_state);
}

void Pt(Bool_t save_state) //Prev type (Ss, S2_S, Double_Int_tergral)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->PrevType(save_state);
}

void update()
{
	if (NULL == post_processor){
		state(kFALSE);
		return;
	}
	post_processor->update();
}

void status(Bool_t full){// displays current state
	state(full);
}

void state(Bool_t full) // displays current state
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
		state(kFALSE);
		return;
	}
	post_processor->set_fit_gauss(1);
}

void set_fit_2_gauss(void)
{
	if (NULL == post_processor){
		state(kFALSE);
		return;
	}
	post_processor->set_fit_gauss(2);
}

void set_fit_3_gauss(void)
{
	if (NULL == post_processor){
		state(kFALSE);
		return;
	}
	post_processor->set_fit_gauss(3);
}

void set_fit_gauss(Int_t N)
{
	if (NULL == post_processor){
		state(kFALSE);
		return;
	}
	post_processor->set_fit_gauss(N);
}

void set_parameter_val(Int_t index, Double_t val)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->set_parameter_val(index, val);
}

void set_parameter_limits(Int_t index, Double_t left, Double_t right)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->set_parameter_limits(index, left, right);
}

void do_fit(Bool_t update_vis)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->do_fit();
}

void draw_limits(Double_t left, Double_t right)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->set_drawn_limits(left, right);
}

void impose_limits(Double_t left, Double_t right)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->set_limits(left, right);
}

void unset_limits(void)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->unset_limits();
}

void next_canvas(void) //creates new canvas, so the current one will stay unchanged
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->new_canvas();
}

void set_calib_N(Int_t from, Int_t to)//in order to set default use invalid values
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.set_N_calib(post_processor->current_channel, from, to);
}

void draw_Npe(void)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->plot_N_pe(post_processor->current_channel, gr_manager);
}
void add_1peS(void)
{
	if (NULL == g_data){
		state(kFALSE);
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
		state(kFALSE);
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
		state(kFALSE);
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
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
	if (prev_method == CalibrationInfo::Using1pe2pe)
		prev_method = CalibrationInfo::Using1pe;
	if ((prev_method == CalibrationInfo::Using2pe) || (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
}

void set_1peS(Double_t val)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.setS1pe(post_processor->current_channel,val);
}
void unset_1peS(void)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->calibr_info.unsetS1pe(post_processor->current_channel);
}
void use_mean(Bool_t do_use) //uses mean value of data instead of gauss' mean. May be usefull for S2_S
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	CalibrationInfo::S1pe_method prev_method = post_processor->calibr_info.get_method(post_processor->current_exp_index, post_processor->current_channel);
	if (do_use)
		prev_method = CalibrationInfo::UsingMean;
	if ((!do_use) && (prev_method == CalibrationInfo::UsingMean))
		prev_method = CalibrationInfo::Ignore;
	post_processor->calibr_info.set_method(post_processor->current_exp_index, post_processor->current_channel, prev_method);
}
void set_N_bin(Int_t n)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->set_N_bins(n);
}

void Exit(Bool_t save)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	if (save)
		post_processor->save_all();
}

void add_hist_cut(FunctionWrapper *picker, std::string name)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->add_hist_cut(picker, name);
}
void remove_hist_cut(std::string name)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->remove_hist_cut(name);
}
void set_as_run_cut(std::string name)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->set_as_run_cut(name);
}
void unset_as_run_cut(std::string name)
{
	if (NULL == g_data){
		state(kFALSE);
		return;
	}
	post_processor->unset_as_run_cut(name);
}

//Int_teractive methods

Int_t main(Int_t argc, char *argv[])
{
	//ParameterPile::Init_globals();
	Int_t n_par = 0;
	char **f = NULL;
	TRint* app = new TRint("test_app",&argc,argv,NULL,0);
	TCanvas* c1 = new TCanvas("test", "test_title", 800, 500);
	TF1 *func = new TF1("test_func", "sin(x)+3*x", 0, 10);
	func->Draw();
	func->Draw();
	
	//SIMULATING SCRIPT
	Initialize();
	state(kTRUE);
	draw_limits(0.0005, DBL_MAX);
	//Ne(kFALSE);
	//do_fit(kTRUE);
	//set_fit_1_gauss();
	//do_fit(kTRUE);
	//add_1peS();
	//state(kTRUE);
	////draw_Npe();
	////next_canvas();
	//Nt(kTRUE);
	//set_fit_1_gauss();
	//do_fit(kTRUE);
	//END OF SIMULATING SCRIPT
	std::cout<<std::endl<<"========================================="<<std::endl << "Finished" << std::endl;

	app->Run();
	delete app;
	return 0;
}
