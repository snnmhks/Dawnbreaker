// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AnimNotify_PlaySoundAndReport.h"
#include "Perception/AISense_Hearing.h"

UAnimNotify_PlaySoundAndReport::UAnimNotify_PlaySoundAndReport()
{

}

void UAnimNotify_PlaySoundAndReport::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

    if (MeshComp)
    {
        AActor* OwnerActor = MeshComp->GetOwner();
        if (OwnerActor)
        {
            FVector Location = MeshComp->GetComponentLocation();
            UAISense_Hearing::ReportNoiseEvent(MeshComp->GetWorld(), Location, ReportLoundness, OwnerActor, 1000.0f);
        }
    }
}
