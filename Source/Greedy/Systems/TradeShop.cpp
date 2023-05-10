// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Systems/TradeShop.h"

ATradeShop::ATradeShop()
{
	OnTag = FGameplayTag::RequestGameplayTag(FName("Event.Interact.Shop.Trade"));
	BasicPrice = 3000.f;
	NotBasicPrice = 3000.f;
	QuestPrice = 10000.f;
	SellMultiplicater = 0.75f;
	CooldownTag = FGameplayTag::RequestGameplayTag(FName("Quest.Cooldown"));
}

void ATradeShop::SeePlayerCards(AActor* TargetActor, AActor* Target, bool BonusTip)
{
	if (TargetActor == nullptr || TargetActor == Target) {
		return;
	}
	UInventoryComponent* Inventory = GetInventoryFromActor(TargetActor);
	UInventoryComponent* TargetInventory = GetInventoryFromActor(Target);
	if (!Inventory || !TargetInventory || !CheckDistance(TargetActor)) {
		return;
	}
	float Price = BasicPrice + BonusTip ? NotBasicPrice : 0.f;
	if (!Inventory->HasEnoughJenis(Price)) {
		NotifyInsufficientJenis(TargetActor);
		return;
	}
	int32 NbOfCards = 0;
	TArray<int32> GameCardsSlot = TargetInventory->FindSlotsInRange(0, 5, false);
	if (!BonusTip) {
		NbOfCards = GameCardsSlot.Num();
		Inventory->NbTargetCardsRevealed = NbOfCards;
		return;
	}
	TArray<FName> TestCard; // Declare it as FCardStruct ?
	for (int32 i = 0; i < GameCardsSlot.Num() - 1; i++) {
		TestCard.Add(TargetInventory->Book.Items[i].Card.CardRowName);
	}

	Inventory->TargetCardsRevealed.Reset();
	Inventory->TargetCardsRevealed = TestCard;
}

void ATradeShop::TrySellCards(const TArray<FName>& CardsToSell, AActor* TargetActor)
{
	if (TargetActor == nullptr) {
		return;
	}
	UInventoryComponent* Inventory = GetInventoryFromActor(TargetActor);
	if (!Inventory || !CheckDistance(TargetActor)) {
		return;
	}
	for (const FName& Card : CardsToSell) {
		FCardStruct* TempCard = ItemDataTable->FindRow<FCardStruct>(Card, "Context");
		if (TempCard) {
			if (!TempCard->IsNullCard() && !TempCard->IsJenisCard() && Inventory->Book.Items.Contains(*TempCard)) {
				if (Inventory->RemoveCardByName(Card)) {
					GameStateRef.Get()->RemoveGlobalCardCount(Card);
					Inventory->CommitJenisAmount(TempCard->BuyCost * SellMultiplicater);
				}
			}
		}
	}
}

void ATradeShop::GiveQuest(AActor* TargetActor)
{
	if (TargetActor == nullptr) {
		return;
	}
	AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(TargetActor);
	if (!GreedyChar) {
		return;
	}
	UQuestComponent* QuestComp = GreedyChar->GetQuestComponent();
	UInventoryComponent* Inventory = GreedyChar->GetInventory();
	UAbilitySystemComponent* ASC = GreedyChar->GetAbilitySystemComponent();
	if (!QuestComp || !Inventory || !ASC || !CheckDistance(TargetActor)) {
		return;
	}
	if (ASC->HasMatchingGameplayTag(CooldownTag)) {
		return;
	}
	TArray<FName> RowNames = QuestDT->GetRowNames();
	TArray<FName> Keys;
	QuestComp->AllQuestsStatus.GetKeys(Keys);
	for (FName& LocalQuestName : Keys) {
		if (RowNames.Contains(LocalQuestName)) {
			RowNames.Remove(LocalQuestName);
		}
	}
	if (RowNames.IsEmpty()) {
		NotifyAllQuestDone(TargetActor);
		return;
	}
	if (!Inventory->HasEnoughJenis(QuestPrice)) {
		return;
	}
	Inventory->CommitJenisAmount(QuestPrice * -1);
	FQuestInfos* TempQuest = nullptr;
	for (int32 i = 0; i < RowNames.Num(); i++) {
		int32 Index = FMath::RandRange(i, RowNames.Num() - 1);
		TempQuest = QuestDT->FindRow<FQuestInfos>(RowNames[Index], "Context");
		if (TempQuest && TempQuest->bMainQuest && TempQuest->bInternal) {
			break;
		}
		RowNames.Swap(Index, i);
	}
	QuestComp->AddQuest(TempQuest->QuestName, EQuestStatus::Accepted);
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CooldownGE, 1, ContextHandle);
	if (SpecHandle.IsValid()) {
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), ASC->GetPredictionKeyForNewAction());
	}
}

void ATradeShop::GiveVoucher(AActor* TargetActor)
{
	if (TargetActor == nullptr) {
		return;
	}
	if (AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(TargetActor)) {
		UInventoryComponent* Inventory = GreedyChar->GetInventory();
		if (Inventory) {
			if (CheckDistance(TargetActor) && Inventory->bHasAllSpellCards) { 
				bool bAngel = GameStateRef->FetchCard("AngelsBreath");
				FName TempCardName = bAngel ? "AngelsBreath" : "Voucher";
				Inventory->RemoveEachSpellCard();
				Inventory->AddCard(TempCardName);
				if (bAngel) {
					Inventory->bWaitsAngelsBreath = Inventory->CheckForVoucher();
				}
				GameStateRef->AddGlobalCardCount(TempCardName);
			}
		}
	}
}
