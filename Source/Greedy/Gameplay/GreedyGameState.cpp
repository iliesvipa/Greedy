// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Gameplay/GreedyGameState.h"
#include "Greedy/Inventory/InventoryComponent.h"
#include "Greedy/GreedyCharacter.h"
#include "Greedy/Gameplay/GreedyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AGreedyGameState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		for (FName& RowName : RowNames) {
			FCardStruct* CardStruct = ItemDataTable->FindRow<FCardStruct>(RowName, "Context", false);
			FCardsData TempCard;
			if (CardStruct) {
				TempCard.CardRowName = RowName;
				TempCard.CardType = CardStruct->CardType;
				TempCard.MaxCards = CardStruct->MaxCards;
				TempCard.CurrentCards = 0;
				AllCardsData.Add(TempCard);
				if ((CardStruct->CardType == ECardType::HealCard || CardStruct->CardType == ECardType::ItemCard || CardStruct->IsSwordOfTruth()) && !CardStruct->IsGalgaida()) {
					CardShops.Add(*CardStruct);
				}
			}
		}
		GetWorld()->GetTimerManager().SetTimer(ShowCaseTimer, this, &AGreedyGameState::GetShowcase, 6.f, true, 3.f); //60.f & 30.f
	}
}

void AGreedyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGreedyGameState, ShowcaseCards);
	DOREPLIFETIME(AGreedyGameState, CardShops);
}

void AGreedyGameState::RefreshCards(const FName& CardName, bool bExpire)
{
	FCardStruct* TempCard = ItemDataTable.Get()->FindRow<FCardStruct>(CardName, "Context");
	if (TempCard) {
		int32 Index;
		if (TempCard->CardType == ECardType::HealCard || TempCard->CardType == ECardType::ItemCard || TempCard->IsSwordOfTruth()) {
			if (bExpire) {
				bool bFound = CardShops.Find(*TempCard, Index);
				if (bFound) {
					CardShops.RemoveAt(Index);
				}
			}
			else {
				CardShops.Add(*TempCard);
			}
			if (HasAuthority()) {
				OnRep_CardShops();
			}
		}
		else if (TempCard->CardType == ECardType::SpellCard) {
			if (ShowcaseCards.Contains(CardName) && bExpire) {
				ShowcaseCards.Remove(*TempCard);
				if (HasAuthority()) {
					OnRep_Showcase();
				}
			}
		}
	}
}

void AGreedyGameState::GetShowcase()
{
	ShowcaseCards.Empty();

	TArray<FCardStruct*> CardArray;
	for (FCardsData& TempCard : AllCardsData) {
		if (TempCard.IsCardAvailable() && TempCard.CardType == ECardType::SpellCard && !TempCard.IsList()) {
			FCardStruct* TempStruct = ItemDataTable->FindRow<FCardStruct>(TempCard.CardRowName, "Context", false);
			if (TempStruct) {
				CardArray.Add(TempStruct);
			}
		}
	}
	if (CardArray.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("CardArray was empty !"));
		return;
	}
	TArray<FCardStruct*> RandomCards;
	int32 MaxIndex = FMath::Clamp(CardArray.Num(), 1, 3);
	for (int32 x = 0; x < MaxIndex; x++) {
		const int32 RandomIndex = FMath::RandRange(x, CardArray.Num() - 1);
		RandomCards.Add(CardArray[RandomIndex]);
		CardArray.Swap(x, RandomIndex);
	}

	for (FCardsData& TempCard : AllCardsData) {
		if (TempCard.IsList() && TempCard.IsCardAvailable()) {
			FCardStruct* TempStruct = ItemDataTable->FindRow<FCardStruct>("List", "Context");
			if (TempStruct) {
				RandomCards.Add(TempStruct);
			}
		}
	}
	for (FCardStruct* CardA : RandomCards) {
		ShowcaseCards.Add(*CardA);
	}
	if (HasAuthority()) {
		OnRep_Showcase();
	}
}

void AGreedyGameState::OnRep_Showcase()
{
	OnShowcaseUpdated.Broadcast(ShowcaseCards);
}

void AGreedyGameState::OnRep_CardShops()
{
	OnCardShopModified.Broadcast(CardShops);
}

void AGreedyGameState::AddGlobalCardCount(const FName& Card)
{
	for (FCardsData& CardData : AllCardsData) {
		if (CardData == Card && CardData.IsCardAvailable()) {
			CardData.CurrentCards += 1;
			if (!CardData.IsCardAvailable()) {
				RefreshCards(Card, true);
			}
			return;
		}
	}
}

void AGreedyGameState::RemoveGlobalCardCount(const FName& Card)
{
	for (FCardsData& CardData : AllCardsData) {
		if (CardData == Card && CardData.CurrentCards >= 0) {
			if (!CardData.IsCardAvailable()) {
				RefreshCards(Card, false);
			}
			CardData.CurrentCards -= 1;
			if (CardData.CurrentCards == 2 && CardData.IsAngelsBreath()) {
				GiveAngelsBreath();
			}
			return;
		}
	}
}

bool AGreedyGameState::FetchCard(const FName& CardName)
{
	for (FCardsData& CardData : AllCardsData) {
		if (CardData == CardName) {
			return CardData.IsCardAvailable();
		}
	}
	return false;
}

void AGreedyGameState::GiveAngelsBreath()
{
	TArray<UInventoryComponent*> InventoryPlayers;
	for (APlayerState* PS : PlayerArray) {
		APawn* Pawn = PS->GetPawn();
		if (AGreedyCharacter* Character = Cast<AGreedyCharacter>(Pawn)) {
			UInventoryComponent* Inventory = Character->GetInventory();
			if (Inventory && Inventory->bWaitsAngelsBreath) {
				InventoryPlayers.Add(Inventory);
			}
		}
	}
	if (!InventoryPlayers.IsEmpty()) {
		UInventoryComponent* InventoryPlayer = InventoryPlayers[FMath::RandRange(0, InventoryPlayers.Num() - 1)];
		if (InventoryPlayer) {
			InventoryPlayer->TransformVoucher();
			AddGlobalCardCount("AngelsBreath");
		}
	}
}

