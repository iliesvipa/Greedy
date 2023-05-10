// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Greedy/Systems/MasterQuest.h"
#include "MainQuest_01.generated.h"


/**
 * 
 */
UCLASS()
class GREEDY_API AMainQuest_01 : public AMasterQuest
{
	GENERATED_BODY()

public:

	void QuestLogic(AGreedyCharacter* GreedyCharacter) override;

	TArray<FName> Things;

	void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
