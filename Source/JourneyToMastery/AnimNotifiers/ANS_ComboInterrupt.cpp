#include "ANS_ComboInterrupt.h"
#include "JourneyToMastery/ComboSystem/ComboSystemInterface.h"
#include "JourneyToMastery/Player/UE/UEHeroCharacter.h"
#include "JourneyToMastery/Player/TD_Player.h"

void UANS_ComboInterrupt::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AUEHeroCharacter* Player = Cast<AUEHeroCharacter>(MeshComp->GetOwner());
	if(Player)
	{
		UAttackSystemComponent* AttackComp = Player->AttackSystem;
		if(AttackComp)
		{
			IComboSystemInterface* ComboInterface = Cast<IComboSystemInterface>(AttackComp->CurrentWeapon);

			if(ComboInterface)
			{
				ComboInterface->Execute_OnInterruptStateUpdated(ComboInterface->_getUObject(), true);
			}
		}
	}
}

void UANS_ComboInterrupt::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AUEHeroCharacter* Player = Cast<AUEHeroCharacter>(MeshComp->GetOwner());
	if(Player)
	{
		UAttackSystemComponent* AttackComp = Player->AttackSystem;
		if(AttackComp)
		{
			IComboSystemInterface* ComboInterface = Cast<IComboSystemInterface>(AttackComp->CurrentWeapon);

			if(ComboInterface)
			{
				ComboInterface->Execute_OnInterruptStateUpdated(ComboInterface->_getUObject(), false);
			}
		}
	}
}

