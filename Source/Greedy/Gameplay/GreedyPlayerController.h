// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Greedy/Systems/SpellShop.h"
#include "Greedy/Systems/Card.h"
#include "GreedyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API AGreedyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void OnPossess(APawn* P) override;

	UPROPERTY()
	TWeakObjectPtr<class AGreedyCharacter> GreedyCharacter;
	
	// Notifies

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyInsufficientJenis();
};
