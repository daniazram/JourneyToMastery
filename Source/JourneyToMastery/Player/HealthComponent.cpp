#include "HealthComponent.h"

#include "AbilitySystemComponent.h"
#include "JourneyToMastery/AttackSystem/Weapons/WeaponBase.h"
#include "JourneyToMastery/GAS/PlayerAttributeSet.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::Init(UAbilitySystemComponent* InASC)
{
	ASC = InASC;
	AttribSet = Cast<UPlayerAttributeSet>(ASC->GetAttributeSet(UPlayerAttributeSet::StaticClass()));
	ASC->GetGameplayAttributeValueChangeDelegate(AttribSet->GetHealthAttribute()).AddUObject(this, &UHealthComponent::OnHealthValueChanged);
}

void UHealthComponent::ApplyDamage(UGameplayEffect* DamageEffect, int level)
{
	if(IsDead)
	{
		return;
	}
	
	ApplyEffectToSelf(DamageEffect, level);
}

void UHealthComponent::OnHit(AWeaponBase* Weapon, AActor* Hitter, FHitResult HitResult)
{
	if(IsDead)
	{
		if(HitTakenEvent.IsBound())
		{
			HitTakenEvent.Broadcast(Hitter, HitResult);
		}
		return;
	}
	
	ApplyDamage(Weapon->DamageEffect.GetDefaultObject(), 1);
	if(HitTakenEvent.IsBound())
	{
		HitTakenEvent.Broadcast(Hitter, HitResult);
	}
}

void UHealthComponent::ApplyEffectToSelf(UGameplayEffect* Effect, int level)
{
	if(ASC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("ASC not found"));
		return;		
	}
	
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(ASC->GetOwnerActor());

	ASC->ApplyGameplayEffectToSelf(Effect, level, EffectContext);
}

void UHealthComponent::OnHealthValueChanged(const FOnAttributeChangeData& CallbackData)
{
	if(IsDead)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(449, 2.f, FColor::Green, FString::Printf(TEXT("Health = %f"), CallbackData.NewValue));
	if(CallbackData.NewValue <= 0)
	{
		Die();
	}
}

void UHealthComponent::Die()
{
	IsDead = true;
}


