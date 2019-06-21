// -*- c++ -*-
//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb  7 14:17:55 2018 by ROOT version 6.10/04
// from TTree CollectionTree/CollectionTree
// found on file: vbf2600_tower_tuple_001.root
//////////////////////////////////////////////////////////

#ifndef JetAnalysisExample_h
#define JetAnalysisExample_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <string>

#include "AnalysisHelper.h"

class JetAnalysisExample {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  
  // Fixed size dimensions of array or collections stored in the TTree if any.
  
  // Declaration of leaf types
  Float_t         event_rho_em;
  Float_t         event_sigma_em;
  Float_t         event_area_em;
  Float_t         event_rho_lcw;
  Float_t         event_sigma_lcw;
  Float_t         event_area_lcw;
  Float_t         event_mu;
  Int_t           event_npv;
  Float_t         event_weight;
  vector<float>   *recovx_x;
  vector<float>   *recovx_y;
  vector<float>   *recovx_z;
  vector<float>   *recovx_sumPt;
  vector<int>     *recovx_index;
  vector<float>   *truthvx_x;
  vector<float>   *truthvx_y;
  vector<float>   *truthvx_z;
  vector<float>   *truthvx_t;
  vector<int>     *truthvx_index;
  vector<float>   *topoclus_lcw_e;
  vector<float>   *topoclus_lcw_pt;
  vector<float>   *topoclus_lcw_eta;
  vector<float>   *topoclus_lcw_phi;
  vector<float>   *topoclus_em_e;
  vector<float>   *topoclus_em_pt;
  vector<float>   *topoclus_em_eta;
  vector<float>   *topoclus_em_phi;
  vector<float>   *topoclus_em_time;
  vector<float>   *topoclus_em_sign;
  vector<float>   *topoclus_em_epos;
  vector<float>   *topoclus_FIRST_ETA;
  vector<float>   *topoclus_FIRST_PHI;
  vector<float>   *topoclus_DELTA_THETA;
  vector<float>   *topoclus_DELTA_PHI;
  vector<float>   *topoclus_DELTA_ALPHA;
  vector<float>   *topoclus_CENTER_X;
  vector<float>   *topoclus_CENTER_Y;
  vector<float>   *topoclus_CENTER_Z;
  vector<float>   *topoclus_CENTER_MAG;
  vector<float>   *topoclus_CENTER_LAMBDA;
  vector<float>   *topoclus_SECOND_R;
  vector<float>   *topoclus_SECOND_LAMBDA;
  vector<float>   *topoclus_LATERAL;
  vector<float>   *topoclus_LONGITUDINAL;
  vector<float>   *topoclus_ENG_FRAC_EM;
  vector<float>   *topoclus_ENG_FRAC_MAX;
  vector<float>   *topoclus_ENG_FRAC_CORE;
  vector<float>   *topoclus_FIRST_ENG_DENS;
  vector<float>   *topoclus_SECOND_ENG_DENS;
  vector<float>   *topoclus_PTD;
  vector<float>   *topoclus_EM_PROBABILITY;
  vector<float>   *topoclus_CELL_SIGNIFICANCE;
  vector<int>     *topoclus_CELL_SIG_SAMPLING;
  vector<float>   *topoclus_ISOLATION;
  vector<float>   *topoclus_MASS;
  vector<float>   *ttowfine_lcw_e;
  vector<float>   *ttowfine_lcw_pt;
  vector<float>   *ttowfine_lcw_eta;
  vector<float>   *ttowfine_lcw_phi;
  vector<float>   *ttowfine_em_e;
  vector<float>   *ttowfine_em_pt;
  vector<float>   *ttowfine_em_eta;
  vector<float>   *ttowfine_em_phi;
  vector<float>   *ttowfine_em_time;
  vector<float>   *ttowfine_em_sign;
  vector<float>   *ttowfine_em_epos;
  vector<float>   *ttowfine_FIRST_ETA;
  vector<float>   *ttowfine_FIRST_PHI;
  vector<float>   *ttowfine_DELTA_THETA;
  vector<float>   *ttowfine_DELTA_PHI;
  vector<float>   *ttowfine_DELTA_ALPHA;
  vector<float>   *ttowfine_CENTER_X;
  vector<float>   *ttowfine_CENTER_Y;
  vector<float>   *ttowfine_CENTER_Z;
  vector<float>   *ttowfine_CENTER_MAG;
  vector<float>   *ttowfine_CENTER_LAMBDA;
  vector<float>   *ttowfine_SECOND_R;
  vector<float>   *ttowfine_SECOND_LAMBDA;
  vector<float>   *ttowfine_LATERAL;
  vector<float>   *ttowfine_LONGITUDINAL;
  vector<float>   *ttowfine_ENG_FRAC_EM;
  vector<float>   *ttowfine_ENG_FRAC_MAX;
  vector<float>   *ttowfine_ENG_FRAC_CORE;
  vector<float>   *ttowfine_FIRST_ENG_DENS;
  vector<float>   *ttowfine_SECOND_ENG_DENS;
  vector<float>   *ttowfine_PTD;
  vector<float>   *ttowfine_EM_PROBABILITY;
  vector<float>   *ttowfine_CELL_SIGNIFICANCE;
  vector<int>     *ttowfine_CELL_SIG_SAMPLING;
  vector<float>   *ttowfine_ISOLATION;
  vector<float>   *ttowfine_MASS;
  vector<float>   *ttownorm_lcw_e;
  vector<float>   *ttownorm_lcw_pt;
  vector<float>   *ttownorm_lcw_eta;
  vector<float>   *ttownorm_lcw_phi;
  vector<float>   *ttownorm_em_e;
  vector<float>   *ttownorm_em_pt;
  vector<float>   *ttownorm_em_eta;
  vector<float>   *ttownorm_em_phi;
  vector<float>   *ttownorm_em_time;
  vector<float>   *ttownorm_em_sign;
  vector<float>   *ttownorm_em_epos;
  vector<float>   *ttownorm_FIRST_ETA;
  vector<float>   *ttownorm_FIRST_PHI;
  vector<float>   *ttownorm_DELTA_THETA;
  vector<float>   *ttownorm_DELTA_PHI;
  vector<float>   *ttownorm_DELTA_ALPHA;
  vector<float>   *ttownorm_CENTER_X;
  vector<float>   *ttownorm_CENTER_Y;
  vector<float>   *ttownorm_CENTER_Z;
  vector<float>   *ttownorm_CENTER_MAG;
  vector<float>   *ttownorm_CENTER_LAMBDA;
  vector<float>   *ttownorm_SECOND_R;
  vector<float>   *ttownorm_SECOND_LAMBDA;
  vector<float>   *ttownorm_LATERAL;
  vector<float>   *ttownorm_LONGITUDINAL;
  vector<float>   *ttownorm_ENG_FRAC_EM;
  vector<float>   *ttownorm_ENG_FRAC_MAX;
  vector<float>   *ttownorm_ENG_FRAC_CORE;
  vector<float>   *ttownorm_FIRST_ENG_DENS;
  vector<float>   *ttownorm_SECOND_ENG_DENS;
  vector<float>   *ttownorm_PTD;
  vector<float>   *ttownorm_EM_PROBABILITY;
  vector<float>   *ttownorm_CELL_SIGNIFICANCE;
  vector<int>     *ttownorm_CELL_SIG_SAMPLING;
  vector<float>   *ttownorm_ISOLATION;
  vector<float>   *ttownorm_MASS;
  vector<int>     *truthlept_pdg;
  vector<int>     *truthlept_status;
  vector<float>   *truthlept_charge;
  vector<int>     *truthlept_barcode;
  vector<bool>    *truthlept_vtxflag;
  vector<int>     *truthlept_vtxid;
  vector<float>   *truthlept_e;
  vector<float>   *truthlept_pt;
  vector<float>   *truthlept_px;
  vector<float>   *truthlept_py;
  vector<float>   *truthlept_pz;
  vector<float>   *truthlept_m;
  vector<int>     *truthhadr_pdg;
  vector<int>     *truthhadr_status;
  vector<float>   *truthhadr_charge;
  vector<int>     *truthhadr_barcode;
  vector<bool>    *truthhadr_vtxflag;
  vector<int>     *truthhadr_vtxid;
  vector<float>   *truthhadr_e;
  vector<float>   *truthhadr_pt;
  vector<float>   *truthhadr_px;
  vector<float>   *truthhadr_py;
  vector<float>   *truthhadr_pz;
  vector<float>   *truthhadr_m;
  vector<float>   *truthjet_e;
  vector<float>   *truthjet_pt;
  vector<float>   *truthjet_px;
  vector<float>   *truthjet_py;
  vector<float>   *truthjet_pz;
  vector<float>   *truthjet_rap;
  vector<float>   *truthjet_eta;
  vector<float>   *truthjet_phi;
  vector<float>   *truthjet_mass;
  vector<vector<int> > *truthjet_comp;

  // List of branches
  TBranch        *b_event_rho_em;   //!
  TBranch        *b_event_sigma_em;   //!
  TBranch        *b_event_area_em;   //!
  TBranch        *b_event_rho_lcw;   //!
  TBranch        *b_event_sigma_lcw;   //!
  TBranch        *b_event_area_lcw;   //!
  TBranch        *b_event_mu;   //!
  TBranch        *b_event_npv;   //!
  TBranch        *b_event_weight;   //!
  TBranch        *b_recovx_x;   //!
  TBranch        *b_recovx_y;   //!
  TBranch        *b_recovx_z;   //!
  TBranch        *b_recovx_sumPt;   //!
  TBranch        *b_recovx_index;   //!
  TBranch        *b_truthvx_x;   //!
  TBranch        *b_truthvx_y;   //!
  TBranch        *b_truthvx_z;   //!
  TBranch        *b_truthvx_t;   //!
  TBranch        *b_truthvx_index;   //!
  TBranch        *b_topoclus_lcw_e;   //!
  TBranch        *b_topoclus_lcw_pt;   //!
  TBranch        *b_topoclus_lcw_eta;   //!
  TBranch        *b_topoclus_lcw_phi;   //!
  TBranch        *b_topoclus_em_e;   //!
  TBranch        *b_topoclus_em_pt;   //!
  TBranch        *b_topoclus_em_eta;   //!
  TBranch        *b_topoclus_em_phi;   //!
  TBranch        *b_topoclus_em_time;   //!
  TBranch        *b_topoclus_em_sign;   //!
  TBranch        *b_topoclus_em_epos;   //!
  TBranch        *b_topoclus_FIRST_ETA;   //!
  TBranch        *b_topoclus_FIRST_PHI;   //!
  TBranch        *b_topoclus_DELTA_THETA;   //!
  TBranch        *b_topoclus_DELTA_PHI;   //!
  TBranch        *b_topoclus_DELTA_ALPHA;   //!
  TBranch        *b_topoclus_CENTER_X;   //!
  TBranch        *b_topoclus_CENTER_Y;   //!
  TBranch        *b_topoclus_CENTER_Z;   //!
  TBranch        *b_topoclus_CENTER_MAG;   //!
  TBranch        *b_topoclus_CENTER_LAMBDA;   //!
  TBranch        *b_topoclus_SECOND_R;   //!
  TBranch        *b_topoclus_SECOND_LAMBDA;   //!
  TBranch        *b_topoclus_LATERAL;   //!
  TBranch        *b_topoclus_LONGITUDINAL;   //!
  TBranch        *b_topoclus_ENG_FRAC_EM;   //!
  TBranch        *b_topoclus_ENG_FRAC_MAX;   //!
  TBranch        *b_topoclus_ENG_FRAC_CORE;   //!
  TBranch        *b_topoclus_FIRST_ENG_DENS;   //!
  TBranch        *b_topoclus_SECOND_ENG_DENS;   //!
  TBranch        *b_topoclus_PTD;   //!
  TBranch        *b_topoclus_EM_PROBABILITY;   //!
  TBranch        *b_topoclus_CELL_SIGNIFICANCE;   //!
  TBranch        *b_topoclus_CELL_SIG_SAMPLING;   //!
  TBranch        *b_topoclus_ISOLATION;   //!
  TBranch        *b_topoclus_MASS;   //!
  TBranch        *b_ttowfine_lcw_e;   //!
  TBranch        *b_ttowfine_lcw_pt;   //!
  TBranch        *b_ttowfine_lcw_eta;   //!
  TBranch        *b_ttowfine_lcw_phi;   //!
  TBranch        *b_ttowfine_em_e;   //!
  TBranch        *b_ttowfine_em_pt;   //!
  TBranch        *b_ttowfine_em_eta;   //!
  TBranch        *b_ttowfine_em_phi;   //!
  TBranch        *b_ttowfine_em_time;   //!
  TBranch        *b_ttowfine_em_sign;   //!
  TBranch        *b_ttowfine_em_epos;   //!
  TBranch        *b_ttowfine_FIRST_ETA;   //!
  TBranch        *b_ttowfine_FIRST_PHI;   //!
  TBranch        *b_ttowfine_DELTA_THETA;   //!
  TBranch        *b_ttowfine_DELTA_PHI;   //!
  TBranch        *b_ttowfine_DELTA_ALPHA;   //!
  TBranch        *b_ttowfine_CENTER_X;   //!
  TBranch        *b_ttowfine_CENTER_Y;   //!
  TBranch        *b_ttowfine_CENTER_Z;   //!
  TBranch        *b_ttowfine_CENTER_MAG;   //!
  TBranch        *b_ttowfine_CENTER_LAMBDA;   //!
  TBranch        *b_ttowfine_SECOND_R;   //!
  TBranch        *b_ttowfine_SECOND_LAMBDA;   //!
  TBranch        *b_ttowfine_LATERAL;   //!
  TBranch        *b_ttowfine_LONGITUDINAL;   //!
  TBranch        *b_ttowfine_ENG_FRAC_EM;   //!
  TBranch        *b_ttowfine_ENG_FRAC_MAX;   //!
  TBranch        *b_ttowfine_ENG_FRAC_CORE;   //!
  TBranch        *b_ttowfine_FIRST_ENG_DENS;   //!
  TBranch        *b_ttowfine_SECOND_ENG_DENS;   //!
  TBranch        *b_ttowfine_PTD;   //!
  TBranch        *b_ttowfine_EM_PROBABILITY;   //!
  TBranch        *b_ttowfine_CELL_SIGNIFICANCE;   //!
  TBranch        *b_ttowfine_CELL_SIG_SAMPLING;   //!
  TBranch        *b_ttowfine_ISOLATION;   //!
  TBranch        *b_ttowfine_MASS;   //!
  TBranch        *b_ttownorm_lcw_e;   //!
  TBranch        *b_ttownorm_lcw_pt;   //!
  TBranch        *b_ttownorm_lcw_eta;   //!
  TBranch        *b_ttownorm_lcw_phi;   //!
  TBranch        *b_ttownorm_em_e;   //!
  TBranch        *b_ttownorm_em_pt;   //!
  TBranch        *b_ttownorm_em_eta;   //!
  TBranch        *b_ttownorm_em_phi;   //!
  TBranch        *b_ttownorm_em_time;   //!
  TBranch        *b_ttownorm_em_sign;   //!
  TBranch        *b_ttownorm_em_epos;   //!
  TBranch        *b_ttownorm_FIRST_ETA;   //!
  TBranch        *b_ttownorm_FIRST_PHI;   //!
  TBranch        *b_ttownorm_DELTA_THETA;   //!
  TBranch        *b_ttownorm_DELTA_PHI;   //!
  TBranch        *b_ttownorm_DELTA_ALPHA;   //!
  TBranch        *b_ttownorm_CENTER_X;   //!
  TBranch        *b_ttownorm_CENTER_Y;   //!
  TBranch        *b_ttownorm_CENTER_Z;   //!
  TBranch        *b_ttownorm_CENTER_MAG;   //!
  TBranch        *b_ttownorm_CENTER_LAMBDA;   //!
  TBranch        *b_ttownorm_SECOND_R;   //!
  TBranch        *b_ttownorm_SECOND_LAMBDA;   //!
  TBranch        *b_ttownorm_LATERAL;   //!
  TBranch        *b_ttownorm_LONGITUDINAL;   //!
  TBranch        *b_ttownorm_ENG_FRAC_EM;   //!
  TBranch        *b_ttownorm_ENG_FRAC_MAX;   //!
  TBranch        *b_ttownorm_ENG_FRAC_CORE;   //!
  TBranch        *b_ttownorm_FIRST_ENG_DENS;   //!
  TBranch        *b_ttownorm_SECOND_ENG_DENS;   //!
  TBranch        *b_ttownorm_PTD;   //!
  TBranch        *b_ttownorm_EM_PROBABILITY;   //!
  TBranch        *b_ttownorm_CELL_SIGNIFICANCE;   //!
  TBranch        *b_ttownorm_CELL_SIG_SAMPLING;   //!
  TBranch        *b_ttownorm_ISOLATION;   //!
  TBranch        *b_ttownorm_MASS;   //!
  TBranch        *b_truthlept_pdg;   //!
  TBranch        *b_truthlept_status;   //!
  TBranch        *b_truthlept_charge;   //!
  TBranch        *b_truthlept_barcode; //!
  TBranch        *b_truthlept_vtxflag; //!
  TBranch        *b_truthlept_vtxid; //!
  TBranch        *b_truthlept_e;   //!
  TBranch        *b_truthlept_pt;   //!
  TBranch        *b_truthlept_px;   //!
  TBranch        *b_truthlept_py;   //!
  TBranch        *b_truthlept_pz;   //!
  TBranch        *b_truthlept_m;   //!
  TBranch        *b_truthhadr_pdg;   //!
  TBranch        *b_truthhadr_status;   //!
  TBranch        *b_truthhadr_charge;   //!
  TBranch        *b_truthhadr_barcode; //!
  TBranch        *b_truthhadr_vtxflag; //!
  TBranch        *b_truthhadr_vtxid; //!
  TBranch        *b_truthhadr_e;   //!
  TBranch        *b_truthhadr_pt;   //!
  TBranch        *b_truthhadr_px;   //!
  TBranch        *b_truthhadr_py;   //!
  TBranch        *b_truthhadr_pz;   //!
  TBranch        *b_truthhadr_m;   //!
  TBranch        *b_truthjet_e;   //!
  TBranch        *b_truthjet_pt;   //!
  TBranch        *b_truthjet_px;   //!
  TBranch        *b_truthjet_py;   //!
  TBranch        *b_truthjet_pz;   //!
  TBranch        *b_truthjet_rap;   //!
  TBranch        *b_truthjet_eta;   //!
  TBranch        *b_truthjet_phi;   //!
  TBranch        *b_truthjet_mass;   //!
  TBranch        *b_truthjet_comp;   //!
  
  JetAnalysisExample(TTree *tree=0,Analysis::Config::Input::Selector inp=Analysis::Config::Input::TopoCluster);
  virtual ~JetAnalysisExample();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

};

#endif

#ifdef JetAnalysisExample_cxx
JetAnalysisExample::JetAnalysisExample(TTree *tree,Analysis::Config::Input::Selector inp) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("vbf2600_tower_tuple_001.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("vbf2600_tower_tuple_001.root");
      }
      f->GetObject("CollectionTree",tree);

   }
   Init(tree);
   //
   Analysis::Config::Input::collection = inp;
}

JetAnalysisExample::~JetAnalysisExample()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t JetAnalysisExample::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t JetAnalysisExample::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void JetAnalysisExample::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   recovx_x = 0;
   recovx_y = 0;
   recovx_z = 0;
   recovx_sumPt = 0;
   recovx_index = 0;
   truthvx_x = 0;
   truthvx_y = 0;
   truthvx_z = 0;
   truthvx_t = 0;
   truthvx_index = 0;
   topoclus_lcw_e = 0;
   topoclus_lcw_pt = 0;
   topoclus_lcw_eta = 0;
   topoclus_lcw_phi = 0;
   topoclus_em_e = 0;
   topoclus_em_pt = 0;
   topoclus_em_eta = 0;
   topoclus_em_phi = 0;
   topoclus_em_time = 0;
   topoclus_em_sign = 0;
   topoclus_em_epos = 0;
   topoclus_FIRST_ETA = 0;
   topoclus_FIRST_PHI = 0;
   topoclus_DELTA_THETA = 0;
   topoclus_DELTA_PHI = 0;
   topoclus_DELTA_ALPHA = 0;
   topoclus_CENTER_X = 0;
   topoclus_CENTER_Y = 0;
   topoclus_CENTER_Z = 0;
   topoclus_CENTER_MAG = 0;
   topoclus_CENTER_LAMBDA = 0;
   topoclus_SECOND_R = 0;
   topoclus_SECOND_LAMBDA = 0;
   topoclus_LATERAL = 0;
   topoclus_LONGITUDINAL = 0;
   topoclus_ENG_FRAC_EM = 0;
   topoclus_ENG_FRAC_MAX = 0;
   topoclus_ENG_FRAC_CORE = 0;
   topoclus_FIRST_ENG_DENS = 0;
   topoclus_SECOND_ENG_DENS = 0;
   topoclus_PTD = 0;
   topoclus_EM_PROBABILITY = 0;
   topoclus_CELL_SIGNIFICANCE = 0;
   topoclus_CELL_SIG_SAMPLING = 0;
   topoclus_ISOLATION = 0;
   topoclus_MASS = 0;
   ttowfine_lcw_e = 0;
   ttowfine_lcw_pt = 0;
   ttowfine_lcw_eta = 0;
   ttowfine_lcw_phi = 0;
   ttowfine_em_e = 0;
   ttowfine_em_pt = 0;
   ttowfine_em_eta = 0;
   ttowfine_em_phi = 0;
   ttowfine_em_time = 0;
   ttowfine_em_sign = 0;
   ttowfine_em_epos = 0;
   ttowfine_FIRST_ETA = 0;
   ttowfine_FIRST_PHI = 0;
   ttowfine_DELTA_THETA = 0;
   ttowfine_DELTA_PHI = 0;
   ttowfine_DELTA_ALPHA = 0;
   ttowfine_CENTER_X = 0;
   ttowfine_CENTER_Y = 0;
   ttowfine_CENTER_Z = 0;
   ttowfine_CENTER_MAG = 0;
   ttowfine_CENTER_LAMBDA = 0;
   ttowfine_SECOND_R = 0;
   ttowfine_SECOND_LAMBDA = 0;
   ttowfine_LATERAL = 0;
   ttowfine_LONGITUDINAL = 0;
   ttowfine_ENG_FRAC_EM = 0;
   ttowfine_ENG_FRAC_MAX = 0;
   ttowfine_ENG_FRAC_CORE = 0;
   ttowfine_FIRST_ENG_DENS = 0;
   ttowfine_SECOND_ENG_DENS = 0;
   ttowfine_PTD = 0;
   ttowfine_EM_PROBABILITY = 0;
   ttowfine_CELL_SIGNIFICANCE = 0;
   ttowfine_CELL_SIG_SAMPLING = 0;
   ttowfine_ISOLATION = 0;
   ttowfine_MASS = 0;
   ttownorm_lcw_e = 0;
   ttownorm_lcw_pt = 0;
   ttownorm_lcw_eta = 0;
   ttownorm_lcw_phi = 0;
   ttownorm_em_e = 0;
   ttownorm_em_pt = 0;
   ttownorm_em_eta = 0;
   ttownorm_em_phi = 0;
   ttownorm_em_time = 0;
   ttownorm_em_sign = 0;
   ttownorm_em_epos = 0;
   ttownorm_FIRST_ETA = 0;
   ttownorm_FIRST_PHI = 0;
   ttownorm_DELTA_THETA = 0;
   ttownorm_DELTA_PHI = 0;
   ttownorm_DELTA_ALPHA = 0;
   ttownorm_CENTER_X = 0;
   ttownorm_CENTER_Y = 0;
   ttownorm_CENTER_Z = 0;
   ttownorm_CENTER_MAG = 0;
   ttownorm_CENTER_LAMBDA = 0;
   ttownorm_SECOND_R = 0;
   ttownorm_SECOND_LAMBDA = 0;
   ttownorm_LATERAL = 0;
   ttownorm_LONGITUDINAL = 0;
   ttownorm_ENG_FRAC_EM = 0;
   ttownorm_ENG_FRAC_MAX = 0;
   ttownorm_ENG_FRAC_CORE = 0;
   ttownorm_FIRST_ENG_DENS = 0;
   ttownorm_SECOND_ENG_DENS = 0;
   ttownorm_PTD = 0;
   ttownorm_EM_PROBABILITY = 0;
   ttownorm_CELL_SIGNIFICANCE = 0;
   ttownorm_CELL_SIG_SAMPLING = 0;
   ttownorm_ISOLATION = 0;
   ttownorm_MASS = 0;
   truthlept_pdg = 0;
   truthlept_status = 0;
   truthlept_charge = 0;
   truthlept_barcode = 0;
   truthlept_vtxflag = 0;
   truthlept_vtxid = 0;
   truthlept_e = 0;
   truthlept_pt = 0;
   truthlept_px = 0;
   truthlept_py = 0;
   truthlept_pz = 0;
   truthlept_m = 0;
   truthhadr_pdg = 0;
   truthhadr_status = 0;
   truthhadr_charge = 0;
   truthhadr_barcode = 0;
   truthhadr_vtxflag = 0;
   truthhadr_vtxid = 0;
   truthhadr_e = 0;
   truthhadr_pt = 0;
   truthhadr_px = 0;
   truthhadr_py = 0;
   truthhadr_pz = 0;
   truthhadr_m = 0;
   truthjet_e = 0;
   truthjet_pt = 0;
   truthjet_px = 0;
   truthjet_py = 0;
   truthjet_pz = 0;
   truthjet_rap = 0;
   truthjet_eta = 0;
   truthjet_phi = 0;
   truthjet_mass = 0;
   truthjet_comp = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event_rho_em", &event_rho_em, &b_event_rho_em);
   fChain->SetBranchAddress("event_sigma_em", &event_sigma_em, &b_event_sigma_em);
   fChain->SetBranchAddress("event_area_em", &event_area_em, &b_event_area_em);
   fChain->SetBranchAddress("event_rho_lcw", &event_rho_lcw, &b_event_rho_lcw);
   fChain->SetBranchAddress("event_sigma_lcw", &event_sigma_lcw, &b_event_sigma_lcw);
   fChain->SetBranchAddress("event_area_lcw", &event_area_lcw, &b_event_area_lcw);
   fChain->SetBranchAddress("event_mu", &event_mu, &b_event_mu);
   fChain->SetBranchAddress("event_npv", &event_npv, &b_event_npv);
   fChain->SetBranchAddress("event_weight", &event_weight, &b_event_weight);
   fChain->SetBranchAddress("recovx_x", &recovx_x, &b_recovx_x);
   fChain->SetBranchAddress("recovx_y", &recovx_y, &b_recovx_y);
   fChain->SetBranchAddress("recovx_z", &recovx_z, &b_recovx_z);
   fChain->SetBranchAddress("recovx_sumPt", &recovx_sumPt, &b_recovx_sumPt);
   fChain->SetBranchAddress("recovx_index", &recovx_index, &b_recovx_index);
   fChain->SetBranchAddress("truthvx_x", &truthvx_x, &b_truthvx_x);
   fChain->SetBranchAddress("truthvx_y", &truthvx_y, &b_truthvx_y);
   fChain->SetBranchAddress("truthvx_z", &truthvx_z, &b_truthvx_z);
   fChain->SetBranchAddress("truthvx_t", &truthvx_t, &b_truthvx_t);
   fChain->SetBranchAddress("truthvx_index", &truthvx_index, &b_truthvx_index);
   fChain->SetBranchAddress("topoclus_lcw_e", &topoclus_lcw_e, &b_topoclus_lcw_e);
   fChain->SetBranchAddress("topoclus_lcw_pt", &topoclus_lcw_pt, &b_topoclus_lcw_pt);
   fChain->SetBranchAddress("topoclus_lcw_eta", &topoclus_lcw_eta, &b_topoclus_lcw_eta);
   fChain->SetBranchAddress("topoclus_lcw_phi", &topoclus_lcw_phi, &b_topoclus_lcw_phi);
   fChain->SetBranchAddress("topoclus_em_e", &topoclus_em_e, &b_topoclus_em_e);
   fChain->SetBranchAddress("topoclus_em_pt", &topoclus_em_pt, &b_topoclus_em_pt);
   fChain->SetBranchAddress("topoclus_em_eta", &topoclus_em_eta, &b_topoclus_em_eta);
   fChain->SetBranchAddress("topoclus_em_phi", &topoclus_em_phi, &b_topoclus_em_phi);
   fChain->SetBranchAddress("topoclus_em_time", &topoclus_em_time, &b_topoclus_em_time);
   fChain->SetBranchAddress("topoclus_em_sign", &topoclus_em_sign, &b_topoclus_em_sign);
   fChain->SetBranchAddress("topoclus_em_epos", &topoclus_em_epos, &b_topoclus_em_epos);
   fChain->SetBranchAddress("topoclus_FIRST_ETA", &topoclus_FIRST_ETA, &b_topoclus_FIRST_ETA);
   fChain->SetBranchAddress("topoclus_FIRST_PHI", &topoclus_FIRST_PHI, &b_topoclus_FIRST_PHI);
   fChain->SetBranchAddress("topoclus_DELTA_THETA", &topoclus_DELTA_THETA, &b_topoclus_DELTA_THETA);
   fChain->SetBranchAddress("topoclus_DELTA_PHI", &topoclus_DELTA_PHI, &b_topoclus_DELTA_PHI);
   fChain->SetBranchAddress("topoclus_DELTA_ALPHA", &topoclus_DELTA_ALPHA, &b_topoclus_DELTA_ALPHA);
   fChain->SetBranchAddress("topoclus_CENTER_X", &topoclus_CENTER_X, &b_topoclus_CENTER_X);
   fChain->SetBranchAddress("topoclus_CENTER_Y", &topoclus_CENTER_Y, &b_topoclus_CENTER_Y);
   fChain->SetBranchAddress("topoclus_CENTER_Z", &topoclus_CENTER_Z, &b_topoclus_CENTER_Z);
   fChain->SetBranchAddress("topoclus_CENTER_MAG", &topoclus_CENTER_MAG, &b_topoclus_CENTER_MAG);
   fChain->SetBranchAddress("topoclus_CENTER_LAMBDA", &topoclus_CENTER_LAMBDA, &b_topoclus_CENTER_LAMBDA);
   fChain->SetBranchAddress("topoclus_SECOND_R", &topoclus_SECOND_R, &b_topoclus_SECOND_R);
   fChain->SetBranchAddress("topoclus_SECOND_LAMBDA", &topoclus_SECOND_LAMBDA, &b_topoclus_SECOND_LAMBDA);
   fChain->SetBranchAddress("topoclus_LATERAL", &topoclus_LATERAL, &b_topoclus_LATERAL);
   fChain->SetBranchAddress("topoclus_LONGITUDINAL", &topoclus_LONGITUDINAL, &b_topoclus_LONGITUDINAL);
   fChain->SetBranchAddress("topoclus_ENG_FRAC_EM", &topoclus_ENG_FRAC_EM, &b_topoclus_ENG_FRAC_EM);
   fChain->SetBranchAddress("topoclus_ENG_FRAC_MAX", &topoclus_ENG_FRAC_MAX, &b_topoclus_ENG_FRAC_MAX);
   fChain->SetBranchAddress("topoclus_ENG_FRAC_CORE", &topoclus_ENG_FRAC_CORE, &b_topoclus_ENG_FRAC_CORE);
   fChain->SetBranchAddress("topoclus_FIRST_ENG_DENS", &topoclus_FIRST_ENG_DENS, &b_topoclus_FIRST_ENG_DENS);
   fChain->SetBranchAddress("topoclus_SECOND_ENG_DENS", &topoclus_SECOND_ENG_DENS, &b_topoclus_SECOND_ENG_DENS);
   fChain->SetBranchAddress("topoclus_PTD", &topoclus_PTD, &b_topoclus_PTD);
   fChain->SetBranchAddress("topoclus_EM_PROBABILITY", &topoclus_EM_PROBABILITY, &b_topoclus_EM_PROBABILITY);
   fChain->SetBranchAddress("topoclus_CELL_SIGNIFICANCE", &topoclus_CELL_SIGNIFICANCE, &b_topoclus_CELL_SIGNIFICANCE);
   fChain->SetBranchAddress("topoclus_CELL_SIG_SAMPLING", &topoclus_CELL_SIG_SAMPLING, &b_topoclus_CELL_SIG_SAMPLING);
   fChain->SetBranchAddress("topoclus_ISOLATION", &topoclus_ISOLATION, &b_topoclus_ISOLATION);
   fChain->SetBranchAddress("topoclus_MASS", &topoclus_MASS, &b_topoclus_MASS);
   fChain->SetBranchAddress("ttowfine_lcw_e", &ttowfine_lcw_e, &b_ttowfine_lcw_e);
   fChain->SetBranchAddress("ttowfine_lcw_pt", &ttowfine_lcw_pt, &b_ttowfine_lcw_pt);
   fChain->SetBranchAddress("ttowfine_lcw_eta", &ttowfine_lcw_eta, &b_ttowfine_lcw_eta);
   fChain->SetBranchAddress("ttowfine_lcw_phi", &ttowfine_lcw_phi, &b_ttowfine_lcw_phi);
   fChain->SetBranchAddress("ttowfine_em_e", &ttowfine_em_e, &b_ttowfine_em_e);
   fChain->SetBranchAddress("ttowfine_em_pt", &ttowfine_em_pt, &b_ttowfine_em_pt);
   fChain->SetBranchAddress("ttowfine_em_eta", &ttowfine_em_eta, &b_ttowfine_em_eta);
   fChain->SetBranchAddress("ttowfine_em_phi", &ttowfine_em_phi, &b_ttowfine_em_phi);
   fChain->SetBranchAddress("ttowfine_em_time", &ttowfine_em_time, &b_ttowfine_em_time);
   fChain->SetBranchAddress("ttowfine_em_sign", &ttowfine_em_sign, &b_ttowfine_em_sign);
   fChain->SetBranchAddress("ttowfine_em_epos", &ttowfine_em_epos, &b_ttowfine_em_epos);
   fChain->SetBranchAddress("ttowfine_FIRST_ETA", &ttowfine_FIRST_ETA, &b_ttowfine_FIRST_ETA);
   fChain->SetBranchAddress("ttowfine_FIRST_PHI", &ttowfine_FIRST_PHI, &b_ttowfine_FIRST_PHI);
   fChain->SetBranchAddress("ttowfine_DELTA_THETA", &ttowfine_DELTA_THETA, &b_ttowfine_DELTA_THETA);
   fChain->SetBranchAddress("ttowfine_DELTA_PHI", &ttowfine_DELTA_PHI, &b_ttowfine_DELTA_PHI);
   fChain->SetBranchAddress("ttowfine_DELTA_ALPHA", &ttowfine_DELTA_ALPHA, &b_ttowfine_DELTA_ALPHA);
   fChain->SetBranchAddress("ttowfine_CENTER_X", &ttowfine_CENTER_X, &b_ttowfine_CENTER_X);
   fChain->SetBranchAddress("ttowfine_CENTER_Y", &ttowfine_CENTER_Y, &b_ttowfine_CENTER_Y);
   fChain->SetBranchAddress("ttowfine_CENTER_Z", &ttowfine_CENTER_Z, &b_ttowfine_CENTER_Z);
   fChain->SetBranchAddress("ttowfine_CENTER_MAG", &ttowfine_CENTER_MAG, &b_ttowfine_CENTER_MAG);
   fChain->SetBranchAddress("ttowfine_CENTER_LAMBDA", &ttowfine_CENTER_LAMBDA, &b_ttowfine_CENTER_LAMBDA);
   fChain->SetBranchAddress("ttowfine_SECOND_R", &ttowfine_SECOND_R, &b_ttowfine_SECOND_R);
   fChain->SetBranchAddress("ttowfine_SECOND_LAMBDA", &ttowfine_SECOND_LAMBDA, &b_ttowfine_SECOND_LAMBDA);
   fChain->SetBranchAddress("ttowfine_LATERAL", &ttowfine_LATERAL, &b_ttowfine_LATERAL);
   fChain->SetBranchAddress("ttowfine_LONGITUDINAL", &ttowfine_LONGITUDINAL, &b_ttowfine_LONGITUDINAL);
   fChain->SetBranchAddress("ttowfine_ENG_FRAC_EM", &ttowfine_ENG_FRAC_EM, &b_ttowfine_ENG_FRAC_EM);
   fChain->SetBranchAddress("ttowfine_ENG_FRAC_MAX", &ttowfine_ENG_FRAC_MAX, &b_ttowfine_ENG_FRAC_MAX);
   fChain->SetBranchAddress("ttowfine_ENG_FRAC_CORE", &ttowfine_ENG_FRAC_CORE, &b_ttowfine_ENG_FRAC_CORE);
   fChain->SetBranchAddress("ttowfine_FIRST_ENG_DENS", &ttowfine_FIRST_ENG_DENS, &b_ttowfine_FIRST_ENG_DENS);
   fChain->SetBranchAddress("ttowfine_SECOND_ENG_DENS", &ttowfine_SECOND_ENG_DENS, &b_ttowfine_SECOND_ENG_DENS);
   fChain->SetBranchAddress("ttowfine_PTD", &ttowfine_PTD, &b_ttowfine_PTD);
   fChain->SetBranchAddress("ttowfine_EM_PROBABILITY", &ttowfine_EM_PROBABILITY, &b_ttowfine_EM_PROBABILITY);
   fChain->SetBranchAddress("ttowfine_CELL_SIGNIFICANCE", &ttowfine_CELL_SIGNIFICANCE, &b_ttowfine_CELL_SIGNIFICANCE);
   fChain->SetBranchAddress("ttowfine_CELL_SIG_SAMPLING", &ttowfine_CELL_SIG_SAMPLING, &b_ttowfine_CELL_SIG_SAMPLING);
   fChain->SetBranchAddress("ttowfine_ISOLATION", &ttowfine_ISOLATION, &b_ttowfine_ISOLATION);
   fChain->SetBranchAddress("ttowfine_MASS", &ttowfine_MASS, &b_ttowfine_MASS);
   fChain->SetBranchAddress("ttownorm_lcw_e", &ttownorm_lcw_e, &b_ttownorm_lcw_e);
   fChain->SetBranchAddress("ttownorm_lcw_pt", &ttownorm_lcw_pt, &b_ttownorm_lcw_pt);
   fChain->SetBranchAddress("ttownorm_lcw_eta", &ttownorm_lcw_eta, &b_ttownorm_lcw_eta);
   fChain->SetBranchAddress("ttownorm_lcw_phi", &ttownorm_lcw_phi, &b_ttownorm_lcw_phi);
   fChain->SetBranchAddress("ttownorm_em_e", &ttownorm_em_e, &b_ttownorm_em_e);
   fChain->SetBranchAddress("ttownorm_em_pt", &ttownorm_em_pt, &b_ttownorm_em_pt);
   fChain->SetBranchAddress("ttownorm_em_eta", &ttownorm_em_eta, &b_ttownorm_em_eta);
   fChain->SetBranchAddress("ttownorm_em_phi", &ttownorm_em_phi, &b_ttownorm_em_phi);
   fChain->SetBranchAddress("ttownorm_em_time", &ttownorm_em_time, &b_ttownorm_em_time);
   fChain->SetBranchAddress("ttownorm_em_sign", &ttownorm_em_sign, &b_ttownorm_em_sign);
   fChain->SetBranchAddress("ttownorm_em_epos", &ttownorm_em_epos, &b_ttownorm_em_epos);
   fChain->SetBranchAddress("ttownorm_FIRST_ETA", &ttownorm_FIRST_ETA, &b_ttownorm_FIRST_ETA);
   fChain->SetBranchAddress("ttownorm_FIRST_PHI", &ttownorm_FIRST_PHI, &b_ttownorm_FIRST_PHI);
   fChain->SetBranchAddress("ttownorm_DELTA_THETA", &ttownorm_DELTA_THETA, &b_ttownorm_DELTA_THETA);
   fChain->SetBranchAddress("ttownorm_DELTA_PHI", &ttownorm_DELTA_PHI, &b_ttownorm_DELTA_PHI);
   fChain->SetBranchAddress("ttownorm_DELTA_ALPHA", &ttownorm_DELTA_ALPHA, &b_ttownorm_DELTA_ALPHA);
   fChain->SetBranchAddress("ttownorm_CENTER_X", &ttownorm_CENTER_X, &b_ttownorm_CENTER_X);
   fChain->SetBranchAddress("ttownorm_CENTER_Y", &ttownorm_CENTER_Y, &b_ttownorm_CENTER_Y);
   fChain->SetBranchAddress("ttownorm_CENTER_Z", &ttownorm_CENTER_Z, &b_ttownorm_CENTER_Z);
   fChain->SetBranchAddress("ttownorm_CENTER_MAG", &ttownorm_CENTER_MAG, &b_ttownorm_CENTER_MAG);
   fChain->SetBranchAddress("ttownorm_CENTER_LAMBDA", &ttownorm_CENTER_LAMBDA, &b_ttownorm_CENTER_LAMBDA);
   fChain->SetBranchAddress("ttownorm_SECOND_R", &ttownorm_SECOND_R, &b_ttownorm_SECOND_R);
   fChain->SetBranchAddress("ttownorm_SECOND_LAMBDA", &ttownorm_SECOND_LAMBDA, &b_ttownorm_SECOND_LAMBDA);
   fChain->SetBranchAddress("ttownorm_LATERAL", &ttownorm_LATERAL, &b_ttownorm_LATERAL);
   fChain->SetBranchAddress("ttownorm_LONGITUDINAL", &ttownorm_LONGITUDINAL, &b_ttownorm_LONGITUDINAL);
   fChain->SetBranchAddress("ttownorm_ENG_FRAC_EM", &ttownorm_ENG_FRAC_EM, &b_ttownorm_ENG_FRAC_EM);
   fChain->SetBranchAddress("ttownorm_ENG_FRAC_MAX", &ttownorm_ENG_FRAC_MAX, &b_ttownorm_ENG_FRAC_MAX);
   fChain->SetBranchAddress("ttownorm_ENG_FRAC_CORE", &ttownorm_ENG_FRAC_CORE, &b_ttownorm_ENG_FRAC_CORE);
   fChain->SetBranchAddress("ttownorm_FIRST_ENG_DENS", &ttownorm_FIRST_ENG_DENS, &b_ttownorm_FIRST_ENG_DENS);
   fChain->SetBranchAddress("ttownorm_SECOND_ENG_DENS", &ttownorm_SECOND_ENG_DENS, &b_ttownorm_SECOND_ENG_DENS);
   fChain->SetBranchAddress("ttownorm_PTD", &ttownorm_PTD, &b_ttownorm_PTD);
   fChain->SetBranchAddress("ttownorm_EM_PROBABILITY", &ttownorm_EM_PROBABILITY, &b_ttownorm_EM_PROBABILITY);
   fChain->SetBranchAddress("ttownorm_CELL_SIGNIFICANCE", &ttownorm_CELL_SIGNIFICANCE, &b_ttownorm_CELL_SIGNIFICANCE);
   fChain->SetBranchAddress("ttownorm_CELL_SIG_SAMPLING", &ttownorm_CELL_SIG_SAMPLING, &b_ttownorm_CELL_SIG_SAMPLING);
   fChain->SetBranchAddress("ttownorm_ISOLATION", &ttownorm_ISOLATION, &b_ttownorm_ISOLATION);
   fChain->SetBranchAddress("ttownorm_MASS", &ttownorm_MASS, &b_ttownorm_MASS);
   fChain->SetBranchAddress("truthlept_pdg", &truthlept_pdg, &b_truthlept_pdg);
   fChain->SetBranchAddress("truthlept_status", &truthlept_status, &b_truthlept_status);
   fChain->SetBranchAddress("truthlept_charge", &truthlept_charge, &b_truthlept_charge);
   fChain->SetBranchAddress("truthlept_barcode", &truthlept_barcode, &b_truthlept_barcode);
   fChain->SetBranchAddress("truthlept_vtxflag", &truthlept_vtxflag, &b_truthlept_vtxflag);
   fChain->SetBranchAddress("truthlept_vtxid", &truthlept_vtxid, &b_truthlept_vtxid);
   fChain->SetBranchAddress("truthlept_e", &truthlept_e, &b_truthlept_e);
   fChain->SetBranchAddress("truthlept_pt", &truthlept_pt, &b_truthlept_pt);
   fChain->SetBranchAddress("truthlept_px", &truthlept_px, &b_truthlept_px);
   fChain->SetBranchAddress("truthlept_py", &truthlept_py, &b_truthlept_py);
   fChain->SetBranchAddress("truthlept_pz", &truthlept_pz, &b_truthlept_pz);
   fChain->SetBranchAddress("truthlept_m", &truthlept_m, &b_truthlept_m);
   fChain->SetBranchAddress("truthhadr_pdg", &truthhadr_pdg, &b_truthhadr_pdg);
   fChain->SetBranchAddress("truthhadr_status", &truthhadr_status, &b_truthhadr_status);
   fChain->SetBranchAddress("truthhadr_charge", &truthhadr_charge, &b_truthhadr_charge);
   fChain->SetBranchAddress("truthhadr_barcode", &truthhadr_barcode, &b_truthhadr_barcode);
   fChain->SetBranchAddress("truthhadr_vtxflag", &truthhadr_vtxflag, &b_truthhadr_vtxflag);
   fChain->SetBranchAddress("truthhadr_vtxid", &truthhadr_vtxid, &b_truthhadr_vtxid);
   fChain->SetBranchAddress("truthhadr_e", &truthhadr_e, &b_truthhadr_e);
   fChain->SetBranchAddress("truthhadr_pt", &truthhadr_pt, &b_truthhadr_pt);
   fChain->SetBranchAddress("truthhadr_px", &truthhadr_px, &b_truthhadr_px);
   fChain->SetBranchAddress("truthhadr_py", &truthhadr_py, &b_truthhadr_py);
   fChain->SetBranchAddress("truthhadr_pz", &truthhadr_pz, &b_truthhadr_pz);
   fChain->SetBranchAddress("truthhadr_m", &truthhadr_m, &b_truthhadr_m);
   fChain->SetBranchAddress("truthjet_e", &truthjet_e, &b_truthjet_e);
   fChain->SetBranchAddress("truthjet_pt", &truthjet_pt, &b_truthjet_pt);
   fChain->SetBranchAddress("truthjet_px", &truthjet_px, &b_truthjet_px);
   fChain->SetBranchAddress("truthjet_py", &truthjet_py, &b_truthjet_py);
   fChain->SetBranchAddress("truthjet_pz", &truthjet_pz, &b_truthjet_pz);
   fChain->SetBranchAddress("truthjet_rap", &truthjet_rap, &b_truthjet_rap);
   fChain->SetBranchAddress("truthjet_eta", &truthjet_eta, &b_truthjet_eta);
   fChain->SetBranchAddress("truthjet_phi", &truthjet_phi, &b_truthjet_phi);
   fChain->SetBranchAddress("truthjet_mass", &truthjet_mass, &b_truthjet_mass);
   fChain->SetBranchAddress("truthjet_comp", &truthjet_comp, &b_truthjet_comp);
   Notify();
}

Bool_t JetAnalysisExample::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void JetAnalysisExample::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t JetAnalysisExample::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef JetAnalysisExample_cxx
