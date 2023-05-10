// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Systems/Interactable.h"
#include "Components/BoxComponent.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollision->SetupAttachment(RootComponent);
	OffTag = FGameplayTag::RequestGameplayTag(FName("Event.Interact.Off"));
	Tags.Add(FName("Component.Interact"));
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::StartOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractable::EndOverlap);	
}


void AInteractable::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("The StartOverlap should be subclassed from child classes"));
}

void AInteractable::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FGameplayEventData EventData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, OffTag, EventData);
}


