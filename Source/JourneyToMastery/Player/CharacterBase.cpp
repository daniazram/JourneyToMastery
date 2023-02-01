#include "CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "TDPlayerMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

ACharacterBase::ACharacterBase()
{
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	PlayerVisualsRoot = CreateDefaultSubobject<USceneComponent>("Visuals");
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>("PlayerMesh");
	MovementComponent = CreateDefaultSubobject<UTDPlayerMovementComponent>("MovementComponent");

	PlayerVisualsRoot->SetupAttachment(Root);
	PlayerMesh->SetupAttachment(PlayerVisualsRoot);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerController =  Cast<APlayerController>(GetController());
	MovementComponent->Init(this, PlayerVisualsRoot);
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::OnMovementTicked(FVector Location, FRotator Rotation)
{
	if(GetController() && GetController()->GetLocalRole() == ROLE_AutonomousProxy )
	{
		SetActorLocation(Location);
		PlayerVisualsRoot->SetRelativeRotation(Rotation);
		ServerMove(Location, Rotation);
	}
	else if(GetController() && GetController()->GetLocalRole() == ROLE_Authority)
	{
		SetActorLocation(Location);
		PlayerVisualsRoot->SetRelativeRotation(Rotation);
	}
}

void ACharacterBase::ServerMove_Implementation(FVector Location, FRotator Rotation)
{
	SetActorLocation(Location);
	PlayerVisualsRoot->SetRelativeRotation(Rotation);

	ClientMove(Location, Rotation);
}

void ACharacterBase::ClientMove_Implementation(FVector Location, FRotator Rotation)
{
	//SetActorLocation(Location);
	PlayerVisualsRoot->SetRelativeRotation(Rotation);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
}

void ACharacterBase::HandleSprint(bool sprinting)
{
	MovementComponent->OnSprintToggle(sprinting);
}

