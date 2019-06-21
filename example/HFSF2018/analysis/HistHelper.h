// -*- c++ -*-
#ifndef HISTHELPER_H
#define HISTHELPER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <limits>

#include <TStyle.h>
#include <TLatex.h>
#include <TString.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TH1D.h>
#include <TH2D.h>

namespace HistHelper {
  namespace Tags {
    static const std::string vbf = "VBF";
    static const std::string mcg = "Pythia8";
    static const double      cms = 2600.  ;
    static const double      sqs = 14000.0;
    static const double      amu = 200.; 
    static const std::vector<std::pair<double,double> > yregions = 
      { 
	std::pair<double,double>(0.,1.5), 
	std::pair<double,double>(1.5,2.5), 
	std::pair<double,double>(2.5,3.2),
	std::pair<double,double>(3.2,4.5)
      };
    static std::map<std::string,std::pair<double,double> > ylookup = std::map<std::string,std::pair<double,double> >();
    static const std::map<std::string,std::pair<double,double> >& regionLookup() {
      if ( ylookup.empty() ) { 
	ylookup["00_15"] = yregions.at(0);
	ylookup["15_25"] = yregions.at(1);
	ylookup["25_32"] = yregions.at(2);
	ylookup["32_49"] = yregions.at(3);
      }
      return ylookup;
    }
    static double ylow(const std::string& ystr) { 
      auto fmap = regionLookup().find(ystr); return fmap == regionLookup().end() ? 0. : fmap->second.first; 
    }
    static double yhigh(const std::string& ystr) { 
      auto fmap = regionLookup().find(ystr); return fmap == regionLookup().end() ? -1. : fmap->second.second; 
    }
    static bool yRange(const std::string& ystr,double& yl,double& yh) { 
      auto fmap = regionLookup().find(ystr); 
      if ( fmap == regionLookup().end() ) { 
	return false;
      } else { 
	yl = fmap->second.first; yh = fmap->second.second; return true; 
      }
    }
  } // Tags
  namespace Names {
    static std::map<std::string,std::string> signalmap = std::map<std::string,std::string>();
    static const std::map<std::string,std::string>& lookupSignal() { 
      if ( signalmap.empty() ) { 
	signalmap["topoclus"] = "topo-cluster";
	signalmap["topotow"]  = "topo-tower 0.1#times0.1";
	signalmap["topotowf"] = "topo-tower 0.05#times0.05";
	signalmap["unknown"]  = "unknown calorimeter signal";
      }
      return signalmap;
    } 
    static std::string signalName(const std::string& signalTag) {
      auto fmap = lookupSignal().find(signalTag);
      if ( fmap == lookupSignal().end() ) { 
	printf("HistHelper::Names::signalName(\042%s\042) - no name found for this tag\n",signalTag.c_str());
	return lookupSignal().find("unknown")->second;
      }
      return fmap->second;
    }
    static std::map<std::string,std::string> pileupmap = std::map<std::string,std::string>();
    static const std::map<std::string,std::string>& lookupPileup() {
      if ( pileupmap.empty() ) {
	pileupmap["JAPU"]    = "JA";
	pileupmap["CSPU"]    = "CS";
	pileupmap["CSSK"]    = "CS+SK";
	pileupmap["unknown"] = "unknown pile-up suppression technique";
      }
      return pileupmap;
    }
    static std::string pileupName(const std::string& pileupTag) {
      auto fmap = lookupPileup().find(pileupTag);
      if ( fmap == lookupPileup().end() ) { 
	printf("HistHelper::Names::pileupName(\042%s\042) - no name found for this tag\n",pileupTag.c_str());
	return lookupPileup().find("unknown")->second;
      }
      return fmap->second;
    }
    static std::map<std::string,std::string> modemap = std::map<std::string,std::string>();
    static const std::map<std::string,std::string>& lookupMode() {
      if ( modemap.empty() ) { 
	modemap["INCL"]    = "inclusive";
	modemap["TIME"]    = "signal time filter";
	modemap["unknown"] = "unknown mode";
      }
      return modemap;
    }
    static std::string modeName(const std::string& modeTag) {
      auto fmap = lookupMode().find(modeTag);
      if ( fmap == lookupMode().end() ) { 
	printf("HistHelper::Names::modeName(\042%s\042) - no name found for this tag\n",modeTag.c_str());
	return lookupMode().find("unknown")->second;
      }
      return fmap->second;
    }
    static std::string extractSignal(const std::string& fname) {
      static std::vector<std::string> _knownTags = { "topoclus", "topotowf", "topotow" };
      std::string name(signalName("unknown"));
      for ( auto tag : _knownTags ) { 
	if ( fname.find(tag) != std::string::npos ) { name = signalName(tag); break; }
      }
      return name;
    }

    static std::string extractPileup(const std::string& fname) {
      static std::vector<std::string> _knownTags = { "JAPU", "CSPU", "CSSK" };
      std::string name(pileupName("unknown"));
      for ( auto tag : _knownTags ) { 
	if ( fname.find(tag) != std::string::npos ) { name = pileupName(tag); break; }
      }
      return name;
    }

    static std::string extractMode(const std::string& fname) { 
      static std::vector<std::string> _knownTags = { "INCL", "TIME" };
      std::string name(modeName("unknown"));
      for ( auto tag : _knownTags ) { 
	if ( fname.find(tag) != std::string::npos ) { name = modeName(tag); break; }
      }
      return name;
    }

    static std::string extractYrange(const std::string& fname) {
      size_t ft(fname.find_last_of('.'));
      if ( ft == std::string::npos ) { 
	ft = fname.find_last_of('_');
	printf("HistHelper::Names::extractYrange(\042%s\042) - found separator at %zu/%zu\n",fname.c_str(),ft,fname.length());
	if ( ft != std::string::npos && fname.length()-ft == 3 ) { 
	  return fname.substr(fname.length()-5);
	} else { 
	  return std::string();
	} 
      }
      return fname.substr(ft-5,5);
    }

    static bool extractSignalAndPileup(const std::string& fname,std::string& signame,std::string& puname) {
      std::string sn(extractSignal(fname)); if ( sn == signalName("unknown") ) { return false; }
      std::string pn(extractPileup(fname)); if ( pn == pileupName("unknown") ) { return false; }
      signame = sn; puname = pn;
      return true;
    }
    static bool extractAllNames(const std::string& fname,std::string& signame,std::string& puname,std::string& mname)
    {
      if ( extractSignalAndPileup(fname,signame,puname) ) { 
	std::string mn(extractMode(fname)); if ( mn == modeName("unknown") ) { return false; }
	mname = mn; 
	return true;
      } else {
	return false; 
      }
    }
    static std::string stripExtension(const std::string& fname) { 
      auto fi = fname.find_last_of('.');
      if ( fi == std::string::npos ) { return fname; }
      return fname.substr(0,fi);
    }
    static std::string findNameKernel(const std::string& fname) { 
      static std::vector<std::string> _knownTags = { "JAPU", "CSPU", "CSSK" };
      size_t fi(0); 
      for ( auto tag : _knownTags ) { 
	fi = fname.find(tag); if ( fi != std::string::npos ) { break; } 
      }
      if ( fi == std::string::npos ) { return std::string(); }
      return stripExtension(fname.substr(fi));
    }
    static std::string histName(const std::string& fstr,const std::string& hn,bool isFmt=false) {
      static std::string _sep = "_";
      return isFmt ? std::string(TString::Format(fstr.c_str(),hn.c_str()).Data()) : findNameKernel(fstr)+_sep+hn;
    }
    static std::string pdfFileName(const std::string& fname,const std::string& var) {
      static std::string _sep = "_";
      static std::string _ext = ".pdf";
      return findNameKernel(fname)+_sep+var+_ext;
    }
    static std::string pngFileName(const std::string& fname,const std::string& var) {
      static std::string _sep = "_";
      static std::string _ext = ".png";
      return findNameKernel(fname)+_sep+var+_ext;
    }
    static std::string jpgFileName(const std::string& fname,const std::string& var) {
      static std::string _sep = "_";
      static std::string _ext = ".jpg";
      return findNameKernel(fname)+_sep+var+_ext;
    }
  } // Names
  namespace Draw { 
    static void description(const std::string& /*etaRange*/,double x,double y) {
      // double yl(0.); double yh(0.);
      // if ( !HistHelper::Tags::yRange(etaRange,yl,yh) ) { 
      // 	printf("HistHelper::Draw::description(\042%s\042,%.2f,%.2f) - cannot find y range, no description\n",
      // 	       etaRange.c_str(),x,y);
      // 	return;
      // }
      TLatex tl;
      tl.SetTextAlign(33); tl.SetTextSize(tl.GetTextSize()*2./3.);
      tl.SetNDC();
      std::string msg(TString::Format("%s %s M = %.1f TeV at #sqrt{s} = %.0f TeV, #LT#mu#GT = %.0f",
				      HistHelper::Tags::mcg.c_str(),HistHelper::Tags::vbf.c_str(),HistHelper::Tags::cms/1000.,
				      HistHelper::Tags::sqs/1000.,HistHelper::Tags::amu).Data());
      tl.DrawLatex(x,y,msg.c_str());
      // printf("HistHelper::Draw::description(\042%s\042,%.2f,%.2f) - draw \042%s\042\n",
      // 	     etaRange.c_str(),x,y,msg.c_str());
    }
    static void description(const std::string& etaRange) {
      auto thisStyle = gStyle;//ROOT->GetStyle();
      double x(1.0-thisStyle->GetPadRightMargin());
      double y(0.997); 
      description(etaRange,x,y);
    }
    static void AtlasLabel(double x,double y,const std::string& tag="",int talign=13) {
      TLatex tl;
      tl.SetNDC();
      tl.SetTextFont(72);
      tl.SetTextAlign(talign);
      if ( tag.empty() || tag == "" ) {
	tl.DrawLatex(x,y,"ATLAS"); 
      } else {
	tl.DrawLatex(x,y,TString::Format("ATLAS #font[42]{%s}",tag.c_str()).Data());
	//	tl.DrawLatex(x,y,TString::Format("#splitline{ATLAS}{#scale[0.75]{#font[42]{%s}}}",tag.c_str()).Data());
      }
    }
    static void AtlasLabel(const std::string& tag="") {
      double x = gStyle->GetPadLeftMargin();    x += 0.050; 
      double y = 1.0-gStyle->GetPadTopMargin(); y -= 0.025;
      AtlasLabel(x,y,tag); 
    }
    static void processing(const std::string& sname,const std::string& pname,const std::string& mname) {
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize(tl.GetTextSize()/1.75);
      tl.SetTextAlign(12);
      double x = gStyle->GetPadLeftMargin();    x += 0.050; 
      double y = 1.0-gStyle->GetPadTopMargin(); y -= 0.050;
      tl.DrawLatex(x,0.82+0.04,TString::Format("Anti-k_{t} R = 0.4 jets, %s %s",mname.c_str(),sname.c_str()).Data());
      tl.DrawLatex(x,0.79+0.04,TString::Format("%s pile-up suppression",pname.c_str()).Data());
    }
    static void processing(double x,double y,const std::string& sname,const std::string& pname,const std::string& mname,int talign=12) {
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize(tl.GetTextSize()/1.75);
      tl.SetTextAlign(talign);
      tl.DrawLatex(x,0.82+0.04,TString::Format("Anti-k_{t} R = 0.4 jets, %s %s",mname.c_str(),sname.c_str()).Data());
      tl.DrawLatex(x,0.79+0.04,TString::Format("%s pile-up suppression",pname.c_str()).Data());
    }
    static void processing(const std::string& fname) {
      std::string sn; std::string pn; std::string mn;
      if ( !HistHelper::Names::extractAllNames(fname,sn,pn,mn) ) { 
	printf("HistHelper::Draw::processing() - cannot extract names\n");
	return;
      }
      double yl(0.); double yh(0.);
      if ( !HistHelper::Tags::yRange(HistHelper::Names::extractYrange(fname),yl,yh) ) {
	printf("HistHelper::Draw::processing() - cannot extract y range\n");
	return;
      }     
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize(tl.GetTextSize()/1.75);
      tl.SetTextAlign(12);
      double x = gStyle->GetPadLeftMargin();    x += 0.050; 
      double y = 1.0-gStyle->GetPadTopMargin(); y -= 0.050;
      tl.DrawLatex(x,0.82+0.04,TString::Format("Anti-k_{t} R = 0.4 jets, %s %s",mn.c_str(),sn.c_str()).Data());
      tl.DrawLatex(x,0.74+0.04,TString::Format("%s pile-up suppression",pn.c_str()).Data());
      if ( yl == 0. ) { 
	tl.DrawLatex(x,0.78+0.04,TString::Format("|y_{jet}| < %.1f",yh).Data());
      } else {
	tl.DrawLatex(x,0.78+0.04,TString::Format("%.1f < |y_{jet}| < %.1f",yl,yh).Data());
      }
    }
    static void processing(const std::string& fname,double x,double y) {
      std::string sn; std::string pn; std::string mn;
      if ( !HistHelper::Names::extractAllNames(fname,sn,pn,mn) ) { 
	printf("HistHelper::Draw::processing() - cannot extract names\n");
	return;
      }
      double yl(0.); double yh(0.);
      if ( !HistHelper::Tags::yRange(HistHelper::Names::extractYrange(fname),yl,yh) ) {
	printf("HistHelper::Draw::processing() - cannot extract y range\n");
	return;
      }     
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize(tl.GetTextSize()/1.75);
      tl.SetTextAlign(12);
      tl.DrawLatex(x,y,     TString::Format("Anti-k_{t} R = 0.4 jets, %s %s",mn.c_str(),sn.c_str()).Data());
      tl.DrawLatex(x,y-0.08,TString::Format("%s pile-up suppression",pn.c_str()).Data());
      if ( yl == 0. ) { 
	tl.DrawLatex(x,y-0.04,TString::Format("|y_{jet}| < %.1f",yh).Data());
      } else {
	tl.DrawLatex(x,y-0.04,TString::Format("%.1f < |y_{jet}| < %.1f",yl,yh).Data());
      }
    }
    static void processing(const std::string& fname,double yl,double yh,double x,double y,int talign=12) {
      std::string sn; std::string pn; std::string mn;
      if ( !HistHelper::Names::extractAllNames(fname,sn,pn,mn) ) { 
	printf("HistHelper::Draw::processing() - cannot extract names\n");
	return;
      }
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize(tl.GetTextSize()/1.75);
      tl.SetTextAlign(talign);
      tl.DrawLatex(x,y,     TString::Format("Anti-k_{t} R = 0.4 jets, %s %s",mn.c_str(),sn.c_str()).Data());
      tl.DrawLatex(x,y-0.08,TString::Format("%s pile-up suppression",pn.c_str()).Data());
      if ( yl == 0. ) { 
	tl.DrawLatex(x,y-0.04,TString::Format("|y_{jet}| < %.1f",yh).Data());
      } else {
	tl.DrawLatex(x,y-0.04,TString::Format("%.1f < |y_{jet}| < %.1f",yl,yh).Data());
      }
    }
    static void processingNoSignal(const std::string& fname) {
      std::string sn; std::string pn; std::string mn;
      if ( !HistHelper::Names::extractAllNames(fname,sn,pn,mn) ) { 
	printf("HistHelper::Draw::processing() - cannot extract names\n");
	return;
      }
      double yl(0.); double yh(0.);
      if ( !HistHelper::Tags::yRange(HistHelper::Names::extractYrange(fname),yl,yh) ) {
	printf("HistHelper::Draw::processing() - cannot extract y range\n");
	return;
      }     
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize(tl.GetTextSize()/1.75);
      tl.SetTextAlign(12);
      double x = gStyle->GetPadLeftMargin();    x += 0.050; 
      double y = 1.0-gStyle->GetPadTopMargin(); y -= 0.050;
      tl.DrawLatex(x,0.82+0.04,TString::Format("Anti-k_{t} R = 0.4 jets, %s",mn.c_str()).Data());
      tl.DrawLatex(x,0.74+0.04,TString::Format("%s pile-up suppression",pn.c_str()).Data());
      if ( yl == 0. ) { 
	tl.DrawLatex(x,0.78+0.04,TString::Format("|y_{jet}| < %.1f",yh).Data());
      } else {
	tl.DrawLatex(x,0.78+0.04,TString::Format("%.1f < |y_{jet}| < %.1f",yl,yh).Data());
      }
    }
    template<class T>
    static void setCommonStyle(T* g,int color,int marker,double markerSize,int line=1) { 
      g->SetMarkerStyle(marker);
      g->SetMarkerColor(color);
      g->SetMarkerSize(markerSize);
      g->SetLineColor(color);
      g->SetLineStyle(line);
      g->SetLineWidth(2);
    }
  } // Draw
  namespace Data {
    static void writeGraph(TGraphErrors* pg,const std::string& fname) {
      std::ofstream f;
      f.open(fname,std::ofstream::out);
      double* xv = pg->GetX();
      double* yv = pg->GetY();
      double* xd = pg->GetEX();
      double* yd = pg->GetEY();
      //
      std::vector<std::vector<double> > store; store.reserve(pg->GetN());
      for ( int i(0); i<pg->GetN(); ++i ) { 
	std::vector<double> dt = { xv[i], yv[i], xd[i], yd[i] };
	if ( std::find(store.begin(),store.end(),dt) == store.end() ) { store.push_back(dt); }
      }
      for ( auto val : store ) { f << val.at(0) << " " << val.at(1) << " " << val.at(2) << " " << val.at(3) << std::endl; }
      // for ( int i(0); i<pg->GetN(); ++i ) { 
      // 	f << xv[i] << " " << yv[i] << " " << xd[i] << " " << yd[i] << std::endl;
      // }
      f.close();
    }
    static void writeGraph(TGraph* pg,const std::string& fname) {
      std::ofstream f;
      f.open(fname,std::ofstream::out);
      double* xv = pg->GetX();
      double* yv = pg->GetY();
      //
      std::vector<std::vector<double> > store; store.reserve(pg->GetN());
      for ( int i(0); i<pg->GetN(); ++i ) { 
	std::vector<double> dt = { xv[i], yv[i]	};
	if ( std::find(store.begin(),store.end(),dt) == store.end() ) { store.push_back(dt); }
      }
      for ( auto val : store ) { f << val.at(0) << " " << val.at(1) << std::endl; }
      // 	f << xv[i] << " " << yv[i] << std::endl;
      // }
      f.close();
    }
    static TGraph* fillTGraph(const std::vector<double>& xval,const std::vector<double>& yval)
    {
      double* xarr = new double[xval.size()];
      double* yarr = new double[yval.size()];
      for ( size_t i(0); i<xval.size(); ++i ) { xarr[i] = xval.at(i); yarr[i] = yval.at(i); } 
      TGraph* pg = new TGraph(xval.size(),xarr,yarr);
      delete[] xarr;
      delete[] yarr;
      return pg;
    }
    static TGraphErrors* fillTGraphErrors(const std::vector<double>& xval,const std::vector<double>& yval,const std::vector<double>& xerr,const std::vector<double>& yerr)
    {
      double* xarv = new double[xval.size()];
      double* yarv = new double[yval.size()];
      double* xare = new double[xerr.size()];
      double* yare = new double[yerr.size()];
      for ( size_t i(0); i<xval.size(); ++i ) { xarv[i] = xval.at(i); yarv[i] = yval.at(i); xare[i] = xerr.at(i); yare[i] = yerr.at(i); } 
      TGraphErrors* pg = new TGraphErrors(xval.size(),xarv,yarv,xare,yare);
      delete[] xarv;
      delete[] yarv;
      delete[] xare;
      delete[] yare;
      return pg;
    }
    static TGraphAsymmErrors* fillTGraphAsymmErrors(const std::vector<double>& xval,const std::vector<double>& yval,
						    const std::vector<double>& xerd,const std::vector<double>& xeru,
						    const std::vector<double>& yerd,const std::vector<double>& yeru)
    {
      double* xarv = new double[xval.size()];
      double* yarv = new double[yval.size()];
      double* xaru = new double[xeru.size()];
      double* xard = new double[xerd.size()];
      double* yaru = new double[yeru.size()];
      double* yard = new double[yerd.size()];
      for ( size_t i(0); i<xval.size(); ++i ) { 
	xarv[i] = xval.at(i); yarv[i] = yval.at(i); xaru[i] = xeru.at(i); yaru[i] = yeru.at(i); xard[i] = xerd.at(i); yard[i] = yerd.at(i); 
      } 
      TGraphAsymmErrors* pg = new TGraphAsymmErrors(xval.size(),xarv,yarv,xard,xaru,yard,yaru);
      delete[] xarv;
      delete[] yarv;
      delete[] xaru;
      delete[] yaru;
      delete[] xard;
      delete[] yard;
      return pg;
    }
    static std::vector<std::string> fileNames(const std::vector<std::string>& allFiles,const std::string& putag,const std::string& stag) {
      static std::string _sep = "_";
      std::vector<std::string> flist; flist.reserve(allFiles.size());
      for ( auto fn : allFiles ) {
	if ( fn.find(putag) != std::string::npos && fn.find(_sep+stag+_sep) != std::string::npos ) { 
	  flist.push_back(fn); 
	}
      }
      return flist;
    }
    static bool stringBreaker(const std::string& sstr,std::vector<std::string>& fstr,const std::string& sep=" ")
    {
      std::string dstr;
      for ( auto siter(sstr.begin()); siter!=sstr.end(); ++siter ) { 
	if ( *siter != *(sep.c_str()) ) { 
	  dstr += *siter; 
	} else {
	  if ( dstr.length() > 0 ) { fstr.push_back(dstr); }
	  dstr = std::string("");
	}
      }
      if ( dstr.length() > 0 ) { fstr.push_back(dstr); }
      // printf("\042%s\042 ---> ",sstr.c_str());
      // for ( auto fn : fstr ) { printf("\042%s\042 ",fn.c_str()); }
      // printf("\n");
      return !fstr.empty();
    }
    template<class V>
    static void fillHists(const V& variables,const std::vector<bool>& tags,const std::vector<int>& regions,std::vector<TH1D*>& h_sig,std::vector<TH1D*>& h_bck) {
      for ( size_t i(0); i<variables.size(); ++i ) { 
	int iy(regions.at(i)); double var(variables.at(i));
	if ( tags.at(i) ) { 
	  h_sig.at(iy)->Fill(var);
	} else { 
	  h_bck.at(iy)->Fill(var); 
	} // signal or background
      } // loop on all entries
    } 
    static bool compress(std::vector<double>& xave,std::vector<double>& xerl,std::vector<double>& xerh, 
			 std::vector<double>& yave,std::vector<double>& yerl,std::vector<double>& yerh,
			 bool useX=true,bool keepFirst=true) {
      static double _minVal = 1e-09;
      size_t ndat(xave.size());
      if ( xerl.size() < ndat ) { return false; }
      if ( xerh.size() < ndat ) { return false; }
      if ( yave.size() < ndat ) { return false; }
      if ( yerl.size() < ndat ) { return false; }
      if ( yerh.size() < ndat ) { return false; }
      // local work space
      std::vector<double> uave; uave.reserve(xave.size());
      std::vector<double> uerl; uave.reserve(xerl.size());
      std::vector<double> uerh; uave.reserve(xerh.size());
      std::vector<double> vave; vave.reserve(yave.size());
      std::vector<double> verl; verl.reserve(yerl.size());
      std::vector<double> verh; verh.reserve(yerh.size());
      // filter 
      if ( useX ) { 
	if ( keepFirst ) {
	  bool fval(false); 
	  for ( size_t i(0); i<ndat; ++i ) {
	    if ( !fval ) { 
	      fval = std::abs(xave.at(i)) > _minVal;
	      uave.push_back(xave.at(i)); uerl.push_back(xerl.at(i)); uerh.push_back(xerh.at(i)); 
	      vave.push_back(yave.at(i)); verl.push_back(yerl.at(i)); verh.push_back(yerh.at(i)); 
	    } else { 
	      if ( std::abs(xave.at(i)) > _minVal ) { 
		uave.push_back(xave.at(i)); uerl.push_back(xerl.at(i)); uerh.push_back(xerh.at(i)); 
		vave.push_back(yave.at(i)); verl.push_back(yerl.at(i)); verh.push_back(yerh.at(i)); 
	      }
	    }
	  }
	} else { 
	  for ( size_t i(0); i<ndat; ++i ) {
	    if ( std::abs(xave.at(i)) > _minVal ) { 
	      uave.push_back(xave.at(i)); uerl.push_back(xerl.at(i)); uerh.push_back(xerh.at(i)); 
	      vave.push_back(yave.at(i)); verl.push_back(yerl.at(i)); verh.push_back(yerh.at(i)); 
	    }
	  }
	}
      } else { 
	if ( keepFirst ) {
	  bool fval(false); 
	  for ( size_t i(0); i<ndat; ++i ) {
	    if ( !fval ) { 
	      fval = std::abs(yave.at(i)) > _minVal;
	      uave.push_back(xave.at(i)); uerl.push_back(xerl.at(i)); uerh.push_back(xerh.at(i)); 
	      vave.push_back(yave.at(i)); verl.push_back(yerl.at(i)); verh.push_back(yerh.at(i)); 
	    } else { 
	      if ( std::abs(yave.at(i)) > _minVal ) { 
		uave.push_back(xave.at(i)); uerl.push_back(xerl.at(i)); uerh.push_back(xerh.at(i)); 
		vave.push_back(yave.at(i)); verl.push_back(yerl.at(i)); verh.push_back(yerh.at(i)); 
	      }
	    }
	  }
	} else { 
	  for ( size_t i(0); i<ndat; ++i ) {
	    if ( std::abs(yave.at(i)) > _minVal ) { 
	      uave.push_back(xave.at(i)); uerl.push_back(xerl.at(i)); uerh.push_back(xerh.at(i)); 
	      vave.push_back(yave.at(i)); verl.push_back(yerl.at(i)); verh.push_back(yerh.at(i)); 
	    }
	  }
	}
      }
      // copy results
      xave = uave; xerl = uerl; xerh = uerh; 
      yave = vave; yerl = yerl; yerh = yerh; 
      return !xave.empty(); 
    }
    static bool findValueRange(TGraph* g,double threshold,int& ileft,int& iright,const std::string& axis="x",const std::string& mode="fromLeft") {
      static std::vector<std::string> _mtags = { "fromLeft", "fromRight", "both" };
      // check directives
      if ( axis != "x" && axis != "y" )                                  { return false; }
      if ( std::find(_mtags.begin(),_mtags.end(),mode) == _mtags.end() ) { return false; }
      bool fromLeft( mode=="fromLeft" ||mode=="both");
      bool fromRight(mode=="fromRight"||mode=="both");
      // allocate data
      int     np = g->GetN();
      double* pv = axis=="x" ? g->GetX() : g->GetY();
      // find range
      ileft = 0; iright = np-1; 
      if ( fromLeft ) {
	while ( ileft < np && pv[ileft] < threshold ) { ++ileft; }
      }
      if ( fromRight ) { 
	while ( iright >= 0 && pv[iright] < threshold ) { --iright; }
      }
      // extract data
      if ( ileft < 0 || ileft >= np || iright <= ileft || iright >= np ) { return false; }
      return true; 
    }
    static TGraph* compressGraph(TGraph* g, double threshold=1e-05,const std::string& axis="x",const std::string& mode="fromLeft") {
      // find value range
      int ileft(0); int iright(0);
      if ( !findValueRange(g,threshold,ileft,iright,axis,mode) ) { return g; }
      // prepare workspace
      TGraph* b  = new TGraph(*g);
      double* px = g->GetX();
      double* py = g->GetY();
      // extract data
      b->Set(iright-ileft+1); int l(0);
      for ( int k(ileft); k<=iright; ++k, ++l ) { b->SetPoint(l,px[k],py[k]); }
      // return results
      return b;
    } 
    static TGraph* compressGraph(TGraphErrors* g, double threshold=1e-05,const std::string& axis="x",const std::string& mode="fromLeft") {
      // find value range
      int ileft(0); int iright(0);
      if ( !findValueRange((TGraph*)g,threshold,ileft,iright,axis,mode) ) { return g; }
      // prepare workspace
      TGraphErrors* b = new TGraphErrors(*g);
      double* pxv = g->GetX();
      double* pyv = g->GetY();
      double* pxd = g->GetEX();
      double* pyd = g->GetEY();
      // extract data
      b->Set(iright-ileft+1); int l(0);
      for ( int k(ileft); k<=iright; ++k, ++l ) { b->SetPoint(l,pxv[k],pyv[k]); b->SetPointError(l,pxd[k],pyd[k]); }
      // return results
      return (TGraph*)b;
    } 
    static TGraph* compressGraph(TGraphAsymmErrors* g, double threshold=1e-05,const std::string& axis="x",const std::string& mode="fromLeft") {
      // find value range
      int ileft(0); int iright(0);
      if ( !findValueRange((TGraph*)g,threshold,ileft,iright,axis,mode) ) { return g; }
      // prepare workspace
      TGraphAsymmErrors* b = new TGraphAsymmErrors(*g);
      double* pxv = g->GetX();
      double* pyv = g->GetY();
      double* pxl = g->GetEXlow();
      double* pxh = g->GetEXhigh();
      double* pyl = g->GetEYlow();
      double* pyh = g->GetEYhigh();
      // extract data
      b->Set(iright-ileft+1); int l(0);
      for ( int k(ileft); k<=iright; ++k, ++l ) {  
	b->SetPoint(l,pxv[k],pyv[k]); b->SetPointEXlow(l,pxl[k]);  b->SetPointEXhigh(l,pxh[k]);  b->SetPointEYlow(l,pyl[k]);  b->SetPointEYhigh(l,pyh[k]); 
      }
      // return new graph
      return (TGraph*)b;
    } 
  } // Data 
  namespace Hist {
    template<class H>
    static void realign(H* h,bool bothSides=true,int axis=1,double threshold=0.) {
      if ( axis < 1 || axis > 3 ) { 
	printf("HistHelper::Hist::realign(...) - invalid axis number %i, no alignment\n",axis);
	return;
      }
      if ( bothSides ) {
	int ii(h->FindFirstBinAbove(threshold,axis));
	int ij(h->FindLastBinAbove(threshold,axis));
	switch ( axis ) {
	case 1:
	  h->GetXaxis()->SetRangeUser(h->GetXaxis()->GetBinLowEdge(ii),h->GetXaxis()->GetBinUpEdge(ij));
	  break;
	case 2: 
	  h->GetYaxis()->SetRangeUser(h->GetYaxis()->GetBinLowEdge(ii),h->GetYaxis()->GetBinUpEdge(ij));
	  break;
	case 3:
	  h->GetZaxis()->SetRangeUser(h->GetZaxis()->GetBinLowEdge(ii),h->GetZaxis()->GetBinUpEdge(ij));
	  break;
	default:
	  break;
	}
      } else {
	int ij(h->FindLastBinAbove(threshold,axis));
	switch ( axis ) {
	case 1:
	  h->GetXaxis()->SetRangeUser(h->GetXaxis()->GetBinLowEdge(1),h->GetXaxis()->GetBinUpEdge(ij));
	  break;
	case 2: 
	  h->GetYaxis()->SetRangeUser(h->GetYaxis()->GetBinLowEdge(1),h->GetYaxis()->GetBinUpEdge(ij));
	  break;
	case 3:
	  h->GetZaxis()->SetRangeUser(h->GetZaxis()->GetBinLowEdge(1),h->GetZaxis()->GetBinUpEdge(ij));
	  break;
	default:
	  break;
	} // axis switch
      } // high end alignment only
    } // realign(...)
    static bool stats(TH1D* h,double& ave,double& dup,double& ddn,const std::string& smode="error",const std::string& amode="mean") {
      // get mean or median 
      if ( amode == "mean" ) { 
	ave = h->GetMean(); 
      } else if ( amode == "median" ) {
	double prob(0.5);
	h->ComputeIntegral();
	h->GetQuantiles(1,&ave,&prob);
      } else {
	return false; 
      }
      if ( h->GetEntries() < 2. ) { dup = 0.; ddn = 0.; return true; }
      // calculate error or rms
      bool useError = smode == "error";
      // calculate asymmetric errors 
      dup = 0.; ddn = 0.;
      double wl(0.); double wh(0.);
      for ( int ib(1); ib<=h->GetNbinsX(); ++ib ) {
	double x(h->GetBinCenter(ib)); double y(h->GetBinContent(ib));
	double d(x-ave);
	if ( d < 0. ) {
	  ddn += y*d*d; wl += y;
	} else {
	  dup += y*d*d; wh += y;
	}
      } // accumulate data for mean and spread
      // calculate spread 
      if ( wl != 0. ) { ddn = useError ? std::sqrt(std::abs(ddn))/std::abs(wl) : std::sqrt(std::abs(ddn)/std::abs(wl)); }
      if ( wh != 0. ) { dup = useError ? std::sqrt(std::abs(dup))/std::abs(wh) : std::sqrt(std::abs(dup)/std::abs(wh)); }
      //
      return true; 
    }
    //
    template<class H>
    static int histBins(H* h) {
      int nb(h->GetNbinsX());
      if ( h->GetNbinsY() > 0 ) { 
	nb *= h->GetNbinsY(); 
	if ( h->GetNbinsZ() > 0 ) { nb *= h->GetNbinsZ(); } 
      }
      return nb;
    }
    //
    template<class H>
    static H* histRatio(H* h,H* g,double wh=1.,double wg=1. /*,const std::string& opts=""*/) {
      int nbh(histBins(h)); int nbg(histBins(g));
      if ( nbh != nbg ) { return (H*)0; }
      H* hh = new H(*h);
      for ( int i(1); i<=nbg; ++i ) { 
	double y(g->GetBinContent(i)); double dy(g->GetBinError(i));
	if ( y != 0. ) { 
	  double v(h->GetBinContent(i)); double dv(h->GetBinError(i));
	  double f(v/y); 
	  double df(std::sqrt(std::pow(dv/v,2)+std::pow(dy/y,2))*f);
	  hh->SetBinContent(i,f); hh->SetBinError(i,df); 
	} else { 
	  hh->SetBinContent(i,0.); hh->SetBinError(i,0.);
	}
      } 
      //      h->Divide(h,g,wh,wg,opts.c_str()); return h;
      return hh;
    } 
    // 
    template<class H>
    static H* histSum(H* h,H* g,double wh=1.,double wg=1.) {
      int nbh(histBins(h)); int nbg(histBins(g));
      if ( nbh != nbg ) { return (H*)0; }
      H* hh = new H(*h); 
      for ( int i(1); i<=nbh; ++i ) { 
	double y0(h->GetBinContent(i)); double dy0(h->GetBinError(i));
	double y1(g->GetBinContent(i)); double dy1(g->GetBinError(i));
	double yn(y0+y1); double dyn(std::sqrt(dy0*dy0+dy1*dy1));
	hh->SetBinContent(i,yn); hh->SetBinError(i,dyn);
      }
      return hh;
    }
    //
    template<class H>
    static bool histEntriesRange(H* h,double& ymin,double& ymax) {
      int nb(histBins(h));
      ymin = std::numeric_limits<double>::max();
      ymax = std::numeric_limits<double>::min();
      for ( int i(1); i<=nb; ++i ) { 
	double y(h->GetBinContent(i)); 
	if ( y < ymin ) { ymin = y; }
	if ( y > ymax ) { ymax = y; }
      }
      return ymin < ymax; 
    }
    //
    static std::vector<TH1D*> extract(TH2D* d,const std::string& name,int alongAxis=1) {
      std::vector<TH1D*> hists;
      // projection along x axis
      if ( alongAxis == 1 ) { 
	//     printf("HistUtils::extract() loop %i projections\n",d->GetNbinsY());
	for ( int ib(1); ib<=d->GetNbinsY(); ++ib ) {
	  hists.push_back(d->ProjectionX(TString::Format("%s_%04i",name.c_str(),ib).Data(),ib,ib,"e"));
	}
	return hists;
      }
      // distribution along y axis
      if ( alongAxis == 2 ) {
	//      printf("HistUtils::extract() loop %i projections\n",d->GetNbinsX());
	for ( int ib(1); ib<=d->GetNbinsX(); ++ib ) {
	  hists.push_back(d->ProjectionY(TString::Format("%s_%04i",name.c_str(),ib).Data(),ib,ib,"e"));
	}
	return hists;
      }
      //
      return hists;
    }
    //
    static TGraphAsymmErrors* graph(TH2D* d,const std::string& name,int alongAxis,const std::string& opt="mean")
    {
      printf("graph(...) enter with TH2D* = %p, name = \042%s\042, axis = %i, Stats = %s\n",(void*)d,name.c_str(),alongAxis,opt.c_str());
      std::vector<TH1D*> hists = extract(d,name,alongAxis);
      printf("graph(...) extracted %i projections...\n",(int)hists.size());
      TGraphAsymmErrors* gptr = 0;
      if ( hists.empty() ) { return gptr; }
      //
      int nb(0);
      std::vector<double> x;
      std::vector<double> dxl;
      std::vector<double> dxh;
      std::vector<double> y;
      std::vector<double> dyl;
      std::vector<double> dyh;
      switch ( alongAxis )
	{
	case 1:
	  nb  = d->GetNbinsY();
	  x   = std::vector<double>(nb,0.);
	  dxl = std::vector<double>(nb,0.);
	  dxh = std::vector<double>(nb,0.);
	  y   = std::vector<double>(nb,0.);
	  dyl = std::vector<double>(nb,0.);
	  dyh = std::vector<double>(nb,0.);
	  for ( int i(1); i<=nb; ++i ) {
	    stats(hists.at(i-1),y[i-1],dyl[i-1],dyh[i-1],opt);
	    x[i-1]   = d->GetYaxis()->GetBinCenter(i);
	    dxl[i-1] = d->GetYaxis()->GetBinWidth(i)/2.;
	    dxh[i-1] = dxl[i-1];
	  }
	  gptr = new TGraphAsymmErrors(nb,&x[0],&y[0],&dxl[0],&dxh[0],&dyl[0],&dyh[0]);
	  break;
	case 2:
	  nb  = d->GetNbinsX();
	  x   = std::vector<double>(nb,0.);
	  dxl = std::vector<double>(nb,0.);
	  dxh = std::vector<double>(nb,0.);
	  y   = std::vector<double>(nb,0.);
	  dyl = std::vector<double>(nb,0.);
	  dyh = std::vector<double>(nb,0.);
	  for ( int i(1); i<=nb; ++i ) {
	    stats(hists.at(i-1),y[i-1],dyl[i-1],dyh[i-1],opt);
	    x[i-1]   = d->GetXaxis()->GetBinCenter(i);
	    dxl[i-1] = d->GetXaxis()->GetBinWidth(i)/2.;
	    dxh[i-1] = dxl[i-1];
	  }
	  gptr = new TGraphAsymmErrors(nb,&x[0],&y[0],&dxl[0],&dxh[0],&dyl[0],&dyh[0]);
	  break;
	default:
	  break;
	}
      return gptr;
    }
  } // Hist
} // HistHelper

#endif
