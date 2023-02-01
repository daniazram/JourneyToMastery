#include "LatentFunctionsLibrary.h"
#include "FDoOverTimeLatentAction.h"

class FDoOverTimeLatentAction;

void ULatentFunctionsLibrary::DoOverTime(AActor* InActor, float& Alpha, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine->GetWorldFromContextObject(InActor, EGetWorldErrorMode::ReturnNull);
	FLatentActionManager& LatentManager = World->GetLatentActionManager();
	FDoOverTimeLatentAction* Latent =  new FDoOverTimeLatentAction(5, [Alpha](float ratio) mutable
	{
		Alpha = ratio;
	}, []()
	{
	});

	LatentManager.AddNewAction(InActor, 0, Latent);
}