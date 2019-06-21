
#include "SlidingWindow.h"

//#include "TowerJets/HistogramCfg.h"

#include "AnalysisUtils.h"

#include <cstdio>
#include <numeric>
#include <algorithm>
#include <cmath>

//ClassImp( SlidingWindow::Bin )
//ClassImp( SlidingWindow::Window )

size_t SlidingWindow::Window::m_invalidIndex = size_t(-1);

std::string SlidingWindow::Window::_named_algo_Window = "SlidingWindow::Window";

const std::string& SlidingWindow::Window::algName() { return _named_algo_Window; }

SlidingWindow::Window::Window()
  : SlidingWindow::Window("SlidingWindow",0,0.,0.,0.,false)
{ }

SlidingWindow::Window::Window(const std::string& pName,size_t nBins,double xMin,double xMax,double window,bool storeObjects)
  : m_nBins(nBins)
  , m_min(xMin)
  , m_max(xMax)
  , m_window(window)
  , m_binWidth(0.)
  , m_adjustBins(0)
  , m_adjustBinRange(0)
  , m_adjustMin(0.)
  , m_adjustMax(0.)
  , m_adjustBinWidth(0.)
  , m_firstCall(true)
  , m_isDead(true)
  , m_storeObjects(storeObjects)
  , m_updateProfile(true)
  , m_name(pName)
  , h_spreadProfile((TH1D*)0)
  , h_areaProfile((TH1D*)0)
  , h_medianProfile((TH1D*)0)
  , h_entriesProfile((TH1D*)0)
  , p_eventProfile((TProfile*)0)
{ }

bool SlidingWindow::Window::book(bool prn)
{
  static double      _twoPi = 4.*std::asin(1.);
  static std::string _mname = "book";

  std::string modName(moduleName(_mname));

  // already booked
  if ( !m_isDead ) { PRINT_WARNING( modName, "already booked, additional booking call ignored"); return false; }

  // invalid bin options
  if ( m_nBins == 0 || ( m_min >= m_max ) ) {
    PRINT_ERROR( modName, "Invalid bin specifications #bins/min/max = %i/%4.1f/%4.1f", (int)m_nBins,m_min,m_max );
    m_isDead = true;
    return false;
  }
  // invalid window size
  if ( m_window <= 0. ) { 
    PRINT_ERROR( modName, "Invalid window size %4.1f", m_window );
    m_isDead = true;
    return false;
  }

  // setup
  m_binWidth = (m_max-m_min)/((double)m_nBins); 
  // book event profile
  m_adjustBins = m_nBins % 2 == 0 ? m_nBins + 1 : m_nBins; ///////////////////////
  double fx(m_binWidth/2.);                                // Re-adjusting bins //
  double ctr((m_min+m_max)/2.);                            ///////////////////////  
  size_t ir((m_adjustBins-1)/2); 
  m_adjustMin      = (ctr-fx)-ir*m_binWidth;
  m_adjustMax      = (ctr+fx)+ir*m_binWidth;
  m_adjustBinWidth = (m_adjustMax-m_adjustMin)/((double)m_adjustBins); 
  std::string hname = "WindowProfile_"+m_name;
  p_eventProfile = new TProfile(hname.c_str(),hname.c_str(),m_adjustBins,m_adjustMin,m_adjustMax);
  // store area profile
  std::string hName = "WindowArea_"+m_name;
  h_areaProfile = new TH1D(hName.c_str(),hName.c_str(),m_adjustBins,m_adjustMin,m_adjustMax);
  // bin spread
  hName = "WindowSpread_"+m_name;
  h_spreadProfile = new TH1D(hName.c_str(),hName.c_str(),m_adjustBins,m_adjustMin,m_adjustMax);
  // median profile
  hName = "Median_"+m_name;
  h_medianProfile = new TH1D(hName.c_str(),hName.c_str(),m_adjustBins,m_adjustMin,m_adjustMax);
  // entries profile
  hName = "Entries_"+m_name;
  h_entriesProfile = new TH1D(hName.c_str(),hName.c_str(),m_adjustBins,m_adjustMin,m_adjustMax);
  
  // adjusted parameters and descriptors
  if (  p_eventProfile != 0 && h_areaProfile != 0 && h_spreadProfile != 0 && h_medianProfile != 0 && h_entriesProfile != 0 ) { m_isDead = false; }

  if ( m_isDead ) {
    PRINT_ERROR( modName, "Module dead due to missing histograms" ); 
    return false;
  }

  // calculate areas and binning
  double xm(m_adjustMin+m_adjustBinWidth/2.);
  for ( size_t i(0); i<m_adjustBins; ++i ) {
    double mn = std::max(xm-m_window/2.,m_adjustMin);
    double mx = std::min(xm+m_window/2.,m_adjustMax);
    double ar((mx-mn)*_twoPi);
    m_bins.push_back(Bin(xm,ar,mn,mx));
    xm += m_adjustBinWidth;
  }

  // range of window in bin index space (one-sided)
  m_adjustBinRange = (size_t)((m_window/2.)/m_adjustBinWidth);

  // plot areas and set up collector
  for ( auto bin : m_bins ) { h_areaProfile->Fill(bin.binCenter(),bin.binArea()); }
  // set up internal store
  this->setupCollector();
  // print configuration if requested
  if ( prn ) { 
    std::string useArea = m_useZeroSuppression ? "SignalArea (zero-area suppression)" : "CatchmentArea (zero-area inclusion)";
    PRINT_INFO( modName, "Specifications" );
    PRINT_INFO( modName, "Number of bins ........... %i",            (int)m_adjustBins-1 );
    PRINT_INFO( modName, "Range .................... [%5.2f,%5.2f]", m_adjustMin, m_adjustMax );
    PRINT_INFO( modName, "Bin width ................ %5.2f",         m_adjustBinWidth );
    PRINT_INFO( modName, "Window size .............. %5.2f",         m_window );
    PRINT_INFO( modName, "Window index range ....... +/-%i",         (int)m_adjustBinRange );
    PRINT_INFO( modName, "Rho calculation based on . \042%s\042",    useArea.c_str() );     
  }

  return true; 
}    


SlidingWindow::Window::~Window()
{ }

void SlidingWindow::Window::reset()
{
  static std::string _mname = "reset";

  std::string modName(moduleName(_mname));

  PRINT_DEBUG( modName, "Entering with flags and pointers:");

  if ( m_isDead ) {
    PRINT_DEBUG( modName, "isDead        [true]  -- exiting with error"); return;
  } else {
    PRINT_DEBUG( modName, "isDead        [false] -- continue"); 
  }
  //
  if ( m_firstCall ) { 
    PRINT_DEBUG( modName, "firstCall     [true]  -- exiting without error"); m_firstCall = false; return; 
  } else {
    PRINT_DEBUG( modName, "firstCall     [false] -- continue"); 
  }

  if ( p_eventProfile != 0 ) {
    for ( size_t i(0); i<m_collector.size(); ++i  ) {
      if ( i < m_bins.size() ) { 
	PRINT_DEBUG( modName, "eta bin %2i of %2i # of rho entries %i", (int)i+1, (int)m_bins.size(), (int)m_collector.at(i).size() );
	for ( auto rhomed : m_collector.at(i) ) { p_eventProfile->Fill(m_bins.at(i).binCenter(),rhomed); } 
      } else {
	PRINT_WARNING( modName, "cache confusion: collector bin index %zu > maximum eta bin index %zu", i, m_bins.size()-1 );
      } 
    }
  } else { /*if ( PRINT_CHECK( _named_algo_Window, this->name(), DEBUG ) ) { printf(" (invalid pointer!)"); }*/ }

  // 
  PRINT_DEBUG( modName, "medianProfile [%p]", (void*)h_medianProfile );
  if ( h_medianProfile != 0 && h_entriesProfile != 0 ) { 
    PRINT_DEBUG( modName, "entries %i", (int)h_medianProfile->GetEntries() ); 
    h_medianProfile->Reset();
    m_updateProfile = true; 
  } else {
    PRINT_WARNING( modName, "medianProfile has invalid pointer" );
  }


  // reset event summation
  PRINT_DEBUG( modName, "invoking collector reset..." );
  this->resetCollector();
}

bool SlidingWindow::Window::integrate(const fastjet::PseudoJet& pMom)
{
  // retrieve area
  double area = this->isComplete(pMom) ? pMom.area() : 0.; 
  return this->integrate(pMom,area);
}

bool SlidingWindow::Window::integrate(const fastjet::PseudoJet& pMom,double area)
{
  if ( m_isDead ) return false;
  // exclude outside boundaries 
  double eta(pMom.pseudorapidity());
  if ( eta < m_min || eta > m_max ) return false;
  // find index range
  double rhoLocal = area > 0. ? pMom.pt()/area : 0.;
  int idx(this->binIndex(eta));
  int ilo(std::max(idx-(int)m_adjustBinRange,0));
  int ihi(std::min(idx+m_adjustBinRange,m_adjustBins-1));
  // store object links in central bin only
  if ( m_storeObjects ) { m_objects[idx].push_back(pMom); }
  // store data in windows
  for ( int i(ilo); i<=ihi; ++i ) { m_collector[i].push_back(rhoLocal); m_areas[i].push_back(area); }
  return true;
}

void SlidingWindow::Window::setupCollector()
{
  static std::string _mname = "setupCollector";
  static size_t _reserve = 10000;

  std::string modName(moduleName(_mname));

  m_collector.resize(m_bins.size(),data_t());
  m_areas.resize(m_bins.size(),data_t());
  if ( !m_storeObjects ) { 
    for ( size_t i(0); i<m_bins.size(); ++i ) { m_collector.at(i).reserve(_reserve); m_areas.at(i).reserve(_reserve); }
    PRINT_INFO( modName, "Collector/area cache size %i/%i, reserved %i in %i bins", 
		    (int)m_collector.size(), (int)m_areas.size(), (int)_reserve, (int)m_bins.size() );
  } else {
    m_objects.resize(m_bins.size(),object_t());
    for ( size_t i(0); i<m_bins.size(); ++i ) { m_collector.at(i).reserve(_reserve); m_areas.at(i).reserve(_reserve); m_objects.at(i).reserve(_reserve); }
    PRINT_INFO( modName, "Collector/area/object cache size %i/%i/%i, reserved %i in %i bins", 
		    (int)m_collector.size(), (int)m_areas.size(), (int)m_objects.size(), (int)_reserve, (int)m_bins.size() );
  }
}

void SlidingWindow::Window::resetCollector()
{
  if ( m_isDead ) return; 
  if ( m_storeObjects ) { 
    for ( size_t i(0); i<m_bins.size(); ++i ) { m_collector[i].clear(); m_areas[i].clear(); m_objects[i].clear(); } 
  } else {
    for ( size_t i(0); i<m_bins.size(); ++i ) { m_collector[i].clear(); m_areas[i].clear(); } 
  }
}

size_t SlidingWindow::Window::binIndex(double value) const
{ return ( m_isDead || ( value < m_adjustMin || value >= m_adjustMax ) ) ? m_invalidIndex : (size_t)((value-m_adjustMin)/m_adjustBinWidth); }

bool SlidingWindow::Window::findAllBins(double value,std::vector<size_t>& indices) const
{
  if ( m_isDead ) return false;
  indices.clear();
  int idx(this->binIndex(value));
  if ( idx < 0 || idx >= (int)m_adjustBins ) { return false; }
  int ilo(std::max(idx-(int)m_adjustBinRange,0));
  int ihi(std::min(idx+m_adjustBinRange,m_adjustBins-1));
  for ( int i(ilo); i<=ihi; ++i ) { indices.push_back(i); } 
  return true; 
}

std::vector<double> SlidingWindow::Window::findMedians()
{
  // module is dead, no data
  if ( m_isDead ) return data_t();

  // cache for medians
  data_t medians(m_collector.size(),0.); 

  // check on median calculation mode
  if ( m_useZeroSuppression ) {
    // consider only occupied area in eta windows
    for ( size_t i(0); i<m_collector.size(); ++i ) { medians[i] = this->median(m_collector.at(i)); }
  } else {                          
    // consider occupied and void areas in eta windows
    std::vector<double> entries; entries.reserve(1000000);
    for ( size_t i(0); i<m_collector.size(); ++i ) { 
      // clean up
      entries.clear();
      // get number of void patches (estimate)
      int numVoids(this->getVoids(m_areas.at(i),m_bins.at(i)));
      // fill patch densities (= 0) if any
      if ( numVoids > 0 ) { entries.resize(numVoids,0.); }
      // fill densities in occupied patches
      for ( auto rho : m_collector.at(i) ) { entries.push_back(rho); }
      // collect median
      medians[i] = this->median(entries);
    }
    
    // fill distribution if needed
    if ( h_medianProfile->GetEntries() == 0 ) { 
      for ( auto bin : m_bins ) { 
	h_medianProfile->Fill(bin.binCenter(),medians.at(this->binIndex(bin.binCenter())));
      }
      m_updateProfile = false;  
    }
  }
  return medians;
}

double SlidingWindow::Window::median(data_t& values)                 //////////////////////
{                                                                    // Sorts input data //
  if ( !values.empty() ) {                                           //////////////////////
    if ( values.size() == 1 ) { return values.front(); }
    std::sort(values.begin(),values.end());
    size_t n(values.size());
    return n % 2 != 0 ? values.at((n-1)/2) : (values.at(n/2-1)+values.at(n/2))/2.;
  } else { return 0.; }
}

double SlidingWindow::Window::median(object_t& objects)              //////////////////////   
{                                                                    // Sorts input data //
  static _sortObjects _sorter;                                       //////////////////////                                                                   
  if ( !objects.empty() ) {                                          
    if ( objects.size() == 1 ) { return this->rhoFromJet(objects.front()); }
    std::sort(objects.begin(),objects.end(),_sorter);
    if ( this->rhoFromJet(objects.front()) >= this->rhoFromJet(objects.back()) ) { printf("SlidingWindow::Window:%s::median() WARNING BUG wrong sort order",m_name.c_str()); }
    size_t n(objects.size());
    if ( n % 2 != 0 ) { 
      return this->rhoFromJet(objects.at((n-1)/2)); 
    } else {
      return (this->rhoFromJet(objects.at((n-1)/2))+this->rhoFromJet(objects.at(n/2)))/2.;
    }
  } else { return 0.; }
}

int SlidingWindow::Window::getVoids(const data_t& areas,const bin_t& bin)
{
  int numVoids(0);
  if ( areas.size() > 0 ) {                  
    double occArea(std::accumulate(areas.begin(),areas.end(),0.)); 
    if ( occArea < bin.binArea() ) { 
      double aveArea(occArea/(static_cast<double>(areas.size())));
      numVoids = aveArea != 0. ? static_cast<int>(std::round((bin.binArea()-occArea)/aveArea)) : 0;
    }
  }
  return numVoids;
}

SlidingWindow::RhoProvider::RhoProvider(SlidingWindow::Window* rhoProf)
  : fastjet::BackgroundEstimatorBase()
  , _rhoProfile(rhoProf)
{ }

SlidingWindow::RhoProvider::~RhoProvider() 
{ }

double SlidingWindow::RhoProvider::rho() const {
  static double _ylow  = -2.; static double _yhigh =  2.;
  if ( _rhoProfile == 0 ) { return 0.; }
  // calculate central rho
  size_t il(_rhoProfile->binIndex(_ylow)); size_t ih(_rhoProfile->binIndex(_yhigh));
  if ( il < ih ) { 
    std::vector<double> mds = _rhoProfile->findMedians();
    if ( ih < mds.size() ) {  
      std::vector<double> sds; sds.reserve(mds.size());
      for ( size_t i(il); il<ih; ++il ) { sds.push_back(mds.at(i)); }
      return _rhoProfile->median(sds);
    }
  }
  return 0;
}

double SlidingWindow::RhoProvider::rho(const fastjet::PseudoJet& pj) 
{
  if ( _rhoProfile == 0 ) { return 0.; }
  size_t ij = _rhoProfile->binIndex(pj.rap());
  std::vector<double> rp = _rhoProfile->findMedians(); 
  return ij < rp.size() ? rp.at(ij) :  0.; 
}

    double SlidingWindow::RhoProvider::sigma() const { return 0.; }

double SlidingWindow::RhoProvider::sigma(const fastjet::PseudoJet& /*pj*/) { return 0.; }

bool SlidingWindow::RhoProvider::has_sigma()       { return false; }
bool SlidingWindow::RhoProvider::has_rho_m() const { return false; }

void SlidingWindow::RhoProvider::set_particles(const std::vector<fastjet::PseudoJet>& pjvec) {
  _rhoProfile->reset();
  for ( auto pj : pjvec ) { _rhoProfile->addObject(pj); }
}

std::string SlidingWindow::RhoProvider::description() const  {
  static std::string _msg = "background estimator implements rho(y) profile from sliding rapdity windows";
  return _msg; 
} 
