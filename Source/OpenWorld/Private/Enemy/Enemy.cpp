// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "OpenWorld/DebugMacros.h"
#include "Animation/AnimInstance.h"
#include "HUD/HealthyBarComponent.h"
#include "Components/AttributeComponent.h"
#include "Characters/MyCharacter.h"
#include "AIController.h"
#include "NavigationData.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapons/Weapon.h"
AEnemy::AEnemy()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	HealthBarWidget = CreateDefaultSubobject<UHealthyBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());



	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawnsensing"));
	PawnSensing->SightRadius = 2000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);


}

void AEnemy::GetHit(const FVector &Point)
{
	//DRAW_SPHERE(Point)
	if(HealthBarWidget)HealthBarWidget->SetVisibility(true);


	if (!Attributes->ActorIsAlive()) Die();
	else DirectionReact(Point);

}




float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (Attributes && HealthBarWidget ) {
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		
	}
	TargetActor = Cast<AMyCharacter>(EventInstigator->GetPawn());
	EnemyState2 = EEnemyState::EES_Chasing;
	//设置速度
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	TestMoveTo(TargetActor);
	return DamageAmount;
}

void AEnemy::TestMoveTo(AActor* Target)
{


	if (EnemyController && Target) {
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(15.f);
		EPathFollowingRequestResult::Type Result = EnemyController->MoveTo(MoveRequest);

	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget) {
		HealthBarWidget->SetHealthPercent(1.f);
		HealthBarWidget->SetVisibility(false);
	}


	EnemyController = Cast<AAIController>(GetController());

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[this](){
			TestMoveTo(PatrolTarget);
		},
		1.0f, 
		false
	);

	if (PawnSensing) {
	//	UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), *GetController()->GetName());

		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	UWorld* World = GetWorld();
	if (World && Weaponclass) {
	AWeapon* DefaultWeapon =	World->SpawnActor<AWeapon>(Weaponclass);
	DefaultWeapon->Equip(GetMesh(),FName("RightHandSocket"));
	EquippedWeapon = DefaultWeapon;
	DefaultWeapon->SetInstigator(this);
	DefaultWeapon->SetOwner(this);
//	DefaultWeapon->MySetOwner(Cast<ABaseCharacter>(this));
	//Owner = DefaultWeapon;

	}
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target) {
		 

		float Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
		return Distance <= Radius;
	}
	else {
		//UE_LOG(LogTemp,Error,TEXT("FUNCTION InTargetRange :no Target"));
		
		return false; }
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	//UE_LOG(LogTemp,Warning, TEXT("can see"));
	if (EnemyState2 >= EEnemyState::EES_Chasing) return;  //如果只是==EES_Chasing  会导致状态在EES_Chasing 和EES_Attack之间来回切换
	if (SeenPawn->ActorHasTag(FName("MyCharacter"))) {
		EnemyState2 = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		TargetActor = Cast<AMyCharacter>(SeenPawn);
		TestMoveTo(SeenPawn);
	}
}

void AEnemy::Attack()
{
	AttackMontagePlay(FName("EnemyAttack2"));
}

void AEnemy::AttackMontagePlay(FName AttackNum)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage) {
		AnimInstance->Montage_Play(AttackMontage, 1.5f);
		AnimInstance->Montage_JumpToSection(AttackNum, AttackMontage);
	}
}

void AEnemy::AttackEnd()
{
	EnemyState2 = EEnemyState::EES_Unoccupied;
	EquippedWeapon->GetIgnoreActor().Empty();
}

void AEnemy::Die()
{	
	EnemyState = ECharacterState::ECS_Die;
	DeathMontagePlay(FName("Die"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(6.f);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyState2 > EEnemyState::EES_Patrolling)
	CheckCombat();
	else
	CheckPatrol();
}

void AEnemy::CheckCombat()
{
	if (TargetActor && HealthBarWidget) {
		if (!InTargetRange(TargetActor, CombatRadius)) {
			TargetActor = nullptr;
			HealthBarWidget->SetVisibility(false);
			//巡逻
			EnemyState2 = EEnemyState::EES_Patrolling;
			GetCharacterMovement()->MaxWalkSpeed = 100.f;
			TestMoveTo(PatrolTarget);
		}
		else if (!InTargetRange(TargetActor, AttackRadius) && EnemyState2 != EEnemyState::EES_Chasing) {
			EnemyState2 = EEnemyState::EES_Chasing;
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			TestMoveTo(TargetActor);
			//追逐
		}
		else if (InTargetRange(TargetActor, AttackRadius) && EnemyState2 != EEnemyState::EES_Attacking) {
			EnemyState2 = EEnemyState::EES_Attacking;

			Attack();
			//攻击
		}
	
	}
}

void AEnemy::CheckPatrol()
{
	if (PatrolTarget) {
		if (InTargetRange(PatrolTarget, PatrolRadius)) {

			TArray<AActor*> VaildTargets;
			for (AActor* Target : PatrolTargets) {
				if (Target != PatrolTarget) {
					VaildTargets.AddUnique(Target);
				}
			}

			const int32 NumPatrolTargets = PatrolTargets.Num();
			if (NumPatrolTargets > 0) {
				const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
				AActor* Target = PatrolTargets[TargetSelection];
				PatrolTarget = Target;
				TestMoveTo(PatrolTarget);
			}
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetTargetActor(AMyCharacter* Target)
{
	TargetActor = Target;
}

