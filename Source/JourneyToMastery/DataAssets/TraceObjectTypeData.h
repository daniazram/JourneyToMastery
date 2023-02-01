#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TraceObjectTypeData.generated.h"

UCLASS()
class JOURNEYTOMASTERY_API UTraceObjectTypeData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
};