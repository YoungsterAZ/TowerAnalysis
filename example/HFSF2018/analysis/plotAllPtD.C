
#include "HistHelper.h"
#include "plotPtD.C"

#include <string>
#include <vector>

void plotAllPtD() {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllPtD - found %zu files\n",fnames.size());

  // loop on all files
  gStyle->SetPadRightMargin(0.05);
  for ( size_t ic(0); ic < fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("JetPtDSpectra_%02zu",ic).Data(),"Jet PtD");
    a->SetLogy();
    plotPtD(fnames.at(ic),htags.at(ic));
    printf("plotAllPtD - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"PtDspectra").c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"PtDspectra").c_str());
  }

}
