
#include "HistHelper.h"
#include "plotPtLikelihood.C"

#include <string>
#include <vector>
#include <TFile.h>

void plotAllPtLikelihood() {

#include "FileNames.icc"
#include "HistTags.icc"

  std::vector<std::string> mstr = { "all", "signal", "pile-up" };

  printf("plotAllPtLikelihood - found %zu files\n",fnames.size());

  TFile* f = new TFile("debug.root","recreate");
  // loop on all files
  gStyle->SetPadRightMargin(0.05);
  for ( auto mm : mstr ) { 
    for ( size_t ic(0); ic < fnames.size(); ++ic ) { //fnames.size(); ++ic ) {
      TCanvas* a = new TCanvas(TString::Format("JetPtSpectra_Likelihood_%02zu_%s",ic,mm.c_str()).Data(),"Efficiencies by probability");
      //    a->SetLogy();
      a->SetLogx();
      plotPtLikelihood(fnames.at(ic),htags.at(ic),mm);
      printf("plotAllPtLikelihood - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
      a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),TString::Format("pTeff_prob_%s",mm.c_str()).Data()).c_str());
      a->Print(HistHelper::Names::pngFileName(fnames.at(ic),TString::Format("pTeff_prob_%s",mm.c_str()).Data()).c_str());
    }
    f->Write();
    f->Close();
  }
}
