#include "HeroCharacter.h"

#include "AbilitySystemComponent.h"
#include "HealthComponent.h"
#include "JourneyToMastery/AttackSystem/AttackSystemComponent.h"
#include "JourneyToMastery/GAS/PlayerAttributeSet.h"

AHeroCharacter::AHeroCharacter() : AGASCharacter()
{
	WeaponSlot = CreateDefaultSubobject<USceneComponent>("WeaponSlot");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	AttackSystem = CreateDefaultSubobject<UAttackSystemComponent>("AttackSystemComponent");

	WeaponSlot->SetupAttachment(PlayerVisualsRoot, FName("socket-weapon"));
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	AttackSystem->Init(WeaponSlot, PlayerMesh);
	AttackSystem->WeaponChangeDelegate.BindUObject(this, &AHeroCharacter::OnWeaponChanged);
	HealthComponent->Init(ASC);
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(InputComp != nullptr)
	{
		AttackSystem->BindInput(InputComp, WeaponSlot, PlayerMesh);
	}
}

void AHeroCharacter::OnWeaponChanged(AWeaponBase* Weapon, bool equipped)
{
	if(equipped)
	{
		OnWeaponEquipped(Weapon);
	}
	else
	{
		OnWeaponHolstered(Weapon);
	}
}


