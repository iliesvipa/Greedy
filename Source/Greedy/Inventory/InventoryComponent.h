// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Engine/NetSerialization.h"
#include "Greedy/Systems/Card.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBookUpdated, const FName&, CardName, int32, CardIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLastPackBought, const TArray<FName>&, Cards);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVoucherGet, bool, WaitingAngelsBreath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJenisModified, float, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardChanges, int32, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRestrictChanged, const FGameplayTag&, RestrictTag, bool, bNew);

USTRUCT(BlueprintType)
struct FCardItem : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY()

	FCardItem() : Card() {}

	UPROPERTY(BlueprintReadWrite)
	FCardStruct Card;

	bool operator==(const FCardItem& Other) const
	{
		return Card.CardRowName == Other.Card.CardRowName;
	}
	bool operator==(const FCardStruct& Other) const
	{
		return Card.CardRowName == Other.CardRowName;
	}

};

USTRUCT(BlueprintType)
struct FCardContainer : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FCardItem> Items;

	UInventoryComponent* BookOwner;

	bool bIsNullCard;

	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FCardItem, FCardContainer>(Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FCardContainer> : public TStructOpsTypeTraitsBase2<FCardContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

USTRUCT(BlueprintType)
struct FRestrictTags
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer RestrictTags;

	UPROPERTY()
	FGameplayTag LastTag;

	UPROPERTY()
	bool bAdded;
};

USTRUCT(BlueprintType)
struct FCardDetails : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CardName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText OptionalDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> CardAbility;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GREEDY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	static const int32 MaxCardsInBook = 17;
	static const int32 CardsToBuyForCardShop = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
	FCardContainer Book;

	UPROPERTY(BlueprintAssignable)
	FOnBookUpdated OnBookUpdated;

	UPROPERTY(BlueprintReadOnly)
	FName LastUsedCardAbility;

	// Restricted Tags : START
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_RestrictedTags)
	FRestrictTags RestrictedTags;

	UFUNCTION()
	void OnRep_RestrictedTags();
	
	UFUNCTION(BlueprintCallable)
	void AddGameplayTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasMatchingGameplayTag(const FGameplayTag& Tag) const;
	// Restricted Tags : END

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FGameplayTag SwordTag;

	UFUNCTION(BlueprintCallable)
	bool AddCard(const FName& CardName);
	UFUNCTION(BlueprintCallable)
	void RemoveCardByIndex(int32 InIndex);
	UFUNCTION(BlueprintCallable)
	bool RemoveCardByName(const FName& CardName);
	int32 FindEmptyOrFullSlot(bool GameSlots, bool EmptySlots) const;

	bool CheckForVoucher() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	TObjectPtr<UDataTable> ItemDT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	TObjectPtr<UDataTable> CardAbilitiesDT;

	UFUNCTION()
	void GiveCardAbility(const FName& CardName, const TSubclassOf<UGameplayAbility>& CardAbility);

	UFUNCTION()
	void RemoveCardAbility(const FName& CardName, const TSubclassOf<UGameplayAbility>& CardAbility);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void FetchCardAbility(const FName& CardName);
	void FetchCardAbility_Implementation(const FName& CardName);
	bool FetchCardAbility_Validate(const FName& CardName);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jenis", ReplicatedUsing = OnRep_Jenis)
	float JenisAmount;

	// HUD Helper Values..
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentSpellCards)
	int32 CurrentSpellCards;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CardShopBuys)
	int32 CardShopBuys;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_QuickSlot)
	FName Quickslot;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaitingAngelsBreath)
	bool bWaitsAngelsBreath;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bHasAllSpellCards;

	UPROPERTY(BlueprintAssignable)
	FOnJenisModified OnJenisModified;

	UPROPERTY(BlueprintAssignable)
	FOnLastPackBought OnLastPackBought;

	UPROPERTY(BlueprintAssignable)
	FOnCardChanges OnCurrentSpellCards;

	UPROPERTY(BlueprintAssignable)
	FOnCardChanges OnCardShopBuys;

	UPROPERTY(BlueprintAssignable)
	FOnVoucherGet OnVoucherGet;

	UPROPERTY(BlueprintAssignable)
	FOnRestrictChanged OnRestrictChanged;

	// OnRep  

	UFUNCTION()
	void OnRep_CurrentSpelLCards();

	UFUNCTION()
	void OnRep_WaitingAngelsBreath();

	UFUNCTION()
	void OnRep_Jenis();

	UFUNCTION()
	void OnRep_LastPackBought();

	UFUNCTION()
	void OnRep_QuickSlot();

	UFUNCTION()
	void OnRep_CardShopBuys();

	// Inventory Helper Functions

	UFUNCTION(BlueprintCallable)
	const bool IsInventoryFull(bool bGameSlots) const;

	UFUNCTION(BlueprintCallable)
	const int32 InventoryFreeSlots(bool bGameSlots, bool bFreeSlots) const;

	// Spell Card Interface

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FCardStruct> ViewCards(bool bGameSlots);

	UFUNCTION(BlueprintCallable)
	bool GiveCard(bool bGameSlots, FName& CardName);

	// Money Interactions 

	UFUNCTION(BlueprintCallable)
	bool HasEnoughJenis(float InJenis) const;

	UFUNCTION(BlueprintCallable)
	void CommitJenisAmount(float InJenis);
	
	// Notifies
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyInventoryFull();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyAlreadyObtained();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyVoucherAtTradeShop();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyVoucherNotAvailable();

	// Voucher stuff

	UFUNCTION()
	void TransformVoucher();

	// Spell Cards stuff

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasAllSpellCards() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CountDifferentSpellCards() const;

	UFUNCTION()
	void RemoveEachSpellCard();

	// Gameplay Effects 

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<class UGameplayEffect> SwordOfTruthGE;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_LastPackBought)
	TArray<FName> LastPackBought;

	UFUNCTION()
	void AddCardShopBuys();

	UFUNCTION(BlueprintCallable)
	void SetQuickslotCard(const FName& InQuickslot);

	UFUNCTION()
	void SetLastPack(TArray<FName> CardsPack);

	UFUNCTION(BlueprintCallable)
	const TArray<FCardStruct> GetCardRewards();

	UFUNCTION(BlueprintCallable)
	TArray<int32> FindSlotsInRange(int32 StartIndex, int32 EndIndex, bool bFreeSlots) const;

	void SetTargetCards(FName InName, int32 InInt);

	// Needs to be changed !!
	UPROPERTY(BlueprintReadOnly)
		int32 NbTargetCardsRevealed;

	UPROPERTY(BlueprintReadOnly)
		TArray<FName> TargetCardsRevealed;

protected:
	virtual void BeginPlay() override;	
};
