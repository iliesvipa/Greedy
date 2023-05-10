// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_Sphere.generated.h"

/**
 * 
 */
class UGameplayAbility;

UCLASS(Blueprintable, notplaceable)
class GREEDY_API ATA_Sphere : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:

	ATA_Sphere(const FObjectInitializer& ObjectInitializer);

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Parameters")
	float Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Parameters")
	FVector LengthVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Parameters")
	bool bIgnoreSelf;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Parameters")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
protected:

	FHitResult PerformOverlap(const FVector& OriginVector);
	FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& TargetDataHitResult) const;
};
