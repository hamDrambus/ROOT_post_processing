{
  for (Int_t exp = 0, exp_end_ = g_data->N_pe_direct.size(); exp != exp_end_; ++exp) {
    for (Int_t ch = 0, ch_end_ = post_processor->MPPC_channels.size(); ch != ch_end_; ++ch) {
      if (exp > post_processor->calibr_info.get_N_calib(MPPC_channels[ch]).second)
	g_data->N_pe_result[exp][ch] = g_data->N_pe_Double_I[exp][ch];
      else
	g_data->N_pe_result[exp][ch] = g_data->N_pe_direct[exp][ch];    
    }
  }
}
