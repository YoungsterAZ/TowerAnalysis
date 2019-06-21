#define JetAnalysisExample_cxx
#include "JetAnalysisExample.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>

#include <vector>

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"

#include "AnalysisHelper.h"


void JetAnalysisExample::Loop()
{
  //////////////
  // Preamble //
  //////////////

  // consistency check
  if (fChain == 0) return;

  // total number of entries
  Long64_t nentries = fChain->GetEntries();

  /////////////////////
  // Book histograms //
  /////////////////////

  //--------------------------------------------------------------------------------
  // There are three levels of simlar distributions:
  //
  // [Initial] plot quantities from events passing the very basic initial selection
  //           (in this code: histogram names ending with <selection_tag> = _ini) 
  // [Accept]  plot quantities from events passing a refined selection
  //           (in this code: histogram names ending with <selection_tag> = _acc)
  // [Final]   plot quantities from events passing the final selection
  //           (in this code: histogram names ending with <selection_tag> = _fin)
  //
  // The code is set up to store the histograms in the output file in a directory
  // structure following the event selections:
  //
  // Initial/<histogram_name>
  // Accept/<histogram_name>
  // Final/<histogram_name>
  //
  // where <histogram_name> does not include any of the _ini, _acc, or _fin tags.
  // Histogram names are constructed using "tjet" for truth jets, "rjet" for reco
  // jets, and "mjet" for matched reco jets. Variables holding pointers to 
  // 1-dimensional histograms start with "h_", while 2-dimensional distributions 
  // start with "d_". The general convention used in this code is (e.g. for a 
  // 1-dimensional histograms):
  //
  // h_<object>_<var>_<selection_tag>  (variable name in code)
  // <selection_name>/h_<object>_<var> (histogram on file)
  //
  // Additional qualifiers may be added to the histogram name, like "lead" for
  // leading jet and "subl" for subleading jet.
  //
  //  <object> | object type | comments
  // ==========*=============*====================================================
  // tjet      |    jet      | truth (particle) jet 
  // rjet      |    jet      | reconstructed (calorimeter) jet
  // mjet      |    jet      | matched reconstructed jet 
  // ----------+-------------+----------------------------------------------------
  // evnt      |   event     | whole event variables like invariant mass etc.
  //    
  //---------------------------------------------------------------------------------

  // binning number of jets/event
  int njBin(200); double njMin(-0.5); double njMax(njMin+njBin);

  // binning in pT [GeV]
  int ptBin(200); double ptMin(0.); double ptMax(2000.);

  // binning in rapidity/pseudorapidity (central bin has center at 0)
  int rapBin(101); double rapMin(-5.05); double rapMax(5.05);

  // binning in phi (central bin has center at 0);
  int phiBin(65); double phiMin(-3.25); double phiMax(3.35);

  // binning in invariant mass
  int imBin(200); double imMin(0.); double imMax(4000.);

  // low jet multiplicity binning
  int njlBin(10); double njlMin(-0.5); double njlMax(9.5);

  // jet constituent binning
  int njcBin(500); double njcMin(-0.5); double njcMax(njcMin+njcBin);

  // rapidity gap binning
  int gapBin((rapBin-1)/2); double gapMin(0.); double gapMax(rapMax-0.05);

  // delta R binning
  int drBin(100); double drMin(0.); double drMax(2.);

  // [Initial] >>------------------------------------------------------------------------------------------------------------------------------------------------------------
  // inclusive truth jets
  TH1D* h_tjet_num_ini      = AH::book1D<TH1D>("Initial/h_tjet_num","Number of truth jets",njBin,njMin,njMax,   "N_{jet}^{truth}",    "Entries");
  TH1D* h_tjet_pt_ini       = AH::book1D<TH1D>("Initial/h_tjet_pt", "Truth jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{truth} [GeV]","Entries");
  TH1D* h_tjet_rap_ini      = AH::book1D<TH1D>("Initial/h_tjet_rap","Truth jet rapidity",  rapBin,rapMin,rapMax,"y^{truth}",          "Entries");
  TH1D* h_tjet_phi_ini      = AH::book1D<TH1D>("Initial/h_tjet_phi","Truth jet #phi",      phiBin,phiMin,phiMax,"#phi^{truth}",       "Entries");
  TH2D* d_tjet_num_rap_ini  = AH::book2D<TH2D>("Initial/d_tjet_num_eta","N_{const}^{truth jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y^{truth}","N_{const}^{truth}");
  // leading truth jet
  TH1D* h_tjet_pt_lead_ini       = AH::book1D<TH1D>("Initial/h_tjet_pt_lead", "Leading truth jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{truth,lead} [GeV]","Entries");
  TH1D* h_tjet_rap_lead_ini      = AH::book1D<TH1D>("Initial/h_tjet_rap_lead","Leading truth jet rapidity",rapBin,rapMin,rapMax,"y^{truth,lead}",          "Entries");
  TH1D* h_tjet_phi_lead_ini      = AH::book1D<TH1D>("Initial/h_tjet_phi_lead","Leading truth jet #phi",    phiBin,phiMin,phiMax,"#phi^{truth,lead}",       "Entries");
  // subleading truth jet
  TH1D* h_tjet_pt_subl_ini  = AH::book1D<TH1D>("Initial/h_tjet_pt_subl","Subleading truth jet p_{T}",    ptBin,ptMin,ptMax,   "p_{T}^{truth,subl} [GeV]","Entries");
  TH1D* h_tjet_rap_subl_ini = AH::book1D<TH1D>("Initial/h_tjet_rap_subl","Subleading truth jet rapidity",rapBin,rapMin,rapMax,"y^{truth,subl}",          "Entries");
  TH1D* h_tjet_phi_subl_ini = AH::book1D<TH1D>("Initial/h_tjet_phi_subl","Subleading truth jet #phi",    phiBin,phiMin,phiMax,"#phi^{truth,subl}",       "Entries");
  // inclusive reco jets
  TH1D* h_rjet_num_ini = AH::book1D<TH1D>("Initial/h_rjet_num","Number of reco jets",njBin,njMin,njMax,   "N_{jet}^{reco}",    "Entries");
  TH1D* h_rjet_pt_ini  = AH::book1D<TH1D>("Initial/h_rjet_pt", "Reco jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{reco} [GeV]","Entries");
  TH1D* h_rjet_rap_ini = AH::book1D<TH1D>("Initial/h_rjet_rap","Reco jet rapidity",  rapBin,rapMin,rapMax,"y^{reco}",          "Entries");
  TH1D* h_rjet_phi_ini = AH::book1D<TH1D>("Initial/h_rjet_phi","Reco jet #phi",      phiBin,phiMin,phiMax,"#phi^{reco}",       "Entries");
  TH2D* d_rjet_num_rap_ini  = AH::book2D<TH2D>("Initial/d_rjet_num_eta","N_{const}^{reco jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y^{reco}","N_{const}^{reco}");
  // leading reco jet
  TH1D* h_rjet_pt_lead_ini  = AH::book1D<TH1D>("Initial/h_rjet_pt_lead", "Leading reco jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{reco,lead} [GeV]","Entries");
  TH1D* h_rjet_rap_lead_ini = AH::book1D<TH1D>("Initial/h_rjet_rap_lead","Leading reco jet rapidity",rapBin,rapMin,rapMax,"y^{reco,lead}",          "Entries");
  TH1D* h_rjet_phi_lead_ini = AH::book1D<TH1D>("Initial/h_rjet_phi_lead","Leading reco jet #phi",    phiBin,phiMin,phiMax,"#phi^{reco,lead}",       "Entries");
  // subleading reco jet
  TH1D* h_rjet_pt_subl_ini  = AH::book1D<TH1D>("Initial/h_rjet_pt_subl", "Subleading reco jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{reco,subl} [GeV]","Entries");
  TH1D* h_rjet_rap_subl_ini = AH::book1D<TH1D>("Initial/h_rjet_rap_subl","Subleading reco jet rapidity",rapBin,rapMin,rapMax,"y^{reco,subl}",          "Entries");
  TH1D* h_rjet_phi_subl_ini = AH::book1D<TH1D>("Initial/h_rjet_phi_subl","Subleading reco jet #phi",    phiBin,phiMin,phiMax,"#phi^{reco,subl}",       "Entries");
  // event quantities
  TH1D* h_evnt_m_ini     = AH::book1D<TH1D>("Initial/h_evnt_m",  "Truth jet invariant mass M_{inv}",imBin,imMin,imMax,   "M_{inv}^{truth} [GeV]",           "Entries"); 
  TH1D* h_evnt_njm_ini   = AH::book1D<TH1D>("Initial/h_evnt_njm","N_{jet}^{truth} for M_{inv}",     njlBin,njlMin,njlMax,"N_{jet}^{truth} used for M_{inv}","Entries");
  TH1D* h_evnt_gap_ini   = AH::book1D<TH1D>("Initial/h_evnt_gap","Truth jet rapidity gap",          gapBin,gapMin,gapMax,"#Deltay^{truth}",                 "Entries");
  // [Initial] <<------------------------------------------------------------------------------------------------------------------------------------------------------------

  // [Accept] >>-------------------------------------------------------------------------------------------------------------------------------------------------------------
  // inclusive truth jets
  TH1D* h_tjet_num_acc = AH::book1D<TH1D>("Accept/h_tjet_num","Number of truth jets",njBin,njMin,njMax,   "N_{jet}^{truth}",    "Entries");
  TH1D* h_tjet_pt_acc  = AH::book1D<TH1D>("Accept/h_tjet_pt", "Truth jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{truth} [GeV]","Entries");
  TH1D* h_tjet_rap_acc = AH::book1D<TH1D>("Accept/h_tjet_rap","Truth jet rapidity",  rapBin,rapMin,rapMax,"y^{truth}",          "Entries");
  TH1D* h_tjet_phi_acc = AH::book1D<TH1D>("Accept/h_tjet_phi","Truth jet #phi",      phiBin,phiMin,phiMax,"#phi^{truth}",       "Entries");
  TH2D* d_tjet_num_rap_acc  = AH::book2D<TH2D>("Accept/d_tjet_num_eta","N_{const}^{truth jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y","N_{const}^{truth}");
  // leading truth jet
  TH1D* h_tjet_pt_lead_acc  = AH::book1D<TH1D>("Accept/h_tjet_pt_lead", "Leading truth jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{truth,lead} [GeV]","Entries");
  TH1D* h_tjet_rap_lead_acc = AH::book1D<TH1D>("Accept/h_tjet_rap_lead","Leading truth jet rapidity",rapBin,rapMin,rapMax,"y^{truth,lead}",          "Entries");
  TH1D* h_tjet_phi_lead_acc = AH::book1D<TH1D>("Accept/h_tjet_phi_lead","Leading truth jet #phi",    phiBin,phiMin,phiMax,"#phi^{truth,lead}",       "Entries");
  // subleading truth jet
  TH1D* h_tjet_pt_subl_acc  = AH::book1D<TH1D>("Accept/h_tjet_pt_subl","Subleading truth jet p_{T}",    ptBin,ptMin,ptMax,   "p_{T}^{truth,subl} [GeV]","Entries");
  TH1D* h_tjet_rap_subl_acc = AH::book1D<TH1D>("Accept/h_tjet_rap_subl","Subleading truth jet rapidity",rapBin,rapMin,rapMax,"y^{truth,subl}",          "Entries");
  TH1D* h_tjet_phi_subl_acc = AH::book1D<TH1D>("Accept/h_tjet_phi_subl","Subleading truth jet #phi",    phiBin,phiMin,phiMax,"#phi^{truth,subl}",       "Entries");
  // inclusive reco jets
  TH1D* h_rjet_num_acc = AH::book1D<TH1D>("Accept/h_rjet_num","Number of reco jets",njBin,njMin,njMax,   "N_{jet}^{reco}",    "Entries");
  TH1D* h_rjet_pt_acc  = AH::book1D<TH1D>("Accept/h_rjet_pt", "Reco jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{reco} [GeV]","Entries");
  TH1D* h_rjet_rap_acc = AH::book1D<TH1D>("Accept/h_rjet_rap","Reco jet rapidity",  rapBin,rapMin,rapMax,"y^{reco}",          "Entries");
  TH1D* h_rjet_phi_acc = AH::book1D<TH1D>("Accept/h_rjet_phi","Reco jet #phi",      phiBin,phiMin,phiMax,"#phi^{reco}",       "Entries");
  TH2D* d_rjet_num_rap_acc  = AH::book2D<TH2D>("Accept/d_rjet_num_eta","N_{const}^{reco jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y","N_{const}^{reco}");
  // leading reco jet
  TH1D* h_rjet_pt_lead_acc  = AH::book1D<TH1D>("Accept/h_rjet_pt_lead", "Leading reco jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{reco,lead} [GeV]","Entries");
  TH1D* h_rjet_rap_lead_acc = AH::book1D<TH1D>("Accept/h_rjet_rap_lead","Leading reco jet rapidity",rapBin,rapMin,rapMax,"y^{reco,lead}",          "Entries");
  TH1D* h_rjet_phi_lead_acc = AH::book1D<TH1D>("Accept/h_rjet_phi_lead","Leading reco jet #phi",    phiBin,phiMin,phiMax,"#phi^{reco,lead}",       "Entries");
  // subleading reco jet
  TH1D* h_rjet_pt_subl_acc  = AH::book1D<TH1D>("Accept/h_rjet_pt_subl", "Subleading reco jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{reco,subl} [GeV]","Entries");
  TH1D* h_rjet_rap_subl_acc = AH::book1D<TH1D>("Accept/h_rjet_rap_subl","Subleading reco jet rapidity",rapBin,rapMin,rapMax,"y^{reco,subl}",          "Entries");
  TH1D* h_rjet_phi_subl_acc = AH::book1D<TH1D>("Accept/h_rjet_phi_subl","Subleading reco jet #phi",    phiBin,phiMin,phiMax,"#phi^{reco,subl}",       "Entries");
  // inclusive matched reco jets
  TH1D* h_mjet_num_acc = AH::book1D<TH1D>("Accept/h_mjet_num","Number of reco jets",njBin,njMin,njMax,   "N_{jet}^{matched}",    "Entries");
  TH1D* h_mjet_pt_acc  = AH::book1D<TH1D>("Accept/h_mjet_pt", "Reco jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{matched} [GeV]","Entries");
  TH1D* h_mjet_rap_acc = AH::book1D<TH1D>("Accept/h_mjet_rap","Reco jet rapidity",  rapBin,rapMin,rapMax,"y^{matched}",          "Entries");
  TH1D* h_mjet_phi_acc = AH::book1D<TH1D>("Accept/h_mjet_phi","Reco jet #phi",      phiBin,phiMin,phiMax,"#phi^{matched}",       "Entries");
  TH1D* h_mjet_dr_acc  = AH::book1D<TH1D>("Accept/h_mjet_dr", "Matching radius",    drBin,drMin,drMax,   "#DeltaR_{match}",      "Entries");
  TH2D* d_mjet_num_rap_acc  = AH::book2D<TH2D>("Accept/d_mjet_num_rap","N_{const}^{matched jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y^{matched}","N_{const}^{matched}");
  // leading matched reco jet
  TH1D* h_mjet_pt_lead_acc  = AH::book1D<TH1D>("Accept/h_mjet_pt_lead", "Leading reco jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{matched,lead} [GeV]","Entries");
  TH1D* h_mjet_rap_lead_acc = AH::book1D<TH1D>("Accept/h_mjet_rap_lead","Leading reco jet rapidity",  rapBin,rapMin,rapMax,"y^{matched,lead}",          "Entries");
  TH1D* h_mjet_phi_lead_acc = AH::book1D<TH1D>("Accept/h_mjet_phi_lead","Leading reco jet #phi",      phiBin,phiMin,phiMax,"#phi^{matched,lead}",       "Entries");
  TH1D* h_mjet_dr_lead_acc  = AH::book1D<TH1D>("Accept/h_mjet_dr_lead", "Leading jet matching radius",drBin,drMin,drMax,   "#DeltaR_{match}^{lead}",    "Entries");
  // subleading matched reco jet
  TH1D* h_mjet_pt_subl_acc  = AH::book1D<TH1D>("Accept/h_mjet_pt_subl", "Subleading reco jet p_{T}",          ptBin,ptMin,ptMax,   "p_{T}^{reco,subl} [GeV]","Entries");
  TH1D* h_mjet_rap_subl_acc = AH::book1D<TH1D>("Accept/h_mjet_rap_subl","Subleading reco jet rapidity",       rapBin,rapMin,rapMax,"y^{reco,subl}",          "Entries");
  TH1D* h_mjet_phi_subl_acc = AH::book1D<TH1D>("Accept/h_mjet_phi_subl","Subleading reco jet #phi",           phiBin,phiMin,phiMax,"#phi^{reco,subl}",       "Entries");
  TH1D* h_mjet_dr_subl_acc  = AH::book1D<TH1D>("Accept/h_mjet_dr_subl", "Subleading reco jet matching radius",drBin,drMin,drMax,   "#DeltaR_{match}^{subl}", "Entries");
  // event quantities
  TH1D* h_evnt_m_acc     = AH::book1D<TH1D>("Accept/h_evnt_m",  "Truth jet invariant mass M_{inv}",imBin,imMin,imMax,   "M_{inv}^{truth} [GeV]",           "Entries"); 
  TH1D* h_evnt_njm_acc   = AH::book1D<TH1D>("Accept/h_evnt_njm","N_{jet}^{truth} for M_{inv}",     njlBin,njlMin,njlMax,"N_{jet}^{truth} used for M_{inv}","Entries");
  TH1D* h_evnt_gap_acc   = AH::book1D<TH1D>("Accept/h_evnt_gap","Truth jet rapidity gap",          gapBin,gapMin,gapMax,"#Deltay^{truth}",                 "Entries");
  // [Accept] <<-------------------------------------------------------------------------------------------------------------------------------------------------------------

  // [Final] >>--------------------------------------------------------------------------------------------------------------------------------------------------------------
  // inclusive truth jets
  TH1D* h_tjet_num_fin = AH::book1D<TH1D>("Final/h_tjet_num","Number of truth jets",njBin,njMin,njMax,   "N_{jet}^{truth}",    "Entries");
  TH1D* h_tjet_pt_fin  = AH::book1D<TH1D>("Final/h_tjet_pt", "Truth jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{truth} [GeV]","Entries");
  TH1D* h_tjet_rap_fin = AH::book1D<TH1D>("Final/h_tjet_rap","Truth jet rapidity",  rapBin,rapMin,rapMax,"y^{truth}",          "Entries");
  TH1D* h_tjet_phi_fin = AH::book1D<TH1D>("Final/h_tjet_phi","Truth jet #phi",      phiBin,phiMin,phiMax,"#phi^{truth}",       "Entries");
  TH2D* d_tjet_num_rap_fin  = AH::book2D<TH2D>("Final/d_tjet_num_eta","N_{const}^{truth jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y^{truth}","N_{const}^{truth}");
  // leading truth jet
  TH1D* h_tjet_pt_lead_fin  = AH::book1D<TH1D>("Final/h_tjet_pt_lead", "Leading truth jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{truth,lead} [GeV]","Entries");
  TH1D* h_tjet_rap_lead_fin = AH::book1D<TH1D>("Final/h_tjet_rap_lead","Leading truth jet rapidity",rapBin,rapMin,rapMax,"y^{truth,lead}",          "Entries");
  TH1D* h_tjet_phi_lead_fin = AH::book1D<TH1D>("Final/h_tjet_phi_lead","Leading truth jet #phi",    phiBin,phiMin,phiMax,"#phi^{truth,lead}",       "Entries");
  // subleading truth jet
  TH1D* h_tjet_pt_subl_fin  = AH::book1D<TH1D>("Final/h_tjet_pt_subl","Subleading truth jet p_{T}",    ptBin,ptMin,ptMax,   "p_{T}^{truth,subl} [GeV]","Entries");
  TH1D* h_tjet_rap_subl_fin = AH::book1D<TH1D>("Final/h_tjet_rap_subl","Subleading truth jet rapidity",rapBin,rapMin,rapMax,"y^{truth,subl}",          "Entries");
  TH1D* h_tjet_phi_subl_fin = AH::book1D<TH1D>("Final/h_tjet_phi_subl","Subleading truth jet #phi",    phiBin,phiMin,phiMax,"#phi^{truth,subl}",       "Entries");
  // inclusive reco jets
  TH1D* h_rjet_num_fin = AH::book1D<TH1D>("Final/h_rjet_num","Number of reco jets",njBin,njMin,njMax,   "N_{jet}^{reco}",    "Entries");
  TH1D* h_rjet_pt_fin  = AH::book1D<TH1D>("Final/h_rjet_pt", "Reco jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{reco} [GeV]","Entries");
  TH1D* h_rjet_rap_fin = AH::book1D<TH1D>("Final/h_rjet_rap","Reco jet rapidity",  rapBin,rapMin,rapMax,"y^{reco}",          "Entries");
  TH1D* h_rjet_phi_fin = AH::book1D<TH1D>("Final/h_rjet_phi","Reco jet #phi",      phiBin,phiMin,phiMax,"#phi^{reco}",       "Entries");
  TH2D* d_rjet_num_rap_fin  = AH::book2D<TH2D>("Final/d_rjet_num_eta","N_{const}^{reco jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y^{reco}","N_{const}^{reco}");
  // leading reco jet
  TH1D* h_rjet_pt_lead_fin  = AH::book1D<TH1D>("Final/h_rjet_pt_lead", "Leading reco jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{reco,lead} [GeV]","Entries");
  TH1D* h_rjet_rap_lead_fin = AH::book1D<TH1D>("Final/h_rjet_rap_lead","Leading reco jet rapidity",rapBin,rapMin,rapMax,"y^{reco,lead}",          "Entries");
  TH1D* h_rjet_phi_lead_fin = AH::book1D<TH1D>("Final/h_rjet_phi_lead","Leading reco jet #phi",    phiBin,phiMin,phiMax,"#phi^{reco,lead}",       "Entries");
  // subleading reco jet
  TH1D* h_rjet_pt_subl_fin  = AH::book1D<TH1D>("Final/h_rjet_pt_subl", "Subleading reco jet p_{T}",   ptBin,ptMin,ptMax,   "p_{T}^{reco,subl} [GeV]","Entries");
  TH1D* h_rjet_rap_subl_fin = AH::book1D<TH1D>("Final/h_rjet_rap_subl","Subleading reco jet rapidity",rapBin,rapMin,rapMax,"y^{reco,subl}",          "Entries");
  TH1D* h_rjet_phi_subl_fin = AH::book1D<TH1D>("Final/h_rjet_phi_subl","Subleading reco jet #phi",    phiBin,phiMin,phiMax,"#phi^{reco,subl}",       "Entries");
  // inclusive matched reco jets
  TH1D* h_mjet_num_fin = AH::book1D<TH1D>("Final/h_mjet_num","Number of matched reco jets",njBin,njMin,njMax,   "N_{jet}^{matched}",    "Entries");
  TH1D* h_mjet_pt_fin  = AH::book1D<TH1D>("Final/h_mjet_pt", "Matched reco jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{matched} [GeV]","Entries");
  TH1D* h_mjet_rap_fin = AH::book1D<TH1D>("Final/h_mjet_rap","Matched reco jet rapidity",  rapBin,rapMin,rapMax,"y^{matched}",          "Entries");
  TH1D* h_mjet_phi_fin = AH::book1D<TH1D>("Final/h_mjet_phi","Matched reco jet #phi",      phiBin,phiMin,phiMax,"#phi^{matched}",       "Entries");
  TH1D* h_mjet_dr_fin  = AH::book1D<TH1D>("Final/h_mjet_dr", "Matching radius",    drBin,drMin,drMax,   "#DeltaR_{match}",      "Entries");
  TH2D* d_mjet_num_rap_fin = AH::book2D<TH2D>("Final/d_mjet_num_eta","N_{const}^{matched jet} vs y",rapBin,rapMin,rapMax,njcBin,njcMin,njcMax,"y^{matched}","N_{const}^{matched}");
  TH2D* d_mjet_ptd_rap_fin = AH::book2D<TH2D>("Final/d_mjet_ptd_rap","PtD^{matched jet} vs y",rapBin,rapMin,rapMax,101,0.,1.01,"y^{matched}","p_{T}^{D}");
  TH2D* d_mjet_rptd_rap_fin = AH::book2D<TH2D>("Final/d_mjet_rptd_rap","PtD ratio^{matched jet} vs y",rapBin,rapMin,rapMax,201,0.,2.01,"y^{matched}","p_{T}^{D,jet}/p_{T}^{D,calo}");
  // leading matched reco jet
  TH1D* h_mjet_pt_lead_fin  = AH::book1D<TH1D>("Final/h_mjet_pt_lead", "Leading matched reco jet p_{T}",     ptBin,ptMin,ptMax,   "p_{T}^{matched,lead} [GeV]","Entries");
  TH1D* h_mjet_rap_lead_fin = AH::book1D<TH1D>("Final/h_mjet_rap_lead","Leading matched reco jet rapidity",  rapBin,rapMin,rapMax,"y^{matched,lead}",          "Entries");
  TH1D* h_mjet_phi_lead_fin = AH::book1D<TH1D>("Final/h_mjet_phi_lead","Leading matched reco jet #phi",      phiBin,phiMin,phiMax,"#phi^{matched,lead}",       "Entries");
  TH1D* h_mjet_dr_lead_fin  = AH::book1D<TH1D>("Final/h_mjet_dr_lead", "Leading jet matching radius",drBin,drMin,drMax,   "#DeltaR_{match}^{lead}",    "Entries");
  TH2D* d_mjet_pt_lead_tjet_fin  = AH::book2D<TH2D>("Final/d_mjet_pt_lead_tjet", "Leading matched truth jet p_T vs leading truth jet p_T",
					       ptBin,ptMin,ptMax,ptBin,ptMin,ptMax,"p_{T}^{truth,lead} [GeV]","p_{T}^{matched truth,lead} [GeV]");
  TH2D* d_mjet_rap_lead_tjet_fin  = AH::book2D<TH2D>("Final/d_mjet_rap_lead_tjet", "Leading matched truth jet y vs leading truth jet y",
						     rapBin,rapMin,rapMax,rapBin,rapMin,rapMax,"y^{truth,lead}","y^{matched truth,lead}");
  // subleading matched reco jet
  TH1D* h_mjet_pt_subl_fin  = AH::book1D<TH1D>("Final/h_mjet_pt_subl", "Subleading matched reco jet p_{T}",          ptBin,ptMin,ptMax,   "p_{T}^{reco,subl} [GeV]","Entries");
  TH1D* h_mjet_rap_subl_fin = AH::book1D<TH1D>("Final/h_mjet_rap_subl","Subleading matched reco jet rapidity",       rapBin,rapMin,rapMax,"y^{reco,subl}",          "Entries");
  TH1D* h_mjet_phi_subl_fin = AH::book1D<TH1D>("Final/h_mjet_phi_subl","Subleading matched reco jet #phi",           phiBin,phiMin,phiMax,"#phi^{reco,subl}",       "Entries");
  TH1D* h_mjet_dr_subl_fin  = AH::book1D<TH1D>("Final/h_mjet_dr_subl", "Subleading reco jet matching radius",drBin,drMin,drMax,   "#DeltaR_{match}^{subl}", "Entries");
  TH2D* d_mjet_pt_subl_tjet_fin  = AH::book2D<TH2D>("Final/d_mjet_pt_subl_tjet", "Subleading matched truth jet p_T vs subleading truth jet p_T",
						    ptBin,ptMin,ptMax,ptBin,ptMin,ptMax,"p_{T}^{truth,subl} [GeV]","p_{T}^{matched truth,subl} [GeV]");
  TH2D* d_mjet_rap_subl_tjet_fin  = AH::book2D<TH2D>("Final/d_mjet_rap_subl_tjet", "Subleading matched truth jety vs subleading truth jet p_T",
						    rapBin,rapMin,rapMax,rapBin,rapMin,rapMax,"y^{truth,subl}","y^{matched truth,subl}");
  // event quantities
  TH1D* h_evnt_m_fin     = AH::book1D<TH1D>("Final/h_evnt_m",  "Truth jet invariant mass M_{inv}",imBin,imMin,imMax,   "M_{inv}^{truth} [GeV]",           "Entries"); 
  TH1D* h_evnt_njm_fin   = AH::book1D<TH1D>("Final/h_evnt_njm","N_{jet}^{truth} for M_{inv}",     njlBin,njlMin,njlMax,"N_{jet}^{truth} used for M_{inv}","Entries");
  TH1D* h_evnt_gap_fin   = AH::book1D<TH1D>("Final/h_evnt_gap","Truth jet rapidity gap",          gapBin,gapMin,gapMax,"#Deltay^{truth}",                 "Entries");
  // [Final] <<--------------------------------------------------------------------------------------------------------------------------------------------------------------
 
  ///////////////////
  // Analysis cuts //
  ///////////////////

  //----------------------------------------------------------------------------
  // Here you can enter analysis cuts for e.g. event and object selection.
  // You can declare your own variables, but if you use AnalysisHelper functions, 
  // you should use the (static) variables available in the Analysis::Select
  // namespace. For exmaple, to change the pT threshold for truth jets from the
  // default 20 GeV to 40 GeV, uncomment the following line:
  //
  // Analysis::Select::TruthJet::pTmin = 40000.;   // remember this is MeV!
  //
  // Similar for reconstructed (calorimeter) jets, uncommenting
  //
  // Analysis::Select::RecoJet::rapMin = -2.5;      
  //
  // changes the lower boundary of the rapidity range from the default -4.5 to 2.5.
  // A cut flow histogram is booked belwo to keep track of the number of surviving
  // events after a given selection is applied. This is useful for e.g. efficiency
  // measurements. 
  //------------------------------------------------------------------------------

  // change isolation criteria to basically no isolation
  Analysis::Select::TruthJet::isoDeltaR = 0.;
  // mass window
  Analysis::Select::Event::minMinv = 1000000.; // [MeV]
  Analysis::Select::Event::maxMinv = 4200000.; // [MeV]
  // rapidity gap
  Analysis::Select::Event::rapGap = 1.0;  

  bool doTopoCluster(false);
  bool doTopoTower(false);
  bool doTopoTowerFine(false);
  printf("JetAnalysisExample::Loop() #-----------------------------------------\n");
  if ( Analysis::Config::Input::isTopoCluster() ) {
    printf("JetAnalysisExample::Loop() # INFO   analyzing TopoCluster\n");
    doTopoCluster = true;
  } else if ( Analysis::Config::Input::isTopoTower() ) {
    printf("JetAnalysisExample::Loop() # INFO   analyzing TopoTower (0.1 x 0.1 grid)\n");
    doTopoTower = true;
  } else if ( Analysis::Config::Input::isTopoTowerFine() ) {
    printf("JetAnalysisExample::Loop() # INFO   analyzing TopoTower (0.05 x 0.05 grid)\n");
    doTopoTowerFine = true;
  } else {
    printf("JetAnalysisExample::Loop() # FATAL  no valid signal input type!!!!\n");
    printf("JetAnalysisExample::Loop() #-----------------------------------------\n");
    return;
  }
  printf("JetAnalysisExample::Loop() #-----------------------------------------\n");


  // cut flow histogram
  TH1D* h_cutflow = AH::book1D<TH1D>("h_cutflow","h_cutflow",20,-0.5,19.5);
  // tags to be used as axis labels in the cut flow histogram
  std::vector<std::string> cutTags;                                                                                            /* cut index */
  cutTags.push_back(std::string("EventsIn"));                                                                                  /*      0    */
  cutTags.push_back(std::string("Data consistent"));                                                                           /*      1    */
  cutTags.push_back(std::string(TString::Format("N_{jet}^{truth} #geq %zu",Analysis::Select::TruthJet::numMin).Data()));       /*      2    */
  cutTags.push_back(std::string(TString::Format("#Delta y^{truth} > %3.1f",Analysis::Select::Event::rapGap).Data()));          /*      3    */
  cutTags.push_back(std::string("y_{jet,1}^{truth} #times y_{jet,2}^{truth} < 0"));                                            /*      4    */
  cutTags.push_back(std::string(TString::Format("dR_{iso}^{truth} > %3.1f",Analysis::Select::TruthJet::isoDeltaR).Data()));    /*      5    */
  cutTags.push_back(std::string(TString::Format("%6.1f < M_{inv} < %6.1f GeV",                                                 /*      6    */
						Analysis::Select::Event::minMinv/1000.,                                        /*           */
						Analysis::Select::Event::maxMinv/1000.).Data()));                              /*           */
  cutTags.push_back(std::string("Invalid indices"));                                                                           /*      7    */ 
  cutTags.push_back(std::string(TString::Format("N_{jet}^{matched} #geq %zu",Analysis::Select::MatchedJet::numMin).Data()));   /*      8    */
  cutTags.push_back(std::string(TString::Format("N_{jet}^{filtered} #geq %zu",Analysis::Select::MatchedJet::numMin).Data()));  /*      9    */

  for ( size_t i(0); i<cutTags.size(); ++i ) { h_cutflow->GetXaxis()->SetBinLabel((int)i+1,cutTags.at(i).c_str()); } 

  ////////////////
  // Event loop //
  ////////////////

  // nice formatting
  int nd = AH::ndigits((int)nentries);
  printf("JetAnalysisExample::Loop() Analyze %*i events (from entries scan)\n",nd,(int)nentries);
  std::string msgFmt("JetAnalysisExample::Loop() Event (read/total) %*i/%*i\n");

  Long64_t nbytes = 0, nb = 0;
  Long64_t dbgCtr = 0;

  // debug info printout control
  int _dbgThr = nentries;
  int _dbgLon = nentries;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    int ievt = jentry + 1;
    if ( ievt == 1 || ievt % 500 == 0 ) { 
      printf(msgFmt.c_str(),nd,ievt,nd,(int)nentries);
    }

    bool debugOn(jentry>=_dbgThr); 

    //////////////////////
    // Build truth jets //
    //////////////////////

    // cut flow
    /* 0 */ h_cutflow->Fill(cutTags.at(0).c_str(),1.);

    // use helper function to get selected truth jets 
    std::vector<fastjet::PseudoJet> tjet = AH::truthJets(this,Analysis::Select::TruthJet::pTmin,Analysis::Select::TruthJet::rapMin,Analysis::Select::TruthJet::rapMax);
    // require at least one truth jet
    if ( tjet.size() < 2 ) { continue; }
    /* 1 */ h_cutflow->Fill(cutTags.at(1).c_str(),1.);
    //    if ( tjet.size() < Analyis::Select::TruthJet::numMin ) { continue; }

    /////////////////////
    // Build reco jets //
    /////////////////////
    //                                                     
    // topo-cluster jets
    std::vector<fastjet::PseudoJet> cjet;                                   
    if ( doTopoCluster ) {
      cjet = AH::topoClusterJets(this,"lcw",Analysis::Select::RecoJet::pTmin);
    } else if ( doTopoTower ) { 
      cjet = AH::topoTowerJets(this,"lcw",Analysis::Select::RecoJet::pTmin);
    } else if ( doTopoTowerFine ) {
      cjet = AH::topoTowerFineJets(this,"lcw",Analysis::Select::RecoJet::pTmin);
    }
    //  if ( cjet.empty() ) {
    //   printf("JetAnalysisExample::Loop() FATAL - no valid input collection specified! Set Analysis::Config::Input::collection property to valid value\n");
    //   break;
    // }

    ///////////////////////////////////////
    // Select events based on truth jets //
    ///////////////////////////////////////

    // variable filled by the accept event function 
    unsigned int passlvl(0); // this tells us a bit about what happened when selecting events (bit pattern)         
    double minv(0.);         // highest invariant mass from jets
    size_t njets(2);         // this is the actual number of jets used to calculate the invariant mass
    double rapgap(0.);       // rapidity distance between two leading jets

    if ( jentry == 460 ) { printf("[%*i] ############# calling accept with %zu jets\n",nd,(int)jentry,tjet.size()); }
    // check if event passes
    size_t ijet(0); size_t jjet(0);
    Analysis::Select::Event::FailureLvl checkPass = AH::acceptTruthJetEvent(tjet,passlvl,ijet,jjet,minv,rapgap);

    //    printf("JetAnalysisExample::Loop() DEBUG   M_inv = %9.3f GeV, delta_y = %4.2f, N_jet = %zu\n",minv/1000.,rapgap,tjet.size());

    ////////////////////////////////////////////////////////////////////////////
    // First round of plots (most inclusive, only very basic event selection) //
    ////////////////////////////////////////////////////////////////////////////

    // number of jets
    double ntjets = (double)tjet.size();
    double ncjets = (double)cjet.size();

    // fill event level plots
    h_tjet_num_ini->Fill(ntjets);                                // original number of truth jets
    h_rjet_num_ini->Fill(ncjets);                                // original number of reco jets

    h_evnt_m_ini->Fill(minv/1000.);                                           // truth invariant mass of collision system
    h_evnt_njm_ini->Fill((double)njets);                                      // number of jets used to calculate invariant mass
    h_evnt_gap_ini->Fill(rapgap);                                             // rapidity distance between two leading truth jets

    // fill truth jet level plots
    for ( size_t i(0); i<tjet.size(); ++i ) {
      // pick up and rescale variables as needed
      double pT(tjet.at(i).pt()/1000.); double rap(tjet.at(i).rap()); double phi(tjet.at(i).phi_std());
      double nc(AH::numConstits(tjet.at(i)));
      // leading jet
      if ( i == 0 ) { h_tjet_pt_lead_ini->Fill(pT); h_tjet_rap_lead_ini->Fill(rap); h_tjet_phi_lead_ini->Fill(phi); }
      // subleading jet
      if ( i == 1 ) { h_tjet_pt_subl_ini->Fill(pT); h_tjet_rap_subl_ini->Fill(rap); h_tjet_phi_subl_ini->Fill(phi); }
      // all jets
      h_tjet_pt_ini->Fill(pT); h_tjet_rap_ini->Fill(rap); h_tjet_phi_ini->Fill(phi);
      d_tjet_num_rap_ini->Fill(rap,nc);
    } // fill jet properties

    // fill reco level jet plots
    for ( size_t i(0); i<cjet.size(); ++i ) { 
      // pick up and rescale variables as needed
      double pT(cjet.at(i).pt()/1000.); double rap(cjet.at(i).rap()); double phi(cjet.at(i).phi_std());
      double nc(AH::numConstits(cjet.at(i)));
      // leading jet
      if ( i == 0 ) { h_rjet_pt_lead_ini->Fill(pT); h_rjet_rap_lead_ini->Fill(rap); h_rjet_phi_lead_ini->Fill(phi); }
      // subleading jet
      if ( i == 1 ) { h_rjet_pt_subl_ini->Fill(pT); h_rjet_rap_subl_ini->Fill(rap); h_rjet_phi_subl_ini->Fill(phi); }
      // all jets
      h_rjet_pt_ini->Fill(pT); h_rjet_rap_ini->Fill(rap); h_rjet_phi_ini->Fill(phi); 
      d_rjet_num_rap_ini->Fill(rap,nc);
   }

    if ( jentry == _dbgLon ) { printf("JetAnalysisExample::Loop() [%*i] event selection start\n",nd,(int)jentry); }
    // skip event if failed but check at which state of the event selection
    if ( Analysis::Select::Event::check(checkPass,Analysis::Select::Event::FailNTruthJet) )  { continue; }
    /* 2 */ h_cutflow->Fill(cutTags.at(2).c_str(),1.);
    if ( Analysis::Select::Event::check(checkPass,Analysis::Select::Event::FailRapGap) )     { continue; }
    /* 3 */ h_cutflow->Fill(cutTags.at(3).c_str(),1.);
    if ( Analysis::Select::Event::check(checkPass,Analysis::Select::Event::FailHemisphere) ) { continue; }
    /* 4 */ h_cutflow->Fill(cutTags.at(4).c_str(),1.);
    if ( Analysis::Select::Event::check(checkPass,Analysis::Select::Event::FailIsolation) )  { continue; }
    /* 5 */ h_cutflow->Fill(cutTags.at(5).c_str(),1.);
    if ( Analysis::Select::Event::check(checkPass,Analysis::Select::Event::FailMinv) )       { continue; }
    /* 6 */ h_cutflow->Fill(cutTags.at(6).c_str(),1.);                 
    if ( ijet > tjet.size() || jjet > tjet.size() || ijet == jjet ) { continue; }
    /* 7 */ h_cutflow->Fill(cutTags.at(7).c_str(),1.);
    if ( jentry == _dbgLon ) { printf("JetAnalysisExample::Loop() [%*i] event selection stop\n",nd,(int)jentry); }

    // remove all signal jets but the two forming the largest mass
    std::vector<fastjet::PseudoJet> tjetsel;
    tjetsel.push_back(tjet.at(ijet));
    tjetsel.push_back(tjet.at(jjet));
 
    ////////////////
    // Match jets //
    ////////////////

    // find closest matches
    if ( jentry == _dbgLon ) { printf("JetAnalysisExample::Loop() [%*i] jet matching start\n",nd,(int)jentry); }
    std::vector<int>    truthJetIdx;  // indices of matched truth jet in their container
    std::vector<int>    recoJetIdx;   // indices of matched reco jet in their container
    std::vector<double> deltaR;       // delta R between them
    size_t nmatch = AH::matchJets(tjet,cjet,truthJetIdx,recoJetIdx,deltaR,2.);  // find all matches with deltaR < 2
    if ( jentry == _dbgLon ) { printf("JetAnalysisExample::Loop() [%*i] jet matching stop (%zu matched jets)\n",nd,(int)jentry,nmatch); }
    //   printf("JetAnalysisExample::Loop() DEBUG   number of matched jets %zu\n",nmatch);

    if ( nmatch < 2 ) { continue; }
    /* 8 */ h_cutflow->Fill(cutTags.at(8).c_str(),1.);

    if ( jentry == _dbgLon ) { printf("JetAnalysisExample::Loop() [%*i] find matches for selected jets\n",nd,(int)jentry); }
    double ri(0.); double rj(0.); 
    auto ijf = std::find(truthJetIdx.begin(),truthJetIdx.end(),ijet); size_t iijf(size_t(-1));
    if ( ijf == truthJetIdx.end() ) { ri = -1.; } else { iijf = *ijf; if ( iijf < deltaR.size() ) { ri = deltaR.at(iijf); } else { ri = -1.; } }
    auto jjf = std::find(truthJetIdx.begin(),truthJetIdx.end(),jjet); size_t ijjf(size_t(-1));
    if ( jjf == truthJetIdx.end() ) { rj = -1.; } else { ijjf = *jjf; if ( ijjf < deltaR.size() ) { rj = deltaR.at(ijjf); } else { ri = -1.; } }
    if ( jentry == _dbgLon ) { printf("JetAnalysisExample::Loop() [%*i] found matches (%3zu,%3zu) (%6.3f,%6.3f) indices (%3zu,%3zu), maximum %3zu\n",
				      nd,(int)jentry,ijet,jjet,ri,rj,iijf,ijjf,deltaR.size()); }
    //   printf("JetAnalysisExample::Loop() DEBUG   signal jets (%3zu,%3zu) are matched at (%6.3f,%6.3f)\n",ijet,jjet,ri,rj);

    // check if signal jets have narrow matches
    size_t jctr(0); size_t nmfilter(0);
    while ( jctr < truthJetIdx.size() && nmfilter < 2 ) { 
      if ( ( truthJetIdx.at(jctr) == (int)ijet || truthJetIdx.at(jctr) == (int)jjet ) && deltaR.at(jctr) < Analysis::Select::MatchedJet::matchR ) { ++nmfilter; }
      ++jctr;  
    }
    //    printf("JetAnalysisExample::Loop() DEBUG   number of filtered jets %zu\n",nmfilter);

    ////////////////////////////////////////////////////////////////
    // Inclusive plots after event selection for all matched jets //
    ////////////////////////////////////////////////////////////////

    // fill event level plots
    h_tjet_num_acc->Fill(ntjets);
    h_rjet_num_acc->Fill(ncjets);
    h_mjet_num_acc->Fill((double)recoJetIdx.size());

    h_evnt_m_acc->Fill(minv/1000.);           
    h_evnt_njm_acc->Fill((double)njets);      
    h_evnt_gap_acc->Fill(rapgap);                

    // fill truth jet level plots
    for ( size_t i(0); i<tjet.size(); ++i ) {
      double pT(tjet.at(i).pt()/1000.); double rap(tjet.at(i).rap()); double phi(tjet.at(i).phi_std());
      double nc(AH::numConstits(tjet.at(i)));
      // leading jet
      if ( i == 0 ) { h_tjet_pt_lead_acc->Fill(pT); h_tjet_rap_lead_acc->Fill(rap); h_tjet_phi_lead_acc->Fill(phi); }
      // subleading jet
      if ( i == 1 ) { h_tjet_pt_subl_acc->Fill(pT); h_tjet_rap_subl_acc->Fill(rap); h_tjet_phi_subl_acc->Fill(phi); }
      // all jets
      h_tjet_pt_acc->Fill(pT); h_tjet_rap_acc->Fill(rap); h_tjet_phi_acc->Fill(phi);
      d_tjet_num_rap_acc->Fill(rap,nc);
    } // fill jet properties

    // fill reco level jet plots
    for ( size_t i(0); i<cjet.size(); ++i ) { 
      double pT(cjet.at(i).pt()/1000.); double rap(cjet.at(i).rap()); double phi(cjet.at(i).phi_std());
      double nc(AH::numConstits(cjet.at(i)));
      // leading jet
      if ( i == 0 ) { h_rjet_pt_lead_acc->Fill(pT); h_rjet_rap_lead_acc->Fill(rap); h_rjet_phi_lead_acc->Fill(phi); }
      // subleading jet
      if ( i == 1 ) { h_rjet_pt_subl_acc->Fill(pT); h_rjet_rap_subl_acc->Fill(rap); h_rjet_phi_subl_acc->Fill(phi); }
      // all jets
      h_rjet_pt_acc->Fill(pT); h_rjet_rap_acc->Fill(rap); h_rjet_phi_acc->Fill(phi);
      d_rjet_num_rap_acc->Fill(rap,nc);
    }

    // fill matched reco jet level plots
    for ( size_t i(0); i<truthJetIdx.size(); ++i ) {
      // indices and such
      size_t itj(truthJetIdx.at(i));
      size_t irj(recoJetIdx.at(i));
      double dR(deltaR.at(i));                      // matching radius (all matches)
      // kinematics
      double pTtruth(tjet.at(itj).pt()/1000.);
      double rapTruth(tjet.at(itj).rap());
      double phiTruth(tjet.at(itj).phi_std());
      double pTreco(cjet.at(irj).pt()/1000.);
      double rapReco(cjet.at(irj).rap());
      double phiReco(cjet.at(irj).phi_std());
      double nCreco(AH::numConstits(cjet.at(irj)));
      // leading jet 
      if ( i == 0 ) { h_mjet_pt_lead_acc->Fill(pTreco); h_mjet_rap_lead_acc->Fill(rapReco); h_mjet_phi_lead_acc->Fill(phiReco); h_mjet_dr_lead_acc->Fill(dR); }
      // subleading jet
      if ( i == 1 ) { h_mjet_pt_subl_acc->Fill(pTreco); h_mjet_rap_subl_acc->Fill(rapReco); h_mjet_phi_subl_acc->Fill(phiReco); h_mjet_dr_subl_acc->Fill(dR); }
      // all jets
      h_mjet_pt_acc->Fill(pTreco); h_mjet_rap_acc->Fill(rapReco); h_mjet_phi_acc->Fill(phiReco); h_mjet_dr_acc->Fill(dR); 
      d_mjet_num_rap_acc->Fill(rapReco,nCreco);
      // get PTD
      double cptd(AH::calculateCaloJetPTD(cjet.at(irj)));
      double jptd(AH::calculatePTD(cjet.at(irj)));
      double rptd = cptd > 0. ? jptd/cptd : 0.;
      d_mjet_ptd_rap_fin->Fill(rapReco,cptd);
      d_mjet_rptd_rap_fin->Fill(rapReco,rptd);
    }

    /////////////////////
    // Final selection //
    /////////////////////

    // reject if both jets with highest mass are not matched
    if ( nmfilter < 2 ) { continue; }
    /* 8 */ h_cutflow->Fill(cutTags.at(9).c_str(),1.);

    // debug and warning
    static int _nmsg(20); 
    double dm((tjetsel.front()+tjetsel.back()).m()/minv);
    bool warn(dm>1.001);
    if ( debugOn || dbgCtr < _nmsg || warn ) { 
      size_t ihrd(0), jhrd(1);
      std::string msgForm = warn 
	? "JetAnalysisExample::Loop() WARNING [%*i] highest mass jet system (m_jj = %9.3f GeV) at (%3zu,%3zu) with pT (%9.3f,%9.3f) GeV at y (%5.2f,%5.2f) (expected mass %9.3f GeV)\n"
	: "JetAnalysisExample::Loop() DEBUG   [%*i] highest mass jet system (m_jj = %9.3f GeV) at (%3zu,%3zu) with pT (%9.3f,%9.3f) GeV at y (%5.2f,%5.2f)\n";
      printf(msgForm.c_str(),nd,(int)jentry,minv/1000.,ijet,jjet,tjet.at(ijet).pt()/1000.,tjet.at(jjet).pt()/1000.,tjet.at(ijet).rap(),tjet.at(jjet).rap(),dm*minv/1000.);
      if ( dbgCtr < _nmsg || warn ) { 
	printf("JetAnalysisExample::Loop()         [%*i] hardest jet system (m_jj = %9.3f GeV) at .... (%3zu,%3zu) with pT (%9.3f,%9.3f) GeV at y (%5.2f,%5.2f)\n",
	       nd,(int)jentry,(tjet.at(ihrd)+tjet.at(jhrd)).m()/1000., ihrd, jhrd, tjet.at(ihrd).pt()/1000.,tjet.at(jhrd).pt()/1000.,tjet.at(ihrd).rap(),tjet.at(jhrd).rap());
      }
      if ( dbgCtr == _nmsg-1 ) { 
	printf("JetAnalysisExample::Loop()         [%*i] hardest jet system (m_jj = %9.3f GeV) at .... (%3zu,%3zu) with pT (%9.3f,%9.3f) GeV at y (%5.2f,%5.2f) (last such message)\n",
	       nd,(int)jentry,(tjet.at(ihrd)+tjet.at(jhrd)).m()/1000., ihrd, jhrd, tjet.at(ihrd).pt()/1000.,tjet.at(jhrd).pt()/1000.,tjet.at(ihrd).rap(),tjet.at(jhrd).rap());
      }
      if ( !warn ) { ++dbgCtr; }
    }

    // extract the two matching reco jets 
    std::vector<fastjet::PseudoJet> rjetsel(njets); 
    for ( size_t i(0); i<truthJetIdx.size(); ++i ) { 
      if ( truthJetIdx.at(i) == (int)ijet ) { rjetsel[0] = cjet.at(recoJetIdx.at(i)); }
      if ( truthJetIdx.at(i) == (int)jjet ) { rjetsel[1] = cjet.at(recoJetIdx.at(i)); }
    }

    // pT ordering
    if ( tjetsel.front().pt() < tjetsel.back().pt() ) {
      fastjet::PseudoJet jj = tjetsel.front();
      fastjet::PseudoJet kk = rjetsel.front(); 
      tjetsel[0] = tjetsel.back(); rjetsel[0] = rjetsel.back();
      tjetsel[1] = jj;           ; rjetsel[1] = kk;
    }

    // fill event level plots
    h_tjet_num_fin->Fill(ntjets);
    h_rjet_num_fin->Fill(ncjets);
    h_mjet_num_fin->Fill((double)rjetsel.size());

    h_evnt_m_fin->Fill(minv/1000.);           
    h_evnt_njm_fin->Fill((double)njets);      
    h_evnt_gap_fin->Fill(rapgap);                

    // fill truth jet level plots
    for ( size_t i(0); i<tjetsel.size(); ++i ) {
      double pT(tjetsel.at(i).pt()/1000.); double rap(tjetsel.at(i).rap()); double phi(tjetsel.at(i).phi_std());
      double nc(AH::numConstits(tjetsel.at(i)));
      // leading jet
      if ( i == 0 ) { h_tjet_pt_lead_fin->Fill(pT); h_tjet_rap_lead_fin->Fill(rap); h_tjet_phi_lead_fin->Fill(phi); }
      // subleading jet
      if ( i == 1 ) { h_tjet_pt_subl_fin->Fill(pT); h_tjet_rap_subl_fin->Fill(rap); h_tjet_phi_subl_fin->Fill(phi); }
      // all jets
      h_tjet_pt_fin->Fill(pT); h_tjet_rap_fin->Fill(rap); h_tjet_phi_fin->Fill(phi);
      d_tjet_num_rap_fin->Fill(rap,nc);
    } // fill jet properties

    // fill reco level jet plots
    for ( size_t i(0); i<cjet.size(); ++i ) { 
      double pT(cjet.at(i).pt()/1000.); double rap(cjet.at(i).rap()); double phi(cjet.at(i).phi_std());
      double nc(AH::numConstits(cjet.at(i)));
      // leading jet
      if ( i == 0 ) { h_rjet_pt_lead_fin->Fill(pT); h_rjet_rap_lead_fin->Fill(rap); h_rjet_phi_lead_fin->Fill(phi); }
      // subleading jet
      if ( i == 1 ) { h_rjet_pt_subl_fin->Fill(pT); h_rjet_rap_subl_fin->Fill(rap); h_rjet_phi_subl_fin->Fill(phi); }
      // all jets
      h_rjet_pt_fin->Fill(pT); h_rjet_rap_fin->Fill(rap); h_rjet_phi_fin->Fill(phi);
      d_rjet_num_rap_fin->Fill(rap,nc);
    }

    // fill matched jets
    for ( size_t i(0); i<rjetsel.size(); ++i ) {
      // kinematics
      double pTtruth(tjetsel.at(i).pt()/1000.); double rapTruth(tjetsel.at(i).rap()); double phiTruth(tjetsel.at(i).phi_std());
      double pTreco(rjetsel.at(i).pt()/1000.);  double rapReco(rjetsel.at(i).rap());  double phiReco(rjetsel.at(i).phi_std());
      double dR(tjetsel.at(i).delta_R(rjetsel.at(i)));
      double nCreco(AH::numConstits(rjetsel.at(i)));
      // leading jet 
      if ( i == 0 ) { 
	h_mjet_pt_lead_fin->Fill(pTreco); h_mjet_rap_lead_fin->Fill(rapReco); h_mjet_phi_lead_fin->Fill(phiReco); h_mjet_dr_lead_fin->Fill(dR); 
	d_mjet_pt_lead_tjet_fin->Fill(tjet.at(i).pt()/1000.,pTtruth); 
	d_mjet_rap_lead_tjet_fin->Fill(tjet.at(i).rap(),rapTruth);
      }
      // subleading jet
      if ( i == 1 ) { 
	h_mjet_pt_subl_fin->Fill(pTreco); h_mjet_rap_subl_fin->Fill(rapReco); h_mjet_phi_subl_fin->Fill(phiReco); h_mjet_dr_subl_fin->Fill(dR);
	d_mjet_pt_subl_tjet_fin->Fill(tjet.at(i).pt()/1000.,pTtruth); 
	d_mjet_rap_subl_tjet_fin->Fill(tjet.at(i).rap(),rapTruth);
      }
      // all jets
      h_mjet_pt_fin->Fill(pTreco); h_mjet_rap_fin->Fill(rapReco); h_mjet_phi_fin->Fill(phiReco); h_mjet_dr_fin->Fill(dR); 
      d_mjet_num_rap_fin->Fill(rapReco,nCreco);
    }
    //    /* 9 */ h_cutflow->Fill(cutTags.at(9).c_str(),1.);
  } // event loop 

  ////////////
  // Output //
  ////////////

  double ie = h_cutflow->GetBinContent(1);
  if ( ie > 0. ) { 
    nd = AnalysisHelper::ndigits((int)ie);               
    // preview finds maximum string length 
    size_t kd(0);
    for ( int i(1); i<=h_cutflow->GetNbinsX(); ++i ) {
      std::string xlabel(h_cutflow->GetXaxis()->GetBinLabel(i)); 
      if ( xlabel.length() > kd ) { kd = xlabel.length(); } 
    }
    // print summary
    printf("\nJetAnalysisExample::Loop() [begin] cutflow summary >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    for ( int i(1); i<=h_cutflow->GetNbinsX(); ++i ) {
      std::string tag(h_cutflow->GetXaxis()->GetBinLabel(i));
      if ( !tag.empty() && tag !="" ) { 
	printf("JetAnalysisExample::Loop() Events after cut %*s (bin %2i): %*i (%5.1f%%)\n",
	       (int)kd,tag.c_str(),i,nd,(int)h_cutflow->GetBinContent(i),h_cutflow->GetBinContent(i)/ie*100.);
      }
    }
    printf("JetAnalysisExample::Loop() [end]   Cutflow summary <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  }

  std::string ofname(TString::Format("jetanalysis_rapgap_%03.1f",Analysis::Select::Event::rapGap).Data());
  if ( doTopoCluster ) {
    ofname += "_topocl.root";
  } else if ( doTopoTower ) {
    ofname += "_topotn.root";
  } else if ( doTopoTowerFine ) {
    ofname += "_topotf.root";
  }
  int nhists = AH::writeHists(ofname);
}
