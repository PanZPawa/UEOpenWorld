// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include <InputActionValue.h>
#include "Items/item.h"
#include "Animation/AnimInstance.h"
#include "Items/Weapons/Weapon.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthyBarComponent.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void AMyCharacter::Attack()
{
	
		if (ActionState == EActionState::ECS_Unoccupied && GetCharacterState() == ECharacterState::ECS_EquippedOneHandWeapon) {
			ActionState = EActionState::ECS_Attacking;


			if (ComboAttackNum == EComboStage::attack1) {

				FName AttackNum("attack1");
				AttackMontagePlay(AttackNum);
				ComboAttackNum = EComboStage::attack2;

				GetWorld()->GetTimerManager().SetTimer(
					ComboTimerHandle,
					this,
					&AMyCharacter::ResetCombo,
					1.8f,
					false);
			}
			else if (ComboAttackNum == EComboStage::attack2) {

				FName AttackNum("attack2");
				AttackMontagePlay(AttackNum);
			}
	}
}
void AMyCharacter::ResetCombo()
{
	ComboAttackNum = EComboStage::attack1;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("MyCharacter"));
	
}

void AMyCharacter::GetHit(const FVector& Point)
{
	HitReactMontagePlay(FName("CharacterReact1"));
}


void AMyCharacter::move(const FVector2D& MoveValue)
{
	if (ActionState == EActionState::ECS_Attacking) return;
	const FRotator Rotation = GetControlRotation();

	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
	if (this)
	{
		this->AddMovementInput(ForwardDirection, MoveValue.X);
		this->AddMovementInput(RightDirection, MoveValue.Y);
	}
}

void AMyCharacter::Lookup(const FVector2D& LookValue)
{
	AddControllerYawInput(LookValue.X * 1.0f);
	AddControllerPitchInput(LookValue.Y * 1.0f);
}

void AMyCharacter::EquipWeapon()
{
	if (ActionState == EActionState::ECS_Attacking) return;

	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, TEXT("EKeyPressed"));
	AWeapon* OverlappingWeapon = Cast<AWeapon>(GetOverlappingItem());
	if (OverlappingWeapon) {
		OverlappingWeapon->Equip(GetMesh(), FName("Righthand_rSocket"));

		OverlappingWeapon->SetOwner(this);
		OverlappingWeapon->SetInstigator(this);
		SetCharacterState();
		SetEquippedWeapon(OverlappingWeapon);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

inline Aitem* AMyCharacter::GetOverlappingItem()
{
	return OverlappingItem;
}

void AMyCharacter::AttackEnd()
{
	ActionState = EActionState::ECS_Unoccupied;
	EquippedWeapon->GetIgnoreActor().Empty();
//	UE_LOG(LogTemp, Log, TEXT(" empty"));
}

void AMyCharacter::AttackMontagePlay(FName AttackNum)
{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		UAnimMontage* Montage = GetCharacterMontage();

		if (AnimInstance && Montage) {

			AnimInstance->Montage_Play(Montage, 1.5f);
			AnimInstance->Montage_JumpToSection(AttackNum, Montage);
			//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, TEXT("attack1"));
		}
}


float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (Attributes && HealthBarWidget) {
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());

	}
	return DamageAmount;
}
