std::vector<std::vector<double> > S_T_to_exclude; //per channel, fill via S-t distribution in "S_t_cuts.cpp"
std::vector<double> S_min_thresholds;
std::vector<double> S_max_thresholds;
std::vector<std::pair<double,double> > S2_times;
std::vector<double> *x_y_regions = NULL;

bool Peak_S_min (std::vector<double> &pars, int run, void* stat_data){
    return pars[0]>=(*(double*)stat_data);
}
bool Peak_S_max (std::vector<double> &pars, int run, void* stat_data){
    return pars[0]<=(*(double*)stat_data);
}
bool Peak_t_min_max (std::vector<double> &pars, int run, void* stat_data){
    return (pars[4]>=((std::pair<double,double>*)stat_data)->first)&&(pars[4]<=((std::pair<double,double>*)stat_data)->second);
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

bool Peak_A_S_exclude (std::vector<double> &pars, int run, void* stat_data) {
  std::vector<double> *exl_region = (std::vector<double> *) stat_data;
  //{t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
  if (0!=(exl_region->size()%4))
    return true;
  double time = pars[1];
  for (int i=0, _end_=exl_region->size()/4;i!=_end_;++i){
    if ((time>=(*exl_region)[4*i])&&(time<=(*exl_region)[4*i+1])&&(pars[0]>=(*exl_region)[4*i+2])&&(pars[0]<=(*exl_region)[4*i+3]))
      return false;
  }
  return true;
}

bool Peak_A_S_exclude_fast_PMT (std::vector<double> &pars, int run, void* stat_data) {
  std::vector<double> *exl_region = (std::vector<double> *) stat_data;
  //{A_min, A0, S0, A1, S1, A_max}
  if (6>exl_region->size())
    return true;
  double A_min = (*exl_region)[0];
  double A0 = (*exl_region)[1];
  double S0 = (*exl_region)[2];
  double A1 = (*exl_region)[3];
  double S1 = (*exl_region)[4];
  double A_max = (*exl_region)[5];
  if (pars[1]<A_min)
     return false;
  if ((pars[1]<A_max)&&(pars[0]< (S0 + (S1-S0)*(pars[1]-A0)/(A1-A0)) ))
     return false;    
  return true;
}

bool Peak_S_t_select (std::vector<double> &pars, int run, void* stat_data) {
  std::vector<double> *exl_region = (std::vector<double> *) stat_data;
  //{t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
  if (0!=(exl_region->size()%4))
    return true;
  double time = pars[4];
  for (int i=0, _end_=exl_region->size()/4;i!=_end_;++i){
    if ((time>=(*exl_region)[4*i])&&(time<=(*exl_region)[4*i+1])&&(pars[0]>=(*exl_region)[4*i+2])&&(pars[0]<=(*exl_region)[4*i+3]))
      return true;
  }
  return false;
}

bool Picker_Run_max (std::vector<double> &pars, int run, void* stat_data) {
  int *Max_run = (int *) stat_data;
  return run<(*Max_run);
}

void select_S_t (double t_min, double t_max, double S_min, double S_max, bool do_update=true) {
  std::vector<double> *area = new std::vector<double>;
  area->push_back(std::min(t_min, t_max));
  area->push_back(std::max(t_min, t_max));
  area->push_back(std::min(S_min, S_max));
  area->push_back(std::max(S_min, S_max));
  int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
  if (ch_ind<0)
     ch_ind = post_processor->pmt_channel_to_index(post_processor->current_channel);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(area);
  cutter_S_T->SetFunction(&Peak_S_t_select);
  remove_hist_cut("S_T_select");
  add_hist_cut(cutter_S_T, "S_T_select");
}

//multichannel case
void select_S_t_MC (double t_min, double t_max, double S_min, double S_max, bool do_update=true) {
  std::vector<double> *area = new std::vector<double>;
  area->push_back(std::min(t_min, t_max));
  area->push_back(std::max(t_min, t_max));
  area->push_back(std::min(S_min, S_max));
  area->push_back(std::max(S_min, S_max));
  FunctionWrapper* cutter_S_T = new FunctionWrapper(area);
  cutter_S_T->SetFunction(&Peak_S_t_select);
  if (post_processor->is_PMT_type(post_processor->current_type)) {
     for (int ch_ind = 0; ch_ind!=post_processor->PMT_channels.size(); ++ch_ind) {
        remove_hist_cut("S_T_select", post_processor->PMT_channels[ch_ind]);
        add_hist_cut(cutter_S_T, "S_T_select", post_processor->PMT_channels[ch_ind]);
     }
  } else {
     for (int ch_ind = 0; ch_ind!=post_processor->MPPC_channels.size(); ++ch_ind) {
        remove_hist_cut("S_T_select", post_processor->MPPC_channels[ch_ind]);
        add_hist_cut(cutter_S_T, "S_T_select", post_processor->MPPC_channels[ch_ind]);
     }
  }
  if (do_update)
     update();
}

bool Peak_exclude_all (std::vector<double> &pars, int run, void* stat_data) {
  return false;
}

void turn_off_ch (int ch, bool do_update = true) {
  if (!post_processor->isMultichannel(post_processor->current_type)) {
     std::cout<<"Error: can't use this functoin for single channel type"<<std::endl;
     return;
  }
  FunctionWrapper* cutter_ = new FunctionWrapper(NULL);
  cutter_->SetFunction(&Peak_exclude_all);
  remove_hist_cut("ch_off", ch);
  add_hist_cut(cutter_, "ch_off", ch);
}

void turn_on_ch (int ch, bool do_update = true) {
  if (!post_processor->isMultichannel(post_processor->current_type)) {
     std::cout<<"Error: can't use this functoin for single channel type"<<std::endl;
     return;
  }
  remove_hist_cut("ch_off", ch);
	if (do_update)
     update();
}

//Following are using std::vector<double> *x_y_regions which must be created before every call
void select_S_t (bool do_update=true) {
  if (x_y_regions==NULL) {
    std::cout<<"x_y_regions vector is not created, no cuts"<<std::endl;
    return;
  }
  int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
  if (ch_ind<0)
     ch_ind = post_processor->pmt_channel_to_index(post_processor->current_channel);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(x_y_regions);
  x_y_regions = NULL;
  cutter_S_T->SetFunction(&Peak_S_t_select);
  remove_hist_cut("S_T_select");
  add_hist_cut(cutter_S_T, "S_T_select");
}

void exlude_S_t (bool do_update = true) {
  if (x_y_regions==NULL) {
    std::cout<<"x_y_regions vector is not created, no cuts"<<std::endl;
    return;
  }
  FunctionWrapper* cutter_S_T = new FunctionWrapper(x_y_regions);
  x_y_regions = NULL;
  cutter_S_T->SetFunction(&Peak_S_t_exclude);
  remove_hist_cut("S_T_exclude");
  add_hist_cut(cutter_S_T, "S_T_exclude");
}

void exlude_A_S (bool do_update = true) {
  if (x_y_regions==NULL) {
    std::cout<<"x_y_regions vector is not created, no cuts"<<std::endl;
    return;
  }
  FunctionWrapper* cutter_S_T = new FunctionWrapper(x_y_regions);
  x_y_regions = NULL;
  cutter_S_T->SetFunction(&Peak_A_S_exclude);
  remove_hist_cut("A_S_exclude");
  add_hist_cut(cutter_S_T, "A_S_exclude");
}

void exlude_A_S_fPMT (bool do_update = true) {
  if (x_y_regions==NULL) {
    std::cout<<"x_y_regions vector is not created, no cuts"<<std::endl;
    return;
  }
  FunctionWrapper* cutter_A_S = new FunctionWrapper(x_y_regions);
  x_y_regions = NULL;
  cutter_A_S->SetFunction(&Peak_A_S_exclude_fast_PMT);
  remove_hist_cut("A_S_exclude_fPMT");
  add_hist_cut(cutter_A_S, "A_S_exclude_fPMT");
}

//Following are using data set in 'date'/S_t_cuts.cpp
void apply_S_cut (bool do_update = true) {
    if (post_processor->isMultichannel(post_processor->current_type)) {
      std::cout<<"apply_S_cut(bool do_update):: Wrong function called for multichannel type"<<std::endl;
      return;
    }
    int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
    if (ch_ind<S_min_thresholds.size()) {
      FunctionWrapper* cutter = new FunctionWrapper(&S_min_thresholds[ch_ind]);
      cutter->SetFunction(&Peak_S_min);
      remove_hist_cut("S_min");
      add_hist_cut(cutter,"S_min");
    }
    if (ch_ind<S_max_thresholds.size()) {
      FunctionWrapper* cutter = new FunctionWrapper(&S_max_thresholds[ch_ind]);
      cutter->SetFunction(&Peak_S_max);
      remove_hist_cut("S_max");
      add_hist_cut(cutter,"S_max");
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
      remove_hist_cut("S_min",ch);
      add_hist_cut(cutter,"S_min",ch);
    }
    if (ch_ind<S_max_thresholds.size()) {
      FunctionWrapper* cutter = new FunctionWrapper(&S_max_thresholds[ch_ind]);
      cutter->SetFunction(&Peak_S_max);
      remove_hist_cut("S_max", ch);
      add_hist_cut(cutter,"S_max", ch);
    }
    if (do_update)
      update();
}

void apply_S_t_cut (bool do_update=true) {
  int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(&(S_T_to_exclude[ch_ind]));
  cutter_S_T->SetFunction(&Peak_S_t_exclude);
  remove_hist_cut("S_T_exclude");
  add_hist_cut(cutter_S_T, "S_T_exclude");
}

void apply_S_t_cut (int ch, bool do_update = true) {
  int ch_ind = post_processor->mppc_channel_to_index(ch);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(&(S_T_to_exclude[ch_ind]));
  cutter_S_T->SetFunction(&Peak_S_t_exclude);
  remove_hist_cut("S_T_exclude", ch);
  add_hist_cut(cutter_S_T, "S_T_exclude", ch);
}

void apply_run_cut (int less_than_N, bool do_update = true) {
    int *data = new int;
    (*data) = less_than_N;
    int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
    FunctionWrapper* cutter = new FunctionWrapper(data);
    cutter->SetFunction(&Picker_Run_max);
    unset_as_run_cut("Run_N_max");
    if (post_processor->isMultichannel(post_processor->current_type)){
      remove_hist_cut("Run_N_max", -1);
      add_hist_cut(cutter,"Run_N_max", -1);
      set_as_run_cut("Run_N_max");
      remove_hist_cut("Run_N_max",-1);
    } else {
      remove_hist_cut("Run_N_max");
      add_hist_cut(cutter,"Run_N_max");
      set_as_run_cut("Run_N_max");
      remove_hist_cut("Run_N_max");
    }
}

void apply_S_min_cut (double threshold, bool do_update = true)
{
   double *val = new double;
   *val = threshold;
   FunctionWrapper* cutter = new FunctionWrapper(val);
   cutter->SetFunction(&Peak_S_min);
   remove_hist_cut("S_min_cut");
   add_hist_cut(cutter,"S_min_cut");
}
