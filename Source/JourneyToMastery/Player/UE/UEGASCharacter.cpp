#include "UEGASCharacter.h"
#include "AbilitySystemComponent.h"
#include "JourneyToMastery/GAS/PlayerAttributeSet.h"


AUEGASCharacter::AUEGASCharacter(const FObjectInitializer& ObjectInitializer) : AUECharacterBase(ObjectInitializer)
{
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("Attributes");
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AUEGASCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthChangeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AUEGASCharacter::OnHealthAttributeValueChanged);
	StaminaChangeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AUEGASCharacter::OnStaminaAttributeValueChanged);
}

void AUEGASCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).Remove(HealthChangeDelegateHandle);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).Remove(StaminaChangeDelegateHandle);
}

UAbilitySystemComponent* AUEGASCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUEGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUEGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	InitializeAttributes();
	GiveDefaultAbilities();
}

void AUEGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if(ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	InitializeAttributes();
}

void AUEGASCharacter:: InitializeAttributes()
{
	if(ASC == nullptr || DefaultAttributeEffect == nullptr)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext(); EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

	if(SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AUEGASCharacter::GiveDefaultAbilities()
{
	if(!HasAuthority() || ASC == nullptr)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities )
	{
		ASC->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
	}
}

void AUEGASCharacter::OnHealthAttributeValueChanged(const FOnAttributeChangeData& CallbackData)
{
	OnHealthValueChanged.Broadcast(CallbackData.Attribute, CallbackData.NewValue);
}

void AUEGASCharacter::OnStaminaAttributeValueChanged(const FOnAttributeChangeData& CallbackData)
{
	OnStaminaValueChanged.Broadcast(CallbackData.Attribute, CallbackData.NewValue);
}
