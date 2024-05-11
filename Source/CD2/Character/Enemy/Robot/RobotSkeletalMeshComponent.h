// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "RobotSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Robot Skeletal Mesh Component", BlueprintSpawnableComponent))
class CD2_API URobotSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	URobotSkeletalMeshComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	int32 MaxHP;
	UPROPERTY()
	int32 HP;

	UFUNCTION()
	bool HPMius(int32 Value);

	FTimerHandle DestroyComponentHandler;

	UFUNCTION(BlueprintCallable)
	void DestroyRobotComponent(float DDT);
	
	UFUNCTION(BlueprintCallable)
	void SetRCPhysics();

	UFUNCTION(BlueprintCallable)
	void ApplyDestroyForce(FVector HittedLocation, float ForecScale);

	void DRC();

protected:
	virtual void BeginPlay() override;
};
