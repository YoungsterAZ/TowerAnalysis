
#include "SelectorApplication.h"

#include "AnalysisUtils.h"

Analysis::Selector::CaloTiming::CaloTiming(double timeMin,double timeMax)
  : Base()
  , _timeMin(timeMin)
  , _timeMax(timeMax)
  , _actTime(Analysis::FastJet::CaloSignalInfo::defTime())
{
  PRINT_INFO("CaloTiming::CaloTiming","selector instantiated with t_min = %.1f ns, t_max = %.1f ns",_timeMin,_timeMax);
}

Analysis::Selector::CaloTiming::~CaloTiming()
{ }

bool Analysis::Selector::CaloTiming::accept(const fastjet::PseudoJet& pj) const {
  double t(Analysis::FastJet::CaloSignalInfo::caloTime(pj)); _setTime(t);
  return t > Analysis::FastJet::CaloSignalInfo::defTime() && ( t > _timeMin && t < _timeMax ); 
}

double Analysis::Selector::CaloTiming::time() const 
{ return _actTime; }

bool Analysis::Selector::CaloTiming::isValid() const
{ return _isValid(); }

Analysis::Selector::JetTiming::JetTiming(double timeMin,double timeMax)
  : CaloTiming(timeMin,timeMax)
{ }

Analysis::Selector::JetTiming::~JetTiming()
{ }

bool Analysis::Selector::JetTiming::accept(const fastjet::PseudoJet& pj) const {
  if ( !pj.has_valid_cs() ) { return false; }
  double tjet(0.); double ejet(0.); bool jetHasTime(false);
  for ( auto cj : pj.constituents() ) { 
    double t(Analysis::FastJet::CaloSignalInfo::caloTime(cj));
    if ( _isValid(t) ) { tjet += (pj.e()*t); ejet += pj.e(); jetHasTime = true; }
  }
  if ( !jetHasTime ) { _setTime(Analysis::FastJet::CaloSignalInfo::defTime()); return false; } 
  tjet /= ejet; _setTime(tjet);
  return tjet > _timeMin && tjet < _timeMax;
}
