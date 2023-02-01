#include "ComboBase.h"
#include "InputAction.h"

void UComboBase::Init(UAnimInstance* Value)
{
	AnimInstance = Value;
	StartingItem = ComboList[0];
	LastItem = ComboList.Last();
	Reset();
}

void UComboBase::Run()
{
	Started = true;
	IsRunning = true;
	CurrentItem = ComboList[0];
	UAnimMontage* Montage = CurrentItem.Montage;
	PlayMontage(Montage);
}

void UComboBase::Update(FComboItem NextItem)
{
	if(!NextItem.IsValid())
	{
		//Do nothing if we are at the end of the combo
		GEngine->AddOnScreenDebugMessage(201, 2.f, FColor::Green, FString::Printf(TEXT("Item isn't valid")));
		return;
	}

	PlayMontage(NextItem.Montage);
}

void UComboBase::Reset()
{
	ComboIndex = 0;
	Started = false;
	IsRunning = false;
	ComboWindowAvailable = false;
	CanBeInterrupted = false;
	InputLock = false;
}

void UComboBase::OnInputUpdated(const FInputActionInstance& ActionInstance)
{
	if(!Started)
	{
		return;
	}
	
	const UInputAction* IncomingAction = ActionInstance.GetSourceAction();
	
	if(!IsRunning)
	{
		Run();
	}
	else if(!ComboWindowAvailable && CanBeInterrupted)
	{
		if(IncomingAction == StartingItem.InputAction)
		{
			Reset();
			Run();
		}
	}
	else if(ComboWindowAvailable)
	{
		if(CurrentItem == LastItem)
		{
			Reset();
			Run();
			return;
		}
		
		ComboWindowAvailable = false;
		CanBeInterrupted = false;

		FComboItem Item = GetItemFromInputAction(IncomingAction); 
		Update(Item);
	}
}

void UComboBase::OnComboWindowUpdated(bool Available)
{
	ComboWindowAvailable = Available;
	if(Available)
	{
		CanBeInterrupted = false;
	}
}

void UComboBase::OnComboInterruptStateUpdated(bool Value)
{
	CanBeInterrupted = Value;
}

void UComboBase::PlayMontage(UAnimMontage* Montage)
{
	MontageEndDelegate.BindUObject(this, &UComboBase::OnMontageEnded);
	AnimInstance->Montage_Play(Montage, 0.85f);
	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, Montage);
}

void UComboBase::OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if(Interrupted)
	{
		return;
	}
	
	Reset();
	MontageEndDelegate.Unbind();
	if(ComboEndDelegate.IsBound())
	{
		ComboEndDelegate.Broadcast();
	}
}

FComboItem UComboBase::GetItemFromInputAction(const UInputAction* Action)
{
	FComboItem Item;
	for(int i=0; i<ComboList.Num(); i++)
	{
		if(i <= ComboIndex)
		{
			continue;
		}

		FComboItem CurrItem = ComboList[i];
		if(CurrItem.InputAction == Action)
		{
			ComboIndex++;
			Item = CurrItem;
			CurrentItem = Item;
			
			return Item;
		}
	}

	return Item;
}
