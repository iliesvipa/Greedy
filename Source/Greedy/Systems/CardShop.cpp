// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Systems/CardShop.h"

ACardShop::ACardShop()
{
	OnTag = FGameplayTag::RequestGameplayTag(FName("Event.Interact.Shop.Card"));
}

void ACardShop::TryBuyCard(AActor* TargetActor, const FCardStruct& CardToBuy)
{
	if (TargetActor == nullptr) {
		return;
	}
	AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(TargetActor);
	if (!GreedyChar) {
		return;
	}
	UInventoryComponent* Inventory = GreedyChar->GetInventory();
	if (!Inventory || !GameStateRef.Get()->FetchCard(CardToBuy.CardRowName)) {
		return;
	}
	float Price = CardToBuy.BuyCost;
	bool bCondition = 
		(CardToBuy.CardType == ECardType::HealCard || CardToBuy.CardType == ECardType::ItemCard 
		|| CardToBuy.IsSwordOfTruth()) && !CardToBuy.IsGalgaida();
	if (!bCondition || !Inventory->HasEnoughJenis(Price) || !CheckDistance(TargetActor)) {
		return;
	}

	if (CardToBuy.bRestricted) {
		if (!Inventory->HasMatchingGameplayTag(CardToBuy.RestrictTag)) {
			return;
		}
	}
	if (Inventory->AddCard(CardToBuy.CardRowName)) {
		GameStateRef->AddGlobalCardCount(CardToBuy.CardRowName);
		Inventory->CommitJenisAmount(Price * -1);
		Inventory->AddCardShopBuys();
	}
}

void ACardShop::SortElements(TArray<FCardStruct> CardsToSort, TArray<FCardStruct>& ItemCards, TArray<FCardStruct>& HealCards, FCardStruct& SwordOfTruthCard)
{
	CardsToSort.Sort([](const FCardStruct& A, const FCardStruct& B) { return A.BuyCost < B.BuyCost; });
	for (const FCardStruct& CardToSort : CardsToSort)
	{
		if (CardToSort.CardType == ECardType::ItemCard)
		{
			ItemCards.Add(CardToSort);
		}
		else if (CardToSort.CardType == ECardType::HealCard)
		{
			HealCards.Add(CardToSort);
		}
		else if (CardToSort.IsSwordOfTruth()) {
			SwordOfTruthCard = CardToSort;
		}
	}
}
