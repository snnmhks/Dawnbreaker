// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyCharacterBase.h"
#include "RobotCharacter.generated.h"

/**
 * 
 */

class URobotSkeletalMeshComponent;

UCLASS()
class CD2_API ARobotCharacter : public AEnemyCharacterBase
{
	GENERATED_BODY()
	
public:
	ARobotCharacter();

	// Character Animation Instance
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<class URobotAnimInstance*> RobotAnims;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* Head;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* LeftLeg;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* LeftLegLower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* LeftLegFoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* RightLeg;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* RightLegLower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* RightLegFoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* LeftArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* LeftArmLower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* LeftArmHand;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* RightArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* RightArmLower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	URobotSkeletalMeshComponent* RightArmHand;

	// ºÎÀ§ ÆÄ±«
	UFUNCTION(BlueprintNativeEvent)
	void PartsSeperate(URobotSkeletalMeshComponent* Parts, FVector HittiedLocation);
	virtual void PartsSeperate_Implementation(URobotSkeletalMeshComponent* Parts, FVector HittiedLocation);
	UFUNCTION(BlueprintNativeEvent)
	void DestroyArmor(URobotSkeletalMeshComponent* Parts);
	virtual void DestroyArmor_Implementation(URobotSkeletalMeshComponent* Parts);

	virtual void EnemyAttack(class AActor* Target) override;
	virtual void EnemyReadyToAttack(class AActor* Target) override;

protected:
	virtual void BeginSetting() override;
	virtual void DeathEventInScript();
	virtual void ComponentHit(class UPrimitiveComponent* _HittedComponent, int32 _Damage, FVector HittiedLocation) override;

	UFUNCTION()
	void MontageEndEvnet(class UAnimMontage* Montage, bool bInterrupted);
};
