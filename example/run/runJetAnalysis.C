//////////////////////
// Run Jet Analysis //
//////////////////////

#include <TChain.h>

#include <string>

void runJetAnalysis(const std::string& cfgFile="")
{
  // create chain
  TChain* indata = new TChain("CollectionTree");
  // create analysis object
  JetAnalysisExample* ja = new JetAnalysisExample(indata,cfgFile);
  // run analysis
  ja->Loop();
}
