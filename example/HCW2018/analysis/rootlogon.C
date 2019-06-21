
#include <cstdio>
#include <string>
#include <sstream>

#include <TString.h>

#include "AtlasStyle.C"
//#include "AtlasUtils.C"

void rootlogon()
{
  std::string apath(gSystem->Getenv("ALRB_BOOST_ROOT"));
  if ( !apath.empty() && apath != "" ) { 
    //    size_t p(apath.find("boost-"));
    //    if ( p == std::string::npos ) { 
    TString ts = TString::Format("%s/include/boost-1_62",apath.c_str());
    gSystem->AddIncludePath(TString::Format("-I\042%s\042",ts.Data()).Data()); 
    printf("\n## [rootlogon] added boost include path \042%s\042\n",ts.Data());
    // } else {
    //   TString ts = TString::Format("-I\042%s/include/%s\042",apath.c_str(),apath.substr(p,10).c_str());
    //   gSystem->AddIncludePath(ts.Data()); 
    //   printf("\n## [rootlogon] added boost include path \042%s\042\n",ts.Data());
    // }
  }
  printf("\n## [rootlogon] %s\n\n",SetAtlasStyle());
}

