
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

void plotPtD(const std::string& fname,const std::string& htag="")
{
  ReadMLFile indata(fname);
  printf("plotPtD - read %zu records\n",indata.size());

  double xl(-0.01); double dx(0.02); int nb(52); double xh(xl+(double)nb*dx);
  TH1D* h_ptdall = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptdall",true).c_str(),"P_{T}^{D} (all jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptdall").c_str(),"P_{T}^{D} (all jets)",    nb,xl,xh);
  TH1D* h_ptdsig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptdsig",true).c_str(),"P_{T}^{D} (signal jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptdsig").c_str(),"P_{T}^{D} (signal jets)",    nb,xl,xh);
  TH1D* h_ptdbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptdbck",true).c_str(),"P_{T}^{D} (pile-up jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptdbck").c_str(),"P_{T}^{D} (pile-up jets)",    nb,xl,xh);
  
  auto fiter = indata.begin();
  size_t i(1);
  for ( ; fiter != indata.end(); ++fiter ) { 
    double ptd(indata.ptd(fiter));
    h_ptdall->Fill(ptd);
    if ( indata.tag(fiter) == 1 ) {
      h_ptdsig->Fill(ptd);
    } else {
      h_ptdbck->Fill(ptd);
    }
  }

  HistHelper::Hist::realign(h_ptdall);
  HistHelper::Hist::realign(h_ptdsig);
  HistHelper::Hist::realign(h_ptdbck);

  TH1D* _frame = new TH1D(HistHelper::Names::histName(fname,"_frame").c_str(),"_frame",h_ptdall->GetNbinsX(),h_ptdall->GetXaxis()->GetXmin()-dx,h_ptdall->GetXaxis()->GetXmax()+dx);
  _frame->SetMaximum(1000000.);
  _frame->SetMinimum(0.5);
  _frame->GetXaxis()->SetTitle("P_{T}^{D}");
  _frame->GetYaxis()->SetTitle(TString::Format("#partialN/#partialP_{T}^{D} [1/%.2f]",h_ptdall->GetBinWidth(1)).Data());

  _frame->DrawCopy("axis");

  h_ptdsig->SetFillColor(kYellow);
  h_ptdsig->SetLineColor(kRed);
  h_ptdbck->SetFillColor(kCyan);
  h_ptdbck->SetLineColor(kBlue);
  h_ptdall->SetFillColor(kGray);
  h_ptdall->SetLineColor(kBlack);

  h_ptdall->DrawCopy("same hist");
  h_ptdbck->DrawCopy("same hist");
  h_ptdsig->DrawCopy("same hist");

  TH1D* h_ptdbck_lin = new TH1D(*h_ptdbck);
  h_ptdbck_lin->SetFillColor(0);
  h_ptdbck_lin->DrawCopy("same hist");

  TH1D* h_ptdall_lin = new TH1D(*h_ptdall);
  h_ptdall_lin->SetFillColor(0);
  h_ptdall_lin->DrawCopy("same hist");

  _frame->DrawCopy("same axis");

  TLegend* leg = new TLegend(0.8,0.75,1-gStyle->GetPadRightMargin()-0.025,1-gStyle->GetPadTopMargin()-0.05);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  TLegendEntry* l1 = leg->AddEntry(h_ptdall,"all jets","lf");     l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(h_ptdsig,"signal jets","lf");  l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_ptdbck,"pile-up jets","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("Simulation internal");
  HistHelper::Draw::processing(fname);

}
