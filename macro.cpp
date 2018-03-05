{
  /*bool Picker_1 (std::vector<double> &pars, void* stat_data){
      return pars[0]>=(*(double*)stat_data);
  }*/
  double MPPC_32_6kV_form_cut = 0.0007;
  FunctionWrapper * cutter_ch32_6kV_form = new FunctionWrapper(&MPPC_32_6kV_cut);
  cutter->SetFunction(&Picker_1);
}