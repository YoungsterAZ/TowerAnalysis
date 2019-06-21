
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
#include <TLine.h>
#include <TPad.h>
#include <TGaxis.h>

#include <string>

#include <boost/tuple/tuple.hpp>

//#include "effshp.C"

void plotNCLH(const std::string& fname,const std::string& htag)
{

  gStyle->SetLineStyleString(10,"32 8"); // signal
  gStyle->SetLineStyleString(11,"10 10"); // pile-up

  gPad->SetFillStyle(0);

  int    lsall(1);
  int    lssig(10);
  int    lsbck(11);

#include "NCProbThresholds.icc"

  int    nbins(nb);
  double xmin(xl);
  double xmax(xh);

  ReadMLFile indata(fname);
  printf("plotNCLH - read %zu records\n\n",indata.size());

  printf("///////////////////////////////////\n");
  printf("// Book global 1-D distributions //\n");
  printf("///////////////////////////////////\n\n");

  TH1D* h_ncall = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ncall",true).c_str(),"N_{const} (all jets)",nbins,xmin,xmax)
    : new TH1D(HistHelper::Names::histName(fname,"h_ncall").c_str(),"N_{const} (all jets)",    nbins,xmin,xmax);
  TH1D* h_ncsig = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ncsig",true).c_str(),"N_{const} (signal jets)",nbins,xmin,xmax)
    : new TH1D(HistHelper::Names::histName(fname,"h_ncsig").c_str(),"N_{const} (signal jets)",    nbins,xmin,xmax);
  TH1D* h_ncbck = htag != "" 
    ? new TH1D(HistHelper::Names::histName(htag,"h_ncbck",true).c_str(),"N_{const} (pile-up jets)",nbins,xmin,xmax)
    : new TH1D(HistHelper::Names::histName(fname,"h_ncbck").c_str(),"N_{const} (pile-up jets)",    nbins,xmin,xmax);

  h_ncall->Sumw2();
  h_ncsig->Sumw2();
  h_ncbck->Sumw2();

  printf("///////////////////////////////////\n");
  printf("// Book global 2-D distributions //\n");
  printf("///////////////////////////////////\n\n");

  std::vector<TH2D*> d_ncave_all; std::vector<TH2D*> d_ncave_sig; std::vector<TH2D*> d_ncave_bck; 
  for ( size_t i(1); i<lThresholds.size(); ++i ) { 
    std::string hname(TString::Format("d_ave_all_%03.0f_%03.0f",std::max(std::abs(lThresholds.at(i-1)*100.),0.),std::min(std::abs(lThresholds.at(i)*100.),100.)).Data());
    std::string sname(TString::Format("d_ave_sig_%03.0f_%03.0f",std::max(std::abs(lThresholds.at(i-1)*100.),0.),std::min(std::abs(lThresholds.at(i)*100.),100.)).Data());
    std::string bname(TString::Format("d_ave_bck_%03.0f_%03.0f",std::max(std::abs(lThresholds.at(i-1)*100.),0.),std::min(std::abs(lThresholds.at(i)*100.),100.)).Data());
    //
    int nydb = (int)((xmax-xmin)/((double)nbins));
    d_ncave_all.push_back(new TH2D(hname.c_str(),"average variable",nbins,xmin,xmax,nydb*nbins,xmin,xmax)); d_ncave_all.back()->Sumw2();
    d_ncave_sig.push_back(new TH2D(sname.c_str(),"average variable",nbins,xmin,xmax,nydb*nbins,xmin,xmax)); d_ncave_sig.back()->Sumw2();
    d_ncave_bck.push_back(new TH2D(bname.c_str(),"average variable",nbins,xmin,xmax,nydb*nbins,xmin,xmax)); d_ncave_bck.back()->Sumw2();
  }

  printf("////////////////\n");
  printf("// Fetch data //\n");
  printf("////////////////\n\n");
  
  auto fiter = indata.begin();
  for ( ; fiter != indata.end(); ++fiter ) { 
    double nc(indata.nc(fiter)); double seff(indata.psig(fiter));
    // MC truth references
    h_ncall->Fill(nc);
    if ( indata.isSignal(fiter) ) { h_ncsig->Fill(nc); } else { h_ncbck->Fill(nc); }
    // in signal efficiency bins
    int ib = h_probBins->FindFixBin(seff);
    if ( ib < 1 && ib > h_probBins->GetNbinsX() ) { continue; }
    size_t ih(0);
    //    printf("plotNCLH - jet width %5.3f signal probability %5.3f isSignal %i probability bin %2i contributes to bins { ",w,seff,indata.tag(fiter),ib);
    for ( size_t ih(0); ih<(size_t)ib; ++ih ) { 
      //      printf("%i ",(int)ih+1);
      allHists.at(ih)->Fill(nc); d_ncave_all.at(ih)->Fill(nc,nc);
      if ( indata.isSignal(fiter) ) { sigHists.at(ih)->Fill(nc); d_ncave_sig.at(ih)->Fill(nc,nc); } else { bckHists.at(ih)->Fill(nc); d_ncave_bck.at(ih)->Fill(nc,nc); }
    }
    //    printf("}\n");
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
  for ( auto hp : plotAllList ) { plotfall.push_back(HistHelper::Hist::histRatio(hp,h_ncall)); }
  for ( auto hp : plotSigList ) { plotfsig.push_back(HistHelper::Hist::histRatio(hp,h_ncsig)); }
  for ( auto hp : plotBckList ) { plotfbck.push_back(HistHelper::Hist::histRatio(hp,h_ncbck)); }
  printf("   (*) calculated ratios\n");

  // find binning for frame
  //  int ilb = h_ncall->GetXaxis()->FindFixBin(400.); 
  //  double xfmax(h_ncall->GetXaxis()->GetBinLowEdge(ilb));
  TH1D* _frame = new TH1D(HistHelper::Names::histName(fname,"_frame").c_str(),"_frame",h_ncall->GetNbinsX(),h_ncall->GetXaxis()->GetXmin(),h_ncall->GetXaxis()->GetXmax());

  gPad->SetTicks(1,0);

  //  _frame->GetXaxis()->SetMoreLogLabels();
  _frame->SetMaximum(1.5);
  _frame->SetMinimum(-0.1);
  _frame->GetXaxis()->SetTitle("N_{const}");
  _frame->GetYaxis()->SetTitle("N_{P_{signal}^{jet} #geq P_{min}}^{#kappa}/N_{P_{signal}^{jet} #geq 0}^{#kappa}");
  _frame->DrawCopy("axis");
  printf("   (*) plot frame\n\n");

  double tsize(1.-(gStyle->GetPadBottomMargin()+gStyle->GetPadTopMargin()));
  double fsize(_frame->GetMaximum()-_frame->GetMinimum());
  double yspan(tsize/fsize);
  double yref((0.75-_frame->GetMinimum())*yspan);
  printf("plotNCLH - scale factor %.3f y-ccordinate %.3f window height %.3f\n",yspan,yref,1.-(gStyle->GetPadBottomMargin()+gStyle->GetPadTopMargin()));
  // inset plot
  TStyle* thisStyle = (TStyle*)gStyle;
  TPad* thisPad     = (TPad*)gPad;
  double x0pos(0); 
  double x1pos(1.);
  double y0pos(gStyle->GetPadBottomMargin());
  double y1pos(gStyle->GetPadBottomMargin()+yref);
  double wsize(x1pos-x0pos);
  double hsize(y1pos-y0pos);
  //  TPad* insetPad    = new TPad("dists","dists",gStyle->GetPadLeftMargin(),gStyle->GetPadBottomMargin(),1.-gStyle->GetPadRightMargin(),gStyle->GetPadBottomMargin()+yref);
  TPad* insetPad    = new TPad("dists","dists",x0pos,y0pos,x1pos,y1pos);
  insetPad->cd();
  insetPad->SetFillStyle(4000);
  insetPad->SetFillColorAlpha(kWhite,0.0);
  insetPad->SetBorderSize(0);
  insetPad->SetLogy();
  //  insetPad->SetLogx();
  insetPad->SetTicks(0,1);
  insetPad->SetBottomMargin(0.);
  insetPad->SetTopMargin(0.);
  insetPad->SetLeftMargin(gStyle->GetPadLeftMargin());
  insetPad->SetRightMargin(gStyle->GetPadRightMargin());
  insetPad->SetFrameLineColor(0);
  TH1D* _inframe = new TH1D(*_frame);
  _inframe->SetMaximum(750000.);
  _inframe->SetMinimum(0.5);
  _inframe->GetXaxis()->SetTitle(""); _inframe->GetXaxis()->SetLabelSize(0); _inframe->GetXaxis()->SetLabelOffset(9999.); _inframe->GetXaxis()->SetTickSize(0);
  _inframe->GetYaxis()->SetTitle(""); _inframe->GetYaxis()->SetLabelSize(0); _inframe->GetYaxis()->SetLabelOffset(9999.); _inframe->GetYaxis()->SetTickSize(0);
  int nlabel = _inframe->GetMaximum() > 1. ? int(std::log10(_inframe->GetMaximum())) : 3;

  _inframe->DrawCopy("axis");
  TGaxis* aa = new TGaxis(_inframe->GetXaxis()->GetXmax(),_inframe->GetMinimum(),_inframe->GetXaxis()->GetXmax(),_inframe->GetMaximum(),
			  _inframe->GetMinimum(),0.999*_inframe->GetMaximum(),510,"G+");
  aa->SetLabelSize(_frame->GetYaxis()->GetLabelSize()*tsize/hsize);
  aa->SetLabelFont(_frame->GetXaxis()->GetLabelFont());
  aa->SetLabelOffset(-aa->GetLabelOffset());
  for ( int l(1); l<=nlabel+1; ++l ) { aa->ChangeLabel(l,-1.,-1.,12); }
  aa->Draw();
  TLatex ylabel;
  ylabel.SetTextSize(gStyle->GetTextSize()*tsize/hsize);
  ylabel.SetTextFont(_frame->GetYaxis()->GetTitleFont());
  ylabel.SetTextAlign(13);
  ylabel.SetNDC();
  ylabel.DrawLatex(1.-gStyle->GetPadRightMargin()+0.014,0.99,"Entries");
  h_ncbck->SetLineWidth(1); h_ncbck->SetLineStyle(lsbck); h_ncbck->DrawCopy("same hist");
  h_ncsig->SetLineWidth(1); h_ncsig->SetLineStyle(lssig); h_ncsig->DrawCopy("same hist");
  h_ncall->SetLineWidth(1); h_ncall->SetLineStyle(lsall); h_ncall->DrawCopy("same hist");

  thisPad->cd();

  double xleg(1-gStyle->GetPadRightMargin()+0.01);
  double xwdt(0.3/2.-0.02);
  double yleg(gStyle->GetPadBottomMargin()+0.4);
  double ywdt(1-gStyle->GetPadTopMargin()-yleg);
  printf("plotNCLH - legend (x0,y0,x1,y1) = (%5.3f,%5.3f,%5.3f,%5.3f) w = %5.3f h = %5.3f\n",xleg,xleg+xwdt,yleg,yleg+ywdt,xwdt,ywdt);
  TLegend* leg = new TLegend(xleg,yleg,xleg+xwdt,yleg+ywdt);
  leg->SetFillStyle(0);
  leg->SetFillColorAlpha(kWhite,1.0);
  leg->SetLineColor(kBlack);
  leg->SetLineWidth(0);
  leg->SetTextFont(42);

  TLegend* ltx = new TLegend(xleg,gStyle->GetPadBottomMargin(),xleg+xwdt,gStyle->GetPadBottomMargin()+0.375);
  ltx->SetFillStyle(1001);
  ltx->SetFillColorAlpha(kWhite,0.8);
  ltx->SetLineColor(kBlack);
  ltx->SetLineWidth(1);
  ltx->SetTextFont(42);
  ltx->SetHeader("Jet selections:","l");

  int lstyle(lsall); int mstyle(0);  

  std::vector<TGraphAsymmErrors*> log;

  printf("//////////////////////////////////////////////\n");
  printf("// Loop probability thresholds for all jets //\n"); 
  printf("//////////////////////////////////////////////\n");
  
  size_t ip(0); int nleg(0); log.clear();
  for ( auto fh(plotfall.begin()); fh!=plotfall.end(); ++fh, ++ip ) { 
    // pt values (x-axis values)
    TGraphAsymmErrors* pg = HistHelper::Hist::graph(d_ncave_all[allIdx.at(ip)],"_aname",2);
    int npts = pg->GetN();                     // number of points ingraph (# bins on pt axis)
    std::vector<double> ncave((size_t)npts,0.); // pt averages
    std::vector<double> ncel((size_t)npts,0.);  // down error
    std::vector<double> nceh((size_t)npts,0.);  // up error
    for ( size_t i(0); i<ncave.size(); ++i ) { 
      int j(i+1); double xl(d_ncave_all[allIdx.at(ip)]->GetXaxis()->GetBinLowEdge(j)); double xh(d_ncave_all[allIdx.at(ip)]->GetXaxis()->GetBinUpEdge(j));
      ncave[i] = pg->GetY()[i]; 
      if ( ncave[i] == 0. ) { ncave[i] = (xl+xh)/2.; }
      ncel[i] = ncave.at(i)-xl;
      nceh[i] = xh-ncave.at(i);
    }
    // 
    std::vector<double> rave(npts,0.); std::vector<double> rael(npts,0.); std::vector<double> raeh(npts,0.);
    size_t iptb(0);
    for ( auto pti : ncave ) { 
      int k(plotfall.at(ip)->FindFixBin(pti)); 
      double y(plotfall.at(ip)->GetBinContent(k));
      double dy(plotfall.at(ip)->GetBinError(k));
      rave[iptb] = y; 
      if ( y+dy > 1. ) { raeh[iptb] = 0.; } else { raeh[iptb] = dy; } 
      if ( y-dy < 0. ) { rael[iptb] = y;  } else { rael[iptb] = dy; } 
      ++iptb;
    }
    printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector) (pre-filter)\n",ncave.size(),rave.size());
    // filter
    if ( HistHelper::Data::compress(rave,rael,raeh,ncave,ncel,nceh) ) {
      // create graph
      TGraphAsymmErrors* pgs = new TGraphAsymmErrors((int)ncave.size(),&ncave[0],&rave[0],&ncel[0],&nceh[0],&rael[0],&raeh[0]);
      pgs->SetMarkerSize(1.0); pgs->SetMarkerStyle(mstyle); pgs->SetMarkerColor(hlColor.at(allIdx.at(ip)));
      pgs->SetLineWidth(2);                                 pgs->SetLineColor(pgs->GetMarkerColor());       pgs->SetLineStyle(lstyle);
      pgs->SetFillColorAlpha(hfColor.at(allIdx.at(ip)),0.25);
      if ( ip == 0 ) { 
	TLine* pl = new TLine(_frame->GetXaxis()->GetXmin(),1.,_frame->GetXaxis()->GetXmax(),1.);
	pl->SetLineWidth(pgs->GetLineWidth()); 
	pl->SetLineStyle(pgs->GetLineStyle());
	pl->SetLineColor(pgs->GetLineColor());
	pgs->SetLineStyle(0); pgs->SetLineWidth(0); pgs->Draw("le2 same");
	pl->Draw("lsame");
	// add to legend
	TGraphAsymmErrors* g = new TGraphAsymmErrors(*pgs); 
	g->SetLineStyle(1); g->SetLineWidth(2); 
	TLegendEntry* l = leg->AddEntry(g,hKey[allIdx.at(ip)].c_str(),"lf"); l->SetTextFont(42); ++nleg;
      } else { 
	// add to legend
	TLegendEntry* l = leg->AddEntry(pgs,hKey[allIdx.at(ip)].c_str(),"l"); l->SetTextFont(42); ++nleg; 
      }
      log.push_back(pgs);
    } // graph from enough data
    printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector) (after filter)\n",ncave.size(),rave.size());
  }

  double xgrid(0.);
  double dxgrid(5.); if ( h_ncall->GetXaxis()->GetXmax() > 100. ) { dxgrid = 10.; }
  while ( xgrid < _frame->GetXaxis()->GetXmax() ) { 
    TLine* lgrid = new TLine(xgrid,_frame->GetMinimum(),xgrid,1.); lgrid->SetLineStyle(3); lgrid->SetLineColor(13); lgrid->SetLineWidth(1);
    lgrid->Draw("same");
    xgrid += dxgrid;
  }

  insetPad->Draw("L");

  if ( nleg % 2 != 0 ) { leg->AddEntry((TObject*)0,"",""); }

  ip = log.size()-1;
  for ( auto fh(log.rbegin()); fh != log.rend(); ++fh, --ip ) { 
    if ( ip > 0 ) { if ( (*fh)->GetN() == 1 ) { (*fh)->Draw("l same"); } else {  (*fh)->Draw("lx same"); } } 
  }
  
  TGraphAsymmErrors* gall = new TGraphAsymmErrors(*log.back());
  gall->SetLineColor(kBlack);
  gall->SetMarkerColor(kBlack);
  TLegendEntry* l0 = ltx->AddEntry(gall,"all jets (#kappa = all)","l"); l0->SetTextFont(42);

  printf("\n");     

  printf("/////////////////////////////////////////////////\n");
  printf("// Loop probability thresholds for signal jets //\n"); 
  printf("/////////////////////////////////////////////////\n");
  
  ip = 0; log.clear(); lstyle = lssig;

  for ( auto fh(plotfsig.begin()); fh!=plotfsig.end(); ++fh, ++ip ) { 
    // pt values (x-axis values)
    TGraphAsymmErrors* pg = HistHelper::Hist::graph(d_ncave_sig[sigIdx.at(ip)],"_aname",2);
    int npts = pg->GetN();                      // number of points ingraph (# bins on pt axis)
    std::vector<double> ncave((size_t)npts,0.); // pt averages
    std::vector<double> ncel((size_t)npts,0.);  // down error
    std::vector<double> nceh((size_t)npts,0.);  // up error
    for ( size_t i(0); i<ncave.size(); ++i ) { 
      int j(i+1); double xl(d_ncave_sig[sigIdx.at(ip)]->GetXaxis()->GetBinLowEdge(j)); double xh(d_ncave_sig[sigIdx.at(ip)]->GetXaxis()->GetBinUpEdge(j));
      ncave[i] = pg->GetY()[i]; 
      if ( ncave[i] == 0. ) { ncave[i] = (xl+xh)/2.; }
      ncel[i] = ncave.at(i)-xl; //pg->GetEYlow()[i]; 
      nceh[i] = xh-ncave.at(i); //pg->GetEYhigh()[i]; 
    }
    // 
    std::vector<double> rave(npts,0.); std::vector<double> rael(npts,0.); std::vector<double> raeh(npts,0.);
    size_t iptb(0);
    for ( auto pti : ncave ) { 
      int k((*fh)->FindFixBin(pti)); 
      double y((*fh)->GetBinContent(k));
      double dy((*fh)->GetBinError(k));
      rave[iptb] = y; 
      if ( y+dy > 1. ) { raeh[iptb] = 0.; } else { raeh[iptb] = dy; } 
      if ( y-dy < 0. ) { rael[iptb] = y;  } else { rael[iptb] = dy; } 
      ++iptb;
    }
    printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector) (pre-filter)\n",ncave.size(),rave.size());
    // filter
    if ( HistHelper::Data::compress(rave,rael,raeh,ncave,ncel,nceh) ) {
      // create graph
      TGraphAsymmErrors* pgs = new TGraphAsymmErrors((int)ncave.size(),&ncave[0],&rave[0],&ncel[0],&nceh[0],&rael[0],&raeh[0]);
      pgs->SetMarkerSize(1.0); pgs->SetMarkerStyle(mstyle); pgs->SetMarkerColor(hlColor.at(sigIdx.at(ip)));
      pgs->SetLineWidth(2);                                  pgs->SetLineColor(pgs->GetMarkerColor());       pgs->SetLineStyle(lstyle);
      pgs->SetFillColorAlpha(hfColor.at(sigIdx.at(ip)),0.25);
      log.push_back(pgs);
    }
    printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector) (after filter)\n",ncave.size(),rave.size());
  }

  for ( auto fh(log.rbegin()); fh != --log.rend(); ++fh /*, --ip*/ ) { if ( (*fh)->GetN() == 1 ) { (*fh)->Draw("l same"); } else { (*fh)->Draw("lx same"); } }

  TGraphAsymmErrors* gsig = new TGraphAsymmErrors(*log.back());
  gsig->SetLineColor(kBlack);
  gsig->SetMarkerColor(kBlack);
  TLegendEntry* l1 = ltx->AddEntry(gsig,"signal jets (#kappa = signal)","l"); l1->SetTextFont(42);
    
  printf("\n");
    
  printf("//////////////////////////////////////////////////\n");
  printf("// Loop probability thresholds for pile-up jets //\n"); 
  printf("//////////////////////////////////////////////////\n");
  
  ip = 0; log.clear(); lstyle = lsbck;

  for ( auto fh(plotfbck.begin()); fh!=plotfbck.end(); ++fh, ++ip ) { 
    // pt values (x-axis values)
    TGraphAsymmErrors* pg = HistHelper::Hist::graph(d_ncave_bck[bckIdx.at(ip)],"_aname",2);
    int npts = pg->GetN();                      // number of points ingraph (# bins on pt axis)
    std::vector<double> ncave((size_t)npts,0.); // pt averages
    std::vector<double> ncel((size_t)npts,0.);  // down error
    std::vector<double> nceh((size_t)npts,0.);  // up error
    for ( size_t i(0); i<ncave.size(); ++i ) { 
      int j(i+1); double xl(d_ncave_bck[bckIdx.at(ip)]->GetXaxis()->GetBinLowEdge(j)); double xh(d_ncave_bck[bckIdx.at(ip)]->GetXaxis()->GetBinUpEdge(j));
      ncave[i] = pg->GetY()[i]; 
      if ( ncave[i] == 0. ) { ncave[i] = (xl+xh)/2.; }
      ncel[i] = ncave.at(i)-xl; //pg->GetEYlow()[i]; 
      nceh[i] = xh-ncave.at(i); //pg->GetEYhigh()[i]; 
    }
    // 
    std::vector<double> rave(npts,0.); std::vector<double> rael(npts,0.); std::vector<double> raeh(npts,0.);
    size_t iptb(0);
    for ( auto pti : ncave ) { 
      int k((*fh)->FindFixBin(pti)); 
      double y((*fh)->GetBinContent(k));
      double dy((*fh)->GetBinError(k));
      rave[iptb] = y; 
      if ( y+dy > 1. ) { raeh[iptb] = 0.; } else { raeh[iptb] = dy; } 
      if ( y-dy < 0. ) { rael[iptb] = y;  } else { rael[iptb
] = dy; } 
      ++iptb;
    }
    printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector) (pre-filter)\n",ncave.size(),rave.size());
    // filter
    if ( HistHelper::Data::compress(rave,rael,raeh,ncave,ncel,nceh) ) {
      // create graph
      TGraphAsymmErrors* pgs = new TGraphAsymmErrors((int)ncave.size(),&ncave[0],&rave[0],&ncel[0],&nceh[0],&rael[0],&raeh[0]);
      pgs->SetMarkerSize(1.0); pgs->SetMarkerStyle(mstyle); pgs->SetMarkerColor(hlColor.at(bckIdx.at(ip)));
      pgs->SetLineWidth(2);                                 pgs->SetLineColor(pgs->GetMarkerColor());       pgs->SetLineStyle(lstyle);
      pgs->SetFillColorAlpha(hfColor.at(bckIdx.at(ip)),0.25);
      log.push_back(pgs);
    }
    printf("   (i) number of data points %2zu (x-vector) %2zu (y-vector) (after filter)\n",ncave.size(),rave.size());
  }

  for ( auto fh(log.rbegin()); fh != --log.rend(); ++fh /*, --ip*/ ) { if ( (*fh)->GetN() == 1 ) { (*fh)->Draw("l same"); } else { (*fh)->Draw("lx same"); } }

  TGraphAsymmErrors* gbck = new TGraphAsymmErrors(*log.front());
  gbck->SetLineColor(kBlack);
  gbck->SetMarkerColor(kBlack);
  TLegendEntry* l2 = ltx->AddEntry(gbck,"pile-up jets (#kappa = pile-up)","l"); l2->SetTextFont(42);

  printf("\n");     

  _frame->DrawCopy("same axis");

  //  leg->SetNColumns(2);
  leg->Draw("same");

  xleg = 0.7;
  yleg = 0.9;

  TLatex tj; 
  tj.SetTextSize(0.7*gStyle->GetLabelSize("x"));
  tj.SetNDC();
  tj.SetTextAlign(11);
  tj.DrawLatex(xleg,yleg-0.01,"Jet selections:");

  tj.SetTextSize(0.6*gStyle->GetLabelSize("x"));
  tj.SetTextAlign(12);
  tj.DrawLatex(xleg+xwdt/4.+0.01,yleg-0.04,"#kappa = all");
  TLine* lja = new TLine(xleg,yleg-0.04,xleg+xwdt/4.,yleg-0.04);
  lja->SetLineWidth(2); lja->SetLineStyle(lsall); lja->SetNDC();
  lja->Draw("same");
  tj.DrawLatex(xleg+xwdt/4.+0.01,yleg-0.08,"#kappa = signal");
  TLine* ljs = new TLine(xleg,yleg-0.08,xleg+xwdt/4.,yleg-0.08);
  ljs->SetLineWidth(2); ljs->SetLineStyle(lssig); ljs->SetNDC();
  ljs->Draw("same");
  tj.DrawLatex(xleg+xwdt/4.+0.01,yleg-0.12,"#kappa = pile-up");
  TLine* ljb = new TLine(xleg,yleg-0.12,xleg+xwdt/4.,yleg-0.12);
  ljb->SetLineWidth(2); ljb->SetLineStyle(lsbck); ljb->SetNDC();
  ljb->Draw("same");

  printf("plotNCLH - label text size is %.3f\n",leg->GetTextSize());

  //  ltx->Draw("same");

  HistHelper::Draw::description("00_15");
  double xal(gStyle->GetPadLeftMargin()+0.0275);       //    x += 0.050; 
  double yal(1.0-gStyle->GetPadTopMargin()-0.025);    // y -= 0.025;
  HistHelper::Draw::AtlasLabel(xal,yal,"Simulation internal");
  yal = 0.86;
  HistHelper::Draw::processing(fname,xal,yal);

  gPad->RedrawAxis();
}

