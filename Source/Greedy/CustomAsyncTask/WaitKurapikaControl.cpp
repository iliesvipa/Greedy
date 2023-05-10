// Fill out your copyright notice in the Description page of Project Settings.


#include "Greedy/CustomAsyncTask/WaitKurapikaControl.h"

UWaitKurapikaControl::UWaitKurapikaControl(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

//UWaitKurapikaControl* UWaitKurapikaControl::WaitKurapikaControl(UGameplayAbility* OwningAbility, float TimeSeconds, float TimerInterval)
//{
//	UWaitKurapikaControl* MyObj = NewAbilityTask<UWaitKurapikaControl>(OwningAbility);
//	/*MyObj->TimeInterval = TimerInterval;
//	MyObj->Seconds = TimeSeconds;*/
//	return MyObj;
//}

void UWaitKurapikaControl::Activate()
{

}

void UWaitKurapikaControl::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}