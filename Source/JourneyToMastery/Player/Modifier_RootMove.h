#pragma once

#include "MovementComponentModifier.h"

class Modifier_RootMove : public MovementComponentModifier
{
	FVector RootMotionPositionDelta;

public:
	
	virtual void OnInputUpdate(FVector input) override;
	virtual struct FMovementData Execute(float DeltaTime) override;
};
