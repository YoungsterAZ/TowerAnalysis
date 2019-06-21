

#include "HistHelper.h"
#include "DataProvider.C"
#include "DataBase.h"

#include <TH1D.h>
#include <TChain.h>
#include <TString.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLegendEntry.h>

#include <string>
#include <vector>
#include <algorithm>

void plotAveValues(const std::string& vname="pt") {

  gStyle->SetPadRightMargin(0.05);
  gStyle->SetEndErrorSize(1.5);
  
  std::vector<std::string> varName  = { "pt", "m", "w", "ptd", "nc" };
  std::vector<std::string> varLabel = { "#LTp_{T}^{jet}#GT^{#plusRMS#uparrow}_{#minusRMS#downarrow}(y^{jet}) [GeV]",
					"#LTm^{jet}#GT^{#plusRMS#uparrow}_{#minusRMS#downarrow}(y^{jet}) [GeV]",
					"#LTw^{jet}#GT^{#plusRMS#uparrow}_{#minusRMS#downarrow}(y^{jet})",
					"#LTP_{T}^{D}#GT^{#plusRMS#uparrow}_{#minusRMS#downarrow}(y^{jet})",
					"#LTN_{const}^{jet}#GT^{#plusRMS#uparrow}_{#minusRMS#downarrow}(y^{jet})" };
  std::vector<std::string> varUnits = { "GeV", "GeV", "", "", "" };

  std::vector<double>      varMin   = {    0.,    0.,    0.,    0.,    0.0 };
  std::vector<double>      varMax   = { 1000.,   70.,   0.4,   2.0,  300.0 };
  std::vector<bool>        varIsLog = { false, false, false, false,  false };
  std::vector<int>         varCase  = {     0,     1,     2,     3,      4 };

  std::vector<double>      varNbins = {   150,   150, 100, 120,   700 };
  std::vector<double>      varXmin  = {    0.,    0.,  0.,  0.,  -0.5 };
  std::vector<double>      varXmax  = { 1500., 1500.,  1., 1.2, 699.5 };  

  std::vector<int>         varPrecA = {     1,    1,    2,   2,     1 };
  std::vector<int>         varPrecE = {     1,    2,    3,   3,     1 };


  std::vector<double>      varXatls = { 1.0-gStyle->GetPadRightMargin()-0.05, 
					1.0-gStyle->GetPadRightMargin()-0.05, 
					gStyle->GetPadLeftMargin()+0.05, 
					gStyle->GetPadLeftMargin()+0.05, 
					gStyle->GetPadLeftMargin()+0.05  }; 
  std::vector<double>      varYatls = {  1.0-gStyle->GetPadTopMargin()-0.025,  
					 1.0-gStyle->GetPadTopMargin()-0.025, 
					 1.0-gStyle->GetPadTopMargin()-0.025, 
					 1.0-gStyle->GetPadTopMargin()-0.025, 
					 1.0-gStyle->GetPadTopMargin()-0.025  };
  std::vector<int>         varLatls = {                                   33,
									  33,
									  13,
									  13,
									  13  };
  std::vector<double> varXproc(varXatls);
  std::vector<double> varYproc; for ( auto yy : varYatls ) { varYproc.push_back(yy-0.025); } 
  std::vector<int>    varLproc = { 32, 32, 12, 12, 12 };

  std::vector<double> varXlegl = { 0.75, 0.75, 0.75, 0.75, 0.25 };
  std::vector<double> varXlegr = { varXatls.at(0), varXatls.at(0), varXatls.at(0), varXatls.at(0), varXatls.at(0)-0.5 };

  std::vector<double> varYlegl = { 0.60, 0.60, 0.725, 0.725, 0.6 };
  std::vector<double> varYlegh = { 1-gStyle->GetPadTopMargin()-0.15,
				   1-gStyle->GetPadTopMargin()-0.15,
				   1-gStyle->GetPadTopMargin()-0.025,
				   1-gStyle->GetPadTopMargin()-0.15,
				   1-gStyle->GetPadTopMargin()-0.15 };


  auto fvn = std::find(varName.begin(),varName.end(),vname);
  if ( fvn == varName.end() ) { printf("plotAveValues - unknown variable name \042%s\042\n",vname.c_str()); return; }

  size_t vidx(size_t(fvn-varName.begin()));
  if ( vidx >= varLabel.size() ) { printf("plotAveValues - invalid index for variable name \042%s\042\n",vname.c_str()); return; }

  std::string yltxt(varLabel.at(vidx));
  std::string yunit(varUnits.at(vidx));

  double      yamin(varMin.at(vidx));
  double      yamax(varMax.at(vidx));
  bool        yusel(varIsLog.at(vidx));
  int         ycase(varCase.at(vidx));
  int         ynbin(varNbins.at(vidx));
  double      yxmin(varXmin.at(vidx));
  double      yxmax(varXmax.at(vidx));
  int         yaprc(varPrecA.at(vidx));
  int         yeprc(varPrecE.at(vidx));

  double      atlsx(varXatls.at(vidx));
  double      atlsy(varYatls.at(vidx));
  int         atlsl(varLatls.at(vidx));

  double      procx(varXproc.at(vidx));
  double      procy(varYproc.at(vidx));
  int         procl(varLproc.at(vidx));

  double      legxl(varXlegl.at(vidx));
  double      legxr(varXlegr.at(vidx));

  double      legyl(varYlegl.at(vidx));
  double      legyh(varYlegh.at(vidx));

  double      dstep(75.*(yamax-yamin)/(varMax.at(0)-varMin.at(0)));

  printf("\nplotAveValues - collect and plot variable <%s> (case %1i)\n\n",vname.c_str(),ycase);

  std::vector<std::string> putags = { "JAPU+INCL", "JAPU+TIME", "CSPU+INCL", "CSPU+TIME", "CSSK+INCL", "CSSK+TIME" };
  std::vector<std::string> stags  = { "topoclus",  "topotowf",  "topotow"                                          };
  std::vector<std::string> yrstr  = { "00_15",     "15_25",     "25_32",     "32_49"                               };

  TH1D* _frame = new TH1D("_frame","_frame",100,0.,4.5);
  _frame->GetXaxis()->SetTitle("|y^{jet}|");
  _frame->GetYaxis()->SetTitle(yltxt.c_str());
  _frame->SetMinimum(yamin);
  _frame->SetMaximum(yamax);

  DataBase* sigDB = new DataBase("signal"); sigDB->printData();
  DataBase* bckDB = new DataBase("backgd"); bckDB->printData();

  for ( auto pu : putags ) {
    // new plot 
    for ( auto st : stags ) { 
      // work space
      std::vector<TH1D*> sigv;
      std::vector<TH1D*> bckv;
      // instantiate input 
      std::string ifname(TString::Format("tuple/%s_%s_tuple.root",pu.c_str(),st.c_str()).Data());
      TChain* ch = new TChain("TagJets","TagJets");
      ch->Add(ifname.c_str());
      // allocate data (only one event)
      DataProvider* dp = new DataProvider(ch); 
      // load tree
      if ( dp->LoadTree(0) < 0. ) { 
	printf("plotAveValues - problems allocating tree in file \042%s\042\n",ifname.c_str());
	delete ch;
	delete dp;
	continue; 
      }
      // get entry
      dp->fChain->GetEntry(0);
      // check
      printf("plotAveValues - file \042%s\042 found %zu jets\n",ifname.c_str(),dp->yJetReg->size());
      // book all histograms
      for ( auto ys : yrstr ) { 
	sigv.push_back(new TH1D(TString::Format("h_signal_%s_%s_%s",pu.c_str(),st.c_str(),ys.c_str()),"Variable",ynbin,yxmin,yxmax)); sigv.back()->Sumw2();
	bckv.push_back(new TH1D(TString::Format("h_backgd_%s_%s_%s",pu.c_str(),st.c_str(),ys.c_str()),"Variable",ynbin,yxmin,yxmax)); sigv.back()->Sumw2();
      }
      printf("plotAveValues - booked 2 x %zu histograms\n",sigv.size());
      //
      size_t njets(dp->ptJet->size());
      std::vector<double> sigym(sigv.size(),0.);
      std::vector<double> sigyl(sigv.size(),0.);
      std::vector<double> sigyr(sigv.size(),0.);
      std::vector<double> bckym(sigv.size(),0.);
      std::vector<double> bckyl(sigv.size(),0.);
      std::vector<double> bckyr(sigv.size(),0.);
      std::vector<double> delml(sigv.size(),0.);
      std::vector<double> delmr(sigv.size(),0.);
      // fill rapdity ranges
      int yrmin(100); int yrmax(0);
      for ( size_t i(0); i<njets; ++i ) {
	int iy(dp->yJetReg->at(i));
	if ( iy < yrmin ) { yrmin = iy; }
	if ( iy > yrmax ) { yrmax = iy; }
      } 
      if ( yrmin >= 0 && (size_t)yrmax < yrstr.size() ) {
	printf("plotAveValues - index range for rapdity bins is [%i,%i]\n",yrmin,yrmax); 
	for ( int k(yrmin); k < yrmax+1; ++k ) {
	  // set signl coordinates
	  sigym[k] = sigDB->binAverage(sigDB->find(pu,st),yrstr.at(k));
	  sigyl[k] = sigDB->binLeftWidth(sigDB->find(pu,st),yrstr.at(k));
	  sigyr[k] = sigDB->binRightWidth(sigDB->find(pu,st),yrstr.at(k));
	  // get background coordinates
	  bckym[k] = bckDB->binAverage(bckDB->find(pu,st),yrstr.at(k));  
	  bckyl[k] = bckDB->binLeftWidth(bckDB->find(pu,st),yrstr.at(k));  
	  bckyr[k] = bckDB->binRightWidth(bckDB->find(pu,st),yrstr.at(k));
	  // get delimiters  
	  delml[k] = sigDB->binLowerBound(sigDB->find(pu,st),yrstr.at(k));
	  delmr[k] = std::min(sigDB->binUpperBound(sigDB->find(pu,st),yrstr.at(k)),4.5);
	} 
      }
      // loop tuple jets
      if ( ycase == 0 ) { 
	HistHelper::Data::fillHists<std::vector<double> >(*(dp->ptJet),*(dp->isSigJet),*(dp->yJetReg),sigv,bckv);
      } else if ( ycase == 1 ) {
      	HistHelper::Data::fillHists<std::vector<double> >(*(dp->mJet),*(dp->isSigJet),*(dp->yJetReg),sigv,bckv);
      } else if ( ycase == 2 ) { 
      	HistHelper::Data::fillHists<std::vector<double> >(*(dp->wJet),*(dp->isSigJet),*(dp->yJetReg),sigv,bckv);
      } else if ( ycase == 3 ) { 
      	HistHelper::Data::fillHists<std::vector<double> >(*(dp->ptdJet),*(dp->isSigJet),*(dp->yJetReg),sigv,bckv);
      } else if ( ycase == 4 ) { 
	HistHelper::Data::fillHists<std::vector<int> >(*(dp->ncJet),*(dp->isSigJet),*(dp->yJetReg),sigv,bckv);
      }
      // collect graphics data
      std::vector<double> ptaves(sigv.size(),0.); std::vector<double> dptdns(sigv.size(),0.); std::vector<double> dptups(sigv.size(),0.);
      std::vector<double> ptaveb(sigv.size(),0.); std::vector<double> dptdnb(sigv.size(),0.); std::vector<double> dptupb(sigv.size(),0.);
      for ( size_t i(0); i<sigv.size(); ++i ) { 
	HistHelper::Hist::stats(sigv.at(i),ptaves[i],dptups[i],dptdns[i],"rms","mean");
	HistHelper::Hist::stats(bckv.at(i),ptaveb[i],dptupb[i],dptdnb[i],"rms","mean");
	printf("plotAveValues - configuration [%s]_%s y range <%s> signal %8.3f+%8.3f-%8.3f GeV; background %8.3f+%8.3f-%8.3f GeV\n",
	       pu.c_str(),st.c_str(),yrstr.at(i).c_str(),ptaves.at(i),dptups.at(i),dptdns.at(i),ptaveb.at(i),dptupb.at(i),dptdnb.at(i));
      }
      // get graphics objects
      printf("plotAveValues - x/y value array size is %zu/%zu\n",sigym.size(),ptaves.size());
      TGraphAsymmErrors* pgsig = HistHelper::Data::fillTGraphAsymmErrors(sigym,ptaves,sigyl,sigyr,dptdns,dptups);
      printf("plotAveValues - number of points in graph %i\n",pgsig->GetN());
      printf("plotAveValues - signal coordinates\n");
      for ( int i(0); i<pgsig->GetN(); ++i ) {
	printf("                {Vx,Vy,Vdxl,Vdxh,Vydxl,Vydxh}, {Ax,Ay,Adxl,Adxh,Adyl,Adyh} = {%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f}, {%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f}\n",
	       sigym.at(i),ptaves.at(i),sigyl.at(i),sigyr.at(i),dptdns.at(i),dptups.at(i),
	       pgsig->GetX()[i],pgsig->GetY()[i],pgsig->GetErrorXlow(i),pgsig->GetErrorXhigh(i),pgsig->GetErrorYlow(i),pgsig->GetErrorYhigh(i)
	       );
      }
      HistHelper::Draw::setCommonStyle(pgsig,kRed,20,1.0);pgsig->SetFillColor(kYellow);pgsig->SetFillStyle(3153);
      TGraphAsymmErrors* pgbck = HistHelper::Data::fillTGraphAsymmErrors(bckym,ptaveb,bckyl,bckyr,dptdnb,dptupb);  
      HistHelper::Draw::setCommonStyle(pgbck,kBlue,20,1.0);pgbck->SetFillColor(kCyan); pgbck->SetFillStyle(3135);
      // plot
      TCanvas* aa = new TCanvas(TString::Format("%s_%s_Ave%s",pu.c_str(),st.c_str(),vname.c_str()).Data(),TString::Format("[%s]_%s_AvePt",pu.c_str(),st.c_str()).Data());
      if ( yusel ) { aa->SetLogy(); }
      _frame->DrawCopy("axis");
      // if ( ycase != 2 ) {
      // 	pgsig->Draw("2 same"); //pgsig->Draw("p same");
      // 	pgbck->Draw("2 same"); pgbck->Draw("p same"); pgsig->Draw("p same");
      // } else {
	pgbck->Draw("2 same"); //pgsig->Draw("p same");
	pgsig->Draw("2 same"); pgsig->Draw("p same"); pgbck->Draw("p same");
	//      }
      TLatex tf;
      tf.SetTextSize(tf.GetTextSize()*2./3.);
      tf.SetTextAlign(23);
      for ( size_t i(0); i<sigym.size(); ++i) {
	double ys(pgsig->GetY()[i]+pgsig->GetErrorYhigh(i));
	double yb(pgbck->GetY()[i]+pgbck->GetErrorYhigh(i));
	double y = ys > yb ? ys+2.*dstep : yb+2.*dstep; 
	tf.SetTextColor(pgsig->GetMarkerColor());
	if ( yunit != "" ) { 
	  tf.DrawLatex((delmr.at(i)+delml.at(i))/2.,y,
		       TString::Format("%.*f^{ #plus%.*f}_{ #minus%.*f} %s",
				       yaprc,pgsig->GetY()[i],yeprc,pgsig->GetErrorYhigh(i),yeprc,pgsig->GetErrorYlow(i),yunit.c_str()).Data());
	  y -= dstep;
	  tf.SetTextColor(pgbck->GetMarkerColor());
	  tf.DrawLatex((delmr.at(i)+delml.at(i))/2.,y,
		       TString::Format("%.*f^{ #plus%.*f}_{ #minus%.*f} %s",
				       yaprc,pgbck->GetY()[i],yeprc,pgbck->GetErrorYhigh(i),yeprc,pgbck->GetErrorYlow(i),yunit.c_str()).Data());
	} else { 
	  tf.DrawLatex((delmr.at(i)+delml.at(i))/2.,y,
		       TString::Format("%.*f^{ #plus%.*f}_{ #minus%.*f}",yaprc,pgsig->GetY()[i],yeprc,pgsig->GetErrorYhigh(i),yeprc,pgsig->GetErrorYlow(i)).Data());
	  y -= dstep;
	  tf.SetTextColor(pgbck->GetMarkerColor());
	  tf.DrawLatex((delmr.at(i)+delml.at(i))/2.,y,
		       TString::Format("%.*f^{ #plus%.*f}_{ #minus%.*f}",yaprc,pgbck->GetY()[i],yeprc,pgbck->GetErrorYhigh(i),yeprc,pgbck->GetErrorYlow(i)).Data());
	}
      }
      //
      HistHelper::Draw::description("00_15");
      // double xl(gStyle->GetPadLeftMargin()+delmr[0]/_frame->GetXaxis()->GetXmax()*(1.-(gStyle->GetPadLeftMargin()+gStyle->GetPadRightMargin())));
      // HistHelper::Draw::AtlasLabel(xl,yl,"Simulation internal");
      //      double xl(1.0-gStyle->GetPadRightMargin()); xl -= 0.050;
      //      double yl(1.0-gStyle->GetPadTopMargin());   yl -= 0.025;
      HistHelper::Draw::AtlasLabel(atlsx,atlsy,"Simulation internal",atlsl);
      //      yl -= 0.025;
      HistHelper::Draw::processing(procx,procy,HistHelper::Names::extractSignal(st),HistHelper::Names::extractPileup(pu),HistHelper::Names::extractMode(pu),procl);

      //      double xl(varXatls.at(0));
      //      double yl(1-gStyle->GetPadTopMargin()-0.15);   //0.05;
      //      TLegend* leg = new TLegend(0.75,0.60,xl,yl);
      TLegend* leg = new TLegend(legxl,legyl,legxr,legyh);
      leg->SetFillStyle(1001);
      leg->SetFillColor(kWhite);
      leg->SetLineColor(kBlack);
      leg->SetLineWidth(1);
      TLegendEntry* l1 = leg->AddEntry(pgsig," signal jets","lpfe");   l1->SetTextFont(42); l1->SetMarkerSize(pgsig->GetMarkerSize());
      TLegendEntry* l2 = leg->AddEntry(pgbck," pile-up jets","lpfe");  l2->SetTextFont(42); l2->SetMarkerSize(pgbck->GetMarkerSize());
      leg->Draw("same");

      aa->Print(TString::Format("%s/%save_vs_y_%s.pdf",pu.c_str(),vname.c_str(),st.c_str()).Data());
      aa->Print(TString::Format("%s/%save_vs_y_%s.png",pu.c_str(),vname.c_str(),st.c_str()).Data());
      _frame->DrawCopy("same axis");
    } // loop on signal type
  } // loop on pile-up suppression methods
}
