// Fill out your copyright notice in the Description page of Project Settings.


#include "CDCharacterState.h"
#include "CD2/GameAbilitySystems/CDAbilitySystemComponent.h"
#include "CD2/GameAbilitySystems/CDAttributeSet.h"

ACDCharacterState::ACDCharacterState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UCDAttributeSet>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* ACDCharacterState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCDAttributeSet* ACDCharacterState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ACDCharacterState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ACDCharacterState::ShowAbilityConfirmCancelText(bool ShowText)
{
	// UIÀÛ¾÷
}

float ACDCharacterState::GetCharacterLevel() const
{
	return AttributeSetBase->GetLevel();
}

float ACDCharacterState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ACDCharacterState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ACDCharacterState::GetMana() const
{
	return AttributeSetBase->GetMana();
}

float ACDCharacterState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

void ACDCharacterState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		CharacterLevelChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACDCharacterState::CharacterLevelChanged);
		HealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACDCharacterState::HealthChanged);
		MaxHealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACDCharacterState::MaxHealthChanged);
		ManaChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACDCharacterState::ManaChanged);
		MaxManaChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ACDCharacterState::MaxManaChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACDCharacterState::StunTagChanged);
	}
}

void ACDCharacterState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("Character Level Changed"));
}

void ACDCharacterState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("Health Changed"));
}

void ACDCharacterState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("Max Health Changed"));
}

void ACDCharacterState::ManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("Mana Changed"));
}

void ACDCharacterState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("Max Mana Changed"));
}

void ACDCharacterState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToChannel;
		AbilityTagsToChannel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToChannel, &AbilityTagsToIgnore);
	}
}

void UCharacterStates::PrintStateEnum(EStateEnum StateEnum)
{
	FString Log;
	Log.Empty();

	switch (StateEnum)
	{
	case EStateEnum::STATE_Attack:
		Log += FString("Attack ");
		break;
	case EStateEnum::STATE_Jump:
		Log += FString("Jump ");
		break;
	case EStateEnum::STATE_Move:
		Log += FString("Move ");
		break;
	default:
		Log += FString("None ");
		break;
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Log);
}

void UCharacterStates::StatesLog()
{
	if (CurrentStates.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Idle"));
		return;
	}

	FString Log;
	Log.Empty();

	for (const auto State : CurrentStates)
	{
		switch (State.Key)
		{
		case EStateEnum::STATE_Attack:
			Log += FString("Attack ");
			break;
		case EStateEnum::STATE_Jump:
			Log += FString("Jump ");
			break;
		case EStateEnum::STATE_Move:
			Log += FString("Move ");
			break;
		default:
			Log += FString("None ");
			break;
		}
		
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Log);
}

void UCharacterStates::DeleteState(EStateEnum StateEnum)
{
	if (!CurrentStates.Contains(StateEnum)) return;

	delete CurrentStates[StateEnum];
	CurrentStates.FindAndRemoveChecked(StateEnum);
	StatesLog();
	OnCharacterStateUpdated.Broadcast();
}

inline bool UCharacterStates::IsPossibleAction(EStateEnum StateEnum)
{
	for (const TPair<EStateEnum, UCharacterStateClasses*> State : CurrentStates)
	{
		if (!State.Value->FindAction(StateEnum)) return false;
	}

	return true;
}

bool UCharacterStates::IsPossibleJump()
{
	return IsPossibleAction(EStateEnum::STATE_Jump);
}

bool UCharacterStates::IsPossibleAttack()
{
	return IsPossibleAction(EStateEnum::STATE_Attack);
}

bool UCharacterStates::IsPossibleMove()
{
	return IsPossibleAction(EStateEnum::STATE_Move);
}

bool UCharacterStateClasses::FindAction(EStateEnum StateEnum)
{
	switch (StateEnum)
	{
	case EStateEnum::STATE_Attack:
		return Attack();
	case EStateEnum::STATE_Jump:
		return Jump();
	case EStateEnum::STATE_Move:
		return Move();
	}

	return false;
}
