// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/CustomAsyncTask/WaitBeginEndCooldown.h"

UWaitBeginEndCooldown* UWaitBeginEndCooldown::ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InCooldownTag, bool InUseServerCooldown)
{
	UWaitBeginEndCooldown* ListenForCooldownChange = NewObject<UWaitBeginEndCooldown>();
	ListenForCooldownChange->ASC = AbilitySystemComponent;
	ListenForCooldownChange->CooldownTag = InCooldownTag;
	ListenForCooldownChange->UseServerCooldown = InUseServerCooldown;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		ListenForCooldownChange->EndTask();
		return nullptr;
	}

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(ListenForCooldownChange, &UWaitBeginEndCooldown::OnActiveGameplayEffectAddedCallback);
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(ListenForCooldownChange, &UWaitBeginEndCooldown::CooldownTagChanged);

	return ListenForCooldownChange;
}

void UWaitBeginEndCooldown::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitBeginEndCooldown::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		float TimeRemaining = 0.0f;
		float Duration = 0.0f;
		// Expecting cooldown tag to always be first tag
		GetCooldownRemainingForTag(GrantedTags.GetByIndex(0), TimeRemaining, Duration);

		if (ASC->GetOwnerRole() == ROLE_Authority)
		{
			// Player is Server
			OnCooldownBegin.Broadcast(TimeRemaining, Duration);
		}
		else if (!UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
		{
			// Client using predicted cooldown
			OnCooldownBegin.Broadcast(TimeRemaining, Duration);
		}
		else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated() == nullptr)
		{
			// Client using Server's cooldown. This is Server's corrective cooldown GE.
			OnCooldownBegin.Broadcast(TimeRemaining, Duration);
		}
		else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
		{
			// Client using Server's cooldown but this is predicted cooldown GE.
			// This can be useful to gray out abilities until Server's cooldown comes in.
			OnCooldownBegin.Broadcast(-1.0f, -1.0f);
		}
	}
}

void UWaitBeginEndCooldown::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		OnCooldownEnd.Broadcast(-1.0f, -1.0f);
	}
}

bool UWaitBeginEndCooldown::GetCooldownRemainingForTag(FGameplayTag InCooldownTag, float& TimeRemaining, float& CooldownDuration)
{
	if (IsValid(ASC) && InCooldownTag.IsValid())
	{
		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
		TPair<float, float> DurationAndTimeRemaining = ASC->GetActiveEffectsTimeRemainingAndDuration(Query)[0];

		TimeRemaining = DurationAndTimeRemaining.Key;
		CooldownDuration = DurationAndTimeRemaining.Value;

		return true;
	}
	return false;
}