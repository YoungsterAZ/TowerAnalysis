
#include "HistHelper.h"
#include "plotWLH.C"

#include <string>
#include <vector>
#include <TFile.h>

void plotAllWLH() {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllWLH - found %zu files\n",fnames.size());

  double wcvs(gStyle->GetCanvasDefW());
  double hcvs(gStyle->GetCanvasDefH());
  gStyle->SetCanvasDefW(wcvs+79);
  gStyle->SetPadRightMargin(0.05+0.225/2.);

  //  TFile* f = new TFile("debug.root","RECREATE");

  for ( size_t ic(0); ic < fnames.size(); ++ic ) { //fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("WPtSpectra_Likelihood_%02zu",ic).Data(),"Efficiencies by probability");
    //    a->SetLogx();
    plotWLH(fnames.at(ic),htags.at(ic));
    printf("plotAllWLH - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"Weff_prob").c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"Weff_prob").c_str());
  }

  //  f->Write(); f->Close();
}
