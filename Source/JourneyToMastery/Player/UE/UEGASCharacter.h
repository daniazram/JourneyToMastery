#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "UECharacterBase.h"
#include "../GASCharacter.h"
#include "JourneyToMastery/GAS/PlayerAttributeSet.h"
#include "UEGASCharacter.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UPlayerAttributeSet;
class UAbilitySystemComponent;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChanged, FGameplayAttribute, Attribute, float, NewValue);

UCLASS()
class JOURNEYTOMASTERY_API AUEGASCharacter : public AUECharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChanged OnHealthValueChanged;
	UPROPERTY(BlueprintAssignable, Category = "Attribute callbacks")
	FAttributeChanged OnStaminaValueChanged;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* ASC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	UPlayerAttributeSet* AttributeSet;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	
protected:

	FDelegateHandle HealthChangeDelegateHandle;
	FDelegateHandle StaminaChangeDelegateHandle;
	
	void OnHealthAttributeValueChanged(const FOnAttributeChangeData& CallbackData);
	void OnStaminaAttributeValueChanged(const FOnAttributeChangeData& CallbackData);
	
public:

	AUEGASCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();
	
};
