
#include <string>

#include "efffct.C"

void fitEff(const std::string& fname="ROC_Curves_32_49_JAPU+INCL_topoclus.dat")
{
  TGraphErrors* pg = new TGraphErrors(fname.c_str());

  TH1D* _frame = new TH1D("_frame","_frame",100,-0.1,1.1);
  _frame->SetMaximum(1.1);
  _frame->SetMinimum(-0.1);
  _frame->DrawCopy("axis");

  //  pg->SetMarkerStyle(20);
  // pg->SetMarkerSize(0.75);
  pg->SetFillColor(kGray);
  pg->SetLineWidth(2);
  pg->Draw("3 same");
  pg->Draw("l same");

  //  TF1* ff = new TF1("f","abs([0])+abs([1])*pow([2]*x-[4]/(2*,2)+abs([4])*x",0.5,1.);
  TF1* ff = new TF1("f","abs([0])+abs([1])*x+abs([2])*pow(x-max([1]/(2*[2]),0.5),2)",0.4,1.);
  ff->SetParameter(0,0.05);
  ff->SetParameter(1,1.);
  ff->SetParameter(2,1.);
  pg->Fit(ff);
}
