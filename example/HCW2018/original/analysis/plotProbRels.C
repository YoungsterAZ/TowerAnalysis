
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


#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <boost/tuple/tuple.hpp>

#define CONTEXT_PLOTPROBRELS
bool plotProbRels(const std::string& vname="pt",const std::string& ystr="00_15",const std::string& fname="tuple/JAPU+INCL_topoclus_tuple.root") {

#undef VARNAMES_ICC
#include "VarNames.icc"

#undef TUPLENAMES_ICC
#include "TupleFileNames.icc"

#include "PtProbThresholds.icc"
#include "MassProbThresholds.icc"

  // allocate caches
  const std::vector<std::string>& sstr = signalNames;
  const std::vector<std::string>& pstr = procNames;

  std::string sigst;
  for ( auto s : sstr ) { if ( fname.find(s) != std::string::npos ) { sigst = s; break; } }
  if ( sigst == "" ) { printf("plotProbRels - cannot find signal type in file name \042%s\042 - fatal\n",fname.c_str()); return false; }

  std::string pustr;
  for ( auto s : pstr ) { if ( fname.find(s) != std::string::npos ) { pustr = s; break; } }
  if ( pustr == "" ) { printf("plotProbRels - cannot find pile-up indicator in file name \042%s\042 - fatal\n",fname.c_str()); return false; }

  int cidx(1959);
  TColor* mycolor = new TColor(cidx,152./255.,251./255.,152./255.);

  gStyle->SetPalette(kDarkBodyRadiator);

  std::vector<double>      varMin   = {   20.,    0., -0.2,    0.,    0.0 };
  std::vector<double>      varMax   = {  150.,  200.,  0.5,   1.2,   70.0 };
  std::vector<int>         varBins  = {    65,   100,   50,   110,     35 };  
  std::vector<bool>        varIsLog = { false, false, false, false, false };
  std::vector<int>         varCase  = {     0,     1,     2,     3,     4 };
  std::vector<bool>        varIsInt = { false, false, false, false, true  }; 

  auto fvn = std::find(varName.begin(),varName.end(),vname);
  if ( fvn == varName.end() ) { printf("plotProbRels - unknown variable name \042%s\042\n",vname.c_str()); return false; }

  size_t vidx = (size_t)varNameIndex[vname]; 

  std::string sltxt(varLabel.at(vidx));

  double      xamin(varMin.at(vidx));
  double      xamax(varMax.at(vidx));
  int         xbins(varBins.at(vidx));
  bool        xusel(varIsLog.at(vidx));
  int         xcase(varCase.at(vidx));

  if ( vname == "nc" ) { 
    if ( tupleFileSignals[fname] == "topotowf" ) { 
      xamin = 0.;
      xamax = 400.;
      xbins = 100;
    } else if ( tupleFileSignals[fname] == "topotow" ) { 
      xamin = 0.;
      xamax = 140.;
      xbins = 70;
    }
  }

  double      yamin(-0.1);
  double      yamax(1.1);
  int         yabins(55);

  typedef boost::tuples::tuple<double,double,double,double> position_t;
  std::vector<position_t> legPos  = { position_t(0.625,gStyle->GetPadBottomMargin()+0.025,1.-gStyle->GetPadRightMargin()-0.025,gStyle->GetPadBottomMargin()+0.2),
				      position_t(0.625,gStyle->GetPadBottomMargin()+0.025,1.-gStyle->GetPadRightMargin()-0.025,gStyle->GetPadBottomMargin()+0.2),
				      position_t(0.625,gStyle->GetPadBottomMargin()+0.025,1.-gStyle->GetPadRightMargin()-0.025,gStyle->GetPadBottomMargin()+0.2),
				      position_t(0.625,gStyle->GetPadBottomMargin()+0.025,1.-gStyle->GetPadRightMargin()-0.025,gStyle->GetPadBottomMargin()+0.2),
				      position_t(0.625,gStyle->GetPadBottomMargin()+0.025,1.-gStyle->GetPadRightMargin()-0.025,gStyle->GetPadBottomMargin()+0.2) };

  double legx0(legPos.at(vidx).get<0>());
  double legx1(legPos.at(vidx).get<2>());
  double legy0(legPos.at(vidx).get<1>());
  double legy1(legPos.at(vidx).get<3>());

  if ( varIndexName[vidx] == "ptd" ) { 
    double legw(legx1-legx0);
    double legh(legy1-legy0);
    legx0 = 0.025;
    legx1 = legx0 + legw;
  }


  typedef boost::tuples::tuple<double,double,int> draw_t;
  std::vector<draw_t> prcPos = { draw_t(1.-gStyle->GetPadRightMargin()-0.025, 0.5, 33),
				 draw_t(1.-gStyle->GetPadRightMargin()-0.025, 0.5, 33),
				 draw_t(1.-gStyle->GetPadRightMargin()-0.025, 0.5, 33),
				 draw_t(1.-gStyle->GetPadRightMargin()-0.025, 0.5, 33),
				 draw_t(1.-gStyle->GetPadRightMargin()-0.025, 0.5, 33) };
  double prcx0(prcPos.at(vidx).get<0>());
  double prcy0(prcPos.at(vidx).get<1>());
  int    prcta(prcPos.at(vidx).get<2>());


  // instantiate data inout chain 
  TChain* dc = new TChain("TagJets","TagJets");
  dc->Add(fname.c_str());

  DataProvider* dp = new DataProvider(dc);
  if ( dp->LoadTree(0) < 0 ) { 
    printf("plotProbRels - cannot allocate data in \042%s\042, fatal!\n",fname.c_str());
    return false;
  }

  dp->GetEntry(0);

  //////////////////
  // Collect data //
  //////////////////

  // y range
  double ymin(0.); double ymax(0.);
  if ( !HistHelper::Tags::yRange(ystr,ymin,ymax) ) {
    printf("plotProbRels - cannot find y-range <%s>\n",ystr.c_str());
    return false;
  }

  // caches
  std::vector<double> xsig;
  std::vector<double> ysig;
  std::vector<double> xbck;
  std::vector<double> ybck;

  int nybins(50); double ybmin(0); double ybmax(1.); 

  TH2D* d_bck = 0; 
  int idx(0);
  std::vector<double>* pdata = 0;
  
  switch ( vidx ) { 
  case varIndexPt:
    d_bck = new TH2D("Pile-up","Pile-up",16,a_ptbins,nybins,ybmin,ybmax); d_bck->Sumw2();
    pdata = dp->ptJet;
    break;
  case varIndexM:
    d_bck = new TH2D("Pile-up","Pile-up",16,a_mbins,nybins,ybmin,ybmax);  d_bck->Sumw2();
    pdata = dp->mJet;
    break;
  case varIndexW:
    idx   = varIndexW;
    d_bck = new TH2D("Pile-up","Pile-up",xbins,xamin,xamax,nybins,ybmin,ybmax); d_bck->Sumw2();
    pdata = dp->wJet;
    break;
  case varIndexPtD:
    idx   = varIndexPtD;
    d_bck = new TH2D("Pile-up","Pile-up",xbins,xamin,xamax,nybins,ybmin,ybmax); d_bck->Sumw2();
    pdata = dp->ptdJet;
    break;
  case varIndexNC:
    idx   = varIndexNC;
    d_bck = new TH2D("Pile-up","Pile-up",xbins,xamin,xamax,nybins,ybmin,ybmax); d_bck->Sumw2();
    pdata = new std::vector<double>; pdata->reserve(dp->ncJet->size());
    for ( auto fd : *(dp->ncJet) ) { pdata->push_back(static_cast<double>(fd)); }
    break;
  default:
    break;
  }

  if ( d_bck == 0 || pdata == 0) { 
    printf("plotProbRels - invalid data source with index %zu at %p - fatal\n",vidx,(void*)pdata);
    return false;
  }

  xsig.reserve(pdata->size()); ysig.reserve(pdata->size());
  xbck.reserve(pdata->size()); ybck.reserve(pdata->size());
  // allocate additional data sources
  std::vector<double>* yjet = dp->yJetAve; std::vector<double>* prob = dp->psigJet; std::vector<bool>*   stag = dp->isSigJet;
  
  // extract data
  printf("plotProbRels - look at %zu data entries\n",pdata->size());
  for ( size_t i(0); i<pdata->size(); ++i ) { 
    if ( yjet->at(i) > ymin && yjet->at(i) < ymax ) { 
      if ( stag->at(i) ) { xsig.push_back(pdata->at(i)); ysig.push_back(prob->at(i)); 
      } else { xbck.push_back(pdata->at(i)); ybck.push_back(prob->at(i)); d_bck->Fill(pdata->at(i),prob->at(i)); }
    }
  }

  //////////
  // Plot //
  //////////

  TGraph* gsig = new TGraph((int)xsig.size(),&xsig[0],&ysig[0]);

  // format hist
  d_bck->GetXaxis()->SetTitle(sltxt.c_str());
  d_bck->GetYaxis()->SetTitle("P_{signal}");
  d_bck->SetMinimum(0.5);
  d_bck->SetMaximum(5.*d_bck->GetMaximum());
  d_bck->GetYaxis()->SetRangeUser(-0.1,1.5);
  d_bck->DrawCopy("colz");

  gsig->SetMarkerStyle(kStar); gsig->SetMarkerColor(kRed);  gsig->SetMarkerSize(1.);
  gsig->Draw("p same");

  gPad->RedrawAxis();

  TLegend* leg = new TLegend(legx0,legy0,legx1,legy1);
  TBox* tb = new TBox(0.,0.,1.,1.);
  tb->SetFillColor(TColor::GetColorPalette(255/2));
  tb->SetLineColor(0);
  TGraph* ppg = new TGraph(*gsig);
  ppg->SetMarkerSize(1.1);
  TLegendEntry* l1 = leg->AddEntry(ppg,"signal jets","p"); l1->SetTextFont(42);
  TLegendEntry* l2 = leg->AddEntry(tb,"pile-up jets","f"); l2->SetTextFont(42);
  leg->Draw("same");

  double yl(0.); double yh(0.);
  if ( HistHelper::Tags::yRange(ystr,yl,yh) ) { HistHelper::Draw::processing(fname,yl,yh,prcx0,prcy0,prcta); }
  HistHelper::Draw::description("00_15");

  return true; 			      
}
