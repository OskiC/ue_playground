#include "CustomPlayerController.h"

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