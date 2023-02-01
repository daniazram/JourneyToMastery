#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "JourneyToMastery/Player/TD_Player.h"
#include "AbilityTask_GetMouseHitLocation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOutputDelegate, FVector, HitLoaction);

UCLASS()
class JOURNEYTOMASTERY_API UAbilityTask_GetMouseHitLocation : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FOutputDelegate OnUpdate;
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE", World = "WorldContextObject"))
	static UAbilityTask_GetMouseHitLocation* GetHitLocation
	(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		APlayerController* PlayerController
	);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

protected:

	FVector LineTraceEnd;
	FVector WorldLocation;
	FVector WorlDirection;
	FHitResult HitResult;
	APlayerController* PlayerController;
};
