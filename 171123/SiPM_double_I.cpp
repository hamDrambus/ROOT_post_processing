{
  exp_ind_start=post_processor->current_exp_index;
  double global_min_DI=-0.1;
  do {
    int ch_ind=0;
    int ch_start=post_processor->current_channel;
    do {
      
      set_fit_gauss(0);
      unset_limits();
      set_N_bin(300);
      
      FunctionWrapper* cutter = new FunctionWrapper(&global_min_DI);
      cutter->SetFunction(&Picker_S);
      add_hist_cut(cutter);
      
      ++ch_ind;
      Nch();
    } while (post_processor->current_channel!=ch_start);
    Ne();
  } while (post_processor->current_exp_index!=exp_ind_start);
}
