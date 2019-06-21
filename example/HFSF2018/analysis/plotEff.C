
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

// Mode:   Was is plotted?
//
//  0      ROC curve with truth knowledge (fraction of true pile-up jets with P>Pmin as function of fraction of true signal jets at same P)
//  1      Fraction of all/signal/pile-up jets as function of Pmin (normalization is total number of jets for all three cases)
//  2      Fraction of signal/pile-up jets as function of fraction of all jets with P>Pmin
//  3      Fraction of signal/pile-up jets as function of Pmin (normalization is total number of jets with P>Pmin)   

void plotEff(const std::string& ystr,const std::string& fname="LHspectra.root",int mode=0)
{

  printf("plotEff(\042%s\042,\042%s\042,%i) called\n",ystr.c_str(),fname.c_str(),mode);

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

  bool prn(true);

  // This script produces two plots, one for each signal processing option ("INCL","TIME").
  // Each plot contains nine graphs, three for each of the three signal types.  
  double xmin(-0.05); double xmax(1.05); double ymin(-0.05); double ymax(1.2);
  std::string xtit("#epsilon_{signal jet}(P_{signal} #geq P_{min})");
  std::string ytit("#epsilon_{pile-up jet}(P_{signal} #geq P_{min})");
  if ( mode == 1 ) { 
    xmin = -0.05; ymax = 1.5; 
    xtit = "Analysis selection P_{min}";
    ytit = "Fraction of jets with P_{signal} #geq P_{min}";
  } else if ( mode == 2 ) { 
    xmin = -0.05; ymax = 1.75; 
    xtit = "N_{all jets}(P_{signal} #geq P_{min})/N_{all jets}(P #geq 0)";
    ytit = "#left(N_{signal(pile-up) jets}/N_{all jets}#right)#left(P_{signal} #geq P_{min}#right)";
  } else if ( mode == 3 ) {
    xmin = -0.05; ymax = 1.75;
    xtit = "Analysis selection P_{min}";
    ytit = "#left(N_{signal(pile-up) jets}/N_{all jets}#right)#left(P_{signal} #geq P_{min}#right)";
  }
  TH1D* _frame = new TH1D("_frame","_frame",100,xmin,xmax);
  //  _frame->SetMinimum(0.0000001);
  _frame->SetMinimum(ymin);
  _frame->SetMaximum(ymax);
  _frame->GetXaxis()->SetTitle(xtit.c_str());
  _frame->GetYaxis()->SetTitle(ytit.c_str());

  // loop all signal processing
  for ( size_t ii(0); ii<proctags.size(); ++ii ) {
    // loop all pile-up suppression techniques
    for ( size_t ik(0); ik<putags.size(); ++ik ) {
      int icolor(0); int iline(pumode.at(ik)); int fcolor(0);
      // loop all pile-up suppression techniques 
      std::vector<TGraphErrors*> agraphs;
      std::vector<TGraphErrors*> fgraphs;
      std::vector<TGraphErrors*> bgraphs;
      std::string ts = mode == 0 
	? TString::Format("ROC_Curves_%s_%s+%s",ystr.c_str(),putags.at(ik).c_str(),proctags.at(ii).c_str()).Data()
	: mode == 1 
	? TString::Format("ROC_Curves_%s_%s+%s_full",ystr.c_str(),putags.at(ik).c_str(),proctags.at(ii).c_str()).Data()
	: mode == 2 
	? TString::Format("ROC_Curves_%s_%s+%s_frac",ystr.c_str(),putags.at(ik).c_str(),proctags.at(ii).c_str()).Data()
	: TString::Format("ROC_Curves_%s_%s+%s_fpts",ystr.c_str(),putags.at(ik).c_str(),proctags.at(ii).c_str()).Data();
      TCanvas* aa = new TCanvas(ts.c_str(),ts.c_str());
      aa->SetGridx(); aa->SetGridy();
      _frame->DrawCopy("axis");
      TH1D* h_s = 0;
      TH1D* h_b = 0;
      TH1D* h_a = 0;
      // loop calorimeter signal definitions
      for ( size_t ij(0); ij<signtags.size(); ++ij ) {
	icolor = signcols.at(ij); fcolor = signfcls.at(ij);
	std::string sstr(TString::Format("%s+%s/h_lsig_%s_%s",putags.at(ik).c_str(),proctags.at(ii).c_str(),signtags.at(ij).c_str(),ystr.c_str()).Data());
	std::string bstr(TString::Format("%s+%s/h_lbck_%s_%s",putags.at(ik).c_str(),proctags.at(ii).c_str(),signtags.at(ij).c_str(),ystr.c_str()).Data());
	std::string astr(TString::Format("%s+%s/h_lall_%s_%s",putags.at(ik).c_str(),proctags.at(ii).c_str(),signtags.at(ij).c_str(),ystr.c_str()).Data());
	h_s = (TH1D*)file->FindObjectAny(sstr.c_str());
	h_b = (TH1D*)file->FindObjectAny(bstr.c_str());
	h_a = (TH1D*)file->FindObjectAny(astr.c_str());
	if ( h_s == 0 || h_b == 0 ) { 
	  printf("plotEff - cannot find histograms \042%s\042 and/or \042%s\042\n",sstr.c_str(),bstr.c_str());
	  continue;
	}
	if ( mode > 0 && h_a == 0 ) { 
	  printf("plotEff - cannot find histogram \042%s\042\n",astr.c_str());
	  continue;
	}
	// calculate efficiencies
	if ( mode == 0 ) { 
	  // measures ROC curve with truth info bias
	  // prepare workspace
	  std::vector<double> xv; xv.reserve(h_s->GetNbinsX()); std::vector<double> xd; xd.reserve(xv.size());
	  std::vector<double> yv; yv.reserve(h_s->GetNbinsX()); std::vector<double> yd; yd.reserve(yv.size());
	  double  sd(0.); double bd(0.);
	  double  si = h_s->IntegralAndError(1,h_s->GetNbinsX(),sd);
	  double  bi = h_b->IntegralAndError(1,h_b->GetNbinsX(),bd);
	  for ( int kp(1); kp<=h_s->GetNbinsX(); ++kp ) { 
	    double x(h_s->GetBinCenter(kp));
	    if ( x >= 0. ) { 
	      double ds(0.); double db(0.);
	      double fs = h_s->IntegralAndError(kp,h_s->GetNbinsX(),ds);
	      double fb = h_b->IntegralAndError(kp,h_b->GetNbinsX(),db);
	      xv.push_back(fs/si); if ( fs > 0. ) { xd.push_back(std::sqrt(std::pow(ds/fs,2)+std::pow(sd/si,2))*xv.back()); } else { xd.push_back(0.); } 
	      yv.push_back(fb/bi); if ( fb > 0. ) { yd.push_back(std::sqrt(std::pow(db/fb,2)+std::pow(bd/bi,2))*yv.back()); } else { yd.push_back(0.); }
	      // if ( prn ) { 
	      // 	printf("plotEff - bin %i4 center %5.3f signal/background/norms %7.1f/%7.1f/%7.1f/%7.1f fractions %6.4f/%6.4f\n",kp,h_s->GetBinCenter(kp),fs,fb,si,bi,xv.back(),yv.back()); 
	      // }
	    }
	  } // probability thresholds
	  agraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&yv[0],&xd[0],&yd[0]));
	  agraphs.back()->SetLineColor(icolor);
	  agraphs.back()->SetFillColor(fcolor);
	  agraphs.back()->SetLineStyle(iline);
	  agraphs.back()->SetLineWidth(2);
	} else {
	// prepare workspace
	  std::vector<double> xv; xv.reserve(h_s->GetNbinsX()); std::vector<double> xd; xd.reserve(xv.size());
	  std::vector<double> av; av.reserve(h_s->GetNbinsX()); std::vector<double> ad; ad.reserve(av.size());
	  std::vector<double> fv; fv.reserve(h_s->GetNbinsX()); std::vector<double> fd; fd.reserve(fv.size());
	  std::vector<double> bv; bv.reserve(h_s->GetNbinsX()); std::vector<double> bd; fd.reserve(bv.size());
 	  double sd(0.);
	  double si(h_a->IntegralAndError(1,h_a->GetNbinsX(),sd));
	  for ( int kp(1); kp<=h_s->GetNbinsX(); ++kp ) { 
	    double x(h_s->GetBinCenter(kp));
	    if ( x >= 0. && x <=1.00001 ) { 
	      double ds(0.); double db(0.); double da(0.);
	      double fs = h_s->IntegralAndError(kp,h_s->GetNbinsX(),ds);
	      double fb = h_b->IntegralAndError(kp,h_b->GetNbinsX(),db);
	      double fa = h_a->IntegralAndError(kp,h_a->GetNbinsX(),da);
	      // for ( int np(kp); np<=h_a->GetNbinsX(); ++np ) { si += h_a->GetBinContent(np); }
	      // //	    double si(fs+fb);// = h_a->IntegralAndError(kp,h_a->GetNbinsX(),sd);
	      // double sd(std::sqrt(std::pow(ds/fs,2)+std::pow(db/fb,2))*si);
	      if ( mode == 1 ) { 
		// measures ratio of jets with P > Pmin as function of Pmin, for all/signal/pile-up
		xv.push_back(h_s->GetBinCenter(kp)); xd.push_back(h_s->GetBinWidth(kp)/2.);
		av.push_back(fa/si); if ( fa > 0. ) { ad.push_back(std::sqrt(std::pow(da/fa,2)+std::pow(sd/si,2))*av.back()); } else { ad.push_back(0.); } 
		fv.push_back(fs/si); if ( fs > 0. ) { fd.push_back(std::sqrt(std::pow(ds/fs,2)+std::pow(sd/si,2))*fv.back()); } else { fd.push_back(0.); }
		bv.push_back(fb/si); if ( fb > 0. ) { bd.push_back(std::sqrt(std::pow(db/fb,2)+std::pow(sd/si,2))*bv.back()); } else { bd.push_back(0.); }
	      } else if ( mode == 2 ) { 
		// measures fraction of signal and pile-up jets of all jets at given fraction of all jets with P>Pmin
		if ( fa > 0. ) { 
		  xv.push_back(fa/si); ad.push_back(std::sqrt(std::pow(da/fa,2)+std::pow(sd/si,2))*av.back()); 
		  fv.push_back(fs/fa); fd.push_back(std::sqrt(std::pow(ds/fs,2)+std::pow(da/fa,2))*fv.back());
		  bv.push_back(fb/fa); bd.push_back(std::sqrt(std::pow(db/fb,2)+std::pow(da/fa,2))*bv.back());
		}
	      } else if ( mode == 3 ) { 
		// measures fraction of signal and pile-up jets of all jets at given Pmin
		if ( fa > 0. ) { 
		  xv.push_back(h_s->GetBinCenter(kp)); xd.push_back(h_s->GetBinWidth(kp)/2.); 
		  fv.push_back(fs/fa); fd.push_back(std::sqrt(std::pow(ds/fs,2)+std::pow(da/fa,2))*fv.back());
		  bv.push_back(fb/fa); bd.push_back(std::sqrt(std::pow(db/fb,2)+std::pow(da/fa,2))*bv.back());
		}
	      }
	    }
	  } // probability thresholds
	  prn = false;
	  if ( mode == 1 ) { 
	    agraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&av[0],&xd[0],&ad[0]));
	    agraphs.back()->SetLineColor(icolor);
	    agraphs.back()->SetFillColor(fcolor);
	    agraphs.back()->SetLineStyle(1);
	    agraphs.back()->SetLineWidth(1);
	    agraphs.back()->SetMarkerStyle(20);
	    fgraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&fv[0],&xd[0],&fd[0]));
	    fgraphs.back()->SetLineColor(icolor);
	    fgraphs.back()->SetFillColor(fcolor);
	    fgraphs.back()->SetLineStyle(9);
	    fgraphs.back()->SetLineWidth(1);
	    fgraphs.back()->SetMarkerStyle(22);
	    bgraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&bv[0],&xd[0],&bd[0]));
	    bgraphs.back()->SetLineColor(icolor);
	    bgraphs.back()->SetFillColor(fcolor);
	    bgraphs.back()->SetLineStyle(2);
	    bgraphs.back()->SetLineWidth(1);
	    bgraphs.back()->SetMarkerStyle(23);
	  } else if ( mode == 2 ) { 
	    fgraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&fv[0],&xd[0],&fd[0]));
	    fgraphs.back()->SetLineColor(icolor);
	    fgraphs.back()->SetFillColor(fcolor);
	    fgraphs.back()->SetLineStyle(1);
	    fgraphs.back()->SetLineWidth(2);
	    fgraphs.back()->SetMarkerStyle(20);
	    bgraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&bv[0],&xd[0],&bd[0]));
	    bgraphs.back()->SetLineColor(icolor);
	    bgraphs.back()->SetFillColor(fcolor);
	    bgraphs.back()->SetLineStyle(2);
	    bgraphs.back()->SetLineWidth(2);
	    bgraphs.back()->SetMarkerStyle(23);
	  } else { 
	    fgraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&fv[0],&xd[0],&fd[0]));
	    fgraphs.back()->SetLineColor(icolor);
	    fgraphs.back()->SetFillColor(fcolor);
	    fgraphs.back()->SetLineStyle(1);
	    fgraphs.back()->SetLineWidth(2);
	    fgraphs.back()->SetMarkerStyle(20);
	    bgraphs.push_back(new TGraphErrors((int)xv.size(),&xv[0],&bv[0],&xd[0],&bd[0]));
	    bgraphs.back()->SetLineColor(icolor);
	    bgraphs.back()->SetFillColor(fcolor);
	    bgraphs.back()->SetLineStyle(2);
	    bgraphs.back()->SetLineWidth(2);
	    bgraphs.back()->SetMarkerStyle(23);
	  }
	}
      } // signal definitions
      if ( h_s != 0 && h_b != 0 ) {
	double x = gStyle->GetPadLeftMargin()+0.050; double y = 0.5;
	if ( mode > 0 ) {
	  x = (1.-gStyle->GetPadRightMargin())-0.3; y = 0.65; 
	}
	TLegend* leg = new TLegend(x,y,x+0.35,y+0.275);
	leg->SetTextFont(42);
	if ( mode == 0 ) {
	  for ( size_t is(0); is<agraphs.size(); ++is ) {
	    //	    TGraphErrors* pg = agraphs.at(is);
	    TGraphErrors* pg = (TGraphErrors*)HistHelper::Data::compressGraph(agraphs.at(is),1e-04,"y"); // remove left 0's
	    pg->SetMarkerStyle(20);
	    pg->SetMarkerSize(0.75);
	    pg->SetMarkerColor(pg->GetLineColor());
	    if ( putags.at(ik) == "CSSK" && signtags.at(is) == "topotowf" && ( ystr == "32_49" || ystr == "25_32" ) ) {
	      pg->SetMarkerStyle(41); 
	      pg->SetMarkerSize(2);
	      pg->Draw("px same");
	      leg->AddEntry(pg,HistHelper::Names::signalName(signtags.at(is)).c_str(),"p")->SetTextColor(pg->GetLineColor());
	    } else { 
	      pg->Draw("lx same");
	      leg->AddEntry(pg,HistHelper::Names::signalName(signtags.at(is)).c_str(),"l")->SetTextColor(pg->GetLineColor());
	    }
	    HistHelper::Data::writeGraph(pg,TString::Format("ROCCurves/%s_%s.dat",aa->GetName(),signtags.at(is).c_str()).Data());
	  } 
	} else if ( mode == 1 ) { 
	  for ( size_t is(0); is<agraphs.size(); ++is ) {
	    TGraphErrors* pg = agraphs.at(is);
	    pg->SetMarkerSize(0.75);
	    pg->SetMarkerColor(pg->GetLineColor());
	    pg->Draw("lx same");
	    leg->AddEntry(pg,std::string(HistHelper::Names::signalName(signtags.at(is))+" (all)").c_str(),"l")->SetTextColor(pg->GetLineColor());
	  } 
	  for ( size_t is(0); is<bgraphs.size(); ++is ) {
	    TGraphErrors* pg = bgraphs.at(is);
	    pg->SetMarkerSize(0.75);
	    pg->SetMarkerColor(pg->GetLineColor());
	    pg->Draw("lx same");
	    leg->AddEntry(pg,std::string(HistHelper::Names::signalName(signtags.at(is))+" (pile-up)").c_str(),"l")->SetTextColor(pg->GetLineColor());
	  } 
	  for ( size_t is(0); is<fgraphs.size(); ++is ) {
	    TGraphErrors* pg = fgraphs.at(is);
	    pg->SetMarkerSize(0.75);
	    pg->SetMarkerColor(pg->GetLineColor());
	    pg->Draw("lx same");
	    leg->AddEntry(pg,std::string(HistHelper::Names::signalName(signtags.at(is))+" (signal)").c_str(),"l")->SetTextColor(pg->GetLineColor());
	  } 
	} else {
	  for ( size_t is(0); is<bgraphs.size(); ++is ) {
	    TGraphErrors* pg = bgraphs.at(is);
	    pg->SetMarkerSize(0.75);
	    pg->SetMarkerColor(pg->GetLineColor());
	    pg->Draw("lx same");
	    leg->AddEntry(pg,std::string(HistHelper::Names::signalName(signtags.at(is))+" (pile-up)").c_str(),"l")->SetTextColor(pg->GetLineColor());
	  } 
	  for ( size_t is(0); is<fgraphs.size(); ++is ) {
	    TGraphErrors* pg = fgraphs.at(is);
	    pg->SetMarkerSize(0.75);
	    pg->SetMarkerColor(pg->GetLineColor());
	    pg->Draw("lx same");
	    leg->AddEntry(pg,std::string(HistHelper::Names::signalName(signtags.at(is))+" (signal)").c_str(),"l")->SetTextColor(pg->GetLineColor());
	  } 
	}
	if ( mode == 0 ) {
	  leg->Draw("same");
	  _frame->DrawCopy("same axig");
	} else {
	  leg->SetFillStyle(1001); 
	  leg->SetFillColor(kWhite);
	  _frame->DrawCopy("same axig");
	  leg->Draw("same");
	}
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
