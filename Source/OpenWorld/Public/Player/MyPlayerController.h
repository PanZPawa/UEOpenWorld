// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"



class UInputMappingContext;
class UInputAction;
class AMyCharacter;
struct FInputActionValue;
struct  FTimerHandle;
/**
 * 
 */

UCLASS()
class OPENWORLD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;



private:
	AMyCharacter* ControlleredCharacter;
	
	//移动
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> MyContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue &InputActionValue);
	//摄像头转向
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> LookupContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookupAction;
	void Lookup(const FInputActionValue& InputActionValue);

	//跳跃
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> JumpContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	void Jump(const FInputActionValue& InputActionValue);
	//E按键
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> EKeyPressedContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> EKeyPressedAction;
	void EKeyPressed();
	//攻击
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext> AttackContext;
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> AttackAction;


	void AttackPressed();
};
