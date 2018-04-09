 
//this is temprorary ROOT macro for writing multi-line code (such as lambda functions for cuts)
{
  SignalOperations::signal_from_file(xs,ys,"C2Trace00010.txt");
  SignalOperations::substract_baseline(ys,-0.0063);
  SignalOperations::Integrate(xs,ys,ys_int,0);
  drr = GM->GetDrawing("GEM_12kV",0,Gnuplot);
  drr->AddToDraw(xs,ys,"GEM 12 kV raw oscill");
  drr->AddToDraw(xs,ys_int,"GEM 12 kV integral oscill","axes x1y2");
  
  GM->Draw();
  GM->Clear();
}