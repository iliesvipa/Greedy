// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "AbilitySystemComponent.h"
#include "Greedy/GAS/GreedyGA.h"
#include "GreedyDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API UGreedyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGreedyGA>> NenAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> EventAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> ServerPassiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> LocalPassiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> BonusStats;

	UFUNCTION()
	void GiveAbilities(UAbilitySystemComponent* ASC);

	UFUNCTION()
	void GiveStats(UAbilitySystemComponent* ASC);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TArray<UAnimMontage*> AttackMontages;
};
