{
  apply_S_t_cut();
  impose_limits(0.0008,0.01);
  set_fit_gauss(0);
  set_N_bin(200);
  draw_limits(0.0014,0.0044);
  use_mean();
  update();
  std::cout<<"s1pe: "<<post_processor->calibr_info.getS1pe(post_processor->current_channel)<<std::endl;
}
