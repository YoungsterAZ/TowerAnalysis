
#include "HistUtils.h"
#include "AnalysisHelperCore.h"

#include <string>

void testAHCore(const std::string& h="test",const std::string& f="test.root")
{
  TH1D* hptr = AH::book1D<TH1D>(h.c_str(),h.c_str(),100,0.,1.);
  hptr->Fill(0.5);
  int nh = AH::writeHists(f.c_str());
}
