#include "TFile.h"
#include"TTree.h"

void AnalyzeTree() {
//Variables used to store the data
Int_t totalSize=0; // Sum of Data size (in bytes) of all events

//open the file
TFile *f=Tfile::Open("http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOT/eventdata.root");
if (f==0){
	//if we cannot open the file, print an error message and return immediately
	printf("Error: cannot open http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOt/eventdat.root!\n")
	return;
}

/Create tyhe tree reader and its dta containers
TTreeReader myReader("EventTree",f);
TTreReaderValue,Int_t> eventSize(myReader, "fEventSize");

//Loop over all entries of the TTree or TChain
while (myReader.Next()){
	\\ Get the data from the current TTree entry by getting
	\\ the value from the connected reader (eventSize):
	totalSize+=*eventSize;
}

Int_t sizeInMB = totalSize/1024/1024;
printf("Total size of all events: %d MB\n", sizeInMB);