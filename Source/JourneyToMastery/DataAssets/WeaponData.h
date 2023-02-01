#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "JourneyToMastery/AttackSystem/Weapons/WeaponBase.h"
#include "WeaponData.generated.h"
UCLASS()
class JOURNEYTOMASTERY_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadonly)
	UInputAction* InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeaponBase> WeaponPrefab;
};
