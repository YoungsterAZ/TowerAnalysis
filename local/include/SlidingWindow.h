// -*- c++ -*-
#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include <TH1D.h>
#include <TProfile.h>

#include <string>
#include <vector>
#include <cmath>
#include <cstdio>

#include "AnalysisCfg.h"
#include "AnalysisData.h"
#include "AnalysisUtils.h"

#include "fastjet/PseudoJet.hh"

#include "fastjet/tools/BackgroundEstimatorBase.hh"

#include "fastjet/contrib/ConstituentSubtractor.hh"

///@brief Sliding @f$ \eta @f$ window collector
namespace SlidingWindow
{
  ///@brief Bin descriptors
  class Bin
  {
  public:
    ///@brief Default constructor
    ///
    /// Constructs non-described (useless) bin
    Bin() : m_left(0.),m_right(0.),m_center(0.),m_area(0.) { }
    ///@brief Loaded constructor
    ///
    /// Constructs fully described bin
    ///
    ///@param center bin center
    ///@param area bin area
    ///@param left bin lower (left) edge
    ///@param right bin upper (right) edge 
    ///
    ///@note This bin description class allows for overlapping and/or asymmetric bins, 
    ///      meaning center, area, left and right edge can be independently defined with the 
    ///      only expectation that left < center < right for operations. 
    Bin(double center,double area,double left,double right) : m_left(left),m_right(right),m_center(center),m_area(area) { }
    ///@brief Base class destructor 
    virtual ~Bin() { }

    ///@name Bin description access
    ///@{
    virtual double binCenter() const { return m_center; } ///<@brief Return bin center
    virtual double binArea()   const { return m_area;   } ///<@brief Return bin area
    virtual double binLower()  const { return m_left;   } ///<@brief Return lower edge of bin
    virtual double binUpper()  const { return m_right;  } ///<@brief Return upper edge of bin
    ///@}

    ///@name Operations on and with bins
    ///@{
    ///@brief Check if value is in bin
    ///
    ///@return @c true if given value @f$ x \in [X_{\rm left},X_{\rm right}] @f$
    ///
    ///@param value value to be checked
    virtual bool   inBin(double value) const { return value >= binLower() && value < binUpper(); }
    ///@brief Define bin order 
    ///
    ///@return @c true if center of given (referenced) bin is smaller than center of this bin
    ///
    ///@param bin reference to non-modifiable bin descriptor 
    bool operator<(const Bin& bin) const     { return this->binCenter() < bin.binCenter(); }
    ///@}

    //    ClassDef( SlidingWindow::Bin, 1 );

  private:

    ///@name Bin description
    ///@{
    ///@brief Left (lower) bin edge
    double m_left;   //!
    ///@brief Right (upper) bin edge
    double m_right;  //!
    ///@brief Bin center
    double m_center; //!
    ///@brief Bin area
    double m_area;   //!
    ///@}
  };

  ///@brief Window collector
  ///
  /// Collects data and objects in (overlapping) windows. The windows 
  /// area characterized by a width and an area. The covered value space is
  /// defined by a minimum and maximum value, and the number of steps the windows 
  /// slide between these two values. Data and objects are stored in each window 
  /// they contribute to. 
  ///
  /// The window width is fixed across the full range, except when the window gets 
  /// close to the limits sliding range. At the correspondign edges the window width
  /// is reduced such that the window does not extend beyond the range delimiters.
  ///
  /// The collected data can be analyzed and a median value can be returned for each 
  /// window. As this collector is intended to be used to measure the median transverse momentum 
  /// density @f$ \rho @f$, which is calculated for each entry using the transverse momentum
  /// @f$ p_{\rm T} @f$ and an associated (user-provided) area in @f$ (\eta,\phi) @f$ space, @f$ A_{\eta\phi} @f$.
  /// After all signals are collected into the respective windows, uncovered (void) areas can exist for a window
  /// with area @f$ A_{\rm window} @f$ if @f$ \sum A_{\eta\phi} < A_{\rm window} @f$. This uncovered area
  /// is not considered part of the <i>catchment area</i> defined by the implemented area association strategy, and thus 
  /// can be associated with @f$ \rho = 0 @f$. Its size affects the median @f$ \rho @f$ in the window.
  ///
  ///@note In case of Voronoi areas, @f$ \sum A_{\eta\phi} \geq A_{\rm window} @f$ is expected, thus no void areas. 
  ///      This is partly a limitation of the present implementation, as it only allows positive (physical) signals to
  ///      contribute to the @f$ \rho @f$ measurement.    
  class Window
  {
  public:

    ///@name Data types
    ///@{
    typedef std::vector<double>             data_t;              ///<@brief Real number list type
    typedef std::vector<data_t>             collector_t;         ///<@brief Real number matrix type (2-dimensional)
    typedef std::vector<fastjet::PseudoJet> object_t;            ///<@brief Object list type
    typedef std::vector<object_t>           object_collector_t;  ///<@brief Object matrix type (2-dimensional)
    typedef Bin                             bin_t;               ///<@brief Bin descriptor type
    typedef std::vector<bin_t>              binning_t;           ///<@brief List of bin descriptors type
    ///@}

    ///@brief Default constructor
    Window();
    ///@brief Loaded constructor
    ///
    ///@param pName name of sliding window collector
    ///@param nBins number of bins (steps) for sliding window
    ///@param min   lower edge of sliding window range
    ///@param max   upper edge of sliding window range 
    Window(const std::string& pName,size_t nBins,double min,double max,double window,bool storeObjects=false);
    ///@brief Base class destructor 
    virtual ~Window();

    ///@name Configurations and states
    ///@{
    ///@brief Book sliding window collector
    ///
    ///@param prn controls printout; if @c true (default) details of the collector configuration are printed, else printing is suppressed.  
    bool book(bool prn=true);
    ///@brief Check if void areas contribute to median 
    ///
    ///@return @c true if void areas (areas not occupied by signals) are not included in median 
    bool useZeroSuppression() const;
    ///@brief Turn on/off void area contribution 
    ///
    ///@param turnOn if @c true (default) excludes void areas from median determination. 
    void setZeroSuppression(bool turnOn=true);
    ///@}

    ///@name Add data and objects
    ///@{
    ///@brief Add a list of objects to the collector
    ///
    /// The object is assumed to be a @c fastjet::PseudoJet with a valid area associated. 
    /// If no area is found, @f$ \rho = 0 @f$ is added to the data.
    ///
    ///@param mom reference to non-modifiable vector of @c fastjet::PseudoJet typed objects 
    bool add(const object_t& mom)
    {
      if ( m_isDead ) return false;
      // find central bin and distribute
      bool isOk(true);
      for ( auto pm : mom ) { isOk = isOk && this->integrate(pm); }
      return isOk;
    }
    ///@brief Add a single object with area to the collector
    ///
    /// The object can be of any type providing an interface @c OBJ::pt(). 
    ///
    ///@param obj  reference to non-modifiable object 
    ///@param area user-provided area measure (default is 0)
    template<class OBJ>
    bool addObject(const OBJ& obj,double area=0.)
    {
      if ( m_isDead ) return false;
      // find central bin and distribute
      fastjet::PseudoJet fj(fastjet::PtYPhiM(obj.pt(),obj.rap(),obj.phi(),obj.m()));
      return integrate(fj,area);
    }
    bool addObject(const fastjet::PseudoJet& fj,double area=0.)
    { return !m_isDead && integrate(fj,area); }
    ///@}

    ///@name Operations and data access
    ///@{
    virtual void   reset();                              ///<@brief Reset the collector
    virtual size_t binIndex(double value) const;         ///<@brief Return the window bin index for a given value
    virtual int    rootBinIndex(double value) const;     ///<@brief Return the window bin index for ROOT histograms for a given value
    virtual int    rootBinIndex(size_t index) const;     ///<@brief Convert the window bin index into the corresponding ROOT histogram bin index
    ///@brief Find all windows a value is contained in
    ///
    ///@return @c true if the value contributes to any described window, else @c false
    ///
    ///@param value value to be checked
    ///@param indices list of window indices the value contributes to (empty if value does not overlap with any window)
    bool findAllBins(double value,std::vector<size_t>& indices) const;
    const binning_t& binning() const { return m_bins; }  ///<@brief Return all bin descriptions
    double windowSize() const { return m_window; }       ///<@brief Return nominal window width (size - actual window in a given bin may be smaller) 
    double min() const { return m_min; }                 ///<@brief Lower value range limit for sliding windows
    double max() const { return m_max; }                 ///<@brief Upper value range limit for sliding windows
    double adjustedMin() const { return m_adjustMin; }   ///<@brief Adjusted lower value range limit for sliding window (after symmetrization)
    double adjustedMax() const { return m_adjustMax; }   ///<@brief Adjusted upper value range limit for sliding window (after symmetrization)
    TH1D*     areaProfile()    { return h_areaProfile; }
    TProfile* eventProfile()   { return p_eventProfile; }
    TH1D*     spreadProfile()  { return h_spreadProfile; }
    TH1D*     medianProfile()  { if ( m_updateProfile ) { findMedians(); } return h_medianProfile; }
    TH1D*     entriesProfile();

    std::vector<double> findMedians(); 

    const collector_t&        rhoCollection()     const { return m_collector; }
    const collector_t&        areaCollection()    const { return m_areas;     }
    std::vector<int>          entriesCollection() const;
    const object_collector_t& objectCollection()  const { return m_objects;   }

    const std::string& name() const { return m_name; }

    bool operator<(const Window& wdow)  const;
    bool operator>(const Window& wdow)  const;
    bool operator==(const Window& wdow) const;

    static const std::string& algName();
    ///@}

    static double median(data_t& values); 

    //    ClassDef( SlidingWindow::Window, 1 );

  protected:
    // stores
    binning_t          m_bins;      //!
    collector_t        m_collector; //!
    collector_t        m_areas;     //!
    object_collector_t m_objects;   //!

    // requested binning and window size
    size_t m_nBins;    //!
    double m_min;      //!
    double m_max;      //!
    double m_window;   //!
    double m_binWidth; //!
    // adjusted binning after centering on mean value in range
    size_t m_adjustBins;     //!
    size_t m_adjustBinRange; //!
    double m_adjustMin;      //!
    double m_adjustMax;      //!
    double m_adjustBinWidth; //!

    // process steering and identification
    bool m_firstCall;    //!
    bool m_isDead;       //!
    bool m_storeObjects; //!
    bool m_updateProfile; //!
    bool m_useZeroSuppression; //!
    std::string m_name;  //!
    static size_t m_invalidIndex;  //!
    std::string moduleName(const std::string& method);


    TH1D*     h_spreadProfile;  //!
    TH1D*     h_areaProfile;    //!
    TH1D*     h_medianProfile;  //!
    TH1D*     h_entriesProfile; //!
    TProfile* p_eventProfile;   //!

    static std::string _named_algo_Window;

    // helpers
    double area(int rootIdx) const;
    double center(int rootIdx) const;
    double lowEdge(int rootIdx) const;
    double upEdge(int rootIdx) const;
    const Bin& bin(int rootIdx) const;

    bool   integrate(const fastjet::PseudoJet& pMom);
    bool   integrate(const fastjet::PseudoJet& pMom,double area);
    void   setupCollector();
    void   resetCollector();
    //    double median(data_t& values);
    double median(object_t& jet);
    bool   isComplete(const fastjet::PseudoJet& jet) const;
    double rhoFromJet(const fastjet::PseudoJet& jet) const;

    int   getVoids(const data_t& areas,const bin_t& bin);

    struct _sortObjects 
    { 
      bool operator()(const fastjet::PseudoJet& p0,const fastjet::PseudoJet& p1) 
      { 
	if ( ( p0.has_valid_cs() && p0.area() > 0. ) && ( p1.has_valid_cs() && p1.area() > 0. ) ) { 
	  return p0.perp()/p0.area() < p1.perp()/p1.area(); 
	} else {
	  return false;
	}
      }
    }; // _sortObjects
  }; // SlidingWindow::Window

  class RhoProvider : public fastjet::BackgroundEstimatorBase
  {
  public:
    RhoProvider(Window* rhoProf);
    virtual ~RhoProvider();

    virtual double rho() const;
    virtual double sigma() const;
    virtual double rho(const fastjet::PseudoJet& pj); 
    virtual double sigma(const fastjet::PseudoJet& pj);
    virtual bool   has_sigma();
    virtual bool   has_rho_m() const;
    virtual std::string description() const;
    virtual void   set_particles(const std::vector<fastjet::PseudoJet>& pjvec);
    
    
  private:
    Window* _rhoProfile;
  };
}

inline bool SlidingWindow::Window::operator<(const Window& wdow)  const { return this->name() < wdow.name(); }
inline bool SlidingWindow::Window::operator>(const Window& wdow)  const { return this->name() > wdow.name(); }
inline bool SlidingWindow::Window::operator==(const Window& wdow) const { return this->name() == wdow.name(); }

inline double SlidingWindow::Window::area(int rootIdx) const
{ return rootIdx > 0 && rootIdx <= (int)m_bins.size() 
    ? m_bins.at(rootIdx-1).binArea() : 0.; }

inline double SlidingWindow::Window::center(int rootIdx) const
{ return rootIdx > 0 && rootIdx <= (int)m_bins.size() 
    ? m_bins.at(rootIdx-1).binCenter() : 0.; }

inline double SlidingWindow::Window::lowEdge(int rootIdx) const
{ return rootIdx > 0 && rootIdx <= (int)m_bins.size() 
    ? m_bins.at(rootIdx-1).binLower() : 0.; }

inline double SlidingWindow::Window::upEdge(int rootIdx) const
{ return rootIdx > 0 && rootIdx <= (int)m_bins.size() 
    ? m_bins.at(rootIdx-1).binUpper() : 0.; }

inline const SlidingWindow::Bin& SlidingWindow::Window::bin(int rootIdx) const
{ return rootIdx > 0 && rootIdx <= (int)m_bins.size() 
    ? m_bins.at(rootIdx-1) : *m_bins.end(); }

inline int SlidingWindow::Window::rootBinIndex(double value) const
{ return this->rootBinIndex(this->binIndex(value)); }

inline int SlidingWindow::Window::rootBinIndex(size_t index) const
{ return index != m_invalidIndex ? index+1 : -1; }


inline bool SlidingWindow::Window::isComplete(const fastjet::PseudoJet& jet) const
{ return jet.has_valid_cs() && jet.area() > 0.; } 
inline double SlidingWindow::Window::rhoFromJet(const fastjet::PseudoJet& jet) const
{ return this->isComplete(jet) ? jet.perp()/jet.area() : 0.; } 

inline bool SlidingWindow::Window::useZeroSuppression() const { return m_useZeroSuppression; }
inline void SlidingWindow::Window::setZeroSuppression(bool turnOn) { 
  static std::string _mname = "setZeroSuppression";
  std::string modName(moduleName(_mname));
  std::string useArea = turnOn ? "SignalArea (zero-area suppression)" : "CatchmentArea (zero-area inclusion)";
  PRINT_INFO( modName, "Rho calculation changed to \042%s\042", useArea.c_str() );     
  m_useZeroSuppression = turnOn; 
}

inline std::vector<int> SlidingWindow::Window::entriesCollection() const
{ 
  std::vector<int> entries(m_collector.size(),0);
  for ( size_t i(0); i<m_collector.size(); ++i ) { entries[i] = static_cast<int>(m_collector.at(i).size()); }
  return entries;
}

inline TH1D* SlidingWindow::Window::entriesProfile()
{
  int ibin(1);
  for ( auto entry : entriesCollection() ) { 
    double xe(static_cast<double>(entry)); 
    h_entriesProfile->SetBinContent(ibin,xe);
    h_entriesProfile->SetBinError(ibin++,std::sqrt(std::abs(xe)));
  }
  return h_entriesProfile;
}

inline std::string SlidingWindow::Window::moduleName(const std::string& method) {
  char buffer[1024];
  sprintf(buffer,"%s::%s(\042%s\042)",_named_algo_Window.c_str(),method.c_str(),name().c_str());
  return std::string(buffer);
} 
#endif
