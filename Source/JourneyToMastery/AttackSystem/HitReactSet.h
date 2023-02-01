#pragma once
#include "JourneyToMastery/Enums.h"

#include "HitReactSet.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FHitReactSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EHitDirection HitDirection;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ReactionMontage;
};
