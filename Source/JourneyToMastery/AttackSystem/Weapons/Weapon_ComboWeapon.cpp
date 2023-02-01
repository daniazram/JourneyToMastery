#include "Weapon_ComboWeapon.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "JourneyToMastery/AttackSystem/AttackSystemComponent.h"

void AWeapon_ComboWeapon::Tick(float DeltaTime)
{
}

void AWeapon_ComboWeapon::Init(UAttackSystemComponent* ParentComp, USkeletalMeshComponent* UserMeshComp)
{
	Super::Init(ParentComp, UserMeshComp);
	AnimInstance = UserMeshComp->GetAnimInstance();
}

void AWeapon_ComboWeapon::Attack()
{
	Super::Attack();
	if(!IsEquipped)
	{
		return;
	}

	if(Combo && !Combo->IsRunning)
	{
		Combo->Run();
		AttackSystem->UpdateAttackState(true);
	}
}

void AWeapon_ComboWeapon::BindInput(UEnhancedInputComponent* InputComp)
{
	Super::BindInput(InputComp);

	const int ComboLength = Combo->ComboList.Num();
	for(int i=0; i<ComboLength; i++)
	{
		FComboItem ComboItem = Combo->ComboList[i];
		InputComp->BindAction(ComboItem.InputAction, ETriggerEvent::Triggered, Combo, &UComboBase::OnInputUpdated);
	}
}

void AWeapon_ComboWeapon::AssignComboInstance_Implementation(UComboBase* ComboInstance)
{
	Combo = ComboInstance;
	Combo->Init(AnimInstance);
	Combo->ComboEndDelegate.AddUObject(this, &ThisClass::OnComboEnded);
}

void AWeapon_ComboWeapon::OnComboWindowUpdated_Implementation(bool Value)
{
	Combo->OnComboWindowUpdated(Value);
}

void AWeapon_ComboWeapon::OnInterruptStateUpdated_Implementation(bool Value)
{
	Combo->OnComboInterruptStateUpdated(Value);
}

void AWeapon_ComboWeapon::OnComboEnded()
{
	AttackSystem->UpdateAttackState(false);
	Combo->ComboEndDelegate.Remove(ComboEndDelegateHandle);
}