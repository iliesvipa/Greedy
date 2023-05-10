// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Systems/SpellShop.h"


ASpellShop::ASpellShop()
{
	OnTag = FGameplayTag::RequestGameplayTag(FName("Event.Interact.Shop.Spell"));
	SpellCardsPack = 10000;
}

const TArray<FName> ASpellShop::FindRandomSpellCards(AActor* TargetActor, bool& Success, UInventoryComponent* InInventory)
{
	TArray<FName> RandomSpellCards;
	UInventoryComponent* LocalInventory = InInventory;
	TArray<FName*> SpellCardsArray;
	TArray<FName*> NotPossessedCards;

	for (FCardsData& TempCard : GameStateRef->AllCardsData) {
		if (TempCard.IsCardAvailable() && TempCard.CardType == ECardType::SpellCard) {
			SpellCardsArray.Add(&TempCard.CardRowName);
		}
	}
	if (SpellCardsArray.IsEmpty()) {
		NotifyNoAvailableSpellCards(TargetActor);
		Success = false;
		return RandomSpellCards;
	}
	const int32 FreeSlots = FMath::Clamp<int32>(LocalInventory->InventoryFreeSlots(false, true), 1, 3);
	if (FreeSlots > 1) {
		const int32 TheIndex = (FreeSlots == 2 || SpellCardsArray.Num() == 1) ? 1 : 2;
		for (int32 i = 0; i < TheIndex; i++) {
			int32 RandomNb = FMath::RandRange(0, SpellCardsArray.Num() - 1);
			RandomSpellCards.Add(*SpellCardsArray[RandomNb]);
			SpellCardsArray.RemoveAt(RandomNb);
		}
	}
	for (FName* TempSpellCard : SpellCardsArray) {
		bool bIsCardFound = false;

		for (int32 i = 0; i < LocalInventory->MaxCardsInBook; i++) {
			if (*TempSpellCard == LocalInventory->Book.Items[i].Card.CardRowName) {
				bIsCardFound = true;
				break;
			}
		}
		if (!bIsCardFound) {
			NotPossessedCards.Add(TempSpellCard);
		}
	}
	if (NotPossessedCards.IsEmpty()) {
		if (!SpellCardsArray.IsEmpty()) {
			RandomSpellCards.Add(*SpellCardsArray[FMath::RandRange(0, SpellCardsArray.Num() - 1)]);
		}
		else {
			LocalInventory->NotifyAlreadyObtained();
		}
	}
	else {
		RandomSpellCards.Add(*NotPossessedCards[FMath::RandRange(0, NotPossessedCards.Num() - 1)]);
	}
	Success = true;
	return RandomSpellCards;
}

void ASpellShop::TryBuySpellCards(AActor* TargetActor)
{
	if (TargetActor) {
		UInventoryComponent* Inventory = GetInventoryFromActor(TargetActor);
		if (!Inventory) {
			return;
		}
		if (Inventory->IsInventoryFull(false)) {
			Inventory->NotifyInventoryFull();
			return;
		}
		if (!Inventory->HasEnoughJenis(SpellCardsPack)) {
			return;
		}
		bool bSuccess = false;
		const TArray<FName> Cards = FindRandomSpellCards(TargetActor, bSuccess, Inventory);
		if (!bSuccess || !CheckDistance(TargetActor)) {
			return;
		}	
		TArray<FName> LastPack;
		bool bBought = false;
		for (const FName& LocalCard : Cards) {
			bool bLocalBought = Inventory->AddCard(LocalCard);
			bBought |= bLocalBought;
			if (bLocalBought) {
				GameStateRef->AddGlobalCardCount(LocalCard);
				LastPack.Add(LocalCard);
			}
		}
		if (bBought) {
			Inventory->CommitJenisAmount(SpellCardsPack * -1);
			Inventory->SetLastPack(LastPack);
		}
	}
}

void ASpellShop::TryBuyCardsFromShowcase(const FName& TheCard, AActor* TargetActor)
{
	if (!GameStateRef.Get()->ShowcaseCards.Contains(TheCard) || TargetActor == nullptr) {
		return;
	}
	FCardStruct* TempCard = ItemDataTable->FindRow<FCardStruct>(TheCard, "Context");
	if (!TempCard) {
		return;
	}
	UInventoryComponent* Inventory = GetInventoryFromActor(TargetActor);
	float Price = TempCard->BuyCost;
	if (Inventory != nullptr && CheckDistance(TargetActor)) {
		if (!Inventory->HasEnoughJenis(Price)) {
			NotifyInsufficientJenis(TargetActor);
			return;
		}
		if (Inventory->AddCard(TheCard)) {
			GameStateRef->AddGlobalCardCount(TheCard);
			Inventory->CommitJenisAmount(Price * -1);
		}
	}
}
