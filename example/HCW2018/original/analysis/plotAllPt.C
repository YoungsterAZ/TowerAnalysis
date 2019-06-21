
#include "HistHelper.h"
#include "plotPt.C"

#include <string>
#include <vector>

void plotAllPt() {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllPt - found %zu files\n",fnames.size());

  // loop on all files
  gStyle->SetPadRightMargin(0.05);
  for ( size_t ic(0); ic < fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("JetPtSpectra_%02zu",ic).Data(),"Jet pT");
    a->SetLogy();
    plotPt(fnames.at(ic),htags.at(ic));
    printf("plotAllPt - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"pTspectra").c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"pTspectra").c_str());
  }

}
