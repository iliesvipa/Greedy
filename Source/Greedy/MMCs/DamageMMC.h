// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DamageMMC.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API UDamageMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UDamageMMC();

	FGameplayEffectAttributeCaptureDefinition DamageDef;
	FGameplayEffectAttributeCaptureDefinition DmgMultiDef;

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;
	
};
