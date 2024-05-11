// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/DamageEvents.h"
#include "Character/Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Enemy/Robot/RobotCharacter.h"
#include "Weapon/WeaponDamageType.h"
#include "Character/Enemy/EnemyWeaponBase.h"
#include "Character/Enemy/Animation/RobotAnimInstance.h"
#include "RobotSkeletalMeshComponent.h"
#include "Character/Enemy/EnemyAIController.h"

ARobotCharacter::ARobotCharacter()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.00085f, -91.997f), FRotator(0.0f, -90.0f, 0.0f));
	Head = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetMesh());

	LeftLeg = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("LeftLeg"));
	LeftLeg->SetupAttachment(GetMesh());

	LeftLegLower = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("LeftLegLower"));
	LeftLegLower->SetupAttachment(LeftLeg);

	LeftLegFoot = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("LeftLegFoot"));
	LeftLegFoot->SetupAttachment(LeftLegLower);

	RightLeg = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("RightLeg"));
	RightLeg->SetupAttachment(GetMesh());

	RightLegLower = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("RightLegLower"));
	RightLegLower->SetupAttachment(RightLeg);

	RightLegFoot = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("RightLegFoot"));
	RightLegFoot->SetupAttachment(RightLegLower);

	LeftArm = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("LeftArm"));
	LeftArm->SetupAttachment(GetMesh());

	LeftArmLower = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("LeftArmLower"));
	LeftArmLower->SetupAttachment(LeftArm);

	LeftArmHand = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("LeftArmHand"));
	LeftArmHand->SetupAttachment(LeftArmLower);

	RightArm = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("RightArm"));
	RightArm->SetupAttachment(GetMesh());

	RightArmLower = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("RightArmLower"));
	RightArmLower->SetupAttachment(RightArm);

	RightArmHand = CreateDefaultSubobject<URobotSkeletalMeshComponent>(TEXT("RightArmHand"));
	RightArmHand->SetupAttachment(RightArmLower);

}

void ARobotCharacter::PartsSeperate_Implementation(URobotSkeletalMeshComponent* Parts, FVector HittiedLocation)
{

}

void ARobotCharacter::DestroyArmor_Implementation(URobotSkeletalMeshComponent* Parts)
{

}

void ARobotCharacter::EnemyAttack(AActor* Target)
{
	if (RobotAnims[0]->IsPlayAttackMontage()) return;

	FVector TargetLocation = Target->GetActorLocation();

	for (URobotAnimInstance* RAI : RobotAnims)
	{
		RAI->PlayAttackMontage(AttackSpeed);
	}

	Weapon->Target = TargetLocation;
}

void ARobotCharacter::EnemyReadyToAttack(AActor* Target)
{
	if (RobotAnims[0]->IsPlayAttackMontage()) return;

	FVector TargetLocation = Target->GetActorLocation();

	for (URobotAnimInstance* RAI : RobotAnims)
	{
		RAI->PlayReadyMontage(AttackSpeed);
	}

	Weapon->Target = TargetLocation;
}

void ARobotCharacter::BeginSetting()
{
	RobotAnims.Add(Cast<URobotAnimInstance>(GetMesh()->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(Head->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(LeftLeg->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(LeftLegLower->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(LeftLegFoot->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(RightLeg->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(RightLegLower->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(RightLegFoot->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(LeftArm->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(LeftArmLower->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(LeftArmHand->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(RightArm->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(RightArmLower->GetAnimInstance()));
	RobotAnims.Add(Cast<URobotAnimInstance>(RightArmHand->GetAnimInstance()));

	RobotAnims[0]->OnFireCheck.AddLambda([this]()->void {
		Weapon->WeaponFire();
	});

	RobotAnims[0]->OnFirePossibleCheck.AddLambda([this]()->void {
		if (Weapon->IsPossibleWeaponFire())
		{
			GetEnemyAIController()->SetBBValueAsBool(AEnemyAIController::KeyIsPossibleAttack, true);
			for (URobotAnimInstance* RAI : RobotAnims)
			{
				RAI->JumpToSectionInAttackMontage();
			}
		}
		else
		{
			GetEnemyAIController()->SetBBValueAsBool(AEnemyAIController::KeyIsPossibleAttack, false);
		}
	});

	RobotAnims[0]->OnMontageEnded.AddDynamic(this, &ARobotCharacter::MontageEndEvnet);

	RunSpeed = 500.0f;
	WalkSpeed = 200.0f;
	JumpHeight = 900.0f;
	WalkMode();
}

void ARobotCharacter::DeathEventInScript()
{
	GetEnemyAIController()->StopBT();

	for (URobotAnimInstance* RAI : RobotAnims)
	{
		RAI->PlayDeathMontage(1.0f);
	}
}

void ARobotCharacter::ComponentHit(UPrimitiveComponent* _HittedComponent, int32 _Damage, FVector HittiedLocation)
{
	URobotSkeletalMeshComponent* RSM = Cast<URobotSkeletalMeshComponent>(_HittedComponent);
	if (!RSM) return;

	if (RSM->HPMius(_Damage))
	{
		FString ComponentName = RSM->GetName();
		if (ComponentName == "Head")
		{
			GetEnemyAIController()->DisableSightPerception();
		}
		else if (ComponentName.Contains("LeftArm"))
		{
			if (Weapon)
			{
				Weapon->SpreadScale *= 2;
			}
		}
		else if (ComponentName.Contains("Leg"))
		{
			WalkMode();
			RunSpeed = 200;
		}
		PartsSeperate(RSM, HittiedLocation);
	}
}

void ARobotCharacter::MontageEndEvnet(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
