// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Quests/MainQuest_01.h"
#include "Greedy/Inventory/QuestComponent.h"
#include "Greedy/Inventory/InventoryComponent.h"
#include "Greedy/GreedyCharacter.h"


void AMainQuest_01::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AMainQuest_01::QuestLogic(AGreedyCharacter* GreedyCharacter)
{
	// Jackpot Logic // 
	bool Random = FMath::RandBool();
	TArray<FName> SomeStuff;
	int32 MaxIndex = Random ? 1 : 3;

	for (int32 i = 0; i < MaxIndex; i++) {
		int32 RandomIndex = FMath::RandRange(0, Things.Num() - 1);
		SomeStuff.Add(Things[RandomIndex]);
	}
	
	QuestEnded.Broadcast(QuestName, Random);

	//// Sword Of Truth
	//if (UQuestComponent* QuestComp = GreedyCharacter->GetQuestComponent()) {
	//	QuestComp->ExecuteAbility(QuestName);
	//	QuestEnded.Broadcast(QuestName, true);
	//}
}