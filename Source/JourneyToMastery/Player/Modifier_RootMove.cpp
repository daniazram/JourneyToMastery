#include "Modifier_RootMove.h"

#include "TDPlayerMovementComponent.h"

void Modifier_RootMove::OnInputUpdate(FVector input)
{
	RootMotionPositionDelta = input;
}

FMovementData Modifier_RootMove::Execute(float DeltaTime)
{
	FMovementData Data; Data.Valid = true;
	if(RootMotionPositionDelta.IsNearlyZero())
	{
		Data.Valid = false;
		return Data;
	}

	float VisualModelAngle = PlayerVisual->GetRelativeRotation().Yaw;
	FVector PlayerPos = Player->GetActorLocation();
	FVector RelativeDeltaPosition = RootMotionPositionDelta.RotateAngleAxis(VisualModelAngle - 90, FVector::ZAxisVector);

	Data.Location = PlayerPos+RelativeDeltaPosition;//Player->SetActorLocation(PlayerPos + RelativeDeltaPosition);
	return Data;
}