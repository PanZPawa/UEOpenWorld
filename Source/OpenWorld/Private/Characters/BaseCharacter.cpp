// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::SetAttackCollisionEnabled(ECollisionEnabled::Type type)
{
		if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
			EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(type);

		}
}


void ABaseCharacter::HitReactMontagePlay(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}

}

void ABaseCharacter::DeathMontagePlay(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}


void  ABaseCharacter::DirectionReact(const FVector& Point)
{
	FVector Forward = GetActorForwardVector();
	FVector HitReactDirection = (Point - GetActorLocation()).GetSafeNormal();

	FVector UnitHitReactDirection(HitReactDirection.X, HitReactDirection.Y, Forward.Z);

	float CosTheta = FVector::DotProduct(Forward, UnitHitReactDirection);
	float ThetaRadian = FMath::Acos(CosTheta);
	float Theta = FMath::RadiansToDegrees(ThetaRadian);
	const FVector CrossProduct = FVector::CrossProduct(Forward, UnitHitReactDirection);

	if (CrossProduct.Z < 0) {
		Theta *= -1.f;
	}

	FName Dir;
	if (Theta >= -45.f && Theta < 45.f) {
		Dir = FName("FromFront");
	}
	else if (Theta >= 45.f && Theta <= 135.f) {
		Dir = FName("FromRight");
	}
	else if (Theta >= -135.f && Theta < -45.f) {
		Dir = FName("FromLeft");
	}
	else {
		Dir = FName("FromBack");
	}
	HitReactMontagePlay(Dir);
}

void  ABaseCharacter::Attack() {

}
void  ABaseCharacter::AttackMontagePlay(FName AttackNum) {

}

void ABaseCharacter::AttackEnd() {

}

void ABaseCharacter::Die() {

}
