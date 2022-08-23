/* Copyright(c) 2022, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

// Definition of prototypes of all methods that are to be implemented in the ALICE Run 2 analysis script

#ifndef AliAnalysisTaskMyTask_H // include guard (to avoid double inclusion)
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"
#include "AliPIDResponse.h"

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

		// Called at the end of an analysis task
		virtual void Terminate(Option_t *option);

	private:

		/*Adding class members; the !<! tags are to be attached to ALL RUN-TIME
		 RESOLVED members for ROOT to generate documentation correctly*/

		AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); //copy constructor, not implemented
		AliAnalysisTaskMyTask &operator=(const AliAnalysisTaskMyTask&);

		/* ClassDef C preproccessor, to be used for classes derived from TObject.
		The '\cond' tags are for generating proper documentation */

		/// \cond CLASSDEF
		ClassDef(AliAnalysisTaskMyTask, 1); // the number must be changed after ANY class modification
		/// \endcond

		AliAODEvent *fAOD; //!<! input event
		TList *fOutputList; //!<!
		TH1F *fHistPt; //!<!
		TH1F *fPV0ZPos; //!<!
		AliPIDResponse *fPIDResponse; //!<!
		TH2F *fTPCResponse; //!<!
		TH1F *fNsigProton; //!<!
};
#endif