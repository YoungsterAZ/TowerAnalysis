
#include <TFile.h>
#include <TString.h>
#include <TCanvas.h>

#include "HistHelper.h"
#include "plotLikelihood.C"

#include <string>
#include <vector>

void plotAllLikelihood(int mode=0) {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllLikelihood - found %zu files\n",fnames.size());

  std::string sstr = mode == 0 ? "LHspectraPlots" : mode == 1 ? "LHspectra" : mode == 2 ? "LHspectraNoNorm" : "UNKNOWN";
  if ( sstr == "UNKNOWN" ) { 
    printf("plotAllLikelihood - invalid mode %i, do nothing\n",mode);
    return;
  }

  TFile* f = 0;
  if ( mode >= 1 ) { f = new TFile(TString::Format("%s.root",sstr.c_str()).Data(),"RECREATE"); }

  // loop on all files
  gStyle->SetPadRightMargin(0.05);
  for ( size_t ic(0); ic < fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("SignalLikehoodSpectra_%02zu",ic).Data(),"Signal Likelihood");
    a->SetLogy();
    plotLikelihood(fnames.at(ic),htags.at(ic),mode);
    printf("plotAllLikelihood - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),sstr).c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),sstr).c_str());
  }

  // write hists
  if ( f != 0 ) {
    f->Write();
    f->Close();
  }
}
