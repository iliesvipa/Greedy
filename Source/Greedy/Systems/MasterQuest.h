// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Greedy/Systems/Interactable.h"
#include "MasterQuest.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestEnded, FName&, QuestName, bool, bSuccess); 

UCLASS()
class GREEDY_API AMasterQuest : public AInteractable
{
	GENERATED_BODY()

public:

	AMasterQuest();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FName QuestName;

	UPROPERTY(BlueprintAssignable)
	FOnQuestEnded QuestEnded;

	virtual void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void QuestLogic(class AGreedyCharacter* GreedyCharacter) {};

protected:
	virtual void BeginPlay() override;
};
