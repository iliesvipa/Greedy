// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Greedy/Systems/Interactable.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Greedy/Systems/Card.h"
#include "MasterItem.generated.h"

UCLASS()
class GREEDY_API AMasterItem : public AInteractable
{
	GENERATED_BODY()

public:

	AMasterItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Defaults")
	FCardStruct CardStruct;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Defaults")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	class UStaticMeshComponent* StaticMesh;

	void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	void BeginPlay() override;
};
