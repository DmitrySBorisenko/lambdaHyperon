#define ChargedSelector_cxx
// The class definition in ChargedSelector.h has been generated automatically
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
// root> T->Process("ChargedSelector.C")
// root> T->Process("ChargedSelector.C","some options")
// root> T->Process("ChargedSelector.C+")
//


#include "ChargedSelector.h"
#include <TH2.h>
#include <TStyle.h>



void ChargedSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

     TFile f("/home/profile4me/eyeForAnEye/medival.root");
   pionFit = (TF1 *)f.Get("pionFit");
   protFit = (TF1 *)f.Get("protFit");
   // Info("Begin", "address of pionFit: %d", (Int_t) pionFit);
}

void ChargedSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   proofFile = new TProofOutputFile(base.Data());
   file = proofFile->OpenFile("RECREATE");
   
   protons = new TTree("protons", "");
   protons->Branch("mh", &i_protons, "mh/I");
   protons->Branch("p", protP, "p[mh]/F");
   protons->Branch("the0", protThe, "the0[mh]/F");
   protons->Branch("phi", protPhi, "phi[mh]/F"); 
   protons->Branch("cent", &protCent, "cent/F"); 
   protons->Branch("dcarm", protArm, "dcarm[mh]/S"); 
   
   pions = new TTree("pions", "");
   pions->Branch("mh", &i_pions, "mh/I");
   pions->Branch("p", pionP, "p[mh]/F");
   pions->Branch("the0", pionThe, "the0[mh]/F");
   pions->Branch("phi", pionPhi, "phi[mh]/F"); 
   pions->Branch("cent", &pionCent, "cent/F"); 
   pions->Branch("dcarm", pionArm, "dcarm[mh]/S"); 

   //also we reading fit parameters
   TFile f("/home/profile4me/eyeForAnEye/medival.root");
   pionFit = (TF1 *)f.Get("pionFit");
   protFit = (TF1 *)f.Get("protFit");
   sigma = pionFit->GetParameter(2);
   minPion = pionFit->GetParameter(1) - sigma;
   maxPion = pionFit->GetParameter(1) + sigma;
   minProt = protFit->GetParameter(1) - sigma;
   maxProt = protFit->GetParameter(1) + sigma;
   
}


Bool_t ChargedSelector::Process(Long64_t entry)
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

   i_protons = i_pions = 0;
   for (Int_t i=0; i<*mh.Get(); i++) {
      if (m.At(i) > minProt && m.At(i) < maxProt && charge.At(i) > 0) {
      
         protArm[i_protons] = dcarm.At(i);
         protP[i_protons] = p.At(i);
         protThe[i_protons] = the0.At(i);
         protPhi[i_protons] = phi.At(i);
         protCent = *cent.Get();
         i_protons++;
      }
      if (m.At(i) > minPion && m.At(i) < maxPion && charge.At(i) < 0) { 
         pionArm[i_pions] = dcarm.At(i);
         pionP[i_pions] = p.At(i);
         pionThe[i_pions] = the0.At(i);
         pionPhi[i_pions] = phi.At(i);
         pionCent = *cent.Get();
         i_pions++;
      }
   }
      
   
   protons->Fill();
   pions->Fill();
   return kTRUE;
}

void ChargedSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   file->cd();
   protons->Write();
   pions->Write();
   fOutput->Add(proofFile);
   file->Close();
}

void ChargedSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}