#pragma once

#include "CoreMinimal.h"
#include "HitReactSet.h"
#include "Components/ActorComponent.h"
#include "JourneyToMastery/Enums.h"
#include "HitReactComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOURNEYTOMASTERY_API UHitReactComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHitReactSet> HitReactions;
	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* InMeshComp);
	UFUNCTION(BlueprintCallable)
	void React(AActor* Hitter, FHitResult HitResult);

protected:

	USkeletalMeshComponent* PlayerMeshComp;
	
	virtual void BeginPlay() override;
	void PlayReactMontage(EHitDirection HitDirection);

public:	

	UHitReactComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
