std::vector<std::vector<double> > S_T_to_exclude; //per chanell, fill via S-t distribution in "S_t_cuts.cpp"
std::vector<double> S_min_thresholds;
std::vector<double> S_max_thresholds;
std::vector<std::pair<double,double> > S2_times;

bool Peak_S_min (std::vector<double> &pars, int run, void* stat_data){
    return pars[0]>=(*(double*)stat_data);
}
bool Peak_S_max (std::vector<double> &pars, int run, void* stat_data){
    return pars[0]<=(*(double*)stat_data);
}
bool Peak_t_min_max (std::vector<double> &pars, int run, void* stat_data){
    return (pars[4]>=((std::pair<double,double>*)stat_data)->first)&&(pars[4]<=((std::pair<double,double>*)stat_data)->second);
}
//COMP for Composite
bool Peak_S_min_COMP (std::vector<double> &pars, int run, void* stat_data){
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true; 
  else
    return pars[1]>=(*(double*)stat_data);
}
bool Peak_S_max_COMP (std::vector<double> &pars, int run, void* stat_data){
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true; 
  else 
    return pars[1]<=(*(double*)stat_data);
}
bool Peak_t_min_max_COMP (std::vector<double> &pars, int run, void* stat_data){
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true; 
  else  
    return (pars[5]>=((std::pair<double,double>*)stat_data)->first)&&(pars[5]<=((std::pair<double,double>*)stat_data)->second);
} 

bool Peak_S_t_exclude (std::vector<double> &pars, int run, void* stat_data) {
  std::vector<double> *exl_region = (std::vector<double> *) stat_data;
  //{t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
  if (0!=(exl_region->size()%4))
    return true;
  double time = pars[4];
  for (int i=0, _end_=exl_region->size()/4;i!=_end_;++i){
    if ((time>=(*exl_region)[4*i])&&(time<=(*exl_region)[4*i+1])&&(pars[0]>=(*exl_region)[4*i+2])&&(pars[0]<=(*exl_region)[4*i+3]))
      return false;
  }
  return true;
}

bool Peak_S_t_exclude_COMP (std::vector<double> &pars, int run, void* stat_data) {
  std::vector<double> *exl_region = (std::vector<double> *) stat_data;
  //{t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true;
  if (0!=(exl_region->size()%4))
    return true;
  double time = pars[5];
  for (int i=0, _end_=exl_region->size()/4;i!=_end_;++i){
    if ((time>=(*exl_region)[4*i])&&(time<=(*exl_region)[4*i+1])&&(pars[1]>=(*exl_region)[4*i+2])&&(pars[1]<=(*exl_region)[4*i+3]))
      return false;
  }
  return true;
}

bool Picker_Run_max (std::vector<double> &pars, int run, void* stat_data) {
  int *Max_run = (int *) stat_data;
  return run<(*Max_run);
}

void apply_S_cut (bool do_update = true) {
    if (post_processor->isMultichannel(post_processor->current_type)) {
      std::cout<<"apply_S_cut(bool do_update):: Wrong function called for multichannel type"<<std::endl;
      return;
    }
    int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
    if (ch_ind<S_min_thresholds.size()) {
      FunctionWrapper* cutter = new FunctionWrapper(&S_min_thresholds[ch_ind]);
      if (post_processor->isComposite(post_processor->current_type))
	cutter->SetFunction(&Peak_S_min_COMP);
      else
	cutter->SetFunction(&Peak_S_min);
      remove_hist_cut("S_min", false);
      add_hist_cut(cutter,"S_min", false);
    }
    if (ch_ind<S_max_thresholds.size()) {
      FunctionWrapper* cutter = new FunctionWrapper(&S_max_thresholds[ch_ind]);
      if (post_processor->isComposite(post_processor->current_type))
	cutter->SetFunction(&Peak_S_max_COMP);
      else
	cutter->SetFunction(&Peak_S_max);
      remove_hist_cut("S_max", false);
      add_hist_cut(cutter,"S_max", false);
    }
    if (do_update)
      update();
}

void apply_S_cut (int ch, bool do_update = true) {
    if (!(post_processor->isMultichannel(post_processor->current_type))) {
      std::cout<<"apply_S_cut(int ch, bool do_update):: Wrong function called for single channel type"<<std::endl;
      return;
    }  
    int ch_ind = post_processor->mppc_channel_to_index(ch);
    if (ch_ind<S_min_thresholds.size()) {
      FunctionWrapper* cutter = new FunctionWrapper(&S_min_thresholds[ch_ind]);
      cutter->SetFunction(&Peak_S_min);
      remove_hist_cut("S_min",ch, false);
      add_hist_cut(cutter,"S_min",ch, false);
    }
    if (ch_ind<S_max_thresholds.size()) {
      FunctionWrapper* cutter = new FunctionWrapper(&S_max_thresholds[ch_ind]);
      cutter->SetFunction(&Peak_S_max);
      remove_hist_cut("S_max", ch, false);
      add_hist_cut(cutter,"S_max", ch, false);
    }
    if (do_update)
      update();
}

void apply_S_t_cut (bool do_update=true) {
  int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(&(S_T_to_exclude[ch_ind]));
  if (post_processor->isComposite(post_processor->current_type))
    cutter_S_T->SetFunction(&Peak_S_t_exclude_COMP);
  else 
    cutter_S_T->SetFunction(&Peak_S_t_exclude);
  remove_hist_cut("S_T_exclude", false);
  add_hist_cut(cutter_S_T, "S_T_exclude", do_update);
}

void apply_S_t_cut (int ch, bool do_update = true) {
  int ch_ind = post_processor->mppc_channel_to_index(ch);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(&(S_T_to_exclude[ch_ind]));
  cutter_S_T->SetFunction(&Peak_S_t_exclude);
  remove_hist_cut("S_T_exclude", ch, false);
  add_hist_cut(cutter_S_T, "S_T_exclude", ch, do_update);
}

void apply_run_cut (int less_than_N, bool do_update = true) {
    int *data = new int;
    (*data) = less_than_N;
    int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
    FunctionWrapper* cutter = new FunctionWrapper(data);
    cutter->SetFunction(&Picker_Run_max);
    unset_as_run_cut("Run_N_max");
    if (post_processor->isMultichannel(post_processor->current_type)){
      remove_hist_cut("Run_N_max", -1, false);
      add_hist_cut(cutter,"Run_N_max", -1, false);
      set_as_run_cut("Run_N_max");
      remove_hist_cut("Run_N_max",-1, do_update);
    } else {
      remove_hist_cut("Run_N_max", false);
      add_hist_cut(cutter,"Run_N_max", false);
      set_as_run_cut("Run_N_max");
      remove_hist_cut("Run_N_max", do_update);
    }
}

void apply_S_min_cut (double threshold, bool do_update = true)
{
   double *val = new double;
   *val = threshold;
   FunctionWrapper* cutter = new FunctionWrapper(val);
   if (post_processor->isComposite(post_processor->current_type))
      cutter->SetFunction(&Peak_S_min_COMP);
   else
      cutter->SetFunction(&Peak_S_min);
   remove_hist_cut("S_min_cut", false);
   add_hist_cut(cutter,"S_min_cut", do_update);
}
