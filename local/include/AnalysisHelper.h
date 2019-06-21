// -*- c++ -*-
#ifndef ANALYSISHELPERS_H
#define ANALYSISHELPERS_H

#include "AnalysisCfg.h"
#include "AnalysisData.h"
#include "AnalysisUtils.h"
#include "AnalysisBanner.h"

#include "SlidingWindow.h"
#include "SelectorBase.h"

#include "HistUtils.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/Selector.hh"

#include "fastjet/contrib/ConstituentSubtractor.hh"
#include "fastjet/contrib/SoftKiller.hh"

#include <vector>
#include <map>
#include <string>
#include <list>
#include <algorithm>
#include <cmath>
#include <limits>
#include <fstream>

#include <boost/tuple/tuple.hpp>

#include <TH1.h>
#include <TFile.h>

///////////////////////////////////////////////
// DO NOT TOUCH THE FOLLOWING DEFINITIONS!!! //
///////////////////////////////////////////////
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
#define _IMPL_PJ_EXTRACT( TAG, TYPE )					\
  fastjet::PseudoJet pj;						\
  pj.set_user_index(idx);						\
  if ( idx >= data-> TAG##_pt->size() ) {					\
    pj = AnalysisHelper::CaloSignal::pseudoJet_empty();			\
    Analysis::FastJet::CaloSignalInfo* ci = new Analysis::FastJet::CaloSignalInfo(pj.user_info<Analysis::FastJet::CaloSignalInfo>()); \
    ci->setDataIndex(idx);						\
    ci->setFaulty();							\
    pj.set_user_info(ci);						\
    return pj;								\
  }									\
  double e(data-> TAG##_e->at(idx));					\
  double pt(data-> TAG##_pt->at(idx));					\
  double eta(data-> TAG##_eta->at(idx));				\
  double phi(data-> TAG##_phi->at(idx));				\
  if ( check && e < Analysis::Select::CaloSignal::eMinReco ) {		\
    pj = AnalysisHelper::CaloSignal::pseudoJet_empty( "Unknown", idx);	\
    Analysis::FastJet::CaloSignalInfo* ci = new Analysis::FastJet::CaloSignalInfo(pj.user_info<Analysis::FastJet::CaloSignalInfo>()); \
    ci->setDataIndex(idx);						\
    ci->setType( TYPE );						\
    ci->setOrigE(e);							\
    if ( e < 0. ) {							\
      pj.reset_PtYPhiM(fastjet::gas::def_mean_ghost_pt*1000.,eta,phi,0.); \
    } else { 								\
      pj.reset_PtYPhiM(pt,eta,phi,0.);					\
    }									\
    pj.set_user_info(ci);						\
    return pj;								\
  }									\
  Analysis::FastJet::CaloSignalInfo* ci = new Analysis::FastJet::CaloSignalInfo( idx, TYPE ); \
  ci->setDataIndex(idx);							\
  if ( e < 0. ) {							\
    pj.reset_PtYPhiM(fastjet::gas::def_mean_ghost_pt*1000.,eta,phi,0.);	\
    ci->setGhost();							\
    ci->setOrigE(e);							\
  } else { 								\
    pj.reset_PtYPhiM(pt,eta,phi,0.);					\
    PRINT_DEBUG("PseudoJetBuilder (reset)","user index %i",pj.user_index()); \
    ci->setOrigE(e);							\
  }									\
  pj.set_user_index(idx);						\
  pj.set_user_info(ci);							\
  return pj

#define _IMPL_PJ_VECT_EXTRACT( TAG, SCALE, IFLG )			\
  std::vector<fastjet::PseudoJet> pjVec; pjVec.reserve(data-> TAG##_pt->size()); \
  for ( size_t idx(0); idx<data-> TAG##_e->size() ; ++idx ) {		\
    fastjet::PseudoJet pj = pseudoJet_##SCALE( data, idx, ! IFLG );	\
    if ( !AnalysisHelper::FastJet::isEmpty(pj) && ( pj.rap() > Analysis::Select::CaloSignal::rapMin && pj.rap() < Analysis::Select::CaloSignal::rapMax ) ) { pjVec.push_back( pj ); } \
  }									\
  return pjVec
#define _CFG_STR( STRG, NAME, VALUE, FLAG )				\
  if ( Analysis::Utils::String::fetchValue( STRG, #NAME, VALUE ) ) {	\
    std::string pstr( #NAME );						\
    PRINT_INFO( "AnalysisHelper::readCfg","# property %-s %-24s [%s]", Analysis::Utils::String::fill(pstr,64).c_str(), VALUE.c_str(), NAME.c_str() ); \
    NAME = VALUE;							\
    FLAG = true;							\
  }
#define _CFG_LOG( STRG, NAME, VALUE, FLAG )				\
  if ( Analysis::Utils::String::fetchValue( STRG, #NAME, VALUE ) ) {	\
    std::string pstr( #NAME );						\
    std::string tag = NAME ? "true" : "false";				\
    PRINT_INFO( "AnalysisHelper::readCfg","# property %-s %-24s [%s]", Analysis::Utils::String::fill(pstr,64).c_str(), VALUE.c_str(), tag.c_str() ); \
    NAME = VALUE == "true";						\
    FLAG + true;							\
  }
#define _CFG_ENM( STRG, NAME, VALUE, FLAG )				\
  if ( Analysis::Utils::String::fetchValue( STRG, #NAME, VALUE ) ) {	\
    std::string pstr( #NAME );						\
    std::string tstr(Analysis::Config::Input::selectorName( NAME ));	\
    Analysis::Print::Info("AnalysisHelper::readCfg","# property %-s %-24s [%s]", Analysis::Utils::String::fill(pstr,64).c_str(), VALUE.c_str(), tstr.c_str() ); \
    FLAG = true;							\
  }
#define _CFG_INT( STRG, NAME, VALUE, FLAG )				\
  if ( Analysis::Utils::String::fetchValue( STRG, #NAME, VALUE ) ) {	\
    std::string pstr( #NAME );						\
    Analysis::Print::Info("AnalysisHelper::readCfg","# property %-s %-24i [%i]", Analysis::Utils::String::fill(pstr,64).c_str(), VALUE, NAME ); \
    NAME = VALUE ;							\
    FLAG = true;							\
  }
#define _CFG_DBL( STRG, NAME, VALUE, FLAG )			     \
  if ( Analysis::Utils::String::fetchValue( STRG, #NAME, VALUE ) ) { \
    std::string pstr( #NAME );						\
    Analysis::Print::Info("AnalysisHelper::readCfg","# property %-s %-24.3g [%.3g]", Analysis::Utils::String::fill(pstr,64).c_str(), VALUE, NAME ); \
    NAME = VALUE ;							\
    FLAG = true;							\
  }
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

///@copydoc Analysis 
namespace Analysis {
  ///@brief Internal references
  ///
  /// @warning Do not use directly - these are purely to avoid calling and handling exceptions
  namespace InternalRef {
    ///@brief Modifiable references
    ///
    /// Arbitrary/unspecified data stores.
    namespace Untrusted { 
      ///@brief @c fastjet::PseudoJet vector
      static std::vector<fastjet::PseudoJet> pjStore = std::vector<fastjet::PseudoJet>();
    }
    ///@brief Non-modifiable references
    ///
    /// Empty data stores.
    namespace Trusted { 
      ///@brief @c fastjet::PseudoJet vector
      static const std::vector<fastjet::PseudoJet> pjStore = std::vector<fastjet::PseudoJet>();
    }
  }
  ///@copydoc Analysis::FastJet
  namespace FastJet {
    ///@brief Ghosted @c fastjet::PseudoJet specifications
    namespace Ghosts {
      static double nominalPt = fastjet::gas::def_mean_ghost_pt*1000.; ///< Nominal @f$ p_{\rm T} @f$ [MeV]
      static double filterPt  = 1.1*nominalPt;                         ///< filter @f$ p_{\rm T} @f$ threshold [MeV] 
    }
  } 
}

///////////////////////
// Histogram manager //
///////////////////////

/// @brief Histogram utilities
namespace HistUtils {
  /// @brief Histogram manager
  ///
  /// This object manages histograms (presently collects them for write to output file). 
  /// Implemented as a singleton (one object instance/job only. Typically used in the background. 
  class HistMgr
  {
  private:
    /// @brief Pointer to only object
    static HistMgr*        _instance;
    /// @brief List of known histograms 
    static std::list<TH1*> _hlist;
    /// @brief Protected constructor for singleton pattern
    HistMgr() { }
  public:
    /// @brief Destructor
    ~HistMgr() { _hlist.clear(); }
    /// @brief Access to object
    static HistMgr* instance()               { if ( _instance == 0 ) { _instance = new HistMgr(); } return _instance; }
    /// @name Histogram cache management
    ///@{
    /// @brief Add a histogram to cache
    static void     add(TH1* hptr)           { if ( hptr != 0 ) { _hlist.push_back(hptr); } }
    /// @brief Remove a histogram from cache
    static void     remove(TH1* hptr)        { auto itr = std::find(_hlist.begin(),_hlist.end(),hptr); if ( itr != _hlist.end() ) { _hlist.erase(itr); } }
    /// @brief Clear cache
    static void     clear()                  { _hlist.clear(); }
    /// @brief Write known histograms
    ///
    /// @return Number of histograms written.
    /// @param noempty do not write empty histograms if @c true (optional, default @c true)
    static size_t   write(bool noempty=true) {  
      TFile* f = HistUtils::getFile();
      if ( f != 0 ) { f->cd(); }
      size_t nctr(0); for ( auto hptr : _hlist ) { if ( hptr != 0 ) { if ( !noempty || hptr->GetEntries()>0 ) { hptr->Write(); ++nctr; } } } return nctr; 
    }
    ///@}
  };
} // HistUtils

HistUtils::HistMgr* HistUtils::HistMgr::_instance = 0;          ///< Singleton pattern pre-set 
std::list<TH1*> HistUtils::HistMgr::_hlist = std::list<TH1*>(); ///< Histogram pointer cache

//-////////////////////
// Analysis helpers //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-////////////////////

/// @brief Collection of useful functions to support analysis
///
/// The @c AnalysisHelper structure can also be referred to as @c AH .
struct AnalysisHelper
{
  //-/////////////////////
  // Configuration file //
  //-/////////////////////

  /// @brief Read configuration file
  ///
  ///
  /// This function reads the configuration file and interprets the statements in it. Format:
  ///@code
  /// <variable name> = <value>
  ///@endcode
  /// where @c <variable name> is the full name of the variable to be configured (including full scope). 
  /// Lines starting with @c # are considered comments and therefore ignored by the parser.
  ///
  /// @return @c true if configuration file could be opened and read
  /// @param  fname name of configuration file (full path if needed)
  static bool readCfg(const std::string& fname)
  {
    static std::string _frame = std::string(80,'#');        ///< Frame for print-out
    static std::string _mname = "AnalysisHelper::readCfg";  ///< Name of module
    
    // open configuration file
    printf("\n");
    std::ifstream cfg;
    cfg.open(fname.c_str());
    if ( cfg.fail() ) {
      PRINT_WARNING( _mname,"%s", _frame.c_str() );
      PRINT_WARNING( _mname,"# failed to open configuration file \042%s\042", fname.c_str() );
      PRINT_WARNING( _mname,"%s", _frame.c_str() );
      printf("\n");
      return false;
    }
    
    PRINT_INFO( _mname, "%s", _frame.c_str() );
    // read configuration file
    PRINT_INFO( _mname, "# configuration file \042%s\042 opened for reading", fname.c_str() );
    std::string mstr;
    bool hasChanged(false);
    do {
      std::getline(cfg,mstr);
      if ( !cfg.eof() ) { 
	// check known configurations
	bool owflg(false); std::string sval; int ival(0); double xval(0.);
	_CFG_ENM( mstr, Analysis::Config::Input::collection, sval, owflg );
	if ( owflg ) { 
	  Analysis::Config::Input::Selector isel = Analysis::Config::Input::selectorType(sval);
	  if ( isel == Analysis::Config::Input::Unknown ) {
	    PRINT_WARNING( _mname, "# configuration requests unknown \042%s\042 input collection type [default \042%s\042 used]",
			   sval.c_str(), Analysis::Config::Input::selectorName(Analysis::Config::Input::collection).c_str() );
	  } else {
	    Analysis::Config::Input::collection = isel;
	    hasChanged = true; 
	  }
	} // enumerator special
	// pick up configuration parameters
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Config::Job::printDebug,                                      sval, owflg ); }
	if ( !owflg ) { _CFG_INT( mstr, Analysis::Config::Job::msgsDebug,                                       ival, owflg ); }
	if ( !owflg ) { _CFG_INT( mstr, Analysis::Config::Job::tickerFreq,                                      ival, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Config::Job::doSlidingWindowRho,                              sval, owflg ); }
	if ( !owflg ) { _CFG_STR( mstr, Analysis::Config::Output::file,                                         sval, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Config::Output::Tuple::write,                                 sval, owflg ); }
	if ( !owflg ) { _CFG_STR( mstr, Analysis::Config::Output::Tuple::fileName,                              sval, owflg ); }
	if ( !owflg ) { _CFG_STR( mstr, Analysis::Config::Output::Tuple::treeName,                              sval, owflg ); }
	if ( !owflg ) { _CFG_INT( mstr, Analysis::Config::Output::Tuple::evtsPerFile,                           ival, owflg ); }
	if ( !owflg ) { _CFG_STR( mstr, Analysis::Config::Input::dataDir,                                       sval, owflg ); }
	if ( !owflg ) { _CFG_STR( mstr, Analysis::Config::Input::fileMask,                                      sval, owflg ); }
	if ( !owflg ) { _CFG_INT( mstr, Analysis::Config::Input::nFiles,                                        ival, owflg ); }  
	if ( !owflg ) { _CFG_INT( mstr, Analysis::Config::Input::nEvents,                                       ival, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Config::Filter::CaloSignal::doTimeFilter,                     sval, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Config::Filter::Pileup::RecoJet::doJetArea,                   sval, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Config::Filter::Pileup::CaloSignal::doConstituentSubtraction, sval, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Config::Filter::Pileup::CaloSignal::doSoftKiller,             sval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Config::Filter::Pileup::CaloSignal::SoftKiller::gridSize,     xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::Event::rapGap,                                        xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::Event::minMinv,                                       xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::Event::maxMinv,                                       xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::Event::Decay::rapMin,                                 xval ,owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::Event::Decay::rapMax,                                 xval ,owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::Event::Decay::minMinv,                                xval ,owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::Event::Decay::maxMinv,                                xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::TruthJet::isoDeltaR,                                  xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::MatchedJet::isoDeltaR,                                xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::MatchedJet::matchR,                                   xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::CaloSignal::timeMin,                                  xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::CaloSignal::timeMax,                                  xval, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Select::CaloSignal::applyTiming,                              sval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::RecoJet::isoDeltaR,                                   xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::RecoJet::timeMin,                                     xval, owflg ); }
	if ( !owflg ) { _CFG_DBL( mstr, Analysis::Select::RecoJet::timeMax,                                     xval, owflg ); }
	if ( !owflg ) { _CFG_LOG( mstr, Analysis::Select::RecoJet::applyTiming,                                 sval, owflg ); }
	hasChanged = hasChanged || owflg;
      } // not EOF
    } while ( !cfg.eof() );
    //
    cfg.close();
    PRINT_INFO( _mname, "%s", _frame.c_str() );
    printf("\n");
    //
    return hasChanged;
  }  

  //-/////////////
  // Event data //
  //-/////////////

  ///@brief FastJet helpers 
  ///
  /// Details on how arbritary information can be attached to a @c fastjet::PseudoJet object can be found at 
  /// <a href="http://fastjet.fr/" title="FastJet documentation">fastjet.fr</a>, with specifics 
  /// <a href="http://fastjet.fr/repo/doxygen-3.3.1/classfastjet_1_1PseudoJet.html" title="PseudoJet documentation">here</a>. 
  ///
   struct FastJet
  {
    ///@name General user info access
    ///@{
    ///@brief Check if certain information is attached to, or the state of this information attachment for, a @c fastjet::PseudoJet object
    ///@param pj reference to non-modifiable @c fastjet::PseudoJet object
    ///
    static bool   isEmpty(const fastjet::PseudoJet& pj)           { return Analysis::FastJet::UserInfo::isEmpty(pj);            } ///< Attached information empty if @c false
    static bool   hasUserInfo(const fastjet::PseudoJet& pj)       { return Analysis::FastJet::UserInfo::hasUserInfo(pj);        } ///< Information is attached if @c true
    static int    userIndex(const fastjet::PseudoJet& pj)         { return Analysis::FastJet::UserInfo::userIndex(pj);          } ///< User assigned index
    ///<@warning Do not use the return value of userIndex() as reference into a vector. It can be negative or otherwise out of range, because it is 
    ///<         also used as a state indicator.
    ///<@todo Fix the @c Analysis::FastJet::UserInfo::index() behaviour (split index feature from status indicator)

    ///@}
    ///@name Calorimeter signal info 
    ///@{
    ///@brief Check if attached information describes a calorimeter signal 
    ///@return @c true if yes
    ///
    ///@param pj reference to non-modifiable @c fastjet::PseudoJet object
    ///
    /// Both functions have identical behaviour.
    static bool   hasCaloSignalInfo(const fastjet::PseudoJet& pj) { return Analysis::FastJet::CaloSignalInfo::isCaloSignal(pj); }
    static bool   isCaloSignal(const fastjet::PseudoJet& pj)      { return Analysis::FastJet::CaloSignalInfo::isCaloSignal(pj); }
    ///@}
    ///@name Truth particle info access
    ///@{
    ///@brief Check if certain information is attached to, or the state of this information attachment for, a @c fastjet::PseudoJet object
    ///@param pj reference to non-modifiable @c fastjet::PseudoJet object
    static bool   hasTruthInfo(const fastjet::PseudoJet& pj)      { return Analysis::FastJet::TruthInfo::isTruthParticle(pj);   } ///< Checks if truth information is available ( @c true ) 
    static bool   isTruthParticle(const fastjet::PseudoJet& pj)   { return Analysis::FastJet::TruthInfo::isTruthParticle(pj);   } ///< Checks if truth information is available ( @c true ) 
    static int    pdg(const fastjet::PseudoJet& pj)               { return Analysis::FastJet::TruthInfo::pdg(pj);               } ///< PDG code of associated truth particle
    ///<@return Valid PDG code as documented <a href="http://pdg.lbl.gov/2017/reviews/rpp2017-rev-monte-carlo-numbering.pdf" title="PDF numbering scheme">here</a>.
    ///<@warning If no Analysis::FastJet::TruthInfo object is attached, the return value is 0.
    static int    status(const fastjet::PseudoJet& pj)            { return Analysis::FastJet::TruthInfo::status(pj);            } ///< Generator status code
    ///<@return Generator status code with generator dependent interpretation. 
    ///<@warning If no Analysis::FastJet::TruthInfo object is attached, the return value is 0.
    static double charge(const fastjet::PseudoJet& pj)            { return Analysis::FastJet::TruthInfo::charge(pj);            } ///< Particle charge
    ///<@return Particle charge in units of the electron charge @f$ e @f$.
    ///<@warning If no Analysis::FastJet::TruthInfo object is attached, the return value is 0.
    static bool   isHadron(const fastjet::PseudoJet& pj)          { return Analysis::FastJet::TruthInfo::isHadron(pj);          } ///< Checks if particle is a hadron (@c true) or not (@c false)
    ///@warning  If no Analysis::FastJet::TruthInfo object is attached, @c false is returned.
    static bool   isLepton(const fastjet::PseudoJet& pj )         { return Analysis::FastJet::TruthInfo::isLepton(pj);          } ///< Checks if particle is a lepton (@c true) or not (@c false)
    ///@warning  If no Analysis::FastJet::TruthInfo object is attached, @c false is returned.
    static bool   isNeutrino(const fastjet::PseudoJet& pj)        { return Analysis::FastJet::TruthInfo::isNeutrino(pj);        } ///< Checks if particle is a neutrino (@c true) or not (@c false)
    ///@warning  If no Analysis::FastJet::TruthInfo object is attached, @c false is returned.
    static bool   isCharged(const fastjet::PseudoJet& pj )        { return Analysis::FastJet::TruthInfo::isCharged(pj);         } ///< Checks if particle is a charged (@c true) or neutral (@c false)
    ///@warning  If no Analysis::FastJet::TruthInfo object is attached, @c false is returned.
    static bool   isNeutral(const fastjet::PseudoJet& pj)         { return Analysis::FastJet::TruthInfo::isNeutral(pj);         } ///< Checks if particle is a neutral (@c true) or charged (@c false)
    static bool   isSignal(const fastjet::PseudoJet& pj)          { return Analysis::FastJet::UserInfo::isSignal(pj);           }
    static bool   isMatched(const fastjet::PseudoJet& pj)         { return Analysis::FastJet::UserInfo::isMatched(pj);          }
    static bool   isPileup(const fastjet::PseudoJet& pj)          { return Analysis::FastJet::UserInfo::isPileup(pj);           }
    ///@}
   };

  ///@brief PseudoJet representation of calorimeter signals (general aspects)
  struct CaloSignal
  {
    ///@brief Empty calorimeter representation
    ///
    ///@return @c fastjet::PseudoJet object with @c Analysis::FastJet::CaloSignalInfo object representing an unknown calorimeter signal attached.
    /// 
    ///@param type calorimeter signal type; any of "TopoCluster", "TopoTower", "TopoTowerFine", or "Unknown" (default)
    ///@param idx  reference index (default is -1, invalid index) 
    static fastjet::PseudoJet pseudoJet_empty(const std::string& type="Unknown",int idx=-1) 
    { 
      fastjet::PseudoJet pj; 
      Analysis::FastJet::CaloSignalInfo* si = new Analysis::FastJet::CaloSignalInfo();
      si->setEmpty();
      si->setTypeName(type);
      pj.set_user_info(si); 
      pj.set_user_index(idx);
      return pj; 
    }
  };

  ///@brief PseudoJet representation of topo-cluster
  struct TopoCluster
  {
    ///@brief Generator for topo-cluster on electromagnetic energy scale
    ///
    ///@return @c fastjet::PseudoJet object with @c Analysis::FastJet::CaloSignalInfo object representing a topo-cluster attached.
    ///           The returned energy and momentum correspond to a massless @a pseudo-particle with physically valid kinematics, even for
    ///           unphysical detector signals.
    ///
    /// The returned object has valid four-momentum kinematics. In case of a signal @f$ E < 0 @f$, the object is "ghosted", e.g. it has 
    /// a tiny positive signal @f$ E \approx 0 @f$ but the correct @f$ (y,\phi) @f$ directions.   
    ///
    ///@param data  pointer to underlying data structure of templated type
    ///@param idx   index of associated data
    ///@param check check on @f$ E > 0 @f$ if @c true, else generate @c fastjet::PseudoJet from all signals
    template<class T>
    static fastjet::PseudoJet pseudoJet_em(T* data,size_t idx,bool check=true)
    { _IMPL_PJ_EXTRACT( topoclus_em, Analysis::FastJet::CaloSignalInfo::TopoCluster ); }
    ///@brief Generator for a list of topo-clusters on electromagnetic energy scale
    ///
    ///@return @c fastjet::PseudoJet object with @c Analysis::FastJet::CaloSignalInfo object representing a topo-cluster attached.
    ///           The returned energy and momentum correspond to a massless @a pseudo-particle with physically valid kinematics, even for
    ///           unphysical detector signals.
    ///
    /// The returned object have valid four-momentum kinematics. In case of signals @f$ E < 0 @f$, the objects are "ghosted", e.g. they
    /// are assigned a tiny positive signal @f$ E \approx 0 @f$ and the correct @f$ (y,\phi) @f$ directions.   
    ///
    ///@param data  pointer to underlying data structure of templated type
    ///@param incl  include unphysical signals as ghosted if @c true, else only include objects from physical signals
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector_em(T* data,bool incl=false)
    { _IMPL_PJ_VECT_EXTRACT( topoclus_em, em, incl ); }
 
    ///@brief Generator for topo-cluster on locally calibrated (hadronic) energy scale
    ///
    ///@copydetails AnalysisHelper::TopoCluster::pseudoJet_em()
    template<class T>
    static fastjet::PseudoJet pseudoJet_lcw(T* data,size_t idx,bool check=true)
    { _IMPL_PJ_EXTRACT( topoclus_lcw, Analysis::FastJet::CaloSignalInfo::TopoCluster ); }
    ///@brief Generator for a list of topo-clusters on locally calibrated (hadronic) energy scale 
    ///
    ///@copydetails AnalysisHelper::TopoCluster::pseudoJetVector_em() 
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector_lcw(T* data,bool incl=false)
    { _IMPL_PJ_VECT_EXTRACT( topoclus_lcw, lcw, incl ); }


    ///@brief Generator for topo-cluster on a given energy scale
    ///
    ///@copydetails AnalysisHelper::TopoCluster::pseudoJet_em()
    ///@param sstr  string indicating energy scale (options "em" and "lcw", default "lcw")
    ///
    ///@warning In case an unknown string is passed to the function, an empty @c fastjet::PseudoJet object is returned.
    template<class T>
    static fastjet::PseudoJet pseudoJet(T* data,size_t idx,const std::string& sstr="lcw",bool check=true)
    { return sstr == "lcw" ? pseudoJet_lcw(data,idx,check) : sstr == "em" ? pseudoJet_em(data,idx,check) : AnalysisHelper::CaloSignal::pseudoJet_empty(); }
    ///@brief Generator for a list of topo-clusters on a given energy scale
    ///
    ///@copydetails AnalysisHelper::TopoCluster::pseudoJetVector_em()
    ///@param sstr  string indicating energy scale (options "em" and "lcw", default "lcw")
    ///
    ///@warning In case an unknown string is passed to the function, an empty vector is returned.
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector(T* data,const std::string& sstr="lcw",bool incl=true)
    { return sstr == "lcw" ? pseudoJetVector_lcw(data,incl) : sstr == "em" ? pseudoJetVector_em(data,incl) : std::vector<fastjet::PseudoJet>(); }
  };

  ///@brief PseudoJet representation of standard topo-tower @f$ \Delta\eta\times\Delta\phi = 0.1\times 0.1 @f$
  struct TopoTower
  {
    ///@brief Generator for topo-towers on electromagnetic energy scale
    ///@copydetails AnalysisHelper::TopoCluster::pseudoJet_em()
    template<class T>
    static fastjet::PseudoJet pseudoJet_em(T* data,size_t idx,bool check=true)
    { _IMPL_PJ_EXTRACT( ttownorm_em, Analysis::FastJet::CaloSignalInfo::TopoTower ); }
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector_em(T* data,bool incl=false)
    { _IMPL_PJ_VECT_EXTRACT( ttownorm_em, em, incl ); }
 
    // LCW cluster
    template<class T>
    static fastjet::PseudoJet pseudoJet_lcw(T* data,size_t idx,bool check=true)
    { _IMPL_PJ_EXTRACT( ttownorm_lcw, Analysis::FastJet::CaloSignalInfo::TopoTower ); }
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector_lcw(T* data,bool incl=false)
    { _IMPL_PJ_VECT_EXTRACT( ttownorm_lcw, lcw, incl ); }

    // General interfaces
    template<class T>
    static fastjet::PseudoJet pseudoJet(T* data,size_t idx,const std::string& sstr="lcw",bool check=true)
    { return sstr == "lcw" ? pseudoJet_lcw(data,idx,check) : sstr == "em" ? pseudoJet_em(data,idx,check) : AnalysisHelper::CaloSignal::pseudoJet_empty(); }
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector(T* data,const std::string& sstr="lcw",bool incl=false)
    { return sstr == "lcw" ? pseudoJetVector_lcw(data,incl) : sstr == "em" ? pseudoJetVector_em(data,incl) : std::vector<fastjet::PseudoJet>(); }
  };

  ///@brief PseudoJet representation of topo-tower
  struct TopoTowerFine
  {
    // EM cluster
    template<class T>
    static fastjet::PseudoJet pseudoJet_em(T* data,size_t idx,bool check=true)
    { _IMPL_PJ_EXTRACT( ttowfine_em, Analysis::FastJet::CaloSignalInfo::TopoTowerFine ); }
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector_em(T* data,bool incl=false)
    { _IMPL_PJ_VECT_EXTRACT( ttowfine_em, em, incl ); }
 
    // LCW cluster
    template<class T>
    static fastjet::PseudoJet pseudoJet_lcw(T* data,size_t idx,bool check=true)
    { _IMPL_PJ_EXTRACT( ttowfine_lcw, Analysis::FastJet::CaloSignalInfo::TopoTowerFine ); }
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector_lcw(T* data,bool incl=false)
    { _IMPL_PJ_VECT_EXTRACT( ttowfine_lcw, lcw, incl ); }

    // General interfaces
    template<class T>
    static fastjet::PseudoJet pseudoJet(T* data,size_t idx,const std::string& sstr="lcw",bool check=true)
    { return sstr == "lcw" ? pseudoJet_lcw(data,idx,check) : sstr == "em" ? pseudoJet_em(data,idx,check) : AnalysisHelper::CaloSignal::pseudoJet_empty(); }
    template<class T>
    static std::vector<fastjet::PseudoJet> pseudoJetVector(T* data,const std::string& sstr="lcw",bool incl=false)
    { return sstr == "lcw" ? pseudoJetVector_lcw(data,incl) : sstr == "em" ? pseudoJetVector_em(data,incl) : std::vector<fastjet::PseudoJet>(); }
  };

  ///////////////////////
  // Histogram helpers //
  ///////////////////////

  // open ROOT file for output
  static bool openFile(const std::string& fname,const std::string& opt="RECREATE") {
    static std::string _mname = "AH::openFile";  
    TFile* fptr = HistUtils::openFile(fname,opt); 
    if ( fptr == 0 ) {
      PRINT_ERROR( _mname,"cannot open file \042%s\042 for output",fname.c_str());
      return false;
    }
    return true;
  } 

  // close output file
  static bool closeFile() { return HistUtils::closeFile(); }

  // get file
  static TFile* getFile() { return HistUtils::getFile(); }

  // get file name
  static std::string getFileName() { return HistUtils::getFileName(); }

  // book 1d histogram (any ROOT type)
  template<class H>
  static H* book1D(const std::string& name,const std::string& title,int nbins,double xmin,double xmax,const std::string xtitle="",const std::string& ytitle="")
  {
    TFile* thisFile = HistUtils::getFile();
    if ( thisFile != 0 ) { thisFile->cd(); }
    H* h = new H(name.c_str(),title.c_str(),nbins,xmin,xmax); h->Sumw2();
    if ( xtitle != "" ) { h->GetXaxis()->SetTitle(xtitle.c_str()); }
    if ( ytitle != "" ) { h->GetYaxis()->SetTitle(ytitle.c_str()); }
    (HistUtils::HistMgr::instance())->add(h); 
    return h;
  }

  // book 2d histogram (any ROOT type)
  template<class H>
  static H* book2D(const std::string& name,const std::string& title,int nxbins,double xmin,double xmax,int nybins,double ymin,double ymax,
	    const std::string xtitle="",const std::string& ytitle="")
  {
    TFile* thisFile = HistUtils::getFile();
    if ( thisFile != 0 ) { thisFile->cd(); }
    H* h = new H(name.c_str(),title.c_str(),nxbins,xmin,xmax,nybins,ymin,ymax); h->Sumw2();
    if ( xtitle != "" ) { h->GetXaxis()->SetTitle(xtitle.c_str()); }
    if ( ytitle != "" ) { h->GetYaxis()->SetTitle(ytitle.c_str()); }
    (HistUtils::HistMgr::instance())->add(h); 
    return h;
  }

  // write to file
  static int writeHists(bool noempty=true) { return HistUtils::HistMgr::instance()->write(noempty); }
  static int writeHists(const std::string& fname,const std::string& ftag="RECREATE",bool noempty=true) 
  {
    TFile* f = new TFile(fname.c_str(),ftag.c_str());
    if ( f == 0 ) { printf("AnalysisHelper::writeHists(\042%s\042,\042%s\042) ERROR cannot create TFile object\n",fname.c_str(),ftag.c_str()); return -1; }
    int nh((HistUtils::HistMgr::instance())->write(noempty));
    f->Close();
    return nh;
  }

  ////////////////////////////////////
  // Pick-up signal and collections //
  ////////////////////////////////////

  struct PseudoJetGetter
  {
    // convert topo-cluster to PseudoJets 
    template<class T>
    static std::vector<fastjet::PseudoJet> getTopoClusters(T* data,const std::string& scale="lcw",bool incl=false,std::vector<fastjet::PseudoJet>& jets=Analysis::InternalRef::Untrusted::pjStore)
    { 
      // get all raw data (incl = true) or only physically meaningful clusters (incl = false)
      std::vector<fastjet::PseudoJet> ctoc = TopoCluster::pseudoJetVector<T>(data,scale,incl);

      //////////////////////////////////////////
      // set associated data for all clusters //
      //////////////////////////////////////////

      for ( auto& pj : ctoc ) { 
	Analysis::FastJet::CaloSignalInfo* si = new Analysis::FastJet::CaloSignalInfo(pj.user_info<Analysis::FastJet::CaloSignalInfo>());
	int idx(si->dataIndex());
	PRINT_DEBUG( "AH::getTopoClusters","user index %i",idx);
	if ( (size_t)idx < data->topoclus_em_time->size() ) { 
	  si->setPTD(data->topoclus_PTD->at(idx));
	  si->setEPOS(data->topoclus_em_epos->at(idx));
	  if ( data->topoclus_em_time->at(idx) != 0. ) {
	    si->setTime(data->topoclus_em_time->at(idx));
	  } else {
	    si->setTime(Analysis::FastJet::CaloSignalInfo::defTime());
	  }
	  si->setArea(Analysis::FastJet::CaloSignalInfo::defArea()); 
	  pj.set_user_info(si);
	  pj.set_user_index(idx);
	}
      }
    
      //////////////////////////////////////////////////////
      // get the area from FastJet (for rho windows only) //
      //////////////////////////////////////////////////////

      if ( Analysis::Config::Job::doSlidingWindowRho ) { 
	// get jets for area calculation with pT > 0 from original input without ghosts (incl = true means has ghosts)
	std::vector<fastjet::PseudoJet> areaJets = incl 
	  ? AnalysisHelper::topoClusterJets(fastjet::SelectorPtMin(1.1*Analysis::FastJet::Ghosts::filterPt)(ctoc),0.) // remove "ghosts" with E<0 
	  : AnalysisHelper::topoClusterJets(ctoc,0.);                                                                 // no "ghosts" in cluster collection
	// copy the area into the output collection - loop on jets with pT > 0.
	for ( auto pj : areaJets ) { 
	  PRINT_DEBUG("AH::getTopoClusters","jet has area %7.3f",pj.area());
	  for ( auto cj : pj.constituents() ) {
	    int ix(cj.user_info<Analysis::FastJet::CaloSignalInfo>().dataIndex()); // unique index for topocluster w/ area in jet
	    size_t i(0);
	    // find a cluster inside a jet in the original input collection
	    while ( i < ctoc.size() && ctoc.at(i).user_info<Analysis::FastJet::CaloSignalInfo>().dataIndex() != ix ) { ++i; }
	    // check if cluster in original collection is found
	    if ( i < ctoc.size() ) { 
		Analysis::FastJet::CaloSignalInfo* si = new Analysis::FastJet::CaloSignalInfo(ctoc.at(i).user_info<Analysis::FastJet::CaloSignalInfo>());
		si->setArea(cj.area());          // overwrites invalid areas !!
		ctoc[i].set_user_info(si);
		ctoc[i].set_user_index(ix);
	    }  // found constituent
	  } // loop clusters in jet
	} // loop jets
	// find jets 
	jets = AnalysisHelper::topoClusterJets(fastjet::SelectorPtMin(Analysis::FastJet::Ghosts::filterPt)(ctoc),Analysis::Select::RecoJet::pTmin);
      } // do rho windows
      return ctoc; 
    } // get topo clusters
    
    // convert fine topo-towers to PseudoJets
    template<class T>
    static std::vector<fastjet::PseudoJet> getTopoTowersFine(T* data,const std::string& scale="lcw",bool incl=false)
    { 
      std::vector<fastjet::PseudoJet> csig = TopoTowerFine::pseudoJetVector<T>(data,scale,incl);
      //      std::vector<fastjet::PseudoJet> cref; cref.reserve(csig.size()); 
      for ( auto& pj : csig ) { 
	if ( (size_t)pj.user_index() < data->ttowfine_em_time->size() ) { 
	  Analysis::FastJet::CaloSignalInfo* si = new Analysis::FastJet::CaloSignalInfo(pj.user_info<Analysis::FastJet::CaloSignalInfo>());
	  si->setArea(si->fixedArea());
	  si->setPTD(data->ttowfine_PTD->at(pj.user_index()));
	  si->setEPOS(data->ttowfine_em_epos->at(pj.user_index()));
	  if ( data->ttowfine_em_time->at(pj.user_index()) != 0. ) {
	    si->setTime(data->ttowfine_em_time->at(pj.user_index()));
	  } else {
	    si->setTime(Analysis::FastJet::CaloSignalInfo::defTime());
	  }
	  pj.set_user_info(si);
	}
      }
      return csig; /* cref; */
    }
      
    // convert standard (coarse) topo-towers to PseudoJets
    template<class T>
    static std::vector<fastjet::PseudoJet> getTopoTowers(T* data,const std::string& scale="lcw",bool incl=false)
    {
      std::vector<fastjet::PseudoJet> csig = TopoTower::pseudoJetVector<T>(data,scale);
      //      std::vector<fastjet::PseudoJet> cref; cref.reserve(csig.size()); 
      for ( auto& pj : csig ) {
	if ( (size_t)pj.user_index() < data->ttownorm_em_time->size() ) { 
	  Analysis::FastJet::CaloSignalInfo* si = new Analysis::FastJet::CaloSignalInfo(pj.user_info<Analysis::FastJet::CaloSignalInfo>());
	  si->setArea(si->fixedArea());
	  si->setPTD(data->ttownorm_PTD->at(pj.user_index()));
	  si->setEPOS(data->ttownorm_em_epos->at(pj.user_index()));
	  si->setEPOS(data->ttownorm_em_epos->at(pj.user_index()));
	  if ( data->ttownorm_em_time->at(pj.user_index()) != 0. ) {
	    si->setTime(data->ttowfine_em_time->at(pj.user_index()));
	  } else {
	    si->setTime(Analysis::FastJet::CaloSignalInfo::defTime());
	  }
	  pj.set_user_info(si);
	}
	// cref.push_back(fastjet::PseudoJet(pj.px(),pj.py(),pj.pz(),pj.e()));
	// cref.back().set_user_info(si);
	// cref.back().set_user_index(pj.user_index()); 
      }
      return csig; /* cref; */
    }

    template<class T>
    static std::vector<fastjet::PseudoJet> getTopoTowersNorm(T* data,const std::string& scale="lcw",bool incl=false)
    { return getTopoTowers<T>(data,scale,incl); }

    // convert truth hadrons to PseudoJets
    template<class T>
    static std::vector<fastjet::PseudoJet> getTruthHadrons(T* data)
    {
      static int _barcodeBar = 200000;
      std::vector<fastjet::PseudoJet> part;
      for ( size_t i(0); i<data->truthhadr_e->size(); ++i ) {
	if ( data->truthhadr_barcode->at(i) < _barcodeBar && data->truthhadr_vtxflag->at(i) && data->truthhadr_vtxid->at(i) == 0 ) {
	  fastjet::PseudoJet pj = fastjet::PseudoJet(data->truthhadr_px->at(i),data->truthhadr_py->at(i),data->truthhadr_pz->at(i),data->truthhadr_e->at(i));
	  pj.set_user_index(i);
	  pj.set_user_info(new Analysis::FastJet::TruthInfo(i,data->truthhadr_pdg->at(i),data->truthhadr_status->at(i),data->truthhadr_charge->at(i)));
	  if ( Analysis::FastJet::TruthInfo::isHadron(pj) || Analysis::FastJet::TruthInfo::isLepton(pj) ) { part.push_back(pj);	}
	}
      }
      return fastjet::SelectorRapRange(Analysis::Select::Particle::rapMin,Analysis::Select::Particle::rapMax)(part);
    }
  
    // convert truth leptons to PseudoJets
    template<class T>
    static std::vector<fastjet::PseudoJet> getTruthLeptons(T* data)
    {
      static int _barcodeBar = 200000;
      std::vector<fastjet::PseudoJet> part;
      for ( size_t i(0); i<data->truthlept_e->size(); ++i ) {
	if ( data->truthlept_barcode->at(i) < _barcodeBar && data->truthlept_vtxflag->at(i) && data->truthlept_vtxid->at(i) == 0 ) { 
	  part.push_back(fastjet::PseudoJet(data->truthlept_px->at(i),
					    data->truthlept_py->at(i),
					    data->truthlept_pz->at(i),
					    data->truthlept_e->at(i)));
	  part.back().set_user_index(i);
	  part.back().set_user_info(new Analysis::FastJet::TruthInfo(i,data->truthlept_pdg->at(i),data->truthlept_status->at(i),data->truthlept_charge->at(i)));
	}	
      }
      return fastjet::SelectorRapRange(Analysis::Select::Particle::rapMin,Analysis::Select::Particle::rapMax)(part);
    }
 
    // general particle pickup
    template<class T>
    static std::vector<fastjet::PseudoJet> getTruthParticles(T* data,const std::string& type="hadrons")
    { return 
	type == "hadrons" ? getTruthHadrons<T>(data) : 
	type == "leptons" ? getTruthLeptons<T>(data)             : 
	std::vector<fastjet::PseudoJet>(); }

    // general calorimeter signal pickup
    template<class T>
    static std::vector<fastjet::PseudoJet> getCaloSignals(T* data,const std::string& type="topocluster",const std::string& scale="lcw",bool incl=false)
    { return 
	type == "topoclus"                                                             ? getTopoClusters<T>(data,scale,incl)   : 
	type == "topotowers" || type == "normtopotowers" || type == "coarsetopotowers" ? getTopoTowers<T>(data,scale,incl)     : 
	type == "finetopotowers"                                                       ? getTopoTowersFine<T>(data,scale,incl) : 
	std::vector<fastjet::PseudoJet>();
    }
  };

  ///////////////////////////////
  // Modify/filter collections //
  ///////////////////////////////

  // general filter implementation
  static std::vector<fastjet::PseudoJet> filter(const std::vector<fastjet::PseudoJet>& pjvec,const Analysis::Selector::Interface& psel) 
  {
    static std::string _mname = "AH::filter";
    std::vector<fastjet::PseudoJet> ojet; ojet.reserve(pjvec.size());
    // filter
    for ( auto pj : pjvec ) { if ( psel(pj) ) { ojet.push_back(pj); } }
    //
    return ojet;
  }
   
  ////////////////////////////////////
  // Pick-up/create jet collections //
  ////////////////////////////////////

  // apply lepton isolation
  template<class T>
  static std::vector<fastjet::PseudoJet> applyLeptonIsolation(T* data,const std::vector<fastjet::PseudoJet>& jets)
  {
    static bool _firstCall = true;
    // get leptons
    std::vector<fastjet::PseudoJet> lept = fastjet::SelectorPtMin(Analysis::Select::TruthJet::leptonPtMin)(PseudoJetGetter::getTruthLeptons(data));
    // check jets
    std::vector<fastjet::PseudoJet> fjets; fjets.reserve(jets.size());
    for ( const auto& pj : jets ) { 
      size_t l(0);
      while ( l < lept.size() && pj.delta_R(lept.at(l)) > Analysis::Select::TruthJet::leptonDeltaR ) { ++l; }
      // jet is isolated
      if ( l == lept.size() ) { fjets.push_back(pj); }
    }
    if ( _firstCall ) { 
      PRINT_INFO("AH::applyLeptonIsolation()","found %2zu leptons, %2zu isolated jets, out of %2zu",lept.size(),fjets.size(),jets.size());
      _firstCall = false;
    }
    return fjets;
  }

  // find jets
  static std::vector<fastjet::PseudoJet> findJets(const std::vector<fastjet::PseudoJet>& input,double pTmin,double rapMin,double rapMax,
						  const fastjet::JetDefinition& jetDef,const fastjet::AreaDefinition& areaDef) //,bool doLeptonIso=false)
  {
    if ( input.empty() ) { return std::vector<fastjet::PseudoJet>(); }
    fastjet::ClusterSequenceArea* pcs = new fastjet::ClusterSequenceArea(input,jetDef,areaDef); 
    fastjet::Selector jsel = fastjet::SelectorPtMin(pTmin) && fastjet::SelectorRapRange(rapMin,rapMax);
    //
    //    if ( doLeptonIso ) { jsel = applyLeptonIsolation(data,jsel); }
    std::vector<fastjet::PseudoJet> pjvec = fastjet::sorted_by_pt(jsel(pcs->inclusive_jets()));
    // attach user info
    for ( size_t i(0); i<pjvec.size(); ++i ) { 
      Analysis::FastJet::UserInfo* si = new Analysis::FastJet::UserInfo(i);
      si->setPileup();
      si->setMatched(false);
      si->setDataIndex(i);
      pjvec[i].set_user_info(si); 
    }
    if ( !pjvec.empty() ) { pcs->delete_self_when_unused(); } // fixes scope problem!!
    return pjvec;
  }

  // create topo-cluster jets 
  template<class T>
  static std::vector<fastjet::PseudoJet> topoClusterJets(T* data,                                                   
							 const std::string& scale       = "lcw",
							 double pTmin                   = Analysis::Select::RecoJet::pTmin,
							 double rapMin                  = Analysis::Select::RecoJet::rapMin,
							 double rapMax                  = Analysis::Select::RecoJet::rapMax,
							 fastjet::JetDefinition jetDef  = Analysis::Config::RecoJet::definition,
							 fastjet::AreaDefinition areaDef= Analysis::Config::RecoJet::areaDef)
  { return findJets(PseudoJetGetter::getTopoClusters<T>(data,scale),pTmin,rapMin,rapMax,jetDef,areaDef); } 
  static std::vector<fastjet::PseudoJet> topoClusterJets(const std::vector<fastjet::PseudoJet>& csig,
							 double pTmin                   = Analysis::Select::RecoJet::pTmin,
							 double rapMin                  = Analysis::Select::RecoJet::rapMin,
							 double rapMax                  = Analysis::Select::RecoJet::rapMax,
							 fastjet::JetDefinition jetDef  = Analysis::Config::RecoJet::definition,
							 fastjet::AreaDefinition areaDef= Analysis::Config::RecoJet::areaDef)
  { return findJets(csig,pTmin,rapMin,rapMax,jetDef,areaDef); }

  // create topo-tower jets 
  template<class T>
  static std::vector<fastjet::PseudoJet> topoTowerJets(T* data,                                                   
						       const std::string& scale       = "lcw",
						       double pTmin                   = Analysis::Select::RecoJet::pTmin,
						       double rapMin                  = Analysis::Select::RecoJet::rapMin,
						       double rapMax                  = Analysis::Select::RecoJet::rapMax,
						       fastjet::JetDefinition jetDef  = Analysis::Config::RecoJet::definition,
						       fastjet::AreaDefinition areaDef= Analysis::Config::RecoJet::areaDef)
  { return findJets(PseudoJetGetter::getTopoTowers<T>(data,scale),pTmin,rapMin,rapMax,jetDef,areaDef); }
  static std::vector<fastjet::PseudoJet> topoTowerJets(const std::vector<fastjet::PseudoJet>& csig,
						       double pTmin                   = Analysis::Select::RecoJet::pTmin,
						       double rapMin                  = Analysis::Select::RecoJet::rapMin,
						       double rapMax                  = Analysis::Select::RecoJet::rapMax,
						       fastjet::JetDefinition jetDef  = Analysis::Config::RecoJet::definition,
						       fastjet::AreaDefinition areaDef= Analysis::Config::RecoJet::areaDef)
  { return findJets(csig,pTmin,rapMin,rapMax,jetDef,areaDef); }

  // create fine topo-tower jets 
  template<class T>
  static std::vector<fastjet::PseudoJet> topoTowerFineJets(T* data,                                                   
							   const std::string& scale       = "lcw",
							   double pTmin                   = Analysis::Select::RecoJet::pTmin,
							   double rapMin                  = Analysis::Select::RecoJet::rapMin,
							   double rapMax                  = Analysis::Select::RecoJet::rapMax,
							   fastjet::JetDefinition jetDef  = Analysis::Config::RecoJet::definition,
							   fastjet::AreaDefinition areaDef= Analysis::Config::RecoJet::areaDef)
  { return findJets(PseudoJetGetter::getTopoTowersFine<T>(data,scale),pTmin,rapMin,rapMax,jetDef,areaDef); }
  static std::vector<fastjet::PseudoJet> topoTowerFineJets(const std::vector<fastjet::PseudoJet>& csig,
							   double pTmin                   = Analysis::Select::RecoJet::pTmin,
							   double rapMin                  = Analysis::Select::RecoJet::rapMin,
							   double rapMax                  = Analysis::Select::RecoJet::rapMax,
							   fastjet::JetDefinition jetDef  = Analysis::Config::RecoJet::definition,
							   fastjet::AreaDefinition areaDef= Analysis::Config::RecoJet::areaDef)
  { return findJets(csig,pTmin,rapMin,rapMax,jetDef,areaDef); }

  // create general calorimetre jets
  template<class T>
  static std::vector<fastjet::PseudoJet> caloJets(const std::vector<fastjet::PseudoJet>& csig,
						  double pTmin                   = Analysis::Select::RecoJet::pTmin,
						  double rapMin                  = Analysis::Select::RecoJet::rapMin,
						  double rapMax                  = Analysis::Select::RecoJet::rapMax,
						  fastjet::JetDefinition jetDef  = Analysis::Config::RecoJet::definition,
						  fastjet::AreaDefinition areaDef= Analysis::Config::RecoJet::areaDef)
  { return findJets(csig,pTmin,rapMin,rapMax,jetDef,areaDef); }


  // pick up truth jets from data
  template <class T>
  static std::vector<fastjet::PseudoJet> truthJets(T* data,double pTmin,double rapMin,double rapMax,bool doLeptonIsol=true)
  {
    static bool _firstCall = true;
    //
    if ( _firstCall ) { 
      if ( doLeptonIsol ) { 
	PRINT_INFO("AH::truthJets()","fetch truth jets with lepton isolation"); 
      } else { 
	PRINT_INFO("AH::truthJets()","fetch truth jets without lepton isolation"); 
      }
      _firstCall = false;
    }

    std::vector<fastjet::PseudoJet> tjet;
    // access truthjet block
    for ( size_t i(0); i<data->truthjet_pt->size(); ++i ) { 
      if ( data->truthjet_pt->at(i) > pTmin ) {
	fastjet::PseudoJet pj(fastjet::PtYPhiM(data->truthjet_pt->at(i),
					       data->truthjet_rap->at(i),
					       data->truthjet_phi->at(i),
					       data->truthjet_mass->at(i)));
	size_t nconstit(data->truthjet_comp->at(i).size());
	pj.set_user_index(i);
	Analysis::FastJet::TruthJetInfo* ti = new Analysis::FastJet::TruthJetInfo(nconstit);
	ti->setDataIndex(i);
	ti->setMatched(false);
	ti->setSignal();
	pj.set_user_info(ti);
	tjet.push_back(pj);
      } // above minimum jet pT
    }
    fastjet::Selector rapSel = fastjet::SelectorRapRange(rapMin,rapMax);
    std::vector<fastjet::PseudoJet> fjet = doLeptonIsol ? fastjet::sorted_by_pt(applyLeptonIsolation<T>(data,rapSel(tjet))) : fastjet::sorted_by_pt(rapSel(tjet));
    // auto  f(fjet.begin());
    // while ( f != fjet.end() )
    // for ( auto tj : fjet ) { 
    // }
    return fjet;
  } 

  // build truth jets on the fly
  template<class T>
  static std::vector<fastjet::PseudoJet> truthJets(T* data,
						   fastjet::JetDefinition jetDef  = Analysis::Config::TruthJet::definition,
						   fastjet::AreaDefinition areaDef= Analysis::Config::TruthJet::areaDef,
						   double pTmin                   = Analysis::Select::TruthJet::pTmin,
						   double rapMin                  = Analysis::Select::TruthJet::rapMin,
						   double rapMax                  = Analysis::Select::TruthJet::rapMax)
  { return findJets(PseudoJetGetter::getTruthHadrons<T>(data),pTmin,rapMin,rapMax,jetDef,areaDef); }

  //////////////////
  // Jet matching //
  //////////////////

  // match truth and reco jets
  static size_t matchJets(const std::vector<fastjet::PseudoJet>& truthJets,const std::vector<fastjet::PseudoJet>& recoJets,
			  std::vector<int>& truthJetIdx,std::vector<int>& recoJetIdx,std::vector<double>& deltaR,
			  double rMatch=Analysis::Select::MatchedJet::matchR)
  {
    // build matrix of matches for n truth jets and m reco jets
    typedef boost::tuples::tuple<size_t,size_t,double,double> link_t;
    std::map<double,link_t> lookup;
    for ( size_t itruth(0); itruth<truthJets.size(); ++itruth ) {
      for ( size_t ireco(0); ireco<recoJets.size(); ++ireco ) {
	double dr(truthJets.at(itruth).delta_R(recoJets.at(ireco)));
	if ( dr < rMatch ) { lookup[dr] = link_t(itruth,ireco,truthJets.at(itruth).pt(),dr); }
      } // loop reco jets
    } // loop truth jets
    // make matches unique and store in sortable container
    auto fmap = lookup.begin();
    std::vector<size_t> uidx; std::vector<link_t> slinks;
    while ( fmap != lookup.end() ) { 
      size_t idx(fmap->second.get<0>());
      if ( std::find(uidx.begin(),uidx.end(),idx) == uidx.end() ) { uidx.push_back(idx); slinks.push_back(fmap->second); ++fmap; }
      else { fmap = lookup.erase(fmap); } // <- needs to be c++11!!!
    } // lookup iterator

    // sort matches in truth jet pT
    std::sort(slinks.begin(),slinks.end(),[](link_t& m1,link_t& m2) {
	volatile double pt1(m1.get<2>()); volatile double pt2(m2.get<2>());
	return ( pt1 > pt2 );
      }
      );

    // prepare output
    truthJetIdx.clear();
    recoJetIdx.clear();
    deltaR.clear();
    for ( auto lnk : slinks ) { truthJetIdx.push_back(lnk.get<0>()); recoJetIdx.push_back(lnk.get<1>()); deltaR.push_back(lnk.get<3>()); }
    //
    return truthJetIdx.size();
  } // matchJets

  //////////////
  // Topology //
  //////////////

  // search for central decays
  static Analysis::Matching::JetRelationContainer hadronicDecay(const std::vector<fastjet::PseudoJet>& pjvec,
								double rapMin  = Analysis::Select::Event::Decay::rapMin,
								double rapMax  = Analysis::Select::Event::Decay::rapMax,
								double minMinv = Analysis::Select::Event::Decay::minMinv,
								double maxMinv = Analysis::Select::Event::Decay::maxMinv) {
    // get masses
    Analysis::Matching::JetRelationContainer jrelcont; jrelcont.fillMass(pjvec);
    // extract results (filter)
    Analysis::Matching::JetRelationContainer jrelout; jrelout.reserve(jrelcont.size());
    for ( auto jr : jrelcont ) { 
      size_t ii(jr.jetIndex(0)); size_t jj(jr.jetIndex(1));
      if ( ii < pjvec.size() && jj < pjvec.size() ) {
	// mass range
	if ( jr.invM() > minMinv && jr.invM() < maxMinv ) { 
	  // rapidity range
	  if ( ( pjvec.at(ii).rap() > rapMin && pjvec.at(ii).rap() < rapMax ) &&
	       ( pjvec.at(jj).rap() > rapMin && pjvec.at(jj).rap() < rapMax ) ) { jrelout.push_back(jr); } 
	} // mass window
      } // valid indices
    } // loop relations
    //
    return jrelout;
  }

  // filter out decay jets
  static std::vector<fastjet::PseudoJet> removeDecay(const std::vector<fastjet::PseudoJet>& pjvec,const Analysis::Matching::JetRelationContainer& jrelcont) {
    std::vector<bool> jveto(pjvec.size(),false); 
    for ( const auto& jr : jrelcont ) { 
      size_t ii(jr.jetIndex(0)); size_t jj(jr.jetIndex(1));
      if ( ii < pjvec.size() && jj < pjvec.size() ) {
	  if ( !jveto.at(ii) ) { jveto[ii] = true; }
	  if ( !jveto.at(jj) ) { jveto[jj] = true; }
      } // valid indices
    } // loop jet relations

    // collect output
    std::vector<fastjet::PseudoJet> pjout; pjout.reserve(pjvec.size());
    for ( size_t i(0); i<pjvec.size(); ++i ) { if ( !jveto.at(i) ) { pjout.push_back(pjvec.at(i)); } }
    return pjout;
  }

  // filter out decay jets
  static std::vector<fastjet::PseudoJet> removeDecay(const std::vector<fastjet::PseudoJet>& pjvec,const Analysis::Matching::JetRelationContainer& jrelcont,
						     double rapMin,double rapMax,double minMinv,double maxMinv) {
    std::vector<bool> jveto(pjvec.size(),false); 
    for ( const auto& jr : jrelcont ) { 
      size_t ii(jr.jetIndex(0)); size_t jj(jr.jetIndex(1));
      if ( ii < pjvec.size() && jj < pjvec.size() ) {
	fastjet::PseudoJet dcyjet(pjvec.at(ii)+pjvec.at(jj));
      	if ( ( jr.invM()    > minMinv && jr.invM() < maxMinv   ) && 
	     ( dcyjet.rap() > rapMin  && dcyjet.rap() < rapMax ) ) {  
	  if ( !jveto.at(ii) ) { jveto[ii] = true; }
	  if ( !jveto.at(jj) ) { jveto[jj] = true; }
	} // within rapidity and mass window
      } // valid indices
    } // loop jet relations

    // collect output
    std::vector<fastjet::PseudoJet> pjout; pjout.reserve(pjvec.size());
    for ( size_t i(0); i<pjvec.size(); ++i ) { if ( !jveto.at(i) ) { pjout.push_back(pjvec.at(i)); } }
    return pjout;
  }

  // filter out decay jets
  static std::vector<fastjet::PseudoJet> removeDecay(const std::vector<fastjet::PseudoJet>& pjvec,
						     double rapMin  = Analysis::Select::Event::Decay::rapMin,
						     double rapMax  = Analysis::Select::Event::Decay::rapMax,
						     double minMinv = Analysis::Select::Event::Decay::minMinv,
						     double maxMinv = Analysis::Select::Event::Decay::maxMinv) {
    Analysis::Matching::JetRelationContainer jrelcont = hadronicDecay(pjvec,rapMin,rapMax,minMinv,maxMinv);
    std::vector<bool> jveto(pjvec.size(),false); 
    for ( const auto& jr : jrelcont ) { 
      size_t ii(jr.jetIndex(0)); size_t jj(jr.jetIndex(1));
      if ( ii < pjvec.size() && jj < pjvec.size() ) { 
	if ( !jveto.at(ii) ) { jveto[ii] = true; }
	if ( !jveto.at(jj) ) { jveto[jj] = true; }
      } // valid indices
    } // loop jet relations

    // collect output
    std::vector<fastjet::PseudoJet> pjout; pjout.reserve(pjvec.size());
    for ( size_t i(0); i<pjvec.size(); ++i ) { if ( !jveto.at(i) ) { pjout.push_back(pjvec.at(i)); } }
    return pjout;
  }

  // check jet self-isolation
  static bool checkJetIsolation(const std::vector<fastjet::PseudoJet>& pjvec,double isoDeltaR)
  {
    // check truth jet isolation
    auto ftj(pjvec.begin()); bool isClose(false);
    while ( ftj != pjvec.end() && !isClose ) { 
      auto ltj = ftj; ++ltj;
      while ( ltj != pjvec.end() && !isClose ) { 
	isClose = ftj->delta_R(*ltj) < isoDeltaR; ++ltj;
      }
      ++ftj;
    }
    return !isClose; 
  }

  // check minimum distance of two particle sets
  static double minDist(const std::vector<fastjet::PseudoJet>& pjvec,const std::vector<fastjet::PseudoJet>& qjvec,size_t& ipj,size_t& iqj)
  {
    ipj = 0; iqj = 0; double deltaR(100000.);
    for ( size_t i(0); i<pjvec.size(); ++i ) { 
      for ( size_t j(0); j<qjvec.size(); ++j ) {
	double dR(pjvec.at(i).delta_R(qjvec.at(j)));
	if ( dR < deltaR ) { deltaR = dR; ipj = i; iqj = j; }
      }
    }
    return deltaR;
  }

  // check internal jet distances
  static bool checkMinDist(const std::vector<fastjet::PseudoJet>& pjet,size_t njets,double pTmin)
  {
    if ( pjet.size() < 2 ) { return false; }
    if ( njets       < 2 ) { return false; }
    bool nflag(true); size_t ifj(0); size_t nfj(std::min(pjet.size(),njets)); size_t lfj(nfj-1);
    while ( ifj < lfj && nflag ) { 
      if ( pjet.at(ifj).pt() > pTmin ) { 
	size_t jfj(ifj+1);
	while ( jfj < nfj && nflag ) { 
	  if ( pjet.at(jfj).pt() > pTmin ) { 
	    nflag = pjet.at(ifj).delta_R(pjet.at(jfj)) > Analysis::Select::TruthJet::isoDeltaR; 
	  } // inner loop jet above pt threshold
	  ++jfj;
	} // inner loop
	++ifj;
      } // outer loop jet above pt threshold
    } // outer jet loop
    return  nflag;
  }

  // select invariant mass
  static bool selectMassWindow(const std::vector<fastjet::PseudoJet>& jets,double& minv,size_t& ijet,size_t& jjet)
  { invariantMass(jets,minv,ijet,jjet,false); return minv > Analysis::Select::Event::minMinv && minv < Analysis::Select::Event::maxMinv; }

  // calculate invariant mass
  static bool invariantMass(const std::vector<fastjet::PseudoJet>& jets,double& minv,size_t& ijet,size_t& jjet,bool doMassWindow=false)
  {
    typedef std::pair<size_t,size_t>   link_t;
    typedef std::map<double,link_t>    linkmap_t;
    // not enough jets
    if ( jets.empty() )     {                                                               return false; }
    if ( jets.size() == 1 ) { ijet = 0; jjet = ijet; minv = jets.front().m();               return false; }
    if ( jets.size() == 2 ) { ijet = 0; jjet = 1;    minv = (jets.front()+jets.back()).m(); return true;  }
    // find the highest invariant mass between any N>2 jets
    linkmap_t orderedLinks;
    if ( !doMassWindow ) { 
      for ( size_t i(0); i<jets.size()-1; ++i ) { 
	for ( size_t j(i+1); j<jets.size(); ++j ) { 
	  double m((jets.at(i)+jets.at(j)).m());
	  orderedLinks[m] = link_t(i,j);
	}
      }
    } else { 
      for ( size_t i(0); i<jets.size()-1; ++i ) { 
	for ( size_t j(i+1); j<jets.size(); ++j ) { 
	  double m((jets.at(i)+jets.at(j)).m());
	  if ( m > Analysis::Select::Event::minMinv && m < Analysis::Select::Event::maxMinv ) { orderedLinks[m] = link_t(i,j); }
	}
      }
    }
    // return the findings
    if ( orderedLinks.empty() ) { return false; }
    auto fmap = orderedLinks.rbegin();
    ijet = fmap->second.first;
    jjet = fmap->second.second;
    minv = fmap->first;
    return true;
  }

  static double invariantMass(const std::vector<fastjet::PseudoJet>& jets,size_t& nJets,size_t nJetsMax,double minM)
  {
    // from the input jets, start 4-vector summing from the first jet until the number of jets summed is the max number jets to be summed
    nJets = 0;
    fastjet::PseudoJet minv;
    for ( auto jt : jets ) { if ( nJets == nJetsMax || minv.m() > minM ) { break; } minv += jt; ++nJets; }
    return minv.m();
  }  
  static double invariantMass(const std::vector<fastjet::PseudoJet>& jets,size_t& nJets)
  { return invariantMass(jets,nJets,Analysis::Select::TruthJet::numMax,Analysis::Select::Event::minMinv); }

  template<class T>
  static double calculateTruthJetPTD(T* data,const fastjet::PseudoJet& pj)
  {
    // is a truth jet
    if ( pj.has_user_info<Analysis::FastJet::TruthJetInfo>() ) {
      double ptsum(0), ptsum2(0.);
      for ( size_t i(0); data->truthjet_comp->size(); ++i ) { 
	double pt(data->truthhadr_pt->at(data->truthjet_comp->at(i)));
	ptsum += pt;
	ptsum2 += pt*pt;
      }
      return ptsum > 0. ? std::sqrt(ptsum2)/ptsum : 0.;
    }
  }

  static double calculatePTD(const fastjet::PseudoJet& pj)
  {
    if ( pj.has_valid_cs() ) {
      double ptsum(0.), ptsum2(0.);
      for ( auto cj : pj.constituents() ) { ptsum += cj.pt(); ptsum2 = cj.pt()*cj.pt(); }
      return ptsum > 0. ? std::sqrt(ptsum2)/ptsum : 0.;
    }

    // is a calosignal
    return Analysis::FastJet::CaloSignalInfo::caloPTD(pj); 
  }

  static double calculateCaloJetPTD(const fastjet::PseudoJet& cjet)
  {
    //    printf("AnalysisHelper::calculateCaloJetPTD() DEBUG received jet with %2zu constituents ",cjet.constituents().size());
    if ( cjet.constituents().empty() || !AnalysisHelper::FastJet::hasCaloSignalInfo(cjet.constituents().front()) ) {
      // if ( cjet.constituents().empty() ) { 
      // 	printf(" with problems [INVALID::IS_EMPTY]\n");
      // } else { 
      // 	printf(" with problems[INVALID::NOT_CALO]\n");
      // }
      return 0.; 
    }
    //    printf("\n");
    double ptsum(0.), ptsum2(0.);
    for ( auto pj : cjet.constituents() ) { 
      double wpt(Analysis::FastJet::CaloSignalInfo::caloPTD(pj) * Analysis::FastJet::CaloSignalInfo::caloEPOS(pj));
      double xpt(wpt/std::cosh(pj.eta())); xpt *= xpt;
      ptsum2 += xpt;
      ptsum  += Analysis::FastJet::CaloSignalInfo::caloEPOS(pj)/std::cosh(pj.eta());
    }
    //    printf(" - calculated ptsum/ptsum2 = %5.3f/%5.3f\n",ptsum,ptsum2);
    return std::sqrt(ptsum2)/ptsum;
  }

  // alternative implementation
  static Analysis::Select::Event::FailureLvl acceptTruthJetEvent(const std::vector<fastjet::PseudoJet>& truthJets,unsigned int& passLvl,size_t& iJet,size_t& jJet,double& invMass,double& rapGap)
  {
    bool massTest(invariantMass(truthJets,invMass,iJet,jJet));
    // number of truth jets too low?
    if ( truthJets.size() < Analysis::Select::TruthJet::numMin ) 
      { passLvl |= Analysis::Select::Event::FailNTruthJet; return Analysis::Select::Event::FailNTruthJet; } 
    passLvl |= Analysis::Select::Event::PassNTruthJet;
    // rapidity gap (needs the two jets forming the hard system)
    rapGap = massTest ? std::abs(truthJets.at(iJet).rap()-truthJets.at(jJet).rap()) : 0.;
    if ( rapGap < Analysis::Select::Event::rapGap ) 
      { passLvl |= Analysis::Select::Event::FailRapGap; return Analysis::Select::Event::FailRapGap; }
    passLvl |= Analysis::Select::Event::PassRapGap;
    // opposite hemispheres 
    if ( !massTest || truthJets.at(iJet).rap() * truthJets.at(jJet).rap() > 0. ) { 
      passLvl |= Analysis::Select::Event::FailHemisphere; return Analysis::Select::Event::FailHemisphere;
    }
    passLvl |= Analysis::Select::Event::PassHemisphere;
    // minimum distance between hardest pT jets
    if ( !massTest && !checkMinDist(truthJets,std::max(iJet,jJet),Analysis::Select::TruthJet::pTmin) ) { 
      { passLvl |= Analysis::Select::Event::FailIsolation; return Analysis::Select::Event::FailIsolation; }
    }
    passLvl |= Analysis::Select::Event::PassIsolation;
    // invariant mass not in window
    bool mrange(Analysis::Select::Event::minMinv < Analysis::Select::Event::maxMinv);
    if ( mrange ) {  
      if ( invMass < Analysis::Select::Event::minMinv || invMass > Analysis::Select::Event::maxMinv ) { 
	passLvl |= Analysis::Select::Event::FailMinv; return Analysis::Select::Event::FailMinv; 
      }
    } else { 
      if ( invMass < Analysis::Select::Event::minMinv ) { 
	passLvl |= Analysis::Select::Event::FailMinv; return Analysis::Select::Event::FailMinv; 
      }
    }
    passLvl |= Analysis::Select::Event::PassMinv;
    //
    return Analysis::Select::Event::Pass;
  }

  // return signal and pileup jets from calo jets
  static bool filterJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& signal,std::vector<fastjet::PseudoJet>& pileup) {
    signal.clear(); pileup.clear();
    for ( const auto& pj : pjvec ) {
      if ( AnalysisHelper::FastJet::isSignal(pj) ) { 
	signal.push_back(pj);
      } else if ( AnalysisHelper::FastJet::isPileup(pj) ) {
	pileup.push_back(pj);
      }
    }
    return !signal.empty() || !pileup.empty();
  }

  // return actual index of jet
  static size_t jetIndex(const std::vector<fastjet::PseudoJet>& pjvec,size_t dataIdx) {
    size_t i(0);
    while ( i < pjvec.size() && (size_t)Analysis::FastJet::UserInfo::dataIndex(pjvec.at(i)) != dataIdx ) { ++i; }
    return i < pjvec.size() ? i : size_t(-1);
  }

  // checks if jet is in collection
  static bool checkJet(const std::vector<fastjet::PseudoJet>& pjvec,size_t dataIdx) {
    size_t i(0);
    while ( i < pjvec.size() && (size_t)Analysis::FastJet::UserInfo::dataIndex(pjvec.at(i)) != dataIdx ) { ++i; }
    return i < pjvec.size();
  }

  ///////////////////
  // Other helpers //
  ///////////////////

  // formatting help
  static int ndigits(int n) 
  { return n == 0 ? 1 : n > 0  ? static_cast<int>(std::log10(n))+1 : static_cast<int>(std::log10(std::abs(n)))+2 ; }

  ///@brief Print calorimeter signals
  static void printCaloSignal(const std::string& module,const std::vector<fastjet::PseudoJet>& pjVec)
  {
    if ( pjVec.empty() ) { PRINT_WARNING(module,"# (no entries in vector!)"); return; }
    std::string typeName = Analysis::FastJet::CaloSignalInfo::signalName(pjVec.front());
    if ( typeName == "Unknown" ) { PRINT_WARNING( module, "signal type \042%s\042 not printable", typeName.c_str() ); return; }
    PRINT_DEBUG(module," ");
    PRINT_DEBUG(module,"#######################################################################################################");
    PRINT_DEBUG(module,"##### Calorimeter signal type \042%13.13s\042 has %5zu signals", typeName.c_str(), pjVec.size());
    PRINT_DEBUG(module,"#######################################################################################################");
    PRINT_DEBUG(module,"#  #     pT [GeV]     y   phi [rad]  t [ns]   E [GeV]   E+ [GeV] Eorig [GeV]   Area   PtD  ");
    //                 #X+++++X+++++.+++XX++.+++XXX++.+++XX++++.+++X+++++.+++X+++++.+++XX+++++.+++XX+++.+++X++.+++X[ghosted]X#
    //                "#######################################################################################################"
    size_t i(0);
    for ( auto pj : pjVec ) { 
      double pt(pj.pt()/1000.); double rap(pj.rap()); double phi(pj.phi_std()); 
      double t(Analysis::FastJet::CaloSignalInfo::caloTime(pj));
      double e(pj.e()/1000.);
      double ep(Analysis::FastJet::CaloSignalInfo::caloEPOS(pj)/Analysis::Units::GeV);
      double eo(Analysis::FastJet::CaloSignalInfo::origE(pj)/Analysis::Units::GeV);
      double a(Analysis::FastJet::CaloSignalInfo::caloArea(pj));
      double ptd(Analysis::FastJet::CaloSignalInfo::caloPTD(pj));
      if ( pj.user_info<Analysis::FastJet::CaloSignalInfo>().isGhost() ) { 
	PRINT_DEBUG(module,"# %5zu %9.3f  %6.3f   %6.3f  %8.3f %9.3f %9.3f  %9.3f  %7.3f %6.3f [ghosted] #",i,pt,rap,phi,t,e,ep,eo,a,ptd);
      } else {
	PRINT_DEBUG(module,"# %5zu %9.3f  %6.3f   %6.3f  %8.3f %9.3f %9.3f  %9.3f  %7.3f %6.3f           #",i,pt,rap,phi,t,e,ep,eo,a,ptd);
      }
      ++i;
    }
    PRINT_DEBUG(module,"#######################################################################################################");
    PRINT_DEBUG(module," ");
  }

  // number of constituents
  static size_t numConstits(const fastjet::PseudoJet& pj) 
  { 
    return pj.has_valid_cluster_sequence() 
      ? pj.constituents().size()            : pj.has_user_info<Analysis::FastJet::TruthJetInfo>()
      ? pj.user_info<Analysis::FastJet::TruthJetInfo>().size() : 0; 
  }

  // print banner
  static void printBanner(const std::string& module,Analysis::Config::Input::Selector sel) {
    printf("\n");
    switch ( sel )
      {
      case Analysis::Config::Input::TopoCluster:
	Analysis::Banner::printTopoCluster(module);
	break;
      case Analysis::Config::Input::TopoTower:
	Analysis::Banner::printTopoTower(module);
	break;
      case Analysis::Config::Input::TopoTowerFine:
	Analysis::Banner::printTopoTowerFine(module);
	break;
      default:
	PRINT_WARNING( module, "collection type is <Unknown>" );
	break;
      }
    printf("\n");	
  }
  static void printBanner(const std::string& module,const std::string& signal) { printBanner(module,Analysis::Config::Input::selectorType(signal)); }

  static void printCaloTimingFilter(const std::string& module) {
    printf("\n");
    Analysis::Banner::print(module,Analysis::Banner::Filter::CaloTiming::text);
    printf("\n");
  }

  static int printPulse(const std::string& module,int nevt,int ntot) {
    int nd = ndigits(ntot);
    ++nevt;
    if ( nevt == 1 || nevt % Analysis::Config::Job::tickerFreq == 0 ) { PRINT_INFO( module, "event (read/total) %*i/%*i",nd,nevt,nd,ntot); }
    return nevt;
  }

  struct Event { 
    template<class T>
    static double centralRho(T* data,const std::string& scale="lcw")
    { return scale == "lcw" ? data->event_rho_lcw : scale == "em" ? data->event_rho_em : 0.; }
    template<class T>
    static double centralSigma(T* data,const std::string& scale="lcw")
    { return scale == "lcw" ? data->event_sigma_lcw : scale == "em" ? data->event_sigma_em : 0.; }
    template<class T>
    static double centralArea(T* data,const std::string& scale="lcw")
    { return scale == "lcw" ? data->event_area_lcw : scale == "em" ? data->event_area_em : 0.; }
    template<class T> 
    static bool centralDensity(T* data,const std::string& scale,double& rho,double& sigma,double& area) 
    { 
      if ( scale == "lcw" ) {
	rho = data->event_rho_lcw; sigma = data->event_sigma_lcw; area = data->event_area_lcw; 
	return true;
      } else if ( scale == "em" ) { 
	rho = data->event_rho_em; sigma = data->event_sigma_em; area = data->event_area_em; 
	return true;
      } else { 
	return false;
      }
    }
    template<class T>
    static double mu(T* data) { return data->event_mu; }
    template<class T>
    static double npv(T* data) { return data->event_npv; }
    //
    static double centralRho(SlidingWindow::Window& rhoProf,double rapMin=Analysis::Config::RhoProfile::rapCtrlMin,double rapMax=Analysis::Config::RhoProfile::rapCtrlMax) {
      std::vector<double> rp = rhoProf.findMedians();
      size_t rapIdxMin(rhoProf.binIndex(rapMin));
      size_t rapIdxMax(rhoProf.binIndex(rapMax));
      std::vector<double> ctrlrhos;
      for ( size_t i(rapIdxMin); i<=rapIdxMax; ++i ) { ctrlrhos.push_back(rp.at(i)); }
      return SlidingWindow::Window::median(ctrlrhos);
    }
    static double centralSigma(SlidingWindow::Window& rhoProf,double rapMin=Analysis::Config::RhoProfile::rapCtrlMin,double rapMax=Analysis::Config::RhoProfile::rapCtrlMax) {
      std::vector<double> rp = rhoProf.findMedians();
      size_t rapIdxMin(rhoProf.binIndex(rapMin));
      size_t rapIdxMax(rhoProf.binIndex(rapMax));
      std::vector<double> ctrlrhos;
      for ( size_t i(rapIdxMin); i<=rapIdxMax; ++i ) { ctrlrhos.push_back(rp.at(i)); }
      double rhomed(SlidingWindow::Window::median(ctrlrhos));
      double sigma(0.); size_t nctr(0);
      for ( size_t i(0); i<ctrlrhos.size(); ++i ) { if ( ctrlrhos.at(i) > rhomed ) { sigma += std::pow(ctrlrhos.at(i)-rhomed,2); ++nctr; } }
      return nctr > 0 ? std::sqrt(sigma/((double)nctr)) : 0.;
    }
    static bool centralDensity(SlidingWindow::Window& rhoProf,double& rho,double& sigma,
			       double rapMin=Analysis::Config::RhoProfile::rapCtrlMin,double rapMax=Analysis::Config::RhoProfile::rapCtrlMax) {
      std::vector<double> rp = rhoProf.findMedians();
      size_t rapIdxMin(rhoProf.binIndex(rapMin));
      size_t rapIdxMax(rhoProf.binIndex(rapMax));
      if ( rapIdxMin >= rapIdxMax || rp.empty() ) { return false; }
      std::vector<double> ctrlrhos;
      for ( size_t i(rapIdxMin); i<=rapIdxMax; ++i ) { ctrlrhos.push_back(rp.at(i)); }
      rho = SlidingWindow::Window::median(ctrlrhos);
      sigma = 0.; size_t nctr(0);
      for ( size_t i(0); i<ctrlrhos.size(); ++i ) { if ( ctrlrhos.at(i) > rho ) { sigma += std::pow(ctrlrhos.at(i)-rho,2); ++nctr; } }
      sigma = nctr > 0 ? std::sqrt(sigma/((double)nctr)) : 0.;
      return nctr > 0;
    }
  }; // Event

  static bool fillRhoProfile(SlidingWindow::Window* rhoProf,const std::vector<fastjet::PseudoJet>& calosignal) {
    rhoProf->reset();                           // reset the profile from previous event
    if ( calosignal.empty() ) { return false; } // empty signal container
    for ( auto pj : calosignal ) {
      if ( FastJet::hasCaloSignalInfo(pj) && !Analysis::FastJet::UserInfo::isGhost(pj) ) { rhoProf->addObject(pj,Analysis::FastJet::area(pj)); }
    }
    return true;
  }

  static bool fillRhoProfile(SlidingWindow::Window* rhoProf,const std::vector<fastjet::PseudoJet>& calosignal,double area) {
    rhoProf->reset();                           // reset the profile from previous event
    if ( calosignal.empty() ) { return false; } // empty signal container
    for ( auto pj : calosignal ) {
      if ( FastJet::hasCaloSignalInfo(pj) && !Analysis::FastJet::UserInfo::isGhost(pj) ) { rhoProf->addObject(pj,area); }
    }
  return true;
  }



  static std::vector<int> getSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& sigpj,double ptmin,double rho,double sigma) {
    std::vector<int> puidx; puidx.reserve(pjvec.size()); sigpj.reserve(pjvec.size());
    for ( size_t i(0); i<pjvec.size(); ++i ) { 
      double area(Analysis::FastJet::area(pjvec.at(i)));
      if ( area > 0. ) { 
	double ptcorr(pjvec.at(i).pt()-(area*rho+sigma*std::sqrt(area)));
	if ( ptcorr > ptmin ) { sigpj.push_back(fastjet::PseudoJet(pjvec.at(i))); sigpj.back() *= ptcorr/pjvec.at(i).pt(); } else { puidx.push_back((int)i); }
      } 
    }
    return puidx;
  }

  static std::vector<int> getSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& sigpj,double rho,double sigma)
  { return getSignalJets(pjvec,sigpj,Analysis::Select::RecoJet::pTmin,rho,sigma); } 

  static std::vector<int> getSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& sigpj,SlidingWindow::Window& rhoProf,double ptmin) {
    std::vector<int> puidx; puidx.reserve(pjvec.size()); sigpj.reserve(pjvec.size()); sigpj.clear();
    std::vector<double> rp = rhoProf.findMedians();
    for ( size_t i(0); i<pjvec.size(); ++i ) { 
      size_t rapIdx(rhoProf.binIndex(pjvec.at(i).rap())); 
      if ( rapIdx < rp.size() ) {
	double area(Analysis::FastJet::area(pjvec.at(i)));
	double rho(rp.at(rapIdx));
	double ptcorr(pjvec.at(i).pt()-rho*area); 
	if ( ptcorr > ptmin ) { sigpj.push_back(fastjet::PseudoJet(pjvec.at(i))); sigpj.back() *= ptcorr/pjvec.at(i).pt(); } else { puidx.push_back((int)i); }
      }
    }
    return puidx;
  }

  static std::vector<int> getSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& sigpj,SlidingWindow::Window& rhoProf) {
    return getSignalJets(pjvec,sigpj,rhoProf,Analysis::Select::RecoJet::pTmin); 
  }

  static std::vector<int> getSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& sigpj,SlidingWindow::Window& rhoProf,double ptmin,bool useSigma,
					double rapMin=Analysis::Config::RhoProfile::rapCtrlMin,double rapMax=Analysis::Config::RhoProfile::rapCtrlMax)
  {
    std::vector<int> puidx; puidx.reserve(pjvec.size()); sigpj.reserve(pjvec.size()); sigpj.clear();
    double rho(0.); double sigma(0.);
    bool calcOk(true);
    if ( useSigma ) {
      calcOk = rapMin < rapMax ? Event::centralDensity(rhoProf,rho,sigma,rapMin,rapMax) : Event::centralDensity(rhoProf,rho,sigma); 
    } else {
      rho = rapMin < rapMax ? Event::centralRho(rhoProf,rapMin,rapMax) : Event::centralRho(rhoProf);
    }
    //
    for ( size_t i(0); i<pjvec.size(); ++i ) { 
      double area(Analysis::FastJet::area(pjvec.at(i)));
      double ptcorr(pjvec.at(i).pt()-(area+sigma)*rho);
      if ( ptcorr > ptmin ) { sigpj.push_back(fastjet::PseudoJet(pjvec.at(i))); sigpj.back() *= ptcorr/pjvec.at(i).pt(); } else { puidx.push_back((int)i); }
    }
    return puidx;
  }

  static std::vector<int> getSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& sigpj,SlidingWindow::Window& rhoProf,bool useSigma,
					double rapMin=Analysis::Config::RhoProfile::rapCtrlMin,double rapMax=Analysis::Config::RhoProfile::rapCtrlMax)
  { return getSignalJets(pjvec,sigpj,rhoProf,Analysis::Select::RecoJet::pTmin,useSigma,rapMin,rapMax); }

  static std::vector<fastjet::PseudoJet> extractSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& puvec,double ptmin,double rho,double sigma) { 
    std::vector<fastjet::PseudoJet> sjets; sjets.reserve(pjvec.size());
    puvec.clear(); puvec.reserve(pjvec.size());
    for ( const auto& pj : pjvec ) { 
      double area(Analysis::FastJet::area(pj));
      if ( area > 0. ) { 
	double ptcorr(pj.pt()-(area*rho+sigma*std::sqrt(area)));
	if ( ptcorr > ptmin ) { sjets.push_back(pj); sjets.back() *= ptcorr/pj.pt(); } else { puvec.push_back(pj); }
      } // area is physical
    } // loop on input jets
    return sjets;
  } 

  static std::vector<fastjet::PseudoJet> extractSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& puvec,double rho,double sigma)
  { return extractSignalJets(pjvec,puvec,Analysis::Select::RecoJet::pTmin,rho,sigma); } 

  static std::vector<fastjet::PseudoJet> extractSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& puvec,SlidingWindow::Window& rhoProf,double ptmin) {
    std::vector<fastjet::PseudoJet> sjets; sjets.reserve(pjvec.size());
    puvec.clear(); puvec.reserve(pjvec.size());
    std::vector<double> rp = rhoProf.findMedians();
    for ( const auto& pj : pjvec ) { 
      size_t rapIdx(rhoProf.binIndex(pj.rap())); 
      if ( rapIdx < rp.size() ) {
	double area(Analysis::FastJet::area(pj));
	double rho(rp.at(rapIdx));
	double ptcorr(pj.pt()-rho*area); 
	if ( ptcorr > ptmin ) { sjets.push_back(pj); sjets.back() *= ptcorr/pj.pt(); } else { puvec.push_back(pj); }
      } // within rapidity range
    } // loop on jets
    return sjets;
  }

  static std::vector<fastjet::PseudoJet> extractSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,std::vector<fastjet::PseudoJet>& puvec,SlidingWindow::Window& rhoProf) {
    return extractSignalJets(pjvec,puvec,rhoProf,Analysis::Select::RecoJet::pTmin); 
  }

  static std::vector<fastjet::PseudoJet> extractSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,
							   std::vector<fastjet::PseudoJet>&       puvec,
							   SlidingWindow::Window& rhoProf,
							   double ptmin,
							   bool useSigma,
							   double rapMin=Analysis::Config::RhoProfile::rapCtrlMin,double rapMax=Analysis::Config::RhoProfile::rapCtrlMax)
  {
    std::vector<fastjet::PseudoJet> sjets; sjets.reserve(pjvec.size());
    puvec.clear(); puvec.reserve(pjvec.size());
    double rho(0.); double sigma(0.);
    bool calcOk(true);
    if ( useSigma ) {
      calcOk = rapMin < rapMax ? Event::centralDensity(rhoProf,rho,sigma,rapMin,rapMax) : Event::centralDensity(rhoProf,rho,sigma); 
    } else {
      rho = rapMin < rapMax ? Event::centralRho(rhoProf,rapMin,rapMax) : Event::centralRho(rhoProf);
    }
    //
    for ( const auto& pj : pjvec ) { 
      double area(Analysis::FastJet::area(pj));
      double ptcorr(pj.pt()-(area+sigma)*rho);
      if ( ptcorr > ptmin ) { sjets.push_back(pj); sjets.back() *= ptcorr/pj.pt(); } else { puvec.push_back(pj); }
    }
    return sjets;
  }

  static std::vector<fastjet::PseudoJet> extractSignalJets(const std::vector<fastjet::PseudoJet>& pjvec,
							   std::vector<fastjet::PseudoJet>&       puvec,
							   SlidingWindow::Window& rhoProf,
							   bool useSigma,
							   double rapMin=Analysis::Config::RhoProfile::rapCtrlMin,double rapMax=Analysis::Config::RhoProfile::rapCtrlMax)
  { return extractSignalJets(pjvec,puvec,rhoProf,Analysis::Select::RecoJet::pTmin,useSigma,rapMin,rapMax); }

  ///@brief Constituent subtractor
  static std::vector<fastjet::PseudoJet> constituentSubtractor(const std::vector<fastjet::PseudoJet>& pjvec,SlidingWindow::Window* rhoProf) {
    std::vector<fastjet::PseudoJet> outvec; outvec.reserve(pjvec.size()); 
    // check input and calorimeter signal
    if ( pjvec.empty() || !FastJet::hasCaloSignalInfo(pjvec.front()) ) { return outvec; }
    // check calorimeter signal type
    if ( Analysis::FastJet::CaloSignalInfo::signalType(pjvec.front()) != Analysis::FastJet::CaloSignalInfo::TopoCluster ) { 
      // tower
      for ( auto pj : pjvec ) {
	if ( pj.e() > 0. )  { 
	  std::vector<double> rm = rhoProf->findMedians();
	  size_t idx(rhoProf->binIndex(pj.rap()));
	  if ( idx < rm.size() ) {
	    double ptcorr(pj.pt() - rm.at(idx) * Analysis::FastJet::CaloSignalInfo::caloArea(pj)); 
	    if ( ptcorr > 0. ) { 
	      fastjet::PseudoJet npj(pj); npj *= ptcorr/pj.pt(); outvec.push_back(npj); 
	    }
	  }
	}
      }
      // cluster
    } else {
      SlidingWindow::RhoProvider rhoProvider(rhoProf);
      outvec = fastjet::contrib::ConstituentSubtractor(&rhoProvider).subtract_event(fastjet::SelectorPtMin(1.1*Analysis::FastJet::Ghosts::filterPt)(pjvec),
										    Analysis::Select::CaloSignal::rapMax);
    }
    return outvec;
  }

  ///@brief Constituent subtractor
  static std::vector<fastjet::PseudoJet> constituentSubtractor(const std::vector<fastjet::PseudoJet>& pjvec,double rho) {
    std::vector<fastjet::PseudoJet> outvec; outvec.reserve(pjvec.size()); 
    // check input and calorimeter signal
    if ( pjvec.empty() || !FastJet::hasCaloSignalInfo(pjvec.front()) ) { return outvec; }
    // check calorimeter signal type
    if ( Analysis::FastJet::CaloSignalInfo::signalType(pjvec.front()) != Analysis::FastJet::CaloSignalInfo::TopoCluster ) { 
      // tower
      for ( auto pj : pjvec ) {
	if ( pj.e() > 0. )  { 
	  double ptcorr(pj.pt() - rho * Analysis::FastJet::CaloSignalInfo::caloArea(pj)); 
	  if ( ptcorr > 0. ) { fastjet::PseudoJet npj(pj); npj *= ptcorr/pj.pt(); outvec.push_back(npj); }
	}
      }
      // cluster
    } else {
      outvec = fastjet::contrib::ConstituentSubtractor(rho).subtract_event(fastjet::SelectorPtMin(1.1*Analysis::FastJet::Ghosts::filterPt)(pjvec),
									   Analysis::Select::CaloSignal::rapMax);
    }
    return outvec;
  }

  ///@brief SoftKiller
  static std::vector<fastjet::PseudoJet> softKiller(const std::vector<fastjet::PseudoJet>& pjvec) {
    fastjet::contrib::SoftKiller sk(Analysis::Select::CaloSignal::rapMax,Analysis::Config::Filter::Pileup::CaloSignal::SoftKiller::gridSize);
    return sk(fastjet::SelectorPtMin(1.1*Analysis::FastJet::Ghosts::filterPt)(pjvec));
  }

  ///@brief SoftKiller
  static std::vector<fastjet::PseudoJet> softKiller(const std::vector<fastjet::PseudoJet>& pjvec,double& pTthreshold) {
    std::vector<fastjet::PseudoJet> outvec; outvec.reserve(pjvec.size());
    fastjet::contrib::SoftKiller sk(Analysis::Select::CaloSignal::rapMax,Analysis::Config::Filter::Pileup::CaloSignal::SoftKiller::gridSize);
    sk.apply(fastjet::SelectorPtMin(1.1*Analysis::FastJet::Ghosts::filterPt)(pjvec),outvec,pTthreshold);
    return outvec;
  }

  // calculate jet properties
  struct JetProperties 
  {
    static double ptD(const fastjet::PseudoJet& pj) {
      if ( !pj.has_valid_cs() ) { return -1.; }
      double spt2(0.);
      double spt(0.);
      for ( auto cs : pj.constituents() ) {
	if ( !Analysis::FastJet::UserInfo::isGhost(cs) ) {
	  spt2 += cs.pt() * cs.pt();
	  spt  += cs.pt();
	}
      }
      return spt > 0. ? std::sqrt(spt2)/spt : -1.;
    }
    static double width(const fastjet::PseudoJet& pj) { 
      if ( !pj.has_valid_cs() ) { return -1.; }
      double spt(0.);
      double sptr(0.);
      for ( auto cs : pj.constituents() ) {
	if ( !Analysis::FastJet::UserInfo::isGhost(cs) ) { 
	  spt  += cs.pt();
	  sptr += cs.pt() * pj.delta_R(cs); 
	}
      }
      return spt > 0. ? sptr/spt : -1. ;
    }
  };
};

typedef AnalysisHelper AH;

#endif
