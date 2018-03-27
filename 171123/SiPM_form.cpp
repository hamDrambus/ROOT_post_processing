{  
  for (int ch_ind =0;ch_ind!=post_processor->MPPC_channels.size();++ch_ind) {
    //remove_hist_cut("_histogram_limits_",false);
    apply_S_cut(post_processor->MPPC_channels[ch_ind], false);
  }
  update();
/*}

{  
  for (int ch_ind =0;ch_ind!=post_processor->MPPC_channels.size();++ch_ind) {
    apply_S_cut(false);
    Nch(true);
  }
  update();*/
}

