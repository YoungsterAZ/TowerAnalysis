
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

  printf("\n");
}

