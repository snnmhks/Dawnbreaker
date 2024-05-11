// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "WeaponDamageType.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API UWeaponDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UWeaponDamageType();
};

UCLASS()
class CD2_API UWeaponDamageType_Head : public UDamageType
{
	GENERATED_BODY()

public:
	UWeaponDamageType_Head();
};