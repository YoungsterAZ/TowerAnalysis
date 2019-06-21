// -*- c++ -*-
#ifndef READMLFILE_H
#define READMLFILE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

//#include <boost/tuple/tuple.hpp>

struct Tuple {
  double pt;
  double m;
  double width; 
  double ptd;
  int    nc;
  double psig;
  double pbck;
  int    tag;
  Tuple(double ipt,double im,double iwidth,double iptd,int inc,double ipsig,double ipbck,int itag) 
    : pt(ipt), m(im), width(iwidth), ptd(iptd), nc(inc), psig(ipsig), pbck(ipbck), tag(itag) { }
  Tuple() : Tuple(0.,0.,0.,0.,0,0.,0.,0) { } 
};

class ReadMLFile 
{
public:

  typedef Tuple                tuple_t;
  typedef std::vector<tuple_t> store_t;

private:

  std::string _fname;
  store_t     _store;

  bool f_readFile() {
    _store.reserve(100000);
    _store.clear();
    std::ifstream f(_fname);
    std::string hstr;
    std::getline(f,hstr,'\n');
    size_t l(0);
    while ( f.good() ) {
      std::string dstr; std::getline(f,dstr,'\n');
      // size_t fcr(dstr.find_first_of('\r'));
      // if ( fcr != std::string::npos ) {
      // 	printf("RedMLFile::f_readFile(...) - found carriage return character at position %zu in line %zu",fcr,l); 
      // 	dstr = dstr.substr(0,fcr-1);
      // 	printf(", adjusted line length to %zu characters\n",dstr.length()); 
      // }
      std::stringstream sstr(dstr);
      tuple_t t; size_t stp(0); size_t icr(std::string::npos);
      while ( std::getline(sstr,dstr,',') ) {
	switch ( stp ) {
	case 0:
	  t.pbck  = atof(dstr.c_str()); break;
	case 1:
	  t.psig  = atof(dstr.c_str()); break;
	case 2:
	  t.tag   = atoi(dstr.c_str()); break;
	case 3:
	  t.pt    = atof(dstr.c_str()); break;
	case 4:
	  t.m     = atof(dstr.c_str()); break;
	case 5:
	  t.width = atof(dstr.c_str()); break;
	case 6:
	  t.ptd   = atof(dstr.c_str()); break;
	case 7:
	  t.nc    = atoi(dstr.c_str()); break;
	default:
	  break;
	}
	// if ( l ==0 || l % 10000 == 0 ) {
	//   printf("ReadMLFile::f_readFile(...) line %6zu field read %zu value \042%s\042\n",l,stp,dstr.c_str());
	// }
	++stp;
      }
      if ( t.nc > 0 ) { _store.push_back(t); }
      ++l;
    }
    printf("ReadMLFile::f_readFile(...) - read %zu entries from file \042%s\042\n",_store.size(),_fname.c_str());
    f.close();
    return !_store.empty();
  }

public: 

  ReadMLFile(const std::string& fname) : _fname(fname) { f_readFile(); }

  const store_t& getdata()        const { return _store;         }
  store_t::const_iterator begin() const { return _store.begin(); }
  store_t::const_iterator end()   const { return _store.end();   }
  size_t                  size()  const { return _store.size();  }
  bool                    empty() const { return _store.empty(); }

  double pt   (store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->pt   : -1.0;   }
  double m    (store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->m    : -1.0;   }
  double width(store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->width  : -1.0; }
  double ptd  (store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->ptd  : -1.0;   }
  int    nc   (store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->nc   : -1;     }  
  double psig (store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->psig : -1.0;   }
  double pbck (store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->pbck : -1.0;   }
  int    tag  (store_t::const_iterator fiter) const { return fiter != _store.end() ? fiter->tag  : -1;     }

  double pt   (const tuple_t& tuple) const { return tuple.pt;    }
  double m    (const tuple_t& tuple) const { return tuple.m;     }
  double width(const tuple_t& tuple) const { return tuple.width; }
  double ptd  (const tuple_t& tuple) const { return tuple.ptd;   }
  int    nc   (const tuple_t& tuple) const { return tuple.nc;    }
  double psig (const tuple_t& tuple) const { return tuple.psig;  }
  double pbck (const tuple_t& tuple) const { return tuple.pbck;  }
  int    tag  (const tuple_t& tuple) const { return tuple.tag;   }
};
#endif
