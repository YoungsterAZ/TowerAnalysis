
#include "AnalysisHelper.h"
#include "JetAnalysisGroup.h"

#include <TString.h>

JetAnalysisGroup::JetAnalysisGroup(const std::string& name)
  : Analysis::GroupBase(name)
  , h_njet((TH1D*)0)
  , h_ptjet((TH1D*)0)
  , h_rapjet((TH1D*)0)
  , d_pt_rapjet((TH2D*)0)
{ this->book(); }

JetAnalysisGroup::~JetAnalysisGroup() 
{ }

bool JetAnalysisGroup::isBooked() {
  return h_njet != 0; 
}

bool JetAnalysisGroup::book()
{
  static std::string _mname = "JetAnalsyisGroup::Book";

  if ( isBooked() ) { PRINT_WARNING( _mname, "histograms already booked" ); return false; }

  size_t nhists(0);

  int njBin(200); double njMin(-0.5); double njMax(njMin+njBin);
  h_njet = AH::book1D<TH1D>(histName("h_njet").c_str(), "Number of Jets", njBin,njMin,njMax,"N_{jet}");
  h_njet->GetYaxis()->SetTitle(axisTitle("Entries per %.0f count(s)",h_njet->GetBinWidth(1)).c_str());
  ++nhists;

  int ptBin(200); double ptMin(0.); double ptMax(2000.);
  h_ptjet = AH::book1D<TH1D>(histName("h_ptjet").c_str(),"Jet Transverse Momentum",ptBin,ptMin,ptMax,"p_{T}^{jet} [GeV]");
  h_ptjet->GetYaxis()->SetTitle(axisTitle("#partialN/#partialp_{T} [1/(%.1f GeV)]",h_ptjet->GetBinWidth(1)).c_str());
  ++nhists;

  int rapBin(101); double rapMin(-5.05); double rapMax(5.05);
  h_rapjet = AH::book1D<TH1D>(histName("h_rapjet").c_str(),"Jet Rapidity",rapBin,rapMin,rapMax,"Rapidity y");
  h_rapjet->GetYaxis()->SetTitle(axisTitle("#partialN/#partialy [1/(%.1f units of rapidity)]",h_rapjet->GetBinWidth(1)).c_str());
  ++nhists;

  d_pt_rapjet = AH::book2D<TH2D>(histName("d_pt_rapjet").c_str(),"Jet Transverse Momentum vs Rapidity",rapBin,rapMin,rapMax,ptBin,ptMin,ptMax,"Rapidity y","p_{T}^{jet} [GeV]");
  d_pt_rapjet->GetZaxis()->SetTitle(axisTitle("#partial^{2}N/(#partialy #partialp_{T} [1/(%.1f#times%.1f GeV#times unit rapidity)]",h_ptjet->GetBinWidth(1),h_rapjet->GetBinWidth(1)).c_str());
  ++nhists;

  int tbin(301); double tmin(-150.5); double tmax(150.5);
  h_tjet = AH::book1D<TH1D>(histName("h_tjet").c_str(),"Jet Timing",tbin,tmin,tmax,"t^{jet} [ns]");
  h_tjet->GetYaxis()->SetTitle(axisTitle("#partialN/#partialt [1/(%.1f ns)]",h_tjet->GetBinWidth(1)).c_str());
  ++nhists;

  h_tsig = AH::book1D<TH1D>(histName("h_tsig").c_str(),"Signal in Jet Timing",tbin,tmin,tmax,"t^{calo-signal} [ns]");
  h_tsig->GetYaxis()->SetTitle(axisTitle("#partialN/#partialt [1/(%.1f ns)]",h_tjet->GetBinWidth(1)).c_str());
  ++nhists;

  d_tjet_rapjet = AH::book2D<TH2D>(histName("d_tjet_rapjet").c_str(),"Jet Timing vs Rapidity",rapBin,rapMin,rapMax,tbin,tmin,tmax,"Rapidity y","t^{jet} [ns]");
  d_tsig_rapsig = AH::book2D<TH2D>(histName("d_tsig_rapsig").c_str(),"Signal in Jet Timing vs Rapidity",rapBin,rapMin,rapMax,tbin,tmin,tmax,"Rapidity y","t^{calo-signal} [ns]");
  ++nhists;
 
  PRINT_INFO( _mname, "group [%s] booked %zu histograms", name().c_str(), nhists );

  return nhists>0;
}

bool JetAnalysisGroup::fill(const std::vector<fastjet::PseudoJet>& pjets)
{
  static std::string _mname = "JetAnalysisGroup::Fill";

  double njets(static_cast<double>(pjets.size()));
  h_njet->Fill(njets);
  if ( njets == 0 ) { return false; }

  // checks if this is a calorimeter jet with timing info
  bool isCalo(pjets.front().has_valid_cs() && AH::FastJet::hasCaloSignalInfo(pjets.front().constituents().front()));
  if ( isCalo ) { PRINT_DEBUG( _mname, "found jets from calorimeter objects with timing info"); }

  if ( isCalo ) { 
    // loop calo jets
    for ( auto& pj : pjets ) { 
      double pt(pj.pt()/Analysis::Units::GeV);
      double rap(pj.rap());
      h_ptjet->Fill(pt);
      h_rapjet->Fill(rap);
      d_pt_rapjet->Fill(rap,pt);
      // fill signal timing 
      double jett(0.); double jete(0.); bool jetHasTime(false);
      PRINT_DEBUG(_mname, "jet area %7.3f",pj.area());
      AH::printCaloSignal(_mname,pj.constituents());
      for ( auto cj : pj.constituents() ) {
	double t(Analysis::FastJet::CaloSignalInfo::caloTime(cj));
	PRINT_DEBUG( _mname,"calo time is %f", t); 
	if ( t != 0. ) { 
	  jett += cj.e()*t; jete += cj.e(); // running sums for energy-weighted time average
	  h_tsig->Fill(t);
	  d_tsig_rapsig->Fill(cj.rap(),t);
	  if ( !jetHasTime) { jetHasTime = true; }
	}
      }
      // fill jet timing
      if ( jetHasTime ) { 
	jett /= jete;
	h_tjet->Fill(jett);
	d_tjet_rapjet->Fill(pj.rap(),jett);
      } // jet has time
    } // loop on calo jets
  } else { 
    // loop calo jets
    for ( auto pj : pjets ) { 
      double pt(pj.pt()/Analysis::Units::GeV);
      double rap(pj.rap());
      h_ptjet->Fill(pt);
      h_rapjet->Fill(rap);
      d_pt_rapjet->Fill(rap,pt);
    } // loop on calo jets
  }   
  return njets > 0;
}

