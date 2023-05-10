// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Greedy/Systems/MasterShop.h"
#include "CardShop.generated.h"

UCLASS()
class GREEDY_API ACardShop : public AMasterShop
{
	GENERATED_BODY()
	
public:

	ACardShop();

	UFUNCTION(BlueprintCallable)
	void TryBuyCard(AActor* TargetActor, const FCardStruct& CardToBuy);

	UFUNCTION(BlueprintCallable)
	void SortElements(TArray<FCardStruct> CardsToSort, TArray<FCardStruct>& ItemCards, TArray<FCardStruct>& HealCards, FCardStruct& SwordOfTruthCard);
};
