// This code is for integrating and plotting charge signal from oscilloscope (LeCroy).
// Use after .x init.cpp
{
  std::vector<double> xs,ys,ys_int;
  //Init_globals(false);
  GraphCollection* GM = new GraphCollection();
  GnuplotDrawing *drr = nullptr;
  
  /*SignalOperations::signal_from_file(xs,ys,"C2Trace00021.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  GnuplotDrawing *drr = GM->GetDrawing("GEM_15kV");
  drr->AddToDraw(xs,ys,"GEM 15 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 15 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00006.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_14kV");
  drr->AddToDraw(xs,ys,"GEM 14 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 14 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00007.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_13kV");
  drr->AddToDraw(xs,ys,"GEM 13 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 13 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00010.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_12kV");
  drr->AddToDraw(xs,ys,"GEM 12 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 12 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00011.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_11kV");
  drr->AddToDraw(xs,ys,"GEM 11 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 11 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00014.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_10kV");
  drr->AddToDraw(xs,ys,"GEM 10 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 10 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00016.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_9kV");
  drr->AddToDraw(xs,ys,"GEM 9 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 9 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00017.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_8kV");
  drr->AddToDraw(xs,ys,"GEM 8 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 8 kV integral oscill","axes x1y2");
  
  SignalOperations::signal_from_file(xs,ys,"C2Trace00020.txt");
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_7kV");
  drr->AddToDraw(xs,ys,"GEM 7 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 7 kV integral oscill","axes x1y2");*/

  /*SignalOperations::signal_from_lecroy_file(xs,ys,"../hdda/Data/231012/231012_lecroy/C3Trace00019.txt", 1e6);
  double baseline = SignalOperations::find_baseline_by_integral(xs, ys, -30.0, -1.0);
  SignalOperations::substract_baseline(ys, baseline);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  GnuplotDrawing *drr = GM->GetDrawing("Charge_20kV");
  drr->AddToDraw(xs,ys,"Charge 20 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"Charge 20 kV integral oscill","axes x1y2");*/
  
  GM->Draw();
  GM->Clear();
}