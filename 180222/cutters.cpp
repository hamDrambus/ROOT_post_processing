std::vector<std::vector<double> > S_T_to_exclude; //per chanell, fill via S-t distribution in "S_t_cuts.cpp"
std::vector<double> S_min_thresholds;
std::vector<std::pair<double,double> > S2_times;

bool Picker_S (std::vector<double> &pars, int run, void* stat_data){
    return pars[0]>=(*(double*)stat_data);
}
bool Picker_S_max (std::vector<double> &pars, int run, void* stat_data){
    return pars[0]<=(*(double*)stat_data);
}
bool Picker_t (std::vector<double> &pars, int run, void* stat_data){
    return (pars[2]>=((std::pair<double,double>*)stat_data)->first)&&(pars[3]<=((std::pair<double,double>*)stat_data)->second);
}
bool Picker_S2_S (std::vector<double> &pars, int run, void* stat_data){
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true; 
  else 
    return pars[1]>=(*(double*)stat_data);
}
bool Picker_S2_t (std::vector<double> &pars, int run, void* stat_data){
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true; 
  else  
    return (pars[3]>=((std::pair<double,double>*)stat_data)->first)&&(pars[4]<=((std::pair<double,double>*)stat_data)->second);
} 

bool Picker_S_T_exclude (std::vector<double> &pars, int run, void* stat_data) {
  std::vector<double> *exl_region = (std::vector<double> *) stat_data;
  //{t_min0, t_max0, S_min0, S_max0, t_min1, t_max1 ...}
  if (0!=(exl_region->size()%4))
    return true;
  double time = 0.5*(pars[2] + pars[3]);
  for (int i=0, _end_=exl_region->size()/4;i!=_end_;++i){
    if ((time>=(*exl_region)[4*i])&&(time<=(*exl_region)[4*i+1])&&(pars[0]>=(*exl_region)[4*i+2])&&(pars[0]<=(*exl_region)[4*i+3]))
      return false;
  }
  return true;
}

void apply_S_cut (bool do_update = true) {
    int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
    FunctionWrapper* cutter = new FunctionWrapper(&S_min_thresholds[ch_ind]);
    cutter->SetFunction(&Picker_S);
    remove_hist_cut("S_min", false);
    add_hist_cut(cutter,"S_min", false);
    /*cutter = new FunctionWrapper(&S_max_thresholds[ch_ind]);
    cutter->SetFunction(&Picker_S_max);
    remove_hist_cut("S_max", false);
    add_hist_cut(cutter,"S_max", false);*/
}

void apply_S_cut (int ch, bool do_update = true) {
    int ch_ind = post_processor->mppc_channel_to_index(ch);
    FunctionWrapper* cutter = new FunctionWrapper(&S_min_thresholds[ch_ind]);
    cutter->SetFunction(&Picker_S);
    remove_hist_cut("S_min",ch, false);
    add_hist_cut(cutter,"S_min",ch, false);
    /*cutter = new FunctionWrapper(&S_max_thresholds[ch_ind]);
    cutter->SetFunction(&Picker_S_max);
    remove_hist_cut("S_max", ch, false);
    add_hist_cut(cutter,"S_max", ch, false);*/
}

void apply_S_t_cut (bool do_update=true) {
  int ch_ind = post_processor->mppc_channel_to_index(post_processor->current_channel);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(&(S_T_to_exclude[ch_ind]));
  cutter_S_T->SetFunction(&Picker_S_T_exclude);
  remove_hist_cut("S_T_exclude", false);
  add_hist_cut(cutter_S_T, "S_T_exclude", do_update);
}

void apply_S_t_cut (int ch, bool do_update = true) {
  int ch_ind = post_processor->mppc_channel_to_index(ch);
  FunctionWrapper* cutter_S_T = new FunctionWrapper(&(S_T_to_exclude[ch_ind]));
  cutter_S_T->SetFunction(&Picker_S_T_exclude);
  remove_hist_cut("S_T_exclude", ch, false);
  add_hist_cut(cutter_S_T, "S_T_exclude", ch, do_update);
}
