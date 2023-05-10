// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Greedy/Greedy.h"
#include "GreedyGA.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API UGreedyGA : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGreedyGA();

	// Move le code de movement speed changes dans PostAttributeChange..

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> GEDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> GEHeal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> ResetGE;

	UPROPERTY()
	FGameplayTag DamageTag;
	UPROPERTY()
	FGameplayTag HealTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	FGameplayTag ResetTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilityTargetingLocationInfo TargetingLocationInfo;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayAbilityTargetingLocationInfo& SetTargetingLocationInfo(float BeginMagnitude, float EndMagnitude, FVector& EndVector);
	
	UFUNCTION(BlueprintCallable)
	void ApplyReset(const FGameplayTag& NewGrantedTag, float Magnitude = 1.f);

	UFUNCTION(BlueprintCallable)
	void SetDamage(float Magnitude);
	UFUNCTION(BlueprintCallable)
	void SetHeal(float Magnitude);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	EGreedyAbilityInputID AbilityInputID {EGreedyAbilityInputID::None};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	bool bActivateAbilityOnGranted;

	void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void OnWaitingForConfirmInputBegin() override;
	void OnWaitingForConfirmInputEnd() override;
};
