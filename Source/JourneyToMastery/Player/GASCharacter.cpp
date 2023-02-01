#include "GASCharacter.h"
#include "AbilitySystemComponent.h"
#include "JourneyToMastery/GAS/PlayerAttributeSet.h"


AGASCharacter::AGASCharacter() : ACharacterBase()
{
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("Attributes");
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthChangeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGASCharacter::OnHealthAttributeValueChanged);
	StaminaChangeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AGASCharacter::OnStaminaAttributeValueChanged);
}

void AGASCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).Remove(HealthChangeDelegateHandle);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).Remove(StaminaChangeDelegateHandle);
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	InitializeAttributes();
	GiveDefaultAbilities();
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if(ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	InitializeAttributes();
}

void AGASCharacter::InitializeAttributes()
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

void AGASCharacter::GiveDefaultAbilities()
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

void AGASCharacter::OnHealthAttributeValueChanged(const FOnAttributeChangeData& CallbackData)
{
	OnHealthValueChanged.Broadcast(CallbackData.Attribute, CallbackData.NewValue);
}

void AGASCharacter::OnStaminaAttributeValueChanged(const FOnAttributeChangeData& CallbackData)
{
	OnStaminaValueChanged.Broadcast(CallbackData.Attribute, CallbackData.NewValue);
}
