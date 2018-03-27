{
    S2_times.resize(1);
    S2_times[0]= std::pair<double,double>(24, 70);//20kV Cd
    //S2_times[0]= std::pair<double,double>(40, 83);//20kV Xray
    {   
    int exp_ind_start=post_processor->current_exp_index;
    for (int ch_ind =0;ch_ind!=post_processor->MPPC_channels.size();++ch_ind) {
      int ch = post_processor->MPPC_channels[ch_ind];
      
      /*remove_hist_cut("S2_time_limits",ch,false);
      remove_hist_cut("S_min",ch,false);
      remove_hist_cut("S_max",ch,false);
      */
      apply_S_cut(ch,false);
      FunctionWrapper *cutter_t = new FunctionWrapper(&S2_times[post_processor->current_exp_index]);
      cutter_t->SetFunction(&Picker_S2_t);
      add_hist_cut(cutter_t, "S2_time_limits", ch, false);
      
    }
    update();
  }
}
