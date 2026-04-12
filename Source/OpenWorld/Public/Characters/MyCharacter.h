// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "Items/item.h"
#include "CharacterTypes.h"
#include "MyCharacter.generated.h"


class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class Aitem;
class UAnimInstance;
class UBoxComponent;
struct FInputActionValue;
struct  FTimerHandle;
class UHealthyBarComponent;



UENUM(BlueprintType)
enum class EComboStage : uint8
{
	attack1,
	attack2
};

UCLASS()
class OPENWORLD_API AMyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EActionState ActionState = EActionState::ECS_Unoccupied;

	virtual void AttackEnd() override;

	virtual void AttackMontagePlay(FName AttackNum) override;

	AMyCharacter();

	//攻击模块
	virtual void Attack() override	;

	FTimerHandle ComboTimerHandle;

	EComboStage ComboAttackNum = EComboStage::attack1;
	void ResetCombo();
	//移动
	void move(const FVector2D &MoveValue);

	//看
	void Lookup(const FVector2D& LookValue);

	//装备武器
	void EquipWeapon();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	inline void SetOverlappingItem(Aitem* Item) { OverlappingItem = Item; }

	inline Aitem* GetOverlappingItem();

	inline void SetEquippedWeapon(AWeapon* item) { EquippedWeapon = item; }

	ECharacterState GetCharacterState() {
		return CharacterState;
	}
	inline void SetCharacterState() {
		CharacterState = ECharacterState::ECS_EquippedOneHandWeapon;
	}

	inline UAnimMontage* GetCharacterMontage() { return AttackMontage; }
protected:

	virtual void BeginPlay() override;

	virtual void GetHit(const FVector& Point) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UFloatingPawnMovement* Movement;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere,Category ="hair")
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = "hair")
	UGroomComponent* Eyebrows;
	UPROPERTY(VisibleInstanceOnly)
	Aitem* OverlappingItem;
	UPROPERTY(VisibleAnywhere)
	UHealthyBarComponent* HealthBarWidget;



};
