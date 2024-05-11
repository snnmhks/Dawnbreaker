// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/BulletBase.h"

ABulletBase::ABulletBase()
{
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	SetRootComponent(BulletMesh);
	BulletMesh->SetCollisionProfileName("Bullet");
	BulletMesh->SetSimulatePhysics(true);
	BulletMesh->SetMobility(EComponentMobility::Movable);
	BulletMesh->SetNotifyRigidBodyCollision(true);

	//BulletMesh->OnComponentHit.AddDynamic(this, &ABulletBase::BulletDestroy);
	OnActorHit.AddDynamic(this, &ABulletBase::BulletDestroy);
}

void ABulletBase::FireBullet(FVector Direction)
{
	BulletMesh->AddImpulse(BulletSpeed * Direction);
}

void ABulletBase::SpawnBulletEffcet_Implementation(FVector ImpacNormal)
{

}

void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(BulletDestroyHandle, this, &ABulletBase::BulletDsetroyTimer, 1.0f, false , BulletLifeTime);
}

void ABulletBase::BulletDestroy(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	SpawnBulletEffcet(Hit.ImpactNormal);
}

void ABulletBase::BulletDsetroyTimer()
{
	Destroy();
}
