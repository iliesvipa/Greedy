// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/AttributeSets/GreedyAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Greedy/GreedyCharacter.h"

void UGreedyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGreedyAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGreedyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGreedyAttributeSet, Nen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGreedyAttributeSet, MaxNen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGreedyAttributeSet, SpeedMultiplicater, COND_None, REPNOTIFY_Always);
}

void UGreedyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGreedyAttributeSet, Health, OldHealth);
}
void UGreedyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGreedyAttributeSet, MaxHealth, OldMaxHealth);
}
void UGreedyAttributeSet::OnRep_Nen(const FGameplayAttributeData& OldNen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGreedyAttributeSet, Nen, OldNen);
}
void UGreedyAttributeSet::OnRep_MaxNen(const FGameplayAttributeData& OldMaxNen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGreedyAttributeSet, MaxNen, OldMaxNen);
}
void UGreedyAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGreedyAttributeSet, SpeedMultiplicater, OldSpeed);
}

void UGreedyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute() && GetMaxHealth() > 1) {
		SetHealth(FMath::Clamp<float>(GetHealth(), 0.0f, GetMaxHealth()));
		if (GetHealth() < 1) {
			AGreedyCharacter* GreedyPlayer = Cast<AGreedyCharacter>(GetOwningActor());
			AActor* TargetActor = Data.EffectSpec.GetEffectContext().GetInstigator();
			GreedyPlayer->Die(TargetActor);
		}
	}
	if (Data.EvaluatedData.Attribute == GetNenAttribute() && GetMaxNen() > 1) {
		SetNen(FMath::Clamp<float>(GetNen(), 0.0f, GetMaxNen()));
	}
}