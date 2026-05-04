#include "CustomPlayerController.h"

#include "PlayerCharacter.h"
#include "TestingPlayground/Components/InteractionComponents/InteractionComponentBase.h"

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
	APlayerCharacter* CustomCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!IsValid(CustomCharacter))
	{
		return;
	}
	
	FVector Start = CustomCharacter->GetInteractionStart();
	FVector Direction = CustomCharacter->GetInteractionDirection();
	FVector End = Start + Direction * InteractLineTraceLength;
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, 0.f, 0, 2.f);
	
	UInteractionComponentBase* CurrentInteractComp = nullptr;
	if (AActor* HitActor = HitResult.GetActor())
	{
		CurrentInteractComp = HitActor->FindComponentByClass<UInteractionComponentBase>();
	}
	
	if (LastHoveredComponent == CurrentInteractComp)
	{
		return;
	}
	
	LastHoveredComponent = CurrentInteractComp;

	if (IsValid(LastHoveredComponent) && !LastHoveredComponent->TooltipText.IsEmpty())
	{
		OnObjectHovered.Broadcast(LastHoveredComponent->TooltipText);
	}
	else
	{
		OnObjectHovered.Broadcast(FText::GetEmpty());
	}
}