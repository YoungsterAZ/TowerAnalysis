// -*- c++ -*-
#ifndef ANALYSISDATA_H
#define ANALYSISDATA_H

#include <string>
#include <map>
#include <vector>
#include <limits>

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"

#include <boost/tuple/tuple.hpp>

#include "AnalysisCfg.h"

//-//////////////////////////////////////////////////
// Event & Object selections, analysis parameters ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-//////////////////////////////////////////////////

///@brief Analysis configuration data
///
/// The data is organized as static variables. They all have default values which can be overwritten
/// in code (by assignment, all non-const variables) or using a configuration file (selected variables,
/// updated as needed).
///
/// The configuration file syntax is:
///@code{.unparsed}
/// <varname> = <value>
///@endcode
/// where @c <varname> is the variable name (including namespace) and @c <value> is the assigned value.
/// Example:
///@code{.cpp}
/// Analysis::Config::Job::printDebug = true 
///@endcode
/// turns on the debug printing.
namespace Analysis {
  ///@brief Configuration of selections
  ///
  /// This namespace holds parameters used for selecting objects in the analysis
  namespace Select {
    ///@name Jet selections
    ///@{
    ///@brief Reconstructed (detector) jet
    namespace RecoJet {
      static double pTmin     = 20000.;                              ///< @f$ p_{\rm T,jet}^{\rm min} [MeV] @f$ 
      static double etaMin    = -4.5;                                ///< Lower boundary of @f$ \eta @f$ range 
      static double etaMax    =  4.5;                                ///< Upper boundary of @f$ \eta @f$ range 
      static double rapMin    = etaMin;                              ///< Lower boundary of rapidity range
      static double rapMax    = etaMax;                              ///< Upper boundary of rapidity range
      static size_t numMin    = 2;                                   ///< Minimum number of jets with @f$ p_{\rm T,jet} > p_{\rm T,jet}^{\rm min} @f$ required 
      static size_t numMax    = std::numeric_limits<size_t>::max();  ///< Maximum number of jets with @f$ p_{\rm T,jet} > p_{\rm T,jet}^{\rm min} @f$ considered
      static double isoDeltaR = std::numeric_limits<double>::min();  ///< Minimum angular distance between the two jets with highest @f$ p_{\rm T,jet} @f$ required
      static double timeMin   = std::numeric_limits<double>::min();  ///< Lower boundary of acceptable jet time range [ns]
      static double timeMax   = std::numeric_limits<double>::max();  ///< Upper boundary of acceptable jet time range [ns]
      static bool   applyTiming = false;                             ///< Controls application of jet selection based on timing
    }
    ///@brief Truth (generated particle level) jet
    namespace TruthJet {
      static double pTmin        = Analysis::Select::RecoJet::pTmin/2.;///<@copydoc Analysis::Select::RecoJet::pTmin
      static double etaMin       = -4.5;                               ///<@copydoc Analysis::Select::RecoJet::etaMin
      static double etaMax       =  4.5;                               ///<@copydoc Analysis::Select::RecoJet::etaMax
      static double rapMin       = etaMin;                             ///<@copydoc Analysis::Select::RecoJet::rapMin
      static double rapMax       = etaMax;                             ///<@copydoc Analysis::Select::RecoJet::rapMax
      static size_t numMin       = 2;                                  ///<@copydoc Analysis::Select::RecoJet::numMin
      static size_t numMax       = 6;                                  ///<@copydoc Analysis::Select::RecoJet::numMax
      static double isoDeltaR    = 1.0;                                ///< Isolation radius against other truth jets
      static double leptonDeltaR = isoDeltaR;                          ///< Isolation radius against leptons
      static double leptonPtMin  = 10000.;                             ///< @f$ p_{{\rm T},\ell}^{\rm min} [MeV] @f$ for leptons considered for isolation
    }
    ///@brief Matched detector jet
    ///
    /// Selection criteria for detecotr jet matched to a truth jet
    namespace MatchedJet { 
      static double pTmin     = Analysis::Select::RecoJet::pTmin;   ///<@copydoc Analysis::Select::RecoJet::pTmin
      static double etaMin    = -4.5;                               ///<@copydoc Analysis::Select::RecoJet::etaMin
      static double etaMax    =  4.5;                               ///<@copydoc Analysis::Select::RecoJet::etaMax
      static double rapMin    = etaMin;                             ///<@copydoc Analysis::Select::RecoJet::rapMin
      static double rapMax    = etaMax;                             ///<@copydoc Analysis::Select::RecoJet::rapMax
      static size_t numMin    = Analysis::Select::TruthJet::numMin; ///<@copydoc Analysis::Select::RecoJet::numMin
      static size_t numMax    = Analysis::Select::TruthJet::numMax; ///<@copydoc Analysis::Select::RecoJet::numMax
      static double isoDeltaR = std::numeric_limits<double>::min(); ///< Isolation radius against other matched jets
      static double matchR    = 0.4;                                ///< @f$ R_{\rm match} @f$ such that @f$ \Delta R = \sqrt{\Delta y^{2}+\Delta\phi^{2}} < R_{\rm match} @f$ is matched
    }
    ///@}
    ///@name Detector signal and particle selections
    ///@{
    ///@brief Calorimeter signal selection
    namespace CaloSignal {
      static double eMin      = std::numeric_limits<double>::min(); ///< Minimum signal considered [MeV]
      static double eMinReco  = 0.;                                 ///< Minimum reconstructed energy requirement for physics object reconstruction [MeV]
      static double timeMin   = std::numeric_limits<double>::min(); ///< Lower boundary of acceptable signal time range [ns]
      static double timeMax   = std::numeric_limits<double>::max(); ///< Upper boundary of acceptable signal time range [ns]
      static double signifMin = std::numeric_limits<double>::min(); ///< Lower boundary for signal significance
      static double signifMax = std::numeric_limits<double>::max(); ///< Upper boundary for signal significance
      static double etaMin    = -4.9;                               ///< Lower boundary of @f$ \eta @f$ range for signal
      static double etaMax    =  4.9;                               ///< Upper boundary of @f$ \eta @f$ range for signal
      static double rapMin    = etaMin;                             ///< Lower boundary rapidity range for signal
      static double rapMax    = etaMax;                             ///< Upper boundary rapidity range for signal
      static bool   applyTiming = false;                            ///< Apply timing cuts if @c true
    }
    ///@brief Particle selection
    namespace Particle {
      static double etaExtend = 1.;                                 ///< Extension of detector @f$ \eta @f$ coverage for particles
      static double etaMin    = CaloSignal::etaMin-etaExtend;       ///< Lower boundary of @f$ \eta @f$ range 
      static double etaMax    = CaloSignal::etaMax+etaExtend;       ///< Upper boundary of @f$ \eta @f$ range
      static double rapMin    = etaMin;                             ///< Lower boundary rapidity range
      static double rapMax    = etaMax;                             ///< Upper boundary rapidity range
    }
    ///@}
    //@name Event selection
    ///@{
    ///@brief Event selection parameters  
    namespace Event {
      ///@brief Decay characterization
      namespace Decay {
	static double rapMin  = -2.5;                       ///< Rapidity range for decay products (lower boundary)
	static double rapMax  =  2.5;                       ///< Rapidity range for decay products (upper boundary)
	static double minMinv =  40.*Analysis::Units::GeV;  ///< Invariant mass window (lower boundary)
	static double maxMinv =  160.*Analysis::Units::GeV; ///< Invariant mass window (upper boundary) 
      } 
      ///@brief Failure level indicators
      enum FailureLvl { Pass           = 0x1000, ///< Event passed
			PassTruthJet   = 0x1010, ///< Event passed truth jet selection
			PassNTruthJet  = 0x1011, ///< Event passed minimum number of truth jets selection
			PassRecoJet    = 0x1020, ///< Event passed reco jet selection
			PassNRecoJet   = 0x1021, ///< Event passed minimum number of reco jets selection
			PassLeptons    = 0x1040, ///< Event passed lepton selection
			PassTopology   = 0x1080, ///< Event passed topology filter
			PassMinv       = 0x1081, ///< Event passed invariant mass window selection
			PassRapGap     = 0x1082, ///< Event passed rapidity gap selection 
			PassIsolation  = 0x1084, ///< Event passed isolation criteria
			PassHemisphere = 0x1088, ///< Event passed different hemisphere requirements
			Fail           = 0x2000, ///< Event failed
			FailTruthJet   = 0x2010, ///< Event failed truth jet selection
			FailNTruthJet  = 0x2011, ///< Event failed minimum number of truth jets selection
			FailRecoJet    = 0x2020, ///< Event failed reco jet selection
			FailNRecoJet   = 0x2021, ///< Event failed minimum number of reco jets selection
			FailTopology   = 0x2040, ///< Event failed toppology filter
			FailMinv       = 0x2041, ///< Event failed invariant mass window selection
			FailRapGap     = 0x2042, ///< Event failed rapidity gap selection
			FailIsolation  = 0x2044, ///< Event failed isolation criteria
			FailHemisphere = 0x2048, ///< Event failed different hemisphere requirements 
			Unknown        = 0x0000 };
      static double rapGapMin = 1.0;                                                                     ///< Minimum required rapidity gap
      static double rapGapMax = Analysis::Select::TruthJet::etaMax - Analysis::Select::TruthJet::etaMin; ///< Maximum required apidity gap
      static double rapGap    = rapGapMin;                                                               ///< @copydoc rapGapMin
      static double sHat      = 2.6*2.6*Analysis::Units::TeV*Analysis::Units::TeV;                       ///< Central @f$ \hat{s} [MeV] @f$
      static double sHatMin   = 0.75*sHat;                                                               ///< Lower boundary @f$ \hat{s} @f$ range [MeV]
      static double sHatMax   = 1.25*sHat;                                                               ///< Upper boundary @f$ \hat{s} @f$ range [MeV]
      static double minMinv   = std::sqrt(sHatMin);                                                      ///< Lower edge invariant mass window [MeV]
      static double maxMinv   = std::sqrt(sHatMax);                                                      ///< Upper edge invariant mass window [MeV]
      ///@brief Check if given @c FailureLevel is passed  
      static bool check(unsigned int flvl,FailureLvl test) { return ( flvl & test ) == test;  }
      ///@brief Check if given failure level indicator is a "pass"
      static bool pass(unsigned int flvl)                  { return check(flvl,Pass);         }
      ///@brief Check if given failure level indicator flags "passing truth jet selections"
      static bool passTruthJet(unsigned int flvl)          { return check(flvl,PassTruthJet); }
      ///@brief Check if given failure level indicator flags "passing reco jet selections"
      static bool passRecoJet(unsigned int  flvl)          { return check(flvl,PassRecoJet);  }      
      ///@brief Check if given failure level indicator flags "passing lepton selections"
      static bool passLeptons(unsigned int flvl)           { return check(flvl,PassLeptons);  }
      ///@brief Check if given failure level indicator flags "passing topology selections"
      static bool passTopology(unsigned int flvl)          { return check(flvl,PassTopology); }
      ///@brief Check if given failure level indicator is a "fail"
      static bool fail(unsigned int flvl)                  { return check(flvl,Fail);         }
    }
    ///@}
  }
  ///@brief Configuration parameters
  ///
  /// This namespace collects all job configuration parameters related to messaging, application of certain
  /// methods, I/O descriptors, etc.
  namespace Config {
    ///@brief Job configuration
    ///
    /// Setting up all general aspects of the job. 
    namespace Job {
      static bool printDebug = false;        ///< Controls debug printing (@c true is "on") 
      static int  msgsDebug  = 20;           ///< Maxiumum number of debug messages
      static int  tickerFreq = 500;          ///< Ticker frequency controls message on processed number of input events (default every 500 events)
      static bool doSlidingWindowRho = false;///< Tunrs on/off @f$ \rho(\eta) @f$ calculation using sliding windows (default @c false os "off") 
    }
    ///@brief Define input
    namespace Input {
      ///@brief Available input data type tags
      enum Selector { TopoCluster = 0x11, TopoTower=0x21, TopoTowerFine=0x22, Unknown=0x00 };
      ///@brief Number of events to be processed in this job (default -1 means all available on input)
      static int      nEvents    = -1;
      ///@name Input data type indicators
      ///@{
      ///@brief Input data type for this job (default is @c TopoCluster) 
      static Selector collection = TopoCluster;
      ///@brief Human-readable translation of input data type (non-modifiable)
      static const std::vector<std::string> names = { "TopoCluster", "TopoTower", "TopoTowerFine", "Unknown" };
      ///@brief List of all availbale types with same sequence (non-modifiable)
      static const std::vector<Selector>    types = { TopoCluster,   TopoTower,   TopoTowerFine,   Unknown   };
      ///@brief Returns @c true if actual type is @c TopoCluster
      static bool isTopoCluster()   { return (collection & TopoCluster)   == TopoCluster; }
      ///@brief Returns @c true if actual type is @c TopoTopwer (@f$ \Delta\eta\times\Delta\phi = 0.1\times 0.1) @f$)
      static bool isTopoTower()     { return (collection & TopoTower)     == TopoTower; }
      ///@brief Returns @c true if actual type is @c TopoTopwer (@f$ \Delta\eta\times\Delta\phi = 0.05\times 0.05) @f$)
      static bool isTopoTowerFine() { return (collection & TopoTowerFine) == TopoTowerFine; }
      ///@brief Lookup of human-readable input data type names (type->name)
      static std::map<Selector,std::string> selectorNameLookup = std::map<Selector,std::string>();
      ///@brief Inverse lookup of input data type (name->type)
      static std::map<std::string,Selector> selectorLookup     = std::map<std::string,Selector>();
      ///@brief Function filling lookup and reverse lookup
      static void fillSelectorMaps() {
 	for ( size_t i(0); i<names.size(); ++i ) {
	  selectorNameLookup[types.at(i)] = names.at(i);
	  selectorLookup[names.at(i)]     = types.at(i);
	}
      }
      ///@brief Function accessing input data type name for a given type tag
      /// 
      /// This is the user interface to the lookup tables. @warning Do not use the table directly!
      static const std::string& selectorName(Selector stype) {
	if ( selectorNameLookup.empty() ) { fillSelectorMaps(); }
	auto fnd = selectorNameLookup.find(stype);
	return fnd != selectorNameLookup.end() ? fnd->second : selectorNameLookup[Unknown];
      }
      ///@brief Function accessing input data type tag for a given type name
      /// 
      /// This is the user interface to the lookup tables. @warning Do not use the table directly!
      static Selector selectorType(const std::string& sname ) {
	if ( selectorLookup.empty() ) { fillSelectorMaps(); }
	size_t fp(sname.find_first_not_of(" "));
	size_t lp(sname.find_last_not_of(" "));
	std::string skey = lp >= fp ? sname.substr(fp,lp-fp+1) : names.back() ;
	auto fnd = selectorLookup.find(skey);
	return fnd != selectorLookup.end() ? fnd->second : selectorLookup[names.back()];
      }
      ///@}
      ///@name Input data specifications
      ///@{
      ///@brief Data directory
      ///
      /// The default expects a directory named @c data in the current working directory  
      static std::string dataDir = "./data";
      ///@brief File name mask
      ///
      /// This can be a format string to support numbered file names, e.g. @c data_000.root could use a 
      /// format string like @c "data_%03i.root". The syntax follows the @c printf (C-style) formatting
      /// directives.
      static std::string fileMask = "";
      /// @brief Number of files on input
      ///
      /// Can be used to restrict the number of files to be read on input. It is ineffective if @c nEvents > 0, in which 
      /// case the specified maximum number of events is read no matter what the number of files is (up to all available files
      /// if needed). It is effective together with @c nEvents = -1. The default value (-1) means all files.
      static int         nFiles   = -1;
      ///@}
    }
    ///@brief Output file specifications
    namespace Output {
      ///@brief Output file name
      /// 
      /// Default is "jetanalysis.root".
      static std::string file = "jetanalysis.root";
      ///@brief Tuple writer
      namespace Tuple {
	static bool write = false;                                     ///< Write tuple if @c true
	static std::string fileName = "output";                        ///< Core output file name for tuple (no extension!)
	static std::string treeName = "Jets";                          ///< Tree name 
	static int  evtsPerFile     = std::numeric_limits<int>::max(); ///< Events per file, default is -1 (all selected events)
      }
    }
    ///@name Jet configurations
    ///@{
    ///@brief General jet finder configuration
    namespace JetFinder {
#include "AnalysisData_jetcfg.icc"
    }
    ///@brief Reconstructed (detector) jet
    namespace RecoJet {
#include "AnalysisData_jetcfg.icc"
    }
    ///@brief Truth (generated particle level) jet
    namespace TruthJet {
#include "AnalysisData_jetcfg.icc"
    }
    ///@}
    ///@brief Configuration for @f$ \rho(\eta) @f$ measurement
    namespace RhoProfile {
      static size_t nBins  = 99;           ///< Number of rapdity bins
      static double rapMin = -4.95;        ///< Lower boundary of rapidity range
      static double rapMax =  4.95;        ///< Upper boundary of rapidity range
      static double rapCtrlMin = -2.0;     ///< Lower boundary of central rapidity region
      static double rapCtrlMax =  2.0;     ///< Upper boundary of central rapidity region
      static double window = 0.8;          ///< Width of rapidity window
      static bool   storeObjects = false;  ///< Store invidual signal object pointers in window if @c true; else only store @f$ \rho @f$ value
    }
    ///@{
    ///@brief Filter turn on/off
    namespace Filter {
      ///@brief Calorimeter signal filtering (detector/reconstruction quality focused)
      namespace CaloSignal { 
	static bool doTimeFilter   = false;  ///< Apply timing cut to calorimeter signal
	static bool doSignificance = false;  ///< Apply calorimeter significance selection
      }    
      ///@brief Jet signal filtering 
      namespace RecoJet {
	static bool doTimeFilter   = false;  ///< Apply timing cut to calorimeter signal
      }
      ///@brief Turn on pileup suppression techniques
      namespace Pileup {
	///@brief Identifier for technique
	enum TechTag { JetAreaBased = 0x0001, ConstituentSubtraction = 0x0101, SoftDrop = 0x0102, VoronoiAreaBased = 0x0104, None = 0x0000, Unknown = 0x0000 };
	///@brief Identifier list for technique
	static const std::vector<TechTag>     techTag  = { JetAreaBased,   ConstituentSubtraction,   SoftDrop,   VoronoiAreaBased,   None,   Unknown   };
	///@brief Name of technique
	static const std::vector<std::string> techName = { "JetAreaBased", "ConstituentSubtraction", "SoftDrop", "VoronoiAreaBased", "None", "Unknown" };
	///@brief Lookup tables for tag-to-name conversion
	static const std::map<TechTag,std::string>& getNameLookup() {
	  static std::map<TechTag,std::string> _lookup = std::map<TechTag,std::string>();
	  if ( _lookup.empty() ) { 
	    for  ( size_t i(0); i<techTag.size(); ++i ) { _lookup[techTag.at(i)] = techName.at(i); } 
	  }
	  return _lookup;
	}
	///@brief Lookup tables for name-to-tag conversion
	static const std::map<std::string,TechTag>& getTagLookup() {
	  static std::map<std::string,TechTag> _lookup = std::map<std::string,TechTag>();
	  if ( _lookup.empty() ) { 
	    for  ( size_t i(0); i<techName.size(); ++i ) { _lookup[techName.at(i)] = techTag.at(i); } 
	  }
	  return _lookup;
	}
	///@brief Converter from enumerator to human-readable string 
	static const std::string& methodName(TechTag tag) {
	  auto flu = getNameLookup().find(tag);
	  return flu == getNameLookup().end() ? techName.back() : flu->second; 
	}
	///@brief Converter from  human-readable string to enumerator
	static TechTag methodTag(const std::string& name) {
	  auto flu = getTagLookup().find(name);
	  return flu == getTagLookup().end() ? techTag.back() : flu->second; 
	}
	///@brief Calorimeter signal 
	namespace CaloSignal { 
	  static bool doConstituentSubtraction = false; ///< Apply constituent subtraction <a href="https://link.springer.com/article/10.1007%2FJHEP06%282014%29092">JHEP 06 (2014) 092</a>
	  static bool doSoftKiller             = false; ///< Apply SoftKiller <a href="https://link.springer.com/article/10.1140%2Fepjc%2Fs10052-015-3267-2">Eur.Phys.J. C 75:59 (2015)</a>
	  ///@brief SoftKiller configuration
	  namespace SoftKiller {
	    static double gridSize        = 0.6;        ///< Grid size 
	    static bool   returnThreshold = false;      ///< Return applied @f p_{\rm T} @f$ threshold if @c true 
	  }
	}
	///@brief Reconstructed jet
	namespace RecoJet { 
	  static bool doJetArea                = false; ///< Jet-area-based pile-up suppression
	}
      }
    } 
    ///@}
  }
  ///@brief @c FastJet related data structures
  ///
  /// Many details can be understood when consulting the FastJet manual at <a href="fastjet.fr">fastjet.fr</a>.
  /// In particular the <a href="http://fastjet.fr/repo/doxygen-3.3.1/">FastJet doxygen pages</a> are very
  /// helpful.
  namespace FastJet {
    ///@brief Attachment of general user information to @c fastjet::PseudoJet
    ///
    /// This object provides a store for an index which in previous implementations was intended 
    /// to reference the index of the data in the underlying data structure (e.g. a @c ROOT tuple).
    /// This index can in general not be relied on since the "ghosted" status is now available. A
    /// ghosted @c fastjet::PseudoJet can have a valid reference index, but will return a numerical
    /// value < 0 when using the @c index() function. Therefore this function should not be used
    /// if possible. Rather, the index to the underlyging data structure is now available directly
    /// using the @c fastjet::PseudoJet::user_index() feature.
    ///
    ///@todo Fix the index/status handling to a more obvious behaviour. 
    class UserInfo : virtual public fastjet::PseudoJet::UserInfoBase
    {
    private:
      ///@name Status indicators
      ///@{
      static const int _idEmpty  = -1; ///< Info object is empty
      static const int _idBroken = -2; ///< Info object is broken/invalid
      static const int _idOk     =  0; ///< Info object is ok
      static const int _idGhost  = -3; ///< Info object indicates that @c fastjet::PseudoJet is ghosted
      ///@}
    protected: 
      ///@brief Indicator of information status
      int _idx;
      ///@brief Underlying data index
      int _dataIdx;
      ///@brief Tagged as signal
      bool _isSignal;
      ///@brief Tagged as matched
      bool _isMatched;
    public:
      ///@brief Default constructor 
      ///
      /// Instantiates an empty @c UserInfo object
      UserInfo()        : UserInfo(_idEmpty) { }
      ///@brief Loaded constructor
      ///
      /// Instantiates a @c UserInfo object with a reference index. There is no check on the validity of this index.
      ///@param idx reference index 
      UserInfo(int idx) : _idx(idx), _dataIdx(-1), _isSignal(false), _isMatched(false) { }
      ///@brief Copy constructor
      UserInfo(const UserInfo& ui) 
	: _idx(ui._idx), _dataIdx(ui._dataIdx), _isSignal(ui._isSignal), _isMatched(ui._isMatched)
      { }
      ///@brief Destructor
      virtual ~UserInfo() { }
      ///@brief Clone object
      virtual UserInfo* clone() const { return new UserInfo(*this); }

      ///@name Set object status
      ///@{
      ///@brief Set status to "empty" (no reference data)
      virtual void setEmpty()  { _idx = _idEmpty;  }
      ///@brief Set status to "faulty" (broken/invalid)
      virtual void setFaulty() { _idx = _idBroken; }
      ///@brief Set status to "ghosted"
      virtual void setGhost()  { _idx = _idGhost;  }
      ///@brief Set signal tag
      virtual void setSignal() { _isSignal = true; }
      ///@brief Set pile-up tag
      virtual void setPileup() { _isSignal = false; }
      ///@brief Set matched
      virtual void setMatched() { _isMatched = true; }
      ///@brief Set matching tag
      virtual void setMatched(bool tag) { _isMatched = tag; }
      ///@brief Set raw data index
      virtual void setDataIndex(int idx) { _dataIdx = idx; }
      ///@}

      ///@name Access object status and data
      ///@{
      ///@brief  Retrieve reference index (or status indicator)
      ///
      ///@return Reference index if >= 0 (may still be invalid - no checking); if <0 returns status. 
      virtual int  index()    const { return _idx; }
      virtual int  dataIndex() const { return _dataIdx; }
      ///@brief  Check if empty
      ///
      ///@return @c true if empty.
      virtual bool isEmpty()  const { return _idx == _idEmpty; }
      ///@brief Check if faulty/broken/invalid
      ///
      ///@return @c true if faulty.
      virtual bool isFaulty() const { return _idx == _idBroken; }
      ///@brief Check if usable
      ///
      ///@return @c true if usable - no indication that the index is valid!
      virtual bool isUsable() const { return _idx >= 0; } 
      ///@brief Check if ghost
      ///
      ///@return @c true if associated @c fastjet::PseudoJet is ghosted
      virtual bool isGhost()  const { return _idx == _idGhost; }
      ///@brief Check if tagged as signal
      ///
      ///@return @c true if tagged as signal
      virtual bool isSignal() const { return _isSignal; }
      ///@brief Check if tagged as pile-up
      ///
      ///@return @c true if tagged as pile-up
      virtual bool isPileup() const { return !_isSignal; }
      ///@brief Check if jet is matched
      ///
      ///@return @c true if matched
      virtual bool isMatched() const { return _isMatched; }
      ///@}

      static bool hasUserInfo(const fastjet::PseudoJet& pj) 
      { return pj.has_user_info<UserInfo>(); } 
      static int  userIndex(const fastjet::PseudoJet& pj)
      { return hasUserInfo(pj) ? pj.user_info<UserInfo>().index() : _idBroken; } 
      static int  dataIndex(const fastjet::PseudoJet& pj)
      { return hasUserInfo(pj) ? pj.user_info<UserInfo>().dataIndex() : -1; }

      static bool isEmpty(const fastjet::PseudoJet& pj) 
      { return userIndex(pj) == _idEmpty; }
      static bool isUsable(const fastjet::PseudoJet& pj)
      { return userIndex(pj) >= _idOk; }
      static bool isGhost(const fastjet::PseudoJet& pj )
      { return userIndex(pj) == _idGhost; }
      static bool isPileup(const fastjet::PseudoJet& pj)
      { return hasUserInfo(pj) && pj.user_info<UserInfo>().isPileup(); }
      static bool isSignal(const fastjet::PseudoJet& pj)
      { return hasUserInfo(pj) && pj.user_info<UserInfo>().isSignal(); }
      static bool isMatched(const fastjet::PseudoJet& pj)
      { return hasUserInfo(pj) && pj.user_info<UserInfo>().isMatched(); }

    };
    // object holding calorimeter signal info
    class CaloSignalInfo : public UserInfo
    {
    public:
      enum Type { TopoCluster = 0x01, TopoTower=0x02, TopoTowerNorm = 0x02, TopoTowerCoarse = 0x02, TopoTowerFine = 0x04, Unknown = 0x00 };
    protected: 
      static std::map<Type,std::string> _mapTypeName;
      static std::map<std::string,Type> _mapNameType;
      static std::map<Type,double>      _mapFixedArea;
      // handle conversion maps
      static bool _fillMaps() {
	_mapTypeName[TopoCluster]   = "TopoCluster";   _mapNameType[_mapTypeName[TopoCluster]]   = TopoCluster;    _mapFixedArea[TopoCluster]   = _defArea;
	_mapTypeName[TopoTower]     = "TopoTower";     _mapNameType[_mapTypeName[TopoTower]]     = TopoTower;      _mapFixedArea[TopoTower]     = 0.1*0.1;
	_mapTypeName[TopoTowerFine] = "TopoTowerFine"; _mapNameType[_mapTypeName[TopoTowerFine]] = TopoTowerFine;  _mapFixedArea[TopoTowerFine] = 0.05*0.05;
	_mapTypeName[Unknown]       = "Unknown";       _mapNameType[_mapTypeName[Unknown]]       = Unknown;        _mapFixedArea[Unknown]       = _defArea;
	return !_mapTypeName.empty() && !_mapNameType.empty();
      }
      static Type _getType(const std::string& name) { 
	if ( _mapNameType.empty() ) { _fillMaps(); }
	auto fmap = _mapNameType.find(name);
	return fmap != _mapNameType.end() ? fmap->second : Unknown;
      }
      static const std::string& _getName(Type stype) { 
	if ( _mapTypeName.empty() ) { _fillMaps(); }
	auto fmap = _mapTypeName.find(stype); 
	return fmap != _mapTypeName.end() ? fmap->second : _mapTypeName[Unknown];
      }
      static double _getFixedArea(Type stype) {
	if ( _mapFixedArea.empty() ) { _fillMaps(); }
	auto fmap = _mapFixedArea.find(stype);
	return fmap != _mapFixedArea.end() ? fmap->second : _mapFixedArea[Unknown];
      }
      //
      static double _defTime;
      static double _defArea;
      // private data members
      Type        _typeId;
      std::string _typeName;
      double      _area;
      double      _PTD;
      double      _EPOS;
      double      _time;
      double      _origE;
    public:
      CaloSignalInfo(int idx,Type stype)       
	: UserInfo(idx)
	, _typeId(stype)
	, _typeName(_getName(stype))
	, _area(_getFixedArea(stype))
	, _PTD(0.)
	, _EPOS(0.)
	, _time(_defTime)
	, _origE(0.)
      { }
      CaloSignalInfo() 
	: UserInfo()
	, _typeId(Unknown)
	, _typeName(_getName(Unknown))
	, _area(_defArea)
	, _PTD(0.)
	,_EPOS(0.)
	, _time(_defTime)
	, _origE(0.)
      { }
      CaloSignalInfo(const CaloSignalInfo& ci) 
	: UserInfo(ci)
	, _typeId(ci._typeId)
	, _typeName(ci._typeName)
	, _area(ci._area)
	, _PTD(ci._PTD)
	, _EPOS(ci._EPOS)
	, _time(ci._time)
	, _origE(ci._origE)
      { }
      CaloSignalInfo(const CaloSignalInfo* ci) : CaloSignalInfo(*ci) { }
      virtual ~CaloSignalInfo() { }

      virtual UserInfo* clone() const { return new CaloSignalInfo(*this); } 

      Type               type() const                            { return _typeId;   }
      Type               type(const std::string& typeName) const { return _getType(typeName); } 
      const std::string& typeName() const                        { return _typeName; } 

      void setArea(double area)                     { _area = area; }
      void setPTD(double  ptd)                      { _PTD  = ptd;  }
      void setEPOS(double epos)                     { _EPOS = epos; }
      void setTime(double time)                     { _time = time; }
      void setType(Type typeId)                     { _typeId = typeId;     _typeName = _getName(_typeId); }  
      void setTypeName(const std::string& typeName) { _typeName = typeName; _typeId = _getType(_typeName); } 
      void setOrigE(double e)                       { _origE = e; }

      double             fixedArea()                         const { return _mapFixedArea[_typeId]; }
      double             area()                              const { return _typeId == TopoCluster ? _area : _typeId == Unknown ? 0. : fixedArea(); }
      double             PTD()                               const { return _PTD;  } 
      double             EPOS()                              const { return _EPOS; }
      double             time()                              const { return _time; }
      static double      defTime()                                 { return _defTime; }
      static double      defArea()                                 { return _defArea; }
      static bool        timeOk(double t)                          { return t > _defTime; }
      double             origE()                             const { return _origE; }

      static bool isCaloSignal(const fastjet::PseudoJet& pj)             { return pj.has_user_info<CaloSignalInfo>(); } 
      static bool hasCaloArea(const fastjet::PseudoJet& pj)              { return isCaloSignal(pj) && pj.user_info<CaloSignalInfo>().area() > 0.; }
      static double caloArea(const fastjet::PseudoJet& pj)               { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().area() : _defArea; }
      static double caloFixedArea(const fastjet::PseudoJet& pj)          { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().fixedArea() : 0.; }
      static double caloPTD(const fastjet::PseudoJet& pj)                { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().PTD() : 0. ; }  
      static double caloEPOS(const fastjet::PseudoJet& pj)               { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().EPOS() : 0. ; }  
      static const std::string& signalName(const fastjet::PseudoJet& pj) { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().typeName() : _getName(Unknown); }  
      static Type signalType(const fastjet::PseudoJet& pj)               { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().type() : Unknown; }  
      static double caloTime(const fastjet::PseudoJet& pj )              { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().time() : _defTime; }
      static double origE(const fastjet::PseudoJet& pj )                 { return isCaloSignal(pj) ? pj.user_info<CaloSignalInfo>().origE() : 0.; } 
      
    protected:
    };
    // object holding truth jet info
    class TruthJetInfo : public UserInfo
    {
    protected:
      size_t _nConstits;
    public:
      TruthJetInfo()                           : UserInfo(),       _nConstits(0)                 { }
      TruthJetInfo(size_t nConstits)           : UserInfo(),       _nConstits(nConstits)         { }
      TruthJetInfo(const TruthJetInfo& tjinfo) : UserInfo(tjinfo), _nConstits(tjinfo._nConstits) { }
      virtual ~TruthJetInfo()                                                                    { }
      virtual UserInfo* clone()     const { return new TruthJetInfo(*this); }
      size_t numberOfConstituents() const { return _nConstits; }
      size_t size()                 const { return _nConstits; }              
    };
    // object holding truth particle info
    class TruthInfo : public UserInfo
    {
    private:
      static const std::vector<int> _leptons;
      static const std::vector<int> _neutrinos;
    protected:
      int    _pdg;
      int    _status; 
      double _charge; 
      bool   _isHadron;
      bool   _isLepton;
      bool   _isNeutrino;
    public:
      TruthInfo() : 
	UserInfo()
	, _pdg(0) 
	, _status(-1) 
	, _charge(0.)
	, _isLepton(false) 
	, _isHadron(false) 
	, _isNeutrino(false)
      { }
      TruthInfo(int idx,int pdg,int status,double charge) 
	: UserInfo(idx)
	, _pdg(pdg) 
	, _status(status) 
	, _charge(charge) 
	, _isLepton(false) 
	, _isHadron(false) 
	, _isNeutrino(false) 
      { 
	if ( std::find(_leptons.begin(),_leptons.end(),pdg) != _leptons.end() ) {
	  _isLepton = true; 
	} else if ( std::find(_neutrinos.begin(),_neutrinos.end(),pdg) != _neutrinos.end() ) {
	  _isNeutrino = true; 
	}
	_isHadron = !_isLepton && !_isNeutrino;
      }
      TruthInfo(const TruthInfo& ti) 
	: UserInfo(ti)
	, _pdg(ti._pdg)
	, _status(ti._status)
	, _charge(ti._charge)
	, _isLepton(ti._isLepton)
	, _isHadron(ti._isHadron)
	, _isNeutrino(ti._isNeutrino)
      { }
      virtual ~TruthInfo() { }
      virtual UserInfo* clone() const { return new TruthInfo(*this); }
      int    pdg()          const { return _pdg; }
      int    status()       const { return _status; }
      double charge()       const { return _charge; }
      bool   isLepton()     const { return _isLepton; }
      bool   isHadron()     const { return _isHadron; }
      bool   isNeutrino()   const { return _isNeutrino; }
      bool   isCharged()    const { return _charge != 0.; }
      bool   isNeutral()    const { return !isCharged(); }
      // checks if PseudoJet has TruthInfo attached
      static bool isTruthParticle(const fastjet::PseudoJet& pj)
      { return pj.has_user_info<TruthInfo>(); } 
      // get particle features
      static int pdg(const fastjet::PseudoJet& pj)
      { return isTruthParticle(pj) ? pj.user_info<TruthInfo>().pdg() : 0; }
      static int status(const fastjet::PseudoJet& pj)
      { return isTruthParticle(pj) ? pj.user_info<TruthInfo>().status() : -1; }
      static double charge(const fastjet::PseudoJet& pj)
      { return isTruthParticle(pj) ? pj.user_info<TruthInfo>().charge() : 0.; }
      static bool isHadron(const fastjet::PseudoJet& pj)
      { return isTruthParticle(pj) && pj.user_info<TruthInfo>().isHadron(); }
      static bool isLepton(const fastjet::PseudoJet& pj )
      {	return isTruthParticle(pj) && pj.user_info<TruthInfo>().isLepton(); }
      static bool isNeutrino(const fastjet::PseudoJet& pj)
      { return isTruthParticle(pj) && pj.user_info<TruthInfo>().isNeutrino(); }
      static bool isCharged(const fastjet::PseudoJet& pj )
      { return isTruthParticle(pj) && pj.user_info<TruthInfo>().isCharged(); }
      static bool isNeutral(const fastjet::PseudoJet& pj)
      { return isTruthParticle(pj) && pj.user_info<TruthInfo>().isNeutral(); }
    }; // TruthInfo
    // constituent area
    static double area(const fastjet::PseudoJet& pj)
    {
      // FastJet area always has priority
      if ( pj.has_area() ) { return pj.area(); }
      // check on CaloSignal (may be a tower with fixed area)
      if ( CaloSignalInfo::hasCaloArea(pj) ) { return CaloSignalInfo::caloArea(pj); }
      // 
      return 0.;
    }
  } // FastJet
  // Jet matching
  namespace Matching {
    // store a geometry relation between a truth jet and a calorimeter jet
    class JetRelation
    {
    public:
      typedef boost::tuples::tuple<size_t,size_t,double> match_t;
      JetRelation() : _matching(size_t(-1),size_t(-1),-1.) { }
      virtual ~JetRelation() { }
      void reset() { _matching = match_t(size_t(-1),size_t(-1),-1.); }
      void fill(size_t itruth,size_t icalo,double param) {
	_matching.get<0>() = itruth; _matching.get<1>() = icalo; _matching.get<2>() = param;
      }
      double param()                                                               const { return _matching.get<2>(); }
      double deltaR()                                                              const { return param();            }
      double deltaR(const fastjet::PseudoJet& tjet,const fastjet::PseudoJet& cjet) const { return tjet.delta_R(cjet); }
      double invM()                                                                const { return param();            }
      double invM(const fastjet::PseudoJet& tjet,const fastjet::PseudoJet& cjet)   const { return (tjet+cjet).m();    }
      const fastjet::PseudoJet& truthJet(const std::vector<fastjet::PseudoJet>& tjets) 
      { return _matching.get<0>() < tjets.size() ? tjets.at(_matching.get<0>()) : _refjet; }
      const fastjet::PseudoJet& caloJet(const std::vector<fastjet::PseudoJet>& cjets) 
      { return _matching.get<1>() < cjets.size() ? cjets.at(_matching.get<1>()) : _refjet; }
      size_t truthIndex()        const { return _matching.get<0>(); }
      size_t caloIndex()         const { return _matching.get<1>(); }
      size_t jetIndex(size_t k)  const { return k == 0 ? _matching.get<0>() : k == 1 ? _matching.get<1>() : size_t(-1); }
      bool   indexOk(size_t idx) const { return idx != size_t(-1);  }
      size_t invalidIndex()      const { return size_t(-1);         }
    protected:
      //                   truth  calo 
      //                   index  index  distance
      match_t _matching;
      static const fastjet::PseudoJet _refjet;
    };
    //
    typedef JetRelation          match_t;
    typedef std::vector<match_t> mcont_base_t;
    // container for all found relations
    class JetRelationContainer : public mcont_base_t
    {
    public:
      JetRelationContainer()                                 : mcont_base_t()       { }
      JetRelationContainer(size_t s)                         : mcont_base_t(s)      { }
      JetRelationContainer(size_t s,const JetRelation& jrel) : mcont_base_t(s,jrel) { }
      virtual ~JetRelationContainer() { }

      // fills all combinations (inclusive, one jet collection)
      virtual void fillMass(const std::vector<fastjet::PseudoJet>& pjvec,double minMinv=0.,double maxMinv= 14.*Analysis::Units::TeV) { 
	// 
	typedef boost::tuples::tuple<size_t,size_t> value_t;
	std::map<double,value_t> lookup;
	for ( size_t i(0); i<pjvec.size(); ++i ) { 
	  for ( size_t j(i+1); j<pjvec.size(); ++j ) { 
	    double e2(std::pow(pjvec.at(i).e()+pjvec.at(j).e(),2));
	    double p2(std::pow(pjvec.at(i).px()+pjvec.at(j).px(),2) + 
		      std::pow(pjvec.at(i).py()+pjvec.at(j).py(),2) + 
		      std::pow(pjvec.at(i).pz()+pjvec.at(j).pz(),2));
	    if ( e2 > p2 ) {
	      double m(std::sqrt(e2-p2)); lookup[m] = value_t(i,j);   
	    } // valid mass
	  } // loop on second jet
	} // loop on first jet
	// extract relations
	for ( auto fmap(lookup.begin()); fmap != lookup.end(); ++fmap ) { 
	  match_t jrel; jrel.fill(fmap->second.get<0>(),fmap->second.get<1>(),fmap->first); this->push_back(jrel);
	}
      }

      // fills all combinations (inclusive, two jet collections)
      virtual void fillMass(const std::vector<fastjet::PseudoJet>& pjvec0,const std::vector<fastjet::PseudoJet>& pjvec1,double minMinv=0.,double maxMinv= 14.*Analysis::Units::TeV) { 
	// 
	typedef boost::tuples::tuple<size_t,size_t> value_t;
	std::map<double,value_t> lookup;
	for ( size_t i(0); i<pjvec0.size(); ++i ) { 
	  for ( size_t j(0); j<pjvec1.size(); ++j ) { 
	    double e2(std::pow(pjvec0.at(i).e()+pjvec1.at(j).e(),2));
	    double p2(std::pow(pjvec0.at(i).px()+pjvec1.at(j).px(),2) + 
		      std::pow(pjvec0.at(i).py()+pjvec1.at(j).py(),2) + 
		      std::pow(pjvec0.at(i).pz()+pjvec1.at(j).pz(),2));
	    if ( e2 > p2 ) {
	      double m(std::sqrt(e2-p2)); lookup[m] = value_t(i,j);   
	    } // valid mass
	  } // loop on second jet
	} // loop on first jet
	// extract relations
	for ( auto fmap(lookup.begin()); fmap != lookup.end(); ++fmap ) { 
	  match_t jrel; jrel.fill(fmap->second.get<0>(),fmap->second.get<1>(),fmap->first); this->push_back(jrel);
	}
      }

      // fills geometrically matched jet pairs exclusively (each jet is only matched once, 1-to-1) or inclusive (all matches are returned, 1-to-many) 
      virtual void fillMatch(std::vector<fastjet::PseudoJet>& tjets,std::vector<fastjet::PseudoJet>& cjets,double minRadius=10.,bool excl=true) {
	typedef boost::tuples::tuple<size_t,size_t> value_t;
	std::map<double,value_t> lookup; 

	// get pairs of truth and calo jets (indices) ordered by delta R
	bool fexcl(true);
	std::vector<size_t> its; std::vector<size_t> ics; std::vector<double> drs;
	size_t itruth(0);
	while ( itruth < tjets.size() && fexcl ) {
	  size_t icalo(0);
	  while ( icalo < cjets.size() && fexcl ) {  
	    double dR(tjets.at(itruth).delta_R(cjets.at(icalo)));
	    if ( dR < minRadius ) { 
	      fexcl =  
		std::find(its.begin(),its.end(),itruth) == its.end() && // check truth jet index
		std::find(ics.begin(),ics.end(),icalo)  == ics.end();
	      if ( fexcl ) {
		its.push_back(itruth);  // truth jet index 
		ics.push_back(icalo);   // calo jet index
		drs.push_back(dR);      // dR
	      } // new entries -> still exclusive!
	    } // within matching radius
	    ++icalo;
	  } // loop on calo jets
	  ++itruth;
	} // loop on truth jets
	// fill relations map
	if ( fexcl ) { 
	  for ( size_t i(0); i<its.size(); ++i ) {
	    JetRelation jrel; jrel.fill(its.at(i),ics.at(i),drs.at(i));
	    this->push_back(jrel);
	    // set calo jet status
	    Analysis::FastJet::UserInfo* si = cjets.at(ics.at(i)).has_user_info<Analysis::FastJet::UserInfo>()
	      ? new Analysis::FastJet::UserInfo(cjets.at(ics.at(i)).user_info<Analysis::FastJet::UserInfo>())
	      : new Analysis::FastJet::UserInfo(ics.at(i));
	    si->setDataIndex(ics.at(i));
	    si->setSignal();
	    si->setMatched();
	    cjets[ics.at(i)].set_user_info(si);
	    // set truth jet status
	    Analysis::FastJet::TruthJetInfo* ti = tjets.at(its.at(i)).has_user_info<Analysis::FastJet::TruthJetInfo>()
	      ? new Analysis::FastJet::TruthJetInfo(tjets.at(its.at(i)).user_info<Analysis::FastJet::TruthJetInfo>())
	      : new Analysis::FastJet::TruthJetInfo();
	    ti->setDataIndex(its.at(i));
	    ti->setMatched();
	    ti->setSignal();
	    tjets[its.at(i)].set_user_info(ti);
	  } // loop on indices and deltaRs
	} // is exclusive
      } // fill method  
      //  historic
      virtual void fill(std::vector<fastjet::PseudoJet>& tjets,std::vector<fastjet::PseudoJet>& cjets,double minRadius=10.,bool excl=true) {
	return fillMatch(tjets,cjets,minRadius,excl);
      }
      // check if index pair is matched    
      virtual bool check(size_t itruth,size_t icalo) const { 
	auto flup = this->begin();
	while ( flup != this->end() && !( itruth == flup->truthIndex() && icalo == flup->caloIndex() ) ) { ++flup; }  
	return flup != this->end(); 
      }
      // check if index pair is matched and return matching parameter
      virtual bool check(size_t itruth,size_t icalo,double& param) const { 
	auto flup = this->begin();
	while ( flup != this->end() && !( itruth == flup->truthIndex() && icalo == flup->caloIndex() ) ) { ++flup; }  
	if ( flup == this->end() ) { return false; } 
	param = flup->param();
	return true;
      }
      // check if truth index is matched
      virtual bool checkTruth(size_t itruth) const {
	auto flup = this->begin();
	while ( flup != this->end() && itruth != flup->truthIndex() ) { ++flup; }
	return flup != this->end(); 
      } 
      // check if calo index is matched
      virtual bool checkCalo(size_t icalo) const { 
	auto flup = this->begin();
	while ( flup != this->end() && icalo != flup->caloIndex() ) { ++flup; }
	return flup != this->end(); 
      }
      // find calo index for truth index
      virtual size_t caloIndex(size_t itruth) const { 
	auto flup = this->begin();
	while ( flup != this->end() && itruth != flup->truthIndex() ) { ++flup; }
	return flup != this->end() ? flup->caloIndex() : size_t(-1);  
      }
      // find calo index and distance for truth index
      virtual size_t caloIndex(size_t itruth,double& deltaR) const { 
	auto flup = this->begin();
	while ( flup != this->end() && itruth != flup->truthIndex() ) { ++flup; }
	if ( flup == this->end() ) { return size_t(-1); }
	deltaR = flup->deltaR(); 
	return flup->caloIndex();
      }
      // find truth index for calo index
      virtual size_t truthIndex(size_t icalo) const { 
	auto flup = this->begin();
	while ( flup != this->end() && icalo != flup->caloIndex() ) { ++flup; }
	return flup != this->end() ? flup->truthIndex() : size_t(-1);  
      }
      // find truth index and distance for calo index  
      virtual size_t truthIndex(size_t icalo,double& deltaR) const { 
	auto flup = this->begin();
	while ( flup != this->end() && icalo != flup->caloIndex() ) { ++flup; }
	if ( flup == this->end() ) { return size_t(-1); }
	deltaR = flup->deltaR();  
	return flup->truthIndex();
      }
    }; // JetRelationContainer
  } // namespace Matching
} // Analysis
// int              Analysis::FastJet::UserInfo::_idEmpty    = -1;
// int              Analysis::FastJet::UserInfo::_idBroken   = -2;
std::map<Analysis::FastJet::CaloSignalInfo::Type,std::string> Analysis::FastJet::CaloSignalInfo::_mapTypeName = std::map<Analysis::FastJet::CaloSignalInfo::Type,std::string>();
std::map<std::string,Analysis::FastJet::CaloSignalInfo::Type> Analysis::FastJet::CaloSignalInfo::_mapNameType = std::map<std::string,Analysis::FastJet::CaloSignalInfo::Type>();
std::map<Analysis::FastJet::CaloSignalInfo::Type,double> Analysis::FastJet::CaloSignalInfo::_mapFixedArea     = std::map<Analysis::FastJet::CaloSignalInfo::Type,double>();
double Analysis::FastJet::CaloSignalInfo::_defTime = -999.;
double Analysis::FastJet::CaloSignalInfo::_defArea = -1.;
const std::vector<int> Analysis::FastJet::TruthInfo::_leptons   = { 11, -11, 13, -13, 15, -15, 17, -17 };
const std::vector<int> Analysis::FastJet::TruthInfo::_neutrinos = { 12, -12, 14, -14, 16, -16, 18, -18 };
const fastjet::PseudoJet Analysis::Matching::JetRelation::_refjet = fastjet::PseudoJet(0.,0.,0.,0.);
 
                                                                                                                          ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Event & Object selections, analysis parameters //                                                                                                                         ////////////////////////////////////////////////////

#endif
