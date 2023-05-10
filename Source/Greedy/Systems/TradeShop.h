// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Greedy/Systems/MasterShop.h"
#include "AbilitySystemComponent.h"
#include "TradeShop.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API ATradeShop : public AMasterShop
{
	GENERATED_BODY()
	
public:

	ATradeShop();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	TObjectPtr<UDataTable> QuestDT;

	//static int32 Counter;

	//UPROPERTY()
	//uint8 UniqueID; // In Master Shop !

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float SellMultiplicater;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float BasicPrice;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float NotBasicPrice;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	float QuestPrice;

	UFUNCTION(BlueprintCallable)
	void GiveQuest(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void GiveVoucher(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void SeePlayerCards(AActor* TargetActor, AActor* Target, bool BonusTip);

	UFUNCTION(BlueprintCallable)
	void TrySellCards(const TArray<FName>& CardsToSell, AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> CooldownGE;

	UPROPERTY()
	FGameplayTag CooldownTag;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyAllQuestDone(const AActor* TargetActor);

};
