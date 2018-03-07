{
  std::vector<double> S_thresholds {0.0007, 0.00066, 0.001, 0.00095, 0.0009, 0.00075, 0.0009, 0.0007, 0.00085,
    0.00075, 0.0009, 0.00085, 0.0008, 0.0008, 0.00065, 0.00065, 0.0008, 0.0009, 0.001, 0.0007, 0.00085, 0.0008,
    0.0008, 0.0008};
  std::vector<std::pair<double,double> > S2_times;
  S2_times.push_back(std::pair<double,double>(80, 180));
  S2_times.push_back(std::pair<double,double>(65, 145));
  S2_times.push_back(std::pair<double,double>(55, 125));
  S2_times.push_back(std::pair<double,double>(50, 120));
  S2_times.push_back(std::pair<double,double>(50, 105));
  S2_times.push_back(std::pair<double,double>(45, 95));
  S2_times.push_back(std::pair<double,double>(45, 105));
  S2_times.push_back(std::pair<double,double>(45, 90));
  S2_times.push_back(std::pair<double,double>(42, 90));
  S2_times.push_back(std::pair<double,double>(40, 90));
  
  int exp_ind_start=post_processor->current_exp_index;
  do {
    int ch_ind=0;
    int ch_start=post_processor->current_channel;
    do {
      FunctionWrapper *cutter_S = new FunctionWrapper(&S_thresholds[ch_ind]);
      cutter_S->SetFunction(&Picker_S2_S);
      add_hist_cut(cutter_S, "S min threshold");
      
      FunctionWrapper *cutter_t = new FunctionWrapper(&S2_times[post_processor->current_exp_index]);
      cutter_t->SetFunction(&Picker_S2_t);
      add_hist_cut(cutter_t, "S2 time limits");
      
      set_fit_gauss(0);
      set_N_bin(100);
      
      ++ch_ind;
      Nch();
    } while (post_processor->current_channel!=ch_start);
    Ne();
  } while (post_processor->current_exp_index!=exp_ind_start);
}
