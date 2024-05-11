// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Animation/RobotAnimInstance.h"

float URobotAnimInstance::PlayHitReactMontage(float PlayRate)
{
	if (HitReactMontage)
	{
		return Montage_Play(HitReactMontage, PlayRate);
	}
	return 0.0f;
}

float URobotAnimInstance::PlayDeathMontage(float PlayRate)
{
	if (DeathMontage)
	{
		return Montage_Play(DeathMontage, PlayRate);
	}
	return 0.0f;
}

float URobotAnimInstance::PlayAttackMontage(float PlayRate)
{
	if (AttackMontage)
	{
		return Montage_Play(AttackMontage, PlayRate);
	}
	return 0.0f;
}

float URobotAnimInstance::PlayReadyMontage(float PlayRate)
{
	if (ReadyMontage)
	{
		return Montage_Play(ReadyMontage, PlayRate);
	}
	return 0.0f;
}

bool URobotAnimInstance::IsPlayAttackMontage()
{
	return Montage_IsPlaying(AttackMontage);
}

void URobotAnimInstance::JumpToSectionInAttackMontage()
{
	Montage_JumpToSection("Fire", AttackMontage);
}

void URobotAnimInstance::AnimNotify_Attack()
{
	OnFireCheck.Broadcast();
}

void URobotAnimInstance::AnimNotify_AttackPossible()
{
	OnFirePossibleCheck.Broadcast();
}

void URobotAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	RobotCharacter = Cast<ARobotCharacter>(TryGetPawnOwner());
	RobotMoement = TryGetPawnOwner()->GetMovementComponent();
}

void URobotAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}
