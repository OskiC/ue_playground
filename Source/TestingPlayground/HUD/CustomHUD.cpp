#include "CustomHUD.h"

#include "Blueprint/UserWidget.h"
#include "TestingPlayground/Widgets/PlayerWidgets/PlayerHUDWidget.h"

void ACustomHUD::BeginPlay()
{
	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
			PlayerHUDWidget->HideTooltip();
		}
	}
}
