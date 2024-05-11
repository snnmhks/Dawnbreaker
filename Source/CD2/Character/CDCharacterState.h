// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "CDCharacterState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnCharacterStateUpdated);

class UCDAttributeSet;
class UCDAbilitySystemComponent;
class UCharacterStateClasses;

UCLASS()
class CD2_API ACDCharacterState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACDCharacterState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCDAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "Player State")
	float GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Player State")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Player State")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Player State")
	float GetMana() const;
	UFUNCTION(BlueprintCallable, Category = "Player State")
	float GetMaxMana() const;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UCDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	UCDAttributeSet* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle CharacterLevelChangeDelegateHandle;
	FDelegateHandle HealthChangeDelegateHandle;
	FDelegateHandle MaxHealthChangeDelegateHandle;
	FDelegateHandle ManaChangeDelegateHandle;
	FDelegateHandle MaxManaChangeDelegateHandle;

	virtual void BeginPlay() override;

	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};

UENUM()
enum class EStateEnum
{
	STATE_None = 0,
	STATE_Jump,
	STATE_Attack,
	STATE_Move
};

class UCharacterStateClasses
{
public:
	virtual bool Jump() { return true; };
	virtual bool Attack() { return true; };
	virtual bool Move() { return true; };

	bool FindAction(EStateEnum StateEnum);
};

class UCharacterJumpStateClass : public UCharacterStateClasses
{
public:
	virtual bool Attack() override { return false; };
};

class UCharacterAttackStateClass : public UCharacterStateClasses
{
public:
	virtual bool Jump() override { return false; };
};

class UCharacterMoveStateClass : public UCharacterStateClasses
{
public:

};

UCLASS()
class CD2_API UCharacterStates : public UObject
{
	GENERATED_BODY()

private:
	UFUNCTION()
	bool IsPossibleAction(EStateEnum StateEnum);

public:
	TMap<EStateEnum, class UCharacterStateClasses*> CurrentStates;

	FOnCharacterStateUpdated OnCharacterStateUpdated;

	template<class T>
	void AddState(EStateEnum StateEnum)
	{
		if (CurrentStates.Contains(StateEnum)) return;

		T* NewState = new T();
		CurrentStates.Add(StateEnum, NewState);
		StatesLog();
		OnCharacterStateUpdated.Broadcast();
	}

	UFUNCTION()
	void StatesLog();
	UFUNCTION()
	void PrintStateEnum(EStateEnum StateEnum);

	UFUNCTION()
	void DeleteState(EStateEnum StateEnum);

	UFUNCTION()
	bool IsPossibleJump();

	UFUNCTION()
	bool IsPossibleAttack();

	UFUNCTION()
	bool IsPossibleMove();
};
