{
if (false) {
  // sPMTs:
  // nch(); set_zoom(0, 0.24, 0, 0.06); set_bins(1500);
  // set_log_z(); set_bins(500);
  // fPMTs:
  // nch(); set_zoom(0, 0.1, 0, 0.006); set_bins(1000);
  // SiPMs:
  // nch(); set_zoom(0, 0.05, 0, 0.01); set_bins(800);
  // set_log_z();
  int channel = 8;
  bool display = true;
  int aggressiveness = 1;
  cut_A_S_upper(0.001, 0.0, 0.00435, 0.0, display, channel, "rem_smallA-S");
  if (aggressiveness == 1) {
    cut_A_S_upper(0.9, 0, 2, 0, display, channel, "rem_A>0.9");
    cut_A_S_upper(0.0, 0.020, 1.6, 0.020, display, channel, "rem_S>0.020");
  }
}
if (true) {
  for (int e = 0; e != exp_area.experiments.size(); ++e) {
    analysis_history(0);
    nex();
  }
  save_SiPM_Npe_table(data_output_path + "SiPM_Npes.txt");
}
//Checking new cuts. UPD: turned out to be useless, not finished or used
if (false) {
  std::string exp = post_processor->experiments[post_processor->current_exp_index];
  if (exp == "221020_S2_LAr_Pu_WLS_18.5kV_0V") {
    std::string folder, S2_start, S2_finish, S1_start, S1_finish, S2_LAr_start, S2_LAr_finish;
    double d_S2_LAr_start, d_S2_LAr_finish;
    int first_run = 0;
    int PMT_state = 0; //850V, 12 dB
    int SiPM_state = 0; //46 V
    folder = "Pu_18.5kV_46V_850V/tests";
    d_S2_start = 85.0; d_S2_finish = 90.0;
    S2_start = dbl_to_str(d_S2_start, 1); S2_finish = dbl_to_str(d_S2_finish, 1);

    d_S1_start = 58; d_S1_finish = 63.5;
    S1_start = dbl_to_str(d_S1_start, 1); S1_finish = dbl_to_str(d_S1_finish, 1);

    d_S2_LAr_start = 81.2; d_S2_LAr_finish = 82.8;
    S2_LAr_start = dbl_to_str(d_S2_LAr_start, 1); S2_LAr_finish = dbl_to_str(d_S2_LAr_finish, 1);

    auto first_run_entry = experiment_runs.find(exp);
    if (first_run_entry != experiment_runs.end())
        first_run = first_run_entry->second;
    else {
        std::cout<<"Could not find starting run index for '"<<exp<<"'! Will print invalid event indexes."<<std::endl;
        return;
    }

    std::string FOLDER = data_output_path + folder + "/";

    ty(AStates::PMT_Npe_sum);
    slow_PMTs_only();
    cut_PMT_noise(PMT_state);
    time_zoom_PMTs(0, 160);

    set_corr(AStates::PMT_sum_N, AStates::PMT_sum_N, -1, -1);
    ty(AStates::Correlation_y);
    time_zoom_PMTs_S1_S2(d_S1_start, d_S1_finish, d_S2_start, d_S2_finish);
    ty(AStates::Correlation);
    set_titles("N_{PE} t#in[0, 160] #mus", "N_{PE} S1+S2, t#in["+S1_start+", "+S1_finish+"]&["+S2_start+", "+S2_finish+"] #mus");
    set_zoom(0, 400, 0, 400);
    set_bins(400);
    x_y_regions = {86.8, 21.3, 134.4, 40.8, 128.8, 62.2, 81.3, 41.4};
    cut_x_y_poly_select(x_y_regions, true, "1");
    update();
    saveaspng(FOLDER + "00_PMTs_Npe_S1_S2_vs_all");
    set_as_run_cut("0");
    print_accepted_events(FOLDER + "00_events.txt", first_run);
    unset_as_run_cut("0");

    x_y_regions = {84.3, 2.0, 128, 2.0, 128, 22.5, 84.3, 22.5};
    cut_x_y_poly_select(x_y_regions, true, "1");
    update();
    saveaspng(FOLDER + "01_PMTs_Npe_S1_S2_vs_all");
    set_as_run_cut("1");
    print_accepted_events(FOLDER + "01_events.txt", first_run);
    unset_as_run_cut("1");

    }
}
}
