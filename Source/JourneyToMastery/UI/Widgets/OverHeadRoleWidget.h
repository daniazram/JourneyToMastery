#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadRoleWidget.generated.h"

UCLASS()
class JOURNEYTOMASTERY_API UOverHeadRoleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LocalRoleText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemoteRoleText;
	
	UFUNCTION(BlueprintCallable)
	void ShowLocalNetRole(APawn* InPawn);
	UFUNCTION(BlueprintCallable)
	void ShowRemoteNetRole(APawn* InPawn);

protected:

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	FString GetRoleString(ENetRole RoleVal);
};
