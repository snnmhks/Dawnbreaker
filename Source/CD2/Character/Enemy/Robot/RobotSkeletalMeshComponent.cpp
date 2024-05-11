// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Robot/RobotSkeletalMeshComponent.h"

URobotSkeletalMeshComponent::URobotSkeletalMeshComponent()
{

}

bool URobotSkeletalMeshComponent::HPMius(int32 Value)
{
	HP -= Value;
	return HP <= 0;
}

void URobotSkeletalMeshComponent::DestroyRobotComponent(float DDT)
{
	if (DDT < 0) return;
	GetWorld()->GetTimerManager().SetTimer(DestroyComponentHandler, this, &URobotSkeletalMeshComponent::DRC, 1.0f, false, DDT);
}

void URobotSkeletalMeshComponent::SetRCPhysics()
{
	DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SetCollisionProfileName("RobotMesh");
	SetSimulatePhysics(true);
}

void URobotSkeletalMeshComponent::ApplyDestroyForce(FVector HittedLocation, float ForecScale)
{
	//FVector DestroyForceDirection = GetRelativeLocation() - HittedLocation;
	//float DestroyForceDistance = DestroyForceDirection.Length();
	//DestroyForceDirection.Normalize();
	
	FVector DestroyForce = FVector::ZeroVector;
	DestroyForce.X = FMath::RandRange(-1.0f, 1.0f);
	DestroyForce.Y = FMath::RandRange(-1.0f, 1.0f);
	DestroyForce.Z = FMath::RandRange(-1.0f, 1.0f);
	DestroyForce.Normalize();

	AddImpulse(DestroyForce * ForecScale);
}

void URobotSkeletalMeshComponent::DRC()
{
	DestroyComponent();
}

void URobotSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	HP = MaxHP;
}