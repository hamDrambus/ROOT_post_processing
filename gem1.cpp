// This code is for integrating and plotting charge signal from oscilloscope (LeCroy).
// Use after .x init.cpp and .x gem.cpp
{
  /*SignalOperations::signal_from_lecroy_file(xs,ys,"../Data/221124/221124_lecroy/221124/C3Trace00029.txt", 1e6);
  baseline = SignalOperations::find_baseline_by_integral(xs, ys, -30.0, -1.0);
  SignalOperations::substract_baseline(ys, baseline);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("Charge_20kV_pure_LAr");
  drr->AddToDraw(xs,ys,"Charge 20 kV raw oscill pure LAr");
  drr->AddToDraw(xs,ys_int,"Charge 20 kV integral oscill pure LAr","axes x1y2");*/
  
  SignalOperations::signal_from_lecroy_file(xs,ys,"../Data/220804/220804_lecroy/C3--Trace--00011.txt", 1e6);
  baseline = SignalOperations::find_baseline_by_integral(xs, ys, -30.0, -1.0);
  SignalOperations::substract_baseline(ys, baseline);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("Charge_20kV_pure_LAr_no_coll");
  drr->AddToDraw(xs,ys,"Charge 20 kV pure LAr no coll");
  drr->AddToDraw(xs,ys_int,"Integral charge 20 kV pure LAr no coll","axes x1y2");

  SignalOperations::signal_from_lecroy_file(xs,ys,"../Data/220804/220804_lecroy/C3--Trace--00012.txt", 1e6);
  baseline = SignalOperations::find_baseline_by_integral(xs, ys, -30.0, -1.0);
  SignalOperations::substract_baseline(ys, baseline);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("Charge_20kV_pure_LAr_35mm_coll");
  drr->AddToDraw(xs,ys,"Charge 20 kV pure LAr 35mm coll");
  drr->AddToDraw(xs,ys_int,"Integral charge 20 kV pure LAr 35mm coll","axes x1y2");

  SignalOperations::signal_from_lecroy_file(xs,ys,"../Data/220804/220804_lecroy/C3--Trace--00018.txt", 1e6);
  baseline = SignalOperations::find_baseline_by_integral(xs, ys, -30.0, -1.0);
  SignalOperations::substract_baseline(ys, baseline);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("Charge_20kV_pure_LAr_14mm_coll");
  drr->AddToDraw(xs,ys,"Charge 20 kV pure LAr 14mm coll");
  drr->AddToDraw(xs,ys_int,"Integral charge 20 kV pure LAr 14mm coll","axes x1y2");

  SignalOperations::signal_from_lecroy_file(xs,ys,"../Data/220804/220804_lecroy/C3--Trace--00020.txt", 1e6);
  baseline = SignalOperations::find_baseline_by_integral(xs, ys, -30.0, -1.0);
  SignalOperations::substract_baseline(ys, baseline);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("Charge_20kV_pure_LAr_14mm_coll_1");
  drr->AddToDraw(xs,ys,"Charge 20 kV pure LAr 14mm coll 1");
  drr->AddToDraw(xs,ys_int,"Integral charge 20 kV pure LAr 14mm coll 1","axes x1y2");

  GM->Draw();
  GM->Clear();
}