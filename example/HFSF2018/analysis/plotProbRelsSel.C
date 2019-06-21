
#include "HistHelper.h"
#include "DataProvider.C"

#include <TH1D.h>
#include <TChain.h>
#include <TString.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TColor.h>
#include <TStyle.h>
#include <TPad.h>
#include <TBox.h>


#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#ifndef CINT
#include <boost/tuple/tuple.hpp>
#endif

#define CONTEXT_PLOTPROBRELS
bool plotProbRelsSel(const std::string& vname="pt",const std::string& ystr="00_15",const std::string& fname="tuple/JAPU+INCL_topoclus_tuple.root",bool scatter=false,int nccut=-1) {

#undef VARNAMES_ICC
#include "VarNames.icc"

#undef TUPLENAMES_ICC
#include "TupleFileNames.icc"

#define PTPROBTHRESHOLDS_NOPRINT
#include "PtProbThresholds.icc"
#include "MassProbThresholds.icc"
#include "MassOverPtProbThresholds.icc"

  ////////////////////////////////////////////////////////
  // This macro plots selected jets with Nconst = nccut //
  // on top of the scatter plot.                        //
  ////////////////////////////////////////////////////////

  int nccolor(0);
  if ( nccut > 0 ) { 
    if ( scatter ) { 
      nccolor = kGreen;
    } else {
      nccolor = kBlack;
    }
  }

  gStyle->SetTitleOffset(1.95,"z");

  // allocate caches
  const std::vector<std::string>& sstr = signalNames;
  const std::vector<std::string>& pstr = procNames;

  std::string sigst;
  for ( auto s : sstr ) { if ( fname.find(s) != std::string::npos ) { sigst = s; break; } }
  if ( sigst == "" ) { printf("plotProbRelsSel[%i] - cannot find signal type in file name \042%s\042 - fatal\n",nccut,fname.c_str()); return false; }

  std::string pustr;
  for ( auto s : pstr ) { if ( fname.find(s) != std::string::npos ) { pustr = s; break; } }
  if ( pustr == "" ) { printf("plotProbRelsSel[%i] - cannot find pile-up indicator in file name \042%s\042 - fatal\n",nccut,fname.c_str()); return false; }

  int cidx(1959);
  TColor* mycolor = new TColor(cidx,152./255.,251./255.,152./255.);

  gStyle->SetPalette(kDarkBodyRadiator);
  //                                           Pt        m            W          PtD               Nc               MoPt
  std::vector<double>      varMin      = {      0.,        0.,     -0.02,       -0.02,              0.0,            -0.02 };
  std::vector<double>      varMax      = {    250.,       50.,      0.28,        1.02,              40.0,            1.02 };
  std::vector<int>         varBins     = {     125,        50,        30,          52,                40,              60 };  
  std::vector<bool>        varIsLog    = {    true,      true,     false,       false,             false,           false };
  std::vector<int>         varCase     = {       0,         1,         2,           3,                 4,               5 };
  std::vector<bool>        varIsInt    = {   false,     false,     false,       false,              true,           false };
  std::vector<double>      varGraphMin = {     15.,      0.25,     -0.02,       -0.02,               0.5,           -0.02 };
  std::vector<double>      varGraphMax = {    500.,       75.,      0.28,        1.02,              40.5,            0.34 };
  std::vector<double>      varRangeMin = {      0.,        0.,       0.0,          0.,               0.0,             0.0 };
  std::vector<double>      varRangeMax = {    250.,       50.,       0.4,          1.,              60.0,             1.0 };
  std::vector<std::string> varZName    = { "p_{T}", "m^{jet}", "w^{jet}", "P_{T}^{D}", "N_{const}^{jet}", "m^{jet}/p_{T}" };



  auto fvn = std::find(varName.begin(),varName.end(),vname);
  if ( fvn == varName.end() ) { printf("plotProbRelsSel[%i] - unknown variable name \042%s\042\n",nccut,vname.c_str()); return false; }

  size_t vidx = (size_t)varNameIndex[vname]; 

  std::string sltxt(varLabel.at(vidx));

  double      xamin(varMin.at(vidx));
  double      xamax(varMax.at(vidx));
  int         xbins(varBins.at(vidx));
  bool        xusel(varIsLog.at(vidx));
  int         xcase(varCase.at(vidx));

  double      vgmin(varGraphMin.at(vidx));
  double      vgmax(varGraphMax.at(vidx));

  if ( vname == "nc" ) { 
    if ( tupleFileSignals[fname] == "topotowf" ) { 
      xamin =  -1.;
      xamax = 299.;
      xbins = 150;
      vgmin = -1.;
      vgmax = 299.;
    } else if ( tupleFileSignals[fname] == "topotow" ) { 
      xamin = -0.5;
      xamax = 74.5;
      xbins = 75;
      vgmin = -0.5;
      vgmax = 59.5;
    }
  }

  double      yamin(-0.1);
  double      yamax(1.1);
  int         yabins(55);

  typedef boost::tuples::tuple<double,double,int> draw_t;
  std::vector<draw_t> prcPos = { draw_t(gStyle->GetPadLeftMargin(), 1.-gStyle->GetPadTopMargin()-0.025, 13),
				 draw_t(gStyle->GetPadLeftMargin(), 1.-gStyle->GetPadTopMargin()-0.025, 13),
				 draw_t(gStyle->GetPadLeftMargin(), 1.-gStyle->GetPadTopMargin()-0.025, 13),
				 draw_t(gStyle->GetPadLeftMargin(), 1.-gStyle->GetPadTopMargin()-0.025, 13),
				 draw_t(gStyle->GetPadLeftMargin(), 1.-gStyle->GetPadTopMargin()-0.025, 13),
				 draw_t(gStyle->GetPadLeftMargin(), 1.-gStyle->GetPadTopMargin()-0.025, 13) };
  double prcx0(prcPos.at(vidx).get<0>());
  double prcy0(prcPos.at(vidx).get<1>());
  int    prcta(prcPos.at(vidx).get<2>());

  // instantiate data inout chain 
  TChain* dc = new TChain("TagJets","TagJets");
  dc->Add(fname.c_str());

  std::unique_ptr<DataProvider> dp = std::unique_ptr<DataProvider>(new DataProvider(dc));
  if ( dp->LoadTree(0) < 0 ) { 
    printf("plotProbRelsSel[%i] - cannot allocate data in \042%s\042, fatal!\n",nccut,fname.c_str());
    return false;
  }

  dp->GetEntry(0);

  std::unique_ptr<TH1D> _lframe = std::unique_ptr<TH1D>(new TH1D("_lframe","_lframe",100,xamin,xamax));
  _lframe->GetXaxis()->SetTitle((std::string("signal jet ")+sltxt).c_str());
  _lframe->GetXaxis()->SetRangeUser(vgmin,vgmax);
  _lframe->GetYaxis()->SetTitle("P_{signal}");
  _lframe->GetXaxis()->SetNdivisions(6,2,0);
  _lframe->SetMinimum(-0.02);
  _lframe->SetMaximum(1.02);

  std::unique_ptr<TH1D> _rframe = std::unique_ptr<TH1D>(new TH1D("_rframe","_rframe",100,xamin,xamax));
  _rframe->GetXaxis()->SetTitle((std::string("pile-up jet ")+sltxt).c_str()); 
  _rframe->GetXaxis()->SetRangeUser(vgmin,vgmax);
  _rframe->GetYaxis()->SetTitle("P_{signal}");
  _rframe->GetXaxis()->SetNdivisions(6,2,0);
  _rframe->SetMinimum(-0.02);
  _rframe->SetMaximum(1.02);

  //////////////////
  // Collect data //
  //////////////////

  // y range
  double ymin(0.); double ymax(0.);
  if ( !HistHelper::Tags::yRange(ystr,ymin,ymax) ) {
    printf("plotProbRelsSel[%i] - cannot find y-range <%s>\n",nccut,ystr.c_str());
    return false;
  }

  // caches for unfiltered jets
  std::vector<double> xsig;
  std::vector<double> ysig;
  std::vector<double> xbck;
  std::vector<double> ybck;

  // caches for filtered jets
  std::vector<double> xfsg;
  std::vector<double> yfsg;
  std::vector<double> xfbk;
  std::vector<double> yfbk;

  int nybins(26); double ybmin(_lframe->GetMinimum()); double ybmax(_lframe->GetMaximum()); 

  std::unique_ptr<TH2D> dsig;
  std::unique_ptr<TH2D> dbck;

  int idx(0);
  std::vector<double>* pdata = 0;
  std::vector<double>* mdata = 0;
  std::vector<int>*    cdata = dp->ncJet;
  
  switch ( vidx ) { 
  case varIndexPt:
    dsig = std::unique_ptr<TH2D>(new TH2D("signal","signal",n_ptbins,a_ptbins,nybins,ybmin,ybmax));
    dbck = std::unique_ptr<TH2D>(new TH2D("pileup","pileup",n_ptbins,a_ptbins,nybins,ybmin,ybmax));  
    pdata = dp->ptJet;
    break;
  case varIndexM:
    dsig = std::unique_ptr<TH2D>(new TH2D("signal","signal",n_mbins,a_mbins,nybins,ybmin,ybmax)); 
    dbck = std::unique_ptr<TH2D>(new TH2D("pileup","pileup",n_mbins,a_mbins,nybins,ybmin,ybmax));  
    pdata = dp->mJet;
    break;
  case varIndexW:
    idx   = varIndexW;
    dsig = std::unique_ptr<TH2D>(new TH2D("signal","signal",xbins,xamin,xamax,nybins,ybmin,ybmax)); 
    dbck = std::unique_ptr<TH2D>(new TH2D("pileup","pileup",xbins,xamin,xamax,nybins,ybmin,ybmax));   
    pdata = dp->wJet;
    break;
  case varIndexPtD:
    idx   = varIndexPtD;
    dsig = std::unique_ptr<TH2D>(new TH2D("signal","signal",xbins,xamin,xamax,nybins,ybmin,ybmax)); 
    dbck = std::unique_ptr<TH2D>(new TH2D("pileup","pileup",xbins,xamin,xamax,nybins,ybmin,ybmax));   
    pdata = dp->ptdJet;
    break;
  case varIndexNC:
    idx   = varIndexNC;
    dsig = std::unique_ptr<TH2D>(new TH2D("signal","signal",xbins,xamin,xamax,nybins,ybmin,ybmax)); 
    dbck = std::unique_ptr<TH2D>(new TH2D("pileup","pileup",xbins,xamin,xamax,nybins,ybmin,ybmax));   
    pdata = new std::vector<double>; pdata->reserve(dp->ncJet->size());
    for ( auto fd : *(dp->ncJet) ) { pdata->push_back(static_cast<double>(fd)); }
    break;
  case varIndexMoP:
    idx   = varIndexMoP;
    dsig  = std::unique_ptr<TH2D>(new TH2D("signal","signal",n_mopbins,a_mopbins,nybins,ybmin,ybmax));
    dbck  = std::unique_ptr<TH2D>(new TH2D("pileup","pileup",n_mopbins,a_mopbins,nybins,ybmin,ybmax));
    pdata = dp->ptJet;
    mdata = dp->mJet;
    break;
  default:
    break;
  }

  if ( pdata == 0) { 
    printf("plotProbRelsSel[%i] - invalid data source with index %zu at %p - fatal\n",nccut,vidx,(void*)pdata);
    return false;
  }

  dsig->Sumw2(); dbck->Sumw2(); 

  xsig.reserve(pdata->size()); ysig.reserve(pdata->size());
  xbck.reserve(pdata->size()); ybck.reserve(pdata->size());
  // allocate additional data sources
  std::vector<double>* yjet = dp->yJetAve; std::vector<double>* prob = dp->psigJet; std::vector<bool>*   stag = dp->isSigJet;
  
  // extract data
  printf("plotProbRelsSel[%i] - look at %zu data entries\n",nccut,pdata->size());
  if ( mdata == 0 ) { 
    for ( size_t i(0); i<pdata->size(); ++i ) { 
      if ( yjet->at(i) > ymin && yjet->at(i) < ymax ) { 
	if ( stag->at(i) ) { 
	  xsig.push_back(pdata->at(i)); ysig.push_back(prob->at(i)); dsig->Fill(pdata->at(i),prob->at(i));
	  if ( ( nccut > 0 && cdata != 0 ) && cdata->at(i) == nccut ) { xfsg.push_back(pdata->at(i)); yfsg.push_back(prob->at(i)); }
	} else { 
	  xbck.push_back(pdata->at(i)); ybck.push_back(prob->at(i)); dbck->Fill(pdata->at(i),prob->at(i)); 
	  if ( ( nccut > 0 && cdata != 0 ) && cdata->at(i) == nccut ) { xfbk.push_back(pdata->at(i)); yfbk.push_back(prob->at(i)); }
	}
      }
    }
  } else {
    for ( size_t i(0); i<pdata->size(); ++i ) { 
      if ( yjet->at(i) > ymin && yjet->at(i) < ymax ) {
	double mop(mdata->at(i)/pdata->at(i)); 
	if ( stag->at(i) ) { 
	  xsig.push_back(mop); ysig.push_back(prob->at(i)); dsig->Fill(mop,prob->at(i)); 
	  if ( ( nccut > 0 && cdata != 0 ) && cdata->at(i) == nccut ) { xfsg.push_back(mop); yfsg.push_back(prob->at(i)); }
	} else { 
	  xbck.push_back(mop); ybck.push_back(prob->at(i)); dbck->Fill(mop,prob->at(i)); 
	  if ( ( nccut > 0 && cdata != 0 ) && cdata->at(i) == nccut ) { xfbk.push_back(mop); yfbk.push_back(prob->at(i)); }
	}
      }
    }
  }    

  // normalization scheme
  //  std::unique_ptr<TH2D> dsum = std::unique_ptr<TH2D>(HistHelper::Hist::histSum(dsig.get(),dbck.get()));

  ////////////////////
  // Plotting Frame //
  ////////////////////

  // TCanvas* tt = new TCanvas("test","test");

  double dxleft(gStyle->GetPadLeftMargin());
  double dxright(gStyle->GetPadRightMargin());
  double dytop(gStyle->GetPadTopMargin());
  double dybottom(gStyle->GetPadBottomMargin());

  double sides(dxleft+dxright);
  double width(1.-sides);
  
  double frame(dytop+dybottom);
  double height(1.-frame);

  double x0pos(0.);
  double x1pos(dxleft+width/2.);
  double u0pos(x1pos);
  double u1pos(1.);

  double y0pos(0.);
  double y1pos(dybottom+0.75*height);
  double v0pos(y0pos);
  double v1pos(y1pos);

  double bwidth(x1pos-x0pos);
  double bheight(y1pos-y0pos);

  double bwscf(1./bwidth);
  double bhscf(1./bheight);

  double bmleft(dxleft*bwscf);
  double bmright(dxright*bwscf);
  double bmbottom(dybottom*bhscf);

  TStyle* thisStyle = (TStyle*)gStyle;
  TPad*   thisPad   = (TPad*)gPad;

  TPad* insetLeft = new TPad("left","left",x0pos,y0pos,x1pos,y1pos);
  insetLeft->cd();
  insetLeft->SetFillStyle(4000);
  insetLeft->SetFillColorAlpha(kWhite,0.0);
  insetLeft->SetBorderSize(0);
  insetLeft->SetTicks(0,1);
  insetLeft->SetBottomMargin(bmbottom);
  insetLeft->SetTopMargin(0.02);
  insetLeft->SetLeftMargin(bmleft);
  insetLeft->SetRightMargin(0.);
  insetLeft->SetGrid();
  if ( varIsLog.at(vidx) ) { insetLeft->SetLogx(); _lframe->GetXaxis()->SetMoreLogLabels(); }

  TPad* insetRight = new TPad("right","right",u0pos,v0pos,u1pos,v1pos);
  insetRight->cd();
  insetRight->SetFillStyle(4000);
  insetRight->SetFillColorAlpha(kWhite,0.0);
  insetRight->SetBorderSize(0);
  insetRight->SetTicks(0,1);
  insetRight->SetBottomMargin(bmbottom);
  insetRight->SetTopMargin(0.02);
  insetRight->SetLeftMargin(0.);
  insetRight->SetRightMargin(bmright);
  insetRight->SetGrid();
  if ( varIsLog.at(vidx) ) { insetRight->SetLogx(); _rframe->GetXaxis()->SetMoreLogLabels(); }

  if ( scatter ) { 
    insetLeft->cd();
    _lframe->DrawCopy("axis");
    insetRight->cd();
    _rframe->DrawCopy("axis");
  }

  thisPad->cd();
  insetLeft->Draw("L");
  insetRight->Draw("R");

  ///////////
  // Plots //
  ///////////

  TGraph* gsig = new TGraph((int)xsig.size(),&xsig[0],&ysig[0]);
  TGraph* gbck = new TGraph((int)xbck.size(),&xbck[0],&ybck[0]);

  gsig->SetMarkerStyle(20); gsig->SetMarkerColor(kRed);  gsig->SetMarkerSize(0.5);
  gbck->SetMarkerStyle(20); gbck->SetMarkerColor(kBlue); gbck->SetMarkerSize(0.5);

  TGraph* gfsg = xfsg.size() > 0 ? new TGraph((int)xfsg.size(),&xfsg[0],&yfsg[0]) : 0;
  TGraph* gfbk = xfbk.size() > 0 ? new TGraph((int)xfbk.size(),&xfbk[0],&yfbk[0]) : 0;

  if ( !scatter ) { 
    //    dsig = std::unique_ptr<TH2D>(HistHelper::Hist::histRatio<TH2D>(dsig.get(),dsum.get())); 
    //    dbck = std::unique_ptr<TH2D>(HistHelper::Hist::histRatio<TH2D>(dbck.get(),dsum.get()));
    double norm(dsig->GetEntries()+dbck->GetEntries()); 
    if ( norm != 0. ) { dsig->Scale(1./norm); dbck->Scale(1./norm); }
    //    double vmin(1.); double vmax(0.); double wmin(1.); double wmax(0.);
    //    if ( !HistHelper::Hist::histEntriesRange<TH2D>(dsig.get(),vmin,vmax) || !HistHelper::Hist::histEntriesRange<TH2D>(dbck.get(),wmin,wmax) ) { return false; }
    // double zmin(std::min(vmin,wmin));
    // double zmax(std::max(vmax,wmax));
    double zmin(1e-5); double zmax(1.0);
    printf("plotProbRelsSel[%i] - z-value range [%6.4f,%6.4f]\n",nccut,zmin,zmax);
    dsig->GetZaxis()->SetRangeUser(zmin/5.,std::min(5.*zmax,1.));
    dbck->GetZaxis()->SetRangeUser(zmin/5.,std::min(5.*zmax,1.));
    dsig->GetXaxis()->SetRangeUser(vgmin,vgmax); 
    dsig->GetXaxis()->SetTitle(_lframe->GetXaxis()->GetTitle());
    dsig->GetYaxis()->SetTitle(_lframe->GetYaxis()->GetTitle());
    dbck->GetXaxis()->SetRangeUser(vgmin,vgmax); 
    dbck->GetXaxis()->SetTitle(_rframe->GetXaxis()->GetTitle());
    dbck->GetYaxis()->SetLabelOffset(999.);
    dbck->GetYaxis()->SetTitle("");
    dbck->GetZaxis()->SetTitle(TString::Format("N_{signal(pile-up) jet}(%s,P_{signal})/N_{all jets}",varZName.at(vidx).c_str()).Data());
    if ( varIsLog.at(vidx) ) { 
      dsig->GetXaxis()->SetMoreLogLabels();
      dbck->GetXaxis()->SetMoreLogLabels();
    }
    // plot selection
  }

  insetLeft->cd();
  insetLeft->SetLogz();
  if ( scatter ) { 
    gsig->Draw("px same");
    if ( gfsg != 0 ) { 
      gfsg->SetMarkerStyle(20); gfsg->SetMarkerColor(nccolor); gfsg->SetMarkerSize(0.5);
      gfsg->Draw("px same");
    }
    _lframe->DrawCopy("same axig");
    insetLeft->RedrawAxis();
  } else {
    dsig->DrawCopy("col");
    dsig->DrawCopy("same axig");
    if ( gfsg != 0 ) { 
      gfsg->SetMarkerStyle(20); gfsg->SetMarkerColor(nccolor); gfsg->SetMarkerSize(0.5);
      gfsg->Draw("px same");
    }
  }

  insetRight->cd();
  insetRight->SetLogz();
  if ( scatter ) { 
    gbck->Draw("px same");
    if ( gfbk != 0 ) { 
      gfbk->SetMarkerStyle(20); gfbk->SetMarkerColor(nccolor); gfbk->SetMarkerSize(0.5);
      gfbk->Draw("px same");
    }
    _rframe->DrawCopy("same axig");
    insetRight->RedrawAxis();
  } else {
    dbck->DrawCopy("colz");
    dsig->DrawCopy("same axig");
    if ( gfbk != 0 ) { 
      gfbk->SetMarkerStyle(20); gfbk->SetMarkerColor(nccolor); gfbk->SetMarkerSize(0.5);
      gfbk->Draw("px same");
    }
  }

  thisPad->cd();
  
  // TBox* bb = new TBox(x1pos-0.1,gStyle->GetPadBottomMargin()-0.05,x1pos+0.1,gStyle->GetPadBottomMargin()-0.01);
  // bb->SetFillStyle(1001);
  // bb->SetFillColorAlpha(kWhite,0.);
  // bb->SetLineStyle(0);
  // bb->Draw("same");

  HistHelper::Draw::description("00_15");
  HistHelper::Draw::AtlasLabel(prcx0,prcy0,"Simulation internal");
  prcy0 -= 0.05;

  double yl(0.); double yh(0.);
  printf("plotProbRelsSel[%i] - y-range string <%s>\n",nccut,ystr.c_str());
  if ( HistHelper::Tags::yRange(ystr,yl,yh) ) { 
    printf("plotProbRelsSel[%i] - y-range string <%s> extracted y in [%3.1f,%3.1f], put label at (%5.3f,%5.3f) with alignment %i\n",
	   nccut,ystr.c_str(),yl,yh,prcx0,prcy0,prcta);
    HistHelper::Draw::processing(fname,yl,yh,prcx0,prcy0,prcta); 
  }

  return true; 			      
}
