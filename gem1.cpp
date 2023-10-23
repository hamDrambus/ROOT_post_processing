// This code is for integrating and plotting charge signal from oscilloscope (LeCroy).
// Use after .x init.cpp and .x gem.cpp
{
  SignalOperations::signal_from_lecroy_file(xs,ys,"../Data/221124/221124_lecroy/221124/C3Trace00029.txt", 1e6);
  baseline = SignalOperations::find_baseline_by_integral(xs, ys, -30.0, -1.0);
  SignalOperations::substract_baseline(ys, baseline);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("Charge_20kV_pure_LAr");
  drr->AddToDraw(xs,ys,"Charge 20 kV raw oscill pure LAr");
  drr->AddToDraw(xs,ys_int,"Charge 20 kV integral oscill pure LAr","axes x1y2");
  
  GM->Draw();
  GM->Clear();
}