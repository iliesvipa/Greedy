// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Gameplay/GreedyPlayerController.h"
#include "Greedy/GreedyCharacter.h"
#include "Greedy/Inventory/InventoryComponent.h"

void AGreedyPlayerController::OnPossess(APawn* P)
{
	Super::OnPossess(P);
	GreedyCharacter = Cast<AGreedyCharacter>(P);
}
