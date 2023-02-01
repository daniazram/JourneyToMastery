#pragma once
#include "MovementComponentModifier.h"

class Modifier_CardinalMove : public MovementComponentModifier
{
	
public:

	virtual struct FMovementData Execute(float DeltaTime) override;

private:

	FVector GetMouseHitLocation();
};
