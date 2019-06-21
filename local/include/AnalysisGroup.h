// -*- c++ -*-
#ifndef ANALYSISGROUP_H
#define ANALYSISGROUP_H

#include <string>
#include <vector>

#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>

#include "fastjet/PseudoJet.hh"

namespace Analysis {

  class IGroup 
  {
  public:
    virtual ~IGroup() { }
    virtual const std::string& name() const                  = 0;
    virtual bool book()                                      = 0;
  };

  class GroupBase : virtual public IGroup
  {
  private:
    std::string _name;
  protected:
    std::string histName(const std::string& hname)                { static std::string sep = "/"; return _name+sep+hname; }
    std::string axisTitle(const std::string& fmt,double binWidth) { return std::string(TString::Format(fmt.c_str(),binWidth).Data()); }  
    std::string axisTitle(const std::string& fmt,double binWidthX,double binWidthY) { return std::string(TString::Format(fmt.c_str(),binWidthX,binWidthY).Data()); }  
  public:
    GroupBase(const std::string& name="GroupBase");
    virtual ~GroupBase() { }
    virtual const std::string& name() const                                                             { return _name; }
    virtual bool book()                                                                                 { return true; }
    virtual bool fill(const std::vector<fastjet::PseudoJet>& /*pjs*/)                                   { return true; }
  };
}

inline Analysis::GroupBase::GroupBase(const std::string& name) : _name(name) { }

#endif
