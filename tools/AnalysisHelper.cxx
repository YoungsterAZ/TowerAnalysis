
#include "AnalysisHelper.h"

bool AnalysisHelper::readCfg(const std::string& fname) 

void AnalysisHelper::printCaloSignal(const std::string& module,const std::vector<fastjet::PseudoJet>& pjVec)
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
