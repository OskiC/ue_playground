#include "CustomHUD.h"

#include "Blueprint/UserWidget.h"
#include "TestingPlayground/Widgets/PlayerWidgets/PlayerHUDWidget.h"
#include <TestingPlayground/Widgets/MenuWidgets/GameMenuWidget.h>
#include <TestingPlayground/PlayerCharacter/CustomPlayerController.h>

void ACustomHUD::RefreshWidget()
{
	if (!IsValid(PlayerHUDWidget) && PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetOwningPlayerController(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}
}

void ACustomHUD::ToggleGameMenu()
{
	ACustomPlayerController* PC = Cast<ACustomPlayerController>(GetOwningPlayerController());
	if (!IsValid(PC))
	{
		return;
	}

	if (!IsValid(GameMenuWidget) && GameMenuWidgetClass)
	{
		GameMenuWidget = CreateWidget<UGameMenuWidget>(PC, GameMenuWidgetClass);
		if (!IsValid(GameMenuWidget))
		{
			return;
		}
	}

	if (GameMenuWidget->IsInViewport())
	{
		GameMenuWidget->RemoveFromParent();

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
	}
	else
	{
		GameMenuWidget->AddToViewport();

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(GameMenuWidget->TakeWidget());

		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
	}
}