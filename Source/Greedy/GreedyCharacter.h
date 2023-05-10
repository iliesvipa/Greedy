// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GreedyCharacter.generated.h"

UCLASS(config=Game)
class AGreedyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystemComponent, meta = (AllowPrivateAccess = "true"))
	class UGreedyASC* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY()
	class UGreedyAttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UQuestComponent* QuestComponent;


public:
	AGreedyCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UGreedyDataAsset> GreedyData;

	void InitializeLocallyStats();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void PossessedBy(AController* NewController) override;
	void OnRep_PlayerState() override;

	void Die(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetNenValues(float& Nen, float& MaxNen);

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UGreedyAttributeSet* GetAttributeSet();
	UInventoryComponent* GetInventory();
	UQuestComponent* GetQuestComponent();

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<class UGameplayEffect> FullHealthGE;
	void ApplyFullHealth();
	FGameplayTag FullHealthTag;

protected:
	void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void BindASC();
};

