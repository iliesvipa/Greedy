// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "Greedy/GreedyCharacter.h"
#include "Greedy/Systems/Card.h"
#include "Engine/DataTable.h"
#include "QuestComponent.generated.h"

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	Accepted,
	Failed, 
	Succeeded
};

USTRUCT(BlueprintType)
struct FQuestInfos : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Ai-je besoin de la description de la carte ?
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName QuestName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bMainQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bInternal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bGivesJenis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AMasterQuest> QuestActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FName> CardRewards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JenisReward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FText> Objectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FailCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag CooldownTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsRestricted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag UnrestrictTag; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> QuestAbility;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class GREEDY_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuestComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	TObjectPtr<UDataTable> QuestDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<FName, EQuestStatus> AllQuestsStatus;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	uint8 MainQuestCount;

	UFUNCTION(BlueprintCallable)
	void AddQuest(FName& InQuestName, EQuestStatus InQuestStatus);

	UFUNCTION()
	void OnQuestEnded(FName& InQuestName, bool bSuccess);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GE")
	TSubclassOf<UGameplayEffect> CooldownGE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GE")
	TSubclassOf<UGameplayEffect> UnrestrictiveGE;

	UFUNCTION(BlueprintCallable)
	void ApplyCD(bool bFailed, const FQuestInfos& InQuestInfos);

	UFUNCTION()
	void ExecuteAbility(FName& InQuestName);

protected:
	virtual void BeginPlay() override;

};
