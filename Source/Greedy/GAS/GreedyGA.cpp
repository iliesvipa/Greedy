// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/GAS/GreedyGA.h"
#include "AbilitySystemComponent.h"

UGreedyGA::UGreedyGA() {
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
	HealTag = FGameplayTag::RequestGameplayTag(FName("Data.Heal"));
	bActivateAbilityOnGranted = false;
}

void UGreedyGA::SetDamage(float Magnitude)
{
	if (GEDamage) {
		FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(GEDamage, 1);
		if (Spec.IsValid()) {
			Spec.Data.Get()->SetSetByCallerMagnitude(DamageTag, Magnitude);
			ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), Spec);
		}
	}
}

void UGreedyGA::SetHeal(float Magnitude)
{
	if (GEHeal) {
		FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(GEHeal, 1);
		if (Spec.IsValid()) {
			Spec.Data.Get()->SetSetByCallerMagnitude(HealTag, Magnitude);
			ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), Spec);
		}
	}
}

FGameplayAbilityTargetingLocationInfo& UGreedyGA::SetTargetingLocationInfo(float BeginMagnitude, float EndMagnitude, FVector& EndVector)
{
	TargetingLocationInfo.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	TargetingLocationInfo.SourceAbility = this;
	TargetingLocationInfo.SourceActor = GetOwningActorFromActorInfo();
	const FVector& ActorLocation = GetOwningActorFromActorInfo()->GetActorLocation();
	const FVector& ActorForward = GetOwningActorFromActorInfo()->GetActorForwardVector();
	FVector BeginVector = ActorLocation + ActorForward * BeginMagnitude;
	EndVector = ActorLocation + ActorForward * EndMagnitude;
	FTransform Transform;
	Transform.SetLocation(BeginVector);
	Transform.SetRotation(GetOwningActorFromActorInfo()->GetActorRotation().Quaternion());
	Transform.SetScale3D(FVector::OneVector);
	TargetingLocationInfo.LiteralTransform = Transform;
	return TargetingLocationInfo;
}

void UGreedyGA::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	FGameplayAbilitySpec* CurrentSpec = GetCurrentAbilitySpec();
	CurrentSpec->InputID = int32(AbilityInputID);
	AbilityInputID;
	if (bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}

	/* Apply cooldown on end of the ability only if a hit has not been hitXD? 
	something like : M1->Hit->Wait 1.9s->M1->During M1 : GE on end applied the cooldown GE that is being stored to 
	be applied on the end of the ability if the  recast tag is not here.*/
}

void UGreedyGA::ApplyReset(const FGameplayTag& NewGrantedTag, float Magnitude)
{
	if (ResetGE) {
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ResetGE, 1.f);
		if (SpecHandle.IsValid()) {
			SpecHandle.Data.Get()->SetSetByCallerMagnitude(ResetTag, Magnitude);
			SpecHandle.Data.Get()->DynamicGrantedTags.AddTag(NewGrantedTag);
			ApplyGameplayEffectSpecToOwner
			(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), SpecHandle);
			EndAbility
			(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
	}
}

void UGreedyGA::OnWaitingForConfirmInputBegin()
{

}

void UGreedyGA::OnWaitingForConfirmInputEnd()
{

}