{  
  for (int ch_ind =0;ch_ind!=post_processor->MPPC_channels.size();++ch_ind) {
    //remove_hist_cut("_histogram_limits_",false);
    FunctionWrapper* cutter = new FunctionWrapper(&S_min_thresholds[ch_ind]);
    cutter->SetFunction(&Picker_S);
    remove_hist_cut("S_min",post_processor->MPPC_channels[ch_ind], false);
    add_hist_cut(cutter,"S_min", post_processor->MPPC_channels[ch_ind], false);
    apply_S_t_cut(post_processor->MPPC_channels[ch_ind], false);
  }
  update();
}
