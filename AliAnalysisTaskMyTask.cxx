/**************************************************************************
 *    Copyright(c) 2022, ALICE Experiment at CERN, All rights reserved.   *
 *                                                                        *
 * Author: Sergei Solokhin                                                *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// Implementation of methods that are to needed for the ALICE Run 3 analysis script

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"

#include "AliAnalysisTaskMyTask.h"

#include "TChain.h"
#include "TList.h"
#include "TH1F.h"

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(): AliAnalysisTaskSE(), fAOD(0), fOutputList(0), fHistPt(0) {

	// ROOT I/O constructor, no memory allocation
}

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char *name): AliAnalysisTaskSE(name), fAOD(0), fOutputList(0), fHistPt(0) {

	DefineInput(0, TChain::Class());
	DefineOutput(1, TList::Class());
}

AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask() {

	if (fOutputList) delete fOutputList;
}

void AliAnalysisTaskMyTask::UserCreateOutputObjects() {

	fOutputList = new TList(); // list of all histograms to be written to output files
	fOutputList->SetOwner(kTRUE); // memory handling delegated to the TList class, no 'delete' statements required 

	fHistPt = new TH1F("fHistPt", "fHistPt", 100, 0, 100);
	fOutputList->Add(fHistPt); // DO NOT FORGET TO ADD OBJECTS TO THE LIST!!!

	PostData(1, fOutputList); // add the TList to the output file
}

void AliAnalysisTaskMyTask::UserExec(Option_t *option) {

	fAOD = dynamic_cast<AliAODEvent*>(InputEvent());

	if (!fAOD) return;

	int iTracks = fAOD->GetNumberOfTracks();

	for (int i = 0; i < iTracks; ++i) {

		AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));
		if (!track || !track->TestFilterBit(1)) continue; // 1 stands for minimum bias trigger
		fHistPt->Fill(track->Pt());
	}

	PostData(1, fOutputList);
}

void AliAnalysisTaskMyTask::Terminate(Option_t *option) {}