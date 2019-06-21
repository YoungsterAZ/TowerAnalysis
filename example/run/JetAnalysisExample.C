
#define JetAnalysisExample_cxx
#include "JetAnalysisExample.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>

#include <vector>
#include <string>

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"

#include "AnalysisData.h"
#include "AnalysisUtils.h"
#include "AnalysisHelper.h"

#include "SlidingWindow.h"

#include "JetAnalysisGroup.h"
#include "TwoJetAnalysisGroup.h"
#include "TopoAnalysisGroup.h"

#include "SelectorApplication.h"

/////////////////////////////////////////////////
// Do not touch the constructor implementation //
/////////////////////////////////////////////////

JetAnalysisExample::JetAnalysisExample(TChain* tchain,const std::string& cfgFile) : fChain(0), _cfgFile(cfgFile), _cfgRead(false), _isDead(false)
{
  if ( tchain == 0 ) {
    PRINT_ERROR( "JetAnalysisExample::JetAnalysisExample", "invalid pointer to TChain (%p)", (void*)tchain );
    _isDead = true;
  } else { 
    // load chain
    _cfgRead = AH::readCfg(_cfgFile);
    if ( !_cfgRead ) { 
      PRINT_ERROR( "JetAnalysisExample::JetAnalysisExample", "cannot read specified configuration file \042%s\042 - module is turned off",_cfgFile.c_str() );
      _isDead = true;
    } else {
      AH::printBanner("JetAnalysisExample::JetAnalysisExample",Analysis::Config::Input::collection);
      PRINT_INFO( "JetAnalysisExample::JetAnalysisExample", "read configuration file \042%s\042",_cfgFile.c_str() );
      int nfile = Analysis::Config::Input::nFiles = -1 ? 40 : Analysis::Config::Input::nFiles;
      for ( int ifile(0); ifile < nfile; ++ifile ) {
	std::string fname(TString::Format(Analysis::Config::Input::fileMask.c_str(),Analysis::Config::Input::dataDir.c_str(),ifile).Data());
	PRINT_INFO("JetAnalysisExample::JetAnalysisExample","connect file \042%s\042 to chain \042%s\042",fname.c_str(),tchain->GetName());
	tchain->Add(fname.c_str());
      }
      _isDead = tchain->GetListOfFiles()->IsEmpty();
      Init(tchain);
    }
    if ( Analysis::Config::Filter::CaloSignal::doTimeFilter ) { AH::printCaloTimingFilter("JetAnalysisExample::JetAnalysisExample"); }
  }
}


/////////////////////////////////
// Analysis playground: Loop() //
/////////////////////////////////

void JetAnalysisExample::Loop(Long64_t nreq)
{
  static std::string _mname = "JetAnalysisExample::Loop";

  //////////////
  // Preamble //
  //////////////

  // consistency check
  if ( _isDead ) {
    PRINT_ERROR("JetAnalysisExample::Loop","module is dead (likely no valid input file found\n");
    return;
  }

  if (fChain == 0) return;

  // total number of entries
  Long64_t nentries = fChain->GetEntries();

  // open output file
  bool doWrite(false);
  if ( !Analysis::Config::Output::file.empty() && Analysis::Config::Output::file != "" ) {
    if ( AH::openFile(Analysis::Config::Output::file) == 0 ) { 
      PRINT_ERROR( _mname, "cannot open file \042%s\042 - fatal!", Analysis::Config::Output::file.c_str() );
      return;
    } 
    doWrite = true;
  }

  /////////////////////
  // Book histograms //
  /////////////////////

  // binning in invariant mass
  int imBin(400); double imMin(0.); double imMax(4000.);

  // fine binning in invariant mass
  int imfBin(100); double imfMin(0.); double imfMax(1000.);

  // rapidity binning
  int rapBin(101); double rapMin(-5.05); double rapMax(5.05);

  // rapidity gap binning
  int gapBin(100); double gapMin(0.); double gapMax(10.);

  // [1][Inclusive]
  //
  // These plots are for the most inclusive event selection - the only requirement is two truth jets with pT > 10 GeV (typically).
  // We plot the inclusive (all truth jets after event selection) and the features of the truth jet candidates which may be related to the 
  // decay.
  //
  /* [1.1] */ JetAnalysisGroup truthIncl("Inclusive/Truth");               // all truth jets 
  /* [1.2] */ JetAnalysisGroup dcyTruthIncl("Inclusive/DcyTruth");         // all combinations of truth jets
  /* [1.3] */ JetAnalysisGroup dcyTruthWndw("Inclusive/DcyTruthSelected"); // truth jets potentially associated with the central particle decay
  //
  // Event shapes can be studied with truth jets for this most inclusive sample. Presently only The invariant mass of the decay candidate system 
  // is considered.
  //
  /* [1.4] */ TH1D* h_dcymass_incl = AH::book1D<TH1D>("Inclusive/DcyTruth/h_dcymass","M_{jj}^{truth}",imBin,imMin,imMax,"M_{jj}^{truth} [GeV]"); 
  /*       */ h_dcymass_incl->GetYaxis()->SetTitle(TString::Format("#partialN/#partialM_{jj}^{truth} [1/(%.1f GeV)]",h_dcymass_incl->GetBinWidth(1)).Data());
  /* [1.5] */ TH2D* d_dcymass_rap_incl = AH::book2D<TH2D>("Inclusive/DcyTruth/d_dcymass_rap","M_{jj}^{truth} vs y_{jj}^{truth}",rapBin,rapMin,rapMax,imBin,imMin,imMax,
					      "y_{jj}^{truth}","M_{jj}^{truth} [GeV]");
  //
  // [2][Initial]
  //
  // These plots reflect the events after (optional) removal truth jets which could be associated with the decay of the centrally produced 
  // heavy particle. Also, an optional isolation criteria is applied to the truth jets. This truth-based event selection enhances the 
  // contribution of the required final state in the simulation sample. It can serve as a normalization for efficiency studies (prior to any
  // detector-based selections).
  //  
  /* [2.1] */ JetAnalysisGroup truthJetIni("Initial/Jets/Truth");         // [2.1] truth jets with the decay candidate jets removed 
  /* [2.2] */ JetAnalysisGroup dcyTruthIni("Initial/Jets/DcyTruth");      // [2.2] decay candidate jets (orthogonal to [1], no overlap in content) 
  /* [2.3' */ JetAnalysisGroup leptonIni("Initial/Leptons");              // [2.3] leptons from particle decay 
  /* [2.4] */ JetAnalysisGroup caloJetIni("Initial/Jets/Calo");           // [2.4] calorimeter jets for this sample
  // 
  // Event shapes can be studied for this sample. This study is based on features reconstructed from truth jets - invariant mass of the decay candidate system - 
  // and calorimeter jets (invariant mass of, and rapidity gaop between, two VBF candidate jets). The VBF candidates are those spawning the highest invariant mass
  // in the dijet system. In addition, the distance between the leptons from the scalar boson decay and he nearest truth jets are analyzed.
  //
  // Analysis of the two tag jets
  /* [2.5] */ TwoJetAnalysisGroup tagJetsIni("Initial/Jets/Calo/Signal"); // topology of signal jet candidates
  //
  // -- truth (decay candidate) jet topoplogies:
  /* [2.6] */ TH1D* h_dcymass_ini = AH::book1D<TH1D>("Initial/Jets/DcyTruthSelected/h_dcymass","M_{jj}^{truth}",imfBin,imMin,imMax,"M_{jj}^{truth} [GeV]");
  /*       */ h_dcymass_ini->GetYaxis()->SetTitle(TString::Format("#partialN/#partialM_{jj}^{truth} [1/(%.1f GeV)]",h_dcymass_ini->GetBinWidth(1)).Data());
  /* [2.7] */ TH2D* d_dcymass_rap_ini = AH::book2D<TH2D>("Initial/Jets/DcyTruthSelected/d_dcymass_rap","M_{jj}^{truth} vs y_{jj}^{truth}",rapBin,rapMin,rapMax,imBin,imMin,imMax,
							 "y_{jj}^{truth}","M_{jj}^{truth} [GeV]");
  // -- truth topology - lepton-jet distances
  /* [2.8] */ TH1D* h_mdistIni = AH::book1D<TH1D>("Initial/Jets/Truth/h_lept_mindist","Minimum #DeltaR^{truth}(lepton,jet)",320,0.,6.4,"#DeltaR","Entries");
  // -- calorimeter jet topologies:
  /* [2.9] */ TH1D* h_cjet_minv_ini  = AH::book1D<TH1D>("Initial/Jets/Calo/h_cjet_minv","Invariant Mass, Calo Jets", imBin,imMin,imMax,   "Invariant Mass, MeV ","Entries"); 
  /* [2.10]*/ TH1D* h_cjet_rapgap_ini = AH::book1D<TH1D>("Initial/Jets/Calo/h_cjet_rapgap","Rapidity Gap, Calo Jets",gapBin,gapMin,gapMax,"Rapditiy Gap}",          "Entries");
  /* [2.11]*/ TH2D* d_cjet_rapgap_minv_ini = AH::book2D<TH2D>("Initial/Jets/Calo/d_cjet_rapgap_minv","Rapidity gap vs Invariant Mass",imBin,imMin,imMax,gapBin,gapMin,gapMax,
							       "M_{inv} [GeV]","#Delta y");
  //
  // [3][Final]
  //
  // The distributions for this selection include requirements for the number of calorimeter jets, the minimum rapidity gap between them, 
  // the invariant two-jet mass within a given window, and each of the VBF candidates located on a different rapidity hemisphere.
  // 
  /* [3.1] */ JetAnalysisGroup truthJetFin("Final/Jets/Truth");         // truth jets with decay candidates removed
  /* [3.2] */ JetAnalysisGroup dcyTruthFin("Final/Jets/DcyTruth");      // decay candidate jets (orthogonal to [1], no overlap in content) 
  /* [3.3] */ JetAnalysisGroup leptonFin("Final/Leptons");              // leptons from particle decays
  /* [3.4] */ JetAnalysisGroup caloJetFin("Final/Jets/Calo");           // calorimeter jets for this sample
  /* [3.5] */ TwoJetAnalysisGroup tagJetsFin("Final/Jets/Calo/Signal"); // topology of signal jet candidates
  /* [3.6] */ TH1D* h_dcymass_fin = AH::book1D<TH1D>("Final/Jets/DcyTruthSelected/h_dcymass","M_{jj}^{truth}",imfBin,imMin,imMax,"M_{jj}^{truth} [GeV]");
  /*       */ h_dcymass_fin->GetYaxis()->SetTitle(TString::Format("#partialN/#partialM_{jj}^{truth} [1/(%.1f GeV)]",h_dcymass_fin->GetBinWidth(1)).Data());
  /* [3.7] */ TH2D* d_dcymass_rap_fin = AH::book2D<TH2D>("Final/Jets/DcyTruthSelected/d_dcymass_rap","M_{jj}^{truth} vs y_{jj}^{truth}",rapBin,rapMin,rapMax,imBin,imMin,imMax,
							 "y_{jj}^{truth}","M_{jj}^{truth} [GeV]");
  /* [3.8] */ TH1D* h_mdistFin = AH::book1D<TH1D>("Final/Jets/Truth/lept_mindist","Minimum #DeltaR^{truth}(lepton,jet)",320,0.,6.4,"#DeltaR","Entries");
  //
  // Event shapes for this selection:
  /* [3.9] */ TH1D* h_cjet_minv_fin  = AH::book1D<TH1D>("Final/Jets/Calo/h_cjet_minv","Invariant Mass, Calo Jets After Cuts", imBin,imMin,imMax,   "Invariant Mass, MeV ","Entries"); 
  /* [3.10]*/ TH1D* h_cjet_rapgap_fin = AH::book1D<TH1D>("Final/Jets/Calo/h_cjet_rapgap","Rapidity Gap, Calo Jets After Cuts",gapBin,gapMin,gapMax,"Rapditiy Gap}",          "Entries");
  /* [3.11]*/ TH2D* d_cjet_rapgap_minv_fin = AH::book2D<TH2D>("Final/Jets/Calo/d_cjet_rapgap_minv","Rapidity gap vs Invariant Mass After Cuts",imBin,imMin,imMax,gapBin,gapMin,gapMax,
							      "M_{inv} [GeV]","#Delta y");
  //
  // For this selection, truth matching was applied to calorimeter jets. Distributions are filled for matched and unmatched truth and calorimeter jets.  
  /* [3.12]*/ JetAnalysisGroup truthMatchedFin("Final/MatchedJets/Truth");      // truth jets matched with calorimeter jets
  /* ]3.13]*/ JetAnalysisGroup truthUnmatchedFin("Final/UnmatchedJets/Truth");  // truth jets not matched with calorimeter jets
  /* [3.14]*/ JetAnalysisGroup caloMatchedFin("Final/MatchedJets/Calo");        // calorimeter jets matched with truth jets      
  /* [3.15]*/ JetAnalysisGroup caloUnmatchedFin("Final/UnmatchedJets/Calo");    // calorimeter jets not matched with truth jets
  //
  // The matched calorimeter jets are analyzed before pile-up suppression
  /* [3.16]*/ TopoAnalysisGroup topoMatchedFin("Final/MatchedJets/Calo");       // calorimeter jets matched with truth jets after pile-up suppression  
  //
  // Analysis of the two tag jets
  /* [3.17]*/ TwoJetAnalysisGroup tagMatchedJetsFin("Final/MatchedJets/Calo/Signal");             // topology of signal jet candidate without truth match
  /* [3.18]*/ TwoJetAnalysisGroup tagUnmatchedJetsFin("Final/UnmatchedJets/Calo/Signal");         // topology of signal jet candidates without truth match
  /* [3.19]*/ TwoJetAnalysisGroup tagSingleMatchedJetsFin("Final/SingleMatchedJets/Calo/Signal"); // topology of signal jet candidates without truth match
  //
  // [4][Jet-area-based pile-up suppression]
  //
  // Without changing the event sample, the effects of the jet-area-based pile-up suppression are studied. 
  //
  /* [4.1] */ JetAnalysisGroup caloAcceptJetPUArea("Final/Jets/Calo/PUAreaAccept");                // calorimeter jets after PU suppression
  /* [4.2] */ JetAnalysisGroup caloRejectJetPUArea("Final/Jets/Calo/PUAreaReject");                // calorimeter jets rejected by PU suppression
  /* [4.3] */ JetAnalysisGroup caloAcceptSignalJetPUArea("Final/MatchedJets/Calo/PUAreaAccept");   // calorimeter jets after PU suppression matched with truth jets (signal jet candidates)
  /* [4.4] */ JetAnalysisGroup caloRejectSignalJetPUArea("Final/MatchedJets/Calo/PUAreaReject");   // calorimeter jets rejected by PU suppression matched with truth jets (signal jet candidates)
  /* [4.5] */ JetAnalysisGroup caloAcceptPileupJetPUArea("Final/UnmatchedJets/Calo/PUAreaAccept"); // calorimeter jets after PU suppression not matched with truth jets (PU jet candidates)
  /* [4.6] */ JetAnalysisGroup caloRejectPileupJetPUArea("Final/UnmatchedJets/Calo/PUAreaReject"); // calorimeter jets rejected by PU suppression not matched with truth jets (PU jet candidates)
  //
  // The matched jets are analyzed with respect to topology and kinematic features.
  //  /* [4.7] */ TopoAnalysisGroup topoAcceptPUAreaFin("Final/MatchedJets/Calo/PUAreaAccept");        // accepted calorimeter jet pairs matched with truth jets after pile-up suppression
  //  /* [4.8] */ TopoAnalysisGroup topoRejectPUAreaFin("Final/MatchedJets/Calo/PUAreaReject");        // rejected calorimeter jet pairs matched with truth jets after pile-up suppression
  //
  // Analysis of the two tag jets
  /* [4.9] */ TwoJetAnalysisGroup tagJetsPUArea("Final/Jets/Calo/PUArea/Signal");                           // topology of signal jet candidates
  /* [4.10]*/ TwoJetAnalysisGroup tagMatchedJetsPUArea("Final/MatchedJets/Calo/PUArea/Signal");             // topology of signal jet candidates with truth match
  /* [4.11]*/ TwoJetAnalysisGroup tagUnmatchedJetsPUArea("Final/UnmatchedJets/Calo/PUArea/Signal");         // topology of signal jet candidates without truth match
  /* [4.12]*/ TwoJetAnalysisGroup tagSingleMatchedJetsPUArea("Final/SingleMatchedJets/Calo/PUArea/Signal"); // topology of signal jet candidates without truth match
  //
  // [5][Events after pile-up suppression]
  //
  // The jets indicating the correct topology after pile-up suppression are studied.
  JetAnalysisGroup truthJetSel("Selected/Jets/Truth");                 // [5.1] truth jets with decay candidates removed
  JetAnalysisGroup dcyTruthSel("Selected/Jets/DcyTruth");              // [5.2] decay candidate jets (orthogonal to [1], no overlap in content) 
  JetAnalysisGroup leptonSel("Selected/Leptons");                      // [5.3] leptons from particle decays
  JetAnalysisGroup caloJetSel("Selected/Jets/Calo");                   // [5.4] calorimeter jets for this sample
  //
  // For this selection, truth matching was applied to calorimeter jets. Distributions are filled for matched and unmatched truth and calorimeter jets.  
  JetAnalysisGroup truthMatchedSel("Selected/MatchedJets/Truth");      // [5.5] truth jets matched with calorimeter jets
  JetAnalysisGroup truthUnmatchedSel("Selected/UnmatchedJets/Truth");  // [5.6] truth jets not matched with calorimeter jets
  JetAnalysisGroup caloMatchedSel("Selected/MatchedJets/Calo");        // [5.7] calorimeter jets matched with truth jets      
  JetAnalysisGroup caloUnmatchedSel("Selected/UnmatchedJets/Calo");    // [5.8] calorimeter jets not matched with truth jets
  //
  // The matched calorimeter jets are analyzed before pile-up suppression
  TopoAnalysisGroup topoMatchedSel("Selected/MatchedJets/Calo");       // [5.9] calorimeter jets matched with truth jets after pile-up suppression  
  //
  // Event shapes for this selection:
  TH1D* h_cjet_minv_sel  = AH::book1D<TH1D>("Selected/Jets/Calo/h_cjet_minv","Invariant Mass, Calo Jets After Cuts", imBin,imMin,imMax,   "Invariant Mass, MeV ","Entries"); 
  TH1D* h_cjet_rapgap_sel = AH::book1D<TH1D>("Selected/Jets/Calo/h_cjet_rapgap","Rapidity Gap, Calo Jets After Cuts",gapBin,gapMin,gapMax,"Rapditiy Gap}",          "Entries");
  TH2D* d_cjet_rapgap_minv_sel = AH::book2D<TH2D>("Selected/Jets/Calo/d_cjet_rapgap_minv","Rapidity gap vs Invariant Mass After Cuts",imBin,imMin,imMax,gapBin,gapMin,gapMax,
						  "M_{inv} [GeV]","#Delta y");
  TH1D* h_mdistSel = AH::book1D<TH1D>("Selected/Jets/Truth/lept_mindist","Minimum #DeltaR^{truth}(lepton,jet)",320,0.,6.4,"#DeltaR","Entries");
  //
  // [6][Jet-area-based pile-up suppression]
  //
  // Without changing the finally selected event sample, the effects of the jet-area-based pile-up suppression are studied. 
  //
  JetAnalysisGroup caloJetPUAreaSel("Selected/Jets/Calo/PUArea");                        // [6.1] calorimeter jets after PU suppression
  JetAnalysisGroup caloSignalJetPUAreaSel("Selected/MatchedJets/Calo/PUArea");           // [6.2] calorimeter jets after PU suppression matched with truth jets (signal jet candidates)
  JetAnalysisGroup caloPileupJetPUAreaSel("Selected/UnmatchedJets/Calo/PUArea");         // [6.3] calorimeter jets after PU suppression not matched with truth jets (PU jet candidates)
  //
  // The matched jets are analyzed with respect to topology and kinematic features.
  TopoAnalysisGroup topoPUAreaSel("Selected/MatchedJets/Calo/PUArea");                   // [6.4] calorimeter jets matched with truth jets after pile-up suppression
  //
  TwoJetAnalysisGroup tagJetsSel("Selected/Jets/Calo/PUArea/Signal");                            // [6.5] topology of signal jet candidates
  TwoJetAnalysisGroup tagMatchedJetsSel("Selected/MatchedJets/Calo/PUArea/Signal");              // [6.6] topology of signal jet candidates with truth match
  TwoJetAnalysisGroup tagUnmatchedJetsSel("Selected/UnmatchedJets/Calo/PUArea/Signal");          // [6.7] topology of signal jet candidates without truth match
  TwoJetAnalysisGroup tagSingleMatchedJetsSel("Selected/SingleMatchedJets/Calo/PUArea/Signal");  // [6.8] topology of signal jet candidates without truth match

  ///////////////////
  // Analysis cuts //
  ///////////////////

  // read external configuration file
  if ( !_cfgRead ) { 
    if  ( !_cfgFile.empty() && _cfgFile !="" ) { 
      AH::readCfg(_cfgFile); 
    } else { 
      PRINT_INFO(_mname,"default configuration");
    }
  }

  // figure out which calorimeter signal we will use
  //
  // TopoCluster      default calorimeter signal, topological cell clusters on EM or LCW (hadronic) scale
  // TopoTower        tower signal on a grid of delta_y X delta_phi = 0.1 X 0.1   (uses only cells from TopoCluster)
  // TopoTowerFine    tower signal on a grid of delta_y X delta_phi = 0.05 X 0.05 (uses only cells from TopoCluster)
  //
  bool doTopoCluster(false);                        
  bool doTopoTower(false);
  bool doTopoTowerFine(false);
  double caloArea(0.);
  PRINT_INFO(_mname,"#-----------------------------------------");      ////////////////////////////////////////////////////////
  if ( Analysis::Config::Input::isTopoCluster() ) {                     // Pick up the chosen calorimeter signal definition   //
    PRINT_INFO(_mname,"# analyzing TopoCluster");                       // from the configuration (only one per job possible) // 
    doTopoCluster = true;                                               ////////////////////////////////////////////////////////
  } else if ( Analysis::Config::Input::isTopoTower() ) {
    PRINT_INFO(_mname,"# analyzing TopoTower (0.1 x 0.1 grid)");
    caloArea = 0.1 * 0.1;
    doTopoTower = true;
  } else if ( Analysis::Config::Input::isTopoTowerFine() ) {
    PRINT_INFO(_mname,"# analyzing TopoTower (0.05 x 0.05 grid)");
    caloArea = 0.05 * 0.05;
    doTopoTowerFine = true;
  } else {
    PRINT_ERROR(_mname,"# no valid signal input type!!!!");
    PRINT_INFO(_mname,"#-----------------------------------------");
    return;
  }
  PRINT_INFO(_mname,"#-----------------------------------------");
  
  ///////////////////////////
  // Do not change for now //
  ///////////////////////////

  // cut flow histogram
  TH1D* h_cutflow = AH::book1D<TH1D>("h_cutflow","h_cutflow",20,-0.5,19.5);
  // tags to be used as axis labels in the cut flow histogram
  std::vector<std::string> cutTags;                                                                                            /* cut index */
  cutTags.push_back(std::string("EventsIn"));                                                                                  /*      0    */
  cutTags.push_back(std::string("Data consistent"));                                                                           /*      1    */
  cutTags.push_back(std::string(TString::Format("#DeltaR_{iso}^{truth} < %.1f",Analysis::Select::TruthJet::isoDeltaR).Data()));/*      2    */
  cutTags.push_back(std::string("N_{jet}^{truth} > 2"));                                                                       /*      3    */
  cutTags.push_back(std::string(TString::Format("N_{jet}^{calo} #geq %zu",Analysis::Select::RecoJet::numMin).Data()));         /*      4    */
  cutTags.push_back(std::string(TString::Format("%6.1f < M_{inv}^{calo-jet} < %6.1f GeV",                                      /*      5    */
						Analysis::Select::Event::minMinv/1000.,                                        /*           */
						Analysis::Select::Event::maxMinv/1000.).Data()));                              /*           */
  cutTags.push_back(std::string("y_{jet1}^{calo}#timesy_{jet2}^{calo}<0"));                                                    /*      6    */
  cutTags.push_back(std::string(TString::Format("#Delta y^{calo} > %3.1f",Analysis::Select::Event::rapGap).Data()));           /*      7    */
  cutTags.push_back(std::string("Exclusive Matching"));                                                                        /*      8    */

  for ( size_t i(0); i<cutTags.size(); ++i ) { h_cutflow->GetXaxis()->SetBinLabel((int)i+1,cutTags.at(i).c_str()); } 

  ////////////////////////////
  // Set up rho calculation //
  ////////////////////////////

  SlidingWindow::Window* pRhoProf = 0;
  bool doRhoProf(false);
  if ( Analysis::Config::Job::doSlidingWindowRho ) {
    pRhoProf = new SlidingWindow::Window("RhoProfile",
					 Analysis::Config::RhoProfile::nBins, Analysis::Config::RhoProfile::rapMin,Analysis::Config::RhoProfile::rapMax,
					 Analysis::Config::RhoProfile::window,Analysis::Config::RhoProfile::storeObjects);
    doRhoProf = pRhoProf != 0;
    if ( doRhoProf ) { pRhoProf->book(true); }
  } else {
    if ( doTopoTower || doTopoTowerFine ) {
      pRhoProf = new SlidingWindow::Window("RhoProfile",
					   Analysis::Config::RhoProfile::nBins, Analysis::Config::RhoProfile::rapMin,Analysis::Config::RhoProfile::rapMax,
					   Analysis::Config::RhoProfile::window,Analysis::Config::RhoProfile::storeObjects);
      if ( pRhoProf != 0 ) { pRhoProf->book(true); }
    }
  }

  ////////////////////
  // Set up filters //
  ////////////////////

  Analysis::Selector::CaloTiming* caloTimingFilter = Analysis::Config::Filter::CaloSignal::doTimeFilter 
    ? new Analysis::Selector::CaloTiming(Analysis::Select::CaloSignal::timeMin,Analysis::Select::CaloSignal::timeMax)
    : 0;

  Analysis::Selector::JetTiming* jetTimingFilter = Analysis::Config::Filter::RecoJet::doTimeFilter
    ? new Analysis::Selector::JetTiming(Analysis::Select::RecoJet::timeMin,Analysis::Select::RecoJet::timeMax)
    : 0;

  ////////////////
  // Event loop //
  ////////////////

  // check on number of events
  int nd = AH::ndigits((int)nentries);
  Long64_t nn = nreq != 0 ? nreq : (Long64_t)Analysis::Config::Input::nEvents;
  if ( nn != -1 ) { 
    if ( nn > nentries ) { 
      PRINT_INFO(_mname,"requested number of event to be analyzed %*i, available %*i, effective %*i",nd,(int)nn,nd,(int)nentries,nd,(int)nentries);
    } else {
      PRINT_INFO(_mname,"requested number of event to be analyzed %*i, available  %*i",nd,(int)nn,nd,(int)nentries);
      nentries = nn;
    }
  } else { 
    PRINT_INFO(_mname,"number of events to be analyzed %*i (from entries scan)",nd,(int)nentries);
  }

  Long64_t nbytes = 0, nb = 0;

  int ievt(0);
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    /////////////////////////
    // Allocate next event //
    /////////////////////////

    Long64_t ientry = LoadTree(jentry);

    // check if done
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // message ticker
    ievt = AH::printPulse("JetAnalysisExample::Loop",ievt,(int)nentries);

    ///////////////////////
    // Build truth event //
    ///////////////////////

    // cut flow
    /* 0 */ h_cutflow->Fill(cutTags.at(0).c_str(),1.);
 
    // use helper function to get selected truth jets 
    PRINT_DEBUG( _mname, "event %5i: try to get truth jets",(int)ievt); 
    std::vector<fastjet::PseudoJet> tjetincl = AH::truthJets(this,Analysis::Select::TruthJet::pTmin,Analysis::Select::TruthJet::rapMin,Analysis::Select::TruthJet::rapMax);
    PRINT_DEBUG( _mname, "event %5i: succeeded getting truth jets",(int)ievt); 
    // require at least one truth jet
    if ( tjetincl.size() < 2 ) { continue; }
    /* 1 */ h_cutflow->Fill(cutTags.at(1).c_str(),1.);

    // plot most inclusive truth jet sample
    truthIncl.fill(tjetincl);   // [1.1]

    // check for decay products (inclusive)
    Analysis::Matching::JetRelationContainer jdcyrel = AH::hadronicDecay(tjetincl,
									 Analysis::Select::TruthJet::rapMin,
									 Analysis::Select::TruthJet::rapMax,
									 0.,
									 std::sqrt(Analysis::Select::Event::sHat)
									 );
    // [1.3][1.4] plot mass spectrum (inclusive)
    std::vector<fastjet::PseudoJet> dcyjet; dcyjet.reserve(tjetincl.size());
    std::vector<bool> dcytag(tjetincl.size(),false);
    for ( auto jdcy : jdcyrel ) {
      size_t ii(jdcy.jetIndex(0)); size_t jj(jdcy.jetIndex(1));
      if ( ii < tjetincl.size() && jj < tjetincl.size() ) { 
	fastjet::PseudoJet pj(tjetincl.at(ii)+tjetincl.at(jj));
	h_dcymass_incl->Fill(jdcy.invM()/Analysis::Units::GeV);
	d_dcymass_rap_incl->Fill(pj.rap(),jdcy.invM()/Analysis::Units::GeV); 
	if ( !dcytag.at(ii) ) { dcyjet.push_back(tjetincl.at(ii)); dcytag[ii] = true; }
	if ( !dcytag.at(jj) ) { dcyjet.push_back(tjetincl.at(jj)); dcytag[jj] = true; }
      }
    }  
    dcyTruthIncl.fill(dcyjet);  // [1.2]

    // check for decay products in restricted phase space
    jdcyrel = AH::hadronicDecay(tjetincl,
				Analysis::Select::Event::Decay::rapMin, 
				Analysis::Select::Event::Decay::rapMax, 
				Analysis::Select::Event::Decay::minMinv, 
				Analysis::Select::Event::Decay::maxMinv);
 
    // plot mass spectrum (removed decays)
    dcyjet.clear();
    std::fill(dcytag.begin(),dcytag.end(),false);
    std::vector<double> dcyrap; dcyrap.reserve(jdcyrel.size());
    std::vector<double> dcymss; dcymss.reserve(jdcyrel.size());
    for ( auto jdcy : jdcyrel ) {
      size_t ii(jdcy.jetIndex(0)); size_t jj(jdcy.jetIndex(1));
      if ( ii < tjetincl.size() && jj < tjetincl.size() ) { 
	fastjet::PseudoJet pj(tjetincl.at(ii)+tjetincl.at(jj));
	h_dcymass_ini->Fill(jdcy.invM()/Analysis::Units::GeV);
	d_dcymass_rap_ini->Fill(pj.rap(),jdcy.invM()/Analysis::Units::GeV);
	dcyrap.push_back(pj.rap()); dcymss.push_back(jdcy.invM()/Analysis::Units::GeV);
	if ( !dcytag.at(ii) ) { dcyjet.push_back(tjetincl.at(ii)); dcytag[ii] = true; }
	if ( !dcytag.at(jj) ) { dcyjet.push_back(tjetincl.at(jj)); dcytag[jj] = true; }
      }
    }  
    dcyTruthWndw.fill(dcyjet); // [1.3]

    std::vector<fastjet::PseudoJet> tjet = AH::removeDecay(tjetincl, jdcyrel);

    // check if there are still enough truth jets after decay product removal
    if ( tjet.size() < 2 ) { continue; }
    /* 2 */ h_cutflow->Fill(cutTags.at(2).c_str(),1.);
    // check truth jet self-isolation
    if ( !AH::checkJetIsolation(tjet,Analysis::Select::TruthJet::isoDeltaR) ) { continue; }
    /* 3 */ h_cutflow->Fill(cutTags.at(3).c_str(),1.);

    /////////////////////////////
    // Get final state leptons //
    /////////////////////////////

    std::vector<fastjet::PseudoJet> lept = AH::PseudoJetGetter::getTruthParticles(this,"leptons");
    PRINT_DEBUG( _mname, "event %i5: found %2zu truth leptons",(int)ievt,lept.size());

  
    //////////////////////////////////////////////////////////////////
    // Get the correct calorimeter signal and the corresponding jet //
    //////////////////////////////////////////////////////////////////

    std::vector<fastjet::PseudoJet> calosignal;  // all TopoCluster/TopoTower/TopoTowerFine objects
    std::vector<fastjet::PseudoJet> calojet;     // all jets reconstructed from the TopoXXXX signals                                   
    if ( doTopoCluster ) { 
      if ( doRhoProf ) { 
	calosignal = AH::PseudoJetGetter::getTopoClusters(this,"lcw",true,calojet);
	// check if filters are to be applied
	if ( Analysis::Config::Filter::CaloSignal::doTimeFilter ) { 
	  calosignal = AH::filter(calosignal,*caloTimingFilter);
	  calojet    = AH::topoClusterJets(calosignal,Analysis::Select::RecoJet::pTmin,Analysis::Select::RecoJet::rapMin,Analysis::Select::RecoJet::rapMax);
	}
      } else {
	PRINT_DEBUG( _mname, "event %5i: try to get calo signals and jets",(int)ievt); 
	calosignal = AH::PseudoJetGetter::getTopoClusters(this,"lcw",true);
	if ( Analysis::Config::Filter::CaloSignal::doTimeFilter ) { calosignal = AH::filter(calosignal,*caloTimingFilter); }
	calojet    = AH::topoClusterJets(calosignal,Analysis::Select::RecoJet::pTmin,Analysis::Select::RecoJet::rapMin,Analysis::Select::RecoJet::rapMax);
	PRINT_DEBUG( _mname, "event %5i: succeeded getting calo signals (%zu) and jets (%zu)",(int)ievt,calosignal.size(),calojet.size());
      } 
    } else if ( doTopoTower ) { 
      calosignal = AH::PseudoJetGetter::getTopoTowers(this,"lcw");
      if ( Analysis::Config::Filter::CaloSignal::doTimeFilter ) { calosignal = AH::filter(calosignal,*caloTimingFilter); }
      calojet    = AH::topoTowerJets(calosignal,Analysis::Select::RecoJet::pTmin,Analysis::Select::RecoJet::rapMin,Analysis::Select::RecoJet::rapMax);
    } else if ( doTopoTowerFine ) { 
      calosignal = AH::PseudoJetGetter::getTopoTowersFine(this,"lcw");
      if ( Analysis::Config::Filter::CaloSignal::doTimeFilter ) { calosignal = AH::filter(calosignal,*caloTimingFilter); }
      calojet    = AH::topoTowerJets(calosignal,Analysis::Select::RecoJet::pTmin,Analysis::Select::RecoJet::rapMin,Analysis::Select::RecoJet::rapMax);
    }

    if ( Analysis::Config::Filter::RecoJet::doTimeFilter ) { calojet = AH::filter(calojet,*jetTimingFilter); }

    AH::printCaloSignal(_mname, calosignal);

    //////////////////////////////////////
    // Select events based on reco jets //
    //////////////////////////////////////

    PRINT_DEBUG( _mname, "event %5i: entering event selection",(int)ievt);

    // variable filled by the accept event function 
    unsigned int passlvl(0); // this tells us a bit about what happened when selecting events (bit pattern)         
    double minv(0.);         // highest invariant mass from jets
    size_t njets(2);         // this is the actual number of jets used to calculate the invariant mass
    double rapgap(-1.);      // rapidity distance between two leading jets
    bool   sepHem(false);    // flag indicating two different rapidity hemispheres
    size_t sigjet_0(0); size_t sigjet_1(0);

    // check if event 
    // ---> do not use truth   Analysis::Select::Event::FailureLvl checkPass = AH::acceptTruthJetEvent(tjet,passlvl,sigjet_0,sigjet_1,minv,rapgap);

    
    // find the two jets forming the minimum required invariant mass
    bool massOk(AH::selectMassWindow(calojet,minv,sigjet_0,sigjet_1));

    // make sure we have two calojets at least
    bool twoCaloJets((sigjet_0 < calojet.size() && sigjet_1 < calojet.size()) && sigjet_0 != sigjet_1); 

    // conditional selections
    if ( twoCaloJets ) { 
      double rap_0(calojet.at(sigjet_0).rap()); double rap_1(calojet.at(sigjet_1).rap());
      // make sure that the two jets are separated in rapidity 
      rapgap = std::abs(rap_0-rap_1); 
      // make sure the two calorimeter jets are in different rapidity hemisphere
      sepHem = ( rap_0*rap_1 < 0. ); 
    }
    bool rapGapOk(rapgap > Analysis::Select::Event::rapGap);


    ////////////////////////////////////////////////////////////////////////////
    // First round of plots (most inclusive, only very basic event selection) //
    ////////////////////////////////////////////////////////////////////////////

 
    // fill jet plots
    truthJetIni.fill(tjet);                                                // [2.1]
    dcyTruthIni.fill(dcyjet);                                              // [2.2]
    leptonIni.fill(lept);                                                  // [2.3]
    caloJetIni.fill(calojet);                                              // [2.4]
    if ( twoCaloJets ) { tagJetsIni.fill(calojet,sigjet_0,sigjet_1); }     // [2.5] (needs two calo jets) 
    // decay candidates
    for ( size_t i(0); i<dcyrap.size(); ++i ) { 
      h_dcymass_ini->Fill(dcymss.at(i));                                   // [2.6]
      d_dcymass_rap_ini->Fill(dcyrap.at(i),dcymss.at(i));                  // [2.7]
    }
    size_t itjet(0); size_t itlep(0);
    double dljMin(AH::minDist(tjet,lept,itjet,itlep));
    h_mdistIni->Fill(dljMin);                                              // [2.8]
    
    // fill event level plots
    h_cjet_minv_ini->Fill(minv/Analysis::Units::GeV);                      // [2.9]
    h_cjet_rapgap_ini->Fill(rapgap);                                       // [2.10]
    d_cjet_rapgap_minv_ini->Fill(minv/Analysis::Units::GeV,rapgap);        // [2.11]

    //////////////////////////////////////////////////////////////
    // Apply final selection without considerations for pile-up //
    //////////////////////////////////////////////////////////////

    if ( !twoCaloJets ) { continue; } 
    /* 4 */ h_cutflow->Fill(cutTags.at(4).c_str(),1.);      
    if ( !massOk ) { continue; }
    /* 5 */ h_cutflow->Fill(cutTags.at(5).c_str(),1.);
    if ( !sepHem ) { continue; }
    /* 6 */ h_cutflow->Fill(cutTags.at(6).c_str(),1.);
    if ( !rapGapOk ) { continue; }
    /* 7 */ h_cutflow->Fill(cutTags.at(7).c_str(),1.);
   
    truthJetFin.fill(tjet);                                         // [3.1]
    dcyTruthFin.fill(dcyjet);                                       // [3.2]
    leptonFin.fill(lept);                                           // [3.3]
    caloJetFin.fill(calojet);                                       // [3.4]
    tagJetsFin.fill(calojet,sigjet_0,sigjet_1);               // [3.5]
    for ( size_t i(0); i<dcyrap.size(); ++i ) { 
      h_dcymass_fin->Fill(dcymss.at(i));                           // [3.6]
      d_dcymass_rap_fin->Fill(dcyrap.at(i),dcymss.at(i));           // [3.7]
    }
    h_mdistFin->Fill(dljMin);                                       // [3.8]
    h_cjet_minv_fin->Fill(minv/Analysis::Units::GeV);               // [3.9]
    h_cjet_rapgap_fin->Fill(rapgap);                                // [3.10]
    d_cjet_rapgap_minv_fin->Fill(minv/Analysis::Units::GeV,rapgap); // [3.11]
   
    /////////////////////////////////////////////
    // Matching of all jets and classification //
    /////////////////////////////////////////////

    // fill container with matching jets
    Analysis::Matching::JetRelationContainer jrelcont;
    jrelcont.fill(tjet,calojet,Analysis::Select::MatchedJet::matchR); // this function matches the jets 
  
    // analyze matches and extract matched (signal) jets
    std::vector<fastjet::PseudoJet> tsigjet;  // matched truth jets (signal)
    std::vector<fastjet::PseudoJet> csigjet;  // matched calo jets (signal)
    for ( auto fmatch(jrelcont.begin()); fmatch!=jrelcont.end(); ++fmatch ) {
      size_t tidx(fmatch->truthIndex());
      size_t cidx(fmatch->caloIndex());
      double dR(fmatch->deltaR());
      tsigjet.push_back(tjet.at(tidx));
      csigjet.push_back(calojet.at(cidx));
      PRINT_DEBUG( _mname+"::Matching","truth jet index %4zu calo jet index %4zu delta R = %7.3f",tidx,cidx,dR);
    }

    // collect unmatched (pile-up) calo jets
    std::vector<fastjet::PseudoJet> pujet; pujet.reserve(calojet.size());
    for ( size_t i(0); i<calojet.size(); ++i ) { if ( AH::FastJet::isPileup(calojet.at(i)) ) { pujet.push_back(calojet.at(i)); } }

    // collect unmatched truth jets (detector inefficiency)
    std::vector<fastjet::PseudoJet> tisojet;
    for ( const auto& pj : tjet ) { if ( !AH::FastJet::isMatched(pj) ) { tisojet.push_back(pj); } }

    truthMatchedFin.fill(tsigjet);                                   // [3.12] truth jets matched with calorimeter jets
    truthUnmatchedFin.fill(tisojet);                                 // [3.13] truth jets not matched with calorimeter jets
    caloMatchedFin.fill(csigjet);                                    // [3.14] calorimeter jets matched with truth jets       
    caloUnmatchedFin.fill(pujet);                                    // [3.15] calorimeter jets not matched with truth jets
    topoMatchedFin.fill(jrelcont,tjet,calojet);                      // [3.16] relations between truth and matched calo jet 

    /////////////////////////
    // Classify topologies //
    /////////////////////////

    // figure out event topology before PU suppression
    //
    // Here we are using the indices of the two calo jets that form Minv and are used for rapgap to figure out if these
    ///are matched to truthjets.
    double deltaR_0(0.); size_t mtidx_0 = jrelcont.truthIndex(sigjet_0,deltaR_0); bool match_0 = mtidx_0 < tjet.size();
    double deltaR_1(0.); size_t mtidx_1 = jrelcont.truthIndex(sigjet_1,deltaR_1); bool match_1 = mtidx_1 < tjet.size();
    // signal jet matching
    bool fullSignalMatch(match_0 && match_1);                       // both signal jet candidates are matched
    bool halfSignalMatch(!fullSignalMatch && (match_0 || match_1)); // one of the signal jets is matched
    bool noSignalMatch(!fullSignalMatch);                           // no signal jet matched

    if ( fullSignalMatch) { 
      tagMatchedJetsFin.fill(calojet,sigjet_0,sigjet_1);            // [3.17]
    } else if ( halfSignalMatch ) { 
      tagSingleMatchedJetsFin.fill(calojet,sigjet_0,sigjet_1);      // [3.18]
    } else if ( noSignalMatch ) { 
      tagUnmatchedJetsFin.fill(calojet,sigjet_0,sigjet_1);          // [3.19]
    }

    ////////////////////////////////////////////
    // Apply pile-up (area-based) subtraction //
    ////////////////////////////////////////////
 
    std::vector<fastjet::PseudoJet> acceptjet; std::vector<fastjet::PseudoJet> rejectjet;        ///////////////////////////////////////////////////////////////////
    if ( doRhoProf ) {                                                                           // This block implements the jet-area-based pile-up suppression. //
      pRhoProf->reset();                                                                         ///////////////////////////////////////////////////////////////////
      for ( auto pj : calosignal ) { 
	if ( !Analysis::FastJet::UserInfo::isGhost(pj) ) { 
	  pRhoProf->addObject(pj,Analysis::FastJet::area(pj)); 
	}
      }
      acceptjet = AH::extractSignalJets(calojet,rejectjet,*pRhoProf);
    } else {
      if ( doTopoCluster ) {
	acceptjet = AH::extractSignalJets(calojet,rejectjet,AH::Event::centralRho(this),0.);
      } else {
	pRhoProf->reset();
	for ( auto pj : calosignal ) { 
	  if ( !Analysis::FastJet::UserInfo::isGhost(pj) ) { 
	    pRhoProf->addObject(pj,caloArea); 
	  }
	}  // fill calorimeter signal into rho profile
	acceptjet = AH::extractSignalJets(calojet,rejectjet,*pRhoProf,false);
      } // CaloTowers (always have a rho profile measurement)
    } // check on rho profile for clusters

    // collect signal and pile-up after suppression
    // std::vector<fastjet::PseudoJet> csigpuarea; 
    // std::vector<fastjet::PseudoJet> cplupuarea;
    // Analysis::Matching::JetRelationContainer jrelsig;
    // Analysis::Matching::JetRelationContainer jrelpup;
    // for ( const auto& pj : acceptjet ) { 
    //   if ( AH::FastJet::isSignal(pj) ) {
    // 	csigpuarea.push_back(pj);
    // 	size_t idx((size_t)Analysis::FastJet::UserInfo::dataIndex(pj));
    // 	if ( idx < calojet.size() ) {
    // 	  size_t jdx(jrelcont.truthIndex(idx));
    // 	  if ( jdx < tjet.size() ) { 
    // 	    Analysis::Matching::JetRelation jr; jr.fill(jdx,idx,jrelcont,calojet.at(idx).delta_r(tjet.at(jdx)));
    // 	    jrelsig.push_back(jr);
    // 	  }
    // 	}
    //   } else if ( AH::FastJet::isPileup(pj) ) { 
    // 	cplupuarea.push_back(pj);
    //   }
    // }

    // find matched jets in rejected and accepted jet sets
    std::vector<fastjet::PseudoJet> accsignal; accsignal.reserve(calojet.size());
    std::vector<fastjet::PseudoJet> rejsignal; rejsignal.reserve(calojet.size());
    std::vector<fastjet::PseudoJet> accpileup; accpileup.reserve(calojet.size());
    std::vector<fastjet::PseudoJet> rejpileup; rejpileup.reserve(calojet.size());
    AH::filterJets(acceptjet,accsignal,accpileup);
    AH::filterJets(rejectjet,rejsignal,rejpileup);

    caloAcceptJetPUArea.fill(acceptjet);            // [4.1]
    caloRejectJetPUArea.fill(rejectjet);            // [4.2]
    caloAcceptSignalJetPUArea.fill(accsignal);      // [4.3]
    caloRejectSignalJetPUArea.fill(rejsignal);      // [4.4]
    caloAcceptPileupJetPUArea.fill(accpileup);      // [4.5]
    caloRejectPileupJetPUArea.fill(rejpileup);      // [4.6]
    //    topoPUAreaFin.fill(jrelsig,tjet,calojet);       // [4.7]
    //   size_t isig(AH::jetIndex(acceptjet),sigjet_0); size_t jsig(AH::jetIndex(acceptjet,sigjet_1));

    //   // both jets survived pileup suppression
    //   if ( isig < calojet.size() && jsig < calojet.size() ) { 
    //     tagJetsPUArea.fill(calojet,isig,jsig);        // [4.9] both jets survived 
    //     // check if signal or pileup
    //     if ( AH::jetIndex(accsignal,isig) != size_t(-1) && AH::jetIndex(accsignal,jsig) != size_t(-1) ) {
    // 	tagMatchedJetsPUArea.fill(calojet,sigjet_0,sigjet_1);
    //     } else if ( AH::jetIndex(accpileup,isig) != size_t(-1) && AH::jetIndex(accpileup,jsig) != size_t(-1) {
    // 	  tagUnmatchedJetsPUArea.fill(calojet,
    
    //  /* [4.9] */ TwoJetAnalysisGroup tagJetsPUArea("Final/Jets/Calo/PUArea/Signal");                           // topology of signal jet candidates
    // /* [4.10]*/ TwoJetAnalysisGroup tagMatchedJetsPUArea("Final/MatchedJets/Calo/PUArea/Signal");             // topology of signal jet candidates with truth match
    // /* [4.11]*/ TwoJetAnalysisGroup tagUnmatchedJetsPUArea("Final/UnmatchedJets/Calo/PUArea/Signal");         // topology of signal jet candidates without truth match
    // /* [4.12]*/ TwoJetAnalysisGroup tagSingleMatchedJetsPUArea("Final/SingleMatchedJets/Calo/PUArea/Signal"); // topology of signal jet candidates without truth match

    // check if any matches 
    if ( tsigjet.empty() || csigjet.empty() ) { continue; }
    /* 8 */ h_cutflow->Fill(cutTags.at(8).c_str(),1.);
 
    ////////////////////////////////////////////////////////////////
    // Inclusive plots after event selection for all matched jets //
    ////////////////////////////////////////////////////////////////

    /////////////////////
    // Final selection //
    /////////////////////

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
    PRINT_INFO(_mname,"[begin] cutflow summary >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    for ( int i(1); i<=h_cutflow->GetNbinsX(); ++i ) {
      std::string tag(h_cutflow->GetXaxis()->GetBinLabel(i));
      if ( !tag.empty() && tag !="" ) { 
	PRINT_INFO(_mname,"events after cut %*s (bin %2i): %*i (%5.1f%%)",
	       (int)kd,tag.c_str(),i,nd,(int)h_cutflow->GetBinContent(i),h_cutflow->GetBinContent(i)/ie*100.);
      }
    }
    PRINT_INFO(_mname,"[end]   Cutflow summary <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
  }

  // write histograms to output file
  if ( doWrite ) { 
    PRINT_INFO( _mname+"[Write]", "write %i histograms", AH::writeHists() );
    AH::closeFile();
  }
}
