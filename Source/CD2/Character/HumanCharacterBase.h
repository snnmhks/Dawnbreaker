// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C:\Users\snnmh\Desktop\CD2\Source\CD2\Interface\IdentificationInterface.h"
#include "HumanCharacterBase.generated.h"

class AWeapon;
class USpringArmComponent;
class UStaticMeshComponent;
class USmoothMoveCameraComponent;
class CharacterStateClasses;
class IIdentificationPlayer;

UCLASS()
class CD2_API AHumanCharacterBase : public ACharacter, public IIdentificationPlayer
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanCharacterBase();

	// �ѵ��� Ŭ���� -> �ӽ÷� �����°Ű� ���Ŀ��� �ʵ忡 �ִ� ���⸦ ��� ó���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState)
	UClass* InitialWeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState)
	UClass* InitialWeaponClass2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState)
	UClass* InitialWeaponClass3;

	// Camera Position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UStaticMeshComponent* BasicCameraPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UStaticMeshComponent* AimCameraPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UStaticMeshComponent* CrouchAimCameraPosition;
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* AimBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CrouchAimBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	USmoothMoveCameraComponent* SmoothMoveCamera;

	UPROPERTY()
	class UCharacterStates* UCS = nullptr;
	UPROPERTY()
	class APeceptionStimuliSourceActor* HeadStimuliSource = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	int32 MaxHP;
	UPROPERTY()
	int32 HP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsDie = false;

	// �տ� ���Ⱑ �ִ°�?
	UPROPERTY()
	bool bIsInWeaponHand = false;
	// ����㿡 ���Ⱑ �ִ°�?
	UPROPERTY()
	bool bIsInWeaponClavicle = false;
	// ������� ���Ⱑ �ִ°�?
	UPROPERTY()
	bool bIsInWeaponThigh = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Crouch")
	bool bIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Slide")
	bool bIsSliding;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Aim")
	float CollectionPitchValue;

	// Character Weapon list
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<AWeapon*> Weapons;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<TSubclassOf<AWeapon>> InitialWeapons;

	// ���� �տ� ��� �ִ� ���� �ε���
	UPROPERTY()
	int32 WeaponIndex = 0;
	UPROPERTY()
	int32 BeforeWeaponIndex = 0;

	// ���� ����
	UPROPERTY()
	class UPrimitiveComponent* HittedComponent;

	// ĳ���Ͱ� �� �����ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState")
	bool IsAiming;

	UFUNCTION(BlueprintNativeEvent)
	void DeathEvent();
	virtual void DeathEvent_Implementation();

	UFUNCTION()
	void CharacterStateUpdate();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Character Animation Instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UCharacterAnimInstance* MyAnim;

	// ���� ��� �ִ� ������ �̸��� ����
	UFUNCTION(BlueprintCallable)
	FName GetHaveWeaponName();

	// ĳ���� �ִ� �ν��Ͻ��� ��ü�Ǹ� �׿� �°� ��ũ��Ʈ���� ���Ǵ� �ִ� �ν��Ͻ��� �ٲپ��ִ� �Լ�
	UFUNCTION(BlueprintCallable)
	void SetAnimInstance();
	UFUNCTION()
	void SetAnimDelegate(UCharacterAnimInstance* _MyAnim);

	UFUNCTION()
	void FixedBeginSetting();

	UFUNCTION()
	void GetWeaponToCharacter(TSubclassOf<AWeapon> _Weapon);
	UFUNCTION()
	void WeaponFireBase(FVector DirectionCorrection);
	UFUNCTION()
	void WeaponFireEndBase();
	UFUNCTION()
	void WeaponReloadBase();
	UFUNCTION()
	void WeaponChangeBase(float Value);
	UFUNCTION()
	virtual void AttatchWeapon(AWeapon* _Weapon);

	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

	bool IsAlive2();
	void HPMius(float Value);

	AWeapon* GetHandWeapon();

	UFUNCTION()
	void AttatchCamera();
	UFUNCTION()
	void CameraSmoothMove();

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void BeginSetting();
	UFUNCTION()
	virtual void PlayHitReactMontage();
	UFUNCTION()
	virtual void PlayFireMontage();
	UFUNCTION()
	virtual void PlayReloadMontage();
};
