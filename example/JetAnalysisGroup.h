// -*- c++ -*-
#ifndef JETANALYSISGROUP_H
#define JETANALYSISGROUP_H

#include "AnalysisGroup.h"

class JetAnalysisGroup : public Analysis::GroupBase
{
 public:
  JetAnalysisGroup(const std::string& name="JetAnalysisGroup");
  virtual ~JetAnalysisGroup();

  virtual bool book();
  virtual bool fill(const std::vector<fastjet::PseudoJet>& pjs);

 protected:

  bool isBooked();

  TH1D* h_njet; 
  TH1D* h_ptjet; 
  TH1D* h_rapjet; 

  TH2D* d_pt_rapjet;


};
#endif
