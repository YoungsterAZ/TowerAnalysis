
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

void plotPt(const std::string& fname,const std::string& htag="")
{
  ReadMLFile indata(fname);
  printf("plotPt - read %zu records\n",indata.size());

  TH1D* h_ptall = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptall",true).c_str(),"p_{T}^{jet} (all jets)",75,0.,1500.)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptall").c_str(),"p_{T}^{jet} (all jets)",    75,0.,1500.);
  TH1D* h_ptsig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptsig",true).c_str(),"p_{T}^{jet} (signal jets)",75,0.,1500.)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptsig").c_str(),"p_{T}^{jet} (signal jets)",    75,0.,1500.);
  TH1D* h_ptbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptbck",true).c_str(),"p_{T}^{jet} (pile-up jets)",75,0.,1500.)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptbck").c_str(),"p_{T}^{jet} (pile-up jets)",    75,0.,1500.);
  
  auto fiter = indata.begin();
  size_t i(1);
  for ( ; fiter != indata.end(); ++fiter ) { 
    double pt(indata.pt(fiter));
    h_ptall->Fill(pt);
    if ( indata.tag(fiter) == 1 ) {
      h_ptsig->Fill(pt);
    } else {
      h_ptbck->Fill(pt);
    }
  }

  HistHelper::Hist::realign(h_ptall);
  HistHelper::Hist::realign(h_ptsig);
  HistHelper::Hist::realign(h_ptbck);

  TH1D* _frame = new TH1D(HistHelper::Names::histName(fname,"_frame").c_str(),"_frame",h_ptall->GetNbinsX(),h_ptall->GetXaxis()->GetXmin(),h_ptall->GetXaxis()->GetXmax());
  _frame->SetMaximum(500000.);
  _frame->SetMinimum(0.5);
  _frame->GetXaxis()->SetTitle("p_{T} [GeV]");
  _frame->GetYaxis()->SetTitle(TString::Format("#partialN/#partialp_{T} [1/(%.1f GeV)]",h_ptall->GetBinWidth(1)).Data());

  _frame->DrawCopy("axis");

  h_ptsig->SetFillColor(kYellow);
  h_ptsig->SetLineColor(kRed);
  h_ptbck->SetFillColor(kCyan);
  h_ptbck->SetLineColor(kBlue);
  h_ptall->SetFillColor(kGray);
  h_ptall->SetLineColor(kBlack);

  h_ptall->DrawCopy("same hist");
  h_ptbck->DrawCopy("same hist");
  h_ptsig->DrawCopy("same hist");

  TH1D* h_ptbck_lin = new TH1D(*h_ptbck);
  h_ptbck_lin->SetFillColor(0);
  h_ptbck_lin->DrawCopy("same hist");

  TH1D* h_ptall_lin = new TH1D(*h_ptall);
  h_ptall_lin->SetFillColor(0);
  h_ptall_lin->DrawCopy("same hist");

  _frame->DrawCopy("same axis");

  TLegend* leg = new TLegend(0.8,0.75,1-gStyle->GetPadRightMargin()-0.025,1-gStyle->GetPadTopMargin()-0.05);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  TLegendEntry* l1 = leg->AddEntry(h_ptall,"all jets","lf");     l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(h_ptsig,"signal jets","lf");  l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_ptbck,"pile-up jets","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("Simulation internal");
  HistHelper::Draw::processing(fname);

}
