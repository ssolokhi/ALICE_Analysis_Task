// Creating Analysis task class instance

AliAnalysisTaskMyTask *AddMyTask(TString name = "name") {

	AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

	if (!mgr || !mgr->GetInputEventHandler()) return;

	TString fileName = AliAnalysisManager::GetCommonFileName();
	fileName += ":MyTask"; // creates a subfolder

	AliAnalysisTaskMyTask *task = new AliAnalysisTaskMyTask(name.Data());
	mgr->AddTask(task);

	mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
	mgr->ConnectOutput(task, 1, mgr->CreateContainer("MyOutputContainer", TList::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));

	return task;
}