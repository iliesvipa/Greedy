// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitForCustomOverlap.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"

UWaitForCustomOverlap::UWaitForCustomOverlap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWaitForCustomOverlap* UWaitForCustomOverlap::WaitForCustomOverlap(UGameplayAbility* OwningAbility, AActor* SpawnedActor, float Seconds, float TimerDelay)
{
	UWaitForCustomOverlap* MyObj = NewAbilityTask<UWaitForCustomOverlap>(OwningAbility);
	MyObj->TimerInterval = TimerDelay;
	MyObj->TimerSeconds = Seconds;
	MyObj->Actor = SpawnedActor;
	return MyObj;
}

void UWaitForCustomOverlap::Activate()
{
	UWorld* World = GetWorld();

	//World->GetTimerManager().SetTimer(TimerHandle, this, &UWaitForCustomOverlap::PerformTrace, TimerInterval, true, 0.f);
}

void UWaitForCustomOverlap::PerformTrace()
{

}

void UWaitForCustomOverlap::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}