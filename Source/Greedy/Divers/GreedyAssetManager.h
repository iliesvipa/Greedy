// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GreedyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API UGreedyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	void StartInitialLoading() override;
	
};
