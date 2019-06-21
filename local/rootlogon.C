
#include <cstdio>
#include <string>
#include <sstream>

#include <TString.h>

#include "AtlasStyle.C"

void rootlogon()
{

  printf("\n####################################################\n");
  printf("## Welcome to the AZ self-configured root session ##\n");
  printf("####################################################\n\n");

  printf("## [rootlogon] Preloading ATLAS style macros from path ");

  std::string apath(gSystem->Getenv("ROOT_STYLEPATH"));
  bool fromEnvironment(true);
  if ( apath.empty() || apath == "" ) { apath = "~lochp/data2/CaloSignals/local"; fromEnvironment = false; }
  gSystem->AddIncludePath(TString::Format("-I\042%s\042",apath.c_str()).Data());
  if ( fromEnvironment ) { 
    printf("\042%s\042 [from environment]\n",apath.c_str());
  } else { 
    printf("\042%s\042 [default]\n",apath.c_str());
  }

  printf("## [rootlogon] %s\n",SetAtlasStyle());

  ///////////////////
  // Local include //
  ///////////////////

  printf("## [rootlogon] Preloading application include path ");

  fromEnvironment = true;
  std::string upath(gSystem->Getenv("ROOT_INCLUDEPATH"));
  if ( upath.empty() || upath == "" ) { upath = "~lochp/data2/CaloSignals/local/include"; fromEnvironment = false; }
  gSystem->AddIncludePath(TString::Format("-I\042%s\042",upath.c_str()).Data());
  if ( fromEnvironment ) {
    printf("\042%s\042 [from environment]\n",upath.c_str());
  } else { 
    printf("\042%s\042 [default]\n",upath.c_str());
  }

  TString tllib((upath.substr(0,upath.find_last_of("/"))+"/tools").c_str());
  printf("## [rootlogon] tool library path \042%s\042\n",tllib.Data());
  tllib += TString("/libTool.so");

  /////////////
  // FastJet //
  /////////////



  std::stringstream sstr;
  sstr << gSystem->GetFromPipe("fastjet-config --prefix");
  std::string fjpath(sstr.str());
  //  std::string fjpath("/afs/cern.ch/user/l/lochp/fastjet"); 

  std::stringstream vstr;
  vstr << gSystem->GetFromPipe("fastjet-config --version");
  std::string fjvers(vstr.str());

  TString fjlib(fjpath.c_str());
  TString fjinc(fjpath.c_str());
  TString fjtlb(fjpath.c_str());
  TString fcslb(fjpath.c_str());
  TString fsklb(fjpath.c_str());

  TString fjsft(".");

  fjinc = fjinc+TString("/include");
  fjlib += TString("/lib/libfastjet.so");

  fjtlb += TString("/lib/libfastjettools.so");

  fcslb += TString("/lib/libConstituentSubtractor.a");
  fsklb += TString("/lib/libSoftKiller.a");


  TString mrk;
  for ( int i(0); i<fjtlb.Length(); ++i ) { mrk += "-"; }

  printf("## [rootlogon] --------------------------%s FastJet v%s configuration (begin)\n",mrk.Data(),fjvers.c_str());
  printf("## [rootlogon] FastJet path           \042%s\042\n",fjpath.c_str());
  printf("## [rootlogon] FastJet include        \042%s\042\n",fjinc.Data());
  printf("## [rootlogon] FastJet lib            \042%s\042\n",fjlib.Data());

  printf("## [rootlogon] FastJet tool lib       \042%s\042\n",fjtlb.Data());

  gSystem->AddIncludePath(TString::Format("-I\042%s\042",fjinc.Data()).Data());
  // fjinc += TString("/fastjet");
  // gSystem->AddIncludePath(fjinc.Data());
  fjinc += TString("/contrib");
  gSystem->AddIncludePath(TString::Format("-I\042%s\042",fjinc.Data()).Data());
  printf("## [rootlogon] FastJet contrib path   \042%s\042\n",fjinc.Data());
  printf("## [rootlogon] --------------------------%s FastJet v%s configuration (end)\n\n",mrk.Data(),fjvers.c_str());

  gROOT->LoadMacro(fjlib.Data());
  gROOT->LoadMacro(fjtlb.Data());
  gROOT->LoadMacro(tllib.Data());
  //  gROOT->LoadMacro(fcslb.Data());
  //  gROOT->LoadMacro(fsklb.Data());
}
