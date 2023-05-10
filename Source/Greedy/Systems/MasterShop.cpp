// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Systems/MasterShop.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Greedy/Gameplay/GreedyGameState.h"

AMasterShop::AMasterShop()
{

}

void AMasterShop::BeginPlay()
{
	Super::BeginPlay();
	GameStateRef = Cast<AGreedyGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void AMasterShop::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FGameplayEventData EventData;
	EventData.Instigator = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, OnTag, EventData);
}

UInventoryComponent* AMasterShop::GetInventoryFromActor(AActor* TargetActor)
{
	AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(TargetActor);
	if (!GreedyChar) {
		return nullptr;
	}
	UInventoryComponent* Inventory = GreedyChar->GetInventory();
	if (!Inventory) {
		return nullptr;
	}
	return Inventory;
}

UQuestComponent* AMasterShop::GetQuestFromActor(AActor* TargetActor)
{
	AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(TargetActor);
	if (!GreedyChar) {
		return nullptr;
	}
	UQuestComponent* QuestComp = GreedyChar->GetQuestComponent();
	if (!QuestComp) {
		return nullptr;
	}
	return QuestComp;
}

bool AMasterShop::CheckDistance(AActor* TargetActor)
{
	if (TargetActor->GetDistanceTo(this) < 500.f && this->GetDistanceTo(TargetActor) < 500.f) {
		return true;
	}
	return false;
}