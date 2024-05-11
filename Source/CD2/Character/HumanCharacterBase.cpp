// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/DamageEvents.h"
#include "Weapon/WeaponDamageType.h"
#include "Animation/CharacterAnimInstance.h"
#include "CD2/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h" // 소캣을 사용하기 위해
#include "Character/HumanCharacterBase.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/PeceptionStimuliSourceActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/SmoothMoveCameraComponent.h"
#include "CDCharacterState.h"

// Sets default values
AHumanCharacterBase::AHumanCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCS = CreateDefaultSubobject<UCharacterStates>(TEXT("CharacterState"));
}

void AHumanCharacterBase::DeathEvent_Implementation()
{

}

void AHumanCharacterBase::CharacterStateUpdate()
{
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (CMC->IsFalling()) UCS->AddState<UCharacterJumpStateClass>(EStateEnum::STATE_Jump);
	else UCS->DeleteState(EStateEnum::STATE_Jump);

	FVector TempVelocity = CMC->Velocity;
	TempVelocity.Z = 0;
	if (TempVelocity.Length() > 2.0f) UCS->AddState<UCharacterMoveStateClass>(EStateEnum::STATE_Move);
	else UCS->DeleteState(EStateEnum::STATE_Move);
}

// Called when the game starts or when spawned
void AHumanCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	FixedBeginSetting();
	BeginSetting();
}

// Called every frame
void AHumanCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CharacterStateUpdate();
}

FName AHumanCharacterBase::GetHaveWeaponName()
{
	if (GetHandWeapon() == nullptr)
	{
		return EName::None;
	}
	return GetHandWeapon()->WeaponName;
}

void AHumanCharacterBase::SetAnimInstance()
{
	MyAnim = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
}

void AHumanCharacterBase::SetAnimDelegate(UCharacterAnimInstance* _MyAnim)
{
	if (_MyAnim && !_MyAnim->bIsBindedDelegate)
	{
		_MyAnim->OnMontageEnded.AddDynamic(this, &AHumanCharacterBase::MontageEnd);

		_MyAnim->OnAttackStart.AddLambda([this]()->void {
			UCS->AddState<UCharacterAttackStateClass>(EStateEnum::STATE_Attack);
		});

		_MyAnim->OnWeaponEquipCheck.AddLambda([this]()->void {

			if (Weapons[WeaponIndex] != nullptr)
			{
				bIsInWeaponHand = false;
				AttatchWeapon(Weapons[WeaponIndex]);
				bIsInWeaponClavicle = false;
				bIsInWeaponThigh = false;
				AttatchWeapon(Weapons[BeforeWeaponIndex]);
			}
		});
	}
}

void AHumanCharacterBase::FixedBeginSetting()
{
	HP = MaxHP;
	
	HeadStimuliSource = GetWorld()->SpawnActor<APeceptionStimuliSourceActor>(APeceptionStimuliSourceActor::StaticClass());
	GetMesh()->GetSocketByName("headSocket")->AttachActor(HeadStimuliSource, GetMesh());

	Tags.Add("Character");

	for (TSubclassOf<AWeapon> W : InitialWeapons)
	{
		GetWeaponToCharacter(W);
	}
	for (AWeapon* W : Weapons)
	{
		AttatchWeapon(W);
	}

	SetAnimInstance();
	SetAnimDelegate(MyAnim);
}

void AHumanCharacterBase::GetWeaponToCharacter(TSubclassOf<AWeapon> _Weapon)
{
	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(_Weapon);
	Weapons.Add(SpawnedWeapon);
}

void AHumanCharacterBase::WeaponFireBase(FVector DirectionCorrection)
{
	if (GetHandWeapon()->RemainBullet >= 1 || GetHandWeapon()->bIsBulletInfinite) {
		GetHandWeapon()->WeaponFire(DirectionCorrection);
		PlayFireMontage();
	}
	else if (GetHandWeapon()->RemainBullet < 1)
	{
		WeaponReloadBase();
	}
}

void AHumanCharacterBase::WeaponFireEndBase()
{

}

void AHumanCharacterBase::WeaponReloadBase()
{
	GetHandWeapon()->WeaponReload();
	PlayReloadMontage();
}

void AHumanCharacterBase::WeaponChangeBase(float Value)
{
	BeforeWeaponIndex = WeaponIndex;
	WeaponIndex += Value;
	if (WeaponIndex == 3) WeaponIndex = 0;
	if (WeaponIndex == -1) WeaponIndex = 2;

	if (GetHaveWeaponName() == "None")
	{
		bIsInWeaponHand = false;
		AttatchWeapon(Weapons[WeaponIndex]);
		bIsInWeaponClavicle = false;
		bIsInWeaponThigh = false;
		AttatchWeapon(Weapons[BeforeWeaponIndex]);
	}

	GetMesh()->SetAnimInstanceClass(GetHandWeapon()->GetLinkedCharacterAnimClass());
	SetAnimInstance();
	SetAnimDelegate(MyAnim);
}

void AHumanCharacterBase::AttatchWeapon(AWeapon* _Weapon) {
	if (!bIsInWeaponHand)
	{
		GetMesh()->GetSocketByName("hand_rSocket")->AttachActor(_Weapon, GetMesh());
		bIsInWeaponHand = true;
	}
	else
	{
		if (!bIsInWeaponClavicle && _Weapon->WeaponName == "Rifle")
		{
			GetMesh()->GetSocketByName("Clavicle_lSocket")->AttachActor(_Weapon, GetMesh());
			bIsInWeaponClavicle = true;
		}
		else if (!bIsInWeaponThigh && _Weapon->WeaponName == "Pistol")
		{
			GetMesh()->GetSocketByName("Thigh_lSocket")->AttachActor(_Weapon, GetMesh());
			bIsInWeaponThigh = true;
		}
	}
}

void AHumanCharacterBase::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UCS->DeleteState(MyAnim->GetMontageTag(Montage));
	MyAnim->DeleteCustomTag(Montage);
}

bool AHumanCharacterBase::IsAlive2()
{
	return HP > 0;
}

void AHumanCharacterBase::HPMius(float Value)
{
	HP -= Value;
	if (!IsAlive2())
	{
		bIsDie = true;
		DeathEvent();
	}
}

AWeapon* AHumanCharacterBase::GetHandWeapon()
{
	return Weapons[WeaponIndex];
}

void AHumanCharacterBase::AttatchCamera()
{
	if (IsAiming)
	{
		if (bIsCrouching) SmoothMoveCamera->AttachToComponent(CrouchAimBoom, FAttachmentTransformRules::KeepWorldTransform);
		else SmoothMoveCamera->AttachToComponent(AimBoom, FAttachmentTransformRules::KeepWorldTransform);
	}
	else 
	{
		SmoothMoveCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AHumanCharacterBase::CameraSmoothMove()
{
	if (IsAiming)
	{
		if (bIsCrouching) SmoothMoveCamera->SmoothMoveToTargetLocation(CrouchAimCameraPosition);
		else SmoothMoveCamera->SmoothMoveToTargetLocation(AimCameraPosition);
	}
	else
	{
		SmoothMoveCamera->SmoothMoveToTargetLocation(BasicCameraPosition);
	}
}

float AHumanCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.DamageTypeClass == UWeaponDamageType::StaticClass())
	{
		PlayHitReactMontage();
		HPMius(DamageAmount);
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);;
}

void AHumanCharacterBase::BeginSetting()
{

}

void AHumanCharacterBase::PlayHitReactMontage()
{

}

void AHumanCharacterBase::PlayFireMontage()
{

}

void AHumanCharacterBase::PlayReloadMontage()
{

}

