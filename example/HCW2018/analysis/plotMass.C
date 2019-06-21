
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

void plotMass(const std::string& fname,const std::string& htag="")
{
  ReadMLFile indata(fname);
  printf("plotMass - read %zu records\n",indata.size());

  TH1D* h_mall = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_mall",true).c_str(),"m^{jet} (all jets)",50,0.,100.)
    : new TH1D(HistHelper::Names::histName(fname,"h_mall").c_str(),"m^{jet} (all jets)",    50,0.,100.);
  TH1D* h_msig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_msig",true).c_str(),"m^{jet} (signal jets)",50,0.,100.)
    : new TH1D(HistHelper::Names::histName(fname,"h_msig").c_str(),"m^{jet} (signal jets)",    50,0.,100.);
  TH1D* h_mbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_mbck",true).c_str(),"m^{jet} (pile-up jets)",50,0.,100.)
    : new TH1D(HistHelper::Names::histName(fname,"h_mbck").c_str(),"m^{jet} (pile-up jets)",    50,0.,100.);
  
  auto fiter = indata.begin();
  size_t i(1);
  for ( ; fiter != indata.end(); ++fiter ) { 
    double m(indata.m(fiter));
    h_mall->Fill(m);
    if ( indata.tag(fiter) == 1 ) {
      h_msig->Fill(m);
    } else {
      h_mbck->Fill(m);
    }
  }

  HistHelper::Hist::realign(h_mall);
  HistHelper::Hist::realign(h_msig);
  HistHelper::Hist::realign(h_mbck);

  TH1D* _frame = new TH1D(HistHelper::Names::histName(fname,"_frame").c_str(),"_frame",h_mall->GetNbinsX(),h_mall->GetXaxis()->GetXmin(),h_mall->GetXaxis()->GetXmax());
  _frame->SetMaximum(1000000.);
  _frame->SetMinimum(0.5);
  _frame->GetXaxis()->SetTitle("m^{jet} [GeV]");
  _frame->GetYaxis()->SetTitle(TString::Format("#partialN/#partialm^{jet} [1/(%.1f GeV)]",h_mall->GetBinWidth(1)).Data());

  _frame->DrawCopy("axis");

  h_msig->SetFillColor(kYellow);
  h_msig->SetLineColor(kRed);
  h_mbck->SetFillColor(kCyan);
  h_mbck->SetLineColor(kBlue);
  h_mall->SetFillColor(kGray);
  h_mall->SetLineColor(kBlack);

  h_mall->DrawCopy("same hist");
  h_mbck->DrawCopy("same hist");
  h_msig->DrawCopy("same hist");

  TH1D* h_mbck_lin = new TH1D(*h_mbck);
  h_mbck_lin->SetFillColor(0);
  h_mbck_lin->DrawCopy("same hist");

  TH1D* h_mall_lin = new TH1D(*h_mall);
  h_mall_lin->SetFillColor(0);
  h_mall_lin->DrawCopy("same hist");

  _frame->DrawCopy("same axis");

  TLegend* leg = new TLegend(0.8,0.75,1-gStyle->GetPadRightMargin()-0.025,1-gStyle->GetPadTopMargin()-0.05);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  TLegendEntry* l1 = leg->AddEntry(h_mall,"all jets","lf");     l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(h_msig,"signal jets","lf");  l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_mbck,"pile-up jets","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("Simulation internal");
  HistHelper::Draw::processing(fname);

}
