#include "Modifier_CardinalMove.h"

#include "TDPlayerMovementComponent.h"

FMovementData Modifier_CardinalMove::Execute(float DeltaTime)
{
	FMovementData Data; Data.Valid = true;
	if(PlayerVisual == nullptr)
	{
		Data.Valid = false;
		return Data;
	}

	FVector HitLocation = GetMouseHitLocation();
	FVector Direction = HitLocation - Player->GetActorLocation(); Direction.Normalize();
	FRotator CurrentRotation = PlayerVisual->GetComponentRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, Direction.Rotation().Yaw, CurrentRotation.Roll);
	Data.Rotation = TargetRotation;//PlayerVisual->SetWorldRotation(TargetRotation);
	
	FVector PlayerPos = Player->GetActorLocation();
	FVector MoveDirection = Input * Speed * DeltaTime * Input.Size();
	
	if(Input.Size() > 0)
	{
		HeadingDirection.X = FVector::DotProduct(Input , PlayerVisual->GetRightVector());
		HeadingDirection.Y = FVector::DotProduct(Input, PlayerVisual->GetForwardVector());
	}
	else
	{
		HeadingDirection = FVector::ZeroVector;
	}
	
	Data.Location = PlayerPos + MoveDirection;//Player->SetActorLocation(PlayerPos + MoveDirection);
	return Data;
}

FVector Modifier_CardinalMove::GetMouseHitLocation()
{
	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;
	Controller->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FVector LineTraceEnd = WorldLocation + (WorldDirection * 10000);
	
	bool IsHit = Player->GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, LineTraceEnd, ECC_WorldStatic);
	if(IsHit)
	{
		return HitResult.Location;
	}

	return FVector::Zero();
}