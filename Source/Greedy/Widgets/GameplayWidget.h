// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "GameplayWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSelected, APlayerState*, PlayerSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestructCalled);

UCLASS()
class GREEDY_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintCallable)
	FOnPlayerSelected OnPlayerSelected;

	UPROPERTY(BlueprintCallable)
	FOnDestructCalled OnDestructCalled;
};
