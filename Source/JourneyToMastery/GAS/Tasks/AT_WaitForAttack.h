#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "JourneyToMastery/ComboSystem/ComboBase.h"
#include "AT_WaitForAttack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoParamAction);

UCLASS()
class JOURNEYTOMASTERY_API UAT_WaitForAttack : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FNoParamAction OnCompleted;
	FDelegateHandle ComboEndDelegateHandle;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_WaitForAttack* WaitForCombo
	(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UComboBase* ComboInstance
	);

	virtual void Activate() override;
	
private:

	UComboBase* Combo;

	void OnComboEnded();
};
