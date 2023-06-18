/* Copyright(c) 2022, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

// Definition of prototypes of all methods that are to be implemented in the ALICE Run 2 analysis script

#ifndef AliAnalysisTaskMyTask_H // include guard (to avoid double inclusion)
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"
#include "AliPIDResponse.h"
#include "AliMCEvent.h"

class AliPIDResponce;
class AliMultSelectionTask;

class AliAnalysisTaskMyTask: public AliAnalysisTaskSE {
	/* derived from the AliAnalysisTask class for Single Events*/
	public:
		// two constructors
		AliAnalysisTaskMyTask();
		AliAnalysisTaskMyTask(const char *name);

		// Destructor
		virtual ~AliAnalysisTaskMyTask();

		// For booking output histograms, TTrees, etc. to the .root file
		virtual void UserCreateOutputObjects();

		// To be called for EACH event that passed the trigger
		virtual void UserExec(Option_t *option);

		// function treating Monte-Carlo generated particles
		void ProcessMCParticles(); 

		// function calculating decay vertex parameters
		void ProcessVertex(AliAODv0 *vertex); 

		// Called at the end of an analysis task
		virtual void Terminate(Option_t *option);

	private:
		/*Adding class members; the //! tags are to be attached to ALL RUN-TIME
		 RESOLVED members for ROOT to generate documentation correctly
		 names must begin with 'f' in accordance with ALICE's codestyle*/

		AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); //copy constructor, not implemented
		AliAnalysisTaskMyTask &operator=(const AliAnalysisTaskMyTask&);

		/* ClassDef C preproccessor, to be used for classes derived from TObject.
		The '\cond' tags are for generating proper documentation */

		/// \cond CLASSDEF
		ClassDef(AliAnalysisTaskMyTask, 1);
		/// \endcond

		AliAODEvent *fAOD; //! input event
		TList *fOutputList; //! List of general output histograms
		TList *fVertexList; //! List of output histograms refering to verteces
		TTree *fVertexTree; //! Tree of vertex parameters
		AliAODv0 *fAODv0; //! primary vertex
		AliPIDResponse *fPIDResponse; //! particle identification response
		AliMCEvent *fMCEvent; //! Monte-Carlo event flag

		// histograms for the 'Histograms' subfolder
		TH1D *fZvertex; //! Vertex z-coordinate 
		TH1D *fMCPDGCode; //! Monte-Carlo generated particles' PDG code

		TH2D *fTPCResponse; //! Time Projection Chamber response 
		TH2D *fTOFResponse; //! Time Of Flight response 
		TH2D *fITSResponse; //! Inner Tracking System response 
		TH2D *fTRDResponse; //! Transition Radiation Detector response 
		TH2D *fHMPIDResponse; //! High-Momentum Particle IDentification response 

		TH2D *fProtonResponse; //! Proton TPC response 
		TH2D *fTrackPtvsMass; //! Event transverse momentum
		TH2D *fThetaVsEta; //! particle azimutal angle vs. pseudorapidity
		TH2D *fThetaVsPhi; //! particle azimutal angle vs. polar angle
		TH2D *fCentralityVsN; //! collision centrality vs. number of contributors
		TH2D *fDalitzPlot; //! Dalitz Plot for kaon 3-body decay

		// histograms for the 'Vertex' subfolder

		TH2D *fArmenterosPodolansky; //! Armenteros-Podolansky plot for V-events

		// N-tuples for the 'Vertex' subfolder

		int fNDaughters;
		int fVertexCharge;
		int fVertexProng;
		double fVertexPt;
		double fChi2;
		double fAlpha;
		double fQt;
		double fVertexRadius;
		double fCPA;
		double fDCAV0Daughters;
		double fDCAtoPrimaryVertex;
		double fPositiveDCA;
		double fNegativeDCA;
		double fDecayLength;

		// Global Cuts
		static constexpr double fZvertexCut = 10; //! maximum primary vertex Z-coordinate [cm]
		
		// Track Cuts
		static constexpr double fProtonSigmaCut = 3; //! maximum number of standard deviations from proton signal in TPC

		// Vertex Cuts
		static constexpr double fMaxVertexPt = 10; //! maximum vertex transverse momentum [GeV]
		static constexpr double fMaxChi2 = 4; //! maximum vertex chi^2 
		static constexpr double fMinCPA = 0.95; //! maximum cosine of angle from interaction point to decay vertex 
		static constexpr double fMaxQt = 1; //! maximum Armenteros-Podolansky plot transverse momentum [GeV]

		static constexpr double fMaxDCAV0Daughters = 2; //! maximum DCA between daughter tracks [cm]
		static constexpr double fMaxDCAtoPrimaryVertex = 30; //! maximum DCA between primary and decay verteces [cm]
		static constexpr double fMinDCA = 0.01; //! minimum DCA from each daughter track to primary vertex [cm]
		static constexpr double fMinDecayLength = 0.01; //! minimum decay length [cm]
};
#endif