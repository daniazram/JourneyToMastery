#include "AT_WaitForAttack.h"

UAT_WaitForAttack::UAT_WaitForAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = false;
}

UAT_WaitForAttack* UAT_WaitForAttack::WaitForCombo(UGameplayAbility* OwningAbility, FName TaskInstanceName, UComboBase* ComboInstance)
{
	UAT_WaitForAttack* Task = NewAbilityTask<UAT_WaitForAttack>(OwningAbility, TaskInstanceName);
	Task->Combo = ComboInstance;

	return Task;
}

void UAT_WaitForAttack::Activate()
{
	Super::Activate();
	if(Combo == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("No Combo Object Found"));
		return;
	}
	ComboEndDelegateHandle = Combo->ComboEndDelegate.AddUObject(this, &UAT_WaitForAttack::OnComboEnded);
}

void UAT_WaitForAttack::OnComboEnded()
{
	if(OnCompleted.IsBound())
	{
		OnCompleted.Broadcast();
	}

	Combo->ComboEndDelegate.Remove(ComboEndDelegateHandle);
}

