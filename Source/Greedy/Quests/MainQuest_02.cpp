// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Quests/MainQuest_02.h"
#include "Greedy/Inventory/QuestComponent.h"
#include "Greedy/Inventory/InventoryComponent.h"
#include "Greedy/GreedyCharacter.h"


void AMainQuest_02::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(OtherActor)) {
		if (UQuestComponent* LocalQuestComp = GreedyChar->GetQuestComponent()) {
			bool bFoundQuest = LocalQuestComp->AllQuestsStatus.Contains(QuestName);
			if (!bFoundQuest) {
				return;
			}
			QuestLogic(GreedyChar);
		}
	}

}

void AMainQuest_02::QuestLogic(AGreedyCharacter* GreedyCharacter)
{
	// Alexandrite Logic // 
	if (UQuestComponent* QuestComp = GreedyCharacter->GetQuestComponent()) {
		QuestComp->ExecuteAbility(QuestName);
		QuestEnded.Broadcast(QuestName, true);
	}
}
