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
		TList *fOutputList; //! List of output histograms
		AliAODVertex *fAODv0; //! primary vertex
		AliPIDResponse *fPIDResponse; //! particle identification response
		AliMCEvent *fMCEvent; //! Monte-Carlo event flag

		TH1D *fZvertex; //! Vertex z-coordinate 
		TH1D *fMCPDGCode; //! Monte-Carlo generated particles' PDG code

		TH2D *fTPCResponse; //! TPC response 
		TH2D *fProtonResponse; //! Pion TPC response 
		TH2D *fTrackPtvsMass; //! Event transverse momentum
		TH2D *fThetaVsEta; //! particle azimutal angle vs. pseudorapidity

		const double fZvertexCut = 10; //! maximum primary vertex Z-coordinate [cm]
		const double fProtonSigmaCut = 1; //! maximum number of standard deviations from proton signal in TPC
};
#endif