#include "Modifier_Dash.h"

#include "TDPlayerMovementComponent.h"

void Modifier_Dash::OnInputUpdate(FVector input)
{
}

void Modifier_Dash::UpdateSpeed(float speedValue, float rotationValue)
{
}

FMovementData Modifier_Dash::Execute(float DeltaTime)
{
	MovementComponentModifier::Execute(DeltaTime);

	FMovementData Data; Data.Valid = true;
	if(!DashInProgress)
	{
		Data.Valid = false;
		return Data;
	}

	if(PlayerVisual == nullptr)
	{
		Data.Valid = false;
		return Data;
	}
	
	FRotator TargetRotation = PlayerVisual->GetComponentRotation();
	if(!DashDirection.IsNearlyZero())
	{
		TargetRotation = DashDirection.Rotation();
		TargetRotation.Pitch = 0; 
	}

	FRotator FinalRotation = TargetRotation;
	Data.Rotation = FinalRotation;//PlayerVisual->SetWorldRotation(FinalRotation);
	
	FVector PlayerPos = Player->GetActorLocation();
	Velocity = DashDirection * Speed * DeltaTime;
	
	Data.Location = PlayerPos+Velocity;//Player->SetActorLocation(PlayerPos + Velocity);

	return Data;
}

void Modifier_Dash::InitDashData(FVector Direction, float DashSpeed)
{
	DashInProgress = true;
	Speed = DashSpeed;
	DashDirection = Direction;
}

void Modifier_Dash::OverrideSpeed(float speed)
{
	Speed = speed;
}
