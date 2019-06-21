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

/////////////////////////////////////////////////
// Do not touch the constructor implementation //
/////////////////////////////////////////////////

JetAnalysisExample::JetAnalysisExample(TChain* tchain,const std::string& cfgFile) : fChain(0), _cfgFile(cfgFile), _cfgRead(false), _isDead(false)
{
  if ( tchain == 0 ) {
    PRINT_ERROR( "JetAnalysisExample::JetAnalysisExample", "invald pointer to TChain (%p)", (void*)tchain );
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
  int gapBin(rapBin); double gapMin(0.); double gapMax(2.*rapMax);

  // delta R binning
  int drBin(100); double drMin(0.); double drMax(2.);

  // PtD binning
  int ptdBin(110); double ptdMin(-0.05); double ptdMax(1.05);
  
  // PtD ratio binning
  int rptdBin(250); double rptdMin(-0.5); double rptdMax(24.5);

  // [Inclusive] >>------------------------------------------------------------------------------------------------------------------------------------------------------------
  //Truth Jet Histograms//
  TH1D* h_tjet_n_incl   = AH::book1D<TH1D>("Inclusive/h_tjet_n_incl", "Number of Truth Jets",   njBin,njMin,njMax,   "N_{jet}^{truth}",        "Entries");
  TH1D* h_tjet_pt_incl  = AH::book1D<TH1D>("Inclusive/h_tjet_pt_incl","Transverse Momentum, Truth Jets", ptBin,ptMin,ptMax,   "p_{T}^{truth-jet} [GeV]","Entries"); 
  TH1D* h_tjet_rap_incl = AH::book1D<TH1D>("Inclusive/h_tjet_rap_incl","Rapidity, Truth Jets",rapBin,rapMin,rapMax,"y^{truth-jet}",          "Entries");
  // TH1D* h_tjet_minv_incl  = AH::book1D<TH1D>("Inclusive/h_tjet_minv_incl","Invariant Mass, Truth Jets", imBin,imMin,imMax,   "Invariant Mass, MeV ","Entries"); 
  // TH1D* h_tjet_rapgap_incl = AH::book1D<TH1D>("Inclusive/h_tjet_rapgap_incl","Rapidity Gap, Truth Jets",gapBin,gapMin,gapMax,"Rapditiy Gap}",          "Entries");
  //Calo Jet Histograms//
  TH1D* h_cjet_n_incl   = AH::book1D<TH1D>("Inclusive/h_cjet_n_incl", "Number of Calo Jets",   njBin,njMin,njMax,   "N_{jet}^{calo}",        "Entries");
  TH1D* h_cjet_pt_incl  = AH::book1D<TH1D>("Inclusive/h_cjet_pt_incl","Transverse Momentum, Calo Jets", ptBin,ptMin,ptMax,   "p_{T}^{calo-jet} [GeV]","Entries"); 
  TH1D* h_cjet_rap_incl = AH::book1D<TH1D>("Inclusive/h_cjet_rap_incl","Rapidity, Truth Jets",rapBin,rapMin,rapMax,"y^{calo-jet}",          "Entries");
  TH1D* h_cjet_minv_incl  = AH::book1D<TH1D>("Inclusive/h_cjet_minv_incl","Invariant Mass, Calo Jets", imBin,imMin,imMax,   "Invariant Mass, MeV ","Entries"); 
  TH1D* h_cjet_rapgap_incl = AH::book1D<TH1D>("Inclusive/h_cjet_rapgap_incl","Rapidity Gap, Calo Jets",gapBin,gapMin,gapMax,"Rapditiy Gap}",          "Entries");
  TH2D* d_cjet_rapgap_minv_incl = AH::book2D<TH2D>("Inclusive/d_cjet_rapgap_minv_incl","Rapidity gap vs Invariant Mass",imBin,imMin,imMax,gapBin,gapMin,gapMax,
						   "M_{inv} [GeV]","#Delta y");


  // [Inclusive] <<------------------------------------------------------------------------------------------------------------------------------------------------------------
  // [Final] >>--------------------------------------------------------------------------------------------------------------------------------------------------------------
  TH1D* h_tjet_n_fin   = AH::book1D<TH1D>("Final/h_tjet_n_fin", "Number of Truth Jets After Cuts",   njBin,njMin,njMax,   "N_{jet}^{truth}",        "Entries");
  TH1D* h_tjet_pt_fin  = AH::book1D<TH1D>("Final/h_tjet_pt_fin","Transverse Momentum, Truth Jets After Cuts", ptBin,ptMin,ptMax,   "p_{T}^{truth-jet} [GeV]","Entries"); 
  TH1D* h_tjet_rap_fin = AH::book1D<TH1D>("Final/h_tjet_rap_fin","Rapidity, Truth Jets After Cuts",rapBin,rapMin,rapMax,"y^{truth-jet}",          "Entries");
  TH1D* h_cjet_n_fin   = AH::book1D<TH1D>("Final/h_cjet_n_fin", "Number of Calo Jets After Cuts",   njBin,njMin,njMax,   "N_{jet}^{calo}",        "Entries");
  TH1D* h_cjet_pt_fin  = AH::book1D<TH1D>("Final/h_cjet_pt_fin","Transverse Momentum, Calo Jets After Cuts", ptBin,ptMin,ptMax,   "p_{T}^{calo-jet} [GeV]","Entries"); 
  TH1D* h_cjet_rap_fin = AH::book1D<TH1D>("Final/h_cjet_rap_fin","Rapidity, Truth Jets After Cuts",rapBin,rapMin,rapMax,"y^{calo-jet}",          "Entries");
  TH1D* h_cjet_minv_fin  = AH::book1D<TH1D>("Final/h_cjet_minv_fin","Invariant Mass, Calo Jets After Cuts", imBin,imMin,imMax,   "Invariant Mass, MeV ","Entries"); 
  TH1D* h_cjet_rapgap_fin = AH::book1D<TH1D>("Final/h_cjet_rapgap_fin","Rapidity Gap, Calo Jets After Cuts",gapBin,gapMin,gapMax,"Rapditiy Gap}",          "Entries");
  TH2D* d_cjet_rapgap_minv_fin = AH::book2D<TH2D>("Final/d_cjet_rapgap_minv_fin","Rapidity gap vs Invariant Mass After Cuts",imBin,imMin,imMax,gapBin,gapMin,gapMax,
						   "M_{inv} [GeV]","#Delta y");
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
  PRINT_INFO(_mname,"#-----------------------------------------");      ////////////////////////////////////////////////////////
  if ( Analysis::Config::Input::isTopoCluster() ) {                     // Pick up the chosen calorimeter signal definition   //
    PRINT_INFO(_mname,"# analyzing TopoCluster");                       // from the configuration (only one per job possible) // 
    doTopoCluster = true;                                               ////////////////////////////////////////////////////////
  } else if ( Analysis::Config::Input::isTopoTower() ) {
    PRINT_INFO(_mname,"# analyzing TopoTower (0.1 x 0.1 grid)");
    doTopoTower = true;
  } else if ( Analysis::Config::Input::isTopoTowerFine() ) {
    PRINT_INFO(_mname,"# analyzing TopoTower (0.05 x 0.05 grid)");
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
  cutTags.push_back(std::string(TString::Format("N_{jet}^{calo} #geq %zu",Analysis::Select::RecoJet::numMin).Data()));         /*      2    */
  cutTags.push_back(std::string(TString::Format("%6.1f < M_{inv}^{calo-jet} < %6.1f GeV",                                      /*      3    */
						Analysis::Select::Event::minMinv/1000.,                                        /*           */
						Analysis::Select::Event::maxMinv/1000.).Data()));                              /*           */
  cutTags.push_back(std::string(TString::Format("#Delta y^{calo} > %3.1f",Analysis::Select::Event::rapGap).Data()));           /*      4    */
  // cutTags.push_back(std::string("y_{jet,1}^{truth} #times y_{jet,2}^{truth} < 0"));                                            /*      4    */
  // cutTags.push_back(std::string(TString::Format("dR_{iso}^{truth} > %3.1f",Analysis::Select::TruthJet::isoDeltaR).Data()));    /*      5    */
  // cutTags.push_back(std::string("Invalid indices"));                                                                           /*      7    */ 
  // cutTags.push_back(std::string(TString::Format("N_{jet}^{matched} #geq %zu",Analysis::Select::MatchedJet::numMin).Data()));   /*      8    */
  // cutTags.push_back(std::string(TString::Format("No matching jets for signal").Data()));                                       /*      9    */
  // cutTags.push_back(std::string(TString::Format("N_{jet}^{filtered} #geq %zu",Analysis::Select::MatchedJet::numMin).Data()));  /*     10    */

  for ( size_t i(0); i<cutTags.size(); ++i ) { h_cutflow->GetXaxis()->SetBinLabel((int)i+1,cutTags.at(i).c_str()); } 

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

    //////////////////////
    // Build truth jets //
    //////////////////////

    // cut flow
    /* 0 */ h_cutflow->Fill(cutTags.at(0).c_str(),1.);
 
    // use helper function to get selected truth jets 
    PRINT_DEBUG( _mname, "event %5i: try to get truth jets",(int)ievt); 
    std::vector<fastjet::PseudoJet> tjet = AH::truthJets(this,Analysis::Select::TruthJet::pTmin,Analysis::Select::TruthJet::rapMin,Analysis::Select::TruthJet::rapMax);
    PRINT_DEBUG( _mname, "event %5i: succeeded getting truth jets",(int)ievt); 
    // require at least one truth jet
    if ( tjet.size() < 2 ) { continue; }
    /* 1 */ h_cutflow->Fill(cutTags.at(1).c_str(),1.);

    //////////////////////////////////////////////////////////////////
    // Get the correct calorimeter signal and the corresponding jet //
    //////////////////////////////////////////////////////////////////

    std::vector<fastjet::PseudoJet> calosignal;  // all TopoCluster/TopoTower/TopoTowerFine objects
    std::vector<fastjet::PseudoJet> calojet;     // all jets reconstructed from the TopoXXXX signals                                   
    if ( doTopoCluster ) { 
      PRINT_DEBUG( _mname, "event %5i: try to get calo signals and jets",(int)ievt); 
      calosignal = AH::PseudoJetGetter::getTopoClusters(this,"lcw",true);
      calojet    = AH::topoClusterJets(this,"lcw",Analysis::Select::RecoJet::pTmin);
      PRINT_DEBUG( _mname, "event %5i: succeeded getting calo signals (%zu) and jets (%zu)",(int)ievt,calosignal.size(),calojet.size()); 
    } else if ( doTopoTower ) { 
      calosignal = AH::PseudoJetGetter::getTopoTowers(this,"lcw");
      calojet    = AH::topoTowerJets(this,"lcw",Analysis::Select::RecoJet::pTmin);
    } else if ( doTopoTowerFine ) { 
      calosignal = AH::PseudoJetGetter::getTopoTowersFine(this,"lcw");
      calojet    = AH::topoTowerFineJets(this,"lcw",Analysis::Select::RecoJet::pTmin);
    }

    //////////////////////////////////////
    // Select events based on reco jets //
    //////////////////////////////////////

    PRINT_DEBUG( _mname, "event %5i: entering event selection",(int)ievt);

    // variable filled by the accept event function 
    unsigned int passlvl(0); // this tells us a bit about what happened when selecting events (bit pattern)         
    double minv(0.);         // highest invariant mass from jets
    size_t njets(2);         // this is the actual number of jets used to calculate the invariant mass
    double rapgap(-1.);      // rapidity distance between two leading jets
    size_t ijet(0); size_t jjet(0);

    // check if event 
     // ---> do not use truth   Analysis::Select::Event::FailureLvl checkPass = AH::acceptTruthJetEvent(tjet,passlvl,ijet,jjet,minv,rapgap);

    
    // find the two jets forming the minimum required invariant mass
    bool massOk(AH::invariantMass(calojet,minv,ijet,jjet));         // true if mass within required window/above required threshold

    // make sure we have two calojets at least
    bool twoCaloJets((ijet < calojet.size() && jjet < calojet.size()) && ijet != jjet); 

    // make sure that the two jets are separated in rapidity 
    if ( twoCaloJets ) { rapgap = std::abs(calojet.at(ijet).rap()-calojet.at(jjet).rap()); }
    bool rapGapOk(rapgap > Analysis::Select::Event::rapGap);

    ////////////////////////////////////////////////////////////////////////////
    // First round of plots (most inclusive, only very basic event selection) //
    ////////////////////////////////////////////////////////////////////////////

 
    // fill event level plots

    // here we want plots of:
    //
    // (*) number of truth jets      (ntjets)                      //////////////////////////////////////////////////////////////////////////////
    // (*) number of calo jets       (ncjets)                      //Done, saved as [number of events]_[histo-type][Truth Jet OR Calo Jet].pdf.//
    // (*) truth jet pT spectrum     (loop on tjet)                //////////////////////////////////////////////////////////////////////////////
    // (*) calo jet pT               (loop on calojet)
    // (*) truth jet rapidity        (loop on tjet)
    // (*) calo jets rapidity        (loop on calojet)
    // (*) invariant mass            (minv)
    // (*) rapidity gap              (rapgap)

    // number of truth jets
    double ntjets = (double)tjet.size(); 
    h_tjet_n_incl->Fill(ntjets);
  
    // number of calo jets
    double ncjets = (double)calojet.size(); 
    h_cjet_n_incl->Fill(ncjets);
 
    // truth jet loop
    for ( auto tj : tjet ) { 
      double pT(tj.pt()); 
      double rap(tj.rap()); 
      //  double minv(tj.minv()); 
      //  double rapgap(tj.rapgap()); 
      h_tjet_pt_incl->Fill(pT/1000.); // pT/Units::GeV ... double mypt = 100*Units::GeV;
      h_tjet_rap_incl->Fill(rap);
     }
 
    // invariant mass (truth)
    h_cjet_minv_incl->Fill(minv/1000.);

    // rapidity gap (truth)
    h_cjet_rapgap_incl->Fill(rapgap);

     // relation between rapidity gap and inv mass
    d_cjet_rapgap_minv_incl->Fill(minv/1000.,rapgap);

    // calorimeter jet loop
    for ( auto cj : calojet ) { 
      double pT(cj.pt());
      double rap(cj.rap());
      //  double minv(cj.minv()); 
      //  double rapgap(cj.rapgap());
      h_cjet_pt_incl->Fill(pT/1000.);
      h_cjet_rap_incl->Fill(rap);
    }

    if ( !twoCaloJets ) { continue; } 
    /* 2 */ h_cutflow->Fill(cutTags.at(2).c_str(),1.);
    if ( !massOk ) { continue; }
    /* 3 */ h_cutflow->Fill(cutTags.at(3).c_str(),1.);
    if ( !rapGapOk ) { continue; }
    /* 4 */ h_cutflow->Fill(cutTags.at(4).c_str(),1.);

    /////////////////////
    // Final selection //
    /////////////////////

    // number of truth jets
    //double ntjets = (double)tjet.size(); 
    h_tjet_n_fin->Fill(ntjets);
  
    // number of calo jets
    //double ncjets = (double)calojet.size(); 
    h_cjet_n_fin->Fill(ncjets);
 
    // truth jet loop
    for ( auto tj : tjet ) { 
      double pT(tj.pt()); 
      double rap(tj.rap()); 
      //  double minv(tj.minv()); 
      //  double rapgap(tj.rapgap()); 
      h_tjet_pt_fin->Fill(pT/1000.); // pT/Units::GeV ... double mypt = 100*Units::GeV;
      h_tjet_rap_fin->Fill(rap);
     }
 
    // invariant mass (truth)
    h_cjet_minv_fin->Fill(minv/1000.);

    // rapidity gap (truth)
    h_cjet_rapgap_fin->Fill(rapgap);

     // relation between rapidity gap and inv mass
    d_cjet_rapgap_minv_fin->Fill(minv/1000.,rapgap);

    // calorimeter jet loop
    for ( auto cj : calojet ) { 
      double pT(cj.pt());
      double rap(cj.rap());
      //  double minv(cj.minv()); 
      //  double rapgap(cj.rapgap());
      h_cjet_pt_fin->Fill(pT/1000.);
      h_cjet_rap_fin->Fill(rap);
    }

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
  int nhists = AH::writeHists(Analysis::Config::Output::file);
}
