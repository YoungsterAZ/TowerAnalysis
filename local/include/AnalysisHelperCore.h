// -*- c++ -*-
#ifndef ANALYSISHELPERCORE_H
#define ANALYSISHELPERCORE_H

#include "HistUtils.h"

#include <string>
#include <list>
#include <algorithm>

#include <TH1.h>
#include <TFile.h>

/// @brief Histogram utilities
namespace HistUtils {
  /// @brief Histogram manager
  ///
  /// This object manages histograms (presently collects them for write to output file). 
  /// Implemented as a singleton (one object instance/job only. Typically used in the background. 
  class HistMgr
  {
  private:
    /// @brief Pointer to only manager object
    static HistMgr*        _instance;
    /// @brief List of known histograms 
    static std::list<TH1*> _hlist;
    /// @brief Protected constructor for singleton pattern
    HistMgr() { }
  public:
    /// @brief Access to object
    static HistMgr* instance()               { if ( _instance == 0 ) { _instance = new HistMgr(); } return _instance; }
    /// @name Histogram cache management
    ///@{
    /// @brief Add a histogram to cache
    /// @param hptr base class pointer to histogram to be added.
    static void     add(TH1* hptr)           { if ( hptr != 0 ) { _hlist.push_back(hptr); } }
    /// @brief Remove a histogram from cache
    /// @param hptr base class pointer to histogram to be removed
    static void     remove(TH1* hptr)        { auto itr = std::find(_hlist.begin(),_hlist.end(),hptr); if ( itr != _hlist.end() ) { _hlist.erase(itr); } }
    /// @brief Clear cache
    static void     clear()                  { _hlist.clear(); }
    /// @brief Write known histograms
    ///
    /// @return Number of histograms written.
    /// @param noempty do not write empty histograms if @c true (optional, default @c true)
    static size_t   write(bool noempty=true) {  
      size_t nctr(0); for ( auto hptr : _hlist ) { if ( hptr != 0 ) { if ( !noempty || hptr->GetEntries()>0 ) { hptr->Write(); ++nctr; } } } return nctr; 
    }
    ///@}
  };
} // HistUtils

HistUtils::HistMgr* HistUtils::HistMgr::_instance = 0;          ///< Singleton pattern pre-set 
std::list<TH1*> HistUtils::HistMgr::_hlist = std::list<TH1*>(); ///< Histogram pointer cache

///@brief Analysis helper functions 
///
/// This is implemented as a @c struct to avoid warnings concerning unused functions in name spaces (to be fixed?)
struct AnalysisHelper {
  ///@ brief  Book 1d histogram (any ROOT type like TH1D, TH1F, TProfile, ...)
  ///
  /// @return pointer to histogram of user specified type.
  ///
  /// This function automatically registers the booked histogram with a 
  /// background histogram manager. This way the histogram is added to the output list and written out if @c AnalysisHelper::writeHists() is invoked.
  /// It also invokes the @c TH1::Sumw2() method for correct error treatment. 
  /// The template specifier should be the desired concrete histogram type.
  ///
  /// Example:
  ///
  ///@code
  /// // book histogram
  /// TH1D* h = AnalysisHelper::book1D<TH1D>("test","test",100,0.,1.);
  /// // fill histogram
  /// h->Fill(0.5);
  /// // write histogram to file "test.root"
  /// AnalysisHelper::writeHists("test.root");
  ///@endcode
  ///
  /// If axis titles should be saved with the histogram, do for titles on both axis ("entries" can be omitted to not add a title to the y axis).
  ///
  ///@code
  /// // book histogram 
  /// TH1D* h = AnalysisHelper::book1D<TH1D>("test","test",100,0.,1.,"x value","entries");
  ///@endcode
  ///
  ///@param  name histogram name (unique histogram identifier)
  ///@param  title  histogram title
  ///@param  nxbins number of bins on x axis
  ///@param  xmin   lower boundary of x value range
  ///@param  xmax   upper boundary of x value range
  ///@param  xtitle x axis title (optional, default is no title)
  ///@param  ytitle y axis title (optional, default is no title)
  template<class H>
  static H* book1D(const std::string& name,const std::string& title,int nxbins,double xmin,double xmax,const std::string xtitle="",const std::string& ytitle="")
  {
    H* h = new H(name.c_str(),title.c_str(),nxbins,xmin,xmax); h->Sumw2();
    if ( xtitle != "" ) { h->GetXaxis()->SetTitle(xtitle.c_str()); }
    if ( ytitle != "" ) { h->GetYaxis()->SetTitle(ytitle.c_str()); }
    (HistUtils::HistMgr::instance())->add(h); 
    return h;
  }

  ///@brief  Book 2d histogram (any ROOT type like TH2D, TH2F, TProfile2D, ...)
  ///@copydetails AnalysisHelper::book1D
  ///@param nybins number of bins on y axis
  ///@param ymin   lower boundary of y value range
  ///@param ymax   upper boundary of y value range
  ///@param ztitle z axis title (optional, default is no title)
  template<class H>
  static H* book2D(const std::string& name,const std::string& title,int nxbins,double xmin,double xmax,int nybins,double ymin,double ymax,
		   const std::string xtitle="",const std::string& ytitle="",const std::string& ztitle="")
  {
    H* h = new H(name.c_str(),title.c_str(),nxbins,xmin,xmax,nybins,ymin,ymax); h->Sumw2();
    if ( xtitle != "" ) { h->GetXaxis()->SetTitle(xtitle.c_str()); }
    if ( ytitle != "" ) { h->GetYaxis()->SetTitle(ytitle.c_str()); }
    (HistUtils::HistMgr::instance())->add(h); 
    return h;
  }

  ///@brief Write histograms to file
  ///
  ///@return Returns the number of histograms written to file. A return value of -1 indicates a file problem.
  ///
  ///@param fname   name of file to be written to (should have the ".root" extension)
  ///@param ftag    tag associated with file access (optional argument); default @c RECREATE - overwite existing file with same name, alternative is @c WRITE - create a new file 
  ///               for output which fails of a file with the same given name already exists
  ///@param noempty flag to control histogram output (optional argument); default @c true suppresses saving empty histograms to file; if @c false all histograms are written    
  static int writeHists(const std::string& fname,const std::string& ftag="RECREATE",bool noempty=true) 
  {
    TFile* f = new TFile(fname.c_str(),ftag.c_str());
    if ( f == 0 ) { printf("AnalysisHelper::writeHists(\042%s\042,\042%s\042) ERROR cannot create TFile object\n",fname.c_str(),ftag.c_str()); return -1; }
    int nh((HistUtils::HistMgr::instance())->write(noempty));
    f->Close();
    return nh;
  }
}; // AnalysisHelper

///@brief Alternative (short) access
///
/// The functions in @c AnalysisHelper can be accessed by 
///
///@code 
/// int nhist = AnalysisHelper::writeHists("test.root");
///@endcode
///
/// or
///
///@code
/// int nhist = AH::writeHists("test.root");
///@endcode 
///
/// This feature is introduced for backward compatibility in various analysis code environments.
typedef AnalysisHelper AH;

#endif
