// Copyright Epic Games, Inc. All Rights Reserved.

#include "GreedyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Greedy/GAS/GreedyASC.h"
#include "GreedyDataAsset.h"
#include "Greedy.h"
#include "Greedy/Inventory/InventoryComponent.h"
#include "Greedy/Inventory/QuestComponent.h"
#include "Greedy/AttributeSets/GreedyAttributeSet.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

AGreedyCharacter::AGreedyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 

	AbilitySystemComponent = CreateDefaultSubobject<UGreedyASC>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGreedyAttributeSet>("AttributeSet");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);

	QuestComponent = CreateDefaultSubobject<UQuestComponent>("QuestComponent");
	QuestComponent->SetIsReplicated(true);

	FullHealthTag = FGameplayTag::RequestGameplayTag(FName("Human.State.FullHealth"));
}

void AGreedyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGreedyCharacter::OnHealthChanged);
	}
}

void AGreedyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue == GetMaxHealth() && !AbilitySystemComponent->HasMatchingGameplayTag(FullHealthTag)) {
		ApplyFullHealth();
	}
	else {
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FullHealthTag.GetSingleTagContainer());
	}
	// Notify UI
}

void AGreedyCharacter::ApplyFullHealth()
{
	if (FullHealthGE != nullptr && AbilitySystemComponent) {
		AbilitySystemComponent->ApplyGameplayEffectToSelf(FullHealthGE.GetDefaultObject(), 1, FGameplayEffectContextHandle());
	}
}

UAbilitySystemComponent* AGreedyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGreedyAttributeSet* AGreedyCharacter::GetAttributeSet()
{
	return AttributeSet;
}

UInventoryComponent* AGreedyCharacter::GetInventory()
{
	return InventoryComponent;
}

UQuestComponent* AGreedyCharacter::GetQuestComponent()
{
	return QuestComponent;
}

void AGreedyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGreedyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGreedyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	BindASC();
}

void AGreedyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		if (HasAuthority()) {
			GreedyData->GiveAbilities(AbilitySystemComponent);
			GreedyData->GiveStats(AbilitySystemComponent);
		}
	}
}

void AGreedyCharacter::OnRep_PlayerState()
{	
	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GreedyData->GiveStats(AbilitySystemComponent);
	}
}

void AGreedyCharacter::InitializeLocallyStats()
{
	if (AbilitySystemComponent) {
		GreedyData->GiveStats(AbilitySystemComponent);
	}
}

void AGreedyCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGreedyCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AGreedyCharacter::GetNenValues(float& Nen, float& MaxNen)
{
	if (AttributeSet) {
		Nen = AttributeSet->GetNen();
		MaxNen = AttributeSet->GetMaxNen();
	}
}

float AGreedyCharacter::GetHealth()
{
	if (AttributeSet) {
		return AttributeSet->GetHealth();
	}
	return 0.f;
}

float AGreedyCharacter::GetMaxHealth()
{
	if (AttributeSet) {
		return AttributeSet->GetMaxHealth();
	}
	return 0.f;
}

void AGreedyCharacter::Die(AActor* TargetActor)
{
	FGameplayEventData EventData;
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Human.State.Die"));
	EventData.Instigator = TargetActor;
	AbilitySystemComponent->HandleGameplayEvent(EventData.EventTag, &EventData);
}

void AGreedyCharacter::BindASC()
{
	if (AbilitySystemComponent && InputComponent) {
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, 
			FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), FString("EGreedyAbilityInputID"), 
				static_cast<int32>(EGreedyAbilityInputID::Confirm), static_cast<int32>(EGreedyAbilityInputID::Cancel)));
	}
}