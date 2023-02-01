#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "LatentActions.h"

class JOURNEYTOMASTERY_API FDoOverTimeLatentAction : public FPendingLatentAction
{
public:
	
	FDoOverTimeLatentAction(float Duration, std::function<void(float)> UpdateCallback, std::function<void()> FinishedCallback);
	~FDoOverTimeLatentAction();

	virtual void UpdateOperation(FLatentResponse& response) override;

private:

	float Duration;
	float TimeElapsed;
	std::function<void(float)> UpdateCallback;
	std::function<void()> FinishedCallback;
};
