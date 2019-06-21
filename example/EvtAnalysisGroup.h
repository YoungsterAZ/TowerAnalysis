// -*- c++ -*-
#ifndef EVTANALYSISGROUP_H
#define EVTANALYSISGROUP_H

#include "AnalysisGroup.h"

class EvtAnalysisGroup : public Analysis::BaseGroup
{
public:
  EvtAnalysisGroup(const std::string& name="EvtAnalysisGroup");
  virtual ~EvtAnalysisGroup();

  virtual bool book();
  virtual bool fill(const std::vector<double>& values);
};
#endif
