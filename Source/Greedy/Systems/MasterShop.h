// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Greedy/Systems/Interactable.h"
#include "Engine/DataTable.h"
#include "Greedy/Systems/Card.h"
#include "Greedy/Gameplay/GreedyGameState.h"
#include "Greedy/GreedyCharacter.h"
#include "Greedy/Inventory/InventoryComponent.h"
#include "Greedy/Inventory/QuestComponent.h"
#include "MasterShop.generated.h"

/**
 * 
 */
UCLASS()
class GREEDY_API AMasterShop : public AInteractable
{
	GENERATED_BODY()

public:

	AMasterShop();

	UPROPERTY()
	TWeakObjectPtr<class AGreedyGameState> GameStateRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	TObjectPtr<UDataTable> ItemDataTable;

	virtual void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Notifies //

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyInsufficientJenis(AActor* TargetActor);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyInventoryFull(AActor* TargetActor);

protected:

	virtual void BeginPlay() override;
	UInventoryComponent* GetInventoryFromActor(AActor* TargetActor);
	UQuestComponent* GetQuestFromActor(AActor* TargetActor);
	bool CheckDistance(AActor* TargetActor);
};
