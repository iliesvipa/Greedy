// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Inventory/InventoryComponent.h"
#include "AbilitySystemComponent.h"
#include "Greedy/Gameplay/GreedyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Greedy/GreedyCharacter.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	JenisAmount = 0.f;
	CurrentSpellCards = 0; 
	CardShopBuys = 0;
	bWaitsAngelsBreath = false;
	bHasAllSpellCards = false; 
}

void FCardContainer::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	if (ChangedIndices.IsEmpty()) {
		return;
	}
	if (APawn* MyPawn = Cast<APawn>(BookOwner->GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			for (int32 i = 0; i < ChangedIndices.Num(); i++) {
				int32 Index = ChangedIndices[i];
				if (bIsNullCard) {}
				FName CardName = bIsNullCard ? FName() : Items[Index].Card.CardRowName;
				BookOwner->OnBookUpdated.Broadcast(CardName, Index);
			}
		}
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Book.BookOwner = this;
	if (GetOwnerRole() == ENetRole::ROLE_Authority) {
		Book.OwningObject = this;
		FCardItem NullCardItem = FCardItem();
		for (int32 i = 0; i < MaxCardsInBook; i++) {
			Book.Items.Add(NullCardItem);
			Book.MarkItemDirty(NullCardItem);
		}
		FCardStruct* TempCard = ItemDT->FindRow<FCardStruct>("Jenis", "Context");
		if (TempCard) {
			Book.Items[5].Card = *TempCard;
			Book.MarkItemDirty(Book.Items[5]);
		}
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, JenisAmount);
	DOREPLIFETIME(UInventoryComponent, CurrentSpellCards);
	DOREPLIFETIME(UInventoryComponent, LastPackBought);
	DOREPLIFETIME(UInventoryComponent, CardShopBuys);
	DOREPLIFETIME(UInventoryComponent, bWaitsAngelsBreath);
	DOREPLIFETIME(UInventoryComponent, Book);
	DOREPLIFETIME(UInventoryComponent, bHasAllSpellCards);
	DOREPLIFETIME(UInventoryComponent, RestrictedTags);
}

bool UInventoryComponent::AddCard(const FName& CardName)
{
	FCardStruct* TempCard = ItemDT->FindRow<FCardStruct>(CardName, "Context");
	if (!TempCard) {
		return false;
	}
	Book.bIsNullCard = false;
	if (CardName == "RulersBlessing") {
		Book.Items[0].Card = *TempCard;
		Book.MarkItemDirty(Book.Items[0]);
		return true;
	}
	bool bGameSlots = TempCard->CardType == ECardType::GameCard ? true : false;
	int32 Slot = FindEmptyOrFullSlot(bGameSlots, true);
	if (Slot == -1) {
		NotifyInventoryFull();
		return false;
	}
	Book.Items[Slot].Card = *TempCard;
	Book.MarkItemDirty(Book.Items[Slot]);
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnBookUpdated.Broadcast(Book.Items[Slot].Card.CardRowName, Slot);
		}
	}
	if (TempCard->IsVoucher()) {
		bWaitsAngelsBreath = true;
	}
	if (TempCard->bHasDetails) {
		FCardDetails* TempCardBinding = CardAbilitiesDT->FindRow<FCardDetails>(TempCard->CardRowName, "Context");
		if (TempCardBinding) {
			if (TempCardBinding->bHasAbility) {
				GiveCardAbility(TempCard->CardRowName, TempCardBinding->CardAbility);
			}
		}
	}

	if (TempCard->CardType == ECardType::SpellCard) {
		CurrentSpellCards++;
		if (HasAllSpellCards()) {
			bHasAllSpellCards = true;
		}
		else {
			bHasAllSpellCards = false;
		}
	}
	return true;
}

void UInventoryComponent::GiveCardAbility(const FName& CardName, const TSubclassOf<UGameplayAbility>& CardAbility)
{
	if (AGreedyCharacter* Character = Cast<AGreedyCharacter>(GetOwner())) {
		if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent()) {
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(CardAbility, 1, -1);
			ASC->GiveAbility(Spec);
		}
	}
}

void UInventoryComponent::RemoveCardAbility(const FName& CardName, const TSubclassOf<UGameplayAbility>& CardAbility)
{
	if (AGreedyCharacter* Character = Cast<AGreedyCharacter>(GetOwner())) {
		if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent()) {
			FGameplayAbilitySpec* LocalSpec = ASC->FindAbilitySpecFromClass(CardAbility);
			ASC->ClearAbility(LocalSpec->Handle);
		}
	}
}

void UInventoryComponent::FetchCardAbility_Implementation(const FName& CardName)
{
	FCardDetails* TempCard = CardAbilitiesDT->FindRow<FCardDetails>(CardName, "Context");
	if (TempCard && TempCard->bHasAbility) {
		if (AGreedyCharacter* Character = Cast<AGreedyCharacter>(GetOwner())) {
			if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent()) {
				if (ASC->TryActivateAbilityByClass(TempCard->CardAbility)) {
					LastUsedCardAbility = CardName;
				}
			}
		}
	}
}	

bool UInventoryComponent::FetchCardAbility_Validate(const FName& CardName)
{
	return true;
}

int32 UInventoryComponent::FindEmptyOrFullSlot(bool GameSlots, bool EmptySlots) const
{
	int32 Index = GameSlots ? 1 : 6;
	int32 MaxIndex = GameSlots ? 5 : MaxCardsInBook;
	for (int32 i = Index; i < MaxIndex; i++) {
		if (Book.Items[i].Card.IsNullCard() == EmptySlots) {
			return i;
		}
	}
	return -1;
}

bool UInventoryComponent::HasMatchingGameplayTag(const FGameplayTag& Tag) const
{
	return RestrictedTags.RestrictTags.HasTagExact(Tag);
}

void UInventoryComponent::OnRep_RestrictedTags()
{
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnRestrictChanged.Broadcast(RestrictedTags.LastTag, RestrictedTags.bAdded);
		}
	}
}

void UInventoryComponent::AddGameplayTag(const FGameplayTag& Tag)
{
	RestrictedTags.bAdded = true;
	RestrictedTags.LastTag = Tag;
	RestrictedTags.RestrictTags.AddTag(Tag);
}

void UInventoryComponent::RemoveGameplayTag(const FGameplayTag& Tag)
{
	RestrictedTags.bAdded = false;
	RestrictedTags.LastTag = Tag;
	RestrictedTags.RestrictTags.RemoveTag(Tag);
}

void UInventoryComponent::RemoveCardByIndex(int32 InIndex)
{
	FCardStruct& TempCard = Book.Items[InIndex].Card;
	bool bHasSpellCards = false;
	if (TempCard.CardType == ECardType::SpellCard) {
		if (HasAllSpellCards()) {
			bHasSpellCards = true;
		}
		CurrentSpellCards--;
	}

	if (TempCard.bHasDetails) {
		FCardDetails* TempCardBinding = CardAbilitiesDT->FindRow<FCardDetails>(TempCard.CardRowName, "Context");
		if (TempCardBinding) {
			if (TempCardBinding->bHasAbility) {
				RemoveCardAbility(TempCard.CardRowName, TempCardBinding->CardAbility);
			}
		}
	}

	TempCard = FCardStruct();
	Book.bIsNullCard = true;
	Book.MarkItemDirty(Book.Items[InIndex]);
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnBookUpdated.Broadcast(FName(), InIndex);
		}
	}
	if (!HasAllSpellCards() && bHasSpellCards) {
		bHasAllSpellCards = false;
	}
}

bool UInventoryComponent::RemoveCardByName(const FName& CardName)
{
	for (int32 i = 0; i < MaxCardsInBook; i++) {
		FCardStruct& TempCard = Book.Items[i].Card;
		if (TempCard == CardName) {
			bool bHasSpellCards = false;
			if (TempCard.CardType == ECardType::SpellCard) {
				if (HasAllSpellCards()) {
					bHasSpellCards = true;
				}
				CurrentSpellCards--;
			}

			if (TempCard.bHasDetails) {
				FCardDetails* TempCardBinding = CardAbilitiesDT->FindRow<FCardDetails>(TempCard.CardRowName, "Context");
				if (TempCardBinding) {
					if (TempCardBinding->bHasAbility) {
						RemoveCardAbility(TempCard.CardRowName, TempCardBinding->CardAbility);
					}
				}
			}
			TempCard = FCardStruct();
			Book.bIsNullCard = true;
			Book.MarkItemDirty(Book.Items[i]);
			if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
				if (MyPawn->IsLocallyControlled()) {
					OnBookUpdated.Broadcast(FName(), i);
				}
			}
			if (!HasAllSpellCards() && bHasSpellCards) {
				bHasAllSpellCards = false;
			}
			return true;
		}
	}
	return false;
}

TArray<FCardStruct> UInventoryComponent::ViewCards(bool bGameSlots)
{
	/*TArray<FCardStruct> LocalArray;
	for (FCardStruct& LocalCard : InventoryContent) {
		if ((LocalCard.CardType == ECardType::GameCard) == bGameSlots) {
			LocalArray.Add(LocalCard);
		}
	}
	return LocalArray;*/
	TArray<FCardStruct> Test;
	return Test;
}

bool UInventoryComponent::GiveCard(bool bGameSlots, FName& CardName)
{
	TArray<FCardItem*> TempArray;
	for (FCardItem& TempCard : Book.Items) {
		if ((TempCard.Card.CardType == ECardType::GameCard) == bGameSlots &&
			TempCard.Card.CardType != ECardType::NullCard && TempCard.Card.CardType != ECardType::JenisCard) {
			TempArray.Add(&TempCard);
		}
	}
	if (TempArray.IsEmpty()) {
		return false;
	}
	CardName = TempArray[FMath::RandRange(0, TempArray.Num() - 1)]->Card.CardRowName;
	return true;
}

bool UInventoryComponent::HasEnoughJenis(float InJenis) const
{
	return JenisAmount > InJenis;
}

void UInventoryComponent::CommitJenisAmount(float InJenis)
{
	JenisAmount += InJenis;
	if (GetOwner()->HasAuthority()) {
		OnRep_Jenis();
	}
}

void UInventoryComponent::OnRep_Jenis()
{
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnJenisModified.Broadcast(JenisAmount);
		}
	}
}

void UInventoryComponent::OnRep_CurrentSpelLCards()
{
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnCurrentSpellCards.Broadcast(CurrentSpellCards);
		}
	}
}

void UInventoryComponent::OnRep_WaitingAngelsBreath()
{
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnVoucherGet.Broadcast(bWaitsAngelsBreath);
		}
	}
}

void UInventoryComponent::OnRep_CardShopBuys()
{
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnCardShopBuys.Broadcast(CardShopBuys);
		}
	}
}

void UInventoryComponent::OnRep_QuickSlot()
{

}

void UInventoryComponent::SetLastPack(TArray<FName> CardsPack)
{
	LastPackBought.Reset();
	LastPackBought = CardsPack;
	if (GetOwner()->HasAuthority()) {
		OnRep_LastPackBought();
	}
}

void UInventoryComponent::OnRep_LastPackBought()
{
	if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
		if (MyPawn->IsLocallyControlled()) {
			OnLastPackBought.Broadcast(LastPackBought);
		}
	}
}

const bool UInventoryComponent::IsInventoryFull(bool bGameSlots) const
{
	return (FindEmptyOrFullSlot(true, true) == 0 && FindEmptyOrFullSlot(false, true) == 0);
}

const int32 UInventoryComponent::InventoryFreeSlots(bool bGameSlots, bool bFreeSlots) const
{
	int32 Count = 0;
	int32 Index = 6;
	int32 MaxIndex = MaxCardsInBook;
	if (bGameSlots) {
		Index = 0;
		MaxIndex = 4;
	}
	for (int32 i = Index; i < MaxIndex; i++) {
		if (Book.Items[i].Card.IsNullCard() == bFreeSlots) {
			Count++;
		}
	}
	return Count;
}

bool UInventoryComponent::HasAllSpellCards() const
{
	return CountDifferentSpellCards() == 8; // 8 is the max spell cards..
}

int32 UInventoryComponent::CountDifferentSpellCards() const
{
	TArray<FName> SpellCards;
	for (int32 i = 6; i < MaxCardsInBook; i++) {
		if (Book.Items[i].Card.CardType == ECardType::SpellCard && !SpellCards.Contains(Book.Items[i].Card.CardRowName)) {
			SpellCards.Add(Book.Items[i].Card.CardRowName);
		}
	}
	return SpellCards.Num();
}

void UInventoryComponent::AddCardShopBuys()
{
	CardShopBuys += 1;
	if (CardShopBuys == CardsToBuyForCardShop) {
		AddGameplayTag(SwordTag);
	}
}

const TArray<FCardStruct> UInventoryComponent::GetCardRewards()
{
	TArray<FCardStruct> TempCards;
	/*TArray<FCardStruct> Cards;
	if (InventoryContent[0].IsRulersBlessing()) {
		TempCards.Add(&InventoryContent[0]); 
	}
	RemoveCardByIndex(0);
	if (!IsInventoryFull(true)) {
		for (int32 i = 0; i < InventoryFreeSlots(true, false); i += 2) {
			FCardStruct* TempCard;
			TArray<int32> LocalArray = FindSlotsInRange(1, 5, false);
			int32 RandomIndex = FMath::RandRange(0, LocalArray.Num() - 1);
			TempCard = &InventoryContent[LocalArray[RandomIndex]];
			TempCards.Add(TempCard);
			RemoveCardByIndex(RandomIndex);
		}

	}
	if (!IsInventoryFull(false)) {
		for (int32 i = 0; i < InventoryFreeSlots(false, false); i += 4) {
			FCardStruct* TempCard;
			TArray<int32> LocalArray = FindSlotsInRange(6, MaxCardsInBook, false);
			int32 RandomIndex = FMath::RandRange(0, LocalArray.Num() - 1);
			TempCard = &InventoryContent[LocalArray[RandomIndex]];
			TempCards.Add(TempCard);
			RemoveCardByIndex(RandomIndex);
		}
	}
	for (FCardStruct* TempCard : TempCards) {
		Cards.Add(*TempCard);
	}
	TArray<int32> PendingCards = FindSlotsInRange(1, MaxCardsInBook, false);
	for (int32 PendingCard : PendingCards) {
		AGreedyGameState* GreedyGS = Cast<AGreedyGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (GreedyGS) {
			GreedyGS->RemoveGlobalCardCount(InventoryContent[PendingCard].CardRowName);
			RemoveCardByIndex(PendingCard);
		}
	}
	return Cards;*/
	return TempCards;
}

TArray<int32> UInventoryComponent::FindSlotsInRange(int32 StartIndex, int32 EndIndex, bool bFreeSlots) const
{
	TArray<int32> TempArray;
	if (StartIndex >= EndIndex) {
		return TempArray;
	}
	for (int32 i = StartIndex; i < EndIndex; i++) {
		if (Book.Items[i].Card.IsNullCard() == bFreeSlots) {
			TempArray.Add(i);
		}
	}
	return TempArray;
}

void UInventoryComponent::RemoveEachSpellCard()
{
	TArray<FName> SpellCards;
	FCardItem TempCardItem = FCardItem();
	for (int32 i = 6; i < MaxCardsInBook; i++) {
		if (Book.Items[i].Card.CardType == ECardType::SpellCard && !SpellCards.Contains(Book.Items[i].Card.CardRowName)) {
			SpellCards.Add(Book.Items[i].Card.CardRowName);
			Book.Items[i] = TempCardItem;
			Book.MarkItemDirty(Book.Items[i]);
			CurrentSpellCards--;
			if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
				if (MyPawn->IsLocallyControlled()) {
					OnBookUpdated.Broadcast(Book.Items[i].Card.CardRowName, i);
				}
			}
		}
	}
	bHasAllSpellCards = false;
}

void UInventoryComponent::TransformVoucher()
{
	for (int32 i = 0; i < Book.Items.Num(); i++) {
		if (Book.Items[i].Card.IsVoucher()) {
			FCardStruct* TempCard = ItemDT->FindRow<FCardStruct>("AngelsBreath", "Context");
			if (TempCard) {
				Book.Items[i].Card = *TempCard;
				Book.MarkItemDirty(Book.Items[i]);
				if (APawn* MyPawn = Cast<APawn>(GetOwner())) {
					if (MyPawn->IsLocallyControlled()) {
						OnBookUpdated.Broadcast(Book.Items[i].Card.CardRowName, i);
					}
				}
				bWaitsAngelsBreath = CheckForVoucher();
			}
		}
	}
}

bool UInventoryComponent::CheckForVoucher() const
{
	for (const FCardItem& TempCardItem : Book.Items) {
		if (TempCardItem.Card.IsVoucher()) {
			return true;
		}
	}
	return false;
}

void UInventoryComponent::SetQuickslotCard(const FName& InQuickslot)
{
	Quickslot = InQuickslot;
}

void UInventoryComponent::SetTargetCards(FName InName, int32 InInt)
{
	NbTargetCardsRevealed = InInt;
	TargetCardsRevealed.Add(InName);
}