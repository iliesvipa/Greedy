// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/MMCs/DamageMMC.h"
#include "Greedy/AttributeSets/GreedyAttributeSet.h"

UDamageMMC::UDamageMMC()
{
	DamageDef.AttributeToCapture = UGreedyAttributeSet::GetDamageAttribute();
	DamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DamageDef.bSnapshot = true;

	DmgMultiDef.AttributeToCapture = UGreedyAttributeSet::GetDamageMultiplicaterAttribute();
	DmgMultiDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DmgMultiDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DamageDef);
	RelevantAttributesToCapture.Add(DmgMultiDef);
}

float UDamageMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	/*const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();*/

	FAggregatorEvaluateParameters EvaluationParameters;
	/*EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;*/

	float LocalDamage = 0.f;
	float LocalDmgMulti = 0.f;
	GetCapturedAttributeMagnitude(DamageDef, Spec, EvaluationParameters, LocalDamage);
	GetCapturedAttributeMagnitude(DmgMultiDef, Spec, EvaluationParameters, LocalDmgMulti);

	float DamageDone = LocalDamage * LocalDmgMulti;

	return DamageDone;
}
