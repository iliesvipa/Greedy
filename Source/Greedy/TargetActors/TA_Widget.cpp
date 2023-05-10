// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/TargetActors/TA_Widget.h"
#include "Greedy/Widgets/GameplayWidget.h"
#include "Abilities/GameplayAbility.h"


ATA_Widget::ATA_Widget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	ShouldProduceTargetDataOnServer = false;
}

void ATA_Widget::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Widget) {
		Widget->OnPlayerSelected.RemoveDynamic(this, &ATA_Widget::ReactPlayerSelected);
		Widget->OnDestructCalled.RemoveDynamic(this, &ATA_Widget::ReceiveCancel);
	}
	Super::EndPlay(EndPlayReason);
}

void ATA_Widget::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();
	BindToWidgetSelection();
}

void ATA_Widget::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor && IsConfirmTargetingAllowed())
	{
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

void ATA_Widget::ReactPlayerSelected(APlayerState* PS)
{
	FVector VectorA;
	FVector VectorB;
	AActor* Actor = Cast<AActor>(PS);
	const FHitResult HitResult(Actor, nullptr, VectorA, VectorB);
	if (OwningAbility) {
		Handle = StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, HitResult);
		ConfirmTargeting();
	}
}

void ATA_Widget::BindToWidgetSelection()
{
	if (MasterPC && MasterPC->IsLocalPlayerController()) {
		UUserWidget* GameplayWidget = CreateWidget(MasterPC, WidgetToSpawn, "GameplayWidget");
		Widget = Cast<UGameplayWidget>(GameplayWidget);
		if (Widget) {
			Widget->OnPlayerSelected.AddDynamic(this, &ATA_Widget::ReactPlayerSelected);
			Widget->OnDestructCalled.AddDynamic(this, &ATA_Widget::ReceiveCancel);
			Widget->AddToViewport();
		}
		else {
			CancelTargeting();
		}
	}
	else {
		CancelTargeting();
	}
}

void ATA_Widget::ReceiveCancel()
{
	CancelTargeting();
}