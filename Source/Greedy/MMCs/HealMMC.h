// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "HealMMC.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API UHealMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	UHealMMC();

	FGameplayEffectAttributeCaptureDefinition HealDef;
	FGameplayEffectAttributeCaptureDefinition HealMultiDef;

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;
};
