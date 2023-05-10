// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Divers/GreedyAssetManager.h"
#include "AbilitySystemGlobals.h"

void UGreedyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}