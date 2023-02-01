#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "../../ComboSystem/ComboBase.h"
#include "JourneyToMastery/ComboSystem/ComboSystemInterface.h"
#include "Weapon_ComboWeapon.generated.h"

UCLASS()
class JOURNEYTOMASTERY_API AWeapon_ComboWeapon : public AWeaponBase, public IComboSystemInterface
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadonly)
	UComboBase* Combo; 

	virtual void Init(UAttackSystemComponent* ParentComp, USkeletalMeshComponent* UserMeshComp) override;
	virtual void Attack() override;
	virtual void BindInput(UEnhancedInputComponent* InputComp) override;

	virtual void AssignComboInstance_Implementation(UComboBase* ComboInstance) override;
	virtual void OnComboWindowUpdated_Implementation(bool Value) override;
	virtual void OnInterruptStateUpdated_Implementation(bool Value) override;

protected:

	UAnimInstance* AnimInstance;
	FDelegateHandle ComboEndDelegateHandle;
	
	virtual void Tick(float DeltaTime) override;
	void OnComboEnded();
};
