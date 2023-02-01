#include "MovementComponentModifier.h"
#include "TDPlayerMovementComponent.h"

void MovementComponentModifier::Init(AActor* player, USceneComponent* playerVisual)
{
	Initialized = true;
	this->Player = player;
	this->PlayerVisual = playerVisual;
	Controller = Cast<APlayerController>(Player->GetInstigatorController());
}

void MovementComponentModifier::Enter()
{
}

void MovementComponentModifier::Exit()
{
	Input = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;
	HeadingDirection = FVector::ZeroVector;
}

FMovementData MovementComponentModifier::Execute(float DeltaTime)
{
	FMovementData Data; Data.Valid = false;
	return Data;
}

void MovementComponentModifier::OnInputUpdate(FVector input)
{
	Input = input;
}

void MovementComponentModifier::UpdateSpeed(float speedValue, float rotationValue)
{
	Speed = speedValue;
}

FVector MovementComponentModifier::GetVelocity()
{
	return Velocity;
}

FVector MovementComponentModifier::GetHeadingDirection()
{
	return HeadingDirection;
}
