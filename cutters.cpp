bool Picker_S (std::vector<double> &pars, void* stat_data){
    return pars[0]>=(*(double*)stat_data);
}
bool Picker_t (std::vector<double> &pars, void* stat_data){
    return (pars[2]>=((std::pair<double,double>*)stat_data)->first)&&(pars[3]<=((std::pair<double,double>*)stat_data)->second);
}
bool Picker_S2_S (std::vector<double> &pars, void* stat_data){
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true; 
  else 
    return pars[1]>=(*(double*)stat_data);
}
bool Picker_S2_t (std::vector<double> &pars, void* stat_data){
  if ((pars[1]==-2)&&(pars[2]==-2)&&(pars[3]==-2)&&(pars[4]==-2)) 
    return true; 
  else  
    return (pars[3]>=((std::pair<double,double>*)stat_data)->first)&&(pars[4]<=((std::pair<double,double>*)stat_data)->second);
} 
