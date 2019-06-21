
#include "XMLData.cxx"
#include "XMLWriter.C"

#include <TChain>

void writeXML()
{
  TChain* fc = new TChain("CollectionTree");
  fc->Add("vbf2600_tower_tuple.root");

  XMLWriter* writer = new XMLWriter(fc);

  writer->Loop("vbf2600_tower_tuple.xml");
}
