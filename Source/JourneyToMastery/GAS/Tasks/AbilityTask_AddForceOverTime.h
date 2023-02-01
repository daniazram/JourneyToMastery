#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "JourneyToMastery/Player/TD_Player.h"
#include "AbilityTask_AddForceOverTime.generated.h"

class Modifier_Dash;
class AGASCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddConstantForceDelegate);

UCLASS()
class JOURNEYTOMASTERY_API UAbilityTask_AddForceOverTime : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FAddConstantForceDelegate OnFinish;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_AddForceOverTime* AddConstantForceForDuration
	(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		float Force, 
		float Duration,
		UCurveFloat* Curve,
		FVector WorldDirection
	);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
protected:

	AGASCharacter* Player;
	
	UPROPERTY(Replicated)
	FVector WorldDirection;
	UPROPERTY(Replicated)
	float Force;
	UPROPERTY(Replicated)
	float Duration;
	UPROPERTY()
	UCurveFloat* Curve;
	
	float Elapsed;
	Modifier_Dash* DashModifier;
};
