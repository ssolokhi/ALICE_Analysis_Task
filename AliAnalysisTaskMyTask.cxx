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
#include "AliAODVertex.h"
#include "AliAODInputHandler.h"
#include "AliMultSelection.h"
#include "TChain.h"
#include "TList.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include "AliPID.h"
#include "TClonesArray.h"
#include "AliAODMCParticle.h"

#include "AliAnalysisTaskMyTask.h"

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(): AliAnalysisTaskSE(), 
fAOD(nullptr), fAODv0(nullptr), fOutputList(nullptr), fPIDResponse(nullptr), fMCEvent(nullptr),
fZvertex(nullptr), fTrackPtvsMass(nullptr), fMCPDGCode(nullptr),
fTPCResponse(nullptr), fTOFResponse(nullptr), fITSResponse(nullptr), fTRDResponse(nullptr), fHMPIDResponse(nullptr),
fProtonResponse(nullptr), fThetaVsEta(nullptr), fThetaVsPhi(nullptr), fCentralityVsN(nullptr),
fArmenterosPodolansky(nullptr), alpha(0), qT(0) {
	// Default ROOT I/O constructor, no memory allocation
}

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char *name): AliAnalysisTaskSE(name),
fAOD(nullptr), fAODv0(nullptr), fOutputList(nullptr), fPIDResponse(nullptr), fMCEvent(nullptr),
fZvertex(nullptr), fTrackPtvsMass(nullptr), fMCPDGCode(nullptr),
fTPCResponse(nullptr), fTOFResponse(nullptr), fITSResponse(nullptr), fTRDResponse(nullptr), fHMPIDResponse(nullptr),
fProtonResponse(nullptr), fThetaVsEta(nullptr), fThetaVsPhi(nullptr), fCentralityVsN(nullptr),
fArmenterosPodolansky(nullptr), alpha(0), qT(0) {
	DefineInput(0, TChain::Class());
	DefineOutput(1, TList::Class());
}

AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask() {
	if (fOutputList) delete fOutputList;
}

void AliAnalysisTaskMyTask::UserCreateOutputObjects() {
	fOutputList = new TList(); // list of all histograms to be written to output files
	fOutputList->SetOwner(kTRUE); // memory handling delegated to the TList class, no 'delete' statements required 

	fZvertex = new TH1D("hZvertex", "Vertex Z-Coordinate;z_{vertex} [cm];N_{entries}", 100, -20, 20);
	fOutputList->Add(fZvertex);

	fMCPDGCode = new TH1D("hMCPDGCode", "MC Particle Particle Data Group Code;PDG Code;N_{entries}", 1000, 0, 3500);
	fOutputList->Add(fMCPDGCode);
 	
	fTPCResponse = new TH2D("hTPCResponse", "Time Projection Chamber Response;p [GeV/c];#frac{dE}{dx} [arb.units]", 100, -5, 5, 250, 0, 250);
	fOutputList->Add(fTPCResponse);

	fTOFResponse = new TH2D("hTOFResponse", "Time OF Flight Response;p [GeV/c];#frac{dE}{dx} [arb.units]", 100, -5, 5, 250, 10000, 40000);
	fOutputList->Add(fTOFResponse);

	fITSResponse = new TH2D("hITSResponse", "Inner Tracking System Response;p [GeV/c];#frac{dE}{dx} [arb.units]", 100, -5, 5, 250, 0, 250);
	fOutputList->Add(fITSResponse);

	fTRDResponse = new TH2D("hTRDResponse", "Transition Radiation Detector Response;p [GeV/c];#frac{dE}{dx} [arb.units]", 100, -5, 5, 100, 0, 30);
	fOutputList->Add(fTRDResponse);

	fHMPIDResponse = new TH2D("hHMPIDResponse", "High-Momentum Particle IDentification Response;p [GeV/c];#frac{dE}{dx} [arb.units]", 200, -25, 25, 100, -1010, -990);
	fOutputList->Add(fHMPIDResponse);

	fProtonResponse = new TH2D("hProtonResponse", "TPC Proton Response;p [GeV/c];#frac{dE}{dx} [arb.units]", 100, -5, 5, 250, 0, 250);
	fOutputList->Add(fProtonResponse);

	fTrackPtvsMass = new TH2D("hTrackPtvsMass", "Track Transverse Momentum vs. Mass;M [GeV/c^{2}]; p_{T} [GeV/c]", 500, 0, 3, 100, 0, 10);
	fOutputList->Add(fTrackPtvsMass);

	fThetaVsEta = new TH2D("hThetaVsEta", "Particle Azimutal Angle vs. Pseudorapidity;#theta;#eta", 100, 0, TMath::Pi(), 100, -5, 5);
	fOutputList->Add(fThetaVsEta);

	fThetaVsPhi = new TH2D("hThetaVsPhi", "Particle Azimutal Angle vs. Polar Angle;#theta;#phi", 100, 0, TMath::Pi(), 100, 0, 2*TMath::Pi());
	fOutputList->Add(fThetaVsPhi);

	fCentralityVsN = new TH2D("hCentralityVsN", "Collision centrality vs. number of contributors;centrality;N_{contr}", 100, 0, 100, 100, 0, 100);
	fOutputList->Add(fCentralityVsN);

	fArmenterosPodolansky = new TH2D("fArmenterosPodolansky", "Armenteros-Podolansky Plot for V-Events;alpha;q_{T} [GeV/c]", 100, -1, 1, 100, 0, 0.25);
	fOutputList->Add(fArmenterosPodolansky);

	PostData(1, fOutputList); // add the TList to the output file
}

void AliAnalysisTaskMyTask::UserExec(Option_t *option) {
	fAOD = static_cast<AliAODEvent*>(InputEvent()); 
	if (!fAOD) return;

	if (fAOD->IsPileupFromSPD()) return; // reject pile-up of consequtive events

	AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
	if (!mgr || !mgr->GetInputEventHandler()) return;

	AliPIDResponse *fPIDResponse = mgr->GetInputEventHandler()->GetPIDResponse();
	if (!fPIDResponse) return;

	double vertexZ = fAOD->GetPrimaryVertex()->GetZ();
	fZvertex->Fill(vertexZ);
	if (TMath::Abs(vertexZ) > fZvertexCut) return;; // to exclude events far from the detector center

	AliMultSelection *multSelection =static_cast<AliMultSelection*>(fAOD->FindListObject("MultSelection"));
	if (!multSelection) return;
	double centralityV0M = multSelection->GetMultiplicityPercentile("V0M");
	fCentralityVsN->Fill(centralityV0M, fAOD->GetPrimaryVertex()->GetNContributors());

	int nTracks = fAOD->GetNumberOfTracks();
	for (int i = 0; i < nTracks; ++i) {
		AliAODTrack *track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));		
		if (!track || !track->TestFilterBit(1)) continue; // 1 stands for minimum bias trigger

		fTrackPtvsMass->Fill(track->M(), track->Pt());
		fThetaVsEta->Fill(track->Theta(), track->Eta());
		fThetaVsPhi->Fill(track->Theta(), track->Phi());

		fTPCResponse->Fill(track->P()/track->Charge(), track->GetTPCsignal());
		fTOFResponse->Fill(track->P()/track->Charge(), track->GetTOFsignal());
		fITSResponse->Fill(track->P()/track->Charge(), track->GetITSsignal());
		fTRDResponse->Fill(track->P()/track->Charge(), track->GetTRDsignal());
		fHMPIDResponse->Fill(track->P()/track->Charge(), track->GetHMPIDsignal());

		// available particle respones: kElectron, kProton, kPion, kKaon, kMoun, kTriton, kHe3
		double protonSignal = fPIDResponse->NumberOfSigmasTPC(track, AliPID::kProton);
		if (protonSignal < fProtonSigmaCut) fProtonResponse->Fill(track->P()/track->Charge(), track->GetTPCsignal());
	}

	int nVerteces = fAOD->GetNumberOfV0s();
	for (int i = 0; i < nVerteces; ++i) {
		fAODv0 = dynamic_cast<AliAODv0*>(fAOD->GetV0(i));
		ProcessVertex(fAODv0);
	} 

	fMCEvent = MCEvent();
	if (fMCEvent) ProcessMCParticles();

	PostData(1, fOutputList);
}

void AliAnalysisTaskMyTask::ProcessMCParticles() {
	TClonesArray *fMCParticlesArray = dynamic_cast<TClonesArray*>(fAOD->FindListObject(AliAODMCParticle::StdBranchName()));
	if (!fMCParticlesArray) return;

	int nMCtracks = fMCParticlesArray->GetEntriesFast();
	for (int i = 0; i < nMCtracks; ++i) {
		AliAODMCParticle *MCtrack = static_cast<AliAODMCParticle*>(fMCParticlesArray->At(i));
		if (!MCtrack) continue;
		fMCPDGCode->Fill(MCtrack->GetPdgCode());
	}
}

void AliAnalysisTaskMyTask::ProcessVertex(AliAODv0 *vertex) {
	TVector3 vertexMomentum(vertex->MomV0X(), vertex->MomV0Y(), vertex->MomV0Z());
	TVector3 positiveDaughterMomentum(vertex->MomPosX(), vertex->MomPosY(), vertex->MomPosZ());
	TVector3 negativeDaughterMomentum(vertex->MomNegX(), vertex->MomNegY(), vertex->MomNegZ());

	double positiveLongitudalProjection = positiveDaughterMomentum.Dot(vertexMomentum)/vertexMomentum.Mag();
	double negativeLongitudalProjection = negativeDaughterMomentum.Dot(vertexMomentum)/vertexMomentum.Mag();

	//standard functions AlphaV0 and PtArmVo are well-defined, but for educational purposes using user-defined functions
	double alpha = (positiveLongitudalProjection - negativeLongitudalProjection)/(positiveLongitudalProjection + negativeLongitudalProjection);
	double qT = positiveDaughterMomentum.Perp(vertexMomentum);
	printf("Alpha: Definded: %f Calculated: %f Delta: %f\n", vertex->AlphaV0(), alpha, vertex->AlphaV0() - alpha);
	printf("Qt: Definded: %f Calculated: %f Delta: %f\n", vertex->PtArmV0(), qT, vertex->PtArmV0() - qT);
	fArmenterosPodolansky->Fill(alpha, qT);
}

void AliAnalysisTaskMyTask::Terminate(Option_t *option) {
	printf("Reconstruction finished!\n");
}