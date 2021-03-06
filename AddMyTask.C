// Creating AliAnalysis task class instance

#include "AliVEvent.h"

AliAnalysisTaskMyTask *AddMyTask(TString name = "name") {

	AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

	if (!mgr || !mgr->GetInputEventHandler()) return 0x0;

	TString fileName = AliAnalysisManager::GetCommonFileName();
	fileName += ":MyTask"; // creates a subfolder

	AliAnalysisTaskMyTask *task = new AliAnalysisTaskMyTask(name.Data());
	if (!task) return 0x0;
	mgr->AddTask(task);
	task->SelectCollisionCandidates(AliVEvent::kINT7);
	mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
	mgr->ConnectOutput(task, 1, mgr->CreateContainer("MyOutputContainer", TList::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));

	return task;
}