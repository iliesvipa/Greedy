// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/GAS/GreedyASC.h"
#include "Greedy/Gameplay/GreedyPlayerController.h"
#include "Greedy/GreedyCharacter.h"

UGreedyASC::UGreedyASC()
{
	HumanAirbone = FGameplayTag::RequestGameplayTag(FName("Human.State.Airborne"));
	FightTag = FGameplayTag::RequestGameplayTag(FName("Human.State.Fight"));
	OffensiveTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Offensive"));
	ControlTag = FGameplayTag::RequestGameplayTag(FName("Human.State.Debuff.Strong"));
}

void UGreedyASC::BeginPlay()
{
	Super::BeginPlay();
	RegisterGameplayTagEvent(ControlTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGreedyASC::CancelTagChanged);
	RegisterGameplayTagEvent(FightTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGreedyASC::SetInteractableOff);
	RegisterGameplayTagEvent(HumanAirbone, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGreedyASC::CancelTagChanged);
	OnGameplayEffectAppliedDelegateToSelf.AddUFunction(this, "OnGEAppliedToSelf");
	AbilityFailedCallbacks.AddUFunction(this, "OnAbilityFailed");
	
	GreedyCharacter = Cast<AGreedyCharacter>(GetAvatarActor());
}

void UGreedyASC::OnGEAppliedToSelf(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer TagContainer; 
	SpecApplied.GetAllAssetTags(TagContainer);
	if (Target != nullptr && TagContainer.HasTagExact(OffensiveTag)) {
		FGameplayEffectContextHandle ContextGE = MakeEffectContext();
		ApplyGameplayEffectToSelf(FightGE.GetDefaultObject(), 1, ContextGE);
		Target->ApplyGameplayEffectToSelf(FightGE.GetDefaultObject(), 1, ContextGE);
		AActor* TargetActor = Target->GetOwnerActor();
		OnHitDelegate.Broadcast(TargetActor ? TargetActor : nullptr);
		if (UGreedyASC* TargetASC = Cast<UGreedyASC>(Target)) {
			TargetActor = GetOwnerActor();
			TargetASC->OnHitDelegate.Broadcast(TargetActor ? TargetActor : nullptr);
		}
		UE_LOG(LogTemp, Warning, TEXT("Gameplay effect applied to Self"));
	}
}

void UGreedyASC::OnPlayerControllerSet()
{
	GreedyPC = Cast<AGreedyPlayerController>(AbilityActorInfo.Get()->PlayerController.Get());
}

void UGreedyASC::CancelTagChanged(const FGameplayTag CallbackTag, int32 TagCount)
{
	if (TagCount > 0) {
		FGameplayTagContainer AbilityTagsToCancel;
		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Human.Ability")));
		CallbackTag.MatchesTagExact(ControlTag) ? 
			AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Details.Uncancelable"))) :
			AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Details.NoFallCancel")));
		CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

void UGreedyASC::SetInteractableOff(const FGameplayTag CallbackTag, int32 TagCount)
{
	if (TagCount > 0) {
		if (NoInteractGE != nullptr) {
			FGameplayEffectContextHandle LocalContext = MakeEffectContext();
			ApplyGameplayEffectToSelf(NoInteractGE.GetDefaultObject(), 1, LocalContext);
		}
	}
	else {
		RemoveActiveEffectsWithGrantedTags(CallbackTag.GetSingleTagContainer());
	}
}

void UGreedyASC::OnAbilityFailed(UGameplayAbility* InGameplayAbility, const FGameplayTagContainer& TagContainer)
{
	if (GreedyPC != nullptr) {
		// Do stuff..
	}
}
