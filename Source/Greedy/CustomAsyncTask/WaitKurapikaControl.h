// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitKurapikaControl.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitKurapikaControl, const FGameplayAbilityTargetDataHandle&, TargetData);

UCLASS()
class GREEDY_API UWaitKurapikaControl : public UAbilityTask
{
	GENERATED_BODY()

	UWaitKurapikaControl(const FObjectInitializer& ObjectInitializer);

	/*UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UWaitKurapikaControl* WaitKurapikaControl(UGameplayAbility* OwningAbility, float TimeSeconds = 2.f, float TimerInterval = 1.f);*/

	UPROPERTY(BlueprintAssignable)
	FWaitKurapikaControl OnStuff;

	/*UFUNCTION()
		void DoStuff();*/

	void Activate() override;


protected:
	float Seconds;
	float TimeInterval;

private:
	void OnDestroy(bool AbilityEnded) override;
};
