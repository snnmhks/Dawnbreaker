// Copyright Epic Games, Inc. All Rights Reserved.

#include "CD2Character.h"
#include "CD2Gas/Tag/CDGameplayTag.h"
#include "Engine/LocalPlayer.h"
#include "Component/SmoothMoveCameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Character/PlayerAIController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/Animation/CharacterAnimInstance.h"
#include "CD2/Weapon/Weapon.h"
#include "Character/CDCharacterState.h"
#include "CD2/GameAbilitySystems/CDAbilitySystemComponent.h"
#include "CD2/GameAbilitySystems/CDAttributeSet.h"
#include "CD2/GameAbilitySystems/CDGameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Engine/SkeletalMeshSocket.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACD2Character

ACD2Character::ACD2Character(const class FObjectInitializer& ObjectInitializer){
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	LeftFootSlideEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LeftFootSlideEffect"));
	LeftFootSlideEffect->SetupAttachment(RootComponent);
	RightFootSlideEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RightFootSlideEffect"));
	RightFootSlideEffect->SetupAttachment(RootComponent);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	AimBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimBoom"));
	AimBoom->SetupAttachment(RootComponent);
	AimBoom->TargetArmLength = 50.0f; // The camera follows at this distance behind the character	
	AimBoom->bUsePawnControlRotation = true; // Rotate the arm based on the 
	AimBoom->SetRelativeLocation(FVector(-42.0f, 37.0f, 78.0f));
	CrouchAimBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CrouchAimBoom"));
	CrouchAimBoom->SetupAttachment(RootComponent);
	CrouchAimBoom->TargetArmLength = 50.0f; // The camera follows at this distance behind the character	
	CrouchAimBoom->bUsePawnControlRotation = true; // Rotate the arm based on the 
	CrouchAimBoom->SetRelativeLocation(FVector(-42.0f, 37.0f, 15.0f));

	BasicCameraPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasicCameraPosition"));
	BasicCameraPosition->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	AimCameraPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimCameraPosition"));
	AimCameraPosition->SetupAttachment(AimBoom, USpringArmComponent::SocketName);
	CrouchAimCameraPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrouchAimCameraPosition"));
	CrouchAimCameraPosition->SetupAttachment(CrouchAimBoom, USpringArmComponent::SocketName);

	// Create a follow camera
	SmoothMoveCamera = CreateDefaultSubobject<USmoothMoveCameraComponent>(TEXT("SmoothMoveCamera"));
	SmoothMoveCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	SmoothMoveCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

	AIControllerClass = APlayerAIController::StaticClass();
}

bool ACD2Character::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 ACD2Character::GetAbilityLevel(CDAbilityID AbilityID) const
{
	return 1;
}

void ACD2Character::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (FGameplayAbilitySpecHandle i : AbilitiesToRemove)
	{
		AbilitySystemComponent->ClearAbility(i);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

void ACD2Character::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->Velocity = FVector(0.0f);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsEremoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage) 
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void ACD2Character::FinishDying()
{
	Destroy();
}

float ACD2Character::GetCharacterLevel() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetLevel();
	}
	return 0.0f;
}

float ACD2Character::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float ACD2Character::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float ACD2Character::GetMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMana();
	}
	return 0.0f;
}

float ACD2Character::GetMaxMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxMana();
	}
	return 0.0f;
}

void ACD2Character::BeginSetting()
{
	SmoothMoveCamera->OnCameraMoveEnd.BindUObject(this, &ACD2Character::AttatchCamera);
	BaseMeshRotator = GetMesh()->GetRelativeRotation();
	LeftFootSlideEffect->SetActive(false);
	RightFootSlideEffect->SetActive(false);
}

void ACD2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	WAimWidget = CreateWidget(GetWorld(), WAimWidgetClass);
}

void ACD2Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ACDCharacterState* PS = GetPlayerState<ACDCharacterState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<UCDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		AttributeSetBase = PS->GetAttributeSetBase();

		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		AttributeSetBase->InitMaxHealth(100.0f);
		AttributeSetBase->InitMaxMana(100.0f);
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());

		AddStartupEffects();

		AddCharacterAbilities();
	}
	SetOwner(NewController);
}

void ACD2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSliding)
	{
		Sliding(DeltaTime);
	}

}

void ACD2Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ACDCharacterState* PS = GetPlayerState<ACDCharacterState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<UCDAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		AttributeSetBase = PS->GetAttributeSetBase();

		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		AttributeSetBase->InitMaxHealth(100.0f);
		AttributeSetBase->InitMaxMana(100.0f);
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());

		InitializeAttributes();
	}
}

UAbilitySystemComponent* ACD2Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ACD2Character::PlayHitReactMontage()
{
	MyAnim->PlayMontage("HitReact", 1.0f);
}

void ACD2Character::PlayFireMontage()
{
	MyAnim->PlayMontage("Fire", 1.0f);
}

void ACD2Character::PlayReloadMontage()
{
	MyAnim->PlayMontage("Reload", 1.0f);
}

void ACD2Character::WeaponRecoil(AWeapon* _Weapon)
{
	WeaponRecoilX++;
	WeaponRecoilVerticalValue = FMath::Clamp(FMath::Exp(_Weapon->RecoilScale * 1.0f * (WeaponRecoilX - _Weapon->RecoilStartBulletNum)) - 1, 0.0f, 1.0f);
	WeaponRecoilHorizontalValue = FMath::Clamp(FMath::Exp(_Weapon->RecoilScale * 1.0f * (WeaponRecoilX - _Weapon->RecoilStartBulletNum)) - 1, 0.0f, 0.1f);
	if (FMath::RandBool()) WeaponRecoilHorizontalValue = -WeaponRecoilHorizontalValue;

	AddControllerPitchInput(-WeaponRecoilVerticalValue);
	AddControllerYawInput(-WeaponRecoilHorizontalValue);

	CallRecoilRecoverEventNum = 0;
	RecoverValue = WeaponRecoilVerticalValue / 10;
	CallWeaponRecoilRecover(0.0f);
}

void ACD2Character::WeaponRecoilRecover()
{
	CallRecoilRecoverEventNum++;

	AddControllerPitchInput(RecoverValue);
	
	if (CallRecoilRecoverEventNum > 10)
	{
		CallRecoilRecoverEventNum = 0;
		GetWorldTimerManager().ClearTimer(WeaponRecoilRecoverHanlde);
		WeaponRecoilReset();
	}
}

void ACD2Character::WeaponRecoilReset()
{
	WeaponRecoilX = 0;
}

void ACD2Character::CallWeaponRecoilRecover(float FirstDelayTime)
{
	GetWorldTimerManager().SetTimer(WeaponRecoilRecoverHanlde, this, &ACD2Character::WeaponRecoilRecover, 0.01, true, FirstDelayTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACD2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//SetupGASInputComponent();
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACD2Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACD2Character::Look);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ACD2Character::WeaponFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ACD2Character::WeaponFireEnd);

		// Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ACD2Character::AimPress);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ACD2Character::AimRelease);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ACD2Character::WeaponReload);

		// Weapon Change
		EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Started, this, &ACD2Character::WeaponChange);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACD2Character::CrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACD2Character::CrouchEnd);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACD2Character::SetupGASInputComponent()
{
	if (bACSInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("CDAbilityID"), static_cast<int32>(CDAbilityID::Confirm), static_cast<int32>(CDAbilityID::Cancel)));

		bACSInputBound = true;
	}
}

void ACD2Character::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	int32 InputID = 0;
	for (TSubclassOf<UGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartupAbility);
		StartSpec.InputID = InputID++;
		AbilitySystemComponent->GiveAbility(StartSpec);
		//SetupGASInputComponent();
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void ACD2Character::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Log, TEXT("%s() Missing DefaultAttrubutes for &s. Please fill in the character`s Blueprint."), *FString("__FUNCTION__"), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ACD2Character::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartUpEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void ACD2Character::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void ACD2Character::SetMana(float Mana)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetMana(Mana);
	}
}

void ACD2Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && IsAlive2() && UCS->IsPossibleMove())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (bIsSliding) AddMovementInput(RightDirection, 0.1 * MovementVector.X);

		else
		{
			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ACD2Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr && IsAlive2())
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		if (IsAiming)
		{
			SetPitchCollection();
		}
	}
}

void ACD2Character::Jump()
{
	if (UCS->IsPossibleJump()) Super::Jump();

}

void ACD2Character::AimPress() {
	if (Controller == nullptr || GetHaveWeaponName() == EName::None || !IsAlive2()) return;
	FGameplayTagContainer TargetTag(CDTAG_Character_Test);

	if (!AbilitySystemComponent->HasMatchingGameplayTag(CDTAG_Character_Tests))
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		AbilitySystemComponent->CancelAbilities(&TargetTag);
	}

	SetPitchCollection();
	bUseControllerRotationYaw = true;
	IsAiming = true;
	bIsAimPressing = true;

	CameraSmoothMove();

	WAimWidget->AddToViewport();
}

void ACD2Character::AimRelease() {
	if (Controller == nullptr || !IsAlive2()) return;

	AimEnd();
	bIsAimPressing = false;
}

void ACD2Character::AimEnd()
{
	CollectionPitchValue = 0;
	bUseControllerRotationYaw = false;
	IsAiming = false;

	CameraSmoothMove();
	WAimWidget->RemoveFromParent();
}

void ACD2Character::WeaponChange(const FInputActionValue& Value)
{
	if (Controller == nullptr || !IsAlive2()) return;

	float WeaponChangeValue = Value.Get<float>();
	if (WeaponChangeValue > 0) WeaponChangeValue = 1.0f;
	else WeaponChangeValue = -1.0f;

	WeaponChangeBase(WeaponChangeValue);

	if (GetHaveWeaponName() == "None")
	{
		AimEnd();
	}

	SetPitchCollection();

	UE_LOG(LogTemp, Log, TEXT("%d"), WeaponIndex);
}

void ACD2Character::CrouchStart()
{
	if (Controller == nullptr || !IsAlive2()) return;

	if (IsPossibleSide()) 
	{
		GetCharacterMovement()->SetWalkableFloorAngle(90.0f);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		LeftFootSlideEffect->SetActive(true);
		RightFootSlideEffect->SetActive(true);
		bIsSliding = true;
	}
	else
	{
		bIsCrouching = true;
		GetCharacterMovement()->MaxWalkSpeed = 300;
	}

	if (IsAiming) CameraSmoothMove();
}

void ACD2Character::CrouchEnd()
{
	if (Controller == nullptr || !IsAlive2()) return;

	if (bIsCrouching)
	{
		bIsCrouching = false;
		GetCharacterMovement()->MaxWalkSpeed = 500;
	}

	if (bIsSliding)	SlideEnd();

	if (IsAiming) CameraSmoothMove();
}

bool ACD2Character::IsPossibleSide()
{
	const FVector FloorNormal = GetCharacterMovement()->CurrentFloor.HitResult.Normal;
	const float FloorSlopeAngle = FMath::RadiansToDegrees(acosf(FloorNormal.Z));
	const FVector CharacterVelocity = GetVelocity().GetSafeNormal();

	return (FloorSlopeAngle > 10 && FVector::DotProduct(FloorNormal, CharacterVelocity) > 0);
}

void ACD2Character::Sliding(float DeltaTime)
{
	const FVector FloorNormal = GetCharacterMovement()->CurrentFloor.HitResult.Normal;
	const float FloorSlopeAngle = FMath::RadiansToDegrees(acosf(FloorNormal.Z));

	// 중력 벡터
	FVector GravityVector = FVector(0.0f, 0.0f, -1.0f);

	// 미끄러지는 방향 벡터 계산
	FVector SlideDirection = GravityVector - (FloorNormal * (GravityVector | FloorNormal));
	SlideDirection.Normalize();

	// 경사면 각도만큼 메쉬 회전
	FVector CharaterLocation = GetActorLocation();
	FVector SlideLocation = CharaterLocation + SlideDirection;
	FRotator NewRotator = UKismetMathLibrary::FindLookAtRotation(CharaterLocation, SlideLocation);
	NewRotator.Yaw -= 90;
	NewRotator.Roll += 50;
	GetMesh()->SetWorldRotation(NewRotator);

	// 속도 방향으로 메쉬 회전
	/*FVector Velocity = GetVelocity();
	Velocity.Normalize();
	FRotator VelocityRotator = UKismetMathLibrary::FindLookAtRotation(CharaterLocation, Velocity);
	FRotator NewRotator2 = GetActorRotation();
	NewRotator2.Yaw = VelocityRotator.Yaw + 90;
	SetActorRotation(NewRotator2);*/

	FVector SlideVelocity = SlideDirection * (980 * FMath::Sin(acosf(FloorNormal.Z)) - FMath::Cos(acosf(FloorNormal.Z))) * DeltaTime;

	GetCharacterMovement()->Velocity += SlideVelocity;
	GetCharacterMovement()->GroundFriction = 0;
	GetCharacterMovement()->BrakingDecelerationWalking = 0;

	if (GetVelocity().Length() < 100 || GetCharacterMovement()->IsFalling() || FloorSlopeAngle < 5) SlideEnd();
}

void ACD2Character::SlideEnd()
{
	GetMesh()->SetRelativeRotation(BaseMeshRotator);
	GetCharacterMovement()->SetWalkableFloorAngle(45.0f);
	GetCharacterMovement()->GroundFriction = 2;
	GetCharacterMovement()->BrakingDecelerationWalking = 2024;
	LeftFootSlideEffect->SetActive(false);
	RightFootSlideEffect->SetActive(false);

	bIsSliding = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACD2Character::SetPitchCollection()
{
	// 카메라 위치가 얼마만큼 떨어져있는지 확인
	float BaseAngle = GetBaseAimRotation().Pitch;
	FVector AimBoomLocation = AimBoom->GetRelativeLocation();
	
	// 카메라 위치 기준으로 적당히 긴 값을 설정해 직각 삼각형을 만듦
	float PitchCollectionConstant = GetHandWeapon()->PitchCollection;
	float BottomLength = PitchCollectionConstant * FMath::Cos(FMath::DegreesToRadians(BaseAngle));
	float HeightLength = PitchCollectionConstant * FMath::Sin(FMath::DegreesToRadians(BaseAngle));

	// 어께 위치를 기준으로 얼마만큼 Aim이 돌아가야하는지 계산
	CollectionPitchValue = FMath::RadiansToDegrees(FMath::Atan((HeightLength + AimBoomLocation.Z - 46.0f) / (BottomLength + AimBoomLocation.X - AimBoom->TargetArmLength)));
}

void ACD2Character::WeaponFireEnd()
{
	if (Controller == nullptr || !IsAlive2() || !IsAiming) return;
	WeaponRecoilReset();
}

void ACD2Character::WeaponFire() {
	if (Controller == nullptr || !IsAlive2() || !IsAiming || GetHandWeapon()->IsFire() || !UCS->IsPossibleAttack()) return;

	FVector Direction = SmoothMoveCamera->GetComponentLocation() + SmoothMoveCamera->GetForwardVector() * 10000;
	struct FHitResult HitResult;
	FCollisionQueryParams Params(EName::None, false, this);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		SmoothMoveCamera->GetComponentLocation(),
		Direction,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params);
	if (IsHit)
	{
		Direction = HitResult.Location;
	}

	WeaponFireBase(Direction);
	if (GetHandWeapon()->RemainBullet >= 1) {
		WeaponRecoil(GetHandWeapon());
	}
}

void ACD2Character::WeaponReload()
{
	if (Controller == nullptr || !IsAlive2() || GetHaveWeaponName() == "None" || GetHandWeapon()->IsReload()) return;

	WeaponReloadBase();
	//AimEnd();
	WeaponRecoilReset();
}
