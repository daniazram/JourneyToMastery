#include "Modifier_SimpleMovement.h"
#include "TDPlayerMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

FMovementData Modifier_SimpleMovement::Execute(float DeltaTime)
{
	FMovementData Data; Data.Valid = true;
	if(PlayerVisual == nullptr)
	{
		Data.Valid = false;
		return Data;
	}
	
	FRotator TargetRotation = PlayerVisual->GetComponentRotation();
	if(!Input.IsNearlyZero())
	{
		TargetRotation = Input.Rotation();
		TargetRotation.Pitch = 0; 
	}

	FRotator FinalRotation = UKismetMathLibrary::RLerp(PlayerVisual->GetComponentRotation(), TargetRotation, 20 * DeltaTime, true);
	Data.Rotation = FinalRotation;//PlayerVisual->SetWorldRotation(FinalRotation);
	
	FVector PlayerPos = Player->GetActorLocation();
	FVector Direction = PlayerVisual->GetForwardVector();
	Velocity = Direction * Speed * DeltaTime;
	
	Data.Location = PlayerPos + Velocity;//Player->SetActorLocation(PlayerPos + Velocity);
	return Data;	
}