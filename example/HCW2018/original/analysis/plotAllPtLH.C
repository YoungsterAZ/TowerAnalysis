
#include "HistHelper.h"
#include "plotPtLH.C"

#include <string>
#include <vector>
#include <TFile.h>

void plotAllPtLH() {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllPtLH - found %zu files\n",fnames.size());

  //  TFile* f = new TFile("debug.root","recreate");
  // loop on all files
  //  gStyle->SetPadRightMargin(0.05);
  double wcvs(gStyle->GetCanvasDefW());
  double hcvs(gStyle->GetCanvasDefH());
  gStyle->SetCanvasDefW(wcvs+79);
  gStyle->SetPadRightMargin(0.05+0.225/2.);
  //  gStyle->SetCanvasDefH(hcvs);
  //

  for ( size_t ic(0); ic < fnames.size(); ++ic ) { //fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("JetPtSpectra_Likelihood_%02zu",ic).Data(),"Efficiencies by probability");
    //    a->SetLogy();
    a->SetLogx();
    plotPtLH(fnames.at(ic),htags.at(ic));
    printf("plotAllPtLikelihood - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"pTeff_prob").c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"pTeff_prob").c_str());
  }
  //    f->Write();
  //    f->Close();
  // }
}
