#include "GlobalParameters.h"
#include "MTAnalysisManager.h"

//interactive methods
void state(bool full = false);
void do_fit(bool update_vis = false);
void update_visual(void);
void changed_point(bool is_new);

void Initialize(void)
{
	ParameterPile::Init_globals();
	manager = new AnalysisManager(ParameterPile::exp_area);
	manager->processAllExperiments();
	g_data = manager->getAllExperimentsResults();

	if ((current_channel < 0) || (current_exp_index < 0)){
		state(false);//TODO: implement state
		std::cout << "Wrong input data: no channels or experments from AnalysisManager" << std::endl;
		g_data = NULL;
		return;
	}
	post_processor = new PostProcessor(g_data);
	gr_manager = new GraphicOutputManager();

	do_fit();
	update_visual();
}

void Ne(bool save_state = true) //Next (E == experiment ==field)
{
	if (NULL == g_data){
		state(false);
		return;
	}

	int prev_index = current_exp_index;
	current_exp_index++;
	if (current_exp_index == g_data->exp_area.experiments.size())
		current_exp_index = 0;
	
	if (save_state){
		post_processor->set_manual_setups(current_setups, g_data->exp_area.experiments[prev_index], current_channel, current_type);
		post_processor->update(g_data->exp_area.experiments[prev_index], current_channel, current_type);
	}
	if (prev_index == current_exp_index)
		return; //there is no change
	if (NULL != post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type)){
		current_setups = post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
		return changed_point(false);
	}
	if (save_state) {
		//current_setups = new per_point_data(*current_setups);//TODO: use default or construct from current_setups??
		current_setups = new per_point_data;
		set_default_setups(current_setups,*post_processor->get_data(current_exp_index,current_channel,current_type));
	}
	changed_point(true);
}
void Pe(bool save_state = true) //Previous (E == experiment ==field)
{
	if (NULL == g_data){
		state(false);
		return;
	}

	int prev_index = current_exp_index;
	current_exp_index--;
	if (current_exp_index == -1)
		current_exp_index = g_data->exp_area.experiments.size()-1;

	if (save_state){
		post_processor->set_manual_setups(current_setups, g_data->exp_area.experiments[prev_index], current_channel, current_type);
		post_processor->update(g_data->exp_area.experiments[prev_index], current_channel, current_type);
	}
	if (prev_index == current_exp_index)
		return; //there is no change
	if (NULL != post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type)){
		current_setups = post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
		return changed_point(false);
	}
	if (save_state) {
		//current_setups = new per_point_data(*current_setups);//TODO: use default or construct from current_setups??
		current_setups = new per_point_data;
		set_default_setups(current_setups, *post_processor->get_data(current_exp_index, current_channel, current_type));
	}
	changed_point(true);
}
void Nch(bool save_state = true) //Next channel
{
	if (NULL == g_data){
		state(false);
		return;
	}

	int prev_channel = current_channel;
	int ch_ind = g_data->get_ch_index(current_channel);
	ch_ind++;
	if (ch_ind == g_data->mppc_channels.size())
		ch_ind = 0;
	current_channel = g_data->mppc_channels[ch_ind];

	if (save_state){
		post_processor->set_manual_setups(current_setups, g_data->exp_area.experiments[current_exp_index], prev_channel, current_type);
		post_processor->update(g_data->exp_area.experiments[current_exp_index], prev_channel, current_type);
	}
	if (prev_channel == current_channel)
		return; //there is no change
	if (NULL != post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type)){
		current_setups = post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
		return changed_point(false);
	}
	if (save_state) {
		//current_setups = new per_point_data(*current_setups);//TODO: use default or construct from current_setups??
		current_setups = new per_point_data;
		set_default_setups(current_setups, *post_processor->get_data(current_exp_index, current_channel, current_type));
	}
	changed_point(true);
}
void Pch(bool save_state = true) //Previous channel
{
	if (NULL == g_data){
		state(false);
		return;
	}

	int prev_channel = current_channel;
	int ch_ind = g_data->get_ch_index(current_channel);
	--ch_ind;
	if (ch_ind == -1)
		ch_ind = g_data->mppc_channels.size()-1;
	current_channel = g_data->mppc_channels[ch_ind];

	if (save_state){
		post_processor->set_manual_setups(current_setups, g_data->exp_area.experiments[current_exp_index], prev_channel, current_type);
		post_processor->update(g_data->exp_area.experiments[current_exp_index], prev_channel, current_type);
	}
	if (prev_channel == current_channel)
		return; //there is no change
	if (NULL != post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type)){
		current_setups = post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
		return changed_point(false);
	}
	if (save_state) {
		//current_setups = new per_point_data(*current_setups);//TODO: use default or construct from current_setups??
		current_setups = new per_point_data;
		set_default_setups(current_setups, *post_processor->get_data(current_exp_index, current_channel, current_type));
	}
	changed_point(true);
}

void GotoCh(int ch, bool save_state = true) //go to specific channel
{
	if (NULL == g_data){
		state(false);
		return;
	}
	int new_ch_ind = g_data->get_ch_index(ch);
	if (new_ch_ind < 0){
		std::cout << "There is no " << ch << " channel" << std::endl;
		return;
	}
	int prev_channel = current_channel;
	current_channel = ch;
	if (save_state){
		post_processor->set_manual_setups(current_setups, g_data->exp_area.experiments[current_exp_index], prev_channel, current_type);
		post_processor->update(g_data->exp_area.experiments[current_exp_index], prev_channel, current_type);
	}
	if (prev_channel == current_channel)
		return; //there is no change
	if (NULL != post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type)){
		current_setups = post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
		return changed_point(false);
	}
	if (save_state) {
		//current_setups = new per_point_data(*current_setups);//TODO: use default or construct from current_setups??
		current_setups = new per_point_data;
		set_default_setups(current_setups, *post_processor->get_data(current_exp_index, current_channel, current_type));
	}
	changed_point(true);
}
void Nt(bool save_state = true) //Next type (Ss, S2_S, double_intergral)
{
	if (NULL == g_data){
		state(false);
		return;
	}

	int prev_type = current_type;
	current_type++;
	if (current_type == 3)
		current_type = 0;
	
	if (save_state){
		post_processor->set_manual_setups(current_setups, g_data->exp_area.experiments[current_exp_index], current_channel, prev_type);
		post_processor->update(g_data->exp_area.experiments[current_exp_index], current_channel, prev_type);
	}
	if (prev_type == current_type)
		return; //there is no change
	if (NULL != post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type)){
		current_setups = post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
		return changed_point(false);
	}
	if (save_state) {
		//current_setups = new per_point_data(*current_setups);//TODO: use default or construct from current_setups??
		current_setups = new per_point_data;
		set_default_setups(current_setups, *post_processor->get_data(current_exp_index, current_channel, current_type));
	}
	changed_point(true);
}
void Pt(bool save_state = true) //Prev type (Ss, S2_S, double_intergral)
{
	if (NULL == g_data){
		state(false);
		return;
	}

	int prev_type = current_type;
	current_type--;
	if (current_type == -1)
		current_type = 2;

	if (save_state){
		post_processor->set_manual_setups(current_setups, g_data->exp_area.experiments[current_exp_index], current_channel, prev_type);
		post_processor->update(g_data->exp_area.experiments[current_exp_index], current_channel, prev_type);
	}
	if (prev_type == current_type)
		return; //there is no change
	if (NULL != post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type)){
		current_setups = post_processor->get_manual_setups(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
		return changed_point(false);
	}
	if (save_state) {
		//current_setups = new per_point_data(*current_setups);//TODO: use default or construct from current_setups??
		current_setups = new per_point_data;
		set_default_setups(current_setups, *post_processor->get_data(current_exp_index, current_channel, current_type));
	}
	changed_point(true);
}

void status(bool full = false){// displays current state
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
	if (NULL == current_setups) {
		std::cout << "run Initialize() first" << std::endl;
		return;
	}
	std::cout << "Current experiment: " << g_data->exp_area.experiments[current_exp_index]
		<< " (" << g_data->Fields[current_exp_index] << " kV)" << std::endl;
	std::cout << "Current channel: " << g_data->mppc_channels[g_data->get_ch_index(current_channel)]<< std::endl;
	std::cout << "Current type: ";
	switch (current_type){
	case (0): {
		std::cout << "Peaks area distribution";
		break;
	}
	case (1) : {
		std::cout << "Peaks area inside S2 area";
		break;
	}
	case (2) : {
		std::cout << "Double integral";
		break;
	}
	default: {
		std::cout << "UNDEFINED";
	}
	}
	std::cout << std::endl;

	if (full){
		std::cout << "Current_setups: "<<std::hex << current_setups <<std::dec<< std::endl;
		std::cout << "\tleft_limit " << current_setups->left_limit << std::endl;
		std::cout << "\tright_limit " << current_setups->right_limit << std::endl;
		std::cout << "\tleft_drawn_limit " << current_setups->left_drawn_limit << std::endl;
		std::cout << "\tright_drawn_limit " << current_setups->right_drawn_limit << std::endl;
		std::cout << "\tN_gauss " << current_setups->N_gauss << std::endl;
		std::cout << "\tN_bins " << current_setups->N_bins<< std::endl;
		std::cout << "\tpar_val [" << current_setups->par_val.size() << "]: ";
		for (auto i = current_setups->par_val.begin(), _end_ = current_setups->par_val.end(); i != _end_; ++i)
			std::cout << *i << (i == (_end_ - 1)) ? "" : " ";
		std::cout << std::endl;
		std::cout << "\tpar_left_limits [" << current_setups->par_left_limits.size() << "]: ";
		for (auto i = current_setups->par_left_limits.begin(), _end_ = current_setups->par_left_limits.end(); i != _end_; ++i)
			std::cout << *i << (i == (_end_ - 1)) ? "" : " ";
		std::cout << std::endl;
		std::cout << "\tpar_right_limits [" << current_setups->par_right_limits.size() << "]: ";
		for (auto i = current_setups->par_right_limits.begin(), _end_ = current_setups->par_right_limits.end(); i != _end_; ++i)
			std::cout << *i << (i == (_end_ - 1)) ? "" : " ";
		std::cout << std::endl;
		std::cout << "\tusing_mean " << current_setups->using_mean << std::endl;
		std::cout << "\tfitted " << current_setups->fitted << std::endl;
		std::cout << "\tusing_S1pe " << current_setups->using_S1pe << std::endl;
		std::cout << "\tusing_S2pe " << current_setups->using_S2pe << std::endl;
	}
}
void set_fit_1_gauss(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	if (current_setups->N_gauss == 1)
		return;
	bool decrease_pars = current_setups->N_gauss > 1;//always true here. 
	current_fit_func->Delete();
	current_setups->fitted = false;
	current_setups->N_gauss = 1;
	current_setups->par_val.resize(current_setups->N_gauss * 3);
	current_setups->par_left_limits.resize(current_setups->N_gauss * 3);
	current_setups->par_right_limits.resize(current_setups->N_gauss * 3);
	if (decrease_pars)
		return;
	else
		set_default_par_limits(current_setups, current_hist);
}
void set_fit_2_gauss(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	if (current_setups->N_gauss == 2)
		return;
	bool decrease_pars = current_setups->N_gauss > 2;
	current_fit_func->Delete();
	current_setups->fitted = false;
	current_setups->N_gauss = 2;
	current_setups->par_val.resize(current_setups->N_gauss * 3);
	current_setups->par_left_limits.resize(current_setups->N_gauss * 3);
	current_setups->par_right_limits.resize(current_setups->N_gauss * 3);
	if (decrease_pars)
		return;
	else
		set_default_par_limits(current_setups, current_hist);
}
void set_fit_3_gauss(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	if (current_setups->N_gauss == 3)
		return;
	bool decrease_pars = current_setups->N_gauss > 3;//never true here. 
	current_fit_func->Delete();
	current_setups->fitted = false;
	current_setups->N_gauss = 3;
	current_setups->par_val.resize(current_setups->N_gauss * 3);
	current_setups->par_left_limits.resize(current_setups->N_gauss * 3);
	current_setups->par_right_limits.resize(current_setups->N_gauss * 3);
	if (decrease_pars)
		return;
	else
		set_default_par_limits(current_setups, current_hist);
}
void set_parameter_val(int index, double val)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	if ((index < 0) || (index >= 3 * current_setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	current_setups->par_val[index] = val;
	current_fit_func->SetParameter(index, val);
}
void set_parameter_limits(int index, double left, double right)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	if ((index < 0) || (index >= 3 * current_setups->N_gauss)){
		std::cout << "index is out of boundaries" << std::endl;
		return;
	}
	current_setups->par_left_limits[index] = std::min(left,right);
	current_setups->par_right_limits[index] = std::max(left, right);
	current_fit_func->SetParLimits(index, left,right);
}

void do_fit(bool update_vis) //assumes that paramter limits are set and valid function is created
{
	if (NULL == g_data){
		state(false);
		return;
	}
	if (NULL==current_fit_func)
		current_fit_func = create_fit_function();
	current_hist->Fit(current_fit_func);
	current_setups->fitted = true;
	for (int par = 0; par < current_setups->par_val.size(); ++par)
		current_setups->par_val[par] = current_fit_func->GetParameter(par);
	if (update_vis)
		update_visual();
}

void draw_limits(double left, double right)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->right_drawn_limit = right;
	current_setups->left_drawn_limit = left;

	//since the limits have changed I must remake the fit, then update the canvas
	current_fit_func->Delete();
	current_fit_func = create_fit_function();
	if (current_setups->fitted)//if there was no fit, then do not fit
		do_fit();
	update_visual();
}
void impose_limits(double left, double right)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->left_drawn_limit =
		(current_setups->left_drawn_limit<current_setups->right_limit && current_setups->left_drawn_limit>current_setups->left_limit) ?
		current_setups->left_drawn_limit : std::min(left, right);
	current_setups->right_drawn_limit =
		(current_setups->right_drawn_limit<current_setups->right_limit && current_setups->right_drawn_limit>current_setups->left_limit) ?
		current_setups->right_drawn_limit : std::max(left, right);
	
	current_setups->left_limit = std::min(left, right);
	current_setups->right_limit = std::max(left, right);
	
	//since the limits have changed I must remake the histogram and fit, then update the canvas
	current_hist->Delete();
	current_hist = create_hist(*post_processor->get_data(current_exp_index, current_channel, current_type));
	current_fit_func->Delete();
	current_fit_func = create_fit_function();
	if (current_setups->fitted)
		do_fit();
	update_visual();
}

void reverse_limits(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	per_point_data temp;
	set_default_setups(&temp, *post_processor->get_data(current_exp_index, current_channel, current_type));
	current_setups->left_limit = temp.left_limit;
	current_setups->right_limit = temp.right_limit;
	current_setups->left_drawn_limit = current_setups->left_limit;
	current_setups->right_drawn_limit = current_setups->right_limit;
	
	current_hist->Delete();
	current_hist = create_hist(*post_processor->get_data(current_exp_index, current_channel, current_type));
	current_fit_func->Delete();
	current_fit_func = create_fit_function();
	if (current_setups->fitted)
		do_fit();
	update_visual();
}

/*void impose_fit_func_limits(double left, double right)
{
	if (NULL == g_data){
		state(false);
		return;
	}

}*/

void next_canvas(void) //creates new canvas, so the current one will stay unchanged
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_canvas = new TCanvas((std::string("Inter_canvas_") + std::to_string(canvas_n)).c_str(), (std::string("Inter_canvas_") + std::to_string(canvas_n)).c_str());
	++canvas_n;
	update_visual();
}

void set_calib_N(int from, int to)//in order to set default use invalid values
{
	if (NULL == g_data){
		state(false);
		return;
	}
	if ((to<from)){
		std::cout << "Using default points: [" << ParameterPile::calibaration_points.first << ", "
			<< ParameterPile::calibaration_points.second<< "]" << std::endl;
		post_processor->set_N_calib(current_channel, ParameterPile::calibaration_points);
		return;
	}
	bool ret = false;
	if (from >= g_data->Fields.size() || (from < 0)) {
		std::cout << "First value must be between [0," << g_data->Fields.size() - 1 << "]" << std::endl;
		ret = true;
	}
	if (to >= g_data->Fields.size() || (to < 0)) {
		std::cout << "Second value must be between [0," << g_data->Fields.size() - 1 << "]" << std::endl;
		ret = true;
	}
	if (ret)
		return;
	post_processor->set_N_calib(current_channel, from, to);
}

void draw_Npe(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->plot_N_pe(current_channel, gr_manager);
}
void add_1peS(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->using_S1pe = true;
	//post_processor->calculateS1pe(current_channel);
	//post_processor->update(g_data->exp_area.experiments[current_exp_index], current_channel, current_type);
}
void add_2peS(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->using_S2pe = true;
}
void rem_1peS(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->using_S1pe = false;
}
void rem_2peS(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->using_S2pe = false;
}
void set_1peS(double val)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->setS1pe(current_channel, val);
}
void unset_1peS(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	post_processor->unsetS1pe(current_channel);
}
void use_mean(bool do_use = true) //uses mean value of data instead of gauss' mean. May be usefull for S2_S
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->using_mean = do_use;
}
void set_N_bin(int n)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_setups->N_bins = n;
	current_hist->Delete();
	current_hist = create_hist(*post_processor->get_data(current_exp_index, current_channel, current_type));
	update_visual();
}
//interactive methods
void update_visual(void)
{
	if (NULL == g_data){
		state(false);
		return;
	}
	current_canvas->cd();
	current_canvas->Clear();
	if (current_hist) 
		current_hist->Draw();
	if (current_fit_func)
		if (current_setups->fitted)
			current_fit_func->Draw();
	if (current_setups->left_drawn_limit > current_setups->left_limit){
		if (NULL == current_vert_line0)
			current_vert_line0 = new TLine();
		current_vert_line0->SetX1(current_setups->left_drawn_limit);
		current_vert_line0->SetX2(current_setups->left_drawn_limit);
		current_vert_line0->SetY1(current_canvas->GetUymin());
		current_vert_line0->SetY2(current_canvas->GetUymax());
		current_vert_line0->SetLineColor(kRed);
		current_vert_line0->Draw();
	} else
		if (current_vert_line0)	{ current_vert_line0->Delete(); current_vert_line0 = NULL; }
	if (current_setups->right_drawn_limit < current_setups->right_limit){
		if (NULL == current_vert_line1)
			current_vert_line1 = new TLine();
		current_vert_line1->SetX1(current_setups->right_drawn_limit);
		current_vert_line1->SetX2(current_setups->right_drawn_limit);
		current_vert_line1->SetY1(current_canvas->GetUymin());
		current_vert_line1->SetY2(current_canvas->GetUymax());
		current_vert_line1->SetLineColor(kRed);
		current_vert_line1->Draw();
	} else
		if (current_vert_line1)	{ current_vert_line1->Delete(); current_vert_line1 = NULL; }
	current_canvas->Update();
}

void changed_point(bool is_new)
{
	current_hist->Delete();
	current_hist = create_hist(*post_processor->get_data(current_exp_index, current_channel, current_type));
	if(is_new)
		set_default_par_limits(current_setups, current_hist);
	current_fit_func->Delete();
	current_fit_func = create_fit_function();
	if (is_new)
		do_fit();
	update_visual();
}

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
