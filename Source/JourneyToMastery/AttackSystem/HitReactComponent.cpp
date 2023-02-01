#include "HitReactComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "JourneyToMastery/Player/CharacterBase.h"

UHitReactComponent::UHitReactComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHitReactComponent::Init(USkeletalMeshComponent* InMeshComp)
{
	PlayerMeshComp = InMeshComp;
}

void UHitReactComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UHitReactComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHitReactComponent::React(AActor* Hitter, FHitResult HitResult)
{
	if(PlayerMeshComp == nullptr)
	{
		return;
	}

	ACharacterBase* Char = Cast<ACharacterBase>(Hitter);
	if(Char == nullptr)
	{
		return;
	}

	const FVector Right = GetOwner()->GetActorRightVector();
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const float DotR = FVector::DotProduct(Right, Char->PlayerVisualsRoot->GetForwardVector());
	const float DotF= FVector::DotProduct(Forward, Char->PlayerVisualsRoot->GetForwardVector());

	if(UKismetMathLibrary::InRange_FloatFloat(DotF, -1.f, -0.85f))
	{
		PlayReactMontage(EHitDirection::Front);	
	}
	else if(UKismetMathLibrary::InRange_FloatFloat(DotF, 0.85f, 1.f))
	{
		PlayReactMontage (EHitDirection::Back);
	}
	else if(UKismetMathLibrary::InRange_FloatFloat(DotR, -1.f, -0.85f))
	{
		PlayReactMontage (EHitDirection::Left);	
	}
	else if(UKismetMathLibrary::InRange_FloatFloat(DotR, 0.85f, 1.f))
	{
		PlayReactMontage (EHitDirection::Right);
	}
}

void UHitReactComponent::PlayReactMontage(EHitDirection HitDirection)
{
	UAnimMontage* MontageToPlay = nullptr;
	for (int i = 0; i < HitReactions.Num(); i++)
	{
		if(HitReactions[i].HitDirection == HitDirection)
		{
			MontageToPlay = HitReactions[i].ReactionMontage;
			break;
		}
	}

	if(MontageToPlay == nullptr)
	{
		return;
	}
	
	PlayerMeshComp->GetAnimInstance()->Montage_Play(MontageToPlay);
}