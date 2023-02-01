#pragma once

class MovementComponentModifier
{
public:

	bool Initialized;
	
	virtual void Init(AActor* player, USceneComponent* playerVisual);
	virtual void Enter();
	virtual void Exit();
	virtual struct FMovementData Execute(float DeltaTime);
	virtual void OnInputUpdate(FVector input);
	virtual void UpdateSpeed(float speedValue, float rotationValue);
	virtual FVector GetVelocity();
	virtual FVector GetHeadingDirection();
	
protected:

	float Speed;
	FVector Input;
	FVector Velocity;
	FVector HeadingDirection;
	AActor* Player;
	APlayerController* Controller;
	USceneComponent* PlayerVisual;
};
