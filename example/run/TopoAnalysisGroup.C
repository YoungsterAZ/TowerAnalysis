
#include <cmath>

#include "TopoAnalysisGroup.h"

TopoAnalysisGroup::TopoAnalysisGroup(const std::string& name)
  : Analysis::GroupBase(name)
  , h_dr((TH1D*)0)
  , d_dr_rap((TH2D*)0)
  , d_pt_calo_truth((TH2D*)0)
  , d_rap_calo_truth((TH2D*)0)
  , d_phi_calo_truth((TH2D*)0)
  , d_ptratio_rap((TH2D*)0)
  , d_rapdistance_rap((TH2D*)0)
  , d_phidistance_rap((TH2D*)0)
{ this->book(); }

TopoAnalysisGroup::~TopoAnalysisGroup() 
{ }

bool TopoAnalysisGroup::isBooked() {
  return h_dr != 0; 
}

bool TopoAnalysisGroup::book()
{
  static std::string _mname = "TopoAnalsyisGroup::Book";

  if ( isBooked() ) { PRINT_WARNING( _mname, "histograms already booked" ); return false; }

  size_t nhists(0);

  int rapBin(101); double rapMin(-5.05); double rapMax(5.05);
  int drBin(100); double drMin(-0.05); double drMax(0.45);
  h_dr = AH::book1D<TH1D>(histName("h_dr").c_str(),"#DeltaR(truth-jet,calo-jet)",drBin,drMin,drMax,"#DeltaR");
  h_dr->GetYaxis()->SetTitle(axisTitle("#partialN/#partialR [1./%.2f]",h_dr->GetBinWidth(1)).c_str());
  ++nhists;

  d_dr_rap = AH::book2D<TH2D>(histName("d_dr_rap").c_str(),"#DeltaR(truth-jet,calo-jet) vs y^{calo-jet}",rapBin,rapMin,rapMax,drBin,drMin,drMax,
			      "y^{calo}","#DeltaR"); 
  ++nhists;
			     

  int ptBin(200); double ptMin(0.); double ptMax(2000.);
  d_pt_calo_truth = AH::book2D<TH2D>(histName("d_pt_calo_truth").c_str(),"p_{T}^{calo-jet} vs p_{T}^{truth-jet}",ptBin,ptMin,ptMax,ptBin,ptMin,ptMax,
				     "p_{T}^{truth-jet} [GeV]","p_{T}^{calo-jet} [GeV]");
  d_pt_calo_truth->GetZaxis()->SetTitle(axisTitle("#partial^{2}N/#partialp_{T}^{2} [1/(%.1f GeV^{2})]",
						  d_pt_calo_truth->GetXaxis()->GetBinWidth(1)*d_pt_calo_truth->GetYaxis()->GetBinWidth(1)).c_str());
  ++nhists;

  d_rap_calo_truth = AH::book2D<TH2D>(histName("d_rap_calo_truth").c_str(),"y^{calo-jet} vs y^{truth-jet}",rapBin,rapMin,rapMax,rapBin,rapMin,rapMax,
				      "y^{truth-jet}","y^{calo-jet}");
  d_rap_calo_truth->GetZaxis()->SetTitle(axisTitle("#partial^{2}N/(%.1f#times%.1f)",
						   d_rap_calo_truth->GetXaxis()->GetBinWidth(1),d_rap_calo_truth->GetYaxis()->GetBinWidth(1)).c_str());
  ++nhists;

  int phiBin(64); double phiMin(-3.2); double phiMax(3.2);
  d_phi_calo_truth = AH::book2D<TH2D>(histName("d_rap_calo_truth").c_str(),"y^{calo-jet} vs y^{truth-jet}",phiBin,phiMin,phiMax,phiBin,phiMin,phiMax,
				      "#phi^{truth-jet}","#phi^{calo-jet}");
  d_phi_calo_truth->GetZaxis()->SetTitle(axisTitle("#partial^{2}N/(%.2f rad^{2})",
						   d_phi_calo_truth->GetXaxis()->GetBinWidth(1)*d_phi_calo_truth->GetYaxis()->GetBinWidth(1)).c_str());
  ++nhists;

  int rptBin(302); double rptMin(-1.01); double rptMax(2.01);
  d_ptratio_rap = AH::book2D<TH2D>(histName("d_ptratio_rap").c_str(),"(p_{T}^{calo-jet} #minus p_{T}^{truth-jet})/p_{T}^{truth-jet} vs y^{calo-jet}",
				   rapBin,rapMin,rapMax,rptBin,rptMin,rptMax,"y^{calo-jet}","(p_{T}^{calo-jet} #minus p_{T}^{truth-jet})/p_{T}^{truth-jet}");
  ++nhists;

  int drpBin(102); double drpMin(-0.51); double drpMax(0.51); 
  d_rapdistance_rap = AH::book2D<TH2D>(histName("d_rapdistance_rap").c_str(),"y^{calo-jet}-y^{truth-jet} vs y^{calo-jet}",
				   rapBin,rapMin,rapMax,drpBin,drpMin,drpMax,"y^{calo-jet}","y^{calo-jet} #minus y^{truth-jet}");
  ++nhists;

  int dphBin(102); double dphMin(-0.51); double dphMax(0.51); 
  d_phidistance_rap = AH::book2D<TH2D>(histName("d_phidistance_rap").c_str(),"#phi^{calo-jet}-#phi^{truth-jet} vs y^{calo-jet}",
				       rapBin,rapMin,rapMax,dphBin,dphMin,dphMax,"y^{calo-jet}","#phi^{calo-jet} #minus #phi^{truth-jet}");
  ++nhists; 
 
  PRINT_INFO( _mname, "group [%s] booked %zu histograms", name().c_str(), nhists );

  return nhists>0;
}

bool TopoAnalysisGroup::fill(const Analysis::Matching::JetRelationContainer& jrel,const std::vector<fastjet::PseudoJet>& tjet,const std::vector<fastjet::PseudoJet>& cjet)
{
  static double _pi    = 2.*std::asin(1.);
  static double _twoPi = 2.*_pi;

  // loop on relations
  for ( auto& jr : jrel ) { 
    // get truth jet
    const auto& tj = tjet.at(jr.truthIndex()); double ptTruth(tj.pt()); double rapTruth(tj.rap()); double phiTruth(tj.phi_std());
    // get calo jet
    const auto& cj = cjet.at(jr.caloIndex()); double ptCalo(cj.pt()); double rapCalo(cj.rap()); double phiCalo(cj.phi_std());
    // get distance
    double dR(jr.deltaR());
    // derived variables
    double ptRatio((ptCalo-ptTruth)/ptTruth);
    double deltaRap(rapCalo-rapTruth); 
    double deltaPhi(phiCalo-phiTruth); 
    if ( deltaPhi > _pi  ) { deltaPhi -= _twoPi; } 
    if ( deltaPhi < -_pi ) { deltaPhi += _twoPi; } 

    // plot radial distance
    h_dr->Fill(dR); d_dr_rap->Fill(rapCalo,dR);
    d_dr_rap->Fill(rapCalo,dR);
    // plot kinematic relations: pT
    d_pt_calo_truth->Fill(ptTruth/Analysis::Units::GeV,ptCalo/Analysis::Units::GeV);
    d_ptratio_rap->Fill(rapCalo,ptRatio);
    // plot kinematic relations: rapidity
    d_rap_calo_truth->Fill(rapTruth,rapCalo);
    d_rapdistance_rap->Fill(rapCalo,deltaRap);
    // plot kinematic relations: azimuth
    d_phi_calo_truth->Fill(phiTruth,phiCalo);
    d_phidistance_rap->Fill(rapCalo,deltaPhi);
  }
  return true;
}

