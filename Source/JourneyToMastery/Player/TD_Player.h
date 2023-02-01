#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "HealthComponent.h"
#include "../GAS/PlayerAttributeSet.h"
#include "JourneyToMastery/AttackSystem/AttackSystemComponent.h"
#include "TD_Player.generated.h"

class UInputAction;
class UGameplayAbility;
class UGameplayEffect;
class UPlayerAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChange, FGameplayAttribute, Attribute, float, NewValue);

UCLASS()
class JOURNEYTOMASTERY_API ATD_Player : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnHealthValueChanged;
	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChange OnStaminaValueChanged;

	UPROPERTY(BlueprintReadOnly)
	UEnhancedInputComponent* InputComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTDPlayerMovementComponent* MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PlayerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttackSystemComponent* AttackSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	UPlayerAttributeSet* AttributeSet;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

protected:

	USceneComponent* Root;
	USceneComponent* PlayerVisualsRoot;
	USceneComponent* WeaponSlot;
	FDelegateHandle HealthChangeDelegateHandle;
	FDelegateHandle StaminaChangeDelegateHandle;
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	const UInputAction* IA_HorizontalMove;
	UPROPERTY(EditAnywhere)
	const UInputAction* IA_VerticalMove;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void OnWeaponChanged(AWeaponBase* Weapon, bool equipped);
	void OnHealthAttributeValueChanged(const FOnAttributeChangeData& CallbackData);
	void OnStaminaAttributeValueChanged(const FOnAttributeChangeData& CallbackData);

public:	

	ATD_Player();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();

	UFUNCTION(BlueprintCallable)
	void ToggleAiming(bool value);
	UFUNCTION(BlueprintCallable)
	void HandleSprint(bool sprinting);
	UFUNCTION(BlueprintCallable)
	void HandleAiming(FVector hitLocation);
	UFUNCTION(BlueprintImplementableEvent)
	void WeaponEquippedEvent(AWeaponBase* Weapon);
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponSheathedEvent(AWeaponBase* Weapon);
};
