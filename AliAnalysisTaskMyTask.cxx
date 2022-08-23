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

// Implementation of methods that are to needed for the ALICE Run 2 analysis script

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliMultSelection.h"

#include "AliAnalysisTaskMyTask.h"

#include "TChain.h"
#include "TList.h"
#include "TH1F.h"
#include "AliPID.h"

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(): AliAnalysisTaskSE(), fAOD(0), fOutputList(0), fHistPt(0), 
fPV0ZPos(0), fPIDResponse(0), fNsigProton(0), fTPCResponse(0) {
	// ROOT I/O constructor, no memory allocation
}

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char *name): AliAnalysisTaskSE(name), fAOD(0), fOutputList(0),
fHistPt(0), fPV0ZPos(0), fPIDResponse(0), fNsigProton(0), fTPCResponse(0) {
	DefineInput(0, TChain::Class());
	DefineOutput(1, TList::Class());
}

AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask() {
	if (fOutputList) delete fOutputList;
}

void AliAnalysisTaskMyTask::UserCreateOutputObjects() {
	fOutputList = new TList(); // list of all histograms to be written to output files
	fOutputList->SetOwner(kTRUE); // memory handling delegated to the TList class, no 'delete' statements required 

	fHistPt = new TH1F("fHistPt", "Transverse Momentum", 100, 0, 6);
	fPV0ZPos = new TH1F("fPV0ZPos", "Vertex Z-Coordinate", 100, -10, 10);
	fNsigProton = new TH1F("fNsigProton", "Proton Signal", 100, 0, 3);
	fTPCResponse = new TH2F("fTPCResponse", "TPC Response", 100, 0, 5, 500, 0, 500);

	fOutputList->Add(fHistPt); // DO NOT FORGET TO ADD OBJECTS TO THE LIST!!!
	fOutputList->Add(fPV0ZPos);
	fOutputList->Add(fNsigProton);
	fOutputList->Add(fTPCResponse);

	PostData(1, fOutputList); // add the TList to the output file
}

void AliAnalysisTaskMyTask::UserExec(Option_t *option) {
	fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
	if (!fAOD) return;
	int iTracks = fAOD->GetNumberOfTracks();

	AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
	if (!mgr || !mgr->GetInputEventHandler()) return;
	AliPIDResponse *fPIDResponse = mgr->GetInputEventHandler()->GetPIDResponse();
	if (!fPIDResponse) return;
	AliMultSelection *multSelection =static_cast<AliMultSelection*>(fAOD->FindListObject("MultSelection"));
	if (!multSelection) return;
	double centralityV0M = multSelection->GetMultiplicityPercentile("V0M");

	for (int i = 0; i < iTracks; ++i) {
		AliAODTrack *track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));
		double vertexZ = fAOD->GetPrimaryVertex()->GetZ();
		fTPCResponse->Fill(track->P(), track->GetTPCsignal());

		if (!track || !track->TestFilterBit(1)) continue; // 1 stands for minimum bias trigger
		if ((vertexZ < -10) || (vertexZ > 10)) continue; // to exclude events far from the detector center
		if (std::abs(fPIDResponse->NumberOfSigmasTPC(track, AliPID::kProton)) > 3) continue;

		fHistPt->Fill(track->Pt());
		fPV0ZPos->Fill(vertexZ);
		fNsigProton->Fill(fPIDResponse->NumberOfSigmasTPC(track, AliPID::kProton));
	}
	PostData(1, fOutputList);
}

void AliAnalysisTaskMyTask::Terminate(Option_t *option) {}