#pragma once
#include "MovementComponentModifier.h"

class Modifier_Dash : public MovementComponentModifier
{
public:
	virtual void OnInputUpdate(FVector input) override;
	virtual void UpdateSpeed(float speedValue, float rotationValue) override;
	virtual struct FMovementData Execute(float DeltaTime) override;

	void InitDashData(FVector Direction, float Speed);
	void OverrideSpeed(float Speed);

private:

	bool DashInProgress;
	FVector DashDirection;
};
