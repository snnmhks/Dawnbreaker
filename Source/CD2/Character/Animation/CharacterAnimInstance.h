// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CDCharacterState.h"
#include "CharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifyCheckDelegate);

/**
 * 
 */

class UAnimMontage;
class UAnimSequence;

UCLASS()
class CD2_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:

public:
	// 태그 종류, 모든 캐릭터 애님 인스턴스가 공유해야함 -> 떨어지는 도중에 무기 바꿨다고 떨어지는 상태가
	// 초기화되면 안됨
	static TMap<UAnimMontage*, EStateEnum> CustomTagMap;

	UFUNCTION()
	float PlayMontage(FName Name, float PlaySpeed);

	// 몽타주를 구별하기 위한 태그
	UFUNCTION()
	void AddCustomTag(UAnimMontage* _Montage, EStateEnum _MontageTag);
	UFUNCTION()
	void DeleteCustomTag(UAnimMontage* _Montage);
	UFUNCTION()
	EStateEnum GetMontageTag(UAnimMontage* _Montage);

	// Death Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* DeathMontage;
	// Hit React Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* HitReactMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class AHumanCharacterBase* Character = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class UCharacterMovementComponent* CharacterMC = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* IdleSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* SlidingSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* LandSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* JumpSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* JumpUpSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* JumpStartSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* CrouchStartSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* CrouchingSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	UAnimSequence* CrouchEndSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "State")
	bool bIsEquip = false;

	// Fire Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* FireMontage;
	// Reroad Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* ReloadMontage = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* WeaponEquipMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Aim")
	float AimOffsetPitchValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Recoil")
	float WeaponRecoilValue;

	FOnNotifyCheckDelegate OnWeaponFireCheck;
	FOnNotifyCheckDelegate OnWeaponEquipCheck;
	FOnNotifyCheckDelegate OnWeaponReloadEndCheck;
	FOnNotifyCheckDelegate OnAttackStart;
	UPROPERTY()
	bool bIsBindedDelegate = false;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	FVector2D Speed2D;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	float ZSpeed;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	float GroundSpeed;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	bool bShoudMove;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	bool bIsAiming;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	bool bIsCrouching;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	bool bIsSliding;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "CharacterState")
	bool bIsFalling;

public:
	UCharacterAnimInstance();

private:
	UFUNCTION()
	void AnimNotify_CallWeaponFire();
	UFUNCTION()
	void AnimNotify_EquipWeapon();
	UFUNCTION()
	void AnimNotify_EquipEnd();
	UFUNCTION()
	void AnimNotify_ReloadEnd();

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void CalculateSpeed();
	UFUNCTION()
	void CalculateState();
	UFUNCTION()
	void CalculatePitchValue();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Delta) override;
};

