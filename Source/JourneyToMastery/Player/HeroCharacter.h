#pragma once

#include "CoreMinimal.h"
#include "GASCharacter.h"
#include "JourneyToMastery/GAS/PlayerAttributeSet.h"
#include "HeroCharacter.generated.h"

class AWeaponBase;
class UHealthComponent;
class UAttackSystemComponent;

UCLASS()
class JOURNEYTOMASTERY_API AHeroCharacter : public AGASCharacter
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

	AHeroCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponEquipped(AWeaponBase* Weapon);
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponHolstered(AWeaponBase* Weapon);

protected:

	void OnWeaponChanged(AWeaponBase* Weapon, bool equipped);
};

#pragma endregion Methods