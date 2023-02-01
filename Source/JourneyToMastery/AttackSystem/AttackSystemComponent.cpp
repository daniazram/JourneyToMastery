#include "AttackSystemComponent.h"

#include "EnhancedInputComponent.h"
#include "JourneyToMastery/Player/TD_Player.h"

UAttackSystemComponent::UAttackSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackSystemComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UAttackSystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	WeaponChangeDelegate.Unbind();
}

void UAttackSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!Initialized || CurrentWeapon == nullptr)
	{
		return;
	}
	
	WeaponIdleTime += DeltaTime;
	if(WeaponIdleTime >= AutoHolsterTime)
	{
		AutoHolsterWeapon();	
	}
}

void UAttackSystemComponent::Init(USceneComponent* WeaponSlot, USkeletalMeshComponent* PlayerMeshComp)
{
	if(Initialized)
	{
		return;
	}

	PlayerMesh = PlayerMeshComp;
	Initialized = true;
	for(int i = 0; i < WeaponsData.Num(); i++)
	{
		UWeaponData* Data = WeaponsData[i];
		
		FTransform SocketTransform = WeaponSlot->GetComponentTransform();//PlayerMeshComp->GetSocketTransform("socket-weapon");
		AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(Data->WeaponPrefab, SocketTransform);
		Weapons.Add(Weapon); 

		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Weapon->Init(this, PlayerMeshComp);
		Weapon->AttachToComponent(PlayerMeshComp, Rules, FName("socket-weapon"));
		InputToWeaponMap.Add(Data->InputAction, Weapon->WeaponType);
	}

	SelectWeaponFromType(Weapons[0]->WeaponType);
}


void UAttackSystemComponent::BindInput(UEnhancedInputComponent* InputComp, USceneComponent* WeaponSlot, USkeletalMeshComponent* PlayerMeshComp)
{
	if(!Initialized)
	{
		Init(WeaponSlot, PlayerMeshComp);
	}
	
	for(int i = 0; i < WeaponsData.Num(); i++)
	{
		UWeaponData* Data = WeaponsData[i];
		InputComp->BindAction(Data->InputAction, ETriggerEvent::Triggered, this, &UAttackSystemComponent::OnInputUpdated);
	}
}

void UAttackSystemComponent::OnInputUpdated(const FInputActionInstance& ActionInstance)
{
	const EWeaponType Type = InputToWeaponMap[ActionInstance.GetSourceAction()];
	if(CurrentWeapon != nullptr && CurrentWeapon->WeaponType == Type)
	{
		WeaponIdleTime = 0;
		CurrentWeapon->Attack();
		return;
	}

	SelectWeaponFromType(Type);
}

bool UAttackSystemComponent::SelectWeaponFromType(EWeaponType Type)
{
	if(CurrentWeapon != nullptr && CurrentWeapon->WeaponType == Type)
	{
		return true;
	}

	for(int i=0; i<Weapons.Num(); i++)
	{
		if(Weapons[i]->WeaponType == Type)
		{
			CurrentWeapon = Weapons[i];
			CurrentWeapon->SetActive(true);
			CurrentWeapon->TryEquipping();
			WeaponChangeDelegate.ExecuteIfBound(CurrentWeapon, true);
			return true;
		}
	}

	return false;
}

void UAttackSystemComponent::AutoHolsterWeapon()
{
	if(CurrentWeapon == nullptr)
	{
		return;
	}

	WeaponIdleTime = 0.f;
	CurrentWeapon->SetActive(false);
	CurrentWeapon->TryHolstering();
	WeaponChangeDelegate.ExecuteIfBound(CurrentWeapon, false);
	CurrentWeapon = nullptr;
}
