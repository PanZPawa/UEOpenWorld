// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/item.h"
#include "Characters/BaseCharacter.h"
#include "Weapon.generated.h"

/**
 * 
 */


class IMyInterface;
class UBoxComponent;
class AMyCharacter;
class ABaseCharacter;
UCLASS()
class OPENWORLD_API AWeapon : public Aitem
{
	GENERATED_BODY()
public:

	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName);
	inline UBoxComponent* GetWeaponBox() { return WeaponBox; }
	inline TArray<AActor*>& GetIgnoreActor() { return IgnoreActor; }
	//inline  ABaseCharacter* GetOwner() { return Owner; }
//	void MySetOwner(ABaseCharacter* actor) { Owner = actor; }
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult &SweepResult);
	
	UPROPERTY(VisibleAnywhere, Category ="Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Weapon Trace")
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Weapon Trace")
	USceneComponent* BoxTraceEnd;
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

	UPROPERTY(EditAnywhere,Category = "Weapon Properties")
	float Damage = 20.f;
private:
	//class  ABaseCharacter* Owner;
	TArray<AActor*> IgnoreActor;
};
