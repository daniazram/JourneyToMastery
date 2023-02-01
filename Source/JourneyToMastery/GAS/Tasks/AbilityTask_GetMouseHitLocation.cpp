#include "AbilityTask_GetMouseHitLocation.h"

UAbilityTask_GetMouseHitLocation::UAbilityTask_GetMouseHitLocation(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;
}

UAbilityTask_GetMouseHitLocation* UAbilityTask_GetMouseHitLocation::GetHitLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, APlayerController* PlayerController)
{
	UAbilityTask_GetMouseHitLocation* Task = NewAbilityTask<UAbilityTask_GetMouseHitLocation>(OwningAbility, TaskInstanceName);
	//Task->World = GEngine->GetWorldFromContextObject(World, EGetWorldErrorMode::LogAndReturnNull);;
	Task->PlayerController = PlayerController;

	return Task;
}

void UAbilityTask_GetMouseHitLocation::Activate()
{
	Super::Activate();
}

void UAbilityTask_GetMouseHitLocation::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorlDirection);
	LineTraceEnd = WorldLocation + (WorlDirection * 10000);
	
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, LineTraceEnd, ECC_WorldStatic);
	if(IsHit)
	{
		OnUpdate.Broadcast(HitResult.Location);
	}
}


