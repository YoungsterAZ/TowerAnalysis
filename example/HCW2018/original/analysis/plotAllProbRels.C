
#include "HistHelper.h"
#include "plotProbRels.C"

#include <string>
#include <vector>
#include <map>

#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>

void plotAllProbRels() {

#undef TUPLEFILENAMES_ICC
#include "TupleFileNames.icc"
#undef REGIONNAMES_ICC
#include "RegionNames.icc"
#undef VARNAMES_ICC
#include "VarNames.icc"

printf("plotAllProbRels - found %zu files\n",tupleFileNames.size());
 
  // TFile* f = new TFile("debug.root","RECREATE");

  for ( auto fn : tupleFileNames ) { 
    std::string sname(tupleFileSignals[fn]);
    std::string dname(tupleFileDirs[fn]);
    std::string pname(tupleFileProc[fn]);
    for ( auto vn : varName ) { 
      for ( auto rn : regionNames ) {
	std::string cnm(TString::Format("Prob_vs_%s_%s_%s_%s",pname.c_str(),sname.c_str(),rn.c_str(),vn.c_str()).Data());
	TCanvas* a = new TCanvas(cnm.c_str(),cnm.c_str());
	a->SetLogz();
	if ( plotProbRels(vn,rn,fn) ) {
	  std::string ofn(TString::Format("%s/prob_vs_%s_%s_%s",dname.c_str(),sname.c_str(),vn.c_str(),rn.c_str()).Data());
	  a->Print((ofn+".pdf").c_str());
	  a->Print((ofn+".png").c_str());
	} else {
	  delete a;
	}
      } // regions
    } // variables
  } // input file names

  // f->Write(); f->Close();
}
