//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug 31 08:01:41 2018 by ROOT version 6.14/04
// from TTree charged/
// found on file: medival.root
//////////////////////////////////////////////////////////

#ifndef ChargedSelector_h
#define ChargedSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
const Ssiz_t size = 80;
const Float_t pionMean = 0.143;
const Float_t sigma = 0.023;
const Float_t protMean = 0.942;


class ChargedSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> mh = {fReader, "mh"};
   TTreeReaderValue<Float_t> cent = {fReader, "cent"};
   TTreeReaderArray<Float_t> p = {fReader, "p"};
   TTreeReaderArray<Float_t> the0 = {fReader, "the0"};
   TTreeReaderArray<Float_t> phi = {fReader, "phi"};
   TTreeReaderArray<Float_t> m = {fReader, "m"};
   TTreeReaderArray<Short_t> dcarm = {fReader, "dcarm"};
   TTreeReaderArray<Short_t> charge = {fReader, "charge"};

   TProofOutputFile *proofFile;
   TFile *file;
   TString base = "/home/profile4me/eyeForAnEye/output.root";
   TTree *protons, *pions;
   TF1 *pionFit, *protFit;
   Float_t minProt, maxProt, sigma;
   Float_t minPion, maxPion;

   Int_t i_protons, i_pions;
   Float_t protP[size],  protThe[size], protPhi[size], protCent;
   Short_t  protArm[size];
   Float_t pionP[size],  pionThe[size], pionPhi[size], pionCent;
   Short_t  pionArm[size];

   ChargedSelector(TTree * /*tree*/ =0) { }
   virtual ~ChargedSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(ChargedSelector,0);

};

#endif

#ifdef ChargedSelector_cxx
void ChargedSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t ChargedSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef ChargedSelector_cxx
