// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Systems/MasterQuest.h"
#include "Greedy/GreedyCharacter.h"


AMasterQuest::AMasterQuest()
{
	OnTag = FGameplayTag::RequestGameplayTag(FName("Event.Interact.Quest"));
}

void AMasterQuest::BeginPlay()
{
	Super::BeginPlay();
}

void AMasterQuest::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FGameplayEventData EventData;
	EventData.Instigator = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, OnTag, EventData);
}