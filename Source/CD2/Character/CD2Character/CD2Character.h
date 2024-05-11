// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/HumanCharacterBase.h"
#include "Logging/LogMacros.h"
#include "CD2/CD2.h"
#include "GameplayTagContainer.h"
#include "CD2Character.generated.h"

class USpringArmComponent;
class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
class AWeapon;
struct FInputActionValue;
class UCDGameplayAbility;
class UCDAbilitySystemComponent;
class UCDAttributeSet;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACD2Character* Character);

UCLASS(config=Game)
class ACD2Character : public AHumanCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// �ʱ⿡ ĳ���� �޽��� ������� ȸ�� ��
	// �����̵��� �ϴ� ���� �޽��� ȸ���ؾ��� ���� ����� ȸ���� ������ ���󺹱͸� �����ֱ� ���� ����
	UPROPERTY()
	FRotator BaseMeshRotator;

	// Zoom�� �ڵ����� ������ ��� �Ŀ� ���� �������̶�� �ٽ� ���� �ϰ��� ����
	UPROPERTY()
	bool bIsAimPressing = false;

	// ���� �ݵ� ��
	UPROPERTY()
	float WeaponRecoilVerticalValue;
	// ���� �ݵ� ��
	UPROPERTY()
	float WeaponRecoilHorizontalValue;
	// �ݵ� ȸ�� �Լ��� ��� ȣ���ߴ°�
	UPROPERTY()
	int32 CallRecoilRecoverEventNum;
	// �ݵ� ȸ�� ��
	UPROPERTY()
	float RecoverValue;

	// Commulative Weapon Recoil Vertical Value
	float CommulativeWRVV;
	// Commulative Weapon Recoil Horizontal Value
	float CommulativeWRHV;
	// �������� ���� ��� ���°� -> �ݵ� ��꿡 ��
	int32 WeaponRecoilX = 0;
	// �ݵ� ȸ�� �Լ��� ȣ���� Ÿ�̸�
	FTimerHandle WeaponRecoilRecoverHanlde;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Fire Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Reload Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/** Weapon Change Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WeaponChangeAction;

	/** Crouch Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* LeftFootSlideEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* RightFootSlideEffect;

public:
	ACD2Character(const class FObjectInitializer& ObjectInitializer);

	//UPROPERTY(BlueprintAssignable, Category = "Character")
	FCharacterDiedDelegate OnCharacterDied;

	// ȭ�� �߾ӿ� ǥ�õ� ������ ����
	UPROPERTY(EditAnyWhere, Category = "Widget")
	TSubclassOf<class UUserWidget> WAimWidgetClass;
	UPROPERTY()
	class UUserWidget* WAimWidget;

	UFUNCTION()
	virtual bool IsAlive() const;

	UFUNCTION()
	virtual int32 GetAbilityLevel(CDAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION()
	virtual void FinishDying();

	UFUNCTION()
	float GetCharacterLevel() const;
	UFUNCTION()
	float GetHealth() const;
	UFUNCTION()
	float GetMaxHealth() const;
	UFUNCTION()
	float GetMana() const;
	UFUNCTION()
	float GetMaxMana() const;

	// �� �߻�
	UFUNCTION()
	void WeaponFire();
	// ������
	UFUNCTION()
	void WeaponReload();
	// �ѱ� �ݵ��� ������ �ݵ��� ���� �����ִ� �Լ�
	UFUNCTION()
	void WeaponFireEnd();

protected:

	UPROPERTY(EditAnywhere, Category = "GAS.ASC")
	TWeakObjectPtr<UCDAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "GAS.AS")
	TWeakObjectPtr<UCDAttributeSet> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
	FText CharaterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Montage")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	TArray<TSubclassOf<class UGameplayAbility>> StartCharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effect")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effect")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpEffects;

	bool bACSInputBound = false;

	virtual void AddCharacterAbilities();

	virtual void InitializeAttributes();
	
	virtual void AddStartupEffects();

	virtual void SetHealth(float Health);

	virtual void SetMana(float Mana);


	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	virtual void Jump() override;

	/** Called for Aiming input */
	void AimPress();
	void AimRelease();
	void AimEnd();

	// ���� ��ü �Լ�
	void WeaponChange(const FInputActionValue& Value);

	// �ޱ��� �ɴ� �Լ�
	void CrouchStart();
	void CrouchEnd();

	// �����̵� �Լ�
	bool IsPossibleSide();
	void Sliding(float DeltaTime);
	void SlideEnd();

	// �۵��� �Ͽ� AimOffset�� ����
	void SetPitchCollection();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// GAS Setup InputComponent
	void SetupGASInputComponent();
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_PlayerState() override;

	// Ability Sysytems

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// BeginPlay�� ����� �Լ�, �⺻���� ���� �ܿ� �� ĳ���͸��� ������ �ʿ��� �� ���
	virtual void BeginSetting() override;

	// �̸��� �´� ��Ÿ�� ��� �Լ�
	virtual void PlayHitReactMontage() override;
	virtual void PlayFireMontage() override;
	virtual void PlayReloadMontage() override;

	// �ѱ� �ݵ� �Լ�, ����� �̿ϼ��̶� ������� ����
	virtual void WeaponRecoil(AWeapon* _Weapon);
	virtual void WeaponRecoilRecover();
	void CallWeaponRecoilRecover(float FirstDelayTime);
	void WeaponRecoilReset();

public:

};

