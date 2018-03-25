{
  {
    std::vector<std::pair<double,double> > S2_times;
    S2_times.push_back(std::pair<double,double>(50, 120));//7
    S2_times.push_back(std::pair<double,double>(40, 110));//8
    S2_times.push_back(std::pair<double,double>(40, 98));//9
    S2_times.push_back(std::pair<double,double>(40, 95));//10
    S2_times.push_back(std::pair<double,double>(40, 80));//11
    S2_times.push_back(std::pair<double,double>(40, 76));//12
    S2_times.push_back(std::pair<double,double>(40, 72));//13
    S2_times.push_back(std::pair<double,double>(40, 70));//14
    S2_times.push_back(std::pair<double,double>(40, 69));//15
    
    int exp_ind_start=post_processor->current_exp_index;
    do {
      int ch_ind=0;
      int ch_start=post_processor->current_channel;
      do {
	set_fit_gauss(0);
	FunctionWrapper *cutter_S = new FunctionWrapper(&S_thresholds[ch_ind]);
	cutter_S->SetFunction(&Picker_S2_S);
	add_hist_cut(cutter_S, "S min threshold", false);
	
	FunctionWrapper *cutter_t = new FunctionWrapper(&S2_times[post_processor->current_exp_index]);
	cutter_t->SetFunction(&Picker_S2_t);
	add_hist_cut(cutter_t, "S2 time limits", false);
	
	set_N_bin(100);
	++ch_ind;
	Nch();
      } while (post_processor->current_channel!=ch_start);
      Ne();
    } while (post_processor->current_exp_index!=exp_ind_start);
  }
}
