
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

void plotNC(const std::string& fname,const std::string& htag="")
{
  ReadMLFile indata(fname);
  printf("plotNC - read %zu records\n",indata.size());

  double xl(-0.5); double dx(2.); int nb(140); double xh(xl+(double)nb*dx);
  TH1D* h_ncall = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ncall",true).c_str(),"N_{const}^{jet} (all jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_ncall").c_str(),"N_{const}^{jet} (all jets)",    nb,xl,xh);
  TH1D* h_ncsig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ncsig",true).c_str(),"N_{const}^{jet} (signal jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_ncsig").c_str(),"N_{const}^{jet} (signal jets)",    nb,xl,xh);
  TH1D* h_ncbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ncbck",true).c_str(),"N_{const}^{jet} (pile-up jets)",nb,xl,xh)
    : new TH1D(HistHelper::Names::histName(fname,"h_ncbck").c_str(),"N_{const}^{jet} (pile-up jets)",    nb,xl,xh);

  
  auto fiter = indata.begin();
  size_t i(1);
  for ( ; fiter != indata.end(); ++fiter ) { 
    double nc(indata.nc(fiter));
    h_ncall->Fill(nc);
    if ( indata.tag(fiter) == 1 ) {
      h_ncsig->Fill(nc);
    } else {
      h_ncbck->Fill(nc);
    }
  }

  HistHelper::Hist::realign(h_ncall);
  HistHelper::Hist::realign(h_ncsig);
  HistHelper::Hist::realign(h_ncbck);

  TH1D* _frame = new TH1D(HistHelper::Names::histName(fname,"_frame").c_str(),"_frame",h_ncall->GetNbinsX(),h_ncall->GetXaxis()->GetXmin()-dx,h_ncall->GetXaxis()->GetXmax()+dx);
  _frame->SetMaximum(1000000.);
  _frame->SetMinimum(0.5);
  _frame->GetXaxis()->SetTitle("N_{const}^{jet}");
  _frame->GetYaxis()->SetTitle(TString::Format("#partialN/#partialN_{const}^{jet} [1/%.0f]",h_ncall->GetBinWidth(1)).Data());

  _frame->DrawCopy("axis");

  h_ncsig->SetFillColor(kYellow);
  h_ncsig->SetLineColor(kRed);
  h_ncbck->SetFillColor(kCyan);
  h_ncbck->SetLineColor(kBlue);
  h_ncall->SetFillColor(kGray);
  h_ncall->SetLineColor(kBlack);

  h_ncall->DrawCopy("same hist");
  h_ncbck->DrawCopy("same hist");
  h_ncsig->DrawCopy("same hist");

  TH1D* h_ncbck_lin = new TH1D(*h_ncbck);
  h_ncbck_lin->SetFillColor(0);
  h_ncbck_lin->DrawCopy("same hist");

  TH1D* h_ncall_lin = new TH1D(*h_ncall);
  h_ncall_lin->SetFillColor(0);
  h_ncall_lin->DrawCopy("same hist");

  _frame->DrawCopy("same axis");

  TLegend* leg = new TLegend(0.8,0.75,1-gStyle->GetPadRightMargin()-0.025,1-gStyle->GetPadTopMargin()-0.05);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  TLegendEntry* l1 = leg->AddEntry(h_ncall,"all jets","lf");     l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(h_ncsig,"signal jets","lf");  l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_ncbck,"pile-up jets","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("Simulation internal");
  HistHelper::Draw::processing(fname);

}
