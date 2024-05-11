// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ArrowComponent.h"
#include "Weapon/BulletBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Character/Enemy/EnemyWeaponBase.h"
#include "Character/HumanCharacterBase.h"
#include "Engine/DamageEvents.h"
#include "Weapon/WeaponDamageType.h"
#include "C:\Users\snnmh\Desktop\CD2\Source\CD2\Interface\IdentificationInterface.h"

// Sets default values
AEnemyWeaponBase::AEnemyWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_WeaponMesh"));
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_WeaponMesh"));
	WeaponFirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("WeaponFirePoint"));

	SetRootComponent(WeaponMesh);

	WeaponStaticMesh->SetupAttachment(RootComponent);
	WeaponFirePoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	RemainBullet = MaxBullet;

}

// Called every frame
void AEnemyWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEnemyWeaponBase::IsPossibleWeaponFire()
{
	struct FHitResult HitResult;
	// FCollisionQueryParams (이 충돌을 식별할 태그 값, 복잡한 충돌 연산을 할 것이냐, 충돌 무시 오브젝트)
	FCollisionQueryParams Params(EName::None, false, this);

	FVector Direction = Target - WeaponFirePoint->GetComponentLocation();
	Direction.Normalize();

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WeaponFirePoint->GetComponentLocation(),
		WeaponFirePoint->GetComponentLocation() + Direction * 3000,
		ECollisionChannel::ECC_GameTraceChannel2,
		Params);

	if (IsHit)
	{
		IIdentificationPlayer* Identifier = HitResult.GetActor() ? Cast<IIdentificationPlayer>(HitResult.GetActor()) : nullptr;
		if (Identifier) return true;
	}

	return false;
}

void AEnemyWeaponBase::WeaponFire()
{
	struct FHitResult HitResult;
	// FCollisionQueryParams (이 충돌을 식별할 태그 값, 복잡한 충돌 연산을 할 것이냐, 충돌 무시 오브젝트)
	FCollisionQueryParams Params(EName::None, false, this);

	FVector Direction = Target - WeaponFirePoint->GetComponentLocation();
	Direction.Normalize();

	// 산탄 방향 설정
	FVector Spread = FVector::ZeroVector;
	Spread.X = FMath::RandRange(-1.0f, 1.0f);
	Spread.Y = FMath::RandRange(-1.0f, 1.0f);
	Spread.Z = -(Direction.X * Spread.X + Direction.Y * Spread.Y) / (Direction.Z + 0.01f);
	Spread.Normalize();

	// 박스 뮬러 공식으로 산탄의 정도를 정규분포를 따르게함
	float BoxMuller1 = FMath::RandRange(0.01f, 1.0f);
	float BoxMuller2 = FMath::RandRange(0.0f, 1.0f);
	float Gaussian = FMath::Sqrt(-2 * FMath::Loge(BoxMuller1)) * FMath::Cos(2 * 3.14 * BoxMuller2);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WeaponFirePoint->GetComponentLocation(),
		WeaponFirePoint->GetComponentLocation() + (Direction + Spread * Gaussian * SpreadScale) * 3000,
		ECollisionChannel::ECC_GameTraceChannel2,
		Params);
	if (IsHit)
	{
		if (HitResult.GetActor())
		{
			float Damage = 1;
			UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), Damage, Direction, HitResult, nullptr, this, UWeaponDamageType::StaticClass());
		}
	}

	ShootBullet(Direction + Spread * Gaussian * SpreadScale);
/*
#if ENABLE_DRAW_DEBUG
	DrawDebugLine(
		GetWorld(),
		WeaponFirePoint->GetComponentLocation(),
		IsHit ? HitResult.Location : WeaponFirePoint->GetComponentLocation() + Direction * 3000 + Spread * Gaussian * SpreadScale,
		FColor::Yellow,
		false,
		0.1,
		0,
		1
	);
#endif
*/
}

void AEnemyWeaponBase::ShootBullet(FVector Direction)
{
	if (BulletClass)
	{
		ABulletBase* Bullet = GetWorld()->SpawnActor<ABulletBase>(BulletClass);
		Bullet->SetActorLocation(WeaponFirePoint->GetComponentLocation());
		Bullet->FireBullet(Direction);
	}
}

