{
  {
    int exp_ind_start=post_processor->current_exp_index;
    double* global_min_DI= new double;
    *global_min_DI = -0.1;
    do {
      int ch_start=post_processor->current_channel;
      do {
	
	set_fit_gauss(0);
	
	remove_hist_cut("DI_min", false);
	FunctionWrapper* cutter = new FunctionWrapper(&global_min_DI);
	cutter->SetFunction(&Peak_S_min);
	add_hist_cut(cutter,"DI_min", false);
	set_N_bin(200);
	Nch();
      } while (post_processor->current_channel!=ch_start);
      Ne();
    } while (post_processor->current_exp_index!=exp_ind_start);
  }
}
