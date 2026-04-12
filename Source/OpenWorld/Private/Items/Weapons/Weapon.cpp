// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Interfaces/MyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy.h"

AWeapon::AWeapon()
{	
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());

	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);      //  疑问 如果设置无碰撞 默认所有通道都是 忽略 那下面的通道响应还是否有效
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnBoxOverlap);
}
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Owner = Cast<AMyCharacter>(InParent->GetOwner() );
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (!OtherActor || OtherActor == this ) return;

	if (IgnoreActor.Contains(OtherActor)) return;

	//UE_LOG(LogTemp, Log, TEXT("Overlap Actor：%s，  OtherActor：%s"), *OtherActor->GetName(), *OtherComp->GetName());
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);

	for (auto x : IgnoreActor) {
		ActorToIgnore.AddUnique(x);
	}
	FHitResult BoxHit;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.0f, 5.0f, 5.0f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorToIgnore,
		EDrawDebugTrace::None, //
		BoxHit,
		true
	);

	if (BoxHit.GetActor()) {
	//	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *BoxHit.GetActor()->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *GetOwner()->GetName());
			
		IMyInterface* HitInterface = Cast<IMyInterface>(BoxHit.GetActor());
		DrawDebugSphere(
			GetWorld(),
			BoxHit.ImpactPoint,
			10.f,     // 半径
			12,       // 分段（越大越圆）
			FColor::Green,
			false,
			2.f
		);

		if (HitInterface) {
			//伤害在动画前面 解决血量更新不及时的问题
			UGameplayStatics::ApplyDamage(
				BoxHit.GetActor(),
				Damage,
				GetInstigator()->GetController(),
				this,
				UDamageType::StaticClass()
			);
			HitInterface->GetHit(BoxHit.ImpactPoint);

			//顿帧
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05f);
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
				}, 0.0005f, false);
			
		}
	
		IgnoreActor.AddUnique(BoxHit.GetActor());
	
		CreateFields(BoxHit.ImpactPoint);

	}


}

