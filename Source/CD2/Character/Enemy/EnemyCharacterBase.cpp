// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/EnemyWeaponBase.h"
#include "Character/Enemy/EnemyAIController.h"
#include "Character/Enemy/EnemyCharacterBase.h"
#include "Weapon/WeaponDamageType.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	BeginSetting();

	HP = MaxHP;
	GetCharacterMovement()->JumpZVelocity = JumpHeight;

	if (IsValid(InitialWeapon))
	{
		Weapon = GetWorld()->SpawnActor<AEnemyWeaponBase>(InitialWeapon);
		GetMesh()->GetSocketByName("hand_rSocket")->AttachActor(Weapon, GetMesh());
	}
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.DamageTypeClass == UWeaponDamageType::StaticClass())
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		FHitResult HitResult = PointDamageEvent->HitInfo;
	
		ComponentHit(HitResult.GetComponent(), DamageAmount, HitResult.Location);
		HPMius(DamageAmount);

		GetEnemyAIController()->EnemyDamageReport(GetWorld(), this, DamageCauser, DamageAmount, DamageCauser->GetActorLocation(), GetActorLocation());
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyCharacterBase::ComponentHit(UPrimitiveComponent* _HittedComponent, int32 _Damage, FVector HittiedLocation)
{

}

UBlackboardComponent* AEnemyCharacterBase::GetEnemyBlackboardComponent()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return nullptr;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) return nullptr;

	return BlackboardComp;
}

AEnemyAIController* AEnemyCharacterBase::GetEnemyAIController()
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (!EnemyAIController) return nullptr;
	return EnemyAIController;
}

void AEnemyCharacterBase::RunMode()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AEnemyCharacterBase::WalkMode()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

FVector AEnemyCharacterBase::SetDetailAim(ACharacter* OwnerActor, ACharacter* Target)
{
	struct FHitResult HitResult;
	FVector StartVector = OwnerActor->GetMesh()->GetBoneLocation("head");

	FVector EndVector = Target->GetMesh()->GetBoneLocation("spine_04");
	if (FindBone(HitResult, StartVector, EndVector)) return EndVector;

	EndVector = Target->GetMesh()->GetBoneLocation("head");
	if (FindBone(HitResult, StartVector, EndVector)) return EndVector;

	EndVector = Target->GetMesh()->GetBoneLocation("lowerarm_r");
	if (FindBone(HitResult, StartVector, EndVector)) return EndVector;

	EndVector = Target->GetMesh()->GetBoneLocation("lowerarm_l");
	if (FindBone(HitResult, StartVector, EndVector)) return EndVector;

	EndVector = Target->GetMesh()->GetBoneLocation("calf_r");
	if (FindBone(HitResult, StartVector, EndVector)) return EndVector;

	EndVector = Target->GetMesh()->GetBoneLocation("calf_l");
	if (FindBone(HitResult, StartVector, EndVector)) return EndVector;

	return OwnerActor->GetActorLocation();
}

bool AEnemyCharacterBase::FindBone(FHitResult HR, FVector StartV, FVector EndV)
{
	FCollisionQueryParams Params(EName::None, false, this);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HR,
		StartV,
		EndV,
		ECollisionChannel::ECC_GameTraceChannel3,
		Params);

	return (IsHit && HR.GetActor());
}

void AEnemyCharacterBase::BeginSetting()
{

}

void AEnemyCharacterBase::EnemyAttack(AActor* Target)
{

}

void AEnemyCharacterBase::EnemyReadyToAttack(AActor* Target)
{

}


void AEnemyCharacterBase::DeathEventInScript()
{

}

void AEnemyCharacterBase::HPMius(float Value)
{
	HP -= Value;
	if (!IsAlive())
	{
		bIsDie = true;
		DeathEventInScript();
	}
}

bool AEnemyCharacterBase::IsAlive()
{
	return HP > 0;
}


