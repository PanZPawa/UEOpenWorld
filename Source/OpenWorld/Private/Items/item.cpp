// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/item.h"
#include "../DebugMacros.h"
#include "Engine/Engine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/MyCharacter.h"
// Sets default values
Aitem::Aitem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;
	ItemMesh->SetMobility(EComponentMobility::Movable);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(ItemMesh);

}

// Called when the game starts or when spawned
void Aitem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&Aitem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &Aitem::OnSphereEndOverlap);
}

// Called every frame
void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//float MovementRate = 50.f;
	//float RotationRate = 45.f;
	//AddActorWorldOffset(FVector(MovementRate*DeltaTime, 0.f, 0.f));
	//AddActorWorldRotation(FRotator(0.f,RotationRate*DeltaTime,0.f));
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_VECTOR_SingleFrame(GetActorLocation(),GetActorLocation()+GetActorForwardVector()*100.f);
}

void Aitem::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter) {
		MyCharacter->SetOverlappingItem(this);
	}
}

void Aitem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter) {
		MyCharacter->SetOverlappingItem(nullptr);
	}
}

