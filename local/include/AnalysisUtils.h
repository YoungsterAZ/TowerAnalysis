// -*- c++ -*-
#ifndef ANALYSISUTILS_H
#define ANALYSISUTILS_H

#include <string>
#include <map>
#include <cstdio>
#include <cstdarg>
#include <typeinfo>
#include <sstream>
#include <cmath>

#include "AnalysisUtils.h"

#define DEF_MSG_FORMATTER( FMT, MSG )		\
  static char _buffer[1024];			\
  va_list args;					\
  va_start(args, FMT );				\
  vsprintf(_buffer, FMT.c_str(),args);		\
  va_end(args);					\
  std::string MSG (_buffer);
#define DEF_MOD_FORMATTER( MODN, MODF, W )				\
  if ( MODN.length() > (size_t) W ) {					\
    MODF = std::string("[") + MODN.substr(0, W );			\
    for ( size_t i(MODF.length()-3); i<MODF.length(); ++i ) {		\
      MODF[i] = '.';							\
    }									\
    MODF += std::string("]");						\
  } else if ( MODN.length() == (size_t) W ) {				\
    MODF = std::string("[") + MODN + std::string("]");			\
  } else {								\
    MODF = std::string("[") + MODN + std::string("]");			\
    for ( size_t i(MODN.length()); i < (size_t) W ; ++i ) { MODF += std::string(" "); } \
  }
										    									       
  
#define DEF_TYPENAME( TYPE , ARG )		\
  template<>					\
  std::string typeName( TYPE ARG )		\
  { return std::string( #TYPE ); }

namespace Analysis {
  namespace Svc {
    namespace Msg {
      namespace Limits {
	static int ctsInfo    = 1000;
	static int ctsWarning = 1000;
	static int ctsError   = 100;
	static int ctsDebug   = 1000;
      } // Limits
      namespace Counts { 
	static std::map<std::string,int> ctsInfo    = std::map<std::string,int>();
	static std::map<std::string,int> ctsWarning = std::map<std::string,int>();
	static std::map<std::string,int> ctsError   = std::map<std::string,int>();
	static std::map<std::string,int> ctsDebug   = std::map<std::string,int>();
      } // Counts
      class Handler 
      {
      private:

	static Handler* _instance;
	Handler() { };

	int _addCts(std::map<std::string,int>& ctsMap,const std::string& module) {
	  if ( ctsMap.find(module) == ctsMap.end() ) { ctsMap[module] = 0; }
	  ctsMap[module] += 1;
	  return ctsMap[module];
	}

	int _addCtsInfo(const std::string& module)    { return this->_addCts(Counts::ctsInfo,module);    }
	int _addCtsWarning(const std::string& module) { return this->_addCts(Counts::ctsWarning,module); }
	int _addCtsError(const std::string& module)   { return this->_addCts(Counts::ctsError,module);   } 
	int _addCtsDebug(const std::string& module)   { return this->_addCts(Counts::ctsDebug,module);   }

      public:

	static Handler* instance() { if ( _instance == 0 ) { _instance = new Handler(); } return _instance; }

	// print Info
	void printInfo(const std::string& module,const std::string& msg)    
	{ 
	  static int _nc = 24; static int _nw = _nc + 2;
	  int ncts(_addCtsInfo(module)); if ( ncts > Limits::ctsInfo ) { return; }
	  std::string modf;
	  DEF_MOD_FORMATTER( module, modf, _nc );
	  if ( ncts == Limits::ctsInfo ) { 
	    printf("%*.*s INFO    %s [final message %i/%i]\n",_nw,_nw,modf.c_str(),msg.c_str(),ncts,Limits::ctsInfo);
	  } else {
	    printf("%*.*s INFO    %s\n",_nw,_nw,modf.c_str(),msg.c_str());
	  }
	} // Info

	// print Warning
	void printWarning(const std::string& module,const std::string& msg)    
	{ 
	  static int _nc = 24; static int _nw = _nc + 2;
	  int ncts(_addCtsWarning(module)); if ( ncts > Limits::ctsWarning ) { return; }
	  std::string modf;
	  DEF_MOD_FORMATTER( module, modf, _nc );
	  if ( ncts == Limits::ctsWarning ) { 
	    printf("%*.*s WARNING %s [final message %i/%i]\n",_nw,_nw,modf.c_str(),msg.c_str(),ncts,Limits::ctsWarning);
	  } else {
	    printf("%*.*s WARNING %s\n",_nw,_nw,modf.c_str(),msg.c_str());
	  }
	} // Warning

	// print Error
	void printError(const std::string& module,const std::string& msg)    
	{ 
	  static int _nc = 24; static int _nw = _nc + 2;
	  int ncts(_addCtsError(module)); if ( ncts > Limits::ctsError ) { return; }
	  std::string modf;
	  DEF_MOD_FORMATTER( module, modf, _nc );
	  if ( ncts == Limits::ctsError ) { 
	    printf("%*.*s ERROR   %s [final message %i/%i]\n",_nw,_nw,modf.c_str(),msg.c_str(),ncts,Limits::ctsError);
	  } else {
	    printf("%*.*s ERROR   %s\n",_nw,_nw,modf.c_str(),msg.c_str());
	  }
	} // Error

	// print Debug
	void printDebug(const std::string& module,const std::string& msg)    
	{ 
	  static int _nc = 24; static int _nw = _nc + 2;
	  if ( !Analysis::Config::Job::printDebug ) { return; }
	  int ml(std::min(Limits::ctsDebug,Analysis::Config::Job::msgsDebug));
	  int ncts(_addCtsDebug(module)); if ( ncts > ml ) { return; }
	  std::string modf;
	  DEF_MOD_FORMATTER( module, modf, _nc );
	  if ( ncts == ml ) { 
	    printf("%*.*s DEBUG   %s [final message %i/%i]\n",_nw,_nw,modf.c_str(),msg.c_str(),ncts,Limits::ctsDebug);
	  } else {
	    printf("%*.*s DEBUG   %s\n",_nw,_nw,modf.c_str(),msg.c_str());
	  }
	} // Debug
      }; // Handler
    } // Msg
  } // Svc
} // Analysis

Analysis::Svc::Msg::Handler* Analysis::Svc::Msg::Handler::_instance = 0;

namespace Analysis { 
  struct Print {
    static void Info(const std::string& module,std::string fmt, ... )    { DEF_MSG_FORMATTER( fmt, msg ); Analysis::Svc::Msg::Handler::instance()->printInfo(module,msg);    }
    static void Warning(const std::string& module,std::string fmt, ... ) { DEF_MSG_FORMATTER( fmt, msg ); Analysis::Svc::Msg::Handler::instance()->printWarning(module,msg); }
    static void Error(const std::string& module,std::string fmt, ... )   { DEF_MSG_FORMATTER( fmt, msg ); Analysis::Svc::Msg::Handler::instance()->printError(module,msg);   }
    static void Debug(const std::string& module,std::string fmt, ... )   { DEF_MSG_FORMATTER( fmt, msg ); Analysis::Svc::Msg::Handler::instance()->printDebug(module,msg);   }
  }; // Print
  namespace Utils {
    namespace Types {
      template<class T>
      std::string typeName(T arg)               { return std::string(typeid(arg).name()); }
      template<>
      std::string typeName(std::string /*arg*/) { return std::string("std::string"); }
      template<>
      std::string typeName(size_t /*arg*/)      { return std::string("size_t"); }
      DEF_TYPENAME( int,      /*arg*/ )
      DEF_TYPENAME( long,     /*arg*/ )
      DEF_TYPENAME( float,    /*arg*/ )
      DEF_TYPENAME( double,   /*arg*/ )
    }
    namespace String {
      static std::string fill(const std::string& astr,size_t slen) {
	static std::string _fstr = ".";	static std::string _sstr = " ";
	// request emnpty string
	if ( slen == 0 )             { return std::string(); }
	// same length
	if ( slen == astr.length() ) { return astr; }
	// requested length is shorter
	if ( slen < astr.length() ) {
	  std::string ostr(astr.substr(0,slen));
	  switch ( ostr.length() )
	    {
	    case 1:
	      return ostr;
	    case 2:
	      return ostr.substr(0,1)+_sstr;
	    case 3:
	      return ostr.substr(0,1)+_sstr+_fstr;
	    default:
	      for ( size_t i(ostr.length()-3); i<ostr.length(); ++i ) { ostr[i] = *(_fstr.c_str()); }
	      return ostr;
	    }
	}
	// requested length is longer
	std::string ostr(astr+_sstr);
	for ( size_t i(ostr.length()); i<slen; ++i ) { ostr += _fstr; }
	return ostr;
      }
      static std::string compress(const std::string& astr,const std::string& fstr,const std::string& lstr) {
	if ( astr.empty() || astr.length() == 0 || astr == "" ) { return astr; }
	size_t fp(astr.find_first_not_of(fstr));   if ( fp == std::string::npos ) { fp = 0; }
	size_t lp(astr.find_last_not_of(lstr,fp));
	return lp == std::string::npos || lp <= fp ? astr.substr(fp) : astr.substr(fp,lp-fp+1);
      }
      static std::string compress(const std::string& astr,const std::string& cstr=" ") {
	return compress(astr,cstr,cstr);
      }; 
      template<class T>
      bool fetchValue(const std::string& astr,const std::string& kstr,T& value) {
	static std::string _ass = "=";
	size_t kp(astr.find(kstr));
	if ( kp == std::string::npos ) { return false; }
	std::string vstr(astr.substr(kp+kstr.length()));
	size_t ap(vstr.find_last_of(_ass));
	if ( ap == std::string::npos ) { return false; }
	vstr = compress(vstr.substr(ap+1));
	T lvar = T();
	std::stringstream sstr;
	sstr << vstr.c_str(); 
	sstr >> lvar;
	if ( sstr.fail() ) { 
	  Analysis::Print::Warning("Analysis::Utils::String::fetchValue","cannot cast \042%s\042 into variable of type <%s>",vstr.c_str(),Analysis::Utils::Types::typeName(value).c_str());
	  return false;
	}
	value = lvar;
	return true;
      }
      template<>
      bool fetchValue(const std::string& astr,const std::string& kstr,std::string& value) {
	static std::string _ass = "=";
	size_t kp(astr.find(kstr));
	if ( kp == std::string::npos ) { return false; }
	std::string vstr(astr.substr(kp+kstr.length()));
	size_t ap(vstr.find_last_of(_ass));
	if ( ap == std::string::npos ) { return false; }
	value = compress(vstr.substr(ap+1));
	return true;
      }
    } // String
  } // Utils
} // Analysis

#define PRINT_INFO    Analysis::Print::Info
#define PRINT_WARNING Analysis::Print::Warning
#define PRINT_ERROR   Analysis::Print::Error
#define PRINT_DEBUG   Analysis::Print::Debug
#endif
