// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GreedyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayEffectModCallbackData;
/**
 * 
 */
UCLASS()
class GREEDY_API UGreedyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Stats", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Stats", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Stats", ReplicatedUsing = OnRep_Nen)
	FGameplayAttributeData Nen;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, Nen)

	UPROPERTY(BlueprintReadOnly, Category = "Stats", ReplicatedUsing = OnRep_MaxNen)
	FGameplayAttributeData MaxNen;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, MaxNen)

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Heal;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, Heal)

	UPROPERTY(BlueprintReadOnly, Category = "Stats", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData SpeedMultiplicater;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, SpeedMultiplicater)

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData DamageMultiplicater;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, DamageMultiplicater)

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData HealMultiplicater;
	ATTRIBUTE_ACCESSORS(UGreedyAttributeSet, HealMultiplicater)

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	void OnRep_Nen(const FGameplayAttributeData& OldNen);
	UFUNCTION()
	void OnRep_MaxNen(const FGameplayAttributeData& OldMaxNen);
	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& OldSpeed);
};
