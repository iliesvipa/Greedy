// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "TA_Widget.generated.h"

/**
 * 
 */
class UGameplayAbility;

UCLASS(Blueprintable, notplaceable)
class GREEDY_API ATA_Widget : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:

	ATA_Widget(const FObjectInitializer& ObjectInitializer);
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	UFUNCTION()
	void ReactPlayerSelected(APlayerState* PS);

	UFUNCTION()
	void ReceiveCancel();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Parameters")
	TSubclassOf<class UGameplayWidget> WidgetToSpawn;

	UPROPERTY()
	class UGameplayWidget* Widget;

protected:
	
	virtual void BindToWidgetSelection();
	FGameplayAbilityTargetDataHandle Handle;
};
