// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/Inventory/QuestComponent.h"
#include "Greedy/Systems/MasterQuest.h"

UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MainQuestCount = 0;
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestComponent::AddQuest(FName& InQuestName, EQuestStatus InQuestStatus)
{
	AllQuestsStatus.Add(InQuestName, InQuestStatus);
	FQuestInfos* LocalQuest = QuestDataTable->FindRow<FQuestInfos>(InQuestName, "Context");
	if (!LocalQuest) {
		UE_LOG(LogTemp, Warning, TEXT("LocalQuest is nullptr because it is not found in the data table.."));
		return;
	}
	if (InQuestStatus == EQuestStatus::Accepted) {
		if (LocalQuest->QuestActor) {
			LocalQuest->QuestActor->QuestEnded.AddDynamic(this, &UQuestComponent::OnQuestEnded);
			if (LocalQuest->bIsRestricted) {
				AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(GetOwner());
				UAbilitySystemComponent* ASC = GreedyChar->GetAbilitySystemComponent();
				FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UnrestrictiveGE, 1, ContextHandle);
				if (SpecHandle.IsValid()) {
					SpecHandle.Data.Get()->DynamicGrantedTags.AddTag(LocalQuest->UnrestrictTag);
					ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
	else if (InQuestStatus == EQuestStatus::Failed) {
		ApplyCD(true, *LocalQuest);
	}
	else if (InQuestStatus == EQuestStatus::Succeeded) {
		ApplyCD(false, *LocalQuest);
		if (LocalQuest->bMainQuest) {
			MainQuestCount++;
			if (MainQuestCount == 4) {
				AllQuestsStatus.Add("RulersBlessing", EQuestStatus::Accepted);
			}
		}
	}
}

void UQuestComponent::ApplyCD(bool bFailed, const FQuestInfos& InQuestInfos)
{
	float Cooldown = bFailed ? InQuestInfos.FailCooldown : InQuestInfos.FailCooldown * 0.3;
	AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(GetOwner());
	UAbilitySystemComponent* ASC = GreedyChar->GetAbilitySystemComponent();
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CooldownGE, 1, ContextHandle);
	if (SpecHandle.IsValid()) {
		SpecHandle.Data.Get()->DynamicGrantedTags.AddTag(InQuestInfos.CooldownTag);
		SpecHandle.Data.Get()->SetDuration(InQuestInfos.FailCooldown * 0.3, false);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UQuestComponent::OnQuestEnded(FName& InQuestName, bool bSuccess)
{
	// Notify User Interface
	AddQuest(InQuestName, bSuccess ? EQuestStatus::Succeeded : EQuestStatus::Failed);
}

void UQuestComponent::ExecuteAbility(FName& InQuestName)
{
	if (GetOwner()->HasAuthority()) {
		FQuestInfos* LocalQuest = QuestDataTable->FindRow<FQuestInfos>(InQuestName, "Context");
		if (LocalQuest) {
			if (LocalQuest->QuestAbility) {
				AGreedyCharacter* GreedyChar = Cast<AGreedyCharacter>(GetOwner());
				UAbilitySystemComponent* ASC = GreedyChar->GetAbilitySystemComponent();
				FGameplayAbilitySpec Spec = FGameplayAbilitySpec(LocalQuest->QuestAbility, 1, -1);
				ASC->GiveAbilityAndActivateOnce(Spec);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Has not authority to executeAbility"));
	}
}