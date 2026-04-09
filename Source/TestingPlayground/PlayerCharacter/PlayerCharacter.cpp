#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TestingPlayground/Abilities/CustomAbilitySystemComponent.h"
#include "TestingPlayground/PlayerState/CustomPlayerState.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	CameraComponent->bUsePawnControlRotation = true;
	
	SkeletalMeshComponent->SetOnlyOwnerSee(true);
	SkeletalMeshComponent->SetupAttachment(CameraComponent);
	SkeletalMeshComponent->bCastDynamicShadow = true;
	SkeletalMeshComponent->CastShadow = true;
	SkeletalMeshComponent->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		EnhancedInputComponent->BindAction(DebugGameplayEffectAction, ETriggerEvent::Started, this, &APlayerCharacter::CallDebugGameplayEffect);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Look(const struct FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (IsValid(Controller))
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (IsValid(Controller))
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

FVector APlayerCharacter::GetInteractionStart() const {
	return CameraComponent ? CameraComponent->GetComponentLocation() : FVector::ZeroVector;
}

FVector APlayerCharacter::GetInteractionDirection() const {
	return CameraComponent ? CameraComponent->GetForwardVector() : FVector::ForwardVector;
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return Cast<ACustomPlayerState>(GetPlayerState())->GetAbilitySystemComponent();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>();
	if (IsValid(PS))
	{
		AbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

void APlayerCharacter::CallDebugGameplayEffect()
{
	if (!IsValid(DebugGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("Gameplay effect is not set up"));
		return;
	}
	
	if (IsValid(AbilitySystemComponent))
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddInstigator(this, this);
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DebugGameplayEffect, DebugGameplayEffectLevel, ContextHandle);
		
		if (SpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Display, TEXT("DebugGameplayEffect correctly triggered"));
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}