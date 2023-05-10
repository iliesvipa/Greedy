// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/MMCs/HealMMC.h"
#include "Greedy/AttributeSets/GreedyAttributeSet.h"

UHealMMC::UHealMMC()
{
	HealDef.AttributeToCapture = UGreedyAttributeSet::GetHealAttribute();
	HealDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	HealDef.bSnapshot = true;

	HealMultiDef.AttributeToCapture = UGreedyAttributeSet::GetHealMultiplicaterAttribute();
	HealMultiDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	HealMultiDef.bSnapshot = true;

	RelevantAttributesToCapture.Add(HealDef);
	RelevantAttributesToCapture.Add(HealMultiDef);
}

float UHealMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	/*const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();*/

	FAggregatorEvaluateParameters EvaluationParameters;
	/*EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;*/

	float LocalHeal = 0.f;
	float LocalHealMulti = 0.f;
	GetCapturedAttributeMagnitude(HealDef, Spec, EvaluationParameters, LocalHeal);
	GetCapturedAttributeMagnitude(HealMultiDef, Spec, EvaluationParameters, LocalHealMulti);

	float HealDone = LocalHeal * LocalHealMulti;

	return HealDone;
}