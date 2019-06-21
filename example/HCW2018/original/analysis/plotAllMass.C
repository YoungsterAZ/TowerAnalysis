
#include "HistHelper.h"
#include "plotMass.C"

#include <string>
#include <vector>

void plotAllMass() {

#include "FileNames.icc"
#include "HistTags.icc"

  printf("plotAllMass - found %zu files\n",fnames.size());

  // loop on all files
  gStyle->SetPadRightMargin(0.05);
  for ( size_t ic(0); ic < fnames.size(); ++ic ) {
    TCanvas* a = new TCanvas(TString::Format("JetMSpectra_%02zu",ic).Data(),"Jet Mass");
    a->SetLogy();
    plotMass(fnames.at(ic),htags.at(ic));
    printf("plotAllMass - file \042%s\042 analyzed\n",fnames.at(ic).c_str());
    a->Print(HistHelper::Names::pdfFileName(fnames.at(ic),"Mspectra").c_str());
    a->Print(HistHelper::Names::pngFileName(fnames.at(ic),"Mspectra").c_str());
  }

}
