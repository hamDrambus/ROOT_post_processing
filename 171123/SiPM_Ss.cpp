{
  apply_S_cut();
  impose_limits(0.0009,0.008);
  set_fit_gauss(0);
  set_N_bin(200);
  draw_limits(0.0015,0.0041);
  use_mean();
  update();
  std::cout<<"s1pe: "<<post_processor->calibr_info.getS1pe(post_processor->current_channel)<<std::endl;
}
