// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RobotAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifyCheckDelegate);

/**
 * 
 */
UCLASS()
class CD2_API URobotAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Character")
	class ARobotCharacter* RobotCharacter = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Character|Movement")
	class UPawnMovementComponent* RobotMoement = nullptr;

	// Fire Ready Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* ReadyMontage;
	// Fire Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* AttackMontage;
	// Death Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DeathMontage;
	// Hit React Montage
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* HitReactMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class AHumanCharacterBase* Character;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	class UAnimSequence* IdleSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	class UAnimSequence* LandSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	class UAnimSequence* JumpSequence;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Sequence")
	class UAnimSequence* JumpStartSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Blend Space")
	class UBlendSpace* WalkAndRunBS;

	FOnNotifyCheckDelegate OnFireCheck;
	FOnNotifyCheckDelegate OnFirePossibleCheck;

public:
	UFUNCTION()
	float PlayHitReactMontage(float PlayRate);
	UFUNCTION()
	float PlayDeathMontage(float PlayRate);
	UFUNCTION()
	float PlayAttackMontage(float PlayRate);
	UFUNCTION()
	float PlayReadyMontage(float PlayRate);

	UFUNCTION()
	bool IsPlayAttackMontage();

	UFUNCTION()
	void JumpToSectionInAttackMontage();

	UFUNCTION()
	void AnimNotify_Attack();
	UFUNCTION()
	void AnimNotify_AttackPossible();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
};
