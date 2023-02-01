#pragma once

#include "CoreMinimal.h"
#include "UEGASCharacter.h"
#include "UEHeroCharacter.generated.h"

class AWeaponBase;
class UHealthComponent;
class UAttackSystemComponent;

UCLASS()
class JOURNEYTOMASTERY_API AUEHeroCharacter : public AUEGASCharacter
{
	GENERATED_BODY()
#pragma region Properties
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttackSystemComponent* AttackSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

protected:

	USceneComponent* WeaponSlot;

#pragma endregion Properties 

#pragma region Methods
	
public:

	AUEHeroCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponEquipped(AWeaponBase* Weapon);
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponHolstered(AWeaponBase* Weapon);

protected:

	void OnWeaponChanged(AWeaponBase* Weapon, bool equipped);

#pragma endregion Methods	
};
