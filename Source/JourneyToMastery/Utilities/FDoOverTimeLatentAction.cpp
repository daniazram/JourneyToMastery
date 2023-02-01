#include "FDoOverTimeLatentAction.h"

FDoOverTimeLatentAction::FDoOverTimeLatentAction(float Duration, std::function<void(float)> UpdateCallback, std::function<void()> FinishedCallback)
												: Duration(Duration), UpdateCallback(UpdateCallback), FinishedCallback(FinishedCallback){}

FDoOverTimeLatentAction::~FDoOverTimeLatentAction()
{
	UpdateCallback = nullptr;
	FinishedCallback = nullptr;
}

void FDoOverTimeLatentAction::UpdateOperation(FLatentResponse& response)
{
	TimeElapsed += response.ElapsedTime();;
	if(UpdateCallback != nullptr)
	{
		UpdateCallback(TimeElapsed/Duration);
	}

	response.DoneIf(TimeElapsed >= Duration);
	if(TimeElapsed >= Duration && FinishedCallback != nullptr)
	{
		FinishedCallback();
	}
}
