
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

void plotW(const std::string& fname,const std::string& htag="")
{
  ReadMLFile indata(fname);
  printf("plotW - read %zu records\n",indata.size());

  double xl(-0.005); double dx(0.01); int nb(40); double xh(xl+(double)nb*dx);
  TH1D* h_wall = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_wall",true).c_str(),"w^{jet} (all jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_wall").c_str(),"w^{jet} (all jets)",    nb,xl,xh);
  TH1D* h_wsig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_wsig",true).c_str(),"w^{jet} (signal jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_wsig").c_str(),"w^{jet} (signal jets)",    nb,xl,xh);
  TH1D* h_wbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_wbck",true).c_str(),"w^{jet} (pile-up jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_wbck").c_str(),"w^{jet} (pile-up jets)",    nb,xl,xh);

  
  auto fiter = indata.begin();
  size_t i(1);
  for ( ; fiter != indata.end(); ++fiter ) { 
    double w(indata.width(fiter));
    h_wall->Fill(w);
    if ( indata.tag(fiter) == 1 ) {
      h_wsig->Fill(w);
    } else {
      h_wbck->Fill(w);
    }
  }

  HistHelper::Hist::realign(h_wall);
  HistHelper::Hist::realign(h_wsig);
  HistHelper::Hist::realign(h_wbck);

  TH1D* _frame = new TH1D(HistHelper::Names::histName(fname,"_frame").c_str(),"_frame",h_wall->GetNbinsX(),h_wall->GetXaxis()->GetXmin()-dx,h_wall->GetXaxis()->GetXmax()+dx);
  _frame->SetMaximum(1000000.);
  _frame->SetMinimum(0.5);
  _frame->GetXaxis()->SetTitle("w^{jet}");
  _frame->GetYaxis()->SetTitle(TString::Format("#partialN/#partialw^{jet} [1/%.2f]",h_wall->GetBinWidth(1)).Data());

  _frame->DrawCopy("axis");

  h_wsig->SetFillColor(kYellow);
  h_wsig->SetLineColor(kRed);
  h_wbck->SetFillColor(kCyan);
  h_wbck->SetLineColor(kBlue);
  h_wall->SetFillColor(kGray);
  h_wall->SetLineColor(kBlack);

  h_wall->DrawCopy("same hist");
  h_wbck->DrawCopy("same hist");
  h_wsig->DrawCopy("same hist");

  TH1D* h_wbck_lin = new TH1D(*h_wbck);
  h_wbck_lin->SetFillColor(0);
  h_wbck_lin->DrawCopy("same hist");

  TH1D* h_wall_lin = new TH1D(*h_wall);
  h_wall_lin->SetFillColor(0);
  h_wall_lin->DrawCopy("same hist");

  _frame->DrawCopy("same axis");

  TLegend* leg = new TLegend(0.8,0.75,1-gStyle->GetPadRightMargin()-0.025,1-gStyle->GetPadTopMargin()-0.05);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  TLegendEntry* l1 = leg->AddEntry(h_wall,"all jets","lf");     l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(h_wsig,"signal jets","lf");  l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_wbck,"pile-up jets","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("Simulation internal");
  HistHelper::Draw::processing(fname);

}
