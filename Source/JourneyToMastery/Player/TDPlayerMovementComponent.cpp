#include "TDPlayerMovementComponent.h"
#include "Modifier_SimpleMovement.h"
#include "Net/UnrealNetwork.h"
#include "CharacterBase.h"

UTDPlayerMovementComponent::UTDPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	OverridingModifiers = new TArray<MovementComponentModifier*>();
}

void UTDPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = GetOwner();

	RootMoveModifier = new Modifier_RootMove();
	SimpleMoveModifier = new Modifier_SimpleMovement();
	CardinalMoveModifier = new Modifier_CardinalMove();

	SetIsReplicated(true);
	MovementModifier = SimpleMoveModifier;
}

void UTDPlayerMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OverridingModifiers->Reset();
	delete(OverridingModifiers);
}

void UTDPlayerMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTDPlayerMovementComponent, InputSize);
	DOREPLIFETIME(UTDPlayerMovementComponent, Horizontal);
	DOREPLIFETIME(UTDPlayerMovementComponent, Vertical);
	DOREPLIFETIME(UTDPlayerMovementComponent, HeadingDirection);

	DOREPLIFETIME(UTDPlayerMovementComponent, Player);
	DOREPLIFETIME(UTDPlayerMovementComponent, PlayerCharacter);
	DOREPLIFETIME(UTDPlayerMovementComponent, PlayerVisualRoot);
}

void UTDPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FMovementData Data;
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	RootMoveModifier->OnInputUpdate(RootMotionPositionDelta);
	MovementModifier->OnInputUpdate(Input);
	MovementModifier->UpdateSpeed(CurrentSpeed, RotationSpeed);
	
	Data = MovementModifier->Execute(DeltaTime);
	/*FMovementData RootMoveData = RootMoveModifier->Execute(DeltaTime);
	if(RootMoveData.Valid)
	{
		Data = RootMoveData;
	}*/
	
	Velocity = MovementModifier->GetVelocity();
	HeadingDirection = MovementModifier->GetHeadingDirection();

	if(PlayerController && PlayerController->GetLocalRole() == ROLE_AutonomousProxy)
	{
		if(Data.Valid)
		{
			Player->SetActorLocation(Data.Location);
			PlayerVisualRoot->SetRelativeRotation(Data.Rotation);
			ServerMove(Data.Location, Data.Rotation);
		}
	}
	else if(PlayerController && PlayerController->GetLocalRole() == ROLE_Authority)
	{
		Player->SetActorLocation(Data.Location);
		PlayerVisualRoot->SetRelativeRotation(Data.Rotation);
	}
}

void UTDPlayerMovementComponent::ServerMove_Implementation(FVector Location, FRotator Rotation)
{
	ClientMove(Location, Rotation);
}

void UTDPlayerMovementComponent::ClientMove_Implementation(FVector Location, FRotator Rotation)
{
	Player->SetActorLocation(Location);
	PlayerVisualRoot->SetRelativeRotation(Rotation);
}

void UTDPlayerMovementComponent::Init(ACharacterBase* InChar, USceneComponent* InVisualRoot)
{
	PlayerCharacter = InChar;
	PlayerVisualRoot = InVisualRoot;
	PlayerController = PlayerCharacter->PlayerController;

	RootMoveModifier->Init(InChar, InVisualRoot);
	SimpleMoveModifier->Init(InChar, InVisualRoot);
	CardinalMoveModifier->Init(InChar, InVisualRoot);
	
	MovementModifier = SimpleMoveModifier;
	MovementModifier->Enter();
}

void UTDPlayerMovementComponent::HandleAiming(FVector hitLocation)
{
}

void UTDPlayerMovementComponent::ChangeModifierTo(MovementComponentModifier* Modifier)
{
	if(!Modifier->Initialized)
	{
		Modifier->Init(PlayerCharacter, PlayerVisualRoot);
	}
	
	MovementModifier = Modifier;
}

void UTDPlayerMovementComponent::AddOverridingModifier(MovementComponentModifier* Modifier)
{
	if(OverridingModifiers->Contains(Modifier))
	{
		return;
	}

	OverridingModifiers->Add(Modifier);
}

void UTDPlayerMovementComponent::RemoveOverridingModifier(MovementComponentModifier* Modifier)
{
	if(!OverridingModifiers->Contains(Modifier))
	{
		return;
	}

	OverridingModifiers->Remove(Modifier);
}

void UTDPlayerMovementComponent::OnSprintToggle(bool value)
{
	Sprinting = value;
	
}

void UTDPlayerMovementComponent::OnAimToggle(bool value)
{
	Aiming = value;
	if(Aiming)
	{
		MovementModifier->Exit();
		MovementModifier = CardinalMoveModifier;
	}
	else
	{
		MovementModifier->Exit();
		MovementModifier = SimpleMoveModifier;
	}
}

void UTDPlayerMovementComponent::OnRotationInputUpdate(float yaw, float pitch)
{
	Yaw = yaw;
	Pitch = pitch;
}

void UTDPlayerMovementComponent::AddHorizontalMoveInput(float value)
{
	OnMovementInputUpdate(value, Vertical);
}

void UTDPlayerMovementComponent::AddVerticalMoveInput(float value)
{
	OnMovementInputUpdate(Horizontal, value);
}

void UTDPlayerMovementComponent::OnMovementInputUpdate(float horizontal, float vertical)
{
	Horizontal = horizontal;
	Vertical = vertical;

	Input = FVector(Vertical, Horizontal, 0);
	Input.Normalize();
	InputSize = Input.Size();
}

void UTDPlayerMovementComponent::ApplyMovementData(FMovementData InData)
{
	Player->SetActorLocation(InData.Location);
	PlayerVisualRoot->SetRelativeRotation(InData.Rotation);
}
