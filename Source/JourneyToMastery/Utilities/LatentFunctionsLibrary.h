#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LatentFunctionsLibrary.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelayOneFrameOutputPin);

UCLASS()
class JOURNEYTOMASTERY_API ULatentFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject"), Category = "LatentFunctionsLibrary")
	static void DoOverTime(AActor* InActor, float&  Alpha, FLatentActionInfo LatentInfo);
};
