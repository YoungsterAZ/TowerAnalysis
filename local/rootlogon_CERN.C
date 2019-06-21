
#include <cstdio>
#include <string>

#include <TString.h>

void rootlogon()
{

  /////////////
  // FastJet //
  /////////////

  std::string fjpath("/afs/cern.ch/user/l/lochp/fastjet"); 
  TString fjlib(fjpath.c_str());
  TString fjinc(fjpath.c_str());
  TString fjtlb(fjpath.c_str());

  TString fjsft(".");

  fjinc = fjinc+TString("/include");
  fjlib += TString("/lib/libfastjet.so");
  fjtlb += TString("/lib/libfastjettools.so");
  printf("rootlogon - FastJet include        \042%s\042\n",fjinc.Data());
  printf("rootlogon - FastJet path           \042%s\042\n",fjpath.c_str());
  printf("rootlogon - FastJet lib            \042%s\042\n",fjlib.Data());
  printf("rootlogon - FastJet tool lib       \042%s\042\n",fjtlb.Data());

  gSystem->AddIncludePath(TString::Format("-I\042%s\042",fjinc.Data()).Data());
  // fjinc += TString("/fastjet");
  // gSystem->AddIncludePath(fjinc.Data());
  fjinc += TString("/contrib");
  gSystem->AddIncludePath(TString::Format("-I\042%s\042",fjinc.Data()).Data());

  gROOT->LoadMacro(fjlib.Data());
  gROOT->LoadMacro(fjtlb.Data());
}
