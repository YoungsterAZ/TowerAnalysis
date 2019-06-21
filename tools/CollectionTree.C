#define CollectionTree_cxx
#include "CollectionTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <vector>

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"


void CollectionTree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L CollectionTree.C
//      root> CollectionTree t
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

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // convert all clusters/towers to PseudoJets
      std::vector<fastjet::PseudoJet> fjvec;
      for ( int i(0); i<topoclus_lcw_e->size(); ++i ) { 
	fastjet::PseudoJet fj(fastjet::PtYPhiM(topoclus_lcw_pt->at(i),
					       topoclus_lcw_eta->at(i),
					       topoclus_lcw_phi->at(i),
					       0.));
	fj.set_user_index(i);
	fjvec.push_back(fj);
      }
      // do jet finding
      fastjet::JetDefinition       jd(fastjet::antikt_algorithm,0.4);
      fastjet::ClusterSequenceArea cs(fjvec,jd,fastjet::VoronoiAreaSpec(0.9));
      // collect the jets with pT > 20 GeV
      std::vector<fastjet::PseudoJet> jvec = cs.inclusive_jets(20000.);
      printf("found %zu jets\n",jvec.size());
      // make jet plots, filter jets, ...
   }
}
