
//#include "AtlasUtils.C"
#include "HistHelper.h"
#include "ReadMLFile.h"

#include <TCanvas.h>
#include <TH1D.h>
#include <TString.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <THStack.h>
#include <TColor.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TLine.h>

#include <string>

//#include "effshp.C"

void plotPtLikelihood(const std::string& fname,const std::string& htag,const std::string& hmode)
{

  int    nbins(75);
  double xmin(0.);
  double xmax(1500.);

#include "ProbThresholds.icc"

  ReadMLFile indata(fname);
  printf("plotPtLikelihood - read %zu records\n\n",indata.size());

  printf("///////////////////////////////////\n");
  printf("// Book global 1-D distributions //\n");
  printf("///////////////////////////////////\n\n");

  TH1D* h_ptall = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptall",true).c_str(),"p_{T}^{jet} (all jets)",n_ptbins,a_ptbins)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptall").c_str(),"p_{T}^{jet} (all jets)",    n_ptbins,a_ptbins);
    // ? new TH1D(HistHelper::Names::histName(htag,"h_ptall",true).c_str(),"p_{T}^{jet} (all jets)",nbins,xmin,xmax)
    // : new TH1D(HistHelper::Names::histName(fname,"h_ptall").c_str(),"p_{T}^{jet} (all jets)",    nbins,xmin,xmax);
  TH1D* h_ptsig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptsig",true).c_str(),"p_{T}^{jet} (signal jets)",n_ptbins,a_ptbins)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptsig").c_str(),"p_{T}^{jet} (signal jets)",    n_ptbins,a_ptbins);
    // ? new TH1D(HistHelper::Names::histName(htag,"h_ptsig",true).c_str(),"p_{T}^{jet} (signal jets)",nbins,xmin,xmax)
    // : new TH1D(HistHelper::Names::histName(fname,"h_ptsig").c_str(),"p_{T}^{jet} (signal jets)",    nbins,xmin,xmax);
  TH1D* h_ptbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ptbck",true).c_str(),"p_{T}^{jet} (pile-up jets)",n_ptbins,a_ptbins)
    : new TH1D(HistHelper::Names::histName(fname,"h_ptbck").c_str(),"p_{T}^{jet} (pile-up jets)",    n_ptbins,a_ptbins);
    // ? new TH1D(HistHelper::Names::histName(htag,"h_ptbck",true).c_str(),"p_{T}^{jet} (pile-up jets)",nbins,xmin,xmax)
    // : new TH1D(HistHelper::Names::histName(fname,"h_ptbck").c_str(),"p_{T}^{jet} (pile-up jets)",    nbins,xmin,xmax);


  h_ptall->Sumw2();
  h_ptsig->Sumw2();
  h_ptbck->Sumw2();

  printf("///////////////////////////////////\n");
  printf("// Book global 2-D distributions //\n");
  printf("///////////////////////////////////\n\n");

  std::vector<TH2D*> d_ptave_all; std::vector<TH2D*> d_ptave_sig; std::vector<TH2D*> d_ptave_bck; 
  for ( size_t i(1); i<lThresholds.size(); ++i ) { 
    std::string hname(TString::Format("d_ave_all_%03.0f_%03.0f",std::max(std::abs(lThresholds.at(i-1)*100.),0.),std::min(std::abs(lThresholds.at(i)*100.),100.)).Data());
    std::string sname(TString::Format("d_ave_sig_%03.0f_%03.0f",std::max(std::abs(lThresholds.at(i-1)*100.),0.),std::min(std::abs(lThresholds.at(i)*100.),100.)).Data());
    std::string bname(TString::Format("d_ave_bck_%03.0f_%03.0f",std::max(std::abs(lThresholds.at(i-1)*100.),0.),std::min(std::abs(lThresholds.at(i)*100.),100.)).Data());
    //
    d_ptave_all.push_back(new TH2D(hname.c_str(),"average variable",n_ptbins,a_ptbins,2000,0.,2000.)); d_ptave_all.back()->Sumw2();
    d_ptave_sig.push_back(new TH2D(sname.c_str(),"average variable",n_ptbins,a_ptbins,2000,0.,2000.)); d_ptave_sig.back()->Sumw2();
    d_ptave_bck.push_back(new TH2D(bname.c_str(),"average variable",n_ptbins,a_ptbins,2000,0.,2000.)); d_ptave_bck.back()->Sumw2();
  }

  printf("////////////////\n");
  printf("// Fetch data //\n");
  printf("////////////////\n\n");
  
  auto fiter = indata.begin();
  for ( ; fiter != indata.end(); ++fiter ) { 
    double pt(indata.pt(fiter)); double seff(indata.psig(fiter));
    // MC truth references
    h_ptall->Fill(pt);
    if ( indata.isSignal(fiter) ) { h_ptsig->Fill(pt); } else { h_ptbck->Fill(pt); }
    // in signal efficiency bins
    int ib = h_probBins->FindFixBin(seff);
    if ( ib < 1 && ib > h_probBins->GetNbinsX() ) { continue; }
    size_t ih((size_t)ib-1);
    allHists.at(ih)->Fill(pt); d_ptave_all.at(ih)->Fill(pt,pt);
    if ( indata.isSignal(fiter) ) { sigHists.at(ih)->Fill(pt); d_ptave_sig.at(ih)->Fill(pt,pt); } else { bckHists.at(ih)->Fill(pt); d_ptave_bck.at(ih)->Fill(pt,pt); }
  }

  for ( size_t i(0); i<allHists.size(); ++i ) {
    for ( size_t j(i+1); j<allHists.size(); ++j ) { allHists[i]->Add(allHists.at(j)); sigHists.at(i)->Add(sigHists.at(j)); bckHists.at(i)->Add(bckHists.at(j)); }
  }

  printf("///////////////////////\n");
  printf("// Collect plot data //\n");
  printf("///////////////////////\n");

  size_t hidx(0);
  std::vector<int> allIdx; 
  std::vector<TH1D*> plotAllList; hidx = 0; for ( auto hp : allHists ) { /*if ( hp->GetEntries() > 0 ) {*/ plotAllList.push_back(hp); allIdx.push_back(hidx); /*}*/ ++hidx; } 
  std::vector<int> sigIdx; 
  std::vector<TH1D*> plotSigList; hidx = 0; for ( auto hp : sigHists ) { /*if ( hp->GetEntries() > 0 ) {*/ plotSigList.push_back(hp); sigIdx.push_back(hidx); /*}*/ ++hidx; } 
  std::vector<int> bckIdx; 
  std::vector<TH1D*> plotBckList; hidx = 0; for ( auto hp : bckHists ) { /*if ( hp->GetEntries() > 0 ) {*/ plotBckList.push_back(hp); bckIdx.push_back(hidx); /*}*/ ++hidx; }  
  printf("   (*) histograms in plot lists (%zu,%zu,%zu)\n",plotAllList.size(),plotSigList.size(),plotBckList.size());

  std::vector<TH1D*> plotfall; 
  std::vector<TH1D*> plotfsig;
  std::vector<TH1D*> plotfbck;
  for ( auto hp : plotAllList ) { plotfall.push_back(HistHelper::Hist::histRatio(hp,h_ptall)); }
  // for ( auto hp : plotSigList ) { plotfsig.push_back(HistHelper::Hist::histRatio(hp,h_ptall)); }
  // for ( auto hp : plotBckList ) { plotfbck.push_back(HistHelper::Hist::histRatio(hp,h_ptall)); }
  for ( auto hp : plotSigList ) { plotfsig.push_back(HistHelper::Hist::histRatio(hp,h_ptsig)); }
  for ( auto hp : plotBckList ) { plotfbck.push_back(HistHelper::Hist::histRatio(hp,h_ptbck)); }
  printf("   (*) calculated ratios\n");

  TH1D* _frame = new TH1D(HistHelper::Names::histName(fname,"_frame").c_str(),"_frame",385,15.,400.);

  _frame->GetXaxis()->SetMoreLogLabels();
  _frame->SetMaximum(1.5);
  _frame->SetMinimum(-0.1);
  _frame->GetXaxis()->SetTitle("p_{T} [GeV]");
  _frame->GetYaxis()->SetTitle(TString::Format("N_{P_{signal} #geq P_{min}}^{%s jet}/N_{P_{signal} #geq 0}^{%s jet}",hmode.c_str(),hmode.c_str()).Data());
  _frame->DrawCopy("axis");
  printf("   (*) plot frame\n\n");

  TLegend* leg = new TLegend(0.6,gStyle->GetPadBottomMargin()+0.05,1-gStyle->GetPadRightMargin()-0.025,gStyle->GetPadBottomMargin()+0.25);
  //  leg->SetFillStyle(1001);
  //  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(1);
  leg->SetTextFont(42);
  std::string mm(hmode); 
  switch ( *(mm.begin()) ) { 
  case 's':
    mm.replace(0,1,"S"); break;
  case 'a':
    mm.replace(0,1,"A"); break;
  case 'p':
    mm.replace(0,1,"P"); break;
  default:
    break;
  }

  leg->SetHeader(TString::Format("%s jet efficiency thresholds:",mm.c_str()).Data(),"l");

  // TLegend* ltx = new TLegend(0.6,gStyle->GetPadBottomMargin()+0.25,1-gStyle->GetPadRightMargin()-0.025,gStyle->GetPadBottomMargin()+0.45);
  // //  ltx->SetFillStyle(1001);
  // //  ltx->SetFillColor(kWhite);
  // ltx->SetLineColor(kBlack);
  // ltx->SetLineWidth(1);
  // ltx->SetTextFont(42);
  // ltx->SetHeader("Jet selections:","l");

  TF1* ff = new TF1("ff","[&](double* x,double* p) { double u(p[0]*x[0]-p[1]); return (1+TMath::Erf(u))/2.; }",20.,120.,2); 
  ff->SetParameter(0,0.1);
  ff->SetParameter(1,7.);
  ff->SetNpx(5000);

  int lstyle(1); int mstyle(24); 

  std::vector<TGraphAsymmErrors*> log;

  if ( hmode == "all" ) {

    printf("//////////////////////////////////////////////\n");
    printf("// Loop probability thresholds for all jets //\n"); 
    printf("//////////////////////////////////////////////\n");

    size_t ip(0); int nleg(0); log.clear();
    for ( auto fh(plotfall.begin()); fh!=plotfall.end(); ++fh, ++ip ) { 
      // pt values (x-axis values)
      TGraphAsymmErrors* pg = HistHelper::Hist::graph(d_ptave_all[allIdx.at(ip)],"_aname",2);
      int npts = pg->GetN();                      // number of points ingraph (# bins on pt axis)
      std::vector<double> ptave((size_t)npts,0.); // pt averages
      std::vector<double> ptel((size_t)npts,0.);  // down error
      std::vector<double> pteh((size_t)npts,0.);  // up error
      for ( size_t i(0); i<ptave.size(); ++i ) { 
	int j(i+1); double xl(d_ptave_all[allIdx.at(ip)]->GetXaxis()->GetBinLowEdge(j)); double xh(d_ptave_all[allIdx.at(ip)]->GetXaxis()->GetBinUpEdge(j));
	ptave[i] = pg->GetY()[i]; 
	if ( ptave[i] == 0. ) { ptave[i] = (xl+xh)/2.; }
	ptel[i] = ptave.at(i)-xl; //pg->GetEYlow()[i]; 
	pteh[i] = xh-ptave.at(i); //pg->GetEYhigh()[i]; 
      }
      // 
      std::vector<double> rave(npts,0.); std::vector<double> rael(npts,0.); std::vector<double> raeh(npts,0.);
      size_t iptb(0);
      for ( auto pti : ptave ) { 
	int k(plotfall.at(ip)->FindFixBin(pti)); 
	double y(plotfall.at(ip)->GetBinContent(k));
	double dy(plotfall.at(ip)->GetBinError(k));
	rave[iptb] = y; rael[iptb] = dy; if ( y+dy > 1. ) { raeh[iptb] = 0.; } else { raeh[iptb] = dy; }
	++iptb;
      }
      printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector)\n",ptave.size(),rave.size());
      // create graph
      TGraphAsymmErrors* pgs = new TGraphAsymmErrors((int)ptave.size(),&ptave[0],&rave[0],&ptel[0],&pteh[0],&rael[0],&raeh[0]);
      pgs->SetMarkerSize(1.0); pgs->SetMarkerStyle(mstyle); pgs->SetMarkerColor(hlColor.at(allIdx.at(ip)));
      pgs->SetLineWidth(2);                                 pgs->SetLineColor(pgs->GetMarkerColor());       pgs->SetLineStyle(lstyle);
      pgs->SetFillColorAlpha(hfColor.at(allIdx.at(ip)),0.25);
      //    pgs->Draw("lp same");
      if ( ip == 0 ) { 
	TLine* pl = new TLine(_frame->GetXaxis()->GetXmin(),1.,_frame->GetXaxis()->GetXmax(),1.);
	pl->SetLineWidth(pgs->GetLineWidth()); 
	pl->SetLineStyle(pgs->GetLineStyle());
	pl->SetLineColor(pgs->GetLineColor());
	pgs->SetLineStyle(0); pgs->SetLineWidth(0); pgs->Draw("e2 same");
	pl->Draw("lsame");
	// add to legend
	TLegendEntry* l = leg->AddEntry(pgs,hKey[allIdx.at(ip)].c_str(),"lf"); l->SetTextFont(42); 
      } else { 
	// add to legend
	TLegendEntry* l = leg->AddEntry(pgs,hKey[allIdx.at(ip)].c_str(),"lp"); l->SetTextFont(42); ++nleg; 
      }
      //
      log.push_back(pgs);
      //
      if ( ip != 0 ) { pgs->Fit(ff,"R0"); }
    }

    if ( nleg % 2 != 0 ) { leg->AddEntry((TObject*)0,"",""); }

    ip = log.size()-1;
    for ( auto fh(log.rbegin()); fh != log.rend(); ++fh, --ip ) { 
      if ( ip > 0 ) { (*fh)->Draw("p same"); } 
    }
    
    // TGraphAsymmErrors* gall = new TGraphAsymmErrors(*log.front());
    // gall->SetLineColor(kBlack);
    // gall->SetMarkerColor(kBlack);
    // TLegendEntry* l0 = ltx->AddEntry(gall,"all jets (#kappa = all)","lp"); l0->SetTextFont(42);

    printf("\n");     

  } else if ( hmode == "signal" ) { 

    printf("/////////////////////////////////////////////////\n");
    printf("// Loop probability thresholds for signal jets //\n"); 
    printf("/////////////////////////////////////////////////\n");

    size_t ip(0); log.clear();

    for ( auto fh(plotfsig.begin()); fh!=plotfsig.end(); ++fh, ++ip ) { 
      // pt values (x-axis values)
      TGraphAsymmErrors* pg = HistHelper::Hist::graph(d_ptave_sig[sigIdx.at(ip)],"_aname",2);
      int npts = pg->GetN();                      // number of points ingraph (# bins on pt axis)
      std::vector<double> ptave((size_t)npts,0.); // pt averages
      std::vector<double> ptel((size_t)npts,0.);  // down error
      std::vector<double> pteh((size_t)npts,0.);  // up error
      for ( size_t i(0); i<ptave.size(); ++i ) { 
	int j(i+1); double xl(d_ptave_sig[sigIdx.at(ip)]->GetXaxis()->GetBinLowEdge(j)); double xh(d_ptave_sig[sigIdx.at(ip)]->GetXaxis()->GetBinUpEdge(j));
	ptave[i] = pg->GetY()[i]; 
	if ( ptave[i] == 0. ) { ptave[i] = (xl+xh)/2.; }
	ptel[i] = ptave.at(i)-xl; //pg->GetEYlow()[i]; 
	pteh[i] = xh-ptave.at(i); //pg->GetEYhigh()[i]; 
      }
      // 
      std::vector<double> rave(npts,0.); std::vector<double> rael(npts,0.); std::vector<double> raeh(npts,0.);
      size_t iptb(0);
      for ( auto pti : ptave ) { 
	int k((*fh)->FindFixBin(pti)); 
	double y((*fh)->GetBinContent(k));
	double dy((*fh)->GetBinError(k));
	rave[iptb] = y; rael[iptb] = dy; if ( y+dy > 1. ) { raeh[iptb] = 0.; } else { raeh[iptb] = dy; }
	++iptb;
      }
      printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector)\n",ptave.size(),rave.size());
      // create graph
      TGraphAsymmErrors* pgs = new TGraphAsymmErrors((int)ptave.size(),&ptave[0],&rave[0],&ptel[0],&pteh[0],&rael[0],&raeh[0]);
      pgs->SetMarkerSize(1.0); pgs->SetMarkerStyle(mstyle); pgs->SetMarkerColor(hlColor.at(sigIdx.at(ip)));
      pgs->SetLineWidth(2);                                  pgs->SetLineColor(pgs->GetMarkerColor());       pgs->SetLineStyle(lstyle);
      pgs->SetFillColorAlpha(hfColor.at(sigIdx.at(ip)),0.25);
      log.push_back(pgs);
      // fit
      if ( ip != 0 ) { pgs->Fit(ff,"R0"); }
    }

    for ( auto fh(log.rbegin()); fh != --log.rend(); ++fh /*, --ip*/ ) { (*fh)->Draw("p same"); }

    // TGraphAsymmErrors* gsig = new TGraphAsymmErrors(*log.back());
    // gsig->SetLineColor(kBlack);
    // gsig->SetMarkerColor(kBlack);
    // TLegendEntry* l1 = ltx->AddEntry(gsig,"signal jets (#kappa = signal)","lp"); l1->SetTextFont(42);
    
    printf("\n");

  } else if ( hmode == "pile-up" ) {      
    
    printf("//////////////////////////////////////////////////\n");
    printf("// Loop probability thresholds for pile-up jets //\n"); 
    printf("//////////////////////////////////////////////////\n");
  
    size_t ip(0); log.clear();

    for ( auto fh(plotfbck.begin()); fh!=plotfbck.end(); ++fh, ++ip ) { 
      // pt values (x-axis values)
      TGraphAsymmErrors* pg = HistHelper::Hist::graph(d_ptave_bck[bckIdx.at(ip)],"_aname",2);
      int npts = pg->GetN();                      // number of points ingraph (# bins on pt axis)
      std::vector<double> ptave((size_t)npts,0.); // pt averages
      std::vector<double> ptel((size_t)npts,0.);  // down error
      std::vector<double> pteh((size_t)npts,0.);  // up error
      for ( size_t i(0); i<ptave.size(); ++i ) { 
	int j(i+1); double xl(d_ptave_bck[bckIdx.at(ip)]->GetXaxis()->GetBinLowEdge(j)); double xh(d_ptave_bck[bckIdx.at(ip)]->GetXaxis()->GetBinUpEdge(j));
	ptave[i] = pg->GetY()[i]; 
	if ( ptave[i] == 0. ) { ptave[i] = (xl+xh)/2.; }
	ptel[i] = ptave.at(i)-xl; //pg->GetEYlow()[i]; 
	pteh[i] = xh-ptave.at(i); //pg->GetEYhigh()[i]; 
      }
      // 
      std::vector<double> rave(npts,0.); std::vector<double> rael(npts,0.); std::vector<double> raeh(npts,0.);
      size_t iptb(0);
      for ( auto pti : ptave ) { 
	int k((*fh)->FindFixBin(pti)); 
	double y((*fh)->GetBinContent(k));
	double dy((*fh)->GetBinError(k));
	rave[iptb] = y; rael[iptb] = dy; if ( y+dy > 1. ) { raeh[iptb] = 0.; } else { raeh[iptb] = dy; }
	++iptb;
      }
      printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector)\n",ptave.size(),rave.size());
      // create graph
      TGraphAsymmErrors* pgs = new TGraphAsymmErrors((int)ptave.size(),&ptave[0],&rave[0],&ptel[0],&pteh[0],&rael[0],&raeh[0]);
      pgs->SetMarkerSize(1.0); pgs->SetMarkerStyle(mstyle); pgs->SetMarkerColor(hlColor.at(bckIdx.at(ip)));
      pgs->SetLineWidth(2);                                 pgs->SetLineColor(pgs->GetMarkerColor());       pgs->SetLineStyle(lstyle);
      pgs->SetFillColorAlpha(hfColor.at(bckIdx.at(ip)),0.25);
      log.push_back(pgs);
      // fit
      if ( ip != 0 ) { pgs->Fit(ff,"R0"); }
    }

    for ( auto fh(log.rbegin()); fh != --log.rend(); ++fh /*, --ip*/ ) { (*fh)->Draw("p same"); }

    // TGraphAsymmErrors* gbck = new TGraphAsymmErrors(*log.front());
    // gbck->SetLineColor(kBlack);
    // gbck->SetMarkerColor(kBlack);
    // TLegendEntry* l2 = ltx->AddEntry(gbck,"pile-up jets (#kappa = pile-up)","lp"); l2->SetTextFont(42);

    printf("\n");     

  }

  leg->SetNColumns(2);
  leg->Draw("same");

  //  ltx->Draw("same");

  _frame->DrawCopy("same axis");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel("Simulation internal");
  HistHelper::Draw::processing(fname);

}

