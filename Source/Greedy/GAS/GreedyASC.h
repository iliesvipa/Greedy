// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GreedyASC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, AActor*, SourceActor);

UCLASS()
class GREEDY_API UGreedyASC : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UGreedyASC();

	UPROPERTY()
	FOnHit OnHitDelegate;

	UPROPERTY()
	TWeakObjectPtr<class AGreedyCharacter> GreedyCharacter;

	UPROPERTY()
	TWeakObjectPtr<class AGreedyPlayerController> GreedyPC;

	UPROPERTY()
	FGameplayTag HumanAirbone;
	UPROPERTY()
	FGameplayTag FightTag;
	UPROPERTY()
	FGameplayTag OffensiveTag;
	UPROPERTY()
	FGameplayTag ControlTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> NoInteractGE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> FightGE;

	UFUNCTION()
	void OnGEAppliedToSelf(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	UFUNCTION()
	void OnAbilityFailed(UGameplayAbility* InGameplayAbility, const FGameplayTagContainer& TagContainer);
	UFUNCTION()
	void CancelTagChanged(const FGameplayTag CallbackTag, int32 TagCount);
	UFUNCTION()
	void SetInteractableOff(const FGameplayTag CallbackTag, int32 TagCount);

	void OnPlayerControllerSet() override;
	
protected:
	void BeginPlay() override;
};
