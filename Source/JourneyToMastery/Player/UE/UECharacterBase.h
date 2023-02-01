#pragma once

#include "CoreMinimal.h"
#include "UEMovementComponent.h"
#include "GameFramework/Character.h"
#include "UECharacterBase.generated.h"


UCLASS()
class JOURNEYTOMASTERY_API AUECharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Sprinting;
	UPROPERTY(BlueprintReadOnly)
	UEnhancedInputComponent* InputComp;
	UUEMovementComponent* MovementComponent;
	
protected:
	
	virtual void BeginPlay() override;

public:	

	AUECharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void HandleSprint(bool InSprinting);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartRootSpeedExtraction();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndRootSpeedExtraction();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartRootPositionExtraction();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndRootPositionExtraction();

};
