
#include "HistHelper.h"
#include "plotEff.C"

#include <string>
#include <vector>

#include <TFile.h>
#include <TString.h>

void plotAllEff(const std::string& fname="LHspectra.root",int mode=0) {

  std::vector<std::string> ystr = { "00_15", "15_25", "25_32", "32_49" };

  printf("plotAllEff - found %zu y ranges\n",ystr.size());

  // loop on all ranges
  
  for ( auto ys : ystr ) { 
    TFile* f = new TFile(TString::Format("EffFits_%s.root",ys.c_str()).Data(),"RECREATE"); 
    plotEff(ys,fname,mode);
    f->Write();
    f->Close();
  } 
}
