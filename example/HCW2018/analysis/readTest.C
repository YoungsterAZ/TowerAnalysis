
#include "ReadMLFile.h"

#include <TCanvas.h>
#include <TH1D.h>

#include <string>

void readTest()
{
  //  gROOT->LoadMacro("ReadMLFile.h++");

  std::string fname("ML_Results/JAPU+INCL/vbf2600_topoclus_00_15.csv");
  ReadMLFile indata(fname);
  printf("readTest - read %zu records\n",indata.size());

  auto fiter = indata.begin();
  size_t i(1);
  for ( ; fiter != indata.end(); ++fiter ) { 
    printf("readTest - record %5zu (pt,m,width,ptd) = (%8.3f GeV,%8.3f GeV,%5.3f,%5.3f) nc = %3i (psig,pbck) = (%10.6f,%10.6f)%% tag = %i\n",
    	   i++,
    	   indata.pt(fiter), indata.m(fiter), indata.width(fiter), indata.ptd(fiter),
    	   indata.nc(fiter),
    	   indata.psig(fiter), indata.pbck(fiter), 
    	   indata.tag(fiter)
    	   );
  }
}
