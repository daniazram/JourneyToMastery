#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UEMovementComponent.generated.h"

UCLASS()
class JOURNEYTOMASTERY_API UUEMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm"))
	float JogSpeed;
	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm"))
	float SprintSpeed;

	void HandleSprint(bool InSprint)
	{
		MaxWalkSpeed = InSprint ? SprintSpeed : JogSpeed;
	}
};