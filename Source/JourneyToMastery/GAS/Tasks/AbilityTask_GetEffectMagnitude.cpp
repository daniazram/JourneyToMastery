#include "AbilityTask_GetEffectMagnitude.h"
#include "AbilitySystemComponent.h"

UAbilityTask_GetEffectMagnitude::UAbilityTask_GetEffectMagnitude(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = false;
}


UAbilityTask_GetEffectMagnitude* UAbilityTask_GetEffectMagnitude::GetEffectModifierMagnitude(UGameplayAbility* OwningAbility, FName TaskInstanceName, FGameplayEffectSpecHandle SpecHandle, float& Magnitude)
{
	UAbilityTask_GetEffectMagnitude* Task = NewAbilityTask<UAbilityTask_GetEffectMagnitude>(OwningAbility, TaskInstanceName);
	Task->SpecHandle = SpecHandle;
	Task->Magnitude = SpecHandle.Data.Get()->Modifiers[0].GetEvaluatedMagnitude();
	
	return Task;
}

void UAbilityTask_GetEffectMagnitude::Activate()
{
	Super::Activate();

	Magnitude = SpecHandle.Data.Get()->Modifiers[0].GetEvaluatedMagnitude();
}