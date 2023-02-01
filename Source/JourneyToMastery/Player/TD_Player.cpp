#include "TD_Player.h"
#include "TDPlayerMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "JourneyToMastery/Enums.h"
#include "JourneyToMastery/MyGameModeBase.h"
#include "EnhancedInputComponent.h"
#include "JourneyToMastery/GAS/InputBoundGameplayAbility.h"

ATD_Player::ATD_Player()
{
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	PlayerVisualsRoot = CreateDefaultSubobject<USceneComponent>("Visuals");
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>("PlayerMesh");
	WeaponSlot = CreateDefaultSubobject<USceneComponent>("WeaponSlot");
	MovementComponent = CreateDefaultSubobject<UTDPlayerMovementComponent>("MovementComponent");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("Attributes");
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttackSystem = CreateDefaultSubobject<UAttackSystemComponent>("AttackSystemComponent");
	
	SetRootComponent(Root);
	PlayerVisualsRoot->SetupAttachment(Root);
	PlayerMesh->SetupAttachment(PlayerVisualsRoot);
	WeaponSlot->SetupAttachment(PlayerVisualsRoot, FName("socket-weapon"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void ATD_Player::BeginPlay()
{
	Super::BeginPlay();

	PlayerController =  Cast<APlayerController>(GetController());
	AttackSystem->Init(WeaponSlot, PlayerMesh);
	//MovementComponent->Init(this, PlayerVisualsRoot);

	AttackSystem->WeaponChangeDelegate.BindUObject(this, &ATD_Player::OnWeaponChanged);
	HealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ATD_Player::OnHealthAttributeValueChanged);
	StaminaChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &ATD_Player::OnStaminaAttributeValueChanged);
}

void ATD_Player::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).Remove(HealthChangeDelegateHandle);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).Remove(StaminaChangeDelegateHandle);
}

void ATD_Player::OnHealthAttributeValueChanged(const FOnAttributeChangeData& CallbackData)
{
	OnHealthValueChanged.Broadcast(AttributeSet->GetHealthAttribute(), CallbackData.NewValue);
}

void ATD_Player::OnStaminaAttributeValueChanged(const FOnAttributeChangeData& CallbackData)
{
	OnStaminaValueChanged.Broadcast(AttributeSet->GetStaminaAttribute(), CallbackData.NewValue);
}

void ATD_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATD_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(AttackSystem == nullptr)
	{
		return;
	}

	InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AttackSystem->BindInput(InputComp, WeaponSlot, PlayerMesh);
}

UAbilitySystemComponent* ATD_Player::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATD_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	InitializeAttributes();
	GiveDefaultAbilities();
}

void ATD_Player::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if(InputComponent)
		{
			FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInputID",
			static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));

			AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
		}
	}

	InitializeAttributes();
}

void ATD_Player::InitializeAttributes()
{
	if(AbilitySystemComponent == nullptr || DefaultAttributeEffect == nullptr)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext(); EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

	if(SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ATD_Player::GiveDefaultAbilities()
{
	if(!HasAuthority() || AbilitySystemComponent == nullptr)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities )
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
	}
}

void ATD_Player::OnWeaponChanged(AWeaponBase* Weapon, bool Equipped)
{
	if(Equipped)
	{
		WeaponEquippedEvent(Weapon);
	}
	else
	{
		OnWeaponSheathedEvent(Weapon);
	}
}

void ATD_Player::ToggleAiming(bool value)
{
	MovementComponent->OnAimToggle(value);
}

void ATD_Player::HandleSprint(bool sprinting)
{
	MovementComponent->OnSprintToggle(sprinting);
}

void ATD_Player::HandleAiming(FVector hitLocation)
{
	MovementComponent->HandleAiming(hitLocation);
}

