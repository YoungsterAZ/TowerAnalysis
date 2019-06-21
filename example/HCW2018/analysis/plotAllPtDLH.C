
#include "HistHelper.h"
#include "plotPtDLH.C"

#include <string>
#include <vector>
#include <TFile.h>

void plotAllPtDLH(bool notAll=false) {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllPtDLH - found %zu files\n",fnames.size());

  double wcvs(gStyle->GetCanvasDefW());
  double hcvs(gStyle->GetCanvasDefH());
  gStyle->SetCanvasDefW(wcvs+79);
  gStyle->SetPadRightMargin(0.05+0.225/2.);

  // TFile* f = new TFile("debug.root","RECREATE");

  for ( size_t ic(0); ic < fnames.size(); ++ic ) { //fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("PtDSpectra_Likelihood_%02zu",ic).Data(),"Efficiencies by probability");
    //    a->SetLogx();
    plotPtDLH(fnames.at(ic),htags.at(ic),notAll);
    printf("plotAllPtDLH - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    if ( !notAll ) { 
      a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"PtDeff_prob").c_str());
      a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"PtDeff_prob").c_str());
    } else {
      a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"PtDeff_prob_notall").c_str());
      a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"PtDeff_prob_notall").c_str());
    }
  }

  // f->Write(); f->Close();
}
