// -*- c++ -*-
#ifndef SELECTORBASE_H
#define SELECTORBASE_H

#include "fastjet/PseudoJet.hh"

namespace Analysis {
  namespace Selector { 
    class Interface 
    {
    public :
      virtual ~Interface() { }
      virtual bool accept(const fastjet::PseudoJet& pj) const = 0;
      virtual bool operator()(const fastjet::PseudoJet& pj) const = 0;
    };
    
    class Base : virtual public Interface 
    {
    public:
      Base() { };
      virtual ~Base() { }
      virtual bool operator()(const fastjet::PseudoJet& pj) const { return accept(pj); }
    };
  }
}
#endif
