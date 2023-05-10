// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Systems/MasterItem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

AMasterItem::AMasterItem()
{
	OnTag = FGameplayTag::RequestGameplayTag(FName("Event.Interact.Item"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMasterItem, CardStruct);
	DOREPLIFETIME(AMasterItem, ItemName);
}

void AMasterItem::BeginPlay() {
	Super::BeginPlay();
	if (ItemName != "Null") {
		FCardStruct* TempItem = ItemDataTable->FindRow<FCardStruct>(ItemName, "Context", false);
		if (TempItem != nullptr) {
			CardStruct = *TempItem;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Item name is not found in the data table. Please setup it in Blueprints"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Item name is null name"));
	}
}

void AMasterItem::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FGameplayEventData EventData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, OnTag, EventData);
}