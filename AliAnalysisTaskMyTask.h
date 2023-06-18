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
		 RESOLVED members for ROOT to generate documentation correctly*/

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
		AliAODv0 *fAODv0; //! primary vertex
		AliPIDResponse *fPIDResponse; //! particle identification response
		AliMCEvent *fMCEvent; //! Monte-Carlo event flag

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
		TH2D *fArmenterosPodolansky; //! Armenteros-Podolansky plot for V-events
		TH2D *fDalitzPlot; //! Dalitz Plot for kaon 3-body decay

		const double fZvertexCut = 10; //! maximum primary vertex Z-coordinate [cm]
		const double fProtonSigmaCut = 3; //! maximum number of standard deviations from proton signal in TPC

		double alpha; // Armenteros-Podolansky plot asymmetry parameter
		double qT; // Armenteros-Podolansky plot kinematic parameter

};
#endif