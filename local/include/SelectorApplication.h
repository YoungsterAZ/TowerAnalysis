// -*- c++ -*-
#ifndef SELECTORAPPLICATION_H
#define SELECTORAPPLICATION_H

#include "SelectorBase.h"

#include "AnalysisData.h"
#include "AnalysisCfg.h"

namespace Analysis {
  namespace Selector {
    class CaloTiming : virtual public Base
    {
    public:
      CaloTiming(double timeLow=Analysis::Select::CaloSignal::timeMin,double timeHigh=Analysis::Select::CaloSignal::timeMax);
      virtual ~CaloTiming();
      virtual bool accept(const fastjet::PseudoJet& pj) const;
      virtual double time()                             const;
      virtual bool   isValid()                          const;  
    protected:
      double _timeMin;
      double _timeMax;
      void   _setTime(double time) const { _actTime = time; } 
      double _getTime()            const { return _actTime; }
      bool _isValid(double time)   const { return time > Analysis::FastJet::CaloSignalInfo::defTime(); }
      bool   _isValid()            const { return _isValid(_actTime); }
    private :
      mutable double _actTime;
    };
    class JetTiming : public CaloTiming
    {
    public:
      JetTiming(double timeLow=Analysis::Select::RecoJet::timeMin,double timeMax=Analysis::Select::RecoJet::timeMax);
      virtual ~JetTiming();
      virtual bool accept(const fastjet::PseudoJet& pj) const;
    };
  }
}
#endif
