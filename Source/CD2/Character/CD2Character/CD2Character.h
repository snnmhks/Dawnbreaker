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

	// 초기에 캐릭터 메쉬의 상대적인 회전 값
	// 슬라이딩을 하는 등의 메쉬가 회전해야할 일이 생기면 회전이 끝난후 원상복귀를 시켜주기 위한 변수
	UPROPERTY()
	FRotator BaseMeshRotator;

	// Zoom이 자동으로 꺼지는 모션 후에 줌을 유지중이라면 다시 줌을 하게할 변수
	UPROPERTY()
	bool bIsAimPressing = false;

	// 수직 반동 값
	UPROPERTY()
	float WeaponRecoilVerticalValue;
	// 수평 반동 값
	UPROPERTY()
	float WeaponRecoilHorizontalValue;
	// 반동 회복 함수를 몇번 호출했는가
	UPROPERTY()
	int32 CallRecoilRecoverEventNum;
	// 반동 회복 값
	UPROPERTY()
	float RecoverValue;

	// Commulative Weapon Recoil Vertical Value
	float CommulativeWRVV;
	// Commulative Weapon Recoil Horizontal Value
	float CommulativeWRHV;
	// 연속으로 총을 몇번 쐈는가 -> 반동 계산에 들어감
	int32 WeaponRecoilX = 0;
	// 반동 회복 함수를 호출할 타이머
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

	// 화면 중앙에 표시될 에임의 위젯
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

	// 총 발사
	UFUNCTION()
	void WeaponFire();
	// 재장전
	UFUNCTION()
	void WeaponReload();
	// 총기 반동이 있을때 반동을 복구 시켜주는 함수
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

	// 무기 교체 함수
	void WeaponChange(const FInputActionValue& Value);

	// 쭈구려 앉는 함수
	void CrouchStart();
	void CrouchEnd();

	// 슬라이딩 함수
	bool IsPossibleSide();
	void Sliding(float DeltaTime);
	void SlideEnd();

	// 작도를 하여 AimOffset값 결정
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

	// BeginPlay때 실행될 함수, 기본적인 셋팅 외에 이 캐릭터만의 세팅이 필요할 때 사용
	virtual void BeginSetting() override;

	// 이름에 맞는 몽타주 재생 함수
	virtual void PlayHitReactMontage() override;
	virtual void PlayFireMontage() override;
	virtual void PlayReloadMontage() override;

	// 총기 반동 함수, 현재는 미완성이라 사용하지 않음
	virtual void WeaponRecoil(AWeapon* _Weapon);
	virtual void WeaponRecoilRecover();
	void CallWeaponRecoilRecover(float FirstDelayTime);
	void WeaponRecoilReset();

public:

};

