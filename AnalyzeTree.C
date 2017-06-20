#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

const Int_t kMaxfParticles = 1293;

void AnalyzeTree() {
	//Variables used to store the data
	Int_t totalSize=0; // Sum of Data size (in bytes) of all events
	Int_t eventSize=0; // Size of the current event
	TH1F *hPosX; // fPosX

	// List of Branches
	TBranch *nParticlesBranch;
	TBranch *particlesPosXBranch;
	TBranch *particlesMomentumBranch;

	// Declaration of leaf types
	Int_t nParticles;
	Double_t particlesPosX[kMaxfParticles];
	Double_t particlesMomentum[kMaxfParticles];

	//open the file
	TFile *f=TFile::Open("http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOT/eventdata.root");
	if (f==0){
		//if we cannot open the file, print an error message and return immediately
		printf("Error: cannot open http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOt/eventdat.root!\n");
		return;
	}

	//Create tyhe tree reader and its dta containers
	TTreeReader myReader("EventTree",f);
	TTreeReaderValue<Int_t> events(myReader, "fEventSize");

	//Loop over all entries of the TTree or TChain
	while (myReader.Next()) {
		totalSize+=*events;
	}

	Int_t sizeInMB = totalSize/1024/1024;
	printf("Total size of all events: %d MB\n", sizeInMB);

	// get a pointer to the tree
	TTree *tree = (TTree *)f->Get("EventTree");
	//To use SetBranchAddress() with simple types (e.g. double, int)
	// instead of objects (e.g. syd::vector<Particle>).
	tree->SetMakeClass(1);

	// creatiing the TH1F histogram
	hPosX = new TH1F("hPosX", "Position in X", 20, -5, 5);
	//enable bin errors:
	hPosX->Sumw2();

	Long64_t nentries = tree->GetEntries();
	for (Long64_t i=0; i<nentries; i++) {
		nParticlesBranch->GetEntry(i);
		particlesPosXBranch->GetEntry(i);
		particlesMomentumBranch->GetEntry(i);
		for (int iParticle=0; iParticle < nParticles; ++iParticle) {
			if (particlesMomentum[iParticle] > 40.0)
				hPosX->Fill(particlesPosX[iParticle]);
		}
	}
	// fit the histogram
	hPosX->Fit("po12");
	// and draw it:
	hPosX->Draw();

}