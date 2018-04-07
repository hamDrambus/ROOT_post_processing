{
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
