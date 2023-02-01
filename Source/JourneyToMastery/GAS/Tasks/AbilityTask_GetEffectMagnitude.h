#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_GetEffectMagnitude.generated.h"

UCLASS()
class JOURNEYTOMASTERY_API UAbilityTask_GetEffectMagnitude : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_GetEffectMagnitude* GetEffectModifierMagnitude
	(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		FGameplayEffectSpecHandle SpecHandle,
		float& Magnitude
	);

	virtual void Activate() override;

protected:
	
	FGameplayEffectSpecHandle SpecHandle;

	UPROPERTY()
	float Magnitude;
};
