
#include <TCanvas.h>
#include <TArc.h>
#include <TH1D.h>
#include <TLine.h>

#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

void plotCellGrid() {

  const double solenoidField(2.0);
  const double fieldForce(0.15e-03*solenoidField);
  const double trackRadiusScale(1./fieldForce);

  const double iLArR(1150.);
  const double rLArR(2250.);
  const double dRLAr(rLArR-iLArR);
  const std::vector<double> dRLArFrac = { 6./28.,  20./28., 2./28. };

  const double iTileR(2280.);
  const double rTileR(4230.);
  const double dRTile(rTileR-iTileR);
  const std::vector<double> dRTileFrac = { 1.4/7.2, 4.0/7.2, 1.8/7.2 };

  // gStyle->SetPaperSize(20,20);
  gStyle->SetPadLeftMargin(0.01);
  gStyle->SetPadRightMargin(0.01);   
  gStyle->SetPadBottomMargin(0.01);
  gStyle->SetPadTopMargin(0.01);
  gStyle->SetCanvasDefW(1400);
  gStyle->SetCanvasDefH(700);


  std::vector<double> rSampLArR;
  rSampLArR.push_back(iLArR);
  for ( auto dr : dRLArFrac ) { rSampLArR.push_back(rSampLArR.back()+dr*dRLAr); }

  std::vector<double> rSampTileR;
  rSampTileR.push_back(iTileR);
  for ( auto dr : dRTileFrac ) { rSampTileR.push_back(rSampTileR.back()+dr*dRTile); }

  const double _pi = 3.141592653589793238463;

  std::vector<double> dEtaLAr = { 0.025/8., 0.025,    0.050    };
  std::vector<double> dPhiLAr = { _pi/32.,  _pi/128., _pi/128. };

  std::vector<double> dEtaTile = {     0.1,      0.1,    0.2  };
  std::vector<double> dPhiTile = { _pi/32.,  _pi/32., _pi/32. };

  double radToAng(180./_pi); double angToRad(_pi/180.);
  double phiMin(0.);
  double phiMax(phiMin+180.);

  gStyle->SetFrameBorderSize(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameLineColor(kWhite);

  TCanvas* aa = new TCanvas("EtaPhiGrid","EtaPhiGrid");

  double ymax(rTileR*std::sin(_pi/2.));

  TH1D* _frame = new TH1D("_frame","_frame",100,-ymax,ymax);
  _frame->SetMinimum(0.);
  _frame->SetMaximum(ymax);
  _frame->DrawCopy("A");

  for ( auto f(rSampTileR.rbegin()); f!=rSampTileR.rend(); ++f ) { 
    TArc* ar = new TArc(0.,0.,(*f),phiMin,phiMax); ar->Draw("same only");
  } 

  for ( auto f(rSampLArR.rbegin()); f!=rSampLArR.rend(); ++f ) { 
    TArc* ar = new TArc(0.,0.,(*f),phiMin,phiMax); ar->Draw("same only");
  } 

  double phiBegin(phiMin*angToRad);
  double phiEnd(phiMax*angToRad);

  for ( size_t ir(1); ir < rSampLArR.size(); ++ir ) { 
    double rmin(rSampLArR.at(ir-1)); double rmax(rSampLArR.at(ir)); 
    double phi0(phiBegin);
    while  ( phi0 < phiEnd+0.01 ) { 
      double xl(rmin*std::cos(phi0)); double xh(rmax*std::cos(phi0));
      double yl(rmin*std::sin(phi0)); double yh(rmax*std::sin(phi0));
      TLine* l = new TLine(xl,yl,xh,yh);
      l->Draw("same");
      phi0 += dPhiLAr.at(ir-1);
    }
  } 

  for ( size_t ir(1); ir < rSampTileR.size(); ++ir ) { 
    double rmin(rSampTileR.at(ir-1)); double rmax(rSampTileR.at(ir)); 
    double phi0(phiBegin);
    while  ( phi0 < phiEnd+0.01 ) { 
      double xl(rmin*std::cos(phi0)); double xh(rmax*std::cos(phi0));
      double yl(rmin*std::sin(phi0)); double yh(rmax*std::sin(phi0));
      TLine* l = new TLine(xl,yl,xh,yh);
      l->Draw("same");
      phi0 += dPhiTile.at(ir-1);
    }
  }

  aa->Print("ATLASCaloGrid.pdf"); 
  aa->Print("ATLASCaloGrid.png"); 

  std::vector<double> chrgPt     = { 1.,   2.5,   5., 10. };
  std::vector<double> chrgPhi    = { 0.,   45,   90., 160. };
  std::vector<double> chrgPhiMax = { 90., 42.,   22.5, 13.0 };
  std::vector<int>    chrgCols   = { kBlack, kBlue, kViolet, kRed };  
  std::vector<string> chrgFiles  = { "-01", "-02", "-03", "-04" }; 

  std::vector<TEllipse*> collection;

  for ( size_t i(0); i<4; /*chrgPt.size();*/ ++i ) {
    double r(trackRadiusScale*chrgPt.at(i)/2.);
    double phi((chrgPhi.at(i)+90.)*angToRad);    // always starts tangential!
    double cosphi(std::cos(phi));
    double sinphi(std::sin(phi));
    double x(r*cosphi);
    double y(r*sinphi);
    double u(2.*x);
    double v(2.*y);
    if ( u > _frame->GetXaxis()->GetXmax() && v > _frame->GetYaxis()->GetXmax() ) { 
      TLine* l = new TLine(0.,0.,rTileR*cosphi,rTileR*sinphi);
      l->Draw("same");
      printf("plot track at (%6.2f,%6.2f) with radius %6.2f\n",x,y,r);
      continue;
    }
    if ( u > rTileR*std::cos(chrgPhi.at(i)*angToRad) ) { 
      u = rTileR*std::cos(chrgPhi.at(i)*angToRad); 
    }
    if ( v > rTileR*std::sin(chrgPhi.at(i)*angToRad) ) { 
      v = rTileR*std::sin(chrgPhi.at(i)*angToRad); 
    }
    double rm(std::sqrt(u*u+v*v));
    double phi0(0.); double phi1(chrgPhiMax.at(i));
    // if ( rm > r ) { 
    //   double dr(rm-r); 
    // }
    TEllipse* ar = new TEllipse(x,y,r,r,phi0,phi1,180.+phi*radToAng);
    //    TEllipse* ar = new TEllipse(x,y,r,r,phi0,phi1,-chrgPhi.at(i));
    ar->SetFillColor(0);
    ar->SetFillStyle(0);
    ar->SetLineColor(chrgCols.at(i));
    ar->SetLineWidth(2);
    ar->Draw("same only");

    std::string fpdf(TString::Format("ATLASCaloGridWithTrack%s.pdf",chrgFiles.at(i).c_str()).Data());
    std::string fpng(TString::Format("ATLASCaloGridWithTrack%s.png",chrgFiles.at(i).c_str()).Data());

    aa->Print(fpdf.c_str());
    aa->Print(fpng.c_str());

    collection.push_back(new TEllipse(*ar)); delete ar;
  }

  for ( auto pc : collection ) { pc->Draw("same only"); }
  aa->Print("ATLASCaloGridWithAllTracks.pdf"); 
  aa->Print("ATLASCaloGridWithAllTracks.png"); 
}
