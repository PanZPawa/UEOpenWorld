// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/MyInterface.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UAttributeComponent;
UCLASS()
class OPENWORLD_API ABaseCharacter : public ACharacter, public IMyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetAttackCollisionEnabled(ECollisionEnabled::Type type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAttributeComponent* Attributes;
	//¹¥»÷
	virtual void Attack();
	virtual void AttackMontagePlay(FName AttackNum);
	void HitReactMontagePlay(const FName& SectionName);
	void DeathMontagePlay(const FName& SectionName);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void Die();
	void DirectionReact(const FVector& Point);
};
