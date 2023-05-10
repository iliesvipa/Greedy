// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Greedy/Systems/MasterShop.h"
#include "SpellShop.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API ASpellShop : public AMasterShop
{
	GENERATED_BODY()

public:

	ASpellShop();

	UFUNCTION(BlueprintCallable)
	const TArray<FName> FindRandomSpellCards(AActor* TargetActor, bool& Success, UInventoryComponent* InInventory);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float SpellCardsPack;

	UFUNCTION(BlueprintCallable)
	void TryBuyCardsFromShowcase(const FName& TheCard, AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void TryBuySpellCards(AActor* TargetActor);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyNoAvailableSpellCards(const AActor* TargetActor);
};
