#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "JourneyToMastery/Enums.h"
#include "InputBoundGameplayAbility.generated.h"

UCLASS()
class JOURNEYTOMASTERY_API UInputBoundGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UInputBoundGameplayAbility();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability Input")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
};
