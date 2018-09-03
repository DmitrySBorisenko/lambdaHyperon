#define PhenixSelector_cxx
// The class definition in PhenixSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("PhenixSelector.C")
// root> T->Process("PhenixSelector.C","some options")
// root> T->Process("PhenixSelector.C+")
//


#include "PhenixSelector.h"
#include <TH2.h>
#include <TStyle.h>



void PhenixSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

  
}

void PhenixSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   
   //need to calculate meanProt mass and meanPion mass. Also sigma
   proofFile = new TProofOutputFile(base.Data());
   file = proofFile->OpenFile("RECREATE");
   charged = new TTree("charged", "");
   charged->Branch("mh", &i_charged, "mh/I");
   charged->Branch("p", chargedP, "p[mh]/F");
   charged->Branch("the0", chargedThe, "the0[mh]/F");
   charged->Branch("phi", chargedPhi, "phi[mh]/F"); 
   charged->Branch("m", chargedM, "m[mh]/F"); 
   charged->Branch("cent", &chargedCent, "cent/F"); 
   charged->Branch("dcarm", chargedArm, "dcarm[mh]/S"); 
   charged->Branch("charge", chargedCh, "charge[mh]/S"); 

}

Bool_t PhenixSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);

   i_charged = 0;
   for (Int_t i=0; i<*mh.Get(); i++) {
      if (pltof.At(i) < 0) continue;
      Float_t beta = pltof.At(i) / ttof.At(i) / TMath::C() * 10000000;
      if (beta > 1) continue;
      chargedM[i_charged] = p.At(i) * TMath::Sqrt(1.0 / beta / beta - 1);
      chargedP[i_charged] = p.At(i);
      chargedThe[i_charged] = the0.At(i);
      chargedPhi[i_charged] = phi.At(i);
      chargedCh[i_charged] = charge.At(i);
      chargedArm[i_charged] = dcarm.At(i); 
      chargedCent = *cent.Get();
      i_charged++;
   }
   charged->Fill();

   return kTRUE;
}

void PhenixSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
      file->cd();
      charged->Write();
      fOutput->Add(proofFile);
      file->Close();
}

void PhenixSelector::Terminate()
{
   gROOT->SetBatch(kTRUE); //blocking of drawing
   
   //feetback for main.C is just fitting functions
   TFile f("medival.root", "UPDATE");
   
   charged = (TTree*)f.Get("charged");
   charged->Draw("m>>massHist(1000, 0, 2)");
   massHist = (TH1F*)gDirectory->Get("massHist");
   //save hist to file
   massHist->Write();
   //fiting
   pionFit = new TF1("pionFit", "gaus", 0.1, 0.2);
   protFit = new TF1("protFit", "gaus", 0.8, 1);
   massHist->Fit(pionFit, "R");
   massHist->Fit(protFit, "R+");
   pionFit->Write();
   protFit->Write();
   f.Close();

   gROOT->SetBatch(kFALSE); //unblo
}