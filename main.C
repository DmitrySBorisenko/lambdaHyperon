	const int mode = 2;
	const int filesAmount = 15;
	const Ssiz_t SIZE = 80;
	const Float_t protM = 0.93827;
	const Float_t pionM = 0.13957;

	//TRIGGER BLOCK
	const Bool_t centOn = true;
	const Int_t minCent = 5;
	const Int_t maxCent = 50;
	const Bool_t sameArm = true;
	const Bool_t pTprot_higher_pTpion = false;
	const Bool_t protP_higher_pionP = true;
	const Bool_t pTprot_infinum = false;
	const Bool_t protP_infinum = true;
	TString histName;
	TH1F *invMassHist;
	TH1F *invMassBackground;
	TCanvas *outputCanvas;

	TTree *protons, *pions;
	TFile *file = new TFile("output.root", "UPDATE");
	Int_t i_protons, i_pions, j_protons, j_pions;
	Float_t protP[SIZE],  protThe[SIZE], protPhi[SIZE], protCent;
   	Short_t  protArm[SIZE];
  	Float_t pionP[SIZE],  pionThe[SIZE], pionPhi[SIZE], pionCent;
   	Short_t  pionArm[SIZE];
   	


	TChain *chain;
	void setProtTree() {		
		protons = (TTree*)file->Get("protons");
		protons->SetBranchAddress("mh", &i_protons);
   		protons->SetBranchAddress("p", protP);
   		protons->SetBranchAddress("the0", protThe);
   		protons->SetBranchAddress("phi", protPhi); 
   		protons->SetBranchAddress("dcarm", protArm); 
   		protons->SetBranchAddress("dcarm", protArm); 
   		protons->SetBranchAddress("cent", &protCent); 
	}

	void setPionTree() {
		pions = (TTree*)file->Get("pions");
		pions->SetBranchAddress("mh", &i_pions);
   		pions->SetBranchAddress("p", pionP);
   		pions->SetBranchAddress("the0", pionThe);
   		pions->SetBranchAddress("phi", pionPhi); 
   		pions->SetBranchAddress("dcarm", pionArm); 
   		pions->SetBranchAddress("cent", &pionCent); 
	}

	
void constructHistName() {
	if (centOn) histName += TString::Format("%d<cent<%d", minCent, maxCent);
	if (sameArm) histName += "+sameArm";
	if (pTprot_higher_pTpion) histName += "+protPt>3pionPt";
	if (pTprot_infinum) histName += "+protPt>1.0";    
	if (protP_higher_pionP) histName += "+protP>3pionP";
	if (protP_infinum) histName += "protP>1.0";
	if (!centOn && !sameArm && !pTprot_infinum && !pTprot_higher_pTpion) histName += "withoutCuts";
	cout << histName << endl;
}


void fillChain(){
	Int_t i = 0;
	chain = new TChain("mtree");
	const char *dirPath = "/home/profile4me/lambdaIndent/input/";
	TSystemDirectory *dir = new TSystemDirectory("dir", dirPath);
	dir->Print();
	TList *filesList = dir->GetListOfFiles();
	TIter it(filesList);
	TSystemFile *file;
	while ((file =(TSystemFile*)it.Next()) && i++<filesAmount) 
	{
		if (file->IsDirectory()) continue;
		chain->Add(TString(dirPath)+TString(file->GetName()).Data()) ;
	}
}


int main(){
	constructHistName();
	Float_t l_invMass = 1.05,
			r_invMass = 1.5;
	invMassHist = new TH1F(histName, "inv mass distribution", 250, l_invMass, r_invMass);
	invMassBackground = new TH1F(histName+="_BG", "background for inv mass distribution", 250 , l_invMass, r_invMass);

	if (mode == 1 || mode == 0) 
	TProof *proof = TProof::Open("");
	switch (mode) {
	//MODE 1 - formation of charged tree in medival.root 
		case 0: {
			fillChain();
			chain->SetProof();
			cout << "chain with " << chain->GetEntries() << " entries\n";
			chain->Process("PhenixSelector.C");
			break;
		}
		//MODE 2 = formation of protons tree and pions tree in output.root
		case 1: {
			chain = new TChain("charged");
			chain->Add("medival.root");
			setProtTree();
			setPionTree();
			chain->SetProof();
			chain->Process("ChargedSelector.C");
			break;
		}
		case 2: {
			setProtTree();
			setPionTree();
			Long64_t nEntries = pions->GetEntries();
			for (Long64_t i=0; i<nEntries; i++) {
				pions->GetEntry(i);
				protons->GetEntry(i);
				if (centOn && (protCent < minCent || protCent > maxCent)) continue;
				for (int j=0; j<i_protons; j++) {
					for (int k=0; k<i_pions; k++) {
						if (sameArm && protArm[j] != pionArm[k]) continue;
						if (protP_infinum && protP[j] < 1) continue;
						if (protP_higher_pionP && protP[j] < 2*pionP[k]) continue;
						TVector3 v3_prot(1,1,1);
						v3_prot.SetMag(protP[j]);
						v3_prot.SetTheta(protThe[j]);
						v3_prot.SetPhi(protPhi[j]);
						if (pTprot_infinum && v3_prot.Perp() < 1.0) continue;
						TVector3 v3_pion(1,1,1);
						v3_pion.SetMag(pionP[j]);
						v3_pion.SetTheta(pionThe[j]);
						v3_pion.SetPhi(pionPhi[j]);
						if (pTprot_higher_pTpion && v3_prot.Perp() < 3*v3_pion.Perp()) continue;
						//calculation of inv mass
						Float_t totalMass = pionM + protM;
						Float_t totalP = (v3_pion+v3_prot).Mag();
						Float_t invMass = TMath::Sqrt(totalMass*totalMass + totalP*totalP);
						invMassHist->Fill(invMass);
					}
				}
			}
			invMassHist->Write();
			invMassHist->Draw();
		}
		case 3: {
			setProtTree();
			setPionTree();
			Long64_t nEntries = pions->GetEntries();
			for (Long64_t i=0; i< (nEntries - 1); i++) {
				pions->GetEntry(i);
				protons->GetEntry(i+1);
				if (centOn && (protCent < minCent || protCent > maxCent)) continue;
				for (int j=0; j<i_protons; j++) {
					for (int k=0; k<i_pions; k++) {
						if (sameArm && protArm[j] != pionArm[k]) continue;
						if (protP_infinum && protP[j] < 1) continue;
						if (protP_higher_pionP && protP[j] < 3*pionP[k]) continue;
						TVector3 v3_prot(1,1,1);
						v3_prot.SetMag(protP[j]);
						v3_prot.SetTheta(protThe[j]);
						v3_prot.SetPhi(protPhi[j]);
						if (pTprot_infinum && v3_prot.Perp() < 1.0) continue;
						TVector3 v3_pion(1,1,1);
						v3_pion.SetMag(pionP[j]);
						v3_pion.SetTheta(pionThe[j]);
						v3_pion.SetPhi(pionPhi[j]);
						if (pTprot_higher_pTpion && v3_prot.Perp() < 3*v3_pion.Perp()) continue;
						//calculation of inv mass
						Float_t totalMass = pionM + protM;
						Float_t totalP = (v3_pion+v3_prot).Mag();
						Float_t invMass = TMath::Sqrt(totalMass*totalMass + totalP*totalP);
						invMassBackground->Fill(invMass);
					}
				}
				pions->GetEntry(i+1);
				protons->GetEntry(i);
				if (centOn && (protCent < minCent || protCent > maxCent)) continue;
				for (int j=0; j<i_protons; j++) {
					for (int k=0; k<i_pions; k++) {
						if (sameArm && protArm[j] != pionArm[k]) continue;
						if (protP_infinum && protP[j] < 1) continue;
						if (protP_higher_pionP && protP[j] < 3*pionP[k]) continue;
						TVector3 v3_prot(1,1,1);
						v3_prot.SetMag(protP[j]);
						v3_prot.SetTheta(protThe[j]);
						v3_prot.SetPhi(protPhi[j]);
						if (pTprot_infinum && v3_prot.Perp() < 1.0) continue;
						TVector3 v3_pion(1,1,1);
						v3_pion.SetMag(pionP[j]);
						v3_pion.SetTheta(pionThe[j]);
						v3_pion.SetPhi(pionPhi[j]);
						if (pTprot_higher_pTpion && v3_prot.Perp() < 3*v3_pion.Perp()) continue;
						//calculation of inv mass
						Float_t totalMass = pionM + protM;
						Float_t totalP = (v3_pion+v3_prot).Mag();
						Float_t invMass = TMath::Sqrt(totalMass*totalMass + totalP*totalP);
						invMassBackground->Fill(invMass);
					}
				}
			}
			invMassBackground->Write();
		}
	}

	return 0;
}