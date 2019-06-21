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

  // jet multiplicity
  TH1D* h_njet; 
  // jet pT
  TH1D* h_ptjet;
  // jet rapidity 
  TH1D* h_rapjet; 
  // jet pT vs rapidity
  TH2D* d_pt_rapjet;

  // jet timing
  TH1D* h_tjet;
  // jet timing vs rapidity
  TH2D* d_tjet_rapjet;
  // calo signal timing in jet
  TH1D* h_tsig;
  // calo signal timing vs rapidity
  TH2D* d_tsig_rapsig;



};
#endif
