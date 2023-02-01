#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ComboBase.h"
#include "ComboSystemInterface.generated.h"

UINTERFACE(Blueprintable)
class UComboSystemInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class JOURNEYTOMASTERY_API IComboSystemInterface
{
	GENERATED_IINTERFACE_BODY()

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AssignComboInstance(UComboBase* ComboInstance);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComboWindowUpdated(bool Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInterruptStateUpdated(bool Value);
};
