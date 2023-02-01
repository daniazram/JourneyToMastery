#include "InputBinderComponent.h"

UInputBinderComponent::UInputBinderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInputBinderComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UInputBinderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

