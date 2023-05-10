// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/TargetActors/TA_Sphere.h"
#include "GameFramework/Pawn.h"
#include "WorldCollision.h"
#include "Abilities/GameplayAbility.h"

ATA_Sphere::ATA_Sphere(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	ShouldProduceTargetDataOnServer = true;
}

void ATA_Sphere::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();
}

void ATA_Sphere::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		FVector Origin = StartLocation.GetTargetingTransform().GetLocation();
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformOverlap(Origin));
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

FGameplayAbilityTargetDataHandle ATA_Sphere::MakeTargetData(const FHitResult& TargetDataHitResult) const
{
	if (OwningAbility)
	{
		return StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, TargetDataHitResult);
	}

	return FGameplayAbilityTargetDataHandle();
}

FHitResult ATA_Sphere::PerformOverlap(const FVector& OriginVector)
{
	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = false;

	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);

	if (bIgnoreSelf) {
		Params.AddIgnoredActor(OwningAbility->GetOwningActorFromActorInfo());
	}
	FHitResult HitResult;
	GetWorld()->SweepSingleByChannel(HitResult, OriginVector, LengthVector, FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(Radius), Params);
	return HitResult;
}
