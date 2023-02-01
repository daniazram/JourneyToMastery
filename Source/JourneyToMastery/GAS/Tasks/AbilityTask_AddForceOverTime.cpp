#include "AbilityTask_AddForceOverTime.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "JourneyToMastery/Player/GASCharacter.h"
#include "JourneyToMastery/Player/Modifier_Dash.h"
#include "JourneyToMastery/Player/Modifier_SimpleMovement.h"
#include "JourneyToMastery/Player/TDPlayerMovementComponent.h"

UAbilityTask_AddForceOverTime::UAbilityTask_AddForceOverTime(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;

	Duration = Force = Elapsed = 0.f;
	WorldDirection = FVector::Zero();
}

UAbilityTask_AddForceOverTime* UAbilityTask_AddForceOverTime::AddConstantForceForDuration(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Force, float Duration, UCurveFloat* Curve, FVector WorldDirection)
{
	UAbilityTask_AddForceOverTime* Task = NewAbilityTask<UAbilityTask_AddForceOverTime>(OwningAbility, TaskInstanceName);
	Task->Force = Force;
	Task->Duration = Duration;
	Task->Curve = Curve;
	Task->WorldDirection = WorldDirection;
	Task->Elapsed = 0.f;
	
	return Task;
}

void UAbilityTask_AddForceOverTime::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityTask_AddForceOverTime, WorldDirection);
	DOREPLIFETIME(UAbilityTask_AddForceOverTime, Force);
	DOREPLIFETIME(UAbilityTask_AddForceOverTime, Duration);
}

void UAbilityTask_AddForceOverTime::Activate()
{
	DashModifier = new Modifier_Dash();
	DashModifier->InitDashData(WorldDirection, Force);
	
	Player = Cast<AGASCharacter>(AbilitySystemComponent->AbilityActorInfo->OwnerActor);
	Player->MovementComponent->ChangeModifierTo(DashModifier);
}

void UAbilityTask_AddForceOverTime::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if(Player == nullptr || Elapsed > Duration)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			Modifier_SimpleMovement* Modifier = new Modifier_SimpleMovement();
			Player->MovementComponent->ChangeModifierTo(Modifier);
			OnFinish.Broadcast();
		}
		
		return;
	}

	const float Speed = FMath::Lerp(0.f, Force, Curve->FloatCurve.Eval(Elapsed/Duration));
	DashModifier->OverrideSpeed(Speed);
	Elapsed += DeltaTime;
}
