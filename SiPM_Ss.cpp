{
  apply_S_t_cut();
  impose_limits(0.0002, 0.0045);
  set_fit_gauss(0);
  set_N_bin(100);
  draw_limits(0.0006, 0.0025);
  use_mean();
  update();
  std::cout<<"s1pe: "<<post_processor->calibr_info.getS1pe(post_processor->current_channel)<<std::endl;
}
