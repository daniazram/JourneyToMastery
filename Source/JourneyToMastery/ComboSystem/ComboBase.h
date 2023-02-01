#pragma once

#include "CoreMinimal.h"
#include "ComboItem.h"
#include "EnhancedInput/Public/InputAction.h"
#include "ComboBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FComboEndedDelegate);

UCLASS(Blueprintable)
class JOURNEYTOMASTERY_API UComboBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FComboItem> ComboList;
	FComboEndedDelegate ComboEndDelegate;
	
	bool IsRunning;
	bool CanBeInterrupted;
	bool ComboWindowAvailable;
	int ComboIndex;

	virtual void Init(UAnimInstance* Value);
	virtual void Run();
	virtual void Update(FComboItem NextItem);
	virtual void Reset();
	virtual void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);
	UFUNCTION(BlueprintCallable)
	virtual void OnInputUpdated(const FInputActionInstance& ActionInstance);
	UFUNCTION(BlueprintCallable)
	virtual void OnComboWindowUpdated(bool Available);
	UFUNCTION(BlueprintCallable)
	virtual void OnComboInterruptStateUpdated(bool Value);

protected:

	bool Started;
	bool InputLock;
	FComboItem StartingItem;
	FComboItem CurrentItem;
	FComboItem LastItem;
	UAnimInstance* AnimInstance;
	FOnMontageEnded MontageEndDelegate;

	void PlayMontage(UAnimMontage* Montage);
	FComboItem GetItemFromInputAction(const UInputAction* Action);
};
