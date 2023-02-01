#pragma once

#include "CoreMinimal.h"
#include "GASCharacter.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UGameplayEffect;
class AWeaponBase;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitDelegate, AActor*, Hitter, FHitResult, HitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOURNEYTOMASTERY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintAssignable)
	FHitDelegate HitTakenEvent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead;
	
protected:
	
	UAbilitySystemComponent* ASC;
	const UPlayerAttributeSet* AttribSet; 
	
	void ApplyEffectToSelf(UGameplayEffect* Effect, int level);
	void OnHealthValueChanged(const FOnAttributeChangeData& CallbackData);

public:	

	UHealthComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void Init(UAbilitySystemComponent* InASC);
	UFUNCTION(BlueprintCallable)
	void OnHit(AWeaponBase* Weapon, AActor* Hitter, FHitResult HitResult);
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(UGameplayEffect* DamageEffect, int level);
	UFUNCTION(BlueprintCallable)
	void Die();
	
};
