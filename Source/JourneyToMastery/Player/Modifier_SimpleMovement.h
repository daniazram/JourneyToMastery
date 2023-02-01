#pragma once
#include "MovementComponentModifier.h"

class Modifier_SimpleMovement : public MovementComponentModifier
{
public:

	virtual struct FMovementData Execute(float DeltaTime) override;
};
