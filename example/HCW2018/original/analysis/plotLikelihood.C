
//#include "AtlasUtils.C"
#include "HistHelper.h"
#include "ReadMLFile.h"

#include <TCanvas.h>
#include <TH1D.h>
#include <TString.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>

#include <string>

void plotLikelihood(const std::string& fname,const std::string& htag="",int mode=0)
{

  ReadMLFile indata(fname);
  printf("plotLikelihood - read %zu records\n",indata.size());

  double xl(-0.1); 
  double xh(1.1);

  int  nb(0);
  switch ( mode ) {
  case 0:
    nb = 120;
    printf("plotLikelihood - mode %i: use coarse binning for plots, (nb,xl,xh,dx) = (%i,%.2f,%.2f,%.3f)\n",mode,nb,xl,xh,(xh-xl)/((double)nb));
    break;
  case 1:
    nb = 1200;
    printf("plotLikelihood - mode %i: use very fine binning for analysis, (nb,xl,xh,dx) = (%i,%.2f,%.2f,%.3f)\n",mode,nb,xl,xh,(xh-xl)/((double)nb));
    break;
  default:
    printf("plotLikelihood - unknown mode %i, do nothing\n",mode);
  }

  TH1D* h_lsig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_lsig",true).c_str(),"P_{signal}^{jet} (signal jets)", nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_lsig").c_str(),    "P_{signal}^{signal} (signal jets)",nb,xl,xh);
  TH1D* h_lbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_lbck",true).c_str(),"P_{signal}^{jet} (pile-up jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_lbck").c_str(),    "P_{signal}^{jet} (pile-up jets)",  nb,xl,xh);
  
  auto fiter = indata.begin();
  size_t i(1);
  for ( ; fiter != indata.end(); ++fiter ) { 
    double ls(indata.psig(fiter));
    if ( indata.tag(fiter) == 1 ) {
      h_lsig->Fill(ls);
    } else {
      h_lbck->Fill(ls);
    }
  }

  double lsigint(h_lsig->GetEntries());
  double lbckint(h_lbck->GetEntries());

  printf("plotLikelihood - signal/pile-up jet integral is %.0f/%.0f\n",lsigint,lbckint);
  h_lsig->Scale(1./lsigint);
  h_lbck->Scale(1./lbckint);

  TH1D* _frame = new TH1D("_frame","_frame",h_lsig->GetNbinsX(),h_lsig->GetXaxis()->GetXmin(),h_lsig->GetXaxis()->GetXmax());
  _frame->SetMaximum(100.);
  _frame->SetMinimum(5e-06);
  _frame->GetXaxis()->SetTitle("P_{signal}^{jet}");
  _frame->GetYaxis()->SetTitle(TString::Format("1/N_{jet} #partialN/#partialP_{signal}^{jet} [1/%.1f%%]",h_lsig->GetBinWidth(1)*100.).Data());

  _frame->DrawCopy("axis");

  //h_lsig->SetFillColor(kYellow);
  h_lsig->SetLineColor(kRed);
  //h_lbck->SetFillColor(kCyan);
  h_lbck->SetLineColor(kBlue);

  h_lsig->DrawCopy("same hist");
  h_lbck->DrawCopy("same hist");

  _frame->DrawCopy("same axis");

  TLegend* leg = new TLegend(0.8,0.75,1-gStyle->GetPadRightMargin()-0.025,1-gStyle->GetPadTopMargin()-0.05);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  TLegendEntry* l2 = leg->AddEntry(h_lsig,"signal jets","lf");  l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_lbck,"pile-up jets","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("simulation internal");
  HistHelper::Draw::processing(fname);

  delete _frame;

}
