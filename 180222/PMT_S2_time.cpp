{
  /*S2_times.resize(9);
  S2_times[0]= std::pair<double,double>(50, 120);//7
  S2_times[1]= std::pair<double,double>(40, 110);//8
  S2_times[2]= std::pair<double,double>(40, 98);//9
  S2_times[3]= std::pair<double,double>(40, 95);//10
  S2_times[4]= std::pair<double,double>(40, 80);//11
  S2_times[5]= std::pair<double,double>(40, 76);//12
  S2_times[6]= std::pair<double,double>(40, 72);//13
  S2_times[7]= std::pair<double,double>(40, 70);//14
  S2_times[8]= std::pair<double,double>(40, 69);//15
  */
  exp_ind_start=post_processor->current_exp_index;
  do {
    set_fit_gauss(0);
    //for(int ch_ind =0, ch_end = post_processor->MPPC_channels.size();ch_ind!=ch_end;++ch_ind) {
      int ch = post_processor->current_channel;//MPPC_channels[ch_ind];
      
      remove_hist_cut("S2_time_limits", ch, false);
      FunctionWrapper *cutter_t = new FunctionWrapper(&S2_times[post_processor->current_exp_index]);
      cutter_t->SetFunction(&Picker_S2_t);
      add_hist_cut(cutter_t, "S2_time_limits", false);
    //}
    set_N_bin(100);
    Ne();
  } while (post_processor->current_exp_index!=exp_ind_start);
}
