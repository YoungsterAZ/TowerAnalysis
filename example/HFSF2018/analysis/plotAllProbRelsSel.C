
#include "HistHelper.h"
#include "plotProbRelsSel.C"

#include <string>
#include <vector>
#include <map>
#include <cmath>

#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>

void plotAllProbRelsSel(const std::string& varn="",bool scatter=false,int nccut=-1) {

#undef TUPLEFILENAMES_ICC
#include "TupleFileNames.icc"
#undef REGIONNAMES_ICC
#include "RegionNames.icc"
#undef VARNAMES_ICC
#include "VarNames.icc"

printf("plotAllProbRelsSel - found %zu files\n",tupleFileNames.size());
 
// TFile* f = new TFile("debug.root","RECREATE");

  for ( auto fn : tupleFileNames ) { 
    std::string sname(tupleFileSignals[fn]);
    std::string dname(tupleFileDirs[fn]);
    std::string pname(tupleFileProc[fn]);
    if ( varn == "" ) { 
      for ( auto vn : varName ) { 
	for ( auto rn : regionNames ) {
	  std::string cnm(TString::Format("Prob_vs_%s_%s_%s_%s",pname.c_str(),sname.c_str(),rn.c_str(),vn.c_str()).Data());
	  TCanvas* a = new TCanvas(cnm.c_str(),cnm.c_str());
	  a->SetLogz();
	  if ( plotProbRelsSel(vn,rn,fn,scatter,nccut) ) {
	    if ( !scatter ) { 
	      std::string ofn(TString::Format("%s/prob_vs_%s_%s_%s_sel%02i_dist",dname.c_str(),sname.c_str(),vn.c_str(),rn.c_str(),std::max(nccut,0)).Data());
	      a->Print((ofn+".pdf").c_str());
	      a->Print((ofn+".png").c_str());
	    } else {
	      std::string ofn(TString::Format("%s/prob_vs_%s_%s_%s_sel%02i_scatter",dname.c_str(),sname.c_str(),vn.c_str(),rn.c_str(),std::max(nccut,0)).Data());
	      a->Print((ofn+".pdf").c_str());
	      a->Print((ofn+".png").c_str());
	    }
	  } else {
	    delete a;
	  } // plot ok
	} // regions
      } // variables
    } else {
      std::string vn(varn);
      for ( auto rn : regionNames ) {
	std::string cnm(TString::Format("Prob_vs_%s_%s_%s_%s",pname.c_str(),sname.c_str(),rn.c_str(),vn.c_str()).Data());
	TCanvas* a = new TCanvas(cnm.c_str(),cnm.c_str());
	a->SetLogz();
	if ( plotProbRelsSel(vn,rn,fn,scatter,nccut) ) {
	  if ( !scatter ) { 
	    std::string ofn(TString::Format("%s/prob_vs_%s_%s_%s_sel%02i_dist",dname.c_str(),sname.c_str(),vn.c_str(),rn.c_str(),std::max(nccut,0)).Data());
	    a->Print((ofn+".pdf").c_str());
	    a->Print((ofn+".png").c_str());
	  } else {
	    std::string ofn(TString::Format("%s/prob_vs_%s_%s_%s_sel%02i_scatter",dname.c_str(),sname.c_str(),vn.c_str(),rn.c_str(),std::max(nccut,0)).Data());
	    a->Print((ofn+".pdf").c_str());
	    a->Print((ofn+".png").c_str());
	  }
	} else {
	  delete a;
	} // plot ok
      } // regions
    } // all or one variable
  } // input file names

  // f->Write(); f->Close();
}
