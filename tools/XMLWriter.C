#define XMLWRITER_CXX
#include "XMLWriter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "XMLData.h"

#include <fstream>


void XMLWriter::Loop(const std::string& ofname)
{
//   In a ROOT session, you can do:
//      root> .L XMLWriter.C
//      root> XMLWriter t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();
   if ( nentries <= 0 ) { 
     printf("XMLWriter - ERROR no input (nentries = %i)\n",(int)nentries); 
     return; 
   } else {
     printf("XMLWriter - INFO  analyze %i events\n",(int)nentries); 
   }
   
   XMLData xmldata("VBF2600");

   Long64_t nbytes = 0, nb = 0;
   int nline(0);
   int ifile(0);
   long int nfreq=1000;
   std::ofstream outfile;
   std::string   fn;
   std::string   fo;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // Long64_t ientry = LoadTree(jentry);
      // if (ientry < 0) break;
      // nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      long int ientry = xmldata.fillEvent(this,jentry);
      if ( ientry < 0 ) { break; }
      // open file
      if ( jentry == 0 || jentry % nfreq == 0 ) {
	if ( outfile.is_open() ) { outfile.close(); } 
	fn = std::string(TString::Format("%s_%03i.xml",ofname.c_str(),ifile).Data()); 
	outfile.open(fn.c_str());
	if ( !outfile.is_open() ) { 
	  printf("XMLWriter - ERROR cannot open file \042%s\042 for output\n",fn.c_str());
	  return;
	} else {
	  if ( jentry != 0 ) { printf("XMLWriter - INFO  %i lines written to file \042%s\042\n",nline,fo.c_str()); }
	  nline = 0; ++ifile; fo = fn;
	  printf("XMLWriter - INFO  output file \042%s\042 open\n",fn.c_str());
	}
      }
      const std::vector<std::string>& wbuf = xmldata.getEvent();
      for ( auto lbuf : wbuf ) { outfile << lbuf << std::endl; ++nline; } 
   }

   outfile.close();

}
