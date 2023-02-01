#include "WeaponBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "JourneyToMastery/AttackSystem/AttackSystemComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	VisualRoot = CreateDefaultSubobject<USceneComponent>("VisualRoot");
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	Mesh->SetupAttachment(VisualRoot);

	IsEquipped = false;
	IsActive = false;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ASC->AbilityActivatedCallbacks.Remove(AbilityActiveHandle);
	ASC->AbilityEndedCallbacks.Remove(AbilityEndHandle);
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::OnAbilityActivated(UGameplayAbility* Ability)
{
	if(Ability->AbilityTags == AbilityTag)
	{
		IsAbilityRunning = true;
	}
}

void AWeaponBase::OnAbilityEnded(UGameplayAbility* Ability)
{
	if(Ability->AbilityTags == AbilityTag)
	{
		IsAbilityRunning = false;
	}
}

void AWeaponBase::Init(UAttackSystemComponent* ParentComp, USkeletalMeshComponent* UserMeshComp)
{
	AttackSystem = ParentComp;
	Wielder = AttackSystem->GetOwner();
	ASC = Cast<IAbilitySystemInterface>(ParentComp->GetOwner())->GetAbilitySystemComponent();

	AbilityActiveHandle = ASC->AbilityActivatedCallbacks.AddUObject(this, &AWeaponBase::OnAbilityActivated);
	AbilityEndHandle = ASC->AbilityEndedCallbacks.AddUObject(this, &AWeaponBase::OnAbilityEnded);
	ActorsToIgnore = {ParentComp->GetOwner()};
}

void AWeaponBase::Attack()
{
	if(IsAbilityRunning)
	{
		return;
	}

	IsAbilityRunning = ASC->TryActivateAbilitiesByTag(AbilityTag);
}

void AWeaponBase::SetActive(bool State)
{
	IsActive = State;
	SetActorHiddenInGame(!State);
	SetActorEnableCollision(State);
	SetActorTickEnabled(State);
}

void AWeaponBase::BindInput(UEnhancedInputComponent* InputComp)
{
}

void AWeaponBase::TraceHit()
{
	FHitResult HitResult; 
	bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetTraceStartPoint(), GetTraceEndPoint(), 15.f, TraceObjectData->ObjectTypes, false, ActorsToIgnore,
	EDrawDebugTrace::Type::None, HitResult, true);

	FVector SwingDirection = this->GetActorLocation() - PreviousSwingPosition;
	if(Hit)
	{
		UHealthComponent* HitComp = Cast<UHealthComponent>(HitResult.GetActor()->GetComponentByClass(UHealthComponent::StaticClass()));
		if(HitComp == nullptr || HitComp == CurrentHitObject)
		{
			return;
		}

		CurrentHitObject = HitComp;
		CurrentHitObject->OnHit(this, Wielder, HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 4, FColor::Red, false, 5.f);
		DrawDebugDirectionalArrow(GetWorld(), HitResult.ImpactPoint,HitResult.ImpactPoint + SwingDirection, 2,  FColor::Red, true, -1, 0, 1);
	}

	PreviousSwingPosition = this->GetActorLocation();
}

void AWeaponBase::ResetTrace()
{
	CurrentHitObject = nullptr;
	PreviousSwingPosition = FVector::Zero();
}

void AWeaponBase::TryEquipping()
{
	UAnimInstance* AnimInstance = AttackSystem->PlayerMesh->GetAnimInstance();
	MontageEndDelegate.BindUObject(this, &AWeaponBase::OnMontageEnded);
	
	AnimInstance->Montage_Play(EquipMontage, 1.5f);
	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, EquipMontage);

	OnEquip();
}

void AWeaponBase::TryHolstering()
{
	IsEquipped = false;
	UAnimInstance* AnimInstance = AttackSystem->PlayerMesh->GetAnimInstance();
	MontageEndDelegate.BindUObject(this, &AWeaponBase::OnMontageEnded);
	
	AnimInstance->Montage_Play(HolsterMontage, 1.5f);
	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, HolsterMontage);

	OnHolster();
}

void AWeaponBase::OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if(Montage == EquipMontage)
	{
		IsEquipped = true;
	}

	MontageEndDelegate.Unbind();
}

FVector AWeaponBase::GetTraceStartPoint()
{
	return Mesh->GetSocketLocation(FName("socket-trace-start"));
}

FVector AWeaponBase::GetTraceEndPoint()
{
	return Mesh->GetSocketLocation(FName("socket-trace-end"));
}


