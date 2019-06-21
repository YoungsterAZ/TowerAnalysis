

#include <TString.h>
#include <TTree.h>
#include <TFile.h>

#include <vector>
#include <string>
#include <cstdio>

#include "HistHelper.h"
#include "ReadMLFile.h"

#include <boost/tuple/tuple.hpp>

void writeTuple(const std::string& putag,const std::string& stag) {

  typedef boost::tuples::tuple<int,double,double,double,double,double> data_t;

  std::vector<std::string> ystr = { "00_15", "15_25", "25_32", "32_49" };
  std::vector<double>      ylow = { 0.,      1.5,     2.5,      3.2    };
  std::vector<double>      yhgh = { 1.5,     2.5,     3.2,      4.5    };

  std::map<std::string,data_t>  ymap;             
  for ( size_t i(0); i<ystr.size(); ++i ) { 
    double yave((ylow.at(i)+yhgh.at(i))/2.);
    double ydlf(yave-ylow.at(i));
    double ydrt(yhgh.at(i)-yave);
    ymap[ystr.at(i)] = data_t((int)i,ylow.at(i),yhgh.at(i),yave,ydlf,ydrt);
  }

#include "FileNames.icc"

  // find all relevant output files
  std::vector<std::string> fn = HistHelper::Data::fileNames(fnames,putag,stag);
  printf("writeTuple - looking for pile-up tag <%s> and signal tag <%s>\n",putag.c_str(),stag.c_str());
  size_t ifile(1);
  for ( auto nn : fn ) { printf("writeTuple - [%02zu] \042%s\042\n",ifile++,nn.c_str()); }

  if ( fn.empty() ) { return; }

  std::vector<int>    yJetReg;
  std::vector<double> yJetMin;
  std::vector<double> yJetMax;
  std::vector<double> yJetAve;

  std::vector<double> ptJet;
  std::vector<double> mJet;
  std::vector<double> wJet;
  std::vector<double> ptdJet;
  std::vector<int>    ncJet;
  std::vector<int>    tagJet;
  std::vector<double> psigJet;
  std::vector<double> pbckJet;

  std::vector<bool>   isSigJet;

  std::string ofname(TString::Format("tuple/%s_%s_tuple.root",putag.c_str(),stag.c_str()).Data());
  TFile* ofile = new TFile(ofname.c_str(),"RECREATE");

  TTree* tt = new TTree("TagJets","TagJets");

  tt->Branch<std::vector<int>    >("yJetReg", &yJetReg );
  tt->Branch<std::vector<double> >("yJetMin", &yJetMin );
  tt->Branch<std::vector<double> >("yJetMax", &yJetMax );
  tt->Branch<std::vector<double> >("yJetAve", &yJetAve );

  tt->Branch<std::vector<double> >("ptJet",   &ptJet   );
  tt->Branch<std::vector<double> >("mJet",    &mJet    );
  tt->Branch<std::vector<double> >("wJet",    &wJet    );
  tt->Branch<std::vector<double> >("ptdJet",  &ptdJet  );
  tt->Branch<std::vector<double> >("psigJet", &psigJet );
  tt->Branch<std::vector<double> >("pbckJet", &pbckJet );
  tt->Branch<std::vector<int>    >("ncJet",   &ncJet   );
  tt->Branch<std::vector<int>    >("tagJet",  &tagJet  );
  tt->Branch<std::vector<bool>   >("isSigJet",&isSigJet);

  for ( auto nn : fn ) { 
    // read file data
    ReadMLFile indata(nn);
    if ( indata.empty() ) { 
      printf("writeTuple - file \042%s\042 cannot be found, skip\n",nn.c_str());
      continue;
    }
    // get rapidity range
    auto fy = ystr.begin();
    while ( fy < ystr.end() && nn.find(*fy) == std::string::npos ) { ++fy; }
    if ( fy == ystr.end() ) { continue; }
    // 
    int    yi(ymap[*fy].get<0>());
    double ya(ymap[*fy].get<3>());
    double yl(ymap[*fy].get<4>());
    double yh(ymap[*fy].get<5>());
    //
    auto fiter = indata.begin();
    for ( ; fiter != indata.end(); ++fiter ) { 
      yJetReg.push_back(yi); 
      yJetMin.push_back(yl); 
      yJetMax.push_back(yh); 
      yJetAve.push_back(ya);
      ptJet.push_back(indata.pt(fiter));
      mJet.push_back(indata.m(fiter));
      wJet.push_back(indata.width(fiter));
      ptdJet.push_back(indata.ptd(fiter));
      ncJet.push_back(indata.nc(fiter));
      tagJet.push_back(indata.tag(fiter));
      psigJet.push_back(indata.psig(fiter));
      pbckJet.push_back(indata.pbck(fiter));
      isSigJet.push_back(indata.isSignal(fiter));
    }
    //
  }

  tt->Fill(); tt->Write();
  ofile->Close();
}
