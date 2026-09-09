#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "TestingPlayground/Abilities/CustomAbilitySystemComponent.h"
#include "TestingPlayground/PlayerState/CustomPlayerState.h"
#include "CustomPlayerController.h"
#include <TestingPlayground/HUD/CustomHUD.h>
#include <TestingPlayground/Components/InventoryComponents/InventoryComponent.h>
#include <TestingPlayground/Components/InventoryComponents/EquipmentComponent.h>
#include <TestingPlayground/Abilities/Attributes/AttributeHealthSet.h>
#include <TestingPlayground/Abilities/Attributes/CoreStatsSet.h>

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);
    
	USkeletalMeshComponent* PlayerMesh = GetMesh();
	PlayerMesh->SetupAttachment(GetCapsuleComponent());
	PlayerMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -96.f), FRotator(0.f, -90.f, 0.f));
    
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bUsePawnControlRotation = true; 
	
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 50.f)); 

	SpringArm->SocketOffset = FVector(0.f, 50.f, 50.f);
    
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	CaptureComponent->ShowOnlyActorComponents(this);
	CaptureComponent->SetupAttachment(GetCapsuleComponent());

	// Custom Components
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
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
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::OnInteractPressed);

		EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &APlayerCharacter::OnToggleMenuPressed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::OnInteractPressed(const FInputActionValue& Value)
{
	ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetController());
	if (!IsValid(PC))
	{
		return;
	}

	UObject* TargetObject = PC->GetCurrentHoveredTarget();

	if (IsValid(TargetObject))
	{
		FGameplayEventData Payload;
		Payload.Instigator = this;
		Payload.OptionalObject = TargetObject;

		FGameplayTag InteractTag = FGameplayTag::RequestGameplayTag(FName("Event.Interaction"));
		AbilitySystemComponent->HandleGameplayEvent(InteractTag, &Payload);
	}
}

void APlayerCharacter::OnToggleMenuPressed(const FInputActionValue& Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetController());
	if (IsValid(PC))
	{
		ACustomHUD* HUD = Cast<ACustomHUD>(PC->GetHUD());
		if (IsValid(HUD))
		{
			HUD->ToggleGameMenu();
		}
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

		AddCharacterAbilities();
	}

	if (IsLocallyControlled())
	{
		if (ACustomPlayerController* PC = Cast<ACustomPlayerController>(NewController))
		{
			if (ACustomHUD* HUD = Cast<ACustomHUD>(PC->GetHUD()))
			{
				HUD->RefreshWidget();
			}
		}
	}
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ACustomPlayerState* PS = GetPlayerState<ACustomPlayerState>();
	if (IsValid(PS))
	{
		AbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		if (IsLocallyControlled())
		{
			if (APlayerController* PC = Cast<ACustomPlayerController>(GetController()))
			{
				if (ACustomHUD* HUD = Cast<ACustomHUD>(PC->GetHUD()))
				{
					HUD->RefreshWidget();
				}
			}
		}
	}
}

void APlayerCharacter::AddCharacterAbilities()
{
	// only server can grant abilities
	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (IsValid(AbilityClass))
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);

			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

void APlayerCharacter::CallDebugGameplayEffect()
{
	Server_CallDebugGameplayEffect();
}

void APlayerCharacter::Server_CallDebugGameplayEffect_Implementation()
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
			UE_LOG(LogTemp, Display, TEXT("DebugGameplayEffect correctly triggered ON THE SERVER"));
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void APlayerCharacter::Server_EquipItemFromInventory_Implementation(int32 InventoryIndex, int32 EquipmentIndex)
{
	TArray<FInventoryItemSlot>& InventoryArray = InventoryComponent->GetActiveInventoryRef();
	TArray<FEquipItemSlot>& EquipmentArray = EquipmentComponent->GetEquipmentSlotsRef();

	if (!InventoryArray.IsValidIndex(InventoryIndex) || !EquipmentArray.IsValidIndex(EquipmentIndex))
	{
		return;
	}

	FInventoryItemSlot& InvSlot = InventoryArray[InventoryIndex];
	FEquipItemSlot& EqSlot = EquipmentArray[EquipmentIndex];

	if (InvSlot.IsStructValid() && InvSlot.ItemInstance->ItemDef->ItemTags.HasTag(EqSlot.SlotRequirementTag))
	{
		if (EqSlot.ActiveEffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(EqSlot.ActiveEffectHandle);
			EqSlot.ActiveEffectHandle.Invalidate();
		}
		
		UItemInstance* PreviouslyEquippedItem = EqSlot.EquippedItem;

		EqSlot.EquippedItem = InvSlot.ItemInstance;

		if (IsValid(PreviouslyEquippedItem))
		{
			InvSlot.ItemInstance = PreviouslyEquippedItem;
			InvSlot.StackCount = 1;
		}
		else
		{
			InvSlot.ItemInstance = nullptr;
			InvSlot.StackCount = 0;
		}

		
		ApplyEquipmentsStats(EqSlot);

		InventoryComponent->OnInventoryUpdated.Broadcast();
		EquipmentComponent->OnEquipmentUpdated.Broadcast();
	}
}

void APlayerCharacter::Server_UnequipItemToInventory_Implementation(int32 EquipmentIndex, int32 InventoryIndex)
{
	TArray<FInventoryItemSlot>& InventoryArray = InventoryComponent->GetActiveInventoryRef();
	TArray<FEquipItemSlot>& EquipmentArray = EquipmentComponent->GetEquipmentSlotsRef();

	if (!InventoryArray.IsValidIndex(InventoryIndex) || !EquipmentArray.IsValidIndex(EquipmentIndex))
	{
		return;
	}

	FInventoryItemSlot& InvSlot = InventoryArray[InventoryIndex];
	FEquipItemSlot& EqSlot = EquipmentArray[EquipmentIndex];

	if (!IsValid(EqSlot.EquippedItem))
	{
		return;
	}

	if (InvSlot.IsStructValid())
	{
		if (!IsValid(InvSlot.ItemInstance) || !IsValid(InvSlot.ItemInstance->ItemDef))
		{
			return;
		}

		if (!InvSlot.ItemInstance->ItemDef->ItemTags.HasTag(EqSlot.SlotRequirementTag))
		{
			return;
		}
	}

	UItemInstance* EquippedItem = EqSlot.EquippedItem;

	if (EqSlot.ActiveEffectHandle.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(EqSlot.ActiveEffectHandle);

		EqSlot.ActiveEffectHandle.Invalidate();
	}

	if (!InvSlot.IsStructValid())
	{
		InvSlot.ItemInstance = EquippedItem;
		InvSlot.StackCount = 1;

		EqSlot.EquippedItem = nullptr;
	}
	else
	{
		EqSlot.EquippedItem = InvSlot.ItemInstance;

		InvSlot.ItemInstance = EquippedItem;
		InvSlot.StackCount = 1;
		
		ApplyEquipmentsStats(EqSlot);
	}

	InventoryComponent->OnInventoryUpdated.Broadcast();
	EquipmentComponent->OnEquipmentUpdated.Broadcast();
}

void APlayerCharacter::ApplyEquipmentsStats(FEquipItemSlot& EqSlot)
{
	if (!IsValid(EqSlot.EquippedItem))
	{
		return;
	}

	if (!IsValid(EqSlot.EquippedItem->ItemDef))
	{
		return;
	}

	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!EquipmentStatsEffect)
	{
		return;
	}

	static const TMap<FGameplayAttribute, FName> EquipmentAttributeToTagMap =
	{
		{ UCoreStatsSet::GetMaxStaminaAttribute(), FName("Data.Equipment.MaxStamina") },
		{ UCoreStatsSet::GetMaxManaAttribute(), FName("Data.Equipment.MaxMana") },
		{ UCoreStatsSet::GetArmorAttribute(), FName("Data.Equipment.Armor") },
		{ UCoreStatsSet::GetStrengthAttribute(), FName("Data.Equipment.Strength") },
		{ UCoreStatsSet::GetIntelligenceAttribute(), FName("Data.Equipment.Intelligence") },
		{ UCoreStatsSet::GetDexterityAttribute(), FName("Data.Equipment.Dexterity") },
		{ UCoreStatsSet::GetLuckAttribute(), FName("Data.Equipment.Luck") },
		{ UAttributeHealthSet::GetMaxHealthAttribute(), FName("Data.Equipment.MaxHealth") }
	};

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddInstigator(this, this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EquipmentStatsEffect, 1.0f, ContextHandle);
	if (!SpecHandle.IsValid()) return;

	FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
	TMap<FGameplayAttribute, float> FinalStats;

	for (const TPair<FGameplayAttribute, float>& Stat : EqSlot.EquippedItem->ItemDef->BaseStats)
	{
		FinalStats.FindOrAdd(Stat.Key) += Stat.Value;
	}
	for (const FItemStatBonus& BonusStat : EqSlot.EquippedItem->BonusStats)
	{
		FinalStats.FindOrAdd(BonusStat.Attribute) += BonusStat.Value;
	}

	for (const auto& KVP : EquipmentAttributeToTagMap)
	{
		FGameplayTag Tag = FGameplayTag::RequestGameplayTag(KVP.Value);
		Spec.SetSetByCallerMagnitude(Tag, 0.0f);
	}

	for (const TPair<FGameplayAttribute, float>& Stat : FinalStats)
	{
		if (const FName* TagName = EquipmentAttributeToTagMap.Find(Stat.Key))
		{
			FGameplayTag MatchingTag = FGameplayTag::RequestGameplayTag(*TagName);
			Spec.SetSetByCallerMagnitude(MatchingTag, Stat.Value);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to apply stat %s but no matching tag was found in the map!"), *Stat.Key.AttributeName);
		}
	}

	EqSlot.ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(Spec);
}