#include "UECharacterBase.h"
#include "EnhancedInputComponent.h"
#include "UEMovementComponent.h"

AUECharacterBase::AUECharacterBase(const FObjectInitializer& ObjectInitializer)
				: Super(ObjectInitializer.SetDefaultSubobjectClass<UUEMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUECharacterBase::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = Cast<UUEMovementComponent>(GetMovementComponent());
}

void AUECharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUECharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
}

void AUECharacterBase::HandleSprint(bool InSprinting)
{
	Sprinting = InSprinting;
	MovementComponent->HandleSprint(InSprinting);
}