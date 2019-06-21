
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TColor.h>

#include <string>
#include <cstdio>


void plotTiming(int mode=0) {

  double rm(gStyle->GetPadRightMargin());
  gStyle->SetPadRightMargin(0.05);

  std::string fname;
  std::string fout;
  std::string sstr;

  switch ( mode ) {
  case 0:
    printf("plotTiming - analysing topo-clusters\n");
    fname = "JAPU+TIME/vbf2600_topoclus_analysis_hists.root";
    fout  = "topoclus";
    sstr  = "topo-cluster";
    break;
  case 1:
    printf("plotTiming - analysing topo-towers 0.1 x 0.1\n");
    fname = "JAPU+TIME/vbf2600_topotow_analysis_hists.root";
    fout  = "topotow";
    sstr  = "topo-tower 0.1#times0.1";
    break;
  case 2:
    printf("plotTiming - analysing topo-towers 0.05 x 0.05\n");
    fname = "JAPU+TIME/vbf2600_topotowf_analysis_hists.root";
    fout  = "topotowf";
    sstr  = "topo-tower 0.05#times0.05";
    break;
  default:
    printf("plotTiming - unknown plotting mode %i, fatal!\n",mode);
    return;
  }

  TFile* f_inp = new TFile(fname.c_str(),"READ");
 
  if ( f_inp == 0 ) { 
    printf("plotTiming - could not open one of the file \042%s\042, fatal\n",fname.c_str());
    return;
  }
 
  std::string hname_incl("AfterPU/CaloSignal/Incl/h_tsig_incl");
  std::string hname_filt("AfterPU/CaloSignal/Filt/h_tsig_epos");
  std::string hname_tneg("AfterPU/CaloSignal/Incl/h_tsig_eneg");

  TH1D* h_incl = (TH1D*)f_inp->FindObjectAny(hname_incl.c_str());
  TH1D* h_time = (TH1D*)f_inp->FindObjectAny(hname_filt.c_str());
  TH1D* h_tneg = (TH1D*)f_inp->FindObjectAny(hname_tneg.c_str());

  bool fail(false);
  if ( h_incl == 0 ) { fail = true; printf("plotTiming - cannot find histogram \042%s\042 (inclusive signal timing)\n",hname_incl.c_str()); }
  if ( h_tneg == 0 ) { fail = true; printf("plotTiming - cannot find histogram \042%s\042 (E<0,inclusive signal timing)\n",hname_tneg.c_str()); }
  if ( h_time == 0 ) { fail = true; printf("plotTiming - cannot find histogram \042%s\042 (E>0,filtered signal timing)\n",hname_filt.c_str()); }
  if ( fail ) { return; }

  TCanvas* aa = new TCanvas("SignalTiming","Signal timing");
  aa->SetLogy();

  TH1D* _frame = new TH1D("_frame","_frame",h_incl->GetNbinsX(),h_incl->GetXaxis()->GetXmin(),h_incl->GetXaxis()->GetXmax());
  _frame->GetXaxis()->SetTitle("t_{signal} [ns]");
  if ( h_incl->GetBinWidth(1) == 1. ) { 
    _frame->GetYaxis()->SetTitle("#partialN/#partialt [ns^{#minus1}]");
  } else { 
    _frame->GetYaxis()->SetTitle(TString::Format("#partialN/#partialt [1/(%.1f ns)]",h_incl->GetBinWidth(1)).Data());
  }
  _frame->SetMinimum(0.5);
  _frame->SetMaximum(10000000.);
  
  _frame->DrawCopy("axis");

  h_incl->SetLineColor(kRed+4);
  h_incl->SetFillColor(kYellow-10);

  h_tneg->SetLineColor(kBlue);
  h_tneg->SetFillColor(kCyan);

  h_time->SetLineColor(kRed);
  h_time->SetFillColor(kYellow);
  //  h_incl->SetFillColor(kCyan);
  //  h_time->SetLineColor(kRed);
  //  h_time->SetLineWidth(1);
  // h_time->SetFillColor(kRed+4);
  // h_time->SetFillStyle(3245);

  int fb = h_incl->FindFirstBinAbove(0.);
  int lb = h_incl->FindLastBinAbove(0.);
  if ( fb > 1 || lb < h_incl->GetNbinsX() ) { 
    double xl = h_incl->GetXaxis()->GetBinLowEdge(fb);
    double xh = h_incl->GetXaxis()->GetBinUpEdge(lb);
    h_incl->GetXaxis()->SetRangeUser(xl,xh);
  }

  fb = h_time->FindFirstBinAbove(0.);
  lb = h_time->FindLastBinAbove(0.);
  if ( fb > 1 || lb < h_time->GetNbinsX() ) { 
    double xl = h_time->GetXaxis()->GetBinLowEdge(fb);
    double xh = h_time->GetXaxis()->GetBinUpEdge(lb);
    h_time->GetXaxis()->SetRangeUser(xl,xh);
  }

  fb = h_tneg->FindFirstBinAbove(0.);
  lb = h_tneg->FindLastBinAbove(0.);
  if ( fb > 1 || lb < h_tneg->GetNbinsX() ) { 
    double xl = h_tneg->GetXaxis()->GetBinLowEdge(fb);
    double xh = h_tneg->GetXaxis()->GetBinUpEdge(lb);
    h_tneg->GetXaxis()->SetRangeUser(xl,xh);
  }

  h_incl->DrawCopy("same hist");
  h_tneg->DrawCopy("same hist");
  h_time->DrawCopy("same hist");

  TH1D* h_incl_ovly = new TH1D(*h_incl);
  h_incl_ovly->SetFillColor(0);
  h_incl_ovly->DrawCopy("same hist");

  TH1D* h_tneg_ovly = new TH1D(*h_tneg);
  h_tneg_ovly->SetFillColor(0);
  h_tneg_ovly->DrawCopy("same hist");

  _frame->DrawCopy("same axis");

  TLatex tl;
  int of(tl.GetTextFont());
  double os(tl.GetTextSize());

  tl.SetNDC(); tl.SetTextFont(72); tl.SetTextAlign(13);
  double xlab = gStyle->GetPadLeftMargin();    xlab += 0.050; 
  double ylab = 1.0-gStyle->GetPadTopMargin(); ylab -= 0.025;
  tl.DrawLatex(xlab,ylab,"ATLAS #font[42]{Simulation internal}"); 
  
  tl.SetTextFont(42); tl.SetTextSize(os/1.75);
  double ydes(0.875);
  tl.DrawLatex(xlab,ydes,TString::Format("%s (full events)",sstr.c_str()).Data());

  double xhead(1.0-gStyle->GetPadRightMargin());
  double yhead(0.997); 

  tl.SetTextAlign(33); tl.SetTextSize(os*2./3.); tl.SetTextFont(of);
  tl.DrawLatex(xhead,yhead,"Pythia8 VBF M = 2.6 TeV at #sqrt{s} = 14 TeV, #LT#mu#GT = 200");

  TLegend* leg = new TLegend(0.7,0.65,1-gStyle->GetPadRightMargin()-0.025,1-gStyle->GetPadTopMargin()-0.05);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  TLegendEntry* l1 = leg->AddEntry(h_incl,"all signals",         "lf"); l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(h_time,"E > 0, time filtered", "lf"); l2->SetTextFont(42);
  TLegendEntry* l3 = leg->AddEntry(h_tneg,"E < 0, no time filter","lf"); l3->SetTextFont(42);
  leg->Draw("same");

  aa->Print(TString::Format("timing_%s.pdf",fout.c_str()).Data());
  aa->Print(TString::Format("timing_%s.png",fout.c_str()).Data());
  
  std::string dname_tpos("AfterPU/CaloSignal/Incl/d_tsig_rap_epos");
  std::string dname_tneg("AfterPU/CaloSignal/Incl/d_tsig_rap_eneg");

  TH2D* d_tpos = (TH2D*)f_inp->FindObjectAny(dname_tpos.c_str());
  if ( d_tpos == 0 ) { printf("plotTiming - cannot find histogram \042%s\042 (E > 0 signal timing versus eta)\n",dname_tpos.c_str()); return; }
  d_tpos->Scale(1./(d_tpos->GetXaxis()->GetBinWidth(1)*d_tpos->GetYaxis()->GetBinWidth(1)));

  double zmin(1.); double zmax(10.*d_tpos->GetMaximum());
  d_tpos->GetZaxis()->SetRangeUser(zmin,zmax);
  d_tpos->GetZaxis()->SetTitleOffset(1.125*d_tpos->GetZaxis()->GetTitleOffset());

  TH2D* d_tneg = (TH2D*)f_inp->FindObjectAny(dname_tneg.c_str());
  if ( d_tneg == 0 ) { printf("plotTiming - cannot find histogram \042%s\042 (E < 0 signal timing versus eta)\n",dname_tneg.c_str()); return; }
  d_tneg->Scale(1./(d_tneg->GetXaxis()->GetBinWidth(1)*d_tneg->GetYaxis()->GetBinWidth(1)));

  d_tneg->GetZaxis()->SetRangeUser(zmin,zmax);
  d_tneg->GetZaxis()->SetTitleOffset(1.125*d_tneg->GetZaxis()->GetTitleOffset());

  double mr(gStyle->GetPadRightMargin());
  gStyle->SetPadRightMargin(rm+0.01);
  gStyle->SetPalette(kDarkBodyRadiator);
  double md(std::abs(gStyle->GetPadRightMargin()-mr));

  TCanvas* bb = new TCanvas("SignalPosTimingVsEta","Signal E > 0 timing versus rapidity");
  bb->SetLogz();

  TH2D* _frame2D = new TH2D("_frame2D","_frame2D",100,d_tpos->GetXaxis()->GetXmin(),d_tpos->GetXaxis()->GetXmax(),100,d_tpos->GetYaxis()->GetXmin(),d_tpos->GetYaxis()->GetXmax()+50.);
  _frame2D->GetZaxis()->SetRangeUser(zmin,zmax);
  _frame2D->GetXaxis()->SetTitle("#eta_{calo}");
  _frame2D->GetYaxis()->SetTitle("t_{signal} [ns]");
  _frame2D->DrawCopy("axis");

  TBox* bx = new TBox(_frame2D->GetXaxis()->GetXmin(),_frame2D->GetYaxis()->GetXmin(),_frame2D->GetXaxis()->GetXmax(),_frame2D->GetYaxis()->GetXmax());
  bx->SetLineWidth(0);
  bx->SetFillColor(19);
  bx->SetFillStyle(1001);

  bx->Draw("same");

  d_tpos->GetZaxis()->SetTitle("#partial^{2}N/(#partial#eta#partialt_{signal}) [ns^{#minus1}]");
  d_tpos->DrawCopy("same colz");

  tl.SetTextSize(os); tl.SetTextFont(72); tl.SetTextAlign(13);
  tl.DrawLatex(xlab,ylab,"ATLAS #font[42]{Simulation internal}"); 
  
  tl.SetTextFont(42); tl.SetTextSize(os/1.75);
  tl.DrawLatex(xlab,ydes,TString::Format("%s, E > 0",sstr.c_str()).Data());

  tl.SetTextAlign(33); tl.SetTextSize(os*2./3.); tl.SetTextFont(of);
  xhead -= md;
  tl.DrawLatex(xhead,yhead,"Pythia8 VBF M = 2.6 TeV at #sqrt{s} = 14 TeV, #LT#mu#GT = 200");

  _frame2D->DrawCopy("same axis");

  bb->Print(TString::Format("timing_%s_eta_pos.pdf",fout.c_str()).Data());
  bb->Print(TString::Format("timing_%s_eta_pos.png",fout.c_str()).Data());

  TCanvas* cc = new TCanvas("SignalNegTimingVsEta","Signal E < 0 timing versus rapidity");
  cc->SetLogz();

  _frame2D->DrawCopy("axis");
  bx->Draw("same");
  d_tneg->GetZaxis()->SetTitle("#partial^{2}N/(#partial#eta#partialt_{signal}) [ns^{#minus1}]");
  d_tneg->DrawCopy("same colz");

  tl.SetTextSize(os); tl.SetTextFont(72); tl.SetTextAlign(13);
  tl.DrawLatex(xlab,ylab,"ATLAS #font[42]{Simulation internal}"); 
  
  tl.SetTextFont(42); tl.SetTextSize(os/1.75);
  tl.DrawLatex(xlab,ydes,TString::Format("%s, E < 0",sstr.c_str()).Data());

  tl.SetTextAlign(33); tl.SetTextSize(os*2./3.); tl.SetTextFont(of);
  tl.DrawLatex(xhead,yhead,"Pythia8 VBF M = 2.6 TeV at #sqrt{s} = 14 TeV, #LT#mu#GT = 200");

  _frame2D->DrawCopy("same axis");

  cc->Print(TString::Format("timing_%s_eta_neg.pdf",fout.c_str()).Data());
  cc->Print(TString::Format("timing_%s_eta_neg.png",fout.c_str()).Data());

}
