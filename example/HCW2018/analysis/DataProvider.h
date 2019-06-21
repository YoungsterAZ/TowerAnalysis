//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Aug 11 12:08:05 2018 by ROOT version 6.10/04
// from TTree TagJets/TagJets
// found on file: CSSK+TIME_topoclus_tuple.root
//////////////////////////////////////////////////////////

#ifndef DataProvider_h
#define DataProvider_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class DataProvider {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   vector<int>     *yJetReg;
   vector<double>  *yJetMin;
   vector<double>  *yJetMax;
   vector<double>  *yJetAve;
   vector<double>  *ptJet;
   vector<double>  *mJet;
   vector<double>  *wJet;
   vector<double>  *ptdJet;
   vector<double>  *psigJet;
   vector<double>  *pbckJet;
   vector<int>     *ncJet;
   vector<int>     *tagJet;
   vector<bool>    *isSigJet;

   // List of branches
   TBranch        *b_yJetReg;   //!
   TBranch        *b_yJetMin;   //!
   TBranch        *b_yJetMax;   //!
   TBranch        *b_yJetAve;   //!
   TBranch        *b_ptJet;   //!
   TBranch        *b_mJet;   //!
   TBranch        *b_wJet;   //!
   TBranch        *b_ptdJet;   //!
   TBranch        *b_psigJet;   //!
   TBranch        *b_pbckJet;   //!
   TBranch        *b_ncJet;   //!
   TBranch        *b_tagJet;   //!
   TBranch        *b_isSigJet;   //!

   DataProvider(TTree *tree=0);
   virtual ~DataProvider();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef DataProvider_cxx
DataProvider::DataProvider(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("CSSK+TIME_topoclus_tuple.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("CSSK+TIME_topoclus_tuple.root");
      }
      f->GetObject("TagJets",tree);

   }
   Init(tree);
}

DataProvider::~DataProvider()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DataProvider::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DataProvider::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DataProvider::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   yJetReg = 0;
   yJetMin = 0;
   yJetMax = 0;
   yJetAve = 0;
   ptJet = 0;
   mJet = 0;
   wJet = 0;
   ptdJet = 0;
   psigJet = 0;
   pbckJet = 0;
   ncJet = 0;
   tagJet = 0;
   isSigJet = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("yJetReg", &yJetReg, &b_yJetReg);
   fChain->SetBranchAddress("yJetMin", &yJetMin, &b_yJetMin);
   fChain->SetBranchAddress("yJetMax", &yJetMax, &b_yJetMax);
   fChain->SetBranchAddress("yJetAve", &yJetAve, &b_yJetAve);
   fChain->SetBranchAddress("ptJet", &ptJet, &b_ptJet);
   fChain->SetBranchAddress("mJet", &mJet, &b_mJet);
   fChain->SetBranchAddress("wJet", &wJet, &b_wJet);
   fChain->SetBranchAddress("ptdJet", &ptdJet, &b_ptdJet);
   fChain->SetBranchAddress("psigJet", &psigJet, &b_psigJet);
   fChain->SetBranchAddress("pbckJet", &pbckJet, &b_pbckJet);
   fChain->SetBranchAddress("ncJet", &ncJet, &b_ncJet);
   fChain->SetBranchAddress("tagJet", &tagJet, &b_tagJet);
   fChain->SetBranchAddress("isSigJet", &isSigJet, &b_isSigJet);
   Notify();
}

Bool_t DataProvider::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DataProvider::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DataProvider::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DataProvider_cxx
