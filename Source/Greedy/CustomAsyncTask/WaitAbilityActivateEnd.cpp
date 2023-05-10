// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/CustomAsyncTask/WaitAbilityActivateEnd.h"

UWaitAbilityActivateEnd* UWaitAbilityActivateEnd::ListenForAbilityActivateEnd(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayTag InAbilityTag)
{
	UWaitAbilityActivateEnd* MyObj = NewObject<UWaitAbilityActivateEnd>();
	MyObj->ASC = InAbilitySystemComponent;
	MyObj->AbilityTag = InAbilityTag;

	if (!IsValid(InAbilitySystemComponent) || !InAbilityTag.IsValid()) {
		MyObj->EndTask();
		return nullptr;
	}

	InAbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(MyObj, &UWaitAbilityActivateEnd::OnAbilityActivatedCallback);
	InAbilitySystemComponent->AbilityEndedCallbacks.AddUObject(MyObj, &UWaitAbilityActivateEnd::OnAbilityEndedCallback);

	return MyObj;
}

void UWaitAbilityActivateEnd::EndTask()
{
	if (IsValid(ASC)) {
		ASC->AbilityActivatedCallbacks.RemoveAll(this);
		ASC->AbilityEndedCallbacks.RemoveAll(this);
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitAbilityActivateEnd::OnAbilityActivatedCallback(UGameplayAbility* InAbility)
{
	if (InAbility->AbilityTags.HasTag(AbilityTag)) {
		OnAbilityActivated.Broadcast(InAbility);
	}
}

void UWaitAbilityActivateEnd::OnAbilityEndedCallback(UGameplayAbility* InAbility)
{
	if (InAbility->AbilityTags.HasTag(AbilityTag)) {
		OnAbilityEnded.Broadcast(InAbility);
	}
}