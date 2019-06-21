// -*- c++ -*-
#ifndef TWOJETANALYSISGROUP_H
#define TWOJETANALYSISGROUP_H

#include "AnalysisGroup.h"

class TwoJetAnalysisGroup : public Analysis::GroupBase
{
 public:
  TwoJetAnalysisGroup(const std::string& name="JetAnalysisGroup");
  virtual ~TwoJetAnalysisGroup();

  virtual bool book();
  using Analysis::GroupBase::fill;
  virtual bool fill(const std::vector<fastjet::PseudoJet>& pjs,size_t ifrst,size_t iscnd);

 protected:

  bool isBooked();

  //  bool fillTiming(const fastjet::PseudoJet& pj,TH1D* 

  // jet pT
  TH1D* h_ptjet_lead;
  TH1D* h_ptjet_sublead;
  // jet rapidity 
  TH1D* h_rapjet_lead; 
  TH1D* h_rapjet_sublead; 
  // jet pT vs rapidity
  TH2D* d_pt_rapjet_lead;
  TH2D* d_pt_rapjet_sublead;

  // topology
  TH1D* h_mass;
  TH2D* d_mass_rap_lead;
  TH2D* d_mass_rap_sublead;
  TH2D* d_mass_rapgap;
  TH2D* d_rap_rap;

  struct TimingPlots { 
    // jet timing
    TH1D* h_tjet;
    // jet timing vs rapidity
    TH2D* d_tjet_rapjet;
    // calo signal timing in jet
    TH1D* h_tsig;
    // calo signal timing vs rapidity
    TH2D* d_tsig_rapsig;
    //
    TimingPlots() : h_tjet((TH1D*)0), d_tjet_rapjet((TH2D*)0), h_tsig((TH1D*)0), d_tsig_rapsig((TH2D*)0) { }
  };

  void fillTiming(const fastjet::PseudoJet& pj,TimingPlots& tplot);

  TimingPlots t_lead;
  TimingPlots t_sublead;

};
#endif
