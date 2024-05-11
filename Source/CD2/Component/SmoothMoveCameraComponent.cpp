// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetMathLibrary.h"
#include "Component/SmoothMoveCameraComponent.h"

void USmoothMoveCameraComponent::SmoothMoveToTargetLocation(USceneComponent* _Target)
{
	bIsCameraMoveable = true;
	CurrentMoveTime = 0.0f;
	Target = _Target;
}

void USmoothMoveCameraComponent::SmoothMove(float DeltaTime)
{
	FVector NewLocation = UKismetMathLibrary::VInterpTo(this->GetComponentLocation(), Target->GetComponentLocation(), DeltaTime, MoveSpeed);
	this->SetWorldLocation(NewLocation);
	if (UKismetMathLibrary::VSize(this->GetComponentLocation() - Target->GetComponentLocation()) < 1.0f || CurrentMoveTime > MaxMoveTime)
	{
		SmoothMoveEnd();
	}
	CurrentMoveTime += DeltaTime;
}

void USmoothMoveCameraComponent::SmoothMoveEnd()
{
	bIsCameraMoveable = false;
	Target = nullptr;
	if (OnCameraMoveEnd.IsBound()) OnCameraMoveEnd.Execute();
}

void USmoothMoveCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCameraMoveable)
	{
		SmoothMove(DeltaTime);
	}
}