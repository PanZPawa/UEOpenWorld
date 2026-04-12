// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/BaseCharacter.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthyBarComponent;
class AMyCharacter;
class UPawnSensingComponent;
UCLASS()
class OPENWORLD_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	virtual void GetHit(const FVector &Point) override;



	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void TestMoveTo(AActor* Target);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckCombat();

	void CheckPatrol();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetTargetActor(AMyCharacter* Target);
	UFUNCTION(BlueprintCallable)
	ECharacterState GetState() { return EnemyState; }
	UPROPERTY(EditAnywhere)
	float CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 150.f;
	UPROPERTY(EditInstanceOnly,Category = "AI Navigation",BlueprintReadWrite,meta = (AllowPrivateAccess))
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;


	UPROPERTY(EditAnywhere)
	float PatrolRadius = 200.f;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool InTargetRange(AActor * Target,double Radius);
	UFUNCTION()
	void PawnSeen(APawn *SeenPawn);

	virtual void Attack() override;

	virtual void AttackMontagePlay(FName AttackNum) override;

	virtual void AttackEnd() override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> Weaponclass;
	UPROPERTY()
	class AAIController* EnemyController;
	UPROPERTY(VisibleAnywhere)
	UHealthyBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;
	
	
	UPROPERTY()
	class AMyCharacter* TargetActor;
	ECharacterState EnemyState = ECharacterState::ECS_Alive;

	EEnemyState EnemyState2 = EEnemyState::EES_Patrolling;

	virtual void Die() override;
};
