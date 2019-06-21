
#include "HistHelper.h"
#include "plotNC.C"

#include <string>
#include <vector>

void plotAllNC() {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllNC - found %zu files\n",fnames.size());

  // loop on all files
  gStyle->SetPadRightMargin(0.05);
  for ( size_t ic(0); ic < fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("JetNCSpectra_%02zu",ic).Data(),"Jet W");
    a->SetLogy();
    plotNC(fnames.at(ic),htags.at(ic));
    printf("plotAllNC - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"NCspectra").c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"NCspectra").c_str());
  }

}
