// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/DecalActor.h"
#include "Engine/DamageEvents.h"
#include "Weapon/WeaponDamageType.h"
#include "Components/ArrowComponent.h"
#include "Components/DecalComponent.h"
#include "Weapon/BulletBase.h"
#include "Character/Enemy/EnemyCharacterBase.h"
#include "WeaponAnimInstance.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Character/Animation/CharacterAnimInstance.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponFirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("WeaponFirePoint"));
	WeaponFirePoint->SetupAttachment(RootComponent);

	WeaponPerceptionSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("WeaponPerceptionSource"));
}

UClass* AWeapon::GetLinkedCharacterAnimClass()
{
	return LinkedCharacterAnimClass;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(WeaponMesh))
	{
		WeaponAnimInstance = Cast<UWeaponAnimInstance>(WeaponMesh->GetAnimInstance());
		WeaponMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

		if (WeaponAnimInstance)
		{
			WeaponAnimInstance->OnReloadEndCheck.AddLambda([this]()->void {
				RemainBullet = MaxBullet;
				});
		}
	}

	RemainBullet = MaxBullet;
	WeaponPerceptionSource->SetActive(true);
}

void AWeapon::SpawnDecalAtLocation(class UMaterialInterface* DecalMaterial, FVector SpawnLocation)
{
	ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(SpawnLocation, FRotator());
	if (Decal)
	{
		Decal->SetDecalMaterial(DecalMaterial);
		Decal->SetLifeSpan(1.0f);
		Decal->GetDecal()->DecalSize = FVector(5.0f, 5.0f, 5.0f);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAiming)
	{
#if ENABLE_DRAW_DEBUG
		DrawDebugLine(
			GetWorld(),
			WeaponFirePoint->GetComponentLocation(),
			WeaponFirePoint->GetComponentLocation() + WeaponFirePoint->GetForwardVector() * 1000,
			FColor::Red,
			false,
			0,
			0,
			1
		);
#endif
	}
}

USkeletalMeshComponent* AWeapon::GetMesh() {
	return WeaponMesh;
}

FVector AWeapon::GetWeaponForwardVector()
{
	return WeaponFirePoint->GetForwardVector();
}

FVector AWeapon::GetWeaponLocation()
{
	return WeaponFirePoint->GetComponentLocation();
}

void AWeapon::WeaponReload()
{
	PlayReloadMontage();
}

void AWeapon::WeaponFire(FVector DirectionCorrection)
{
	if (!bIsBulletInfinite)	RemainBullet--;
	PlayFireMontage();
	//DrawBulletLine(DirectionCorrection);
	FireBullet(DirectionCorrection);
	//SpawnFireEmitter();
}

void AWeapon::FireBullet(FVector DirectionCorrection)
{
	struct FHitResult HitResult;
	// FCollisionQueryParams (이 충돌을 식별할 태그 값, 복잡한 충돌 연산을 할 것이냐, 충돌 무시 오브젝트)
	FCollisionQueryParams Params(EName::None, false, this);

	FVector LastPosition = DirectionCorrection - WeaponFirePoint->GetComponentLocation();
	LastPosition.Normalize();

	ShootBullet(LastPosition);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WeaponFirePoint->GetComponentLocation(),
		WeaponFirePoint->GetComponentLocation() + LastPosition * BulletRange,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params);
	if (IsHit)
	{
		if (HitResult.GetActor())
		{
			float Damage = 1;
			UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), Damage, LastPosition, HitResult, nullptr, this, UWeaponDamageType::StaticClass());
		}
	}
}

void AWeapon::DrawBulletLine(FVector DirectionCorrection)
{
#if ENABLE_DRAW_DEBUG
	DrawDebugLine(
		GetWorld(),
		WeaponFirePoint->GetComponentLocation(),
		DirectionCorrection,
		FColor::Yellow,
		false,
		DrawLifeTime,
		DrawDepthPriority,
		LineThickness
	);
#endif
}

void AWeapon::SpawnFireEmitter()
{
	UGameplayStatics::SpawnEmitterAttached(FireEmitter, WeaponFirePoint, EName::None);
}

void AWeapon::ShootBullet(FVector Direction)
{
	if (BulletClass)
	{
		ABulletBase* Bullet = GetWorld()->SpawnActor<ABulletBase>(BulletClass);
		Bullet->SetActorLocation(WeaponFirePoint->GetComponentLocation());
		Bullet->FireBullet(Direction);
	}
}

bool AWeapon::IsFire()
{
	return WeaponAnimInstance->IsPlayFireMontage();
}

bool AWeapon::IsReload()
{
	return WeaponAnimInstance->IsPlayReloadMontage();
}

float AWeapon::PlayFireMontage() {
	if (!IsValid(WeaponMesh)) return 0.0f;
	return WeaponAnimInstance->PlayMongtage("Fire", FireSpeed);
}

float AWeapon::PlayReloadMontage()
{
	if (!IsValid(WeaponMesh)) return 0.0f;
	return WeaponAnimInstance->PlayMongtage("Reload", 1.0f);;
}

