// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitForCustomOverlap.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetHit, const FGameplayAbilityTargetDataHandle&, TargetData);

class AActor;

UCLASS()
class GREEDY_API UWaitForCustomOverlap : public UAbilityTask
{
	GENERATED_BODY()

	UWaitForCustomOverlap(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UWaitForCustomOverlap* WaitForCustomOverlap(UGameplayAbility* OwningAbility, AActor* SpawnedActor, float Seconds = 2.f , float TimerDelay = 0.1f);

	UPROPERTY(BlueprintAssignable)
	FOnTargetHit OnValidTargetHit;

	UPROPERTY(BlueprintAssignable)
	FOnTargetHit OnInvalidTargetHit;

	FTimerHandle TimerHandle;

	void Activate() override;

	UFUNCTION()
	void PerformTrace();

protected:
	float TimerSeconds;
	float TimerInterval;
	AActor* Actor;

private:
	void OnDestroy(bool AbilityEnded) override;
	
};
