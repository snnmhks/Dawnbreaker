// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

class AEnemyWeaponBase;

UCLASS()
class CD2_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State|HP")
	int32 MaxHP;
	UPROPERTY()
	int32 HP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Die")
	bool bIsDie = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State|Attack")
	float AttackSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	float AimPitchValue;

	UPROPERTY(VisibleAnywhere, Category = "Locomotion|Walk")
	float RunSpeed;
	UPROPERTY(VisibleAnywhere, Category = "Locomotion|Walk")
	float WalkSpeed;
	UPROPERTY(VisibleAnywhere, Category = "Locomotion|Jump")
	float JumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool bOnAimDetail;

	// Enemy Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AEnemyWeaponBase* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AEnemyWeaponBase> InitialWeapon = nullptr;

	UFUNCTION()
	virtual void EnemyAttack(class AActor* Target);

	UFUNCTION()
	virtual void EnemyReadyToAttack(class AActor* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginSetting();

	virtual void DeathEventInScript();

	void HPMius(float Value);
	bool IsAlive();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ComponentHit(class UPrimitiveComponent* _HittedComponent, int32 _Damage, FVector HittiedLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 블랙보드 가져오는 함수
	class UBlackboardComponent* GetEnemyBlackboardComponent();

	// AI 컨트롤러 가져오는 함수
	class AEnemyAIController* GetEnemyAIController();

	// 뛰냐 걷냐
	void RunMode();
	void WalkMode();

	UFUNCTION()
	FVector SetDetailAim(class ACharacter* OwnerActor, class ACharacter* Target);
	UFUNCTION()
	bool FindBone(FHitResult HR, FVector StartV, FVector EndV);
};
