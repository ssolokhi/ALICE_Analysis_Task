/* Steering macros for the data analysis
Run with 'aliroot runAnalysis.C' 4 times:

- The first time: set gridTest = kTRUE to run test analysis by simulating GRID

- The second time: set gridTest = kFALSE to run task on GRID

- The third time: SetRunMode("terminate")

- The fourth time: SetMergeViaJDL(kFALSE) to merge all output files into one
*/
#include "AliAnalysisManager.h"
#include "AliAnalysisAlien.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"
#include "TSystem.h"

void runAnalysis() {

	Bool_t local = true;

	Bool_t gridTest = true;

//The following lines are used to determine the version of ROOT (5 or 6) compiler ot use
#if !defined (__CINT__) || !defined (__CLING__)
	gInterpreter->ProcessLine(".include $ROOTSYS/include");
	gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
#else
	gROOT->ProcessLine(".include $ROOTSYS/include");
	gROOT->ProcessLine(".include $ALICE_ROOT/include");
#endif

	AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisMyTask");
	AliAODInputHandler *aodHandler = new AliAODInputHandler();
	mgr->SetInputEventHandler(aodHandler);

//The following lines are used to determine the version of ROOT (5 or 6) compiler ot use
#if !defined (__CINT__) || !defined (__CLING__)
	gInterpreter->LoadMacro("AliAnalysisTaskMyTask.cxx++g");
	AliAnalysisTaskMyTask *task = reinterpret_cast<AliAnalysisTaskMyTask*>(gInterpreter->ExecuteMacro("AddMyTask.C"));
#else
	gROOT->LoadMacro("AliAnalysisTaskMyTask.cxx++g");
	gROOT->LoadMacro("AddMyTask.C");
	AliAnalysisTaskMyTask *task = AddMyTask();
#endif

	if (!mgr->InitAnalysis()) return 0x0;
	mgr->SetDebugLevel(0);
	mgr->PrintStatus();
	mgr->SetUseProgressBar(1, 100); // updated every 100 events

	if (local) {

		
		TChain *chain = new TChain("aodTree");
		if (gSystem->AccessPathName("AliAOD.root")) return 0x0;
		chain->Add("AliAOD.root"); // the local file to be analysed
		mgr->StartAnalysis("local", chain);

	} else {

	// the code lower allows to run on GRID
		AliAnalysisAlien *alienHandler = new AliAnalysisAlien();

		alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include"); // Load header files in GRID
		alienHandler->SetAdditionalLibs("AliAnalysisTaskMyTask.cxx AliAnalysisTaskMyTask.h");
		alienHandler->SetAnalysisSource("AliAnalysisTaskMyTask.cxx");
		alienHandler->SetAliPhysicsVersion("latest");
		//alienHandler->SetAPIVersion("V1.1x");
		alienHandler->SetGridDataDir("/alice/data/LHC16k"); // Path to input data
		alienHandler->SetDataPattern("*/pass3/AOD/*/AliAOD.root");
		alienHandler->SetRunPrefix("000"); // 000 for real data, nothing for MC data
		alienHandler->AddRunNumber(100000);
		alienHandler->SetSplitMaxInputFileNumber(30);
		alienHandler->SetExecutable("myTask.sh");
		alienHandler->SetTTL(10000);
		alienHandler->SetJDLName("myTask.jdl");
		alienHandler->SetOutputToRunNo(kTRUE); //do you want a subfolder for each runnumber output?
		alienHandler->SetKeepLogs(kTRUE);
		alienHandler->SetMergeViaJDL(kTRUE);
		alienHandler->SetMaxMergeStages(10); // 1 is enough for small amounts of data, otherwise it should be bigger
		alienHandler->SetGridWorkingDir("myWorkingDir");
		alienHandler->SetGridOutputDir("myOutputDir");

		mgr->SetGridHandler(alienHandler); // connecting the AliEn plugin to the manager

		if (gridTest) {

			alienHandler->SetNtestFiles(1);
			alienHandler->SetRunMode("test");
			mgr->StartAnalysis("grid");
			
		} else {

			alienHandler->SetRunMode("full");
			//alienHandler->SetRunMode("terminate"); // for output file merging
			mgr->StartAnalysis("grid");
		}

	}
}