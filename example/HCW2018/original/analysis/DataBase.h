// -*- c++ -*-
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <boost/tuple/tuple.hpp>

class DataBase
{
private:
  
  typedef std::string                                                         rkey_t;
  typedef boost::tuples::tuple<double,double,double,double,double,double,int> bin_t;
  typedef std::map<rkey_t,bin_t>                                              binmap_t;
  typedef std::pair<std::string,std::string>                                  key_t; 
  typedef std::map<key_t,binmap_t>                                            store_t;

  store_t _store;

public:

  typedef store_t::const_iterator  const_iterator;
  typedef binmap_t::const_iterator table_iterator;

  DataBase(const std::string& stype="signal",const std::string& baseDir="ML_Results") { fill(stype,baseDir); }
  //  static DataBase* instance(const std::string& baseDir="ML_Results") { if ( _instance == 0 ) { _instance == new DataBase(baseDir); } return _instance; }

  void fill(const std::string& stype="signal",const std::string& baseDir="ML_Results") {
    static std::vector<std::string> _putags = { "JAPU+INCL", "JAPU+TIME", "CSPU+INCL", "CSPU+TIME", "CSSK+INCL", "CSSK+TIME" };
    static std::vector<std::string> _signal = { "topoclus",  "topotowf",  "topotow"                                          };
    static std::vector<std::string> _stypes = { "signal",    "backgd"                                                        };
    static char _buffer[1024];
    //
    if ( std::find(_stypes.begin(),_stypes.end(),stype) == _stypes.end() ) {
      printf("DataBase::fill(\042%s\042,\042%s\042) - invalid signal type \042%s\042 requested, empty DB!\n",stype.c_str(),baseDir.c_str(),stype.c_str());
      printf("DataBase::fill(\042%s\042,\042%s\042) - valid types are ",stype.c_str(),baseDir.c_str());
      for ( auto st : _stypes ) { printf("\042%s\042 ",st.c_str()); }
      printf("\n");
      return;
    }
    //
    for ( auto pu : _putags ) { 
      for ( auto sn : _signal ) {
	// alllocate database record
	binmap_t bd;
	// open file
	sprintf(_buffer,"%s/%s/vbf_%s_test_sample_rapbins_%s.dat",baseDir.c_str(),pu.c_str(),sn.c_str(),stype.c_str());
	std::string fname(_buffer);
	std::ifstream istr(fname.c_str());
	if ( istr.fail() ) {
	  printf("DataBase::fill() file \042%s\042 does not exist, skipped\n",fname.c_str());
	  continue;
	}
	// read file
	bool readData(false);
	while ( istr.good() ) {
	  readData = true;
	  std::string dstr;
	  if ( std::getline(istr,dstr) ) { 
	    //	    std::istringstream sstr(dstr);
	    std::vector<std::string> fields;
	    std::string tstr;
	    double yblow(0.); double ybhigh(0.); double yave(0.); double yleft(0.); double yright(0.); double yrms(0.); int nj(0);
	    if ( HistHelper::Data::stringBreaker(dstr,fields) )  {
	      for ( size_t i(0); i<fields.size(); ++i ) { 
		switch (i) {
		case 0:
		  yblow  = atof(fields.at(i).c_str()); tstr  = fields.at(i) + "_"; break;
		case 1:
		  ybhigh = atof(fields.at(i).c_str()); tstr += fields.at(i);       break;
		case 2:
		  yave   = atof(fields.at(i).c_str());                             break;
		case 3:
		  yleft  = atof(fields.at(i).c_str());                             break;
		case 4: 
		  yright = atof(fields.at(i).c_str());                             break;
		case 5: 
		  yrms   = atof(fields.at(i).c_str());                             break;
		case 6:
		  nj     = atoi(fields.at(i).c_str()); 
		  if ( nj == 0 ) { 
		    printf("DataBase::fill(\042%s\042,\042%s\042) - number of jets = %i data <%s> ??\n",stype.c_str(),baseDir.c_str(),nj,fields.at(i).c_str());
		  }
		  break;
		default:
		  break;
		}
	      }
	    } // data fields
	    // construct range key
	    rkey_t zstr;
	    for ( auto siter = tstr.begin(); siter != tstr.end(); ++siter ) { if ( (*siter) != '.' ) { zstr += *siter; } }
	    if ( zstr == "32_45" ) { zstr = "32_49"; }
	    // fill entry into database
	    //	    bd.push_back(bin_t(yblow,ybhigh,yave,yleft,yright,yrms,nj));
	    printf("DataBase::fill(\042%s\042,\042%s\042) - fill region tag <%s>\n",pu.c_str(),sn.c_str(),zstr.c_str());
	    bd[zstr] = bin_t(yblow,ybhigh,yave,yleft,yright,yrms,nj);
	  } // analyze line 
	} // read file 
	if ( readData ) { _store[key_t(pu,sn)] = bd; }
      } // signal type loop
    } // pile-up configuration loop
  } // fill function

  const_iterator find(const std::string& putag,const std::string& signaltag) const { 
    return _store.find(key_t(putag,signaltag)); 
  }

  // table_iterator find(const_iterator fiter,const rkey_t& rkey ) const {
  //   if ( fiter != _store.end() ) { return fiter->second.find(rkey); } else { return binmap_t().end(); }
  // }

  const_iterator begin() const { return _store.begin(); }
  const_iterator end()   const { return _store.end();   }

  double binLowerBound(const_iterator fiter,const rkey_t& rkey) const { 
    if ( fiter == end() ) { return -1.; }
    auto fmap(fiter->second.find(rkey)); return fmap != fiter->second.end() ? fmap->second.get<0>() : -1.; 
  }
  double binUpperBound(const_iterator fiter,const rkey_t& rkey) { 
    if ( fiter == end() ) { return -1.; }
    auto fmap(fiter->second.find(rkey)); return fmap != fiter->second.end() ? fmap->second.get<1>() : -1.;
  } 
  double binAverage   (const_iterator fiter,const rkey_t& rkey) { 
    if ( fiter == end() ) { return -1.; }
    auto fmap(fiter->second.find(rkey)); return fmap != fiter->second.end() ? fmap->second.get<2>() : -1.;
  } 
  double binLeftWidth (const_iterator fiter,const rkey_t& rkey) { 
    if ( fiter == end() ) { return -1.; }
    auto fmap(fiter->second.find(rkey)); return fmap != fiter->second.end() ? fmap->second.get<3>() : -1.;
  } 
  double binRightWidth(const_iterator fiter,const rkey_t& rkey) { 
    if ( fiter == end() ) { return -1.; }
    auto fmap(fiter->second.find(rkey)); return fmap != fiter->second.end() ? fmap->second.get<4>() : -1.;
  } 
  double binRMS       (const_iterator fiter,const rkey_t& rkey) { 
    if ( fiter == end() ) { return -1.; }
    auto fmap(fiter->second.find(rkey)); return fmap != fiter->second.end() ? fmap->second.get<5>() : -1.;
  } 
  int    binEntries   (const_iterator fiter,const rkey_t& rkey) { 
    if ( fiter == end() ) { return -1.; }
    auto fmap(fiter->second.find(rkey)); return fmap != fiter->second.end() ? fmap->second.get<6>() : -1;
  } 

  void printKeys(){
    printf("\n");
    for ( auto si : _store ) {
      for ( auto di : si.second ) { 
	printf("DataBase::printKeys() [%s][%-8.8s][%s]\n",si.first.first.c_str(),si.first.second.c_str(),di.first.c_str());
      }
    }
    printf("\n");
  }

  void printData(){
    printf("\n");
    for ( auto si : _store ) {
      for ( auto di : si.second ) { 
	printf("DataBase::printKeys() [%s][%-8.8s][%s]",si.first.first.c_str(),si.first.second.c_str(),di.first.c_str());
	printf(": %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %3i\n",
	       di.second.get<0>(),
	       di.second.get<1>(),
	       di.second.get<2>(),
	       di.second.get<3>(),
	       di.second.get<4>(),
	       di.second.get<5>(),
	       di.second.get<6>()
	       );
      }
    }
    printf("\n");
  }
};
#endif
