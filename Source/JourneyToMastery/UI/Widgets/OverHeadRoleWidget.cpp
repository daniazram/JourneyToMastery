#include "OverHeadRoleWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverHeadRoleWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UOverHeadRoleWidget::ShowLocalNetRole(APawn* InPawn)
{
	FString Role = GetRoleString(InPawn->GetLocalRole());
	FString RoleString = FString::Printf(TEXT("Local Role: %s"), *Role);

	LocalRoleText->SetText(FText::FromString(RoleString));
}

void UOverHeadRoleWidget::ShowRemoteNetRole(APawn* InPawn)
{
	FString Role = GetRoleString(InPawn->GetRemoteRole());
	FString RoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);

	RemoteRoleText->SetText(FText::FromString(RoleString));
}


FString UOverHeadRoleWidget::GetRoleString(ENetRole RoleVal)
{
	FString Role;
	switch (RoleVal)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	}

	return Role;
}
