#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JourneyToMastery/DataAssets/WeaponData.h"
#include "Weapons/WeaponBase.h"
#include "AttackSystemComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FAttackSystemDelegate, AWeaponBase*, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOURNEYTOMASTERY_API UAttackSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttackSystemComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoHolsterTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UWeaponData*> WeaponsData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeaponBase* CurrentWeapon;
	FAttackSystemDelegate WeaponChangeDelegate;
	UPROPERTY(BlueprintReadOnly)
	USkeletalMeshComponent* PlayerMesh;

protected:

	bool Initialized;
	float WeaponIdleTime;
	TArray<AWeaponBase*> Weapons;
	TMap<UInputAction*, EWeaponType> InputToWeaponMap;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(BlueprintCallable)
	void OnInputUpdated(const FInputActionInstance& ActionInstance);

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init(USceneComponent* WeaponSlot, USkeletalMeshComponent* PlayerMeshComp);
	void BindInput(UEnhancedInputComponent* InputComp, USceneComponent* WeaponSlot, USkeletalMeshComponent* PlayerMeshComp);
	void AutoHolsterWeapon();
	
	UFUNCTION(BlueprintCallable)
	bool SelectWeaponFromType(EWeaponType Type);
	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking() { return IsAttacking; }
	UFUNCTION(BlueprintCallable)
	void UpdateAttackState(bool InState) { IsAttacking = InState; }
};
