#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterBase.generated.h"

class UInputAction;

UCLASS()
class JOURNEYTOMASTERY_API ACharacterBase : public APawn
{
	GENERATED_BODY()

#pragma region Properties
	
public:
	
	UPROPERTY(BlueprintReadOnly)
	UEnhancedInputComponent* InputComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTDPlayerMovementComponent* MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PlayerMesh;
	USceneComponent* PlayerVisualsRoot;
	APlayerController* PlayerController;
	
protected:

	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	const UInputAction* IA_HorizontalMove;
	UPROPERTY(EditAnywhere)
	const UInputAction* IA_VerticalMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* RoleWidget;

#pragma endregion Properties 

#pragma region Methods
	
public:	

	ACharacterBase();
	void OnMovementTicked(FVector Location, FRotator Rotation);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void HandleSprint(bool sprinting);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartRootSpeedExtraction();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndRootSpeedExtraction();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartRootPositionExtraction();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndRootPositionExtraction();

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION(Server, Unreliable)
	void ServerMove(FVector Location, FRotator Rotation);
	UFUNCTION(NetMulticast, Unreliable)
	void ClientMove(FVector Location, FRotator Rotation);

#pragma endregion Methods
};
