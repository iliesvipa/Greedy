// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyDataAsset.h"
#include "AttributeSet.h"

void UGreedyDataAsset::GiveAbilities(UAbilitySystemComponent* ASC)
{
	if (ASC) {
		for (TSubclassOf<UGreedyGA>& StartupAbility : NenAbilities) {
			ASC->GiveAbility(FGameplayAbilitySpec
			(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID)));
		}
		for (TSubclassOf<UGameplayAbility>& StartupAbility : EventAbilities) {
			ASC->GiveAbility(FGameplayAbilitySpec
			(StartupAbility.GetDefaultObject(), 1, -1));
		}
		for (TSubclassOf<UGameplayAbility>& StartupAbility : ServerPassiveAbilities) {
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(StartupAbility, 1, -1);
			ASC->GiveAbilityAndActivateOnce(Spec);
		}
		for (TSubclassOf<UGameplayAbility>& StartupAbility : LocalPassiveAbilities) {
			FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, -1));
			ASC->TryActivateAbility(SpecHandle, true);
		}
	}
}

void UGreedyDataAsset::GiveStats(UAbilitySystemComponent* ASC)
{
	if (ASC) {
		int32 Index = 0;
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		for (TSubclassOf<UGameplayEffect> Stat : BonusStats) {
			FGameplayEffectSpecHandle Handle = ASC->MakeOutgoingSpec(Stat, 1, Context);
			if (Stat.GetDefaultObject()->Modifiers[Index].ModifierMagnitude.GetMagnitudeCalculationType()
				== EGameplayEffectMagnitudeCalculation::SetByCaller) {
				Handle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Character.Health")), MaxHealth);
			}
			ASC->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
			Index++;
		}
	}
}
