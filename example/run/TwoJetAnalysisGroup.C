
#include "AnalysisHelper.h"
#include "TwoJetAnalysisGroup.h"

#include <TString.h>

#include <cmath>

TwoJetAnalysisGroup::TwoJetAnalysisGroup(const std::string& name)
  : Analysis::GroupBase(name)
  , h_ptjet_lead((TH1D*)0)
  , h_ptjet_sublead((TH1D*)0)
  , h_rapjet_lead((TH1D*)0) 
  , h_rapjet_sublead((TH1D*)0) 
  , d_pt_rapjet_lead((TH2D*)0)
  , d_pt_rapjet_sublead((TH2D*)0)
  , t_lead()
  , t_sublead()
{ this->book(); }

TwoJetAnalysisGroup::~TwoJetAnalysisGroup() 
{ }

bool TwoJetAnalysisGroup::isBooked() {
  return h_ptjet_lead != 0; 
}

bool TwoJetAnalysisGroup::book()
{
  static std::string _mname = "TwoJetAnalsyisGroup::Book";

  if ( isBooked() ) { PRINT_WARNING( _mname, "histograms already booked" ); return false; }

  size_t nhists(0);

  ///////////////////////////////////////////
  // Leading and subleading jet kinematics //
  ///////////////////////////////////////////

  int ptBin(200); double ptMin(0.); double ptMax(2000.);
  h_ptjet_lead = AH::book1D<TH1D>(histName("h_ptjet_lead"),"Leading jet p_{T}",ptBin,ptMin,ptMax,"p_{T}^{lead} [GeV]");
  h_ptjet_lead->GetYaxis()->SetTitle(axisTitle("#partialN/@partialp_{T}^{lead} [1/(%.1f GeV)]",h_ptjet_lead->GetBinWidth(1)).c_str());
  ++nhists;
  h_ptjet_sublead = AH::book1D<TH1D>(histName("h_ptjet_sublead"),"Sub-leading jet p_{T}",ptBin,ptMin,ptMax,"p_{T}^{sub-lead} [GeV]");
  h_ptjet_sublead->GetYaxis()->SetTitle(axisTitle("#partialN/@partialp_{T}^{sub-lead} [1/(%.1f GeV)]",h_ptjet_sublead->GetBinWidth(1)).c_str());
  ++nhists;

  int rapBin(101); double rapMin(-5.05); double rapMax(5.05);
  h_rapjet_lead = AH::book1D<TH1D>(histName("h_rapjet_lead"),"Leading jet rapidity",rapBin,rapMin,rapMax,"y_{jet}^{lead}");
  h_rapjet_lead->GetYaxis()->SetTitle(axisTitle("#partialN/#partialy_{jet}^{lead} [1/(%.1f units of rapidity)]",h_rapjet_lead->GetBinWidth(1)).c_str());
  ++nhists;
  h_rapjet_sublead = AH::book1D<TH1D>(histName("h_rapjet_sublead"),"Sub-leading jet rapidity",rapBin,rapMin,rapMax,"y_{jet}^{sub-lead}");
  h_rapjet_sublead->GetYaxis()->SetTitle(axisTitle("#partialN/#partialy_{jet}^{sub-lead} [1/(%.1f units of rapidity)]",h_rapjet_sublead->GetBinWidth(1)).c_str());
  ++nhists;

  d_pt_rapjet_lead = AH::book2D<TH2D>(histName("d_pt_rapjet_lead"),"Leading jet p_{T} versus rapidity",rapBin,rapMin,rapMax,ptBin,ptMin,ptMax,"y_{jet}^{lead}","p_{T}^{lead} [GeV]");
  d_pt_rapjet_lead->GetZaxis()->SetTitle(axisTitle("#partial^{2}N/(#partialy_{jet}^{lead} #partialp_{T}^{lead} [(%.1f GeV#times%.1f unit rapidity)^{#minus1}]",
						   h_ptjet_lead->GetBinWidth(1),h_rapjet_lead->GetBinWidth(1)).c_str());  

  d_pt_rapjet_sublead = AH::book2D<TH2D>(histName("d_pt_rapjet_sublead"),"Sub-leading jet p_{T} versus rapidity",rapBin,rapMin,rapMax,ptBin,ptMin,ptMax,
					 "y_{jet}^{sub-lead}","p_{T}^{sub-lead} [GeV]");
  d_pt_rapjet_sublead->GetZaxis()->SetTitle(axisTitle("#partial^{2}N/(#partialy_{jet}^{sub-lead} #partialp_{T}^{sub-lead} [(%.1f GeV#times%.1f unit rapidity)^{#minus1}]",
						      h_ptjet_sublead->GetBinWidth(1),h_rapjet_sublead->GetBinWidth(1)).c_str());  
  ++nhists;

  /////////////////////////////////////////
  // Leading and subleading jet topology //
  /////////////////////////////////////////

  int imBin(400); double imMin(0.); double imMax(4000.);
  h_mass = AH::book1D<TH1D>(histName("h_mass"),"M_{jj}^{lead,sub-lead}",imBin,imMin,imMax,"M_{jj}^{lead,sub-lead} [GeV]");
  h_mass->GetYaxis()->SetTitle(axisTitle("#partial^{2}N_{jj}/#partialM_{jj}^{lead,sub-lead} [1/(%.1f GeV)]",h_mass->GetBinWidth(1)).c_str());
  ++nhists;

  d_mass_rap_lead = AH::book2D<TH2D>(histName("d_mass_rap_lead"),"M_{jj}^{lead,sub-lead} vs y_{jet}^{lead}",
				     rapBin,rapMin,rapMax,imBin,imMin,imMax,"y_{jet}^{lead}","M_{jj}^{lead,sub-lead} [GeV]");
  d_mass_rap_lead->GetZaxis()->SetTitle(axisTitle("#partial^{2}N_{jj}/(#partialy_{jet}^{lead}#partialM_{jj}^{lead,sub-lead}) [(%.1f GeV#times%.1f unit rapidity)^{#minus1}]",
						  h_mass->GetBinWidth(1),h_rapjet_lead->GetBinWidth(1)).c_str());
  ++nhists;

  d_mass_rap_sublead = AH::book2D<TH2D>(histName("d_mass_rap_sublead"),"M_{jj}^{lead,sub-lead} vs y_{jet}^{sub-lead}",
					rapBin,rapMin,rapMax,imBin,imMin,imMax,"y_{jet}^{sub-lead}","M_{jj}^{lead,sub-lead} [GeV]");
  d_mass_rap_sublead->GetZaxis()->SetTitle(axisTitle("#partial^{2}N_{jj}/(#partialy_{jet}^{sub-lead}#partialM_{jj}^{lead,sub-lead}) [(%.1f GeV#times%.1f unit rapidity)^{#minus1}]",
						     h_mass->GetBinWidth(1),h_rapjet_sublead->GetBinWidth(1)).c_str());
  ++nhists;

  d_rap_rap = AH::book2D<TH2D>(histName("d_rap_rap"),"y_{jet}^{sub-lead} vs y_{jet}^{lead}",rapBin,rapMin,rapMax,rapBin,rapMin,rapMax,"y_{jet}^{lead}","y_{jet}^{sub-lead}");
  d_rap_rap->GetZaxis()->SetTitle(axisTitle("#partialN_{jj}^{2}/(#partialy_{jet}^{lead}#partialy_{jet}^{sub-lead}) [(%.1f unit rapidity#times%.1f unit rapidity)^{#minus1}]",
					    h_rapjet_lead->GetBinWidth(1),h_rapjet_sublead->GetBinWidth(1)).c_str());
  ++nhists;

  int gapBin(100); double gapMin(0.); double gapMax(10.);
  d_mass_rapgap = AH::book2D<TH2D>(histName("d_mass_rapgap"),"M_{jj}^{lead,sub-lead} vs #Deltay_{jj}",gapBin,gapMin,gapMax,imBin,imMin,imMax,"|#Deltay_{jj}|","M_{jj}^{lead,sub-lead} [GeV]");
  d_mass_rapgap->GetZaxis()->SetTitle(axisTitle("#partialN_{jj}/#partial|#Deltay| [(%.1f unit rapidity)^{#minus1}]",d_mass_rapgap->GetXaxis()->GetBinWidth(1)).c_str());
  ++nhists;

  ///////////////////////////////
  // Leading jet signal timing //
  ///////////////////////////////

  int tbin(301); double tmin(-150.5); double tmax(150.5);

  t_lead.h_tjet = AH::book1D<TH1D>(histName("h_tjet_lead"),"Jet Timing (lead)",tbin,tmin,tmax,"t_{jet}^{lead} [ns]");
  t_lead.h_tjet->GetYaxis()->SetTitle(axisTitle("#partialN/#partialt_{jet}^{lead} [(%.1f ns)^{#minus1}]",t_lead.h_tjet->GetBinWidth(1)).c_str());
  ++nhists;

  t_lead.d_tjet_rapjet = AH::book2D<TH2D>(histName("d_tjet_rapjet_lead"),"Jet timing (lead) vs y_{jet}^{lead}",rapBin,rapMin,rapMax,tbin,tmin,tmax,"y_{jet}^{lead}","t_{jet}^{lead} [ns]");
  t_lead.d_tjet_rapjet->GetYaxis()->SetTitle(axisTitle("#partial^{2}N/(#partialt_{jet}^{lead}#partialy_{jet}^{lead}) [(%.1f ns#imes%.1f unit rapidity)^{#minus1}]",
						       t_lead.d_tjet_rapjet->GetXaxis()->GetBinWidth(1),t_lead.d_tjet_rapjet->GetYaxis()->GetBinWidth(1)).c_str());
  ++nhists;
  
  t_lead.h_tsig = AH::book1D<TH1D>(histName("h_tsig_lead"),"Calo signal time (lead jet)",tbin,tmin,tmax,"t_{signal}^{calo} (leading jet) [ns]");
  t_lead.h_tsig->GetYaxis()->SetTitle(axisTitle("#partialN_{signal}^{calo}/#partialt_{signal}^{calo} [(%.1f ns)^{#minus1}]",t_lead.h_tsig->GetBinWidth(1)).c_str());
  ++nhists;

  t_lead.d_tsig_rapsig = AH::book2D<TH2D>(histName("d_tsig_rapsig_lead"),"Calo signal time vs y_{signal}^{calo} (leading jet)",rapBin,rapMin,rapMax,tbin,tmin,tmax,
					  "y_{signal}^{calo}","t_{signal}^{calo} [ns]");
  t_lead.d_tsig_rapsig->GetZaxis()->SetTitle(axisTitle("#partial^{2}N_{signal}^{calo}/(#partialt_{signal}^{calo}#partialy_{signal}^{calo} [(%.1f ns#times%.1f unit rapidity)^{#minus2}",
						       t_lead.d_tsig_rapsig->GetXaxis()->GetBinWidth(1),t_lead.d_tsig_rapsig->GetYaxis()->GetBinWidth(1)).c_str());
  ++nhists;

  //////////////////////////////////
  // Subleading jet signal timing //
  //////////////////////////////////

  t_sublead.h_tjet = AH::book1D<TH1D>(histName("h_tjet_sublead"),"Jet Timing (sub-lead)",tbin,tmin,tmax,"t_{jet}^{sub-lead} [ns]");
  t_sublead.h_tjet->GetYaxis()->SetTitle(axisTitle("#partialN/#partialt_{jet}^{sub-lead} [(%.1f ns)^{#minus1}]",t_sublead.h_tjet->GetBinWidth(1)).c_str());
  ++nhists;

  t_sublead.d_tjet_rapjet = AH::book2D<TH2D>(histName("d_tjet_rapjet_sublead"),"Jet timing (sub-lead) vs y_{jet}^{sub-lead}",
					     rapBin,rapMin,rapMax,tbin,tmin,tmax,"y_{jet}^{sub-lead}","t_{jet}^{sub-lead} [ns]");
  t_sublead.d_tjet_rapjet->GetYaxis()->SetTitle(axisTitle("#partial^{2}N/(#partialt_{jet}^{sub-lead}#partialy_{jet}^{sub-lead}) [(%.1f ns#imes%.1f unit rapidity)^{#minus1}]",
							  t_sublead.d_tjet_rapjet->GetXaxis()->GetBinWidth(1),t_sublead.d_tjet_rapjet->GetYaxis()->GetBinWidth(1)).c_str());
  ++nhists;
  
  t_sublead.h_tsig = AH::book1D<TH1D>(histName("h_tsig_sublead"),"Calo signal time (lead jet)",tbin,tmin,tmax,"t_{signal}^{calo} (sub-leading jet) [ns]");
  t_sublead.h_tsig->GetYaxis()->SetTitle(axisTitle("#partialN_{signal}^{calo}/#partialt_{signal}^{calo} [(%.1f ns)^{#minus1}]",t_sublead.h_tsig->GetBinWidth(1)).c_str());
  ++nhists;

  t_sublead.d_tsig_rapsig = AH::book2D<TH2D>(histName("d_tsig_rapsig_sublead"),"Calo signal time vs y_{signal}^{calo} (sub-leading jet)",rapBin,rapMin,rapMax,tbin,tmin,tmax,
					     "y_{signal}^{calo}","t_{signal}^{calo} [ns]");
  t_sublead.d_tsig_rapsig->GetZaxis()->SetTitle(axisTitle("#partial^{2}N_{signal}^{calo}/(#partialt_{signal}^{calo}#partialy_{signal}^{calo} [(%.1f ns#times%.1f unit rapidity)^{#minus2}",
							  t_sublead.d_tsig_rapsig->GetXaxis()->GetBinWidth(1),t_sublead.d_tsig_rapsig->GetYaxis()->GetBinWidth(1)).c_str());
  ++nhists;

  PRINT_INFO( _mname, "group [%s] booked %zu histograms", name().c_str(), nhists );

  return nhists>0;
}

bool TwoJetAnalysisGroup::fill(const std::vector<fastjet::PseudoJet>& pjets,size_t ifrst,size_t iscnd)
{
  static std::string _mname = "TwoJetAnalysisGroup::Fill";

  if ( !isBooked() ) { PRINT_WARNING( _mname, "histograms are not yet booked!"); return false; }

  if ( ifrst == iscnd || ifrst >= pjets.size() || iscnd >= pjets.size() ) { return false; }

  // get leading jet (in pT)
  size_t ild(ifrst);
  size_t isl(iscnd);
  if ( pjets.at(ifrst).pt() < pjets.at(iscnd).pt() ) { ild = iscnd; isl = ifrst; }
  fastjet::PseudoJet ljet = pjets.at(ild) / Analysis::Units::GeV;
  fastjet::PseudoJet sjet = pjets.at(isl) / Analysis::Units::GeV;

  double pTlead(ljet.pt());
  double pTsublead(sjet.pt());
  double rapLead(ljet.rap());
  double rapSublead(sjet.rap());

  double rapGap(std::abs(rapLead-rapSublead));
  double mass((ljet+sjet).m());
  
  h_ptjet_lead->Fill(pTlead);
  h_ptjet_sublead->Fill(pTsublead);
  h_rapjet_lead->Fill(rapLead);
  h_rapjet_sublead->Fill(rapSublead);

  d_pt_rapjet_lead->Fill(rapLead,pTlead);
  d_pt_rapjet_sublead->Fill(rapSublead,pTsublead);

  h_mass->Fill(mass);
  d_mass_rap_lead->Fill(rapLead,mass);
  d_mass_rap_sublead->Fill(rapSublead,mass);
  d_mass_rapgap->Fill(rapGap,mass);

  d_rap_rap->Fill(rapLead,rapSublead);

  fillTiming(ljet,t_lead);
  fillTiming(sjet,t_sublead);

  return true; 
}

void TwoJetAnalysisGroup::fillTiming(const fastjet::PseudoJet& pj,TimingPlots& tplot)
{
  static std::string _mname = "TwoJetAnalysisGroup::FillTiming";
  // checks if this is a calorimeter jet with timing info
  if ( !(pj.has_valid_cs() && AH::FastJet::hasCaloSignalInfo(pj.constituents().front())) ) { return; }
  //
  PRINT_DEBUG( _mname, "found jet from calorimeter objects with timing info");

  // plots
  double pt(pj.pt()); double rap(pj.rap());
  double jett(0.); double jete(0.); bool jetHasTime(false);
  // constuent timing and jet time calculation
  for ( const auto& cj : pj.constituents() ) {
    double t(Analysis::FastJet::CaloSignalInfo::caloTime(cj));
    PRINT_DEBUG( _mname,"calo time is %f", t); 
    if ( t > Analysis::FastJet::CaloSignalInfo::defTime() ) {
      t /= Analysis::Units::nanosec;
      jett += cj.e()*t; jete += cj.e(); // running sums for energy-weighted time average
      tplot.h_tsig->Fill(t);
      tplot.d_tsig_rapsig->Fill(cj.rap(),t);
      if ( !jetHasTime) { jetHasTime = true; }
    }
  } // constituent loop
  // fill jet timing
  if ( jetHasTime ) { jett /= jete; tplot.h_tjet->Fill(jett); tplot.d_tjet_rapjet->Fill(pj.rap(),jett); }   
}
