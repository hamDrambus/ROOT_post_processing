{
  if (post_processor->isMultichannel(post_processor->current_type))
  {
    int exp_ind_start=post_processor->current_exp_index;
    do {
      set_fit_gauss(0);
      for(int ch_ind =0, ch_end = post_processor->MPPC_channels.size();ch_ind!=ch_end;++ch_ind) {
	int ch = post_processor->MPPC_channels[ch_ind];
	apply_S_cut(ch, false);
	apply_S_t_cut(ch, false);
	remove_hist_cut("S2_time_limits", ch, false);
	FunctionWrapper *cutter_t = new FunctionWrapper(&S2_times[post_processor->current_exp_index]);
	cutter_t->SetFunction(&Peak_t_min_max);
	add_hist_cut(cutter_t, "S2_time_limits", ch, false);
      }
      set_N_bin(100);
      Ne();
    } while (post_processor->current_exp_index!=exp_ind_start);
  }
  else //not multichannel
  {
    int exp_ind_start=post_processor->current_exp_index;
    do {
      int ch_start = post_processor->current_channel;
      do {
	set_fit_gauss(0);
	apply_S_cut(false);
	apply_S_t_cut(false);
	remove_hist_cut("S2_time_limits", false);
	FunctionWrapper *cutter_t = new FunctionWrapper(&S2_times[post_processor->current_exp_index]);
	if (post_processor->isComposite(post_processor->current_type))
	  cutter_t->SetFunction(&Peak_t_min_max_COMP);
	else
	  cutter_t->SetFunction(&Peak_t_min_max);
	add_hist_cut(cutter_t, "S2_time_limits", false);
	set_N_bin(100);
	Nch();
      } while (post_processor->current_channel!=ch_start);
      Ne();
    } while (post_processor->current_exp_index!=exp_ind_start);
  }
}

