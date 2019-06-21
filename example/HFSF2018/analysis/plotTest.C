
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

void plotTest()
{
  std::string fname("ML_Results/JAPU+INCL/vbf2600_topoclus_00_15.csv");
  ReadMLFile indata(fname);
  printf("plotTest - read %zu records\n",indata.size());

  TCanvas* aa = new TCanvas("pt","pt");
  aa->SetLogy();

  TH1D* h_ptall = new TH1D("h_ptall","h_ptall",75,0.,1500.);
  TH1D* h_ptsig = new TH1D("h_ptsig","h_ptsig",75,0.,1500.);
  TH1D* h_ptbck = new TH1D("h_ptbck","h_ptbck",75,0.,1500.);
  
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

  TH1D* _frame = new TH1D("_frame","_frame",h_ptall->GetNbinsX(),h_ptall->GetXaxis()->GetXmin(),h_ptall->GetXaxis()->GetXmax());
  _frame->SetMaximum(50000.);
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

  TLegend* leg = new TLegend(0.6,0.7,1-gStyle->GetPadRightMargin()-0.05,1-gStyle->GetPadTopMargin()-0.05);
  TLegendEntry* l1 = leg->AddEntry(h_ptall,"all jets","lf");     l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(h_ptsig,"signal jets","lf");  l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_ptbck,"pile-up jets","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  TH1D* h_ptbck_lin = new TH1D(*h_ptbck);
  h_ptbck_lin->SetFillColor(0);
  h_ptbck_lin->DrawCopy("same hist");

  TH1D* h_ptall_lin = new TH1D(*h_ptall);
  h_ptall_lin->SetFillColor(0);
  h_ptall_lin->DrawCopy("same hist");

  printf("plotTest - plot description\n");
  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("simulation internal");

  std::string sn; std::string pn; std::string mn;
  if ( HistHelper::Names::extractAllNames("ML_Results/JAPU+INCL/vbf2600_topoclus_00_15.csv",sn,pn,mn) ) { 
    printf("plotTest - found:\n");
    printf("           signal ................ : \042%s\042\n",sn.c_str());
    printf("           pile-up suppression ... : \042%s\042\n",pn.c_str());
    printf("           signal filter mode .... : \042%s\042\n",mn.c_str());
    //    HistHelper::Draw::processing(sn,pn,mn);
    HistHelper::Draw::processing("ML_Results/JAPU+INCL/vbf2600_topoclus_00_15.csv");
  }

  _frame->DrawCopy("same axis");
}
