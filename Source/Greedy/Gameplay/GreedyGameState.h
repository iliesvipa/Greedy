// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameplayTagContainer.h"
#include "Greedy/Systems/Card.h"
#include "Engine/DataTable.h"
#include "GreedyGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardShopModified, const TArray<FCardStruct>&, Cards);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowcaseUpdated, const TArray<FCardStruct>&, SpellCards);

USTRUCT(BlueprintType)
struct FCardsData
{
	GENERATED_BODY()

		FCardsData()
		{
		}
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName CardRowName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentCards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ECardType CardType;

	bool operator== (const FCardsData& Other) const
	{
		return CardRowName == Other.CardRowName;
	}

	bool operator== (const FName& OtherName) const
	{
		return CardRowName == OtherName;
	}

	bool IsCardAvailable() const
	{
		return CurrentCards < MaxCards;
	}

	bool IsAngelsBreath() const
	{
		return CardRowName == "AngelsBreath";
	}

	bool IsList() const
	{
		return CardRowName == "List";
	}
};


UCLASS()
class GREEDY_API AGreedyGameState : public AGameState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FTimerHandle ShowCaseTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(BlueprintAssignable)
	FOnShowcaseUpdated OnShowcaseUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnCardShopModified OnCardShopModified;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FCardsData> AllCardsData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spell Shop", ReplicatedUsing = OnRep_Showcase)
	TArray<FCardStruct> ShowcaseCards;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cards", ReplicatedUsing = OnRep_CardShops)
	TArray<FCardStruct> CardShops;

	UFUNCTION(BlueprintCallable)
	void GetShowcase();

	UFUNCTION()
	void OnRep_Showcase();
	UFUNCTION()
	void OnRep_CardShops();

	UFUNCTION(BlueprintCallable)
	bool FetchCard(const FName& CardName);

	UFUNCTION(BlueprintCallable)
	void AddGlobalCardCount(const FName& Card);

	UFUNCTION(BlueprintCallable)
	void RemoveGlobalCardCount(const FName& Card);

	UFUNCTION()
	void RefreshCards(const FName& CardName, bool bExpire);

	UFUNCTION()
	void GiveAngelsBreath();

protected:
	void BeginPlay() override;
};
