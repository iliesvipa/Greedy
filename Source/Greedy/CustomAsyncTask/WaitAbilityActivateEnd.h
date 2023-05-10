// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "WaitAbilityActivateEnd.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityStartEnd, UGameplayAbility*, TheAbility);
/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class GREEDY_API UWaitAbilityActivateEnd : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnAbilityStartEnd OnAbilityActivated;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityStartEnd OnAbilityEnded;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitAbilityActivateEnd* ListenForAbilityActivateEnd(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayTag InAbilityTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTag AbilityTag;

	void OnAbilityActivatedCallback(UGameplayAbility* InAbility);
	void OnAbilityEndedCallback(UGameplayAbility* InAbility);
};
