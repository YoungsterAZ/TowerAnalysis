
//#include "AtlasUtils.C"
#include "HistHelper.h"

#include <TCanvas.h>
#include <TH1D.h>
#include <TString.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TF1.h>

#include <vector>
#include <string>
#include <cmath>

#include "efffct.C"

void plotEff(const std::string& ystr,const std::string& fname="LHspectra.root")
{

  TFile* file = new TFile(fname.c_str(),"READ");
  if ( file == 0 || !file->IsOpen() ) { 
    printf("plotEff - cannot access file \042%s\042\n",fname.c_str());
    return;
  }

  gStyle->SetPadRightMargin(0.05);

#include "HistTags.icc"

  std::string hsname("h_lsig");
  std::string hbname("h_lbck");

  std::vector<std::string> signtags = { "topoclus", "topotowf", "topotow" };
  std::vector<std::string> proctags = { "INCL",     "TIME"                }; 
  std::vector<std::string> putags   = { "JAPU",     "CSPU",     "CSSK"    };

  std::vector<int> signcols = { kBlack, kBlue, kRed    };
  std::vector<int> signfcls = { kGray,  kCyan, kYellow };
  std::vector<int> pumode   = { 1, 1, 1 };

  std::vector<double> probthresh;
  double xstp(0.0001); 
  double xval(xstp);
  while  ( xval < 1. ) { probthresh.push_back(xval); xval += xstp; }

  // This script produces two plots, one for each signal processing option ("INCL","TIME").
  // Each plot contains nine graphs, three for each of the three signal types.  
  TH1D* _frame = new TH1D("_frame","_frame",100,0.25,1.05);
  _frame->SetMaximum(1.2);
  //  _frame->SetMinimum(0.0000001);
  _frame->SetMinimum(-0.05);
  _frame->GetXaxis()->SetTitle("#epsilon_{signal jet}(P_{signal} > P_{threshold})");
  _frame->GetYaxis()->SetTitle("#epsilon_{pile-up jet}(P_{signal} > P_{threshold})");

  // loop all signal processing
  for ( size_t ii(0); ii<proctags.size(); ++ii ) {
    // loop all pile-up suppression techniques
    for ( size_t ik(0); ik<putags.size(); ++ik ) {
      int icolor(0); int iline(pumode.at(ik)); int fcolor(0);
      // loop all pile-up suppression techniques 
      std::vector<TGraphErrors*> graphs;
      std::string ts(TString::Format("ROC_Curves_%s_%s+%s",ystr.c_str(),putags.at(ik).c_str(),proctags.at(ii).c_str()).Data()); 
      TCanvas* aa = new TCanvas(ts.c_str(),ts.c_str());
      aa->SetGridx(); aa->SetGridy();
      _frame->DrawCopy("axis");
      TH1D* h_s = 0;
      TH1D* h_b = 0;
      // loop calorimeter signal definitions
      for ( size_t ij(0); ij<signtags.size(); ++ij ) {
	icolor = signcols.at(ij); fcolor = signfcls.at(ij);
	std::string sstr(TString::Format("%s+%s/h_lsig_%s_%s",putags.at(ik).c_str(),proctags.at(ii).c_str(),signtags.at(ij).c_str(),ystr.c_str()).Data());
	std::string bstr(TString::Format("%s+%s/h_lbck_%s_%s",putags.at(ik).c_str(),proctags.at(ii).c_str(),signtags.at(ij).c_str(),ystr.c_str()).Data());
	h_s = (TH1D*)file->FindObjectAny(sstr.c_str());
	h_b = (TH1D*)file->FindObjectAny(bstr.c_str());
	if ( h_s == 0 || h_b == 0 ) { 
	  printf("plotEff - cannot find histograms \042%s\042 and/or \042%s\042\n",sstr.c_str(),bstr.c_str());
	  continue;
	}
	// prepare workspace
	double* xv = new double[h_s->GetNbinsX()]; double* xd = new double[h_s->GetNbinsX()];
	double* yv = new double[h_b->GetNbinsX()]; double* yd = new double[h_b->GetNbinsX()];
	double  sd(0.); double bd(0.);
	double  si = h_s->IntegralAndError(1,h_s->GetNbinsX(),sd);
	double  bi = h_b->IntegralAndError(1,h_b->GetNbinsX(),bd);
	// calculate efficiencies
	for ( int kp(1); kp<=h_s->GetNbinsX(); ++kp ) { 
	  double ds(0.); double db(0.);
	  double fs = h_s->IntegralAndError(kp,h_s->GetNbinsX(),ds);
	  double fb = h_b->IntegralAndError(kp,h_b->GetNbinsX(),db);
	  int il(kp-1);
	  xv[il] = fs/si; if ( fs > 0. ) { xd[il] = std::sqrt(std::pow(ds/fs,2)+std::pow(sd/si,2))*xv[il]; } else { xd[il] = 0.; } 
	  yv[il] = fb/bi; if ( fb > 0. ) { yd[il] = std::sqrt(std::pow(db/fb,2)+std::pow(bd/bi,2))*yv[il]; } else { yd[il] = 0.; }
	} // probability thresholds
	graphs.push_back(new TGraphErrors(h_s->GetNbinsX(),xv,yv,xd,yd));
	graphs.back()->SetLineColor(icolor);
	graphs.back()->SetFillColor(fcolor);
	graphs.back()->SetLineStyle(iline);
	graphs.back()->SetLineWidth(2);
	delete[] xv;
	delete[] yv;
	delete[] xd;
	delete[] yd;
      } // signal definitions
      if ( h_s != 0 && h_b != 0 ) {
	double x = gStyle->GetPadLeftMargin();    x += 0.050; 
	double y = 0.5;
	TLegend* leg = new TLegend(x,y,x+0.3,y+0.15);
	leg->SetTextFont(42);
	for ( size_t is(0); is<graphs.size(); ++is ) {
	  TGraphErrors* pg = graphs.at(is);
	  pg->SetMarkerStyle(20);
	  pg->SetMarkerSize(0.75);
	  pg->SetMarkerColor(pg->GetLineColor());
	  pg->Draw("px same");
	  HistHelper::Data::writeGraph(pg,TString::Format("ROCCurves/%s_%s.dat",aa->GetName(),signtags.at(is).c_str()).Data());
	  leg->AddEntry(pg,HistHelper::Names::signalName(signtags.at(is)).c_str(),"l")->SetTextColor(pg->GetLineColor());
	}
	leg->Draw("same");    
	_frame->DrawCopy("same axig");
	printf("plotEff - extract descriptors from histogram \042%s\042\n",h_s->GetName());
	HistHelper::Draw::description(ystr);
	HistHelper::Draw::AtlasLabel("Simulation internal");
	HistHelper::Draw::processingNoSignal(h_s->GetName());
	aa->Print(TString::Format("ROCCurves/%s.pdf",aa->GetName()));
	aa->Print(TString::Format("ROCCurves/%s.png",aa->GetName()));
      } else {
	delete aa; // remove empty TCanvas
      } // plotting and output
    } // pile-up suppression
  } // signal processing

  delete _frame;

}
