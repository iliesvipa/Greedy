// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGreedyAbilityInputID : uint8
{
	None UMETA(DisplayName = "None"), 
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Jump UMETA(DisplayName = "Jump"),
	Interact UMETA(DisplayName = "Interact"),
	AbilityM1 UMETA(DisplayName = "AbilityM1"),
	AbilityM2 UMETA(DisplayName = "AbilityM2"),
	AbilityDefense UMETA(DisplayName = "AbilityDef"),
	AbilityMobility UMETA(DisplayName = "AbilityMobi"),
	AbilityControl UMETA(DisplayName = "AbilityCC"),
	AbilityHatsu UMETA(DisplayName = "AbilityHatsu"),
	QuickSlot UMETA(DisplayName = "Quickslot")
};

template <typename T>
void ShuffleArray(TArray<T>& TheArray)
{
	if (TheArray.Num() > 0)
	{
		int32 LastIndex = TheArray.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				TheArray.Swap(i, Index);
			}
		}
	}
}