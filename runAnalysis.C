/* 
Run with 'aliroot runAnalysis.C' 4 times:

- The first time: set gridTest = true to run test analysis by simulating GRID
- The second time: set gridTest = false to run task on GRID
- The third time: SetRunMode("terminate")
- The fourth time: SetMergeViaJDL(kFALSE) to merge all output files into one
*/

#include "AliAnalysisManager.h"
#include "AliAnalysisAlien.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"
#include "TSystem.h"

void runAnalysis() {
	bool localRun = true;
	bool gridTest = true;

	gInterpreter->ProcessLine(".include $ROOTSYS/include");
	gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
	
	AliAnalysisManager *mgr = new AliAnalysisManager("MyTask");
	AliAODInputHandler *aodHandler = new AliAODInputHandler();
	mgr->SetInputEventHandler(aodHandler);

	// Load the TPC parameters
	TMacro PIDadd(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"));
	AliAnalysisTaskPIDResponse *PIDResponseTask = reinterpret_cast<AliAnalysisTaskPIDResponse*>(PIDadd.Exec());

	// Load Multiplicity calculation
	TMacro multSelection(gSystem->ExpandPathName("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C"));
    AliMultSelectionTask* multSelectionTask = reinterpret_cast<AliMultSelectionTask*>(multSelection.Exec());

	gInterpreter->LoadMacro("AliAnalysisTaskMyTask.cxx++g"); // 'g' flag for debugging 
	AliAnalysisTaskMyTask *task = reinterpret_cast<AliAnalysisTaskMyTask*>(gInterpreter->ExecuteMacro("AddMyTask.C"));

	if (!mgr->InitAnalysis()) return 0x0;
	mgr->SetDebugLevel(0);
	mgr->PrintStatus();
	mgr->SetUseProgressBar(1, 100); // updated every 100 events

	if (localRun) {
		TChain *chain = new TChain("aodTree");
		if (gSystem->AccessPathName("AliAOD.root")) return 0x0;
		chain->Add("AliAOD.root"); // the local file to be analysed
		mgr->StartAnalysis("local", chain);
	} else { // the code lower allows to run on GRID
		AliAnalysisAlien *alienHandler = new AliAnalysisAlien();

		alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include"); // Load header files in GRID
		alienHandler->SetAdditionalLibs("AliAnalysisTaskMyTask.cxx AliAnalysisTaskMyTask.h");
		alienHandler->SetAnalysisSource("AliAnalysisTaskMyTask.cxx");
		alienHandler->SetAliPhysicsVersion("vAN-20201115_JALIEN-1");

		alienHandler->SetGridDataDir("/alice/data/2016/LHC16k"); // Path to input data
		alienHandler->SetDataPattern("/pass3/AOD/*/AliAOD.root");
		alienHandler->SetRunPrefix("000"); // 000 for real data, nothing for MC data
		alienHandler->AddRunNumber(256504);

		alienHandler->SetSplitMaxInputFileNumber(30);
		alienHandler->SetExecutable("myTask.sh");
		alienHandler->SetTTL(10000);
		alienHandler->SetJDLName("myTask.jdl");

		alienHandler->SetOutputToRunNo(true); //do you want a subfolder for each runnumber output?
		alienHandler->SetKeepLogs(true);
		alienHandler->SetMergeViaJDL(true);
		alienHandler->SetMaxMergeStages(1); // 1 is enough for small amounts of data, otherwise it should be bigger
		
		alienHandler->SetGridWorkingDir("myWorkingDir"); // will be created in GRID; all code and .xml files will be copied there
		alienHandler->SetGridOutputDir("myOutputDir"); // result containing dir, subfolder of the previous one

		mgr->SetGridHandler(alienHandler); // connecting the AliEn plugin to the manager
		if (gridTest) {
			alienHandler->SetNtestFiles(1);
			alienHandler->SetRunMode("test");
			mgr->StartAnalysis("grid");
		} else {
			alienHandler->SetRunMode("full");
			mgr->StartAnalysis("grid");
		}
	}
}