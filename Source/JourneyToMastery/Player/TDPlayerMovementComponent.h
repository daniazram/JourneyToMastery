#pragma once

#include "CoreMinimal.h"
#include "Modifier_CardinalMove.h"
#include "Modifier_RootMove.h"
#include "Modifier_SimpleMovement.h"
#include "MovementComponentModifier.h"
#include "TD_Player.h"
#include "Components/ActorComponent.h"
#include "TDPlayerMovementComponent.generated.h"

class ACharacterBase;

USTRUCT()
struct FMovementData
{
	GENERATED_BODY();
	bool Valid;
	FVector Location;
	FRotator Rotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOURNEYTOMASTERY_API UTDPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

protected: 	

	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Sprinting;
	UPROPERTY(BlueprintReadWrite)
	bool Aiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	float InputSize;
	UPROPERTY(BlueprintReadWrite, Replicated)
	float Horizontal;
	UPROPERTY(BlueprintReadWrite, Replicated)
	float Vertical;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FVector HeadingDirection;
	UPROPERTY(BlueprintReadOnly)
	FVector Input;
	float Yaw, Pitch;

	UPROPERTY(Replicated)
	AActor* Player;
	UPROPERTY(Replicated)
	ACharacterBase* PlayerCharacter;
	APlayerController* PlayerController;
	UPROPERTY(Replicated)
	USceneComponent* PlayerVisualRoot;
	MovementComponentModifier* MovementModifier;
	Modifier_RootMove* RootMoveModifier;
	Modifier_SimpleMovement* SimpleMoveModifier;
	Modifier_CardinalMove* CardinalMoveModifier;
	TArray<MovementComponentModifier*>* OverridingModifiers;
	
	void OnMovementInputUpdate(float horizontal, float vertical);
	void ApplyMovementData(FMovementData InData);
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	UPROPERTY(BlueprintReadWrite)
	FVector RootMotionPositionDelta;
	
	UTDPlayerMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeModifierTo(MovementComponentModifier* Modifier);
	void AddOverridingModifier(MovementComponentModifier* Modifier);
	void RemoveOverridingModifier(MovementComponentModifier* Modifier);
	void Init(ACharacterBase* InChar, USceneComponent* visualRoot);
	void HandleAiming(FVector hitLocation);

	UFUNCTION(Server, Unreliable)
	void ServerMove(FVector Location, FRotator Rotation);
	UFUNCTION(NetMulticast, Unreliable)
	void ClientMove(FVector Location, FRotator Rotation);
	UFUNCTION(BlueprintCallable)
	void OnSprintToggle(bool value);
	UFUNCTION(BlueprintCallable)
	void OnAimToggle(bool value);
	UFUNCTION(BlueprintCallable)
	void OnRotationInputUpdate(float yaw, float pitch);
	UFUNCTION(BlueprintCallable)
	void AddHorizontalMoveInput(float value);
	UFUNCTION(BlueprintCallable)
	void AddVerticalMoveInput(float value);
};
