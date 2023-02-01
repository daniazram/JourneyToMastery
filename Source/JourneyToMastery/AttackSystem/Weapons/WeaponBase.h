#pragma once

#include "CoreMinimal.h"
#include "../../Enums.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "JourneyToMastery/DataAssets/TraceObjectTypeData.h"
#include "JourneyToMastery/Player/HealthComponent.h"
#include "WeaponBase.generated.h"

class UAttackSystemComponent;

UCLASS(Abstract)
class JOURNEYTOMASTERY_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsEquipped;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponSlot WeaponSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AbilityTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTraceObjectTypeData* TraceObjectData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimSetUnarmed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* EquipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HolsterMontage;
	UPROPERTY(BlueprintReadOnly)
	AActor* Wielder;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;
	
	AWeaponBase();

protected:

	bool IsAbilityRunning;
	FVector PreviousSwingPosition;
	UAbilitySystemComponent* ASC;
	UPROPERTY(BlueprintReadOnly)
	UAttackSystemComponent* AttackSystem;
	UHealthComponent* CurrentHitObject;
	FDelegateHandle AbilityActiveHandle;
	FDelegateHandle AbilityEndHandle;
	FOnMontageEnded MontageEndDelegate;
	
	UPROPERTY(BlueprintReadWrite)
	USceneComponent* VisualRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Mesh;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	void OnAbilityActivated(UGameplayAbility* Ability);
	void OnAbilityEnded(UGameplayAbility* Ability);
	void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);
	
public:	

	virtual void Init(UAttackSystemComponent* ParentComp, USkeletalMeshComponent* UserMeshComp);
	virtual void Attack();
	virtual void SetActive(bool State);
	virtual void TryEquipping();
	virtual void TryHolstering();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEquip();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHolster();
	UFUNCTION(BlueprintCallable)
	virtual void BindInput(UEnhancedInputComponent* InputComp);
	UFUNCTION(BlueprintCallable)
	virtual void TraceHit();
	UFUNCTION(BlueprintCallable)
	virtual void ResetTrace();
	UFUNCTION(BlueprintCallable)
	FVector GetTraceStartPoint();
	UFUNCTION(BlueprintCallable)
	FVector GetTraceEndPoint();
};