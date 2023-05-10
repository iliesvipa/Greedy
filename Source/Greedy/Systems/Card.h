// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "Card.generated.h"

UENUM(BlueprintType)
enum class ECardType : uint8
{
	GameCard,
	SpellCard,
	FreeCard,
	ItemCard,
	JenisCard, 
	HealCard, 
	NullCard
};

USTRUCT(BlueprintType)
struct FCardStruct : public FTableRowBase
{
	GENERATED_BODY()

	FCardStruct()
		: CardRowName(FName()), DisplayName(FText()), Icon(nullptr), BuyCost(0.f), MaxCards(-1), 
		CardType(ECardType::NullCard), bRestricted(false), bHasDetails(false)
	{}

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName CardRowName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float BuyCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 MaxCards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		ECardType CardType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bRestricted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FGameplayTag RestrictTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHasDetails;

	bool operator==(const FCardStruct& Other) const
	{
		return CardRowName == Other.CardRowName;
	}

	bool operator==(const FName& OtherName) const
	{
		return CardRowName == OtherName;
	}
	
	bool IsRulersBlessing() const
	{	
		return CardRowName == "RulersBlessing";
	}

	bool IsNullCard() const
	{
		return CardRowName == NAME_None; // this exists too : FName::IsNone()
	}

	bool IsJenisCard() const
	{
		return CardRowName == "Jenis";
	}

	bool IsListCard() const
	{
		return CardRowName == "List";
	}

	bool IsSwordOfTruth() const
	{
		return CardRowName == "SwordOfTruth";
	}

	bool IsVoucher() const
	{
		return CardRowName == "Voucher";
	}

	bool IsGalgaida() const
	{
		return CardRowName == "Galgaida";
	}
};
