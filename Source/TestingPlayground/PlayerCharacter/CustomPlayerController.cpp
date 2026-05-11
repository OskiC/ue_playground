#include "CustomPlayerController.h"

#include "PlayerCharacter.h"
#include "TestingPlayground/Interfaces/InteractableInterface.h"

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
}

void ACustomPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SendTrace();
}

void ACustomPlayerController::SendTrace()
{
	if (!IsLocalController())
	{
		return;
	}

	APlayerCharacter* CustomCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!IsValid(CustomCharacter))
	{
		return;
	}
	
	FVector Start = CustomCharacter->GetActorLocation() + FVector(0.f, 0.f, 60.f); 
	FVector Direction = CustomCharacter->GetControlRotation().Vector();
	FVector End = Start + (Direction * InteractLineTraceLength);
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CustomCharacter);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
    
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, 0.f, 0, 2.f);
	
	UObject* NewTargetObject = nullptr;

	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitResult.GetComponent() && HitResult.GetComponent()->Implements<UInteractableInterface>())
		{
			NewTargetObject = HitResult.GetComponent();
		}
		else if (HitActor->Implements<UInteractableInterface>())
		{
			NewTargetObject = HitActor;
		}
		else
		{
			NewTargetObject = HitActor->FindComponentByInterface(UInteractableInterface::StaticClass());
		}
	}

	if (CurrentHoveredTarget.GetObject() != NewTargetObject)
	{
		if (CurrentHoveredTarget) 
		{
			CurrentHoveredTarget->OnHoverEnd();
		}
        
		CurrentHoveredTarget = NewTargetObject;

		if (CurrentHoveredTarget)
		{
			CurrentHoveredTarget->OnHoverBegin();
			OnObjectHovered.Broadcast(CurrentHoveredTarget->GetInteractionTooltip());
		}
		else
		{
			OnObjectHovered.Broadcast(FText::GetEmpty());
		}
	}
}

void ACustomPlayerController::Interact()
{
	if (CurrentHoveredTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction started from controller"));
		CurrentHoveredTarget->OnInteract(GetPawn());
	}
}

UObject* ACustomPlayerController::GetCurrentHoveredTarget() const
{
	if (CurrentHoveredTarget.GetInterface())
	{
		return CurrentHoveredTarget.GetObject();
	}
	
	return nullptr;
}