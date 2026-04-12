// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include <InputActionValue.h>
#include <EnhancedInputLibrary.h>
#include "GameFramework/Character.h"
#include "Characters/MyCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "CharacterTypes.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(MyContext);
	//GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Blue,TEXT("Controller BeginPlay"));
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	if(GetPawn())ControlleredCharacter = Cast<AMyCharacter>(GetPawn());
	Subsystem->AddMappingContext(MyContext, 0);
	//Subsystem->AddMappingContext(TurnContext, 0);
	Subsystem->AddMappingContext(LookupContext, 0);
	Subsystem->AddMappingContext(JumpContext, 0);
	Subsystem->AddMappingContext(EKeyPressedContext,0);
	Subsystem->AddMappingContext(AttackContext,0);

}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
	EnhancedInputComponent->BindAction(LookupAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Lookup);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::Jump);
	EnhancedInputComponent->BindAction(EKeyPressedAction,ETriggerEvent::Started, this, &AMyPlayerController::EKeyPressed);
	EnhancedInputComponent->BindAction(AttackAction,ETriggerEvent::Started,this,&AMyPlayerController::AttackPressed);

}




void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{	
	if (ControlleredCharacter) {
		const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
		ControlleredCharacter->move(InputAxisVector);
	}

}


void AMyPlayerController::Lookup(const FInputActionValue& InputActionValue)
{
	if (ControlleredCharacter) {
		const FVector2D value = InputActionValue.Get<FVector2D>();
		ControlleredCharacter->Lookup(value);
	}

}

void AMyPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ControlleredCharacter->ActionState == EActionState::ECS_Attacking) return;

	if (ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->Jump();
	}

}

void AMyPlayerController::EKeyPressed()
{
	if (ControlleredCharacter) {
		ControlleredCharacter->EquipWeapon();
	}

}

void AMyPlayerController::AttackPressed()
{
	ControlleredCharacter->Attack();
}

