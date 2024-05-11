// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "AnimNotify_PlaySoundAndReport.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API UAnimNotify_PlaySoundAndReport : public UAnimNotify_PlaySound
{
	GENERATED_BODY()
	
public:
	UAnimNotify_PlaySoundAndReport();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Report")
	float ReportLoundness;

};
