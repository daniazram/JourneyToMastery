#pragma once

#include "ComboItem.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FComboItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Montage = nullptr;

	bool IsValid();
	
	bool operator == (FComboItem OtherItem);
};
