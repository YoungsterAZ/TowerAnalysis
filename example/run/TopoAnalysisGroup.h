// -*- c++ -*-
#ifndef TOPOANALYSISGROUP_H
#define TOPOANALYSISGROUP_H

#include "AnalysisData.h"

#include "AnalysisGroup.h"

class TopoAnalysisGroup : public Analysis::GroupBase
{
 public:
  TopoAnalysisGroup(const std::string& name="TopoAnalysisGroup");
  virtual ~TopoAnalysisGroup();

  virtual bool book();
  //  virtual bool fill(const std::vector<fastjet::PseudoJet>& pjs);
  using Analysis::GroupBase::fill;
  virtual bool fill(const Analysis::Matching::JetRelationContainer& jrel,const std::vector<fastjet::PseudoJet>& tjet,const std::vector<fastjet::PseudoJet>& cjet);

 protected:

  bool isBooked();

  TH1D* h_dr;              // jet distance
  TH2D* d_dr_rap;          // jet distance as function of rapidity

  TH2D* d_pt_calo_truth;   // pT in calo versus true pT
  TH2D* d_rap_calo_truth;  // rapidity distance between jets
  TH2D* d_phi_calo_truth;  // azimuthal angle between jets

  TH2D* d_ptratio_rap;     // (pT_calo-pT_truth)/pT_truth ratio versus rapidity
  TH2D* d_rapdistance_rap; // distance in rapidity versus rapidity
  TH2D* d_phidistance_rap; // azimuthal distance versus rapdity

  // // jet multiplicity
  // TH1D* h_njet; 
  // // jet pT
  // TH1D* h_ptjet;
  // // jet rapidity 
  // TH1D* h_rapjet; 
  // // jet pT vs rapidity
  // TH2D* d_pt_rapjet;

  // // jet timing
  // TH1D* h_tjet;
  // // jet timing vs rapidity
  // TH2D* d_tjet_rapjet;
  // // calo signal timing in jet
  // TH1D* h_tsig;
  // // calo signal timing vs rapidity
  // TH2D* d_tsig_rapsig;



};


#endif
