//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 30 20:40:16 2018 by ROOT version 6.14/04
// from TTree mtree/Hadron EMC + TOF tree
// found on file: /home/profile4me/lambdaIndent/input/310714.root
//////////////////////////////////////////////////////////

#ifndef PhenixSelector_h
#define PhenixSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector

const Short_t size = 80;
/*const Float_t pionMeat = 0.0;
const Float_t sigma = 0.0;
const Float_t protMean = 0.0;*/

class PhenixSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Float_t> cent = {fReader, "cent"};
   TTreeReaderValue<Int_t> mh = {fReader, "mh"};
   TTreeReaderArray<Short_t> dcarm = {fReader, "dcarm"};
   TTreeReaderArray<Float_t> p = {fReader, "p"};
   TTreeReaderArray<Short_t> charge = {fReader, "charge"};
   TTreeReaderArray<Float_t> phi0 = {fReader, "phi0"};
   TTreeReaderArray<Float_t> the0 = {fReader, "the0"};
   TTreeReaderArray<Float_t> phi = {fReader, "phi"};
   TTreeReaderArray<Float_t> ttof = {fReader, "ttof"};
   TTreeReaderArray<Float_t> pltof = {fReader, "pltof"};
   /*
   TTreeReaderArray<Int_t> slat = {fReader, "slat"};
   TTreeReaderValue<Int_t> run = {fReader, "run"};
   TTreeReaderValue<Float_t> bbcz = {fReader, "bbcz"};
   TTreeReaderValue<Int_t> rh = {fReader, "rh"};
   TTreeReaderArray<Float_t> phir = {fReader, "phir"};
   TTreeReaderArray<Float_t> time = {fReader, "time"};
   TTreeReaderArray<Float_t> qr0 = {fReader, "qr0"};
   TTreeReaderArray<Float_t> etar = {fReader, "etar"};
   TTreeReaderArray<Short_t> armr = {fReader, "armr"};
   TTreeReaderArray<Short_t> ring = {fReader, "ring"};
   TTreeReaderArray<Int_t> chid = {fReader, "chid"};
   TTreeReaderArray<Float_t> alpha = {fReader, "alpha"};
   
   TTreeReaderArray<Float_t> ecore = {fReader, "ecore"};
   TTreeReaderArray<Float_t> plemc = {fReader, "plemc"};
   TTreeReaderArray<Float_t> ecent = {fReader, "ecent"};
   TTreeReaderArray<Float_t> temc = {fReader, "temc"};
   TTreeReaderArray<Float_t> temcpi = {fReader, "temcpi"};
   TTreeReaderArray<Float_t> temcp = {fReader, "temcp"};
   TTreeReaderArray<Float_t> temck = {fReader, "temck"};
   TTreeReaderArray<Short_t> sect = {fReader, "sect"};
   TTreeReaderArray<Float_t> isPiemc = {fReader, "isPiemc"};
   TTreeReaderArray<Float_t> isPemc = {fReader, "isPemc"};
   TTreeReaderArray<Float_t> isKemc = {fReader, "isKemc"};
   TTreeReaderArray<Int_t> idtwr = {fReader, "idtwr"};
   TTreeReaderArray<Float_t> sigtof = {fReader, "sigtof"};
   TTreeReaderArray<Float_t> sigpc3 = {fReader, "sigpc3"};
   TTreeReaderArray<Float_t> sigemc = {fReader, "sigemc"};
   TTreeReaderArray<Float_t> res = {fReader, "res"};
   TTreeReaderArray<Float_t> etof = {fReader, "etof"};
   TTreeReaderArray<Float_t> isPi = {fReader, "isPi"};
   TTreeReaderArray<Float_t> isP = {fReader, "isP"};
   TTreeReaderArray<Float_t> isK = {fReader, "isK"};
*/

   TProofOutputFile *proofFile;
   TFile *file;
   TH1F *massHist;
   TF1 *pionFit, *protFit;
   TTree *charged;

   TString base = "/home/profile4me/eyeForAnEye/medival.root";
   //variables for feedback


   Int_t i_charged;
   Float_t chargedP[size],  chargedThe[size], chargedPhi[size], chargedM[size], chargedCent;
   Short_t chargedCh[size], chargedArm[size]; 
   
   


   PhenixSelector(TTree * /*tree*/ =0) { }
   virtual ~PhenixSelector() { }
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

   ClassDef(PhenixSelector,0);

};

#endif

#ifdef PhenixSelector_cxx
void PhenixSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t PhenixSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef PhenixSelector_cxx
