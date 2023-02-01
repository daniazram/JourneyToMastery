#include "UEHeroCharacter.h"
#include "JourneyToMastery/AttackSystem/AttackSystemComponent.h"

AUEHeroCharacter::AUEHeroCharacter(const FObjectInitializer& ObjectInitializer) : AUEGASCharacter(ObjectInitializer)
{
	WeaponSlot = CreateDefaultSubobject<USceneComponent>("WeaponSlot");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	AttackSystem = CreateDefaultSubobject<UAttackSystemComponent>("AttackSystemComponent");

	WeaponSlot->SetupAttachment(GetRootComponent(), FName("socket-weapon"));
}

void AUEHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	AttackSystem->Init(WeaponSlot, GetMesh());
	AttackSystem->WeaponChangeDelegate.BindUObject(this, &AUEHeroCharacter::OnWeaponChanged);
	HealthComponent->Init(ASC);
}

void AUEHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(InputComp != nullptr)
	{
		AttackSystem->BindInput(InputComp, WeaponSlot, GetMesh());
	}
}

void AUEHeroCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if(AttackSystem && AttackSystem->GetIsAttacking())
	{
		return;
	}
	
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void AUEHeroCharacter::OnWeaponChanged(AWeaponBase* Weapon, bool equipped)
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