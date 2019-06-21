
#include "HistHelper.h"
#include "plotW.C"

#include <string>
#include <vector>

void plotAllW() {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllW - found %zu files\n",fnames.size());

  // loop on all files
  gStyle->SetPadRightMargin(0.05);
  for ( size_t ic(0); ic < fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("JetWSpectra_%02zu",ic).Data(),"Jet W");
    a->SetLogy();
    plotW(fnames.at(ic),htags.at(ic));
    printf("plotAllW - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"Wspectra").c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"Wspectra").c_str());
  }

}
