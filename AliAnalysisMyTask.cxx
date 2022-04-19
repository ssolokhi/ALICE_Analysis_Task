// Implementation of methods that are to needed for the ALICE Run 3 analysis script

#include "Tlist.h"
#include "TH1F.h"
#include "AliAODEvent.h"

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(): AliAnalysisTaskSE(), fAOD(0), fOutputList(0), fHistPt(0) {

	// ROOT I/O constructor, no memory allocation
}

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char *name): AliAnalysisTaskSE(), fAOD(0), fOutputList(0), fHistPt(0) {

	DefineInput(0, TChain::Class());
	DefineOutput(1, TChain::Class());
}

AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask() {

	if (fOutputList) delete fOutputList;
}


AliAnalysisTaskMyTask::CreateOutputObjects() {

	fOutputList = new TList();
	fOutputList->SetOwner(kTRUE); // memory handling delegated to the TList class

	fHistPt = new TH1F("fHistPt", "fHistPt", 100, 0, 100);
	fOutputList->Add(fHistPt);

	PostData(1, fOutputList); // add the TList to the output file
}

AliAnalysisTaskMyTask::UserExec(Option_t\* option) {

	fAOD = dynamic_cast<AliAODEvent*>(InputEvent());

	if (!fAOD) return;

	int iTracks = fAOD->GetNumberOfTracks();

	for (int i = 0, i < iTracks; ++i) {

		AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(1));
		if (!track || !track->TestFilterBit(1)) continue; // 1 stands for minimum bias trigger
		fHistPt->Fill(track->Pt());
	}

	PostData(1, fOutputList);
}

AliAnalysisTaskMyTask::Terminate(Option_t *option) {}